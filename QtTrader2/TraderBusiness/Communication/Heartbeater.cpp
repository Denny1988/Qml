#include "Heartbeater.h"
#include "SSLChannel.h"
#include "Common/AppSetting.h"
#include "Common/LogHelper.h"
#include "Common/Constants/MethodNameConstants.h"

namespace Communication
{
    Heartbeater::Heartbeater(SslChannel* ownerSslChannel, bool sendHeartbeat)
        :QObject(ownerSslChannel)
	{
        this->_ownerSslChannel = ownerSslChannel;
		this->_sendHeartbeat = sendHeartbeat;
		this->beatTimer = new QTimer(this);
		this->checkTimeoutTimer = new QTimer(this);
		this->_lastHeartbeaterSuccessTimeRecord = QDateTime::currentDateTime();

        QObject::connect(this->beatTimer, SIGNAL(timeout()), this, SLOT(beat()));
        QObject::connect(this, SIGNAL(stopBeatTimerSignal()), this->beatTimer, SLOT(stop()));

        QObject::connect(this->_ownerSslChannel, SIGNAL(heartbeatReceived(QSharedPointer<Package>&)),
			this, SLOT(process(QSharedPointer<Package>&)));

        QObject::connect(this->_ownerSslChannel, SIGNAL(startHeartbeaterTimer(int)), this->beatTimer, SLOT(start(int)));

        QObject::connect(this->checkTimeoutTimer, SIGNAL(timeout()), this, SLOT(checkTimeoutSlot()));

        QObject::connect(this->_ownerSslChannel, SIGNAL(startHeartbeaterCheckTimeout(int)), this->checkTimeoutTimer, SLOT(start(int)));
        QObject::connect(this, SIGNAL(stopCheckTimeoutTimerSignal()), this->checkTimeoutTimer, SLOT(stop()));

		this->failedCount = 0;
        this->beatInterval = Common::AppSetting::getInstance()->getBeatInterval()*1000;//心跳时间
        this->beatIntervalOnNotReceiveCommand = Common::AppSetting::getInstance()->getBeatIntervalOnNotReceiveCommand()*1000;//如果没有收到通知的时间与当前时间相比超过就开始心跳
        this->beatTimeoutInterval = Common::AppSetting::getInstance()->getHeartbeaterTimeout()*1000;//心跳超时时间
		this->continuityHeartbeaterTimeoutCount = Common::AppSetting::getInstance()->getContinuityHeartbeaterTimeoutCount();//心跳超时最大次数
		int beatIntervalOnReceiveCommand = Common::AppSetting::getInstance()->getBeatIntervalOnReceiveCommand();
        this->beatIntervalOnReceiveCommand = beatIntervalOnReceiveCommand*1000*60;//如果收到通知，就用这个作为间隔时间去心跳请求,并且超时时间也设置2倍，注意这个值为分钟
        if (this->beatIntervalOnReceiveCommand == 0) this->beatIntervalOnReceiveCommand = 2*60*1000;
        QLOG_INFO(this->_ownerSslChannel->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
                << "Heartbeater::Heartbeater beatInterval:" << this->beatInterval
                << " beatIntervalOnNotReceiveCommand:" << this->beatIntervalOnNotReceiveCommand
                << " beatTimeoutInterval:" << this->beatTimeoutInterval
                << " continuityHeartbeaterTimeoutCount:" << this->continuityHeartbeaterTimeoutCount
                << " config_beatIntervalOnReceiveCommand:" << beatIntervalOnReceiveCommand
                << " beatIntervalOnReceiveCommand:" << this->beatIntervalOnReceiveCommand;

        qDebug() << "Heartbeater::Heartbeater beatInterval:" << this->beatInterval
                << " beatIntervalOnNotReceiveCommand:" << this->beatIntervalOnNotReceiveCommand
                << " beatTimeoutInterval:" << this->beatTimeoutInterval
                << " continuityHeartbeaterTimeoutCount:" << this->continuityHeartbeaterTimeoutCount
                << " config_beatIntervalOnReceiveCommand:" << beatIntervalOnReceiveCommand
                << " beatIntervalOnReceiveCommand:" << this->beatIntervalOnReceiveCommand;
	}

    Heartbeater::~Heartbeater()
    {
        QLOG_INFO(this->_ownerSslChannel->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "Heartbeater::~Heartbeater() ";
        qDebug() << "Heartbeater::~Heartbeater() ";
        delete this->beatTimer;
        delete this->checkTimeoutTimer;
    }


	void Heartbeater::stop()
	{
        emit stopBeatTimerSignal();
        emit stopCheckTimeoutTimerSignal();
	}

	void Heartbeater::checkTimeout(QHash<QString, QDateTime>& asyncHeartbeaterTimeRecord, int beatInterval, qint64 currentMSecs)
	{
		if (asyncHeartbeaterTimeRecord.count() > 0)
		{
			QHashIterator<QString, QDateTime> item(asyncHeartbeaterTimeRecord);
			while (item.hasNext()) {
				item.next();
				qint64 c = item.value().toMSecsSinceEpoch();
				if ((currentMSecs - c) > beatInterval)
				{
					emit this->statusChanged(Common::Enum::BeatStatus::SessionTimeout);
					QLOG_INFO(this->_ownerSslChannel->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) 
						<< "Heartbeater::checkTimeoutSlot: Heart Timeout! " << " requestId:" << item.key() << " SessionId:" << this->_ownerSslChannel->sessionId;
					this->failedCount++;
					if (this->failedCount > this->continuityHeartbeaterTimeoutCount)
					{
						emit this->statusChanged(Common::Enum::BeatStatus::ConnectionIsBroken);
					}
					asyncHeartbeaterTimeRecord.remove(item.key());
				}
			}
		}
	}

	void Heartbeater::innerProcess(QSharedPointer<Package>& package)
	{
		if (package->type() == Package::KeepAliveFailed)
		{
			emit this->statusChanged(Common::Enum::BeatStatus::Failed);
			QLOG_INFO(this->_ownerSslChannel->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
				<< "Heartbeater::process: Heart failed!  SessionId: " << this->_ownerSslChannel->sessionId;
		}
		else
		{
            QLOG_INFO(this->_ownerSslChannel->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
                << "Heartbeater::process: Heart Success!  SessionId: " << this->_ownerSslChannel->sessionId;
			this->failedCount = 0;
			this->_lastHeartbeaterSuccessTimeRecord = QDateTime::currentDateTime();
		}
	}

	QString Heartbeater::beatRequest()
	{
		QString requestId = QUuid::createUuid().toString();
        QSharedPointer<QByteArray> request = Package::buildKeepAliveRequest(this->_ownerSslChannel->sessionId, requestId);
        this->_ownerSslChannel->sendData(request);
		return requestId;
	}

	void Heartbeater::checkTimeoutSlot()
	{
		QDateTime now = QDateTime::currentDateTime();
		this->checkTimeout(this->_asyncHeartbeaterTimeRecord, this->beatTimeoutInterval, now.toMSecsSinceEpoch());
		this->checkTimeout(this->_asyncHeartbeaterLongTimeRecord, this->beatTimeoutInterval*2, now.toMSecsSinceEpoch());
	}

	void Heartbeater::beat()
	{
		if(this->_sendHeartbeat)
		{
			QDateTime now = QDateTime::currentDateTime();
			qint64 nowMSecs = now.toMSecsSinceEpoch();
            qint64 lastActiveMSecs = this->_ownerSslChannel->_lastActiveDateTime.toMSecsSinceEpoch();
			int interval = (nowMSecs - lastActiveMSecs);
			if (interval > this->beatIntervalOnNotReceiveCommand)
			{
				QLOG_INFO(this->_ownerSslChannel->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
					<< "Heartbeater::beat: NowDateTime-lastActiveDateTime>beatInterval appsetting.beatInterval:" << this->beatInterval << " lastActiveDateTime.interval:" << interval;
				this->_asyncHeartbeaterTimeRecord.insert(this->beatRequest(), QDateTime::currentDateTime());
			}
			else
			{
				qint64 lastHeartbeaterSuccessMSecs = this->_lastHeartbeaterSuccessTimeRecord.toMSecsSinceEpoch();
				int interval2 = (nowMSecs - lastHeartbeaterSuccessMSecs);
				if (interval2 > this->beatIntervalOnReceiveCommand)
				{
					QLOG_INFO(this->_ownerSslChannel->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication)
						<< "Heartbeater::beat: NowDateTime-lastHeartbeaterSuccessTimeRecord>beatIntervalOnReceiveCommand appsetting.beatIntervalOnReceiveCommand:" << this->beatIntervalOnReceiveCommand << " lastHeartbeaterSuccessTime.interval:" << interval2;
					this->_asyncHeartbeaterLongTimeRecord.insert(this->beatRequest(), QDateTime::currentDateTime());
				}
			}
		}
	}

	void Heartbeater::process(QSharedPointer<Package>& package)
	{
		if (this->_asyncHeartbeaterTimeRecord.contains(package->requestId()))
		{
			this->innerProcess(package);
			this->_asyncHeartbeaterTimeRecord.remove(package->requestId());
		}
		else
		{
			if (this->_asyncHeartbeaterLongTimeRecord.contains(package->requestId()))
			{
				this->innerProcess(package);
				this->_asyncHeartbeaterLongTimeRecord.remove(package->requestId());
			}
		}
	}
}

#include "moc_Heartbeater.cpp"
