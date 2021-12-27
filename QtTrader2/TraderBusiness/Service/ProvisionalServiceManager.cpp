#include "ProvisionalServiceManager.h"
#include "ServiceManager.h"

#include "Common/TraderBaseInfo.h"
#include "Common/Enum/StatusEnum.h"
#include "Common/Constants/MethodNameConstants.h"
#include "Common/Util/Convert_Common.h"
#include "Common/LogHelper.h"
#include "Common/CustomerSetting.h"
#include "Common/AppSetting.h"

using namespace Communication;

namespace Service
{
	ProvisionalServiceManager* ProvisionalServiceManager::m_Instance = Q_NULLPTR;

	QMutex ProvisionalServiceManager::m_Mutex(QMutex::Recursive);
	ProvisionalServiceManager* ProvisionalServiceManager::getInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new ProvisionalServiceManager;
		}
		return m_Instance;
	}

	ProvisionalServiceManager::ProvisionalServiceManager()
	{
		this->_provisionalTimer = new QTimer(this);
		this->_provisionalTimer->setInterval(1000 * Common::AppSetting::getInstance()->getProvisionalTimerInterval());//1 Minute
		QObject::connect(this->_provisionalTimer, SIGNAL(timeout()), this, SLOT(checkProvisionalConnectTimeOutSlot()));
		QObject::connect(this, SIGNAL(startProvisionalTimer()), this->_provisionalTimer, SLOT(start()));
		emit startProvisionalTimer();
		QObject::connect(this, SIGNAL(getWatchWorldsSignal()), ServiceManager::getInstance(), SLOT(getWatchWorldsSlot()));
	}
		
	///------------------Request Method---------------------
	QSharedPointer<ProvisionalLoginResult> ProvisionalServiceManager::provisionalLogin(QString serverName, quint16 serverPort, QString connectkey)
	{
		QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<  "ProvisionalServiceManager::provisionalLogin";
		if (!this->_provisionalChannels.contains(connectkey))
		{
            QLOG_INFO(Common::Constants::LogKeyConstants::Communication) << "ProvisionalServiceManager::provisionalLogin connectkey:" << connectkey;
			QSharedPointer<Communication::SslChannel> newChannel(new Communication::SslChannel(true, true, false, true));
			newChannel->setServer(serverName, serverPort);			
			this->_provisionalChannels.insert(connectkey, newChannel); 
			QThread::msleep(50);	

			QVector<Common::Parameter> parameters;
			parameters.append(Common::Parameter("userID", QVariant(Common::TraderBaseInfo::getInstance()->getUserId().toString().mid(1, 36))));
			parameters.append(Common::Parameter("organizationCode", QVariant(Common::TraderBaseInfo::getInstance()->getPathName())));
			parameters.append(Common::Parameter("clientId", QVariant(ServiceManager::getInstance()->getClientId())));
			QString watchword = ServiceManager::getInstance()->getWatchword();
            QLOG_INFO(Common::Constants::LogKeyConstants::Communication) << "ProvisionalServiceManager::provisionalLogin watchword:" << watchword;
			parameters.append(Common::Parameter("watchword", QVariant(watchword)));
			QSharedPointer<AsyncResult> result = this->_provisionalChannels[connectkey]->asyncInvoke(Common::Constants::MethodNameConstants::ProvisionalLogin, parameters, 15);
			return QSharedPointer<ProvisionalLoginResult>(new ProvisionalLoginResult(result));
		}
		else
		{
			if(this->_provisionalLoginResults.contains(connectkey))
				return this->_provisionalLoginResults[connectkey];
			else
			{
				QVector<Common::Parameter> parameters;
				parameters.append(Common::Parameter("userID", QVariant(Common::TraderBaseInfo::getInstance()->getUserId().toString().mid(1, 36))));
				parameters.append(Common::Parameter("organizationCode", QVariant(Common::TraderBaseInfo::getInstance()->getPathName())));
				parameters.append(Common::Parameter("clientId", QVariant(ServiceManager::getInstance()->getClientId())));
				QString watchword = ServiceManager::getInstance()->getWatchword();
                QLOG_INFO(Common::Constants::LogKeyConstants::Communication) << "ProvisionalServiceManager::provisionalLogin watchword:" << watchword;
				parameters.append(Common::Parameter("watchword", QVariant(watchword)));
				QSharedPointer<AsyncResult> result = this->_provisionalChannels[connectkey]->asyncInvoke(Common::Constants::MethodNameConstants::ProvisionalLogin, parameters, 15);
				return QSharedPointer<ProvisionalLoginResult>(new ProvisionalLoginResult(result));
			}
		}
	}

	QSharedPointer<Common::Result::PlaceResult> ProvisionalServiceManager::beginProvisionalPlace(QString serverName, quint16 serverPort, QString tranXmlString, QUuid id)
	{		
		QMutexLocker locker(&ProvisionalServiceManager::m_Mutex);
		QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<  "ProvisionalServiceManager::ProvisionalPlace";
		QString connectkey = QString("%1_%2").arg(serverName, QString::number(serverPort));
		QSharedPointer<AsyncResult> result;
		if (this->_authenticatedProvisionalChannels.contains(connectkey))
		{
			QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<  "ProvisionalServiceManager::ProvisionalPlace authenticatedProvisionalChannel exsit";
			result = this->_authenticatedProvisionalChannels[connectkey]->getInvokeAsyncResult(Common::Constants::MethodNameConstants::ProvisionalPlace, 10);
			QSharedPointer<Common::Result::XmlDataResult> XmlDataResult = ServiceManager::getInstance()->getTradeService()->getTransactionXml(tranXmlString);			
			QSharedPointer<GetTransactionXmlResult> getTransactionXmlResult(new GetTransactionXmlResult(XmlDataResult, result, result->getInvokeId(), connectkey));
			QObject::connect(getTransactionXmlResult.data(), SIGNAL(completed()), this, SLOT(getTransactionXmlCompletedSlot()));
			this->_getTransactionXmlResultResults.insert(result->getInvokeId(), getTransactionXmlResult);
		}
		else
		{
			QSharedPointer<ProvisionalLoginResult> provisionalLoginResult = provisionalLogin(serverName, serverPort, connectkey);

			result = this->_provisionalChannels[connectkey]->getInvokeAsyncResult(Common::Constants::MethodNameConstants::ProvisionalPlace, 15);
			provisionalLoginResult->addItem(result, result->getInvokeId(), connectkey, tranXmlString);
			QObject::connect(provisionalLoginResult.data(), SIGNAL(completed()), this, SLOT(authenticatedCompletedSlot()));
			this->_provisionalLoginResults.insert(connectkey, provisionalLoginResult);
		}
		return QSharedPointer<Common::Result::PlaceResult>(new Common::Result::PlaceResult(result, id));
	}

	void ProvisionalServiceManager::close()
	{
		QMutexLocker locker(&ProvisionalServiceManager::m_Mutex);
		QVector<Common::Parameter> parameters;
		foreach(QString key, this->_authenticatedProvisionalChannels.keys())
		{
			auto channel = this->_authenticatedProvisionalChannels[key];
			this->_authenticatedProvisionalChannels.remove(key);
			//channel->asyncInvoke(Common::Constants::MethodNameConstants::ProvisionalLogout, parameters, 2);
			channel->close();
			//channel->deleteLater();
			channel.clear();
		}
	}

	void ProvisionalServiceManager::checkProvisionalConnectTimeOutSlot()
	{
		QMutexLocker locker(&ProvisionalServiceManager::m_Mutex);
		QList<QString> needRemoveList;
		QMap<QString, QSharedPointer<Communication::SslChannel>>::const_iterator item = this->_authenticatedProvisionalChannels.constBegin();
		while (item != this->_authenticatedProvisionalChannels.constEnd()) {
			if (item.value()->isTimeOutFromProvisional())
				needRemoveList.append(item.key());
			++item;
		}
		QVector<Common::Parameter> parameters;
		foreach(QString key, needRemoveList)
		{
			auto channel = this->_authenticatedProvisionalChannels[key];
			this->_authenticatedProvisionalChannels.remove(key);
			//channel->asyncInvoke(Common::Constants::MethodNameConstants::ProvisionalLogout, parameters, 2);
			channel->close();
			//channel->deleteLater();
			channel.clear();
		}
	}

	void ProvisionalServiceManager::authenticatedCompletedSlot()
	{		
		QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<  "ProvisionalServiceManager::authenticatedCompletedSlot";
		QMutexLocker locker(&ProvisionalServiceManager::m_Mutex);

		QString key = ((ProvisionalLoginResult*)sender())->_connectkey;
		QSharedPointer<ProvisionalLoginResult> result = this->_provisionalLoginResults[key];

		disconnect(result.data(), SIGNAL(completed()), this, SIGNAL(authenticatedCompletedSlot()));
		QString connectkey = result->_connectkey;
		if (result->hasError())
		{
			QString error = result->error();
			this->_provisionalChannels.remove(connectkey);
			QMapIterator<QString, QSharedPointer<Communication::AsyncResult>> item(result->_placeAsyncResults);
			while (item.hasNext()) {
				item.next();
				item.value()->complete("ConnectFailed");//ConnectFailed | Authenticatefailed
			}
			result->_placeAsyncResults.clear();
			emit getWatchWorldsSignal();
		}
		else
		{
			this->_authenticatedProvisionalChannels.insert(connectkey, this->_provisionalChannels[connectkey]);
			this->_provisionalChannels.remove(connectkey);
			
			QMapIterator<QString, QSharedPointer<Communication::AsyncResult>> item(result->_placeAsyncResults);
			while (item.hasNext()) {
				item.next();
				QString invokeId = item.key();				
				QSharedPointer<Common::Result::XmlDataResult> XmlDataResult = ServiceManager::getInstance()->getTradeService()->getTransactionXml(result->_tranXmlStrings[invokeId]);
				QSharedPointer<GetTransactionXmlResult> getTransactionXmlResult(new GetTransactionXmlResult(XmlDataResult, result->_placeAsyncResults[invokeId], invokeId, connectkey));
				QObject::connect(getTransactionXmlResult.data(), SIGNAL(completed()), this, SLOT(getTransactionXmlCompletedSlot()));
				this->_getTransactionXmlResultResults.insert(invokeId, getTransactionXmlResult);
			}
		}
		this->_provisionalLoginResults.remove(key);
	}

	void ProvisionalServiceManager::getTransactionXmlCompletedSlot()
	{
		QLOG_INFO(Common::Constants::LogKeyConstants::Service) <<  "ProvisionalServiceManager::getTransactionXmlCompletedSlot";
		QString key = ((GetTransactionXmlResult*)sender())->_invokeId;
		QSharedPointer<GetTransactionXmlResult> result = this->_getTransactionXmlResultResults[key];
		if (result->hasError())
		{
			result->_placeAsyncResult->complete("GetTransactionXmlFailed");
			auto channel = this->_authenticatedProvisionalChannels[result->_connectkey];
			this->_authenticatedProvisionalChannels.remove(result->_connectkey);
			channel->close();
			channel.clear();
		}
		else
		{
			QDomDocument doc;
			doc.setContent(result->_asyncResult->getByteArrayResult());
			QString tranXmlString = doc.toString();
			QVector<Common::Parameter> parameters;
			parameters.append(Common::Parameter("transactionXml", QVariant(tranXmlString)));
			this->_authenticatedProvisionalChannels[result->_connectkey]->asyncInvoke(
				Common::Constants::MethodNameConstants::ProvisionalPlace,
				parameters,
				result->_placeAsyncResult);

		}
		this->_getTransactionXmlResultResults.remove(key);
	}

	

}
#include "moc_ProvisionalServiceManager.cpp"
