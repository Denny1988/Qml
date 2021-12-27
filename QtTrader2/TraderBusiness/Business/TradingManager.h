#ifndef BUSINESS_TRADINGMANAGER_H
#define BUSINESS_TRADINGMANAGER_H
#include <QDomNode>
#include "Business/Model/Trading/Transaction.h"
#include "Business/Model/Trading/TradingOrder.h"
#include "Business/Model/Trading/DeliveryRequest.h"
#include "Business/Model/Trading/ScrapDeposit.h"
#include "Business/Model/Computing/ComputingResult.h"
#include "Business/Model/Trading/Fund.h"
#include "Business/Model/Trading/SubFund.h"
#include "Business/Model/Trading/InventoryData.h"
#include "Business/Model/Trading/PendingInventoryData.h"
#include "Business/Model/Trading/ShortSellData.h"
#include "Business/Model/Trading/InstrumentSummary.h"
#include "Business/Model/Trading/InterestFreeDay.h"
#include <QMap>
#include <QSharedPointer>
#include <QXmlStreamReader>
#include <QTimer>
#include <QMutex>
#include "traderbusiness_global.h"
#include <QStringList>
#include <QDateTime>

using namespace Business::Model::Computing;
using namespace Business::Model::Trading;

namespace Business
{
	struct  AccountCardResult
	{
	public:
		QUuid accountId;
		QString accountName;
		//bool isSelect;
		QString salesCode;
		bool isNoShowAccountStatus;
		QSharedPointer<Fund> fund;
		QMap<QUuid, QSharedPointer<SubFund>> subFund;
	};
	class SettingManager;
	class ComputingManager;
	class InitializeManager;
	class TRADERBUSINESS_EXPORT TradingManager
	{
	public:
		TradingManager(QSharedPointer<SettingManager> settingManager, QSharedPointer<ComputingManager> computingManager, QSharedPointer<InitializeManager> initializeManager);
		void startSetInitData();

	private:
		QMap<QUuid,QSharedPointer<Business::Model::Trading::Transaction>> _transactions;
		QMap<QUuid,QSharedPointer<TradingOrder>> _orders;
		QMap<QUuid,QSharedPointer<TradingOrder>> _removeOpenOrders;
		QMap<QUuid,QSharedPointer<TradingOrder>> _workOrders;
		QMap<QUuid,QSharedPointer<DeliveryRequest>> _deliveryRequests;
		QMap<QUuid,QSharedPointer<ScrapDeposit>> _scrapDeposits;
		QMap<QUuid,QSharedPointer<InstrumentSummary>> _instrumentSummaries;
		QMap<QUuid,QSharedPointer<Fund>> _funds;
		QMap<QUuid,QMap<QUuid, QSharedPointer<SubFund>>> _subFunds;
		QMap<QString,QSharedPointer<InventoryData>> _inventories;//AccountId+InstrumentId
		QMap<QUuid,QSharedPointer<PendingInventoryData>> _pendingInventories;
		QMap<QUuid,QSharedPointer<ShortSellData>> _shortSells;
		QSharedPointer<SettingManager> _settingManager;
		QSharedPointer<ComputingManager> _computingManager;
		QSharedPointer<InitializeManager> _initializeManager;
		QList<QSharedPointer<InterestFreeDay>> _interestFreeDays;
		QMap<QUuid,QSharedPointer<TradingOrder>> _boOrders;
		QList<QUuid> _pendingTransactionIds;
		static QMutex  m_Mutex;
		QString _accountCodes;
		QStringList _resetAlertLevelAccountIds;
		QString _resetAlertLevelAccountCodes;
        QDateTime _loginTime;

    public:
        QList<QUuid> TransactionIds;
        QList<QUuid> OrderIds;
	
	public:
		bool initTradingData(const QByteArray&  tradingDataByteArray);
        bool refreshTradingData(const QByteArray&  tradingDataByteArray);
		void updateTradingDataByNotifyReceived(const QDomNode& xmlTradingData);
		QSharedPointer<AccountCardResult> getAccountCardResult(const QUuid& accountId);
		QSharedPointer<Fund> getOrAddFund(const QUuid& accountId);
		QSharedPointer<Fund> getFund(const QUuid& accountId);
		QSharedPointer<SubFund> getOrAddSubFund(const QUuid& accountId, const QUuid& currencyId);
		QMap<QUuid, QSharedPointer<SubFund>> getSubFunds(const QUuid& accountId);
		QSharedPointer<SubFund> getSubFund(const QUuid& accountId, const QUuid& currencyId);
		
