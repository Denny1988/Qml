#include "Common/SignalForwarder.h"
#include "Common/TraderBaseInfo.h"

namespace Common
{
	SignalForwarder* SignalForwarder::_instance=Q_NULLPTR;
	SignalForwarder* SignalForwarder::getInstance()
	{
			if (Q_NULLPTR==_instance)
			{
				_instance = new SignalForwarder;
			}
			return _instance;
	}

	SignalForwarder::SignalForwarder()
	{
		qRegisterMetaType<MainSytemMenu::MainSytemMenu>("MainSytemMenu::MainSytemMenu");
		qRegisterMetaType<UpdateType::UpdateType>("UpdateType::UpdateType");
		qRegisterMetaType<Phase::Phase>("Phase::Phase");
		qRegisterMetaType<QList<QString>>("QList<QString>");
		qRegisterMetaType<QList<QUuid>>("QList<QUuid>");
		qRegisterMetaType<QMap<QUuid, QUuid>>("QMap<QUuid, QUuid>");		
		qRegisterMetaType<InformationCenterEnum::InformationCenterEnum>("InformationCenterEnum::InformationCenterEnum");		
		qRegisterMetaType<OrderTabType::OrderTabType>("OrderTabType::OrderTabType");
		qRegisterMetaType<SoundsEnum::SoundsEnum>("SoundsEnum::SoundsEnum");
		qRegisterMetaType<SpinBoxType::SpinBoxType>("SpinBoxType::SpinBoxType");
		qRegisterMetaType<FunctionModelMenuType::FunctionModelMenuType>("FunctionModelMenuType::FunctionModelMenuType");
		qRegisterMetaType<QList<QString>>("QList<QString>");
		qRegisterMetaType<QStringList>("QStringList");
	}
	
	SignalForwarder::~SignalForwarder()
	{

	}
	
	void SignalForwarder::sendQuotaionSignal(QUuid quotePolicyId, QUuid instrumentId, QString ask, QString bid, QDateTime timestamp)
	{
		emit quotaionSignal(quotePolicyId, instrumentId, ask, bid, timestamp);
	}

	void SignalForwarder::sendChangedCurrentAccountSignal(QUuid accountId)
	{
		emit changedCurrentAccountSignal(accountId);
	}

	void SignalForwarder::sendAddDataSignal(QUuid id, UpdateType::UpdateType updateType)
	{
		emit addDataSignal(id, updateType);
	}
		
	void SignalForwarder::sendPhaseChangedSignal(QUuid orderId, Phase::Phase phase)
	{
		emit phaseChangedSignal(orderId, phase);
	}

	void SignalForwarder::sendIsShowFunctionModelMenuTypeSignal(FunctionModelMenuType::FunctionModelMenuType functionModelMenuType, bool isShow)
	{
		if(functionModelMenuType == FunctionModelMenuType::Order)Common::TraderBaseInfo::getInstance()->setIsShowOrderPage(isShow);
		emit isShowFunctionModelMenuTypeSignal(functionModelMenuType, isShow);
	}

	void SignalForwarder::sendCalculateInstrumentSummarySignal()
	{
		emit calculateInstrumentSummarySignal();
	}

	void SignalForwarder::sendOrderExecutedSignal(QUuid id)
	{
		emit orderExecutedSignal(id);
	}
			
	void SignalForwarder::sendCloseOrderForPendingSignal(QUuid id)
	{
		emit closeOrderForPendingSignal(id);
	}

	void SignalForwarder::sendAddPendingInventorySignal(QUuid id)
	{
		emit addPendingInventorySignal(id);
	}

	void SignalForwarder::sendThemeChange(QString themeName)
	{
		emit themeChangeSignal();
		emit themeChangeForChartSignal(themeName == "BlackTheme" ? true : false);
	}

	void SignalForwarder::sendRefreshSystemSignal()
	{
		Common::TraderBaseInfo::getInstance()->setIsRefreshSystem(true);
		emit refreshSystemSignal();
	}
	
	void SignalForwarder::sendClearSystemDataSignal()
	{
		emit clearSystemDataSignal();
		this->sendClearSignal();
	}

	void SignalForwarder::sendCustomerNoAccountSignal()
	{
		emit customerNoAccountSignal();
	}

