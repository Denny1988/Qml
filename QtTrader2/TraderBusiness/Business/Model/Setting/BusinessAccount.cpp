#include "BusinessAccount.h"
#include "Common/Util/Convert_Common.h"
#include "Common/SignalForwarder.h"
#include "Common/Enum/UIEnum.h"
#include <QDomNode>
#include <QList>
#include "Common/LogHelper.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			BusinessAccount::BusinessAccount()
			{
				QSharedPointer<Model::Setting::MarginSetting> marginSetting(new Model::Setting::MarginSetting);
				this->_marginSetting = marginSetting;
				QSharedPointer<Model::Setting::LeverageSetting> leverageSetting(new Model::Setting::LeverageSetting);
				this->_leverageSetting = leverageSetting;
                this->_printOrderConfirmation = false;
			}


			BusinessAccount::~BusinessAccount()
			{
			}

			QSharedPointer<BusinessInstrument> BusinessAccount::getInstrument(const QUuid& id) const
			{
                //int n = _instruments.size();
				if (this->_instruments.contains(id))
				{
					return this->_instruments.value(id);
				}
				else
				{
					QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "MethodName:BusinessAccount::getInstrument  Info:Instrument non-existent! id:" + id.toString();
					QSharedPointer<Setting::BusinessInstrument> newInstrument(Q_NULLPTR);
					return newInstrument;
				}
			}

			QList<QUuid> BusinessAccount::getInstrumentIds()
			{
				return  this->_instruments.keys();
			}

			void BusinessAccount::addInstrument(const QUuid& id, const QSharedPointer<BusinessInstrument>& instrument)
			{
				if (!this->_instruments.contains(id))
				{
					this->_instruments.insert(id, instrument);
				}
				else
					this->_instruments[id] = instrument;
			}

			bool BusinessAccount::containsInstrument(const QUuid& id)
			{
				return this->_instruments.contains(id);
			}

			QSharedPointer<BusinessInstrument>& BusinessAccount::getOrAddInstrument(const QUuid& id)
			{
				if (!this->_instruments.contains(id))
				{
					QSharedPointer<BusinessInstrument> instrument(new BusinessInstrument);
					this->_instruments.insert(id, instrument); 
				}
				return this->_instruments[id];
			}

			void BusinessAccount::update(const QDomNode& xmlBusinessAccount)
			{
				QDomNamedNodeMap map = xmlBusinessAccount.attributes();
				for (int i = 0; i < map.count(); i++)
				{
					QDomNode qDomNode = map.item(i);
					QString name = qDomNode.nodeName();
					QString value = Common::fix(qDomNode.nodeValue());
					if (name == "Id")
						this->_id = QUuid(value);
					else if (name == "Code")
						this->_code = value;
					else if (name == "Name")
						this->_name = value;
					else if (name == "SalesCode")
						this->_salesCode = value;
					else if (name == "IsMultiCurrency")
						this->_isMultiCurrency = Common::toBool(value);
					else if (name == "DisplayAlert")
						this->_displayAlert = value.toInt();
					else if (name == "BalanceDeficitAllowPay")
						this->_balanceDeficitAllowPay = Common::toBool(value);
					else if (name == "EnableMarginPin")
						this->_enableMarginPin = Common::toBool(value);
					else if (name == "EnableModifyTelephoneIdentificationCode")
						this->_enableModifyTelephoneIdentificationCode = Common::toBool(value);
					else if (name == "Type")
						this->_accountType = static_cast<Common::Enum::AccountType::AccountType>(value.toInt());
					else if (name == "IsDisplayLedger")
					{
						this->_isDisplayLedger = Common::toBool(value);
						//Common::SignalForwarder::getInstance()->sendIsShowSytemMenuSignal(Common::Enum::MainSytemMenu::Ledger, this->_isDisplayLedger);
					}
					else if (name == "CurrencyId")
						this->_currencyId = QUuid(value);
					else if (name == "CreditAmount")
					{
						this->_creditAmount = QDecNumber::fromQString(value);
						Common::SignalForwarder::getInstance()->sendComputeAccountSignal(this->_id);
					}
					else if (name == "AllowedTrade")
						this->_allowedTrade = Common::toBool(value);
					else if (name == "QuotePolicyId")
					{
						this->_quotePolicyId = QUuid(value);
					}
					else if (name == "PublicQuotePolicyId")
						this->_publicQuotePolicyId = QUuid(value);
					else if (name == "CustomerId")
						this->_customerId = QUuid(value);
					else if (name == "BeginTime")
						this->_beginTime = Common::convertToDateTime(value);
					else if (name == "EndTime")
						this->_endTime = Common::convertToDateTime(value);
				}

				QDomNodeList childlist = xmlBusinessAccount.childNodes();
				for (int i = 0; i < childlist.count(); i++)
				{
					QDomNode itemQDomNode = childlist.item(i);
					QString name = itemQDomNode.nodeName();
					if (name == "MarginSetting")
					{
						if (this->_marginSetting.isNull())
						{
							QSharedPointer<Model::Setting::MarginSetting> marginSetting(new Model::Setting::MarginSetting);
							this->_marginSetting = marginSetting;
						}
						this->_marginSetting->update(itemQDomNode);
					}
					else if (name == "LeverageSetting")
					{
						if (this->_leverageSetting.isNull())
						{
							QSharedPointer<Model::Setting::LeverageSetting> leverageSetting(new Model::Setting::LeverageSetting);
							this->_leverageSetting = leverageSetting;
						}
						this->_leverageSetting->update(itemQDomNode);
					}
					else if (name == "Instrument")
					{
						QUuid id = QUuid(itemQDomNode.toElement().attribute("Id"));
						QSharedPointer<Model::Setting::BusinessInstrument> instrument = this->getOrAddInstrument(id);
						instrument->update(itemQDomNode);
					}
				}
			}

			void BusinessAccount::updateLeverage(int leverage)
			{
				this->_leverageSetting->setLeverage(leverage);
			}

			bool BusinessAccount::canSubmitMargin()
			{
				return this->_marginSetting->canSubmitMargin();
			}

            QSharedPointer<BusinessAccount> BusinessAccount::copy()
            {
                QSharedPointer<BusinessAccount> newObject(new BusinessAccount);
                newObject->_id = _id;
                newObject->_code = _code;
                newObject->_name = _name;
                newObject->_salesId = _salesId;
                newObject->_salesCode = _salesCode;
                newObject->_agentId = _agentId;
                newObject->_isMultiCurrency = _isMultiCurrency;
                newObject->_displayAlert = _displayAlert;
                newObject->_balanceDeficitAllowPay = _balanceDeficitAllowPay;
                newObject->_enableMarginPin = _enableMarginPin;
                newObject->_enableModifyTelephoneIdentificationCode = _enableModifyTelephoneIdentificationCode;
                newObject->_isDisplayLedger = _isDisplayLedger;
                newObject->_accountType = _accountType;
                newObject->_currencyId = _currencyId;
                newObject->_creditAmount = _creditAmount;
                newObject->_allowedTrade = _allowedTrade;
                newObject->_quotePolicyId = _quotePolicyId;
                newObject->_publicQuotePolicyId = _publicQuotePolicyId;
                newObject->_customerId = _customerId;
                newObject->_beginTime = _beginTime;
                newObject->_endTime = _endTime;
                newObject->_forbiddenAlert = _forbiddenAlert;
                newObject->_counterPartiesMask = _counterPartiesMask;
                newObject->_organizationCode = _organizationCode;
                newObject->_showWeightOnly = _showWeightOnly;
                newObject->_printOrderConfirmation = _printOrderConfirmation;

                if(!this->_marginSetting.isNull())
                {
                    newObject->_marginSetting = this->_marginSetting->copy();
                }
                if(!this->_leverageSetting.isNull())
                {
                    newObject->_leverageSetting = this->_leverageSetting->copy();
                }

                if(this->_instruments.count() > 0)
                {
                    QMap<QUuid, QSharedPointer<BusinessInstrument>> copyInstruments;
                    QMapIterator<QUuid, QSharedPointer<BusinessInstrument>> item(this->_instruments);
                    while (item.hasNext()) {
                        item.next();
                        copyInstruments.insert(item.key(), item.value()->copy());
                    }
                    newObject->_instruments = copyInstruments;
                }
                return newObject;
            }

		}
	}
}
