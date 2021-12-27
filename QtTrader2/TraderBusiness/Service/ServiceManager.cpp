#include "Service/ServiceManager.h"
#include "Common/SignalForwarder.h"
#include "Common/StatusManager.h"
#include "Common/LoginSetting.h"
#include "Common/LogHelper.h"
#include "Common/ThreadManager.h"
//#include "Core\QuotatioProvider.h"


namespace Service
{
	ServiceManager* ServiceManager::m_Instance = Q_NULLPTR;

	ServiceManager* ServiceManager::getInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new ServiceManager;
		}
		return m_Instance;
	}

	ServiceManager::ServiceManager()
	{
        qDebug()<< "ServiceManagerThreadId:" << QThread::currentThreadId();
        this->moveToThread(Common::ThreadManager::getInstance()->getBusinessThread());
		this->_isStop = false;		
		this->_getWatchWorldsFailedCount = 0;
		this->_isUseTraderQuotationServer = Common::LoginSetting::getInstance()->getUseTraderQuotationServer();
		this->start();
	}

	QSharedPointer<TradeService> ServiceManager::getTradeService()
	{
		return this->_tradeService;
	}

	QSharedPointer<ChartService> ServiceManager::getChartService()
	{
		return this->_chartService;
	}

	QSharedPointer<QuotationService> ServiceManager::getQuotationService()
	{
		return this->_quotationService;
	}

	QSharedPointer<ReportService> ServiceManager::getReportService()
	{
		return this->_reportService;
	}

    void ServiceManager::setServer(QString& host, int port, QString& quotationServerHost, int quotationServerPort, bool useTraderQuotationServer, bool isReConnect)
	{
		Common::LoginSetting::getInstance()->setValue("LoginSetting", "UseTraderQuotationServer", useTraderQuotationServer);
        this->_isUseTraderQuotationServer = useTraderQuotationServer;
        if ((channelIsConnected() && this->_channel->isChangeServer(host, port)) || isReConnect)
        {
            this->stop(false);
        }

        if (this->_isStop)
		{
			this->start();
			this->_isStop = false;
		}
		this->_channel->setServer(host, port);
		if (this->_isUseTraderQuotationServer)
        {
            if(this->_quotationChannel.isNull())
            {
                QSharedPointer<Communication::SslChannel> newQuotationChannel(new Communication::SslChannel(false, false, this->_isUseTraderQuotationServer));
                this->_quotationChannel = newQuotationChannel;
                connect(this->_quotationChannel.data(), SIGNAL(statusChanged(Common::Enum::ConnectionStatus::ConnectionStatus)), this, SIGNAL(connectionStatusChangedSignal(Common::Enum::ConnectionStatus::ConnectionStatus)));
                connect(this, SIGNAL(startQuotationHeartBeatSignal()), this->_quotationChannel.data(), SLOT(startHeartBeatSlot()), Qt::BlockingQueuedConnection);
                connect(this->_quotationChannel.data(), SIGNAL(getWatchwordsSignal()), this, SLOT(getWatchWorldsSlot()));
                QSharedPointer<QuotationService> newQuotationService(new QuotationService(newQuotationChannel));
                this->_quotationService = newQuotationService;
                this->_quotationService->moveToThread(Common::ThreadManager::getInstance()->getBusinessThread());
            }
			this->_quotationChannel->setServer(quotationServerHost, quotationServerPort);
        }
	}

	void ServiceManager::start()
	{
        qDebug() <<  "ServiceManager::start start";
		QSharedPointer<Communication::SslChannel> newChannel(new Communication::SslChannel());
		this->_channel = newChannel;
		QSharedPointer<TradeService> newTradeService(new TradeService(newChannel));
		this->_tradeService = newTradeService;
        this->_tradeService->moveToThread(Common::ThreadManager::getInstance()->getBusinessThread());
		QSharedPointer<ReportService> newReportService(new ReportService(newChannel));
		this->_reportService = newReportService;
		QSharedPointer<ChartService> newChartService(new ChartService(newChannel));
		this->_chartService = newChartService;

		connect(this->_channel.data(), SIGNAL(statusChanged(Common::Enum::ConnectionStatus::ConnectionStatus)), this, SIGNAL(connectionStatusChangedSignal(Common::Enum::ConnectionStatus::ConnectionStatus)));
		connect(this, SIGNAL(startHeartBeatSignal()), this->_channel.data(), SLOT(startHeartBeatSlot()), Qt::BlockingQueuedConnection);
		if (this->_isUseTraderQuotationServer)
		{
            qDebug() <<  "ServiceManager::start QuotationChannel init";
			QSharedPointer<Communication::SslChannel> newQuotationChannel(new Communication::SslChannel(false, false, this->_isUseTraderQuotationServer));
			this->_quotationChannel = newQuotationChannel;
			connect(this->_quotationChannel.data(), SIGNAL(statusChanged(Common::Enum::ConnectionStatus::ConnectionStatus)), this, SIGNAL(connectionStatusChangedSignal(Common::Enum::ConnectionStatus::ConnectionStatus)));
			connect(this, SIGNAL(startQuotationHeartBeatSignal()), this->_quotationChannel.data(), SLOT(startHeartBeatSlot()), Qt::BlockingQueuedConnection);
			connect(this->_quotationChannel.data(), SIGNAL(getWatchwordsSignal()), this, SLOT(getWatchWorldsSlot()));
			QSharedPointer<QuotationService> newQuotationService(new QuotationService(newQuotationChannel));
			this->_quotationService = newQuotationService;
            this->_quotationService->moveToThread(Common::ThreadManager::getInstance()->getBusinessThread());
		}
		else
        {
			QSharedPointer<QuotationService> newQuotationService(new QuotationService(newChannel));
			this->_quotationService = newQuotationService;
            this->_quotationService->moveToThread(Common::ThreadManager::getInstance()->getBusinessThread());
		}
		connect(Common::StatusManager::getInstance(), SIGNAL(computingCompletedSignal()), this, SLOT(isStartHeartBeatSlot()));
        qDebug() <<  "ServiceManager::start end";
	}

	void ServiceManager::stop(bool isdeleteLater)
	{
        if (this->_isStop)
        {
            QLOG_INFO(Common::Constants::LogKeyConstants::Communication) <<  "ServiceManager::stop stopping";
            qDebug() <<  "ServiceManager::stop stopping";
            return;
        }
		this->_isStop = true;
        QLOG_INFO(Common::Constants::LogKeyConstants::Communication) <<  "ServiceManager::stop start";
        qDebug() <<  "ServiceManager::stop start";
		disconnect(this->_channel.data(), SIGNAL(statusChanged(Common::Enum::ConnectionStatus::ConnectionStatus)), this, SIGNAL(connectionStatusChangedSignal(Common::Enum::ConnectionStatus::ConnectionStatus)));
		disconnect(this, SIGNAL(startHeartBeatSignal()), this->_channel.data(), SLOT(startHeartBeatSlot()));
		disconnect(Common::StatusManager::getInstance(), SIGNAL(computingCompletedSignal()), this, SLOT(isStartHeartBeatSlot()));
				
		if (this->_isUseTraderQuotationServer && !this->_quotationChannel.isNull())
		{
			disconnect(this->_quotationChannel.data(), SIGNAL(statusChanged(Common::Enum::ConnectionStatus::ConnectionStatus)), this, SIGNAL(connectionStatusChangedSignal(Common::Enum::ConnectionStatus::ConnectionStatus)));
			disconnect(this, SIGNAL(startQuotationHeartBeatSignal()), this->_quotationChannel.data(), SLOT(startHeartBeatSlot()));
			disconnect(this->_quotationChannel.data(), SIGNAL(getWatchwordsSignal()), this, SLOT(getWatchWorldsSlot()));

            QLOG_INFO(Common::Constants::LogKeyConstants::Communication) <<  "ServiceManager::stop quotationChannel close";
            qDebug() <<  "ServiceManager::stop quotationChannel close";
			this->_quotationChannel->close();
			if (isdeleteLater)
			{
				this->_quotationChannel->deleteLater();
			}
			else
				this->_quotationChannel.clear();
		}

        QLOG_INFO(Common::Constants::LogKeyConstants::Communication) <<  "ServiceManager::stop channel close";
        qDebug() <<  "ServiceManager::stop channel close";
		this->_channel->close();
		if (isdeleteLater)
			this->_channel->deleteLater();
		else
			this->_channel.clear();

		this->_quotationService.clear();
		this->_tradeService.clear();
		this->_reportService.clear();
		this->_chartService.clear();
        QLOG_INFO(Common::Constants::LogKeyConstants::Communication) <<  "ServiceManager::stop end";
        qDebug() <<  "ServiceManager::stop end";
	}

    void ServiceManager::stopQuotationServer()
    {
        disconnect(this->_quotationChannel.data(), SIGNAL(statusChanged(Common::Enum::ConnectionStatus::ConnectionStatus)), this, SIGNAL(connectionStatusChangedSignal(Common::Enum::ConnectionStatus::ConnectionStatus)));
        disconnect(this, SIGNAL(startQuotationHeartBeatSignal()), this->_quotationChannel.data(), SLOT(startHeartBeatSlot()));
        disconnect(this->_quotationChannel.data(), SIGNAL(getWatchwordsSignal()), this, SLOT(getWatchWorldsSlot()));
        QLOG_INFO(Common::Constants::LogKeyConstants::Communication) <<  "ServiceManager::stopQuotationServer start";
        qDebug() <<  "ServiceManager::stop stopQuotationServer close";
        if (this->_isUseTraderQuotationServer && !this->_quotationChannel.isNull())
        {
            QLOG_INFO(Common::Constants::LogKeyConstants::Communication) <<  "ServiceManager::stopQuotationServer quotationChannel close";
            this->_quotationChannel->close();
            this->_quotationChannel->deleteLater();
        }
        this->_isUseTraderQuotationServer = false;
        this->_quotationService.clear();

        QSharedPointer<QuotationService> newQuotationService(new QuotationService(this->_channel));
        this->_quotationService = newQuotationService;
        this->_quotationService->moveToThread(Common::ThreadManager::getInstance()->getBusinessThread());
        QLOG_INFO(Common::Constants::LogKeyConstants::Communication) <<  "ServiceManager::stopQuotationServer end";
    }

    bool ServiceManager::channelIsConnected()
    {
        return this->_channel->isConnected();
    }

    bool ServiceManager::isUseTraderQuotationServer()
    {
        return this->_isUseTraderQuotationServer;
    }

	void ServiceManager::setQuotationServerParameter(QStringList watchwords, QString clientId)
	{
		if (this->_isUseTraderQuotationServer)
		{
			this->_quotationChannel->setWatchwords(watchwords);
			this->_quotationChannel->setClientId(clientId);
		}
	}

	void ServiceManager::isStartHeartBeatSlot()
	{
		if (Common::StatusManager::getInstance()->isComputingCompleted())
			emit startHeartBeatSignal();
	}

	void ServiceManager::startQuotationHeartBeat()
	{
		emit startQuotationHeartBeatSignal();
	}

	QString ServiceManager::getWatchword()
	{
		if (this->_isUseTraderQuotationServer)
		{
			return this->_quotationChannel->getWatchword();
		}
		return "";
	}

	QString ServiceManager::getClientId()
	{
		if (this->_isUseTraderQuotationServer)
		{
			return this->_quotationChannel->getClientId();
		}
		return "";
	}
	
	void ServiceManager::getWatchWorldsResultSlot()
	{
		QObject::disconnect(this->_getWatchWorldsResult.data(), SIGNAL(completed()), this, SLOT(getWatchWorldsResultSlot()));
		if (this->_getWatchWorldsResult->hasError())
		{
			QString error = this->_getWatchWorldsResult->error();
			QLOG_ERROR( Common::Constants::LogKeyConstants::Service) <<  "ServiceManager::getWatchWorldsResultSlot" << error;
			if(this->_getWatchWorldsFailedCount < 5)
				this->getWatchWorldsSlot();
			this->_getWatchWorldsFailedCount++;
		}
		else
		{
			auto jsonResult = this->_getWatchWorldsResult->getJsonResult();
			QJsonArray watchwordsArray = jsonResult.take("Watchwords").toArray();
			int size = watchwordsArray.size();
			QStringList watchwords;
			for (int i = 0; i < size; i++)
			{
				auto watchword = watchwordsArray.at(i);
				watchwords.append(watchword.toString());
			}
			this->setQuotationServerParameter(watchwords, this->getClientId());
			this->_getWatchWorldsFailedCount = 0;
		}
	}

	void ServiceManager::getWatchWorldsSlot()
	{
		this->_getWatchWorldsResult = this->_tradeService->getWatchWorlds(this->getClientId());
		QObject::connect(this->_getWatchWorldsResult.data(), SIGNAL(completed()), this, SLOT(getWatchWorldsResultSlot()));
	}

}
#include "moc_ServiceManager.cpp"
