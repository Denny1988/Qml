#ifndef COMMON_SIGNALFORWARDER_H
#define COMMON_SIGNALFORWARDER_H

#include <QObject>
#include <QDomNode>
#include <QUuid>
#include <QDateTime>
#include <QMap>
#include "Common/Enum/CommonEnum.h"
#include "Common/Enum/PlaceEnum.h"
#include "Common/Enum/UIEnum.h"
#include "Common/Enum/InformationCenterEnum.h"
#include "Common/Enum/SettingOptionEnum.h"
#include "traderbusiness_global.h"

using namespace Common::Enum;

namespace Common
{
	class TRADERBUSINESS_EXPORT SignalForwarder : public QObject
	{
		Q_OBJECT
		private:
			SignalForwarder();
			~SignalForwarder();

		signals:
			void quotaionSignal(QUuid quotePolicyId, QUuid instrumentId, QString ask, QString bid, QDateTime timestamp);//ask是double类型, bid是double类型
			void changedCurrentAccountSignal(QUuid accountId);
			void addDataSignal(QUuid id, UpdateType::UpdateType updateType);
			void phaseChangedSignal(QUuid orderId, Phase::Phase phase);
			void isShowFunctionModelMenuTypeSignal(FunctionModelMenuType::FunctionModelMenuType functionModelMenuType, bool isShow);
			void calculateInstrumentSummarySignal();
			void orderExecutedSignal(QUuid id);
			void closeOrderForPendingSignal(QUuid id);
			void addPendingInventorySignal(QUuid id);
			void themeChangeSignal();
			void refreshSystemSignal();
			void clearSystemDataSignal();
			void customerNoAccountSignal();
			void needSelectAccountSignal();	
			void logoutSignal();
			void kickoutSignal();
			void closePageSignal();
			void refreshInstrumentUISignal();
			void needSelectInstrumentSignal();
			void isSelectAccountSignal(bool isSelect, QString accountId);
			void isSelectALLAccountSignal(bool isSelect, QList<QString> accountIds);
			void showOrHideOrderTabSignal(OrderTabType::OrderTabType orderTabType, bool isShow);
			void isShowSytemMenuSignal(MainSytemMenu::MainSytemMenu mainSytemMenu, bool isShow);
			void isShowSystemTitleBarSignal(MainSytemMenu::MainSytemMenu mainSytemMenu, bool isShow);		
			void instrumentStatusChangedSignal(QUuid instrumentId, int instrumentStatus);
			void accountMarginNotifySignal(QString accountCodes, QStringList resetAlertLevelAccountIds, QString resetAlertLevelAccountCodes);
			void addOrdersToChartSignal(QList<QUuid> orderIds);
			void removeOrdersToChartSignal(QMap<QUuid, QUuid>  orderIds);
			void enableFastTradeSignal(QUuid instrumentId, bool fastTrade);
			void refeshEnableFastTradeSignal();
			void adjustFontSizeSignal(int fontSizeStyleIndex, QString functionName, QString Style, bool isInit);
			void changeCloseByFIFOStateSignal(QUuid instrumentId, bool closeByFIFO);
			void changeQuickPlacingSpinBoxSignal(QUuid instrumentId, SpinBoxType::SpinBoxType spinBoxType, double value);
			void refreshPageFiFoStatusSignal(QList<QUuid> instrumentIds);
			void addNewsOrChatSignal(QUuid id, InformationCenterEnum::InformationCenterEnum informationCenterEnum);
			void showNewsOrChatSignal(QUuid id, InformationCenterEnum::InformationCenterEnum informationCenterEnum);
			void addPriceAlertSignal(QUuid id);
			void notifyPriceAlertSignal(QList<QUuid> hitItems, QList<QUuid> expiredItems);
			void refreshPriceAlertSignal();
			void addLogSignal(QUuid id);
            void addQtAppLogSignal(QUuid id);
			void interestFreeDaysNotifySignal();
			void refreshPhysicalButtonIsEnableSignal();
			void playSoundsSignal(SoundsEnum::SoundsEnum soundsEnum);
			void quoteCommandSignal(QString ask, QString bid, QString answerLot, QString quoteLot);
			void themeChangeForChartSignal(bool isBlack);
			void computeAllSignal();
			void computeAccountSignal(QUuid accountId);
			void accoutWillExpireOrAccountExpireNotifySignal(QString accountWillExpire, QString accountExpire);
			void computeQuotationSignal(QList<QUuid> instrumentIds);
			void computeAllSignal(QList<QUuid> instrumentIds);
			void computeAccountSignal();
			void changeStyleByConnectionStatusChangedSignal(bool isConnect);
			void clearSignal();
			void refreshQuotePolicySignal();
			void refreshChartSignal();
			void isHideInstalmentOrRepaymentButtonSignal(bool isHide, bool isInstalment);
			void showDisclaimerSignal(QUuid instrumentId, bool fastTrade);
			void customerTradeStatusNotifySignal(bool disallowTrade);
			void changeBOFrequencySignal(QUuid instrumentId, QString frequencyKey);
			void refreshBOFrequenciesSignal(QUuid instrumentId);
			void BOInfoToBOChartSignal(QUuid orderId, QString BOCountdown, QString BOFloatPLInString, QDateTime BOStartTime, QDateTime BOEndTime, QString BOExecutePrice, bool isUp);
			void showBORealtimeChartSignal(QUuid orderId, QUuid accountId, QUuid instrumentId);
			void hideBORealtimeChartSignal(QUuid orderId);
			void changeQuickPlacingWidgetSizeSignal(int value);
			void updateCommandSignal();
			void stopProcessBarSignal();
			void loginQuotationServerFaildSignal();
			void chartQuotationsSignal(const QDomElement& xmlNotify);
			void removeOrderSignal(QList<QUuid> ids);
			void addSubFundSignal(QString accountId, QString currencyId);
			void changeEmployeeAccountListCurrencySignal(QUuid salesId, QUuid currencyId);
			void instrumentPriceWaitConfirmOrderSignal(QUuid instrumentId, QString ask, QString bid, bool isStop);
            void changeUpDownColorSignal(QString upColor, QString downColor);
            void refreshSystemCompleteSignal();
            void quotationChangeSignal();
            void showLoginSignal();
            void changeMarketInformationSignal(int MarketInformationOption);
            void refreshStyleSignal();
            void changeChartTransparentForMouseEventSignal(bool enable);
            void todayExecuteOrderSignal(QUuid id);
            void ordersChangedSignal(QList<QUuid> orderIds);
            void refreshSystemByQueryForAddAccountSignal(QString accountCode, QString accountName);            
            void closeMobileHorizontalScreenChartSignal();
            void clickInstrumentSwitchChartSignal(QUuid instrumentId);
            void refreshTradingDataStatusSignal(bool isSuccess);

