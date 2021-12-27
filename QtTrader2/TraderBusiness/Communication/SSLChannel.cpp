#include "SSLChannel.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QMutexLocker>
#include <QUuid>
#include <QDomElement>
#include <qmetaobject.h>
#include "Common/LogHelper.h"
#include "Common/Constants/MethodNameConstants.h"
#include "Common/AppSetting.h"
#include "QsLog.h"

namespace Communication
{
	SslChannel::SslChannel(bool useSsl, bool isWriteLogOnTraderServerSsl, bool useTraderQuotationServer, bool isProvisionalConnect) :
		_useSsl(useSsl), _isWriteLogOnTraderServerSsl(isWriteLogOnTraderServerSsl), _useTraderQuotationServer(useTraderQuotationServer)//: _mutex(QMutex::Recursive)
	{
		qRegisterMetaType<QSharedPointer<QByteArray>>("QSharedPointer<QByteArray>");
		qRegisterMetaType<QSharedPointer<AsyncResult>>("QSharedPointer<AsyncResult>");
		this->recoverFailedCount = 0;
        this->reConnectionFailedCount = 0;
		this->serverName = "";
		this->needRecover = false;
		this->_isCloseWorkThread = false;
		this->_isProvisionalConnect = isProvisionalConnect;
		if (this->_isProvisionalConnect)
			this->_lastActiveDateTime = QDateTime::currentDateTime();
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << "SslChannel::SslChannel() " << QThread::currentThreadId();
        qDebug() << "SslChannel::SslChannel() ThreadId:" << QThread::currentThreadId();
		qRegisterMetaType<Common::Enum::ConnectionStatus::ConnectionStatus>("ConnectionStatus");

		this->heartbeater = QSharedPointer<Heartbeater>(new Heartbeater(this, !isProvisionalConnect));

		QObject::connect(this->heartbeater.data(), SIGNAL(statusChanged(Common::Enum::BeatStatus::BeatStatus)), 
			this, SLOT(processHeartbeaterStatusChanged(Common::Enum::BeatStatus::BeatStatus)));

        QObject::connect(&this->_asyncTimeoutCheckTimer, SIGNAL(timeout()),  this, SLOT(checkTimeout()));
        QObject::connect(this, SIGNAL(startAsyncTimeoutCheckTimer()),  &this->_asyncTimeoutCheckTimer, SLOT(start()));
        QObject::connect(this, SIGNAL(stopAsyncTimeoutCheckTimer()),  &this->_asyncTimeoutCheckTimer, SLOT(stop()));


		this->moveToThread(&this->workThread);
		this->_asyncTimeoutCheckTimer.moveToThread(&this->workThread);
		this->workThread.start();

		int asyncTimeoutCheckTimerInterval = Common::AppSetting::getInstance()->getAsyncTimeoutCheckTimerInterval();
		this->_asyncTimeoutCheckTimer.setInterval(asyncTimeoutCheckTimerInterval*1000);
        emit startAsyncTimeoutCheckTimer();

        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << "SslChannel.workThread:" << this->workThread.currentThreadId();
	}

	SslChannel::~SslChannel()
	{
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel::~SslChannel() ";
        qDebug() << "SslChannel::~SslChannel() ";
        if (this->workThread.isRunning())
        {
            qDebug() <<  "SslChannel::~SslChannel() workThread terminate!";
            this->workThread.terminate();
        }
	}

	void SslChannel::close()
	{
        qDebug() << "SslChannel::close start";
		this->stop();

		if(this->workThread.isRunning())
		{
            qDebug() <<  "SslChannel::close() workThread quit!";
            this->workThread.quit();
			bool isok = this->workThread.wait(3000);
            QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
                    <<  "SslChannel::close() workThread quit isOk:" << isok;
            qDebug()  <<  "SslChannel::close() workThread quit isOk:" << isok;
			if(!isok)
            {
				this->workThread.terminate();
				QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
					<< "SslChannel::close() workThread quit 1111:";
            }
			this->_isCloseWorkThread = true;
			QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
				<< "SslChannel::close() workThread quit 2222";
		}
	}


