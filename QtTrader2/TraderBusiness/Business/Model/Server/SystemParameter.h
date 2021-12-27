#ifndef BUSINESS_MODEL_SERVER_SYSTEMPARAMETER_H
#define BUSINESS_MODEL_SERVER_SYSTEMPARAMETER_H
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include "Common/Enum/TimeOptionInTraderLogEnum.h"
#include <QUuid>
#include <QXmlStreamReader>

using namespace Common::Enum;
namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class SystemParameter
			{
			private:
				bool _allowEditBankAccountInTrader;
				bool _bOIncludePrincipal;
				bool _balanceDeficitAllowPay;
				bool _bankAccountNameMustSameWithAccountName;
				bool _bankAccountOnly;
				bool _caculateChangeWithDenominator;
				bool _closingUseCustomerQuotePolicy;
				QUuid _cnyCurrencyId;
				QString _consumerCode;
				bool _dQMaxLotApplyAccount;
				bool _enableMarginPin;
				bool _enableModifyLeverage;
				bool _enableModifyTelephoneIdentificationCode;
				bool _enableResetTelephonePin;
				bool _enableTrendSheetChart;
				int _enquiryOutTime;
				int _exceptionEnquiryOutTime;
				bool _highBid;
				bool _includeFeeOnRiskAction;
				int _interestFreeAlertDay;
				int _leverageStep;
				bool _lowBid;
				int _maxAlertPricePerInstrument;
				int _maxCustomerBankNo;
				int _maxLeverage;
				int _minLeverage;
				bool _openPositionShowTime;
				QString _paymentPath;
				int _placeConfirmMinTime;
				int _priceRow;
				bool _ratioIncludeCredit;
				bool _showHundredChart;
				bool _showPriceChangedBeforeCloseConfirmWindow;
				int _sptReconfirmDuration;
				Common::Enum::TimeOptionInTraderLog::TimeOptionInTraderLog _timeOptionInTraderLogAndConfirmWindow;
				bool _useNightNecessaryWhenBreak;
				bool _isAllowLimitInSpread;
				bool _isCheckAcceptDQVariation;
				int _nonQuoteVerificationUiDelay;
				bool _allowTraderResetAlertLevel;
				bool _enableNetAsset;
                bool _allowTraderDQReconfirm;
                QString _reportUrl;
                bool _enableClickTrade;
                int _tradePLFloatColumnNumber;
                int _orderCodeColumnNumber;
                int _accountCodeColumnNumber;
                QString _clientBookTraderUrl;
                QString _iexCrmUserCenterUrl;
                bool _isShowNews;

			public:
				bool getAllowEditBankAccountInTrader() const
				{
					return _allowEditBankAccountInTrader;
				}

				bool getBOIncludePrincipal() const
				{
					return _bOIncludePrincipal;
				}

				bool getBalanceDeficitAllowPay() const
				{
					return _balanceDeficitAllowPay;
				}

				bool getBankAccountNameMustSameWithAccountName() const
				{
					return _bankAccountNameMustSameWithAccountName;
				}

				bool getBankAccountOnly() const
				{
					return _bankAccountOnly;
				}

				bool getCaculateChangeWithDenominator() const
				{
					return _caculateChangeWithDenominator;
				}

				bool getClosingUseCustomerQuotePolicy() const
				{
					return _closingUseCustomerQuotePolicy;
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
					return _dQMaxLotApplyAccount;
				}

				bool getEnableMarginPin() const
				{
					return _enableMarginPin;
				}

				bool getEnableModifyLeverage() const
				{
					return _enableModifyLeverage;
				}

				bool getEnableModifyTelephoneIdentificationCode() const
				{
					return _enableModifyTelephoneIdentificationCode;
				}

				bool getEnableResetTelephonePin() const
				{
					return _enableResetTelephonePin;
				}

				bool getEnableTrendSheetChart() const
				{
					return _enableTrendSheetChart;
				}

				int getEnquiryOutTime() const
				{
					return _enquiryOutTime;
				}

				int getExceptionEnquiryOutTime() const
				{
					return _exceptionEnquiryOutTime;
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

				int getLeverageStep() const
				{
					return _leverageStep;
				}

				bool getLowBid() const
				{
					return _lowBid;
				}

				int getMaxAlertPricePerInstrument() const
				{
					return _maxAlertPricePerInstrument;
				}

				int getMaxCustomerBankNo() const
				{
					return _maxCustomerBankNo;
				}

				int getMaxLeverage() const
				{
					return _maxLeverage;
				}

				int getMinLeverage() const
				{
					return _minLeverage;
				}

				bool getOpenPositionShowTime() const
				{
					return _openPositionShowTime;
				}

				QString getPaymentPath() const
				{
					return _paymentPath;
				}

				int getPlaceConfirmMinTime() const
				{
					return _placeConfirmMinTime;
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

				bool getShowPriceChangedBeforeCloseConfirmWindow() const
				{
					return _showPriceChangedBeforeCloseConfirmWindow;
				}

				int getSptReconfirmDuration() const
				{
					return _sptReconfirmDuration;
				}

				Common::Enum::TimeOptionInTraderLog::TimeOptionInTraderLog getTimeOptionInTraderLogAndConfirmWindow() const
				{
					return _timeOptionInTraderLogAndConfirmWindow;
				}

				bool getUseNightNecessaryWhenBreak() const
				{
					return _useNightNecessaryWhenBreak;
				}

				bool getIsAllowLimitInSpread() const
				{
					return _isAllowLimitInSpread;
				}

				bool getIsCheckAcceptDQVariation() const
				{
					return _isCheckAcceptDQVariation;
				}

				int getNonQuoteVerificationUiDelay() const
				{
					return _nonQuoteVerificationUiDelay;
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

                bool getIsShowNews() const
                {
                    return _isShowNews;
                }


			public:
				void setAllowEditBankAccountInTrader(const bool& value)
				{
					_allowEditBankAccountInTrader = value;
				}

				void setBOIncludePrincipal(const bool& value)
				{
					_bOIncludePrincipal = value;
				}

				void setBalanceDeficitAllowPay(const bool& value)
				{
					_balanceDeficitAllowPay = value;
				}

				void setBankAccountNameMustSameWithAccountName(const bool& value)
				{
					_bankAccountNameMustSameWithAccountName = value;
				}

				void setBankAccountOnly(const bool& value)
				{
					_bankAccountOnly = value;
				}

				void setCaculateChangeWithDenominator(const bool& value)
				{
					_caculateChangeWithDenominator = value;
				}

				void setClosingUseCustomerQuotePolicy(const bool& value)
				{
					_closingUseCustomerQuotePolicy = value;
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
					_dQMaxLotApplyAccount = value;
				}

				void setEnableMarginPin(const bool& value)
				{
					_enableMarginPin = value;
				}

				void setEnableModifyLeverage(const bool& value)
				{
					_enableModifyLeverage = value;
				}

				void setEnableModifyTelephoneIdentificationCode(const bool& value)
				{
					_enableModifyTelephoneIdentificationCode = value;
				}

				void setEnableResetTelephonePin(const bool& value)
				{
					_enableResetTelephonePin = value;
				}

				void setEnableTrendSheetChart(const bool& value)
				{
					_enableTrendSheetChart = value;
				}

				void setEnquiryOutTime(const int& value)
				{
					_enquiryOutTime = value;
				}

				void setExceptionEnquiryOutTime(const int& value)
				{
					_exceptionEnquiryOutTime = value;
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

				void setLeverageStep(const int& value)
				{
					_leverageStep = value;
				}

				void setLowBid(const bool& value)
				{
					_lowBid = value;
				}

				void setMaxAlertPricePerInstrument(const int& value)
				{
					_maxAlertPricePerInstrument = value;
				}

				void setMaxCustomerBankNo(const int& value)
				{
					_maxCustomerBankNo = value;
				}

				void setMaxLeverage(const int& value)
				{
					_maxLeverage = value;
				}

				void setMinLeverage(const int& value)
				{
					_minLeverage = value;
				}

				void setOpenPositionShowTime(const bool& value)
				{
					_openPositionShowTime = value;
				}

				void setPaymentPath(const QString& value)
				{
					_paymentPath = value;
				}

				void setPlaceConfirmMinTime(const int& value)
				{
					_placeConfirmMinTime = value;
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

				void setShowPriceChangedBeforeCloseConfirmWindow(const bool& value)
				{
					_showPriceChangedBeforeCloseConfirmWindow = value;
				}

				void setSptReconfirmDuration(const int& value)
				{
					_sptReconfirmDuration = value;
				}

				void setTimeOptionInTraderLogAndConfirmWindow(const Common::Enum::TimeOptionInTraderLog::TimeOptionInTraderLog& value)
				{
					_timeOptionInTraderLogAndConfirmWindow = value;
				}

				void setUseNightNecessaryWhenBreak(const bool& value)
				{
					_useNightNecessaryWhenBreak = value;
				}

				void setIsAllowLimitInSpread(const bool& value)
				{
					_isAllowLimitInSpread = value;
				}

				void setIsCheckAcceptDQVariation(const bool& value)
				{
					_isCheckAcceptDQVariation = value;
				}

				void setNonQuoteVerificationUiDelay(const int& value)
				{
					_nonQuoteVerificationUiDelay = value;
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

                void setIsShowNews(bool value)
                {
                    _isShowNews = value;
                }



			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_allowEditBankAccountInTrader = Common::toBool(attributes.value("AllowEditBankAccountInTrader").toString());
					this->_bOIncludePrincipal = Common::toBool(attributes.value("BOIncludePrincipal").toString());
					this->_balanceDeficitAllowPay = Common::toBool(attributes.value("BalanceDeficitAllowPay").toString());
					this->_bankAccountNameMustSameWithAccountName = Common::toBool(attributes.value("BankAccountNameMustSameWithAccountName").toString());
					this->_bankAccountOnly = Common::toBool(attributes.value("BankAccountOnly").toString());
					this->_caculateChangeWithDenominator = Common::toBool(attributes.value("CaculateChangeWithDenominator").toString());
					this->_closingUseCustomerQuotePolicy = Common::toBool(attributes.value("ClosingUseCustomerQuotePolicy").toString());
					this->_cnyCurrencyId = QUuid(attributes.value("CnyCurrencyId").toString());
					this->_consumerCode = attributes.value("ConsumerCode").toString();
					this->_dQMaxLotApplyAccount = Common::toBool(attributes.value("DQMaxLotApplyAccount").toString());
					this->_enableMarginPin = Common::toBool(attributes.value("EnableMarginPin").toString());
					this->_enableModifyLeverage = Common::toBool(attributes.value("EnableModifyLeverage").toString());
					this->_enableModifyTelephoneIdentificationCode = Common::toBool(attributes.value("EnableModifyTelephoneIdentificationCode").toString());
					this->_enableResetTelephonePin = Common::toBool(attributes.value("EnableResetTelephonePin").toString());
					this->_enableTrendSheetChart = Common::toBool(attributes.value("EnableTrendSheetChart").toString());
					this->_enquiryOutTime = attributes.value("EnquiryOutTime").toInt();
					this->_exceptionEnquiryOutTime = attributes.value("ExceptionEnquiryOutTime").toInt();
					this->_highBid = Common::toBool(attributes.value("HighBid").toString());
					this->_includeFeeOnRiskAction = Common::toBool(attributes.value("IncludeFeeOnRiskAction").toString());
					this->_interestFreeAlertDay = attributes.value("InterestFreeAlertDay").toInt();
					this->_leverageStep = attributes.value("LeverageStep").toInt();
					this->_lowBid = Common::toBool(attributes.value("LowBid").toString());
					this->_maxAlertPricePerInstrument = attributes.value("MaxAlertPricePerInstrument").toInt();
					this->_maxCustomerBankNo = attributes.value("MaxCustomerBankNo").toInt();
					this->_maxLeverage = attributes.value("MaxLeverage").toInt();
					this->_minLeverage = attributes.value("MinLeverage").toInt();
					this->_openPositionShowTime = Common::toBool(attributes.value("OpenPositionShowTime").toString());
					this->_paymentPath = attributes.value("PaymentPath").toString();
					this->_placeConfirmMinTime = attributes.value("PlaceConfirmMinTime").toInt();
					this->_priceRow = attributes.value("PriceRow").toInt();
					this->_ratioIncludeCredit = Common::toBool(attributes.value("RatioIncludeCredit").toString());
					this->_showHundredChart = Common::toBool(attributes.value("ShowHundredChart").toString());
					this->_showPriceChangedBeforeCloseConfirmWindow = Common::toBool(attributes.value("ShowPriceChangedBeforeCloseConfirmWindow").toString());
					this->_sptReconfirmDuration = attributes.value("SptReconfirmDuration").toInt();
					this->_timeOptionInTraderLogAndConfirmWindow = static_cast<TimeOptionInTraderLog::TimeOptionInTraderLog>(attributes.value("TimeOptionInTraderLogAndConfirmWindow").toInt());
					this->_useNightNecessaryWhenBreak = Common::toBool(attributes.value("UseNightNecessaryWhenBreak").toString());
					this->_isAllowLimitInSpread = Common::toBool(attributes.value("IsAllowLimitInSpread").toString());
					this->_isCheckAcceptDQVariation = Common::toBool(attributes.value("IsCheckAcceptDQVariation").toString());
					this->_nonQuoteVerificationUiDelay = attributes.value("NonQuoteVerificationUiDelay").toInt();
					this->_allowTraderResetAlertLevel = Common::toBool(attributes.value("AllowTraderResetAlertLevel").toString());					
					this->_enableNetAsset = Common::toBool(attributes.value("EnableNetAsset").toString());
                    this->_allowTraderDQReconfirm = Common::toBool(attributes.value("AllowTraderDQReconfirm").toString());
                    this->_reportUrl = attributes.value("ReportUrl").toString();
                    this->_enableClickTrade = Common::toBool(attributes.value("EnableClickTrade").toString());
                    this->_tradePLFloatColumnNumber = attributes.value("TradePLFloatColumnNumber").toInt();
                    this->_orderCodeColumnNumber = attributes.value("OrderCodeColumnNumber").toInt();
                    this->_accountCodeColumnNumber = attributes.value("AccountCodeColumnNumber").toInt();
                    this->_clientBookTraderUrl = attributes.hasAttribute("ClientBookTraderUrl")? attributes.value("ClientBookTraderUrl").toString() : "";
                    this->_iexCrmUserCenterUrl = attributes.hasAttribute("IexCrmUserCenterUrl")? attributes.value("IexCrmUserCenterUrl").toString() : "";
                    this->_isShowNews = attributes.hasAttribute("IsShowNews")? Common::toBool(attributes.value("IsShowNews").toString()) : true;
				}

				void update(const QDomNode& xmlNode)
				{
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if (name == "AllowEditBankAccountInTrader")
							this->_allowEditBankAccountInTrader = Common::toBool(value);
						else if (name == "BOIncludePrincipal")
							this->_bOIncludePrincipal = Common::toBool(value);
						else if (name == "BalanceDeficitAllowPay")
							this->_balanceDeficitAllowPay = Common::toBool(value);
						else if (name == "BankAccountNameMustSameWithAccountName")
							this->_bankAccountNameMustSameWithAccountName = Common::toBool(value);
						else if (name == "BankAccountOnly")
							this->_bankAccountOnly = Common::toBool(value);
						else if (name == "CaculateChangeWithDenominator")
							this->_caculateChangeWithDenominator = Common::toBool(value);
						else if (name == "ClosingUseCustomerQuotePolicy")
							this->_closingUseCustomerQuotePolicy = Common::toBool(value);
						else if (name == "CnyCurrencyId")
							this->_cnyCurrencyId = QUuid(value);
						else if (name == "ConsumerCode")
							this->_consumerCode = value;
						else if (name == "DQMaxLotApplyAccount")
							this->_dQMaxLotApplyAccount = Common::toBool(value);
						else if (name == "EnableMarginPin")
							this->_enableMarginPin = Common::toBool(value);
						else if (name == "EnableModifyLeverage")
							this->_enableModifyLeverage = Common::toBool(value);
						else if (name == "EnableModifyTelephoneIdentificationCode")
							this->_enableModifyTelephoneIdentificationCode = Common::toBool(value);
						else if (name == "EnableResetTelephonePin")
							this->_enableResetTelephonePin = Common::toBool(value);
						else if (name == "EnableTrendSheetChart")
							this->_enableTrendSheetChart = Common::toBool(value);
						else if (name == "EnquiryOutTime")
							this->_enquiryOutTime = value.toInt();
						else if (name == "ExceptionEnquiryOutTime")
							this->_exceptionEnquiryOutTime = value.toInt();
						else if (name == "HighBid")
							this->_highBid = Common::toBool(value);
						else if (name == "IncludeFeeOnRiskAction")
							this->_includeFeeOnRiskAction = Common::toBool(value);
						else if (name == "InterestFreeAlertDay")
							this->_interestFreeAlertDay = value.toInt();
						else if (name == "LeverageStep")
							this->_leverageStep = value.toInt();
						else if (name == "LowBid")
							this->_lowBid = Common::toBool(value);
						else if (name == "MaxAlertPricePerInstrument")
							this->_maxAlertPricePerInstrument = value.toInt();
						else if (name == "MaxCustomerBankNo")
							this->_maxCustomerBankNo = value.toInt();
						else if (name == "MaxLeverage")
							this->_maxLeverage = value.toInt();
						else if (name == "MinLeverage")
							this->_minLeverage = value.toInt();
						else if (name == "OpenPositionShowTime")
							this->_openPositionShowTime = Common::toBool(value);
						else if (name == "PaymentPath")
							this->_paymentPath = value;
						else if (name == "PlaceConfirmMinTime")
							this->_placeConfirmMinTime = value.toInt();
						else if (name == "PriceRow")
							this->_priceRow = value.toInt();
						else if (name == "RatioIncludeCredit")
							this->_ratioIncludeCredit = Common::toBool(value);
						else if (name == "ShowHundredChart")
							this->_showHundredChart = Common::toBool(value);
						else if (name == "ShowPriceChangedBeforeCloseConfirmWindow")
							this->_showPriceChangedBeforeCloseConfirmWindow = Common::toBool(value);
						else if (name == "SptReconfirmDuration")
							this->_sptReconfirmDuration = value.toInt();
						else if (name == "TimeOptionInTraderLogAndConfirmWindow")
							this->_timeOptionInTraderLogAndConfirmWindow = static_cast<TimeOptionInTraderLog::TimeOptionInTraderLog>(value.toInt());
						else if (name == "UseNightNecessaryWhenBreak")
							this->_useNightNecessaryWhenBreak = Common::toBool(value);
						else if (name == "IsAllowLimitInSpread")
							this->_isAllowLimitInSpread = Common::toBool(value);
						else if (name == "IsCheckAcceptDQVariation")
							this->_isCheckAcceptDQVariation = Common::toBool(value);
						else if (name == "NonQuoteVerificationUiDelay")
							this->_nonQuoteVerificationUiDelay = value.toInt();
						else if (name == "AllowTraderResetAlertLevel")
							this->_allowTraderResetAlertLevel = Common::toBool(value);
						else if (name == "EnableNetAsset")
							this->_enableNetAsset = Common::toBool(value);
                        else if (name == "AllowTraderDQReconfirm")
                            this->_allowTraderDQReconfirm = Common::toBool(value);
                        else if (name == "ReportUrl")
                            this->_reportUrl = value;
                        else if (name == "EnableClickTrade")
                            this->_enableClickTrade = Common::toBool(value);                        
                        else if (name == "TradePLFloatColumnNumber")
                            this->_tradePLFloatColumnNumber = value.toInt();
                        else if (name == "OrderCodeColumnNumber")
                            this->_orderCodeColumnNumber = value.toInt();
                        else if (name == "AccountCodeColumnNumber")
                            this->_accountCodeColumnNumber = value.toInt();
                        else if (name == "ClientBookTraderUrl")
                            this->_clientBookTraderUrl = value;
                        else if (name == "IexCrmUserCenterUrl")
                            this->_iexCrmUserCenterUrl = value;
                        else if (name == "IsShowNews")
                            this->_isShowNews = Common::toBool(value);


					}
				}

			};

		}
	}
}
#endif