		QSharedPointer<TradingOrder> getOrAddOrder(const QUuid& id);
		QSharedPointer<TradingOrder> getOrAddOrderByUpdateCommand(const QUuid& id);
		QSharedPointer<DeliveryRequest> getOrAddDeliveryRequest(const QUuid& id);
		QSharedPointer<ScrapDeposit> getOrAddScrapDeposit(const QUuid& id);
		QSharedPointer<Business::Model::Trading::Transaction> getOrAddTransaction(const QUuid& id);	
		QMap<QUuid,QSharedPointer<TradingOrder>> getOpenOrders(QList<QUuid> accountIds);
		QMap<QUuid,QSharedPointer<TradingOrder>> getOpenOrders(QUuid instrumentId);
		QList<QSharedPointer<TradingOrder>> getOpenOrders();
		QList<QSharedPointer<TradingOrder>> getOpenOrdersNotContainPhysical();
		QList<QSharedPointer<TradingOrder>> getBOOrders();
        QList<QSharedPointer<TradingOrder>> getPlacedOrders(QUuid instrumentId);
		QSharedPointer<TradingOrder> getBOOrder(const QUuid& id) const;
        QMap<QUuid,QSharedPointer<TradingOrder>> getOpenOrders(QUuid accountId, QUuid instrumentId);
		QMap<QUuid,QSharedPointer<TradingOrder>> getOpenOrdersByQuote(QUuid accountId, QUuid instrumentId);
		QMap<QUuid,QSharedPointer<TradingOrder>> getWorkOrders(QList<QUuid> accountIds);
		QMap<QUuid,QSharedPointer<TradingOrder>> getWorkOrders();
		QMap<QUuid,QSharedPointer<TradingOrder>> getWorkOrders(QUuid accountId, QUuid instrumentId);
        QList<QSharedPointer<TradingOrder>> getWorkOrdersBySortDateTime();
        QList<QSharedPointer<TradingOrder>> getWorkOrdersBySubmitTime();
        QList<QSharedPointer<TradingOrder>> getWorkOrdersBySubmitTime(QUuid accountId, QUuid instrumentId);
        QList<QSharedPointer<TradingOrder>> getPlacedOrdersBySubmitTime();
		QSharedPointer<TradingOrder> getWordOrder(const QUuid& id) const;
		QList<QSharedPointer<TradingOrder>> getOrders(QList<QUuid> accountIds);
		QList<QSharedPointer<TradingOrder>> getOrders(QUuid accountId, QUuid instrumentId);
		QList<QSharedPointer<TradingOrder>> getOrdersByInstrumentId(QUuid instrumentId);
		QList<QSharedPointer<TradingOrder>> getOrders();
		QList<QSharedPointer<TradingOrder>> getOrdersByAccountId(QUuid accountId);
		QSharedPointer<TradingOrder> getOrder(const QUuid& id) const;
		QSharedPointer<InstrumentSummary> getInstrumentSummary(const QUuid& id) const;
		QMap<QUuid,QSharedPointer<InstrumentSummary>> getInstrumentSummaries();
		QList<QUuid> getOrderIdByBuySell(QUuid accountId, QUuid instrumentId, BuySell::BuySell buySell);
		QMap<QUuid,QSharedPointer<TradingOrder>> getUnConfirmPendingOrders();
        QList<QSharedPointer<TradingOrder>> getUnConfirmPendingOrdersBySortDateTime();
		QList<QSharedPointer<TradingOrder>> getBOOrders(QUuid accountId, QUuid instrumentId);

		QSharedPointer<InventoryData> addInventory(QUuid orderId);
		QSharedPointer<ShortSellData> addShortSell(QUuid orderId);
		QMap<QString,QSharedPointer<InventoryData>> getInventories();
		QSharedPointer<InventoryData> getInventory(QUuid accountId, QUuid instrumentId) const;
		QSharedPointer<PendingInventoryData> getPendingInventory(QUuid id) const;
		QMap<QUuid,QSharedPointer<PendingInventoryData>> getPendingInventories();
        QMap<QUuid,QSharedPointer<PendingInventoryData>> getPendingInventories(QUuid accountId, QUuid instrumentId);
		QMap<QUuid,QSharedPointer<ShortSellData>> getShortSells() ;
		QSharedPointer<ShortSellData> getShortSell(QUuid orderId) const;
		bool isShowShortSelling(double lot, QString key);
		void refreshShortSell();
		void refreshInventory();
		void refreshPendingInventory();
		void calculateInstrumentSummary();
		void calculateInstrumentSummaryByInstrument(QUuid instrumentId);
		QList<QUuid> getShowFloatingByCurrency();
		void refreshPhysicalData();
		void refreshPhysicalDataByInstruments(QList<QUuid> instrumentIds);
		void refreshOpenOrderButtonIsEnable();
		void refreshPhysicalButtonIsVisible();
		void setButtonIsEnable(QSharedPointer<TradingOrder> order);
		void refreshOpenOrderButtonIsEnableByAccount(const QString& accountCode, bool isShow);