	void SslChannel::setServer(QString serverName, quint16 serverPort)
	{
        bool invokeMethodResult = QMetaObject::invokeMethod(this, "innerSetServer", Qt::QueuedConnection,
            Q_ARG(QString, serverName), Q_ARG(quint16, serverPort));
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
                << "SslChannel::setServer end invokeMethodResult:" << invokeMethodResult;
	}
	
	QSharedPointer<AsyncResult> SslChannel::asyncInvoke(QString methodName, QVector<Common::Parameter> parameters, int timeout, bool isReadByteArray)
	{
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
                << "SslChannel::asyncInvoke start methodName:" << methodName;
        //qDebug() << "SslChannel::asyncInvoke start methodName:" << methodName;
		if (this->_isProvisionalConnect)
			this->_lastActiveDateTime = QDateTime::currentDateTime();
		QString requestId = QUuid::createUuid().toString();
		QSharedPointer<AsyncResult> asyncResult = QSharedPointer<AsyncResult>(new AsyncResult(requestId, timeout, isReadByteArray));
		asyncResult->moveToThread(&this->workThread);
		QSharedPointer<QByteArray> request = Package::buildRequest(this->sessionId, requestId, methodName, parameters);
        bool invokeMethodResult = QMetaObject::invokeMethod(this, "innerAsyncInvoke", Qt::QueuedConnection,
			Q_ARG(QSharedPointer<QByteArray>, request), Q_ARG(QSharedPointer<AsyncResult>, asyncResult));
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
                << "SslChannel::asyncInvoke end invokeMethodResult:" << invokeMethodResult;
        //qDebug() << "SslChannel::asyncInvoke end invokeMethodResult:" << invokeMethodResult;
		return asyncResult;
	}

	void SslChannel::setWatchwords(QStringList watchwords)
	{
		this->_watchwords.clear();
		for (int i = 0; i < watchwords.length(); i++)
		{
			if (!this->_usedWatchwords.contains(watchwords[i]))
				this->_watchwords.append(watchwords[i]);
		}
		this->_usedWatchwords.clear();
	}

	QString SslChannel::getWatchword()
	{
		if (this->_watchwords.count() > 0)
		{
			QString watchword = this->_watchwords.at(0);
			this->_usedWatchwords.append(watchword);
			this->_watchwords.removeOne(watchword);
			if (this->_watchwords.count() == Common::AppSetting::getInstance()->getWatchWorldsLessthanCount())
			{
				emit getWatchwordsSignal();
			}			
			return watchword;
		}
		else
			return "";
	}
	
	bool SslChannel::isTimeOutFromProvisional()
	{
		QDateTime date = this->_lastActiveDateTime.addSecs(60 * Common::AppSetting::getInstance()->getTimeOutFromProvisional());// 5 minute
		return QDateTime::currentDateTime() > date;
	}

	void SslChannel::asyncInvoke(QString methodName, QVector<Common::Parameter> parameters, QSharedPointer<AsyncResult> asyncResult)
	{
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
                << "SslChannel::asyncInvoke2 start methodName:" << methodName;
        QSharedPointer<QByteArray> request = Package::buildRequest(this->sessionId, asyncResult->_invokeId, methodName, parameters);
		QMetaObject::invokeMethod(this, "innerSendData", Qt::QueuedConnection,Q_ARG(QSharedPointer<QByteArray>, request));
	}

	QSharedPointer<AsyncResult> SslChannel::getInvokeAsyncResult(QString methodName, int timeout, bool isReadByteArray)
	{
		if (this->_isProvisionalConnect)
			this->_lastActiveDateTime = QDateTime::currentDateTime();
		QString requestId = QUuid::createUuid().toString();
		QSharedPointer<AsyncResult> asyncResult = QSharedPointer<AsyncResult>(new AsyncResult(requestId, timeout, isReadByteArray));
		asyncResult->moveToThread(&this->workThread);
		QMetaObject::invokeMethod(this, "innerAddAsyncResult", Qt::QueuedConnection, Q_ARG(QSharedPointer<AsyncResult>, asyncResult));
		return asyncResult;
	}