	void SignalForwarder::sendNeedSelectAccountSignal()
	{
		emit needSelectAccountSignal();
	}

	void SignalForwarder::sendKickoutSignal()
	{
		emit kickoutSignal();
	}

	void SignalForwarder::sendClosePageSignal()
	{
		emit closePageSignal();
	}

	void SignalForwarder::sendRefreshInstrumentUISignal()
	{
		emit refreshInstrumentUISignal();
	}

	void SignalForwarder::sendNeedSelectInstrumentSignal()
	{
		emit needSelectInstrumentSignal();
	}

	void SignalForwarder::sendIsSelectAccountSignal(bool isSelect, QString accountId)
	{
		emit isSelectAccountSignal(isSelect, accountId);
	}
		
	void SignalForwarder::sendIsSelectALLAccountSignal(bool isSelect, QList<QString> accountIds)
	{
		emit isSelectALLAccountSignal(isSelect, accountIds);
	}

	void SignalForwarder::sendShowOrHideOrderTabSignal(OrderTabType::OrderTabType orderTabType, bool isShow)
	{
		emit showOrHideOrderTabSignal(orderTabType, isShow);
	}

	void SignalForwarder::sendIsShowSytemMenuSignal(MainSytemMenu::MainSytemMenu mainSytemMenu, bool isShow)
	{
		emit isShowSytemMenuSignal(mainSytemMenu, isShow);
	}

	void SignalForwarder::sendIsShowSystemTitleBarSignal(MainSytemMenu::MainSytemMenu mainSytemMenu, bool isShow)
	{
		emit isShowSystemTitleBarSignal(mainSytemMenu, isShow);
	}

	void SignalForwarder::sendInstrumentStatusChangedSignal(QUuid instrumentId, int instrumentStatus)
	{
		emit instrumentStatusChangedSignal(instrumentId, instrumentStatus);
	}

	void SignalForwarder::sendAccountMarginNotifySignal(QString& accountCodes, QStringList& resetAlertLevelAccountIds, QString& resetAlertLevelAccountCodes)
	{
		emit accountMarginNotifySignal(accountCodes, resetAlertLevelAccountIds, resetAlertLevelAccountCodes);
	}

	void SignalForwarder::sendAddOrdersToChartSignal(QList<QUuid> orderIds)
	{
		emit addOrdersToChartSignal(orderIds);
	}
			
	void SignalForwarder::sendRemoveOrdersToChartSignal(QMap<QUuid, QUuid> orderIds)
	{
		emit removeOrdersToChartSignal(orderIds);
	}

	void SignalForwarder::sendEnableFastTradeSignal(QUuid instrumentId, bool fastTrade)
	{
		emit enableFastTradeSignal(instrumentId, fastTrade);
	}

	void SignalForwarder::sendRefeshEnableFastTradeSignal()
	{
		emit refeshEnableFastTradeSignal();
	}

	void SignalForwarder::sendAdjustFontSizeSignal(int fontSizeStyleIndex, QString functionName, QString Style, bool isOnlyRefeshQuotionCard)
	{
		emit adjustFontSizeSignal(fontSizeStyleIndex, functionName, Style, isOnlyRefeshQuotionCard);
	}

	void SignalForwarder::sendChangeCloseByFIFOStateSignal(QUuid instrumentId, bool closeByFIFO)
	{
		emit changeCloseByFIFOStateSignal(instrumentId, closeByFIFO);
	}

	void SignalForwarder::sendChangeQuickPlacingSpinBoxSignal(QUuid instrumentId, SpinBoxType::SpinBoxType spinBoxType, double value)
	{
		emit changeQuickPlacingSpinBoxSignal(instrumentId, spinBoxType, value);
	}

	void SignalForwarder::sendRefreshPageFiFoStatusSignal(QList<QUuid> instrumentIds)
	{
		emit refreshPageFiFoStatusSignal(instrumentIds);
	}

	void SignalForwarder::sendAddNewsOrChatSignal(QUuid id, InformationCenterEnum::InformationCenterEnum informationCenterEnum)
	{
		emit addNewsOrChatSignal(id, informationCenterEnum);
	}