		void addTransaction(QSharedPointer<Common::Struct::Transaction> placingTransaction);
		void addOrder(QSharedPointer<Common::Struct::Order> placingOrder, QSharedPointer<Common::Struct::Transaction> placingTransaction);
		QSharedPointer<TradingOrder> updateOrderInfo(QUuid id, Phase::Phase phase, QString cancelReason);
		QList<QUuid> clearAllForWork();
		bool clearOrderForWork(QUuid id);
		bool hasOpentContract(QUuid accountId, QUuid currencyId);	

		bool canBeDeselected(QUuid instrumentId);
		void deleteInventory(QString key);
		void deleteTradingOrder(QUuid id);

		bool canBeRemoved(QUuid instrumentId);
		bool canPlaceOrder(QUuid quotePolicyId, QUuid instrumentId);
		bool canBackPlaceOrder(QUuid quotePolicyId, QUuid instrumentId);
		bool isTrading();

		bool hasSufficientUsableMargin(QUuid accountId, QUuid currencyId, QString amount, QSharedPointer<Business::Model::Setting::CurrencyRate> currencyRate);
		int getOpenOrdersCount(QUuid accountId, QUuid instrumentId);

		bool canPlaceFifoOrder(QUuid accountId, QUuid instrumentId);
		QList<QSharedPointer<InterestFreeDay>> getInterestFreeDays() { return _interestFreeDays; }

		void refreshPhysicalButtonIsEnable();
		void deletePendingInventoryData(QUuid id);
		void deleteOrderFromCommand(QUuid id);
		bool hasExistInventoryItemData(QString key, QUuid id);
		void updateBOCountdown();
		void cancelExpired(QList<QUuid> tranIds);
		QList<QUuid> getTransactionsMayExpried();

		void updateBussnessAccountInstrumentByUpdateCommand();
		void computeAllMarketingGroupFund();
		void changeMarketingGroupFund(QUuid saleId, QUuid currencyId);
        void computeMarketingGroupFund(QString saleIdAndAccountCurrencyId);
        QStringList getOrderConfirmationParameter(const QList<QUuid>&  executedOrderIds);
        QString getAdministrativeCharge(QUuid accountId, QUuid instrumentId, bool isOpen, QDecNumber lot, QDecNumber price);
        bool canPlaceCloseOrder(QUuid accountId, QUuid instrumentId, bool isPending, bool isBuy);
        void setLoginTime(QDateTime value){ _loginTime = value;}

	private:
		void setOrderMidifyRelation();
		void refreshPendingInventoryFromDeliveryRequest(QSharedPointer<DeliveryRequest> deliveryRequest);
		void refreshPendingInventoryFromScrapDeposit(QSharedPointer<ScrapDeposit> scrapDeposit);
		void readAccountElement(QXmlStreamReader& xmlStreamReader, QString& accountCodes, QStringList& resetAlertLevelAccountIds, QString& resetAlertLevelAccountCodes);
        void readSubFundElement(QXmlStreamReader& xmlStreamReader, QUuid&  accountId, QUuid&  accountCurrencyId, QDecNumber& creditAmount);
		void readDeliveryRequestElement(QXmlStreamReader& xmlStreamReader, QUuid&  accountId);
		void readScrapDepositElement(QXmlStreamReader& xmlStreamReader, QUuid&  accountId);
		void readTransactionRequestElement(QXmlStreamReader& xmlStreamReader, QUuid&  accountId, QSharedPointer<Business::Model::Setting::BusinessAccount>& settingAccount);
		void readOrderElement(QXmlStreamReader& xmlStreamReader, QSharedPointer<Model::Trading::Transaction>& transaction, QSharedPointer<Business::Model::Setting::BusinessInstrument>& settingInstrument);
		void setInitData();
	};

}
#endif
