#ifndef BUSINESS_SETTINGMANAGER_H
#define BUSINESS_SETTINGMANAGER_H
#include <QDomNode>
#include "Business/Model/Setting/BusinessAccount.h"
#include "Business/Model/Setting/SystemSetting.h"
#include "Business/Model/Setting/TradeDay.h"
#include "Business/Model/Setting/InstrumentParameter.h"
#include "Business/Model/Setting/OpenOrderListColumnSetting.h"
#include "Business/Model/Setting/InstrumentColumnSetting.h"
#include "Business/Model/Setting/PaymentInstructionIntegralitySetting.h"
#include "Business/Model/Setting/CalculateQuotationDefaultSetting.h"
#include "Business/Model/Setting/Currency.h"
#include "Business/Model/Setting/CurrencyRate.h"
#include "Business/Model/Setting/ScrapInstrument.h"
#include "Business/Model/Setting/DeliveryHoliday.h"
#include "Business/Model/Setting/InterestRate.h"
#include "Business/Model/Setting/PricingPolicyDetail.h"
#include <QSharedPointer>
#include "Common/Struct/SettingStruct.h"
#include "Business/Model/Setting/SettingDataBase.h"
#include "Common/Enum/InstrumentEnum.h"
#include "Business/Model/Chat.h"
#include "Business/Model/News.h"
#include "Business/Model/PriceAlert.h"
#include "Business/Model/Log.h"
#include "Common/Enum/InformationCenterEnum.h"
#include "Common/Struct/InformationStruct.h"
#include "Business/Model/Trading/MarketingGroupFund.h"
#include "traderbusiness_global.h"

using namespace Common::Enum;
using namespace Business::Model;


namespace Business
{	
	class TRADERBUSINESS_EXPORT SettingManager
	{		
		friend class InitializeManager;
		friend class ServerSourceManager;
		friend class TradingManager;

	private:	
		QMap<QUuid, QSharedPointer<Setting::BusinessAccount>> _accounts;
		QSharedPointer<Setting::SystemSetting>  _systemSetting;
		QMap<QString, QUuid> _quotePolicyAccountMapping;
		QMap<QUuid, QMap<QUuid, QString>> _instrumentQuotePolicyMappings;
		QSharedPointer<Setting::TradeDay>  _tradeDay;	
		QMap<QUuid, QSharedPointer<Setting::InstrumentParameter>> _instrumentParameters;
		QMap<QUuid, QList<InstrumentStatus::InstrumentStatus>> _instrumentStatus;
		QMap<QUuid, QSharedPointer<Chat>> _chats;
		QMap<QUuid, QSharedPointer<News>> _newses;
		QMap<QUuid, QSharedPointer<PriceAlert>> _priceAlerts;
		QMap<QUuid, QSharedPointer<Log>> _logs;
        QMap<QUuid, QSharedPointer<Log>> _qtAppLogs;
		QList<InformationStruct> _informationStructs;

		QMap<QUuid, QSharedPointer<Setting::Currency>> _currencies;
		QMap<QUuid, QMap<QUuid, QSharedPointer<Setting::CurrencyRate>>> _currencyRates;
		QMap<QUuid, QSharedPointer<Setting::ScrapInstrument>> _scrapInstruments;
		QMap<QUuid, QSharedPointer<Setting::InterestRate>> _interestRates;
		QSharedPointer<Setting::PaymentInstructionIntegralitySetting> _paymentInstructionIntegralitySetting;
		QList<QSharedPointer<Setting::DeliveryHoliday>> _deliveryHolidays;
		QMap<QUuid, QSharedPointer<Setting::CalculateQuotationDefaultSetting>> _calculateQuotationDefaultSettings;
        QMap<QString, QSharedPointer<Trading::MarketingGroupFund>> _marketingGroupFunds;//SaleId+Account.CurrenyId

	private:
		void addAccount(const QUuid& id, const QSharedPointer<Setting::BusinessAccount>& acccount);
		QSharedPointer<Setting::SettingDataBase> getInstrumentChildItem(const QUuid& accountId, const QUuid& instrumentId, QString methodName);
		QSharedPointer<Setting::SettingDataBase> getAccountChildItem(const QUuid& accountId, QString methodName);

	public:
		SettingManager();
		void updateSettingData(const QDomNode& xmlSettingData);
		void setQuotePolicyAccountMappingAndAccountExpireNotify(bool isInit);
		void sendUpdateQuotePolicy();