	////-------------------private slots----------------////
	void SslChannel::innerSetServer(QString serverName, quint16 serverPort)
	{
        qDebug() << "SslChannel::innerSetServer start ThreadId:" << QThread::currentThreadId()
                 << " workThread:" << this->workThread.currentThreadId()
                 << " serverName:" << serverName
                 << " serverPort:" << serverPort;
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  QString("SslChannel::setServer() serverName:%1 serverPort:%2").arg(serverName, QString::number(serverPort));

		bool isStop = this->serverName != "";
		bool initailized = this->serverName.isEmpty() || this->serverName.isNull();
		bool serverChanged = this->serverName != serverName || this->serverPort != serverPort;

		this->recoverFailedCount = 0;
        this->reConnectionFailedCount = 0;
		this->needRecover = false;

		if (serverChanged || initailized)
		{
            qDebug() << "SslChannel::innerSetServer serverChanged || initailized";
			this->_queuedSendData.clear();
			this->_asyncResults.clear();
			//this->_asyncTimeoutCheckTimer.stop();
			if (isStop) this->stop();
			this->serverName = serverName;
			this->serverPort = serverPort;
			this->connect();
		}
        else
        {
            bool isConnected = this->isConnected();
            if(!isConnected)
            {
                qDebug() << "Connected is false for reconnect";
                this->_queuedSendData.clear();
                this->_asyncResults.clear();
                //this->_asyncTimeoutCheckTimer.stop();
                this->stop();
                this->connect();
            }
            else
                qDebug() << "Server is Connected";
        }
	}

    bool SslChannel::isChangeServer(QString serverName, quint16 serverPort)
    {
         bool serverChanged = this->serverName != serverName || this->serverPort != serverPort;
         return serverChanged;
    }

	void SslChannel::innerAsyncInvoke(QSharedPointer<QByteArray> request, QSharedPointer<AsyncResult> asyncResult)
    {
        this->addAsyncResult(asyncResult);
		this->sendData(request);
	}

	void SslChannel::innerAddAsyncResult(QSharedPointer<AsyncResult> asyncResult)
	{
		this->addAsyncResult(asyncResult);
	}

	void SslChannel::innerSendData(QSharedPointer<QByteArray> request)
	{
		this->sendData(request);
	}

	///////////////////////////////////Private method region//////////////////////////////////////////////
	void SslChannel::sendData(QSharedPointer<QByteArray>& data)
	{
		if(this->isConnected())
		{
			QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel::sendData";
            //qDebug()  <<  "SslChannel::sendData";
            this->_socketWrapper->send(data);
		}
		else
		{
			QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel::sendData queuedSendData";
            //qDebug()  <<  "SslChannel::sendData queuedSendData";
            this->_queuedSendData.append(data);
		}
	}

	void SslChannel::addAsyncResult(QSharedPointer<AsyncResult>& asyncResult)
	{	
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel::addAsyncResult" << asyncResult->_invokeId;
		this->_asyncResults.insert(asyncResult->_invokeId, asyncResult);
	}

	void SslChannel::processSocketStatusChanged(Common::Enum::ConnectionStatus::ConnectionStatus status)
	{
		if(!this->needRecover)
		{			
			emit this->statusChanged(status);
		}		
		if(status == Common::Enum::ConnectionStatus::Connected)
		{			
            QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
                    <<  "SslChannel::processSocketStatusChanged: Connected successful! queuedSendCount:" << this->_queuedSendData.count() << " needRecover:" << this->needRecover;

			if(this->needRecover)
			{
                //Note: Recover must be done after the communication is established successfully. Otherwise, in some cases, there will be problems if queuedSendData accumulates several recover actions.
                qDebug()<< "SslChannel::processSocketStatusChanged startRecover";
				this->startRecover();
			}
			else
			{
				if(this->_queuedSendData.size() > 0)
				{
					while (!this->_queuedSendData.isEmpty())
					{
						auto request = this->_queuedSendData.dequeue();
						this->_socketWrapper->send(request);
					}
				}
			}
		}
		else if(status == Common::Enum::ConnectionStatus::ConnectFailed || status == Common::Enum::ConnectionStatus::ConnectionBroken)
		{
            QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
                    <<  "SslChannel::processSocketStatusChanged connect failed, try to recorver again! needRecover:" << this->needRecover;
			if (this->_isProvisionalConnect)
			{
				this->_queuedSendData.clear();
				foreach(QSharedPointer<AsyncResult> result, this->_asyncResults.values())
				{
                    QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
                            <<  "SslChannel::checkTimeout, result " << result.data()->_invokeId << " is timeout";
					QString error = "ConnectFailed";
					result->TimeOut();
					this->processAsyncResult(result, error, QJsonObject(), QDomElement());
				}
				return;
			}

            if(!this->needRecover && this->reConnectionFailedCount < Common::AppSetting::getInstance()->getRecoverConnectionMaxCount())
			{
                this->reConnectionFailedCount = this->reConnectionFailedCount + 1;
                QThread::msleep(1000);
				this->recoverConnection();
			}
			else
			{
				emit this->statusChanged(Common::Enum::ConnectionStatus::ConnectionBroken);	
                this->_queuedSendData.clear();
                foreach(QSharedPointer<AsyncResult> result, this->_asyncResults.values())
                {
                    QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel::checkTimeout, result " << result.data()->_invokeId << " is timeout";
                    QString error = "ConnectionBroken";
                    result->TimeOut();
                    this->processAsyncResult(result, error, QJsonObject(), QDomElement());
                }
			}
		}
	}

