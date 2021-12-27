#ifndef BUSINESS_BUSINESSMANAGER_H
#define BUSINESS_BUSINESSMANAGER_H
#include "traderbusiness_global.h"
#include <QDomNode>
#include <QList>
#include <QSharedPointer>
#include "Common/Result/Result.h"
#include "Common/Result/PlaceResult.h"
#include "Business/SettingManager.h"
#include "Business/InitializeManager.h"
#include "Business/TradingManager.h"
#include "Business/ComputingManager.h"
#include "Business/QuotationManager.h"
#include "Business/ServerSourceManager.h"
#include "Common/Enum/CommonEnum.h"
#include <QMap>
#include <QTimer>

namespace Business
{
	class TRADERBUSINESS_EXPORT BusinessManager: public QObject
	{
		Q_OBJECT	
	public:
		static BusinessManager* getInstance();
		static QMutex  m_Mutex;

	private:
		BusinessManager();
		static BusinessManager* m_Instance;
		bool _isNeedLoadTradingData;
		QList<QUuid> _unLoadTradingAccountIds;
		QList<QUuid> _currentLoadAccountIds;
		bool _isCompleteInitTradingData;
		bool _isNeedLoadMultiTradingData;
		int _loadAccountCount;
		QStringList  _tradingAccountIds;
		int _failedGetTradingDataCount;
		//QMutex _mutex;
		QTimer* _updateBOCountdownTimer;
		QTimer* _transactionExpireInspectorTimer;

		QSharedPointer<SettingManager> _settingManager;
		QSharedPointer<InitializeManager> _initializeManager;
		QSharedPointer<TradingManager> _tradingManager;
		QSharedPointer<ComputingManager> _computingManager;
		QSharedPointer<QuotationManager> _quotationManager;
		QSharedPointer<ServerSourceManager> _serverSourceManager;
		
		QSharedPointer<Common::Result::XmlDataResult> _initDataXmlResult;
		QSharedPointer<Common::Result::XmlDataResult> _settingDataXmlResult;
		QSharedPointer<Common::Result::XmlDataResult> _tradingDataXmlResult;
		QSharedPointer<Common::Result::XmlDataResult> _verifyTransactionXmlResult;
        QSharedPointer<Common::Result::XmlDataResult> _refreshTradingDataXmlResult;
		QMap<QUuid, QSharedPointer<Common::Result::PlaceResult>> _placeResults;
		QMap<QUuid, QSharedPointer<Common::Result::XmlDataResult>> _logResults;
		QSharedPointer<Common::Result::InvokeResult> _loginQuotationServerResult;
        QSharedPointer<Common::Result::InvokeResult> _changeSingleChannelResult;

        QList<QString> _requestQuoteMap;
        bool _sleepStatus;
        bool _noData;        
        QDateTime _initCompletedTime;

private:
		void getSettingData();
		void getTradingData(const QStringList &  accountIds);
		void startComputing();
		void initPhysical();
        void loadMultiAccountTradingData();
        void resume();

	public:
		QSharedPointer<SettingManager> getSettingManager();
		QSharedPointer<InitializeManager> getInitializeManager();
		QSharedPointer<TradingManager> getTradingManager();
		QSharedPointer<ComputingManager> getComputingManager();
		QSharedPointer<QuotationManager> getQuotationManager();
		QSharedPointer<ServerSourceManager> getServerSourceManager();
		//QMap<QUuid, QSharedPointer<OverridedQuotationComputingResult>> getOverridedQuotations(QUuid accountId);
		void clearData();
		void isGetSettingDataOrTraderData();
		void addPlaceResult(QSharedPointer<Common::Result::PlaceResult> placeResult);
		void setIsRefreshStatus(bool isRefresh);
		void loginQuotationServer(QStringList instrumentAndQuotePolicyIDs);
		void resetAlertLevel(const QStringList& accountIds, const QString& accountCodes);
		void addRequestQuote(QString id);
		void removeRequestQuote(QString id);
        void setisSleepStatus(bool sleepStatus);
        bool getisSleepStatus(){ return _sleepStatus; }
        bool isNoData();
        void refreshTradingData(QList<QUuid> transactionIds, QList<QUuid> orderIds);

	signals:
		void		getInitDataResultCompleted();
		
	public slots:
		void		getInitData();
		void		refreshInitData();
		void		getSettingAndTradingData(const QStringList &  accountIds);		
		void		initDataResultCompleted();
		void		settingDataResultCompleted();
		void		tradingDataResultCompleted();
		void		verifyTransactionResultCompleted();
		void		notifyReceivedSlot(const QDomElement& xmlNotify);
		void		quotationReceivedSlot(const QString quotations);
		void		calculateInstrumentSummarySlot();
		void        startGetSettingAndTradingDataSlot();
		void		placeResultCompletedSlot(QUuid id);
		void		getLogResultCompletedSlot();
        void		getQtAppLogResultCompletedSlot();
		void		refreshPhysicalButtonIsEnableSlot();
		void		refreshOpenOrderButtonIsEnableSlot();
		void		refreshOpenOrderButtonIsEnableByAccountSlot(const QString& accountCode, bool isShow);		
		void		computeAllSlot();
		void		computeAccountSlot(QUuid accountId);
		void		computeAllSlot(QList<QUuid> instrumentIds);
		void		computeQuotationSlot(QList<QUuid> instrumentIds);
		void		updateBOCountdownSlot();
		void		transactionExpireInspectorSlot();
		void		loginQuotationServerCompleted();
		void		changeEmployeeAccountListCurrencySlot(QUuid salesId, QUuid currencyId);
        void        changeSingleChannelSlot();
        void		refreshTradingDataResultCompletedSlot();
	};
}
#endif
