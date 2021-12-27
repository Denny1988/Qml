#ifndef SERVICE_SERVICEMANAGER_H
#define SERVICE_SERVICEMANAGER_H

#include "traderbusiness_global.h"
#include <QSharedPointer>
#include "Service/TradeService.h"
#include "Service/ReportService.h"
#include "Service/QuotationService.h"
#include "Service/ChartService.h"
#include "Service/ProvisionalServiceManager.h"
#include <QMap>

using namespace Common::Enum;
namespace Service
{	
	class TRADERBUSINESS_EXPORT ServiceManager: public QObject
	{
		Q_OBJECT	
	public:
		static ServiceManager* getInstance();		
		QSharedPointer<TradeService> getTradeService();
		QSharedPointer<ChartService> getChartService();
		QSharedPointer<QuotationService> getQuotationService();
		QSharedPointer<ReportService> getReportService();

        void setServer(QString& host, int port, QString& quotationServerHost, int quotationServerPort, bool useTraderQuotationServer, bool isReConnect = false);
		void start();
		void stop(bool isdeleteLater = true);
        void stopQuotationServer();
		void setQuotationServerParameter(QStringList watchwords, QString clientId);
		void startQuotationHeartBeat();
		QString getWatchword();
		QString getClientId();
		bool isStop() { return _isStop; }
        bool channelIsConnected();
        bool isUseTraderQuotationServer();
		
	private:
		ServiceManager();

	signals:
		void connectionStatusChangedSignal(Common::Enum::ConnectionStatus::ConnectionStatus status);
		//void quotationConnectionStatusChangedSignal(Common::Enum::ConnectionStatus::ConnectionStatus status);
		void startHeartBeatSignal();
		void startQuotationHeartBeatSignal();
		
	public slots:
		void isStartHeartBeatSlot();
		void getWatchWorldsResultSlot();
		void getWatchWorldsSlot();

	private:
		static ServiceManager* m_Instance;
		QSharedPointer<TradeService> _tradeService;
		QSharedPointer<ChartService> _chartService;
		QSharedPointer<QuotationService> _quotationService;
		QSharedPointer<ReportService> _reportService;
		QSharedPointer<Communication::SslChannel> _channel;
		QSharedPointer<Communication::SslChannel> _quotationChannel;
		QSharedPointer<Common::Result::InvokeResult> _getWatchWorldsResult;
		
		bool _isStop;
		bool _isUseTraderQuotationServer;
		int _getWatchWorldsFailedCount;
	};
}

#endif