	void SslChannel::processHeartbeaterStatusChanged(Common::Enum::BeatStatus::BeatStatus status)
	{
		if(status == Common::Enum::BeatStatus::ConnectionIsBroken)
		{
			QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel.processHeartbeaterStatusChanged: ConnectionIsBroken";
            this->needRecover = true;
			this->recoverConnection();	
		}
		else if(status == Common::Enum::BeatStatus::Failed)
		{
			QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel.processHeartbeaterStatusChanged: SessionTimeout";
			emit this->statusChanged(Common::Enum::ConnectionStatus::ConnectionBroken);
		}
		else if (status == Common::Enum::BeatStatus::SessionTimeout)
		{
			emit this->statusChanged(Common::Enum::ConnectionStatus::ConnectUnstable);			
		}
	}

	void SslChannel::recoverConnection()
    {
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel.recoverConnection";
        QMetaObject::invokeMethod(this, "innerRecoverConnection", Qt::QueuedConnection);
	}

    void SslChannel::innerRecoverConnection()
    {
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel.innerRecoverConnection";
        emit this->statusChanged(Common::Enum::ConnectionStatus::Connecting);
        this->stop(true);
        this->connect();
    }

	void SslChannel::recoverResultCompleted()
	{
		QObject::disconnect(this->_recoverResult.data(), SIGNAL(completed()), this, SLOT(recoverResultCompleted()));
		if(this->_recoverResult->hasError())
		{
			QString error = this->_recoverResult->error();
            QLOG_ERROR( this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel::recoverResultCompleted: " + error;
            if ((!this->_useSsl && error == "QuotatuonServerAuthenticateFailed")
             ||  error == "RecoverConnectionFailed")
			{
				emit this->statusChanged(Common::Enum::ConnectionStatus::ConnectionBroken);
				return;
			}			
			if(this->recoverFailedCount>= Common::AppSetting::getInstance()->getRecoverConnectionMaxCount())
			{
				emit this->statusChanged(Common::Enum::ConnectionStatus::ConnectionBroken);	
				QLOG_ERROR( this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel::recover failed ";
			}
			else
			{
				if (!this->isConnected())
				{
					this->recoverConnection();
				}
				this->needRecover = true;
				this->recoverFailedCount = this->recoverFailedCount + 1;
				this->startRecover();
			}
		}
		else
		{
            this->needRecover = false;
			this->recoverFailedCount = 0;
			emit this->statusChanged(Common::Enum::ConnectionStatus::Recovered);
			QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "recover successful!";
			if(this->_queuedSendData.size() > 0)
			{
				while (!this->_queuedSendData.isEmpty())
				{
					auto request = this->_queuedSendData.dequeue();
					this->_socketWrapper->send(request);
				}
			}
			this->startHeartBeatSlot();
		}
	}

	void SslChannel::processPacket(QSharedPointer<QByteArray> packet)
	{		
		QSharedPointer<Package> package = Package::from(packet);
		//QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel.processPacket packet  end";
        //qDebug() << "SslChannel::processPacket ThreadId:" << QThread::currentThreadId();
        if (package->type() != Package::ExceptionPackage)
		{
			if (this->sessionId.isEmpty() && !package->sessionId().isEmpty())
			{
				this->sessionId = package->sessionId();
			}
			if (package->type() == Package::Response)
			{
				this->processResponse(package);

				this->recoverFailedCount = 0;
				this->heartbeater->failedCount = 0;
			}
			else if (package->type() == Package::KeepAlive || package->type() == Package::KeepAliveSuccessed || package->type() == Package::KeepAliveFailed)
			{
				emit this->heartbeatReceived(package);
			}
			else if ((package->type() == Package::Notify))
			{
                qDebug() << "SslChannel::processPacket Notify ThreadId:" << QThread::currentThreadId();
				emit this->notifyReceived(package->content().value<QDomElement>());

				this->recoverFailedCount = 0;
				this->heartbeater->failedCount = 0;
			}
			else if ((package->type() == Package::Price))
			{
                //QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel.processPacket Price Thread:" << QThread::currentThreadId();
                //qDebug() << "SslChannel.processPacket Price Thread:" << QThread::currentThreadId();
                emit this->quotationReceived(package->content().toString());
				this->recoverFailedCount = 0;
				this->heartbeater->failedCount = 0;
			}
			this->_lastActiveDateTime = QDateTime::currentDateTime();
			//QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel.processPacket  end";
		}
		else
		{
			QLOG_ERROR( this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel.processPacket receive exception package!";
		}
	}

	void SslChannel::processResponse(QSharedPointer<Package>& package)
	{
        if(!this->_asyncResults.contains(package->requestId())) return;
        qDebug() << "SslChannel::processResponse ThreadId:" << QThread::currentThreadId();
		if(package->format() == Package::Json)
		{            
            qDebug() << " SslChannel::processResponse: " << package->content();
			QJsonObject content = package->content().toJsonObject();
			QString error;
			if(content.contains(Common::Constants::ResponseConstants::JsonErrorNodeName))
			{
				error = content.value(Common::Constants::ResponseConstants::JsonErrorNodeName).toString();
			}
			QJsonObject result = content.value(Common::Constants::ResponseConstants::ResultNodeName).toObject();
			this->processAsyncResult(this->_asyncResults[package->requestId()], error, result, QDomElement());
		}
		else if(package->format() == Package::Plain)
		{
			auto content = package->content().toByteArray();		
			QString error = "";
			this->_asyncResults[package->requestId()]->complete(error, content);		
			this->_asyncResults.remove(package->requestId());
			/*if(this->_asyncResults.count() == 0)
			{
			this->_asyncTimeoutCheckTimer.stop();
			}*/
            //QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << "SslChannel::Plain Thread:" << QThread::currentThreadId();
		}	
		else
		{
			QDomElement xmlContent = package->content().value<QDomElement>();		
			QString error;
			if(xmlContent.tagName() == Common::Constants::ResponseConstants::XmlErrorNodeName)
			{
				error = xmlContent.text();
			}	
			this->processAsyncResult(this->_asyncResults[package->requestId()], error, QJsonObject(), xmlContent);	
		}	
	}

	void SslChannel::startHeartBeatSlot()
	{	
        //QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << "SslChannel::startHeartBeatSlot() serverName:" << serverName << "serverPort: " << serverPort << "ThreadId:" << QThread::currentThreadId();
		emit this->startHeartbeaterTimer(this->heartbeater->beatInterval);
		emit this->startHeartbeaterCheckTimeout(this->heartbeater->beatInterval);
	}
	
	void SslChannel::connect()
    {
        //qDebug() <<  "SslChannel::connect() start";
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel::connect() start";
		this->_socketWrapper = QSharedPointer<SocketWrapper>(new SocketWrapper(this->_useSsl));
        bool statusChanged = (bool)QObject::connect(this->_socketWrapper.data(), SIGNAL(statusChanged(Common::Enum::ConnectionStatus::ConnectionStatus)),
			this, SLOT(processSocketStatusChanged(Common::Enum::ConnectionStatus::ConnectionStatus)), Qt::QueuedConnection);
        bool packetReceived = (bool)QObject::connect(this->_socketWrapper.data(), SIGNAL(packetReceived(QSharedPointer<QByteArray>)),
			this, SLOT(processPacket(QSharedPointer<QByteArray>)), Qt::QueuedConnection);	

        //qDebug() <<  "SslChannel::connect() statusChanged:" << statusChanged << " packetReceived:" << packetReceived;
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << "SslChannel::connect() statusChanged:" << statusChanged << " packetReceived:" << packetReceived;
		this->_socketWrapper->connect(this->serverName, this->serverPort, Common::AppSetting::getInstance()->getConnectTimeout()*1000);

        emit startAsyncTimeoutCheckTimer();
	}

    void SslChannel::stop(bool isRecover)
	{
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << "SslChannel::stop() ThreadId:" << QThread::currentThreadId();		
		if(!isRecover)
        {		
			this->recoverFailedCount = Common::AppSetting::getInstance()->getRecoverConnectionMaxCount();
            foreach(QSharedPointer<AsyncResult> result, this->_asyncResults.values())
            {
                QString error = "ChannelStop";
                result->TimeOut();
                this->processAsyncResult(result, error, QJsonObject(), QDomElement());
            }
        }
        emit stopAsyncTimeoutCheckTimer();

		this->heartbeater->stop();
		if(!this->_socketWrapper.isNull())
		{
			QObject::disconnect(this->_socketWrapper.data(), SIGNAL(statusChanged(Common::Enum::ConnectionStatus::ConnectionStatus)), 
				this, SLOT(processSocketStatusChanged(Common::Enum::ConnectionStatus::ConnectionStatus)));

			QObject::disconnect(this->_socketWrapper.data(), SIGNAL(packetReceived(QSharedPointer<QByteArray>)), 
				this, SLOT(processPacket(QSharedPointer<QByteArray>)));
			this->_socketWrapper->close();
		}
		QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << " SslChannel::close end";
	}

	void SslChannel::checkTimeout()
	{
		//QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel::checkTimeout";
		if(this->_asyncResults.count() == 0) return;

		foreach(QSharedPointer<AsyncResult> result, this->_asyncResults.values())
		{
			if(result->checkIsTimeout())
			{
				//QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication, QString("SslChannel::checkTimeout, result %1 is timeout").arg(result.data()->_invokeId));
				QString error = "Timeout";
				result->TimeOut();
				this->processAsyncResult(result, error, QJsonObject(), QDomElement());
			}
		}
	}

    void SslChannel::processAsyncResult(QSharedPointer<AsyncResult> asyncResult, QString error, QJsonObject jsonResult, QDomElement xmlResult)
	{
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << "SslChannel::processAsyncResult: " << asyncResult.data()->_invokeId;
		if(asyncResult->isReadByteArray())
		{
			asyncResult->complete(error, jsonResult);
		}
		else
		{
			if(xmlResult.isNull())
				asyncResult->complete(error, jsonResult);
			else
				asyncResult->complete(error, xmlResult);
		}
		this->_asyncResults.remove(asyncResult->_invokeId);
		/*if(this->_asyncResults.count() == 0)
		{
		this->_asyncTimeoutCheckTimer.stop();
		}*/
	}

	bool SslChannel::isConnected()
	{
		return !this->_socketWrapper.isNull() && this->_socketWrapper->isConnected();
	}

    void SslChannel::startRecover()
	{
        qDebug()<< "Communication::SslChannel::startRecover()";
		QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SslChannel::recoverResultCompleted: start recover";
		QVector<Common::Parameter> parameters;
		if (this->_useTraderQuotationServer)
		{
			if (!this->_isWriteLogOnTraderServerSsl)
			{
				parameters.append(Common::Parameter("watchword", QVariant(this->getWatchword())));
				parameters.append(Common::Parameter("clientId", QVariant(this->_clientId)));
			}
			else
				parameters.append(Common::Parameter("sendQuotationInSsl", QVariant(false)));
		}
		this->_recoverResult = this->asyncInvoke(Common::Constants::MethodNameConstants::Recover, parameters, Common::AppSetting::getInstance()->getRecoverTimeout());
		bool result = QObject::connect(this->_recoverResult.data(), SIGNAL(completed()), this, SLOT(recoverResultCompleted()));
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
            <<QString("SslChannel::recoverResultCompleted recover result id = %1; connect result = %2 recoverFailedCount:%3").arg(
			this->_recoverResult.data()->_invokeId,
			(result ? "true" : "false"),
            QString::number(this->recoverFailedCount));
		emit this->statusChanged(Common::Enum::ConnectionStatus::Recovering);
	}
}
#include "moc_SSLChannel.cpp"