		public:
			static SignalForwarder*	getInstance();
			void sendQuotaionSignal(QUuid quotePolicyId, QUuid instrumentId, QString ask, QString bid, QDateTime timestamp);
			void sendChangedCurrentAccountSignal(QUuid accountId);
			void sendAddDataSignal(QUuid id, UpdateType::UpdateType updateType);
			void sendPhaseChangedSignal(QUuid orderId, Phase::Phase phase);			
			void sendIsShowFunctionModelMenuTypeSignal(FunctionModelMenuType::FunctionModelMenuType functionModelMenuType, bool isShow);
			void sendCalculateInstrumentSummarySignal();
			void sendOrderExecutedSignal(QUuid id);
			void sendCloseOrderForPendingSignal(QUuid id);
			void sendAddPendingInventorySignal(QUuid id);
			void sendThemeChange(QString themeName);
			void sendRefreshSystemSignal();
			void sendClearSystemDataSignal();
			void sendCustomerNoAccountSignal();
			void sendNeedSelectAccountSignal();
			void sendKickoutSignal();
			void sendClosePageSignal();
			void sendRefreshInstrumentUISignal();
			void sendNeedSelectInstrumentSignal();
			void sendIsSelectAccountSignal(bool isSelect, QString accountId);
			void sendIsSelectALLAccountSignal(bool isSelect, QList<QString> accountIds);
			void sendShowOrHideOrderTabSignal(OrderTabType::OrderTabType orderTabType, bool isShow);
			void sendIsShowSytemMenuSignal(MainSytemMenu::MainSytemMenu mainSytemMenu, bool isShow);
			void sendIsShowSystemTitleBarSignal(MainSytemMenu::MainSytemMenu mainSytemMenu, bool isShow);
			void sendInstrumentStatusChangedSignal(QUuid instrumentId, int instrumentStatus);
			void sendAccountMarginNotifySignal(QString& accountCodes, QStringList& resetAlertLevelAccountIds, QString& resetAlertLevelAccountCodes);
			void sendAddOrdersToChartSignal(QList<QUuid> orderIds);
			void sendRemoveOrdersToChartSignal(QMap<QUuid, QUuid> orderIds);
			void sendEnableFastTradeSignal(QUuid instrumentId, bool fastTrade);
			void sendRefeshEnableFastTradeSignal();
			void sendAdjustFontSizeSignal(int fontSizeStyleIndex, QString functionName, QString Style, bool isInit = false);
			void sendChangeCloseByFIFOStateSignal(QUuid instrumentId, bool closeByFIFO);
			void sendChangeQuickPlacingSpinBoxSignal(QUuid instrumentId, SpinBoxType::SpinBoxType spinBoxType, double value);
			void sendRefreshPageFiFoStatusSignal(QList<QUuid> instrumentIds);
			void sendAddNewsOrChatSignal(QUuid id, InformationCenterEnum::InformationCenterEnum informationCenterEnum);
			void sendShowNewsOrChatSignal(QUuid id, InformationCenterEnum::InformationCenterEnum informationCenterEnum);
			void sendAddPriceAlertSignal(QUuid id);
			void sendNotifyPriceAlertSignal(QList<QUuid> hitItems, QList<QUuid> expiredItems);
			void sendRefreshPriceAlertSignal();
			void sendAddLogSignal(QUuid id);
            void sendAddQtAppLogSignal(QUuid id);
			void sendInterestFreeDaysNotifySignal();
			void sendRefreshPhysicalButtonIsEnable();
			void sendPlaySoundsSignal(SoundsEnum::SoundsEnum soundsEnum);		
			void sendQuoteCommandSignal(QString ask, QString bid, QString answerLot, QString quoteLot);
			void sendComputeAllSignal();
			void sendComputeAccountSignal(QUuid accountId);
			void sendAccoutWillExpireOrAccountExpireNotifySignal(QString accountWillExpire, QString accountExpire);
			void sendComputeQuotationSignal(QList<QUuid> instrumentIds);
			void sendComputeAllSignal(QList<QUuid> instrumentIds);
			void sendComputeAccountSignal();
			void sendChangeStyleByConnectionStatusChangedSignal(bool isConnect);
			void sendClearSignal();
			void sendRefreshQuotePolicySignal();
			void sendRefreshChartSignal();
			void sendIsHideInstalmentOrRepaymentButtonSignal(bool isHide, bool isInstalment);
			void sendShowDisclaimerSignal(QUuid instrumentId, bool fastTrade);
			void sendCustomerTradeStatusNotifySignal(bool disallowTrade);
			void sendChangeBOFrequencySignal(QUuid instrumentId, QString frequencyKey);
			void sendRefreshBOFrequenciesSignal(QUuid instrumentId);
			void sendBOInfoToBOChartSignal(QUuid orderId, QString BOCountdown, QString BOFloatPLInString, QDateTime BOStartTime, QDateTime BOEndTime, QString BOExecutePrice, bool isUp);
			void sendShowBORealtimeChartSignal(QUuid orderId, QUuid accountId, QUuid instrumentId);
			void sendHideBORealtimeChartSignal(QUuid orderId);
			void sendChangeQuickPlacingWidgetSizeSignal(int value);
			void sendUpdateCommandSignal();
			void sendStopProcessBarSignal();
			void sendLoginQuotationServerFaildSignal();
			void sendChartQuotationsSignal(const QDomElement& xmlNotify);
			void sendRemoveOrderSignal(QList<QUuid> ids);
			void sendAddSubFundSignal(QString accountId, QString currencyId);
			void sendChangeEmployeeAccountListCurrencySignal(QUuid salesId, QUuid currencyId);
			void sendInstrumentPriceWaitConfirmOrderSignal(QUuid instrumentId, QString ask, QString bid, bool isStop);
            void sendChangeUpDownColorSignal(QString upColor, QString downColor);
            void sendRefreshSystemCompleteSignal();
            void sendQuotationChangeSignal();
            void sendShowLoginSignal();
            void sendChangeMarketInformationSignal(int MarketInformationOption);
            void sendRefreshStyleSignal();
            void sendTodayExecuteOrderSignal(QUuid id);
            void sendOrdersChangedSignal(QList<QUuid> orderIds);
            void sendRefreshSystemByQueryForAddAccountSignal(QString accountCode, QString accountName);            
            void sendCloseMobileHorizontalScreenChartSignal();            
            void sendClickInstrumentSwitchChartSignal(QUuid instrumentId);
            void sendRefreshTradingDataStatusSignal(bool isSuccess);

#ifdef RUNTIME_OS_MACX //add for bugs
            void sendChangeChartTransparentForMouseEventSignal(bool enable);
#endif
		private:
			static	SignalForwarder*	_instance; 
	};
}
#endif	  

