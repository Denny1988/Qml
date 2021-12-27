#ifndef BUSINESS_MODEL_SERVER_ACCOUNT_H
#define BUSINESS_MODEL_SERVER_ACCOUNT_H
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include "Common/Enum/AccountEnum.h"
#include <QUuid>
#include <QXmlStreamReader>
#include "Common/SignalForwarder.h"
#include "Common/TraderBaseInfo.h"

using namespace Common::Enum;
namespace Business
{
	namespace Model
	{
		namespace Server
		{
            class Account
			{
			private:
				QString _agentCode;
				QString _agentEmail;
				QUuid _agentId;
				QString _agentName;
				bool _allowAddNewPosition;
				bool _allowChangePasswordInTrader;
				bool _allowSalesTrading;
				bool _allowManagerTrading;
				long _bankAccountDefaultCountryId;
                QUuid _defaultPaymentCurrencyId;
				QDateTime _beginTime;
				QString _code;
				QString _consumerCode;
				QDecNumber _creditAmount;
				QUuid _currencyId;
				QString _customerEmail;
				QUuid _customerId;
				bool _enableAccountTransfer;
				bool _enableAgentRegistration;
				bool _enableCMExtension;
				bool _enableOwnerRegistration;
				bool _enablePI;
				bool _enablePICash;
				bool _enablePIInterACTransfer;
                bool _printOrderConfirmation;
				QDateTime _endTime;
				QUuid _groupId;
				QUuid _id;
				bool _isAutoClose;
				bool _isMultiCurrency;
				bool _isTradingAllowed;
				int _leverage;
				QString _name;
				bool _needBeneficiaryInPaymentInstruction;
				QUuid _organizationId;
				QString _organizationCode;
				QString _organizationPhone;
				QString _paymentInstructionRemark;
				QUuid _publicQuotePolicyId;
				QUuid _quotePolicyId;
				QDecNumber _rateCommission;
				QDecNumber _rateDefaultLot;
				QDecNumber _rateLevy;
				QDecNumber _rateLotMin;
				QDecNumber _rateLotMultiplier;
				QDecNumber _rateMarginD;
				QDecNumber _rateMarginLockD;
				QDecNumber _rateMarginLockO;
				QDecNumber _rateMarginO;
				QString _salesCode;
				QUuid _salesId;
				QUuid _tradePolicyId;
				Common::Enum::AccountType::AccountType _type;
				QUuid _useQuotePolicyId;
				int _userRelation;
				int _forbiddenAlert;
				int _counterPartiesMask;
				bool _showWeightOnly;
                bool _allowSalesPayment;
                bool _allowSalesDeposit;
                int _uploadBankCard;
                int _onlineTransferCount;
                bool _startupQuotation;

			public:
				QString getAgentCode() const
				{
					return _agentCode;
				}

				QString getAgentEmail() const
				{
					return _agentEmail;
				}

				QUuid getAgentId() const
				{
					return _agentId;
				}

				QString getAgentName() const
				{
					return _agentName;
				}

				bool getAllowAddNewPosition() const
				{
					return _allowAddNewPosition;
				}

				bool getAllowChangePasswordInTrader() const
				{
					return _allowChangePasswordInTrader;
				}

				bool getAllowSalesTrading() const
				{
					return _allowSalesTrading;
				}

				bool getAllowManagerTrading() const
				{
					return _allowManagerTrading;
				}

				long getBankAccountDefaultCountryId() const
				{
					return _bankAccountDefaultCountryId;
				}

                QUuid getDefaultPaymentCurrencyId() const
                {
                    return _defaultPaymentCurrencyId;
                }

				QDateTime getBeginTime() const
				{
					return _beginTime;
				}

				QString getCode() const
				{
					return _code;
				}

				QString getConsumerCode() const
				{
					return _consumerCode;
				}

				QDecNumber getCreditAmount() const
				{
					return _creditAmount;
				}

