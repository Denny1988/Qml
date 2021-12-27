#ifndef BUSINESS_MODEL_SETTING_MARGINSETTING_H
#define BUSINESS_MODEL_SETTING_MARGINSETTING_H
#include <QString>
#include <QDomNode>
#include "Common/Util/Convert_Common.h"
#include "SettingDataBase.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class MarginSetting: public SettingDataBase
			{
			private:
				QString  _customerEmail;
				QString _agentCode;
				QString  _agentName;
				QString  _agentEmail;
				QString  _organizationPhone;
                QUuid  _organizationId;
                QString  _organizationCode;
				QString  _organizationName;
				bool  _enableAgentRegistration;
				bool _enableCMExtension;
				bool _enableOwnerRegistration;
				bool _enablePI;
				bool _enablePICash;
				bool _enablePIInterACTransfer;
				bool _allowChangePasswordInTrader;
				bool _needBeneficiaryInPaymentInstruction;
				long _bankAccountDefaultCountryId;
                QUuid _defaultPaymentCurrencyId;
				bool _enableAccountTransfer;
				QString  _paymentInstructionRemark;
				bool _bankAccountNameMustSameWithAccountName;
				QString  _consumerCode;
                bool _allowSalesPayment;
                bool _allowSalesDeposit;
                int _uploadBankCard;
                int _onlineTransferCount;

			public:
				void update(const QDomNode& xmlMarginSetting)
				{
					QDomNamedNodeMap map = xmlMarginSetting.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = Common::fix(qDomNode.nodeValue());
						if(name == "CustomerEmail")
							this->_customerEmail = value;
						else if(name == "AgentCode")
							this->_agentCode = value;
						else if(name == "AgentName")
							this->_agentName = value;
						else if(name == "AgentEmail")
							this->_agentEmail = value;
						else if(name == "OrganizationPhone")
							this->_organizationPhone = value;
                        else if(name == "OrganizationId")
                            this->_organizationId = QUuid(value);
						else if(name == "OrganizationCode")
							this->_organizationCode = value;
						else if(name == "OrganizationName")
							this->_organizationName = value;
						else if(name == "EnableAgentRegistration")
							this->_enableAgentRegistration = Common::toBool(value);
						else if(name == "EnableCMExtension")
							this->_enableCMExtension = Common::toBool(value);	
						else if(name == "EnableOwnerRegistration")
							this->_enableOwnerRegistration = Common::toBool(value);	
						else if(name == "EnablePI")
							this->_enablePI = Common::toBool(value);	
						else if(name == "EnablePICash")
							this->_enablePICash = Common::toBool(value);	
						else if(name == "EnablePIInterACTransfer")
							this->_enablePIInterACTransfer = Common::toBool(value);	
						else if(name == "AllowChangePasswordInTrader")
							this->_allowChangePasswordInTrader = Common::toBool(value);	
						else if(name == "NeedBeneficiaryInPaymentInstruction")
							this->_needBeneficiaryInPaymentInstruction = Common::toBool(value);	
						else if(name == "BankAccountDefaultCountryId")
							this->_bankAccountDefaultCountryId = value.toLong();
                        else if(name == "DefaultPaymentCurrencyID")
                            this->_defaultPaymentCurrencyId = QUuid(value);
						else if(name == "PaymentInstructionRemark")
							this->_paymentInstructionRemark = value;
						else if(name == "BankAccountNameMustSameWithAccountName")
							this->_bankAccountNameMustSameWithAccountName = Common::toBool(value);	
						else if(name == "ConsumerCode")
							this->_consumerCode = value;
						else if(name == "EnableAccountTransfer")
							this->_enableAccountTransfer = Common::toBool(value);						

					}
				}

                QSharedPointer<MarginSetting> copy()
                {
                    QSharedPointer<MarginSetting> newObject(new MarginSetting);
                    newObject->_customerEmail = _customerEmail;
                    newObject->_agentCode = _agentCode;
                    newObject->_agentName = _agentName;
                    newObject->_agentEmail = _agentEmail;
                    newObject->_organizationPhone = _organizationPhone;
                    newObject->_organizationId = _organizationId;
                    newObject->_organizationCode = _organizationCode;
                    newObject->_organizationName = _organizationName;
                    newObject->_enableAgentRegistration = _enableAgentRegistration;
                    newObject->_enableCMExtension = _enableCMExtension;
                    newObject->_enableOwnerRegistration = _enableOwnerRegistration;
                    newObject->_enablePI = _enablePI;
                    newObject->_enablePICash = _enablePICash;
                    newObject->_enablePIInterACTransfer = _enablePIInterACTransfer;
                    newObject->_allowChangePasswordInTrader = _allowChangePasswordInTrader;
                    newObject->_needBeneficiaryInPaymentInstruction = _needBeneficiaryInPaymentInstruction;
                    newObject->_bankAccountDefaultCountryId = _bankAccountDefaultCountryId;
                    newObject->_defaultPaymentCurrencyId = _defaultPaymentCurrencyId;
                    newObject->_enableAccountTransfer = _enableAccountTransfer;
                    newObject->_paymentInstructionRemark = _paymentInstructionRemark;
                    newObject->_bankAccountNameMustSameWithAccountName = _bankAccountNameMustSameWithAccountName;
                    newObject->_consumerCode = _consumerCode;
                    newObject->_allowSalesPayment = _allowSalesPayment;
                    newObject->_allowSalesDeposit = _allowSalesDeposit;
                    newObject->_uploadBankCard = _uploadBankCard;
                    newObject->_onlineTransferCount = _onlineTransferCount;
                    return newObject;
                }

			public:			
				QString getCustomerEmail() const
				{
					return _customerEmail;
				}

				QString getAgentCode() const
				{
					return _agentCode;
				}

				QString getAgentName() const
				{
					return _agentName;
				}

				QString getAgentEmail() const
				{
					return _agentEmail;
				}

				QString getOrganizationPhone() const
				{
					return _organizationPhone;
				}

                QUuid getOrganizationId() const
                {
                    return _organizationId;
                }

				QString getOrganizationCode() const
				{
					return _organizationCode;
				}

				QString getOrganizationName() const
				{
					return _organizationName;
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

				bool getAllowChangePasswordInTrader() const
				{
					return _allowChangePasswordInTrader;
				}

				bool getNeedBeneficiaryInPaymentInstruction() const
				{
					return _needBeneficiaryInPaymentInstruction;
				}

				long getBankAccountDefaultCountryId() const
				{
					return _bankAccountDefaultCountryId;
				}

                QUuid getDefaultPaymentCurrencyId() const
                {
                    return _defaultPaymentCurrencyId;
                }

				QString getPaymentInstructionRemark() const
				{
					return _paymentInstructionRemark;
				}

				bool getBankAccountNameMustSameWithAccountName() const
				{
					return _bankAccountNameMustSameWithAccountName;
				}

				QString getConsumerCode() const
				{
					return _consumerCode;
				}

				bool getEnableAccountTransfer() const
				{
					return _enableAccountTransfer;
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


				void setAgentCode(const QString& value)
				{
					_agentCode = value;
				}

				void setAgentEmail(const QString& value)
				{
					_agentEmail = value;
				}

				void setAgentName(const QString& value)
				{
					_agentName = value;
				}

				void setAllowChangePasswordInTrader(const bool& value)
				{
					_allowChangePasswordInTrader = value;
				}

				void setBankAccountDefaultCountryId(const long& value)
				{
					_bankAccountDefaultCountryId = value;
				}

                void setDefaultPaymentCurrencyId(const QUuid& value)
                {
                    _defaultPaymentCurrencyId = value;
                }


				void setBankAccountNameMustSameWithAccountName(const bool& value)
				{
					_bankAccountNameMustSameWithAccountName = value;
				}

				void setConsumerCode(const QString& value)
				{
					_consumerCode = value;
				}

				void setCustomerEmail(const QString& value)
				{
					_customerEmail = value;
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

				void setOrganizationName(const QString& value)
				{
					_organizationName = value;
				}

				void setOrganizationPhone(const QString& value)
				{
					_organizationPhone = value;
				}

				void setPaymentInstructionRemark(const QString& value)
				{
					_paymentInstructionRemark = value;
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

				
				
				bool canSubmitMargin()
				{
					return this->_enablePICash || this->_enablePIInterACTransfer || this->_enablePI
                        || this->_enableCMExtension || this->_enableAgentRegistration  || this->_enableAccountTransfer;
				}



			};
		}
	}
}
#endif