		QSharedPointer<Setting::BusinessAccount> getAccount(const QUuid& id) const;
		bool containAccount(const QUuid& id);
		QSharedPointer<Setting::BusinessAccount> getFirstAccount();
		QSharedPointer<Setting::MarginSetting> getMarginSetting(const QUuid& accountId);
		QSharedPointer<Setting::LeverageSetting> getLeverageSetting(const QUuid& accountId);
		QSharedPointer<Setting::BusinessInstrument> getInstrument(const QUuid& accountId, const QUuid& instrumentId);
		QSharedPointer<Setting::InterestRate> getInterestRate(const QUuid& accountId, const QUuid& instrumentId);
		QSharedPointer<Setting::InterestRate> getPrepaymentInterestRate(const QUuid& accountId, const QUuid& instrumentId);
		QSharedPointer<Setting::DeliveryCharge> getDeliveryCharge(const QUuid& accountId, const QUuid& instrumentId);
		QSharedPointer<Setting::PlaceSetting> getPlaceSetting(const QUuid& accountId, const QUuid& instrumentId);
		QSharedPointer<Setting::PlaceSettingDetail> getPlaceSettingDetail(const QUuid& accountId, const QUuid& instrumentId, Common::Enum::OrderType::OrderType orderType);
		QSharedPointer<Setting::CalculateSetting> getCalculateSetting(const QUuid& accountId, const QUuid& instrumentId);
		QSharedPointer<Setting::MovePipsInfoSetting> getMovePipsInfoSetting(const QUuid& accountId, const QUuid& instrumentId);
		QSharedPointer<Setting::VolumeNecessary> getVolumeNecessary(const QUuid& accountId, const QUuid& instrumentId);
		QSharedPointer<Setting::InstalmentSetting> getInstalmentSetting(const QUuid& accountId, const QUuid& instrumentId);

		QSharedPointer<Setting::SystemSetting> getSystemSetting();

		QMap<QUuid, QSharedPointer<Common::Struct::SettingQuotePolicy>> getQuotePolicies(QList<QUuid> instrumentIds);
		QList<QSharedPointer<Common::Struct::SettingQuotePolicy>> getQuotePolicies(QUuid instrumentId);

		QList<QUuid> getInstrumentIds(const QUuid& accountId);
		void initSystemSetting(const QDomNode& xmlInitializeData);

		int getAccountCount(){ return _accounts.count();}
		QUuid getAccountIdByQuotePolicyId(QUuid quotePolicyId, QUuid instrumentId);
		bool isAllowChangePassword();
		QList<QSharedPointer<Common::Struct::SettingAccount>> getAccountsByMarginPin();
		QList<QSharedPointer<Common::Struct::SettingAccount>> getAccountsByModifyTelephoneIdentificationCode();
		bool getNeedDeal();

		QSharedPointer<Setting::TradeDay>  getTradeDay();
		QSharedPointer<Setting::InstrumentParameter>  getInstrumentParameter(QUuid instrumentId);
		QList<InstrumentStatus::InstrumentStatus>  getInstrumentStatus(QUuid instrumentId);

		void updateChatOrNews(const QDomNode& xmlInitializeData);
		void updatePriceAlerts(const QDomNode& xmlInitializeData);
		void addLog(QUuid id, const QDomNode& xmlNotifyData);
        void addQtAppLog(QUuid id, const QDomNode& xmlNotifyData);
        bool isContainLog(QUuid id, const QDomNode& xmlNotifyData, bool isQtApp);
		QList<QSharedPointer<Model::Chat>> getChats();
		QList<QSharedPointer<Model::News>> getNewses();	
		void setIsRead(QUuid id, bool value, InformationCenterEnum::InformationCenterEnum informationCenterEnum);
		void removeNewsOrChats(QUuid id, InformationCenterEnum::InformationCenterEnum informationCenterEnum);
		void setContent(QUuid id, QString value, InformationCenterEnum::InformationCenterEnum informationCenterEnum);
		QSharedPointer<Model::Chat> getChat(QUuid id);
		QSharedPointer<Model::News> getNews(QUuid id);
		QList<QSharedPointer<Model::PriceAlert>> getPriceAlerts();
		QList<QSharedPointer<Model::PriceAlert>> getPriceAlerts(QList<QUuid> ids);
		QSharedPointer<Model::PriceAlert> getPriceAlert(QUuid id);		
		void deletePriceAlert(QUuid id);
		void deletePriceAlerts(QList<QUuid> ids);
		void updatePriceAlert(QUuid id, QSharedPointer<Model::PriceAlert> priceAlert);
		void addPriceAlert(QUuid id, QSharedPointer<Model::PriceAlert> priceAlert);
		int getPriceAlertCountByInstrument(QUuid instrumentId);
		bool hasDuplicatedPriceAlert(QUuid instrumentId, int condition, QString Price, QUuid id);
		void changeAlertPrice(QUuid id, QString price);
		bool isExistAlertPrice(QUuid id) { return _priceAlerts.contains(id); }
		QSharedPointer<Model::Log> getLog(QUuid id);
		QList<QSharedPointer<Model::Log>> getLogs();
        QSharedPointer<Model::Log> getQtAppLog(QUuid id);
        QList<QSharedPointer<Model::Log>> getQtAppLogs();
        void deleteLog(QUuid id);
		QList<InformationStruct> getInformationStructs() { return _informationStructs; }
		void deleteInformationStruct(QUuid id);
		bool isShowSptReconfirm();
		QMap<QUuid, QMap<QUuid, QString>> getInstrumentQuotePolicyMappings() { return _instrumentQuotePolicyMappings; }
		bool isExistQuotePolicyInInstrumentMapping(QUuid instrumentId, QUuid quotePolicyId);
		bool isExistQuotePolicyAccountMapping(QUuid instrumentId, QUuid quotePolicyId);
		bool setCurrentAccountAndQuotePolicyId();
		void updateLeverage(QUuid accountId, int leverage);
		bool isMutilQuotePolicy();
		bool isContainQuotePolicy(QUuid quotePolicyId);
		bool canSubmitMagin();
		bool hasConsumeCode();
		bool isShowMagin();
		bool checkIsBOInstrument(QUuid instrumentId);
		QList<QUuid> getPlaceAccountIdList(QUuid instrumentId);
		void updatCurrencyRates(const QDomNode& xmlData);