				QUuid getCurrencyId() const
				{
					return _currencyId;
				}

				QString getCustomerEmail() const
				{
					return _customerEmail;
				}

				QUuid getCustomerId() const
				{
					return _customerId;
				}

				bool getEnableAccountTransfer() const
				{
					return _enableAccountTransfer;
				}

				bool getEnableAgentRegistration() const
				{
					return _enableAgentRegistration;
				}

				bool getEnableCMExtension() const
				{
					return _enableCMExtension;
				}

				bool getEnableOwnerRegistration() const
				{
					return _enableOwnerRegistration;
				}

				bool getEnablePI() const
				{
					return _enablePI;
				}

				bool getEnablePICash() const
				{
					return _enablePICash;
				}

				bool getEnablePIInterACTransfer() const
				{
					return _enablePIInterACTransfer;
				}

                bool getPrintOrderConfirmation() const
                {
                    return _printOrderConfirmation;
                }


				QDateTime getEndTime() const
				{
					return _endTime;
				}

				QUuid getGroupId() const
				{
					return _groupId;
				}

				QUuid getId() const
				{
					return _id;
				}

				bool getIsAutoClose() const
				{
					return _isAutoClose;
				}

				bool getIsMultiCurrency() const
				{
					return _isMultiCurrency;
				}

				bool getIsTradingAllowed() const
				{
					return _isTradingAllowed;
				}

				int getLeverage() const
				{
					return _leverage;
				}

				QString getName() const
				{
					return _name;
				}

				bool getNeedBeneficiaryInPaymentInstruction() const
				{
					return _needBeneficiaryInPaymentInstruction;
				}

				QUuid getOrganizationId() const
				{
					return _organizationId;
				}

				QString getOrganizationCode() const
				{
					return _organizationCode;
				}				

				QString getOrganizationPhone() const
				{
					return _organizationPhone;
				}
				
				QString getPaymentInstructionRemark() const
				{
					return _paymentInstructionRemark;
				}

				QUuid getPublicQuotePolicyId() const
				{
					return _publicQuotePolicyId;
				}

				QUuid getQuotePolicyId() const
				{
					return _quotePolicyId;
				}

				QDecNumber getRateCommission() const
				{
					return _rateCommission;
				}

				QDecNumber getRateDefaultLot() const
				{
					return _rateDefaultLot;
				}

				QDecNumber getRateLevy() const
				{
					return _rateLevy;
				}

				QDecNumber getRateLotMin() const
				{
					return _rateLotMin;
				}

				QDecNumber getRateLotMultiplier() const
				{
					return _rateLotMultiplier;
				}

				QDecNumber getRateMarginD() const
				{
					return _rateMarginD;
				}

				QDecNumber getRateMarginLockD() const
				{
					return _rateMarginLockD;
				}

				QDecNumber getRateMarginLockO() const
				{
					return _rateMarginLockO;
				}

				QDecNumber getRateMarginO() const
				{
					return _rateMarginO;
				}

				QString getSalesCode() const
				{
					return _salesCode;
				}

				QUuid getSalesId() const
				{
					return _salesId;
				}

				QUuid getTradePolicyId() const
				{
					return _tradePolicyId;
				}

				Common::Enum::AccountType::AccountType getType() const
				{
					return _type;
				}

				QUuid getUseQuotePolicyId() const
				{
					return _useQuotePolicyId;
				}

				int getUserRelation() const
				{
					return _userRelation;
				}

				int getForbiddenAlert() const
				{
					return _forbiddenAlert;
				}

				int getCounterPartiesMask() const
				{
					return _counterPartiesMask;
				}

				bool getShowWeightOnly() const
				{
					return _showWeightOnly;
				}

                bool getAllowSalesPayment() const
                {
                    return _allowSalesPayment;
                }

                bool getAllowSalesDeposit() const
                {
                    return _allowSalesDeposit;
                }