	void SignalForwarder::sendShowNewsOrChatSignal(QUuid id, InformationCenterEnum::InformationCenterEnum informationCenterEnum)
	{
		emit showNewsOrChatSignal(id, informationCenterEnum);
	}

	void SignalForwarder::sendAddPriceAlertSignal(QUuid id)
	{
		emit addPriceAlertSignal(id);
	}

	void SignalForwarder::sendNotifyPriceAlertSignal(QList<QUuid> hitItems, QList<QUuid> expiredItems)
	{
		emit notifyPriceAlertSignal(hitItems, expiredItems);
	}

	void SignalForwarder::sendRefreshPriceAlertSignal()
	{
		emit refreshPriceAlertSignal();
	}

	void SignalForwarder::sendAddLogSignal(QUuid id)
	{
		emit addLogSignal(id);
	}

    void SignalForwarder::sendAddQtAppLogSignal(QUuid id)
    {
        emit addQtAppLogSignal(id);
    }

	void SignalForwarder::sendInterestFreeDaysNotifySignal()
	{
		emit interestFreeDaysNotifySignal();
	}

	void SignalForwarder::sendRefreshPhysicalButtonIsEnable()
	{
		emit refreshPhysicalButtonIsEnableSignal();
	}
	
	void SignalForwarder::sendPlaySoundsSignal(SoundsEnum::SoundsEnum soundsEnum)
	{
		emit playSoundsSignal(soundsEnum);
	}

	void SignalForwarder::sendQuoteCommandSignal(QString ask, QString bid, QString answerLot, QString quoteLot)
	{
		emit quoteCommandSignal(ask, bid, answerLot, quoteLot);
	}

	void SignalForwarder::sendComputeAllSignal()
	{
		emit computeAllSignal();
	}

	void SignalForwarder::sendComputeAccountSignal(QUuid accountId)
	{
		emit computeAccountSignal(accountId);
	}

	void SignalForwarder::sendAccoutWillExpireOrAccountExpireNotifySignal(QString accountWillExpire, QString accountExpire)
	{
		emit accoutWillExpireOrAccountExpireNotifySignal(accountWillExpire, accountExpire);
	}

	void SignalForwarder::sendComputeQuotationSignal(QList<QUuid> instrumentIds)
	{
		emit computeQuotationSignal(instrumentIds);
	}

	void SignalForwarder::sendComputeAllSignal(QList<QUuid> instrumentIds)
	{
		emit computeAllSignal(instrumentIds);
	}

	void SignalForwarder::sendComputeAccountSignal()
	{
		emit computeAccountSignal();
	}

	void SignalForwarder::sendChangeStyleByConnectionStatusChangedSignal(bool isConnect)
	{
		emit changeStyleByConnectionStatusChangedSignal(isConnect);
	}

	void SignalForwarder::sendClearSignal()
	{
		emit clearSignal();
	}

	void SignalForwarder::sendRefreshQuotePolicySignal()
	{
		emit refreshQuotePolicySignal();
	}

	void SignalForwarder::sendRefreshChartSignal()
	{
		emit refreshChartSignal();
	}

	void SignalForwarder::sendIsHideInstalmentOrRepaymentButtonSignal(bool isHide, bool isInstalment)
	{
		emit isHideInstalmentOrRepaymentButtonSignal(isHide, isInstalment);
	}

	void SignalForwarder::sendShowDisclaimerSignal(QUuid instrumentId, bool fastTrade)
	{
		emit showDisclaimerSignal(instrumentId, fastTrade);
	}

	void SignalForwarder::sendCustomerTradeStatusNotifySignal(bool disallowTrade)
	{
		emit customerTradeStatusNotifySignal(disallowTrade);
	}

	void SignalForwarder::sendChangeBOFrequencySignal(QUuid instrumentId, QString frequencyKey)
	{
		emit changeBOFrequencySignal(instrumentId, frequencyKey);
	}

	void SignalForwarder::sendRefreshBOFrequenciesSignal(QUuid instrumentId)
	{
		emit refreshBOFrequenciesSignal(instrumentId);
	}