		QSharedPointer<Setting::Currency> getOrAddCurrency(const QUuid& id);
		QSharedPointer<Setting::Currency> getCurrency(const QUuid& id) const;
		void addCurrency(const QUuid& id, const QSharedPointer<Setting::Currency>& currency);
		QMap<QUuid, QSharedPointer<Setting::Currency>> getCurrencies() const;
		QSharedPointer<Setting::CurrencyRate> getOrAddCurrencyRate(const QUuid& sourceCurrencyId, const QUuid& targetCurrencyId);
		QSharedPointer<Setting::CurrencyRate> getCurrencyRate(const QUuid& sourceCurrencyId, const QUuid& targetCurrencyId) const;
		void addCurrencyRate(const QUuid& sourceCurrencyId, const QUuid& targetCurrencyId, const QSharedPointer<Setting::CurrencyRate>& currencyRate);
		QSharedPointer<Setting::ScrapInstrument> getOrAddScrapInstrument(const QUuid& id);
		QSharedPointer<Setting::ScrapInstrument> getScrapInstrument(const QUuid& id) const;
		void addScrapInstrument(const QUuid& id, const QSharedPointer<Setting::ScrapInstrument>& scrapInstrument);
		QSharedPointer<Setting::CalculateQuotationDefaultSetting> getOrAddCalculateQuotationDefaultSetting(const QUuid& instrumentId);
		QSharedPointer<Setting::DeliveryHoliday> getDeliveryHoliday(QDate deliveryTime);
		QSharedPointer<Setting::PaymentInstructionIntegralitySetting> getPaymentInstructionIntegralitySetting() { return _paymentInstructionIntegralitySetting; }
		void setCurrencyRate(const QDomNode& xmlData);
		void AddOrUpdateCalculateQuotationDefaultSetting(QSharedPointer<Setting::CalculateQuotationDefaultSetting>& calculateQuotationDefaultSetting);
		QSharedPointer<Setting::InterestRate>& getInterestRateSource(const QUuid& id);
		bool isAllowTraderResetAlertLevel(int forbiddenAlert);

		bool isTradeOpenedAccount(QUuid accountId, int counterPartyType);
		bool isContainFromCurrentUser(QUuid accountId);
		QList<QSharedPointer<Trading::MarketingGroupFund>> getMarketingGroupFunds() { return  _marketingGroupFunds.values(); }
        QSharedPointer<Trading::MarketingGroupFund> getMarketingGroupFund(QString saleIdAndAccountCurrenyId);
		QString getInstrumentUnit(const QUuid& accountId, const QUuid& instrumentId);
        QString getInstrumentUnitNoUseShowWeightOnly(const QUuid& accountId, const QUuid& instrumentId);
		QString getInstrumentUnitByMultiAccount(const QUuid& instrumentId);
		bool isShowWeightOnly();

        QSharedPointer<Setting::BusinessAccount> getCopyAccount(const QUuid& id);
        QList<QSharedPointer<Setting::BusinessAccount>> getAccounts();
        QList<QSharedPointer<Setting::BusinessAccount>> getAccountsForBankAccount();
        QUuid getFirstAccountIdForClientRegistration();
	};

}
#endif