                int getUploadBankCard() const
                {
                    return _uploadBankCard;
                }

                int getOnlineTransferCount() const
                {
                    return _onlineTransferCount;
                }

                bool getStartupQuotation() const
                {
                    return _startupQuotation;
                }




				void setAgentCode(const QString& value)
				{
					_agentCode = value;
				}

				void setAgentEmail(const QString& value)
				{
					_agentEmail = value;
				}

				void setAgentId(const QUuid& value)
				{
					_agentId = value;
				}

				void setAgentName(const QString& value)
				{
					_agentName = value;
				}

				void setAllowAddNewPosition(const bool& value)
				{
					_allowAddNewPosition = value;
				}

				void setAllowChangePasswordInTrader(const bool& value)
				{
					_allowChangePasswordInTrader = value;
				}

				void setAllowSalesTrading(const bool& value)
				{
					_allowSalesTrading = value;
				}

				void setAllowManagerTrading(const bool& value)
				{
					_allowManagerTrading = value;
				}

				void setBankAccountDefaultCountryId(const long& value)
				{
					_bankAccountDefaultCountryId = value;
				}

                void setDefaultPaymentCurrencyId(const QUuid& value)
                {
                    _defaultPaymentCurrencyId = value;
                }

				void setBeginTime(const QDateTime& value)
				{
					_beginTime = value;
				}

				void setCode(const QString& value)
				{
					_code = value;
				}

				void setConsumerCode(const QString& value)
				{
					_consumerCode = value;
				}

				void setCreditAmount(const QDecNumber& value)
				{
					_creditAmount = value;
				}

				void setCurrencyId(const QUuid& value)
				{
					_currencyId = value;
				}

				void setCustomerEmail(const QString& value)
				{
					_customerEmail = value;
				}

				void setCustomerId(const QUuid& value)
				{
					_customerId = value;
				}

				void setEnableAccountTransfer(const bool& value)
				{
					_enableAccountTransfer = value;
				}

				void setEnableAgentRegistration(const bool& value)
				{
					_enableAgentRegistration = value;
				}

				void setEnableCMExtension(const bool& value)
				{
					_enableCMExtension = value;
				}

				void setEnableOwnerRegistration(const bool& value)
				{
					_enableOwnerRegistration = value;
				}

				void setEnablePI(const bool& value)
				{
					_enablePI = value;
				}

				void setEnablePICash(const bool& value)
				{
					_enablePICash = value;
				}

				void setEnablePIInterACTransfer(const bool& value)
				{
					_enablePIInterACTransfer = value;
				}

                void setPrintOrderConfirmation(const bool& value)
                {
                    _printOrderConfirmation = value;
                }

				void setEndTime(const QDateTime& value)
				{
					_endTime = value;
				}