	void SignalForwarder::sendBOInfoToBOChartSignal(QUuid orderId, QString BOCountdown, QString BOFloatPLInString, QDateTime BOStartTime, QDateTime BOEndTime, QString BOExecutePrice, bool isUp)
	{
		emit BOInfoToBOChartSignal(orderId, BOCountdown, BOFloatPLInString, BOStartTime, BOEndTime, BOExecutePrice, isUp);
	}

	void SignalForwarder::sendShowBORealtimeChartSignal(QUuid orderId, QUuid accountId, QUuid instrumentId)
	{
		emit showBORealtimeChartSignal(orderId, accountId, instrumentId);
	}

	void SignalForwarder::sendHideBORealtimeChartSignal(QUuid orderId)
	{
		emit hideBORealtimeChartSignal(orderId);
	}

	void SignalForwarder::sendChangeQuickPlacingWidgetSizeSignal(int value)
	{
		emit changeQuickPlacingWidgetSizeSignal(value);
	}

	void SignalForwarder::sendUpdateCommandSignal()
	{
		emit updateCommandSignal();
	}

	void SignalForwarder::sendStopProcessBarSignal()
	{
		emit stopProcessBarSignal();
	}

	void SignalForwarder::sendLoginQuotationServerFaildSignal()
	{
		emit loginQuotationServerFaildSignal();
	}

	void SignalForwarder::sendChartQuotationsSignal(const QDomElement& xmlNotify)
	{
		emit chartQuotationsSignal(xmlNotify);
	}

	void SignalForwarder::sendRemoveOrderSignal(QList<QUuid> ids)
	{
		emit removeOrderSignal(ids);
	}
	void SignalForwarder::sendAddSubFundSignal(QString accountId, QString currencyId)
	{
		emit addSubFundSignal(accountId, currencyId);
	}

	void SignalForwarder::sendChangeEmployeeAccountListCurrencySignal(QUuid salesId, QUuid currencyId)
	{
		emit changeEmployeeAccountListCurrencySignal(salesId, currencyId);
	}

	void SignalForwarder::sendInstrumentPriceWaitConfirmOrderSignal(QUuid instrumentId, QString ask, QString bid, bool isStop)
	{
		emit instrumentPriceWaitConfirmOrderSignal(instrumentId, ask, bid, isStop);
	}

    void SignalForwarder::sendChangeUpDownColorSignal(QString upColor, QString downColor)
    {
        emit changeUpDownColorSignal(upColor, downColor);
    }

    void SignalForwarder::sendRefreshSystemCompleteSignal()
    {
        emit refreshSystemCompleteSignal();
    }

    void SignalForwarder::sendQuotationChangeSignal()
    {
        emit quotationChangeSignal();
    }

    void SignalForwarder::sendShowLoginSignal()
    {
        emit showLoginSignal();
    }

    void SignalForwarder::sendChangeMarketInformationSignal(int MarketInformationOption)
    {
        emit changeMarketInformationSignal(MarketInformationOption);
    }

    void SignalForwarder::sendRefreshStyleSignal()
    {
        emit refreshStyleSignal();
    }

    void SignalForwarder::sendTodayExecuteOrderSignal(QUuid id)
    {
        emit todayExecuteOrderSignal(id);
    }

    void SignalForwarder::sendOrdersChangedSignal(QList<QUuid> orderIds)
    {
        emit ordersChangedSignal(orderIds);
    }

    void SignalForwarder::sendRefreshSystemByQueryForAddAccountSignal(QString accountCode, QString accountName)
    {
        emit refreshSystemByQueryForAddAccountSignal(accountCode, accountName);
    }

    void SignalForwarder::sendCloseMobileHorizontalScreenChartSignal()
    {
        emit closeMobileHorizontalScreenChartSignal();
    }

    void SignalForwarder::sendClickInstrumentSwitchChartSignal(QUuid instrumentId)
    {
        emit clickInstrumentSwitchChartSignal(instrumentId);
    }

    void SignalForwarder::sendRefreshTradingDataStatusSignal(bool isSuccess)
    {
        emit refreshTradingDataStatusSignal(isSuccess);
    }

#ifdef RUNTIME_OS_MACX //add for bug
    void SignalForwarder::sendChangeChartTransparentForMouseEventSignal(bool enable)
    {
        emit changeChartTransparentForMouseEventSignal(enable);
    }
#endif

}

#include "moc_SignalForwarder.cpp"
