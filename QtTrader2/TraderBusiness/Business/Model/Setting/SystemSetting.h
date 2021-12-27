#ifndef BUSINESS_MODEL_SETTING_SYSTEMSETTING_H
#define BUSINESS_MODEL_SETTING_SYSTEMSETTING_H
#include <quuid.h>
#include <qstring.h>
#include <qdatetime.h>
#include <QMap>
#include "Common/Enum/TimeOptionInTraderLogEnum.h"
#include "Common/Enum/AccountOrderTypeEnum.h"
#include <QDomNode>
#include "traderbusiness_global.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class TRADERBUSINESS_EXPORT SystemSetting
			{
			public:
				SystemSetting();
				~SystemSetting();
			private:
				bool  _isEmployee;
				bool _needSelectAccount;
				bool _allowFreeAgent;
				bool  _showLog;
				int _DQOrderOutTime;
				bool _enableFastTrade;
				bool _DQMaxLotApplyAccount;
				Common::Enum::TimeOptionInTraderLog::TimeOptionInTraderLog _timeOptionInTraderLogAndConfirmWindow;
				bool _showPriceChangedBeforevCloseConfirmWindow;
				bool _caculateChangeWithDenominator;
				bool _highBid;
				bool _lowBid;
				bool _enableTrendSheetChart;
				int _orderBeginTimeDiff;
				bool _isCheckAcceptDQVariation;
				int _nonQuoteVerificationUiDelay;
				bool _isAllowLimitInSpread;
				bool _enableResetTelephonePin;
				bool _enableModifyLeverage;
				bool _bankAccountOnly;
				QString _paymentPath;
				QString _consumerCode;
				QUuid _cnyCurrencyId;
				bool  _isNoShowAccountStatus;
				bool _isShowOpenOrderAndSummary;
				bool _isShowPhysical;
				int _interestFreeAlertDay;
				int _maxCustomerBankNo;
				bool _allowEditBankAccountInTrader;
				bool _showHundredChart;
				bool _isSptReconfirm;
				int _sptReconfirmDuration;
				int _priceRow;
				bool _ratioIncludeCredit;
				bool _includeFeeOnRiskAction;
				bool _boIncludePrincipal;
				QString _organizationName;
				bool _openPositionShowTime;
				bool _disallowTrade;
				bool _allowTraderResetAlertLevel;
				bool _enableNetAsset;
                bool _allowTraderDQReconfirm;

				Common::Enum::AccountOrderType::AccountOrderType _singleAccountOrderType;
				Common::Enum::AccountOrderType::AccountOrderType _multiAccountsOrderType;

				int _traderIdleTimeout;
				int _maxPasswordReserveCount;
                QString _reportUrl;
                bool _enableClickTrade;                
                int _tradePLFloatColumnNumber;
                int _orderCodeColumnNumber;
                int _accountCodeColumnNumber;
                QString _clientBookTraderUrl;
                QString _iexCrmUserCenterUrl;
                bool _isIexCrmUser;
                bool _isShowNews;
                bool _isUploadClientLog;

			public:
				void update(const QDomNode& xmlSystemSetting);
				
			public:
				bool getAllowEditBankAccountInTrader() const
				{
					return _allowEditBankAccountInTrader;
				}

				bool getAllowFreeAgent() const
				{
					return _allowFreeAgent;
				}

				bool getBankAccountOnly() const
				{
					return _bankAccountOnly;
				}

				bool getBOIncludePrincipal() const
				{
					return _boIncludePrincipal;
				}

				bool getCaculateChangeWithDenominator() const
				{
					return _caculateChangeWithDenominator;
				}

				QUuid getCnyCurrencyId() const
				{
					return _cnyCurrencyId;
				}

				QString getConsumerCode() const
				{
					return _consumerCode;
				}

				bool getDQMaxLotApplyAccount() const
				{
					return _DQMaxLotApplyAccount;
				}

				int getDQOrderOutTime() const
				{
					return _DQOrderOutTime;
				}

				bool getDisallowTrade() const
				{
					return _disallowTrade;
				}

				bool getEnableFastTrade() const
				{
					return _enableFastTrade;
				}

				bool getEnableModifyLeverage() const
				{
					return _enableModifyLeverage;
				}

				bool getEnableResetTelephonePin() const
				{
					return _enableResetTelephonePin;
				}

				bool getEnableTrendSheetChart() const
				{
					return _enableTrendSheetChart;
				}

				bool getHighBid() const
				{
					return _highBid;
				}

				bool getIncludeFeeOnRiskAction() const
				{
					return _includeFeeOnRiskAction;
				}


				int getInterestFreeAlertDay() const
				{
					return _interestFreeAlertDay;
				}

				bool getIsAllowLimitInSpread() const
				{
					return _isAllowLimitInSpread;
				}

				bool getIsCheckAcceptDQVariation() const
				{
					return _isCheckAcceptDQVariation;
				}

				bool getIsEmployee() const
				{
					return _isEmployee;
				}

				bool getIsNoShowAccountStatus() const
				{
					return _isNoShowAccountStatus;
				}

				bool getIsShowOpenOrderAndSummary() const
				{
					return _isShowOpenOrderAndSummary;
				}

				bool getIsShowPhysical() const
				{
					return _isShowPhysical;
				}

				bool getIsSptReconfirm() const
				{
					return _isSptReconfirm;
				}
				
				bool getLowBid() const
				{
					return _lowBid;
				}

				int getMaxCustomerBankNo() const
				{
					return _maxCustomerBankNo;
				}

				Common::Enum::AccountOrderType::AccountOrderType getMultiAccountsOrderType() const
				{
					return _multiAccountsOrderType;
				}

				bool getNeedSelectAccount() const
				{
					return _needSelectAccount;
				}

				int getNonQuoteVerificationUiDelay() const
				{
					return _nonQuoteVerificationUiDelay;
				}

				bool getOpenPositionShowTime() const
				{
					return _openPositionShowTime;
				}

				int getOrderBeginTimeDiff() const
				{
					return _orderBeginTimeDiff;
				}

				QString getOrganizationName() const
				{
					return _organizationName;
				}

				QString getPaymentPath() const
				{
					return _paymentPath;
				}

				int getPriceRow() const
				{
					return _priceRow;
				}

				bool getRatioIncludeCredit() const
				{
					return _ratioIncludeCredit;
				}

				bool getShowHundredChart() const
				{
					return _showHundredChart;
				}

				bool getShowLog() const
				{
					return _showLog;
				}

				bool getShowPriceChangedBeforevCloseConfirmWindow() const
				{
					return _showPriceChangedBeforevCloseConfirmWindow;
				}

				Common::Enum::AccountOrderType::AccountOrderType getSingleAccountOrderType() const
				{
					return _singleAccountOrderType;
				}

				int getSptReconfirmDuration() const
				{
					return _sptReconfirmDuration;
				}

				Common::Enum::TimeOptionInTraderLog::TimeOptionInTraderLog getTimeOptionInTraderLogAndConfirmWindow() const
				{
					return _timeOptionInTraderLogAndConfirmWindow;
				}

				bool getAllowTraderResetAlertLevel() const
				{
					return _allowTraderResetAlertLevel;
				}
				
				bool getEnableNetAsset() const
				{
					return _enableNetAsset;
				}

                bool getAllowTraderDQReconfirm() const
                {
                    return _allowTraderDQReconfirm;
                }

				int getTraderIdleTimeout() const
				{
					return _traderIdleTimeout;
				}

				int getMaxPasswordReserveCount() const
				{
					return _maxPasswordReserveCount;
				}

                QString getReportUrl() const
                {
                    return _reportUrl;
                }

                bool getEnableClickTrade() const
                {
                    return _enableClickTrade;
                }

                int getTradePLFloatColumnNumber() const
                {
                    return _tradePLFloatColumnNumber;
                }

                int getOrderCodeColumnNumber() const
                {
                    return _orderCodeColumnNumber;
                }

                int getAccountCodeColumnNumber() const
                {
                    return _accountCodeColumnNumber;
                }                

                QString getClientBookTraderUrl() const
                {
                    return _clientBookTraderUrl;
                }

                QString getIexCrmUserCenterUrl() const
                {
                    return _iexCrmUserCenterUrl;
                }

                bool getIsIexCrmUser() const
                {
                    return _isIexCrmUser;
                }

                bool getIsShowNews() const
                {
                    return _isShowNews;
                }

                bool getIsUploadClientLog() const
                {
                    return _isUploadClientLog;
                }



				void setAllowEditBankAccountInTrader(const bool& value)
				{
					_allowEditBankAccountInTrader = value;
				}

				void setAllowFreeAgent(const bool& value)
				{
					_allowFreeAgent = value;
				}

				void setBankAccountOnly(const bool& value)
				{
					_bankAccountOnly = value;
				}

				void setBOIncludePrincipal(const bool& value)
				{
					_boIncludePrincipal = value;
				}

				void setCaculateChangeWithDenominator(const bool& value)
				{
					_caculateChangeWithDenominator = value;
				}

				void setCnyCurrencyId(const QUuid& value)
				{
					_cnyCurrencyId = value;
				}

				void setConsumerCode(const QString& value)
				{
					_consumerCode = value;
				}

				void setDQMaxLotApplyAccount(const bool& value)
				{
					_DQMaxLotApplyAccount = value;
				}

				void setDQOrderOutTime(const int& value)
				{
					_DQOrderOutTime = value;
				}

				void setDisallowTrade(const bool& value)
				{
					_disallowTrade = value;
				}

				void setEnableFastTrade(const bool& value)
				{
					_enableFastTrade = value;
				}

				void setEnableModifyLeverage(const bool& value)
				{
					_enableModifyLeverage = value;
				}

				void setEnableResetTelephonePin(const bool& value)
				{
					_enableResetTelephonePin = value;
				}

				void setEnableTrendSheetChart(const bool& value)
				{
					_enableTrendSheetChart = value;
				}

				void setHighBid(const bool& value)
				{
					_highBid = value;
				}

				void setIncludeFeeOnRiskAction(const bool& value)
				{
					_includeFeeOnRiskAction = value;
				}
				
				void setInterestFreeAlertDay(const int& value)
				{
					_interestFreeAlertDay = value;
				}

				void setIsAllowLimitInSpread(const bool& value)
				{
					_isAllowLimitInSpread = value;
				}

				void setIsCheckAcceptDQVariation(const bool& value)
				{
					_isCheckAcceptDQVariation = value;
				}

				void setIsEmployee(const bool& value)
				{
					_isEmployee = value;
				}

				void setIsNoShowAccountStatus(const bool& value)
				{
					_isNoShowAccountStatus = value;
				}

				void setIsShowOpenOrderAndSummary(const bool& value)
				{
					_isShowOpenOrderAndSummary = value;
				}

				void setIsShowPhysical(const bool& value)
				{
					_isShowPhysical = value;
				}

				void setIsSptReconfirm(const bool& value)
				{
					_isSptReconfirm = value;
				}
								
				void setLowBid(const bool& value)
				{
					_lowBid = value;
				}

				void setMaxCustomerBankNo(const int& value)
				{
					_maxCustomerBankNo = value;
				}

				void setMultiAccountsOrderType(const Common::Enum::AccountOrderType::AccountOrderType& value)
				{
					_multiAccountsOrderType = value;
				}

				void setNeedSelectAccount(const bool& value)
				{
					_needSelectAccount = value;
				}

				void setNonQuoteVerificationUiDelay(const int& value)
				{
					_nonQuoteVerificationUiDelay = value;
				}

				void setOpenPositionShowTime(const bool& value)
				{
					_openPositionShowTime = value;
				}

				void setOrderBeginTimeDiff(const int& value)
				{
					_orderBeginTimeDiff = value;
				}

				void setOrganizationName(const QString& value)
				{
					_organizationName = value;
				}

				void setPaymentPath(const QString& value)
				{
					_paymentPath = value;
				}

				void setPriceRow(const int& value)
				{
					_priceRow = value;
				}

				void setRatioIncludeCredit(const bool& value)
				{
					_ratioIncludeCredit = value;
				}

				void setShowHundredChart(const bool& value)
				{
					_showHundredChart = value;
				}

				void setShowLog(const bool& value)
				{
					_showLog = value;
				}

				void setShowPriceChangedBeforevCloseConfirmWindow(const bool& value)
				{
					_showPriceChangedBeforevCloseConfirmWindow = value;
				}

				void setSingleAccountOrderType(const Common::Enum::AccountOrderType::AccountOrderType& value)
				{
					_singleAccountOrderType = value;
				}

				void setSptReconfirmDuration(const int& value)
				{
					_sptReconfirmDuration = value;
				}

				void setTimeOptionInTraderLogAndConfirmWindow(const Common::Enum::TimeOptionInTraderLog::TimeOptionInTraderLog& value)
				{
					_timeOptionInTraderLogAndConfirmWindow = value;
				}

				void setAllowTraderResetAlertLevel(const bool& value)
				{
					_allowTraderResetAlertLevel = value;
				}
				
				void setEnableNetAsset(const bool& value)
				{
					_enableNetAsset = value;
                }

                void setAllowTraderDQReconfirm(const bool& value)
                {
                    _allowTraderDQReconfirm = value;
                }

                void setReportUrl(const QString& value)
                {
                    _reportUrl = value;
                }

                void setEnableClickTrade(const bool& value)
                {
                    _enableClickTrade = value;
                }

                void setTradePLFloatColumnNumber(const int& value)
                {
                    _tradePLFloatColumnNumber = value;
                }

                void setOrderCodeColumnNumber(const int& value)
                {
                    _orderCodeColumnNumber = value;
                }
                void setAccountCodeColumnNumber(const int& value)
                {
                    _accountCodeColumnNumber = value;
                }

                void setClientBookTraderUrl(const QString& value)
                {
                    _clientBookTraderUrl = value;
                }

                void setIexCrmUserCenterUrl(const QString& value)
                {
                    _iexCrmUserCenterUrl = value;
                }

                void setIsIexCrmUser(const bool& value)
                {
                    _isIexCrmUser = value;
                }

                void setIsShowNews(const bool& value)
                {
                    _isShowNews = value;
                }

                void setIsUploadClientLog(const bool& value)
                {
                    _isUploadClientLog = value;
                }


			};
		}
	}
}
#endif