				void setGroupId(const QUuid& value)
				{
					_groupId = value;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setIsAutoClose(const bool& value)
				{
					_isAutoClose = value;
				}

				void setIsMultiCurrency(const bool& value)
				{
					_isMultiCurrency = value;
				}

				void setIsTradingAllowed(const bool& value)
				{
					_isTradingAllowed = value;
				}

				void setLeverage(const int& value)
				{
					_leverage = value;
				}

				void setName(const QString& value)
				{
					_name = value;
				}

				void setNeedBeneficiaryInPaymentInstruction(const bool& value)
				{
					_needBeneficiaryInPaymentInstruction = value;
				}

				void setOrganizationId(const QUuid& value)
				{
					_organizationId = value;
				}

				void setOrganizationCode(const QString& value)
				{
					_organizationCode = value;
				}
				
				void setOrganizationPhone(const QString& value)
				{
					_organizationPhone = value;
				}

				void setPaymentInstructionRemark(const QString& value)
				{
					_paymentInstructionRemark = value;
				}

				void setPublicQuotePolicyId(const QUuid& value)
				{
					_publicQuotePolicyId = value;
				}

				void setQuotePolicyId(const QUuid& value)
				{
					_quotePolicyId = value;
				}

				void setRateCommission(const QDecNumber& value)
				{
					_rateCommission = value;
				}

				void setRateDefaultLot(const QDecNumber& value)
				{
					_rateDefaultLot = value;
				}

				void setRateLevy(const QDecNumber& value)
				{
					_rateLevy = value;
				}

				void setRateLotMin(const QDecNumber& value)
				{
					_rateLotMin = value;
				}

				void setRateLotMultiplier(const QDecNumber& value)
				{
					_rateLotMultiplier = value;
				}

				void setRateMarginD(const QDecNumber& value)
				{
					_rateMarginD = value;
				}

				void setRateMarginLockD(const QDecNumber& value)
				{
					_rateMarginLockD = value;
				}

				void setRateMarginLockO(const QDecNumber& value)
				{
					_rateMarginLockO = value;
				}

				void setRateMarginO(const QDecNumber& value)
				{
					_rateMarginO = value;
				}

				void setSalesCode(const QString& value)
				{
					_salesCode = value;
				}

				void setSalesId(const QUuid& value)
				{
					_salesId = value;
				}

				void setTradePolicyId(const QUuid& value)
				{
					_tradePolicyId = value;
				}

				void setType(const Common::Enum::AccountType::AccountType& value)
				{
					_type = value;
				}

				void setUseQuotePolicyId(const QUuid& value)
				{
					_useQuotePolicyId = value;
				}

				void setUserRelation(const int& value)
				{
					_userRelation = value;
				}

				void setForbiddenAlert(const int& value)
				{
					_forbiddenAlert = value;
				}
				
				void setCounterPartiesMask(const int& value)
				{
					_counterPartiesMask = value;
				}

				void setShowWeightOnly(const bool& value)
				{
					_showWeightOnly = value;
				}

                void setAllowSalesPayment(const bool& value)
                {
                    _allowSalesPayment = value;
                }

                void setAllowSalesDeposit(const bool& value)
                {
                    _allowSalesDeposit = value;
                }

                void setUploadBankCard(const int& value)
                {
                    _uploadBankCard = value;
                }

                void setOnlineTransferCount(const int& value)
                {
                    _onlineTransferCount = value;
                }

                void setStartupQuotation(bool value)
                {
                    _startupQuotation = value;
                }

				

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_agentCode = attributes.value("AgentCode").toString();
					this->_agentEmail = attributes.value("AgentEmail").toString();
                    this->_agentId = attributes.value("AgentId").toString() != "" ?
                                QUuid(attributes.value("AgentId").toString()) : QUuid();
					this->_agentName = attributes.value("AgentName").toString();
					this->_allowAddNewPosition = Common::toBool(attributes.value("AllowAddNewPosition").toString());
					this->_allowChangePasswordInTrader = Common::toBool(attributes.value("AllowChangePasswordInTrader").toString());
					this->_allowManagerTrading = Common::toBool(attributes.value("AllowManagerTrading").toString());
					this->_allowSalesTrading = Common::toBool(attributes.value("AllowSalesTrading").toString());
                    this->_defaultPaymentCurrencyId = attributes.value("DefaultPaymentCurrencyID").toString() != "" ?
                                QUuid(attributes.value("DefaultPaymentCurrencyID").toString()) : QUuid();
                    this->_bankAccountDefaultCountryId = attributes.value("BankAccountDefaultCountryId").toLong();
					this->_beginTime = QDateTime::fromString(attributes.value("BeginTime").toString(), DateTimeConstants::DateTimeFormat);
					this->_code = attributes.value("Code").toString();
					this->_consumerCode = attributes.value("ConsumerCode").toString();
					this->_creditAmount = QDecNumber::fromQString(attributes.value("CreditAmount").toString());
					this->_currencyId = QUuid(attributes.value("CurrencyId").toString());
					this->_customerEmail = attributes.value("CustomerEmail").toString();
					this->_customerId = QUuid(attributes.value("CustomerId").toString());
					this->_enableAccountTransfer = Common::toBool(attributes.value("EnableAccountTransfer").toString());
					this->_enableAgentRegistration = Common::toBool(attributes.value("EnableAgentRegistration").toString());
					this->_enableCMExtension = Common::toBool(attributes.value("EnableCMExtension").toString());
					this->_enableOwnerRegistration = Common::toBool(attributes.value("EnableOwnerRegistration").toString());
					this->_enablePI = Common::toBool(attributes.value("EnablePI").toString());
					this->_enablePICash = Common::toBool(attributes.value("EnablePICash").toString());
					this->_enablePIInterACTransfer = Common::toBool(attributes.value("EnablePIInterACTransfer").toString());
                    this->_printOrderConfirmation = Common::toBool(attributes.value("PrintOrderConfirmation").toString());
					this->_endTime = QDateTime::fromString(attributes.value("EndTime").toString(), DateTimeConstants::DateTimeFormat);
					this->_groupId = QUuid(attributes.value("GroupId").toString());
					this->_id = QUuid(attributes.value("Id").toString());
					this->_isAutoClose = Common::toBool(attributes.value("IsAutoClose").toString());
					this->_isMultiCurrency = Common::toBool(attributes.value("IsMultiCurrency").toString());
					this->_isTradingAllowed = Common::toBool(attributes.value("IsTradingAllowed").toString());
					this->_leverage = attributes.value("Leverage").toInt();
					this->_name = attributes.value("Name").toString();
					this->_needBeneficiaryInPaymentInstruction = Common::toBool(attributes.value("NeedBeneficiaryInPaymentInstruction").toString());
					this->_organizationId = QUuid(attributes.value("OrganizationId").toString());					
					this->_organizationCode = attributes.value("OrganizationCode").toString();
					this->_organizationPhone = attributes.value("OrganizationPhone").toString();
					this->_paymentInstructionRemark = attributes.value("PaymentInstructionRemark").toString();
					this->_publicQuotePolicyId = QUuid(attributes.value("PublicQuotePolicyId").toString());
					this->_quotePolicyId = QUuid(attributes.value("QuotePolicyId").toString());
					this->_rateCommission = QDecNumber::fromQString(attributes.value("RateCommission").toString());
					this->_rateDefaultLot = QDecNumber::fromQString(attributes.value("RateDefaultLot").toString());
					this->_rateLevy = QDecNumber::fromQString(attributes.value("RateLevy").toString());
					this->_rateLotMin = QDecNumber::fromQString(attributes.value("RateLotMin").toString());
					this->_rateLotMultiplier = QDecNumber::fromQString(attributes.value("RateLotMultiplier").toString());
					this->_rateMarginD = QDecNumber::fromQString(attributes.value("RateMarginD").toString());
					this->_rateMarginLockD = QDecNumber::fromQString(attributes.value("RateMarginLockD").toString());
					this->_rateMarginLockO = QDecNumber::fromQString(attributes.value("RateMarginLockO").toString());
					this->_rateMarginO = QDecNumber::fromQString(attributes.value("RateMarginO").toString());
					this->_salesCode = attributes.value("SalesCode").toString();
					this->_salesId = QUuid(attributes.value("SalesId").toString());
					this->_tradePolicyId = QUuid(attributes.value("TradePolicyId").toString());
					this->_type = static_cast<AccountType::AccountType>(attributes.value("Type").toInt());
					this->_useQuotePolicyId = QUuid(attributes.value("UseQuotePolicyId").toString());
					this->_userRelation = attributes.value("UserRelation").toInt();
					this->_forbiddenAlert = attributes.value("ForbiddenAlert").toInt();					
					this->_counterPartiesMask = attributes.value("CounterPartiesMask").toInt();
					this->_showWeightOnly = Common::toBool(attributes.value("ShowWeightOnly").toString());
                    this->_allowSalesPayment = attributes.hasAttribute("AllowSalesPayment") ?
                                Common::toBool(attributes.value("AllowSalesPayment").toString()) : false;
                    this->_allowSalesDeposit = attributes.hasAttribute("AllowSalesDeposit") ?
                                Common::toBool(attributes.value("AllowSalesDeposit").toString()) : false;
                    this->_uploadBankCard = attributes.hasAttribute("UploadBankCard") ? attributes.value("UploadBankCard").toInt() : 0;
                    this->_onlineTransferCount = attributes.hasAttribute("OnlineTransferCount") ? attributes.value("OnlineTransferCount").toInt() : -1;
                    this->_startupQuotation = attributes.hasAttribute("StartupQuotation") ? Common::toBool(attributes.value("StartupQuotation").toString()) : true;
				}

			public:
				void update(const QDomNode& xmlNode)
				{
					QUuid userId = Common::TraderBaseInfo::getInstance()->getUserId();
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if (name == "AgentCode")
							this->_agentCode = value;
						else if (name == "AgentEmail")
							this->_agentEmail = value;
						else if (name == "AgentName")
							this->_agentName = value;
						else if (name == "AllowAddNewPosition")
							this->_allowAddNewPosition = Common::toBool(value);
						else if (name == "AllowChangePasswordInTrader")
							this->_allowChangePasswordInTrader = Common::toBool(value);
						else if (name == "AllowSalesTrading")
							this->_allowSalesTrading = Common::toBool(value);
						else if (name == "AllowManagerTrading")
							this->_allowManagerTrading = Common::toBool(value);
						else if (name == "BankAccountDefaultCountryId")
							this->_bankAccountDefaultCountryId = value.toLong();
                        else if (name == "DefaultPaymentCurrencyID")
                            this->_defaultPaymentCurrencyId = QUuid(value);
						else if (name == "BeginTime")
							this->_beginTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
						else if (name == "Code")
							this->_code = value;
						else if (name == "ConsumerCode")
							this->_consumerCode = value;
						else if (name == "CreditAmount")
							this->_creditAmount = QDecNumber::fromQString(value);
						else if (name == "CurrencyId")
							this->_currencyId = QUuid(value);
						else if (name == "CustomerEmail")
							this->_customerEmail = value;
						else if (name == "EnableAccountTransfer")
							this->_enableAccountTransfer = Common::toBool(value);
						else if (name == "EnableAgentRegistration")
							this->_enableAgentRegistration = Common::toBool(value);
						else if (name == "EnableCMExtension")
							this->_enableCMExtension = Common::toBool(value);
						else if (name == "EnableOwnerRegistration")
							this->_enableOwnerRegistration = Common::toBool(value);
						else if (name == "EnablePI")
							this->_enablePI = Common::toBool(value);
						else if (name == "EnablePICash")
							this->_enablePICash = Common::toBool(value);
						else if (name == "EnablePIInterACTransfer")
							this->_enablePIInterACTransfer = Common::toBool(value);
                        else if (name == "PrintOrderConfirmation")
                            this->_printOrderConfirmation = Common::toBool(value);
						else if (name == "EndTime")
							this->_endTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
						else if (name == "GroupId")
							this->_groupId = QUuid(value);
						else if (name == "Id")
							this->_id = QUuid(value);
						else if (name == "IsAutoClose")
							this->_isAutoClose = Common::toBool(value);
						else if (name == "IsMultiCurrency")
							this->_isMultiCurrency = Common::toBool(value);
						else if (name == "IsTradingAllowed")
							this->_isTradingAllowed = Common::toBool(value);
						else if (name == "Leverage")
							this->_leverage = value.toInt();
						else if (name == "Name")
							this->_name = value;
						else if (name == "NeedBeneficiaryInPaymentInstruction")
							this->_needBeneficiaryInPaymentInstruction = Common::toBool(value);
						else if (name == "OrganizationId")
							this->_organizationId = QUuid(value);
						else if (name == "OrganizationCode")
							this->_organizationCode = value;						
						else if (name == "OrganizationPhone")
							this->_organizationPhone = value;
						else if (name == "PaymentInstructionRemark")
							this->_paymentInstructionRemark = value;
						else if (name == "RateCommission")
							this->_rateCommission = QDecNumber::fromQString(value);
						else if (name == "RateDefaultLot")
							this->_rateDefaultLot = QDecNumber::fromQString(value);
						else if (name == "RateLevy")
							this->_rateLevy = QDecNumber::fromQString(value);
						else if (name == "RateLotMin")
							this->_rateLotMin = QDecNumber::fromQString(value);
						else if (name == "RateLotMultiplier")
							this->_rateLotMultiplier = QDecNumber::fromQString(value);
						else if (name == "RateMarginD")
							this->_rateMarginD = QDecNumber::fromQString(value);
						else if (name == "RateMarginLockD")
							this->_rateMarginLockD = QDecNumber::fromQString(value);
						else if (name == "RateMarginLockO")
							this->_rateMarginLockO = QDecNumber::fromQString(value);
						else if (name == "RateMarginO")
							this->_rateMarginO = QDecNumber::fromQString(value);
						else if (name == "SalesCode")
							this->_salesCode = value;
						else if (name == "Type")
							this->_type = static_cast<AccountType::AccountType>(value.toInt());
						else if (name == "ForbiddenAlert")
							this->_forbiddenAlert = value.toInt();		
						else if (name == "CounterPartiesMask")
							this->_counterPartiesMask = value.toInt();
						else if (name == "SalesId")
						{
							QUuid id = QUuid(value);
							if (this->_salesId != id)
							{
								this->_salesId = id;
								Common::SignalForwarder::getInstance()->sendRefreshSystemSignal();
								break;
							}
						}
						else if (name == "AgentId")
						{
							QUuid id = value != "" ? QUuid(value) : QUuid();
							if (this->_agentId != id && userId == this->_agentId)
							{
								this->_agentId = id;
								Common::SignalForwarder::getInstance()->sendRefreshSystemSignal();
								break;
							}
						}
						else if (name == "CustomerId")
						{
							QUuid id = QUuid(value);
							if (this->_customerId != id && userId == this->_customerId)
							{
								this->_customerId = id;
								Common::SignalForwarder::getInstance()->sendRefreshSystemSignal();
								break;
							}
						}
						else if (name == "TradePolicyId")
						{
							QUuid id = QUuid(value);
							if (this->_tradePolicyId != id)
							{
								this->_tradePolicyId = id;
								Common::SignalForwarder::getInstance()->sendRefreshSystemSignal();
								break;
							}
						}
						else if (name == "QuotePolicyId")
						{
							QUuid id = QUuid(value);
							if (this->_quotePolicyId != id)
							{
								this->_quotePolicyId = id;
								Common::SignalForwarder::getInstance()->sendRefreshSystemSignal();
								break;
							}
						}
						else if (name == "ShowWeightOnly")
							this->_showWeightOnly = Common::toBool(value);
                        else if (name == "AllowSalesPayment")
                            this->_allowSalesPayment = Common::toBool(value);
                        else if (name == "AllowSalesDeposit")
                            this->_allowSalesDeposit = Common::toBool(value);
                        else if (name == "UploadBankCard")
                            this->_uploadBankCard = value.toInt();
                        else if (name == "OnlineTransferCount")
                            this->_onlineTransferCount = value.toInt();
                        else if (name == "StartupQuotation")
                            this->_startupQuotation = Common::toBool(value);


					}
				}

			};

		}
	}
}
#endif

