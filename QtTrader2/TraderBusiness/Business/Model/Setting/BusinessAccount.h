#ifndef BUSINESS_MODEL_SETTING_SETTINGACCOUNT_H
#define BUSINESS_MODEL_SETTING_SETTINGACCOUNT_H
#include <quuid.h>
#include <qdatetime.h>
#include "Currency.h"
#include "LeverageSetting.h"
#include "BusinessInstrument.h"
#include "MarginSetting.h"
#include "Common/Enum/AccountEnum.h"
#include <QMap>
#include <QDateTime>
#include <QDomNode>
#include "traderbusiness_global.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class TRADERBUSINESS_EXPORT BusinessAccount
			{
			public:
				BusinessAccount();
				~BusinessAccount();

			private:
				QUuid  _id;
				QString  _code;
				QString  _name;
				QUuid  _salesId;
				QString  _salesCode;
				QUuid _agentId;
				bool  _isMultiCurrency;
				int   _displayAlert;
				bool  _balanceDeficitAllowPay;
				bool  _enableMarginPin;
				bool  _enableModifyTelephoneIdentificationCode;
				bool  _isDisplayLedger;
				Common::Enum::AccountType::AccountType  _accountType;
				QUuid  _currencyId;
				QDecNumber _creditAmount;
				bool _allowedTrade;
				QUuid  _quotePolicyId;
				QUuid  _publicQuotePolicyId;
				QUuid  _customerId;
				QDateTime _beginTime;
				QDateTime _endTime;
				int _forbiddenAlert;
				int _counterPartiesMask;
				QString  _organizationCode;
				bool _showWeightOnly;
                bool _printOrderConfirmation;
                bool _startupQuotation;

				QMap<QUuid, QSharedPointer<BusinessInstrument>> _instruments;
				QSharedPointer<MarginSetting> _marginSetting;
				QSharedPointer<LeverageSetting> _leverageSetting;

			public:
				QList<QUuid> getInstrumentIds();
				QSharedPointer<BusinessInstrument> getInstrument(const QUuid& id) const;
				void addInstrument(const QUuid& id, const QSharedPointer<BusinessInstrument>& instrument);
				bool containsInstrument(const QUuid& id);
				QSharedPointer<BusinessInstrument>& getOrAddInstrument(const QUuid& id);
				void update(const QDomNode& xmlAccount);
				QList<QSharedPointer<BusinessInstrument>> getInstruments() { return _instruments.values(); }
				void updateLeverage(int leverage);
				bool canSubmitMargin();
                QSharedPointer<BusinessAccount> copy();

			public:
				QUuid getId() const
				{
					return _id;
				}

				QString getCode() const
				{
					return _code;
				}

				QString getName() const
				{
					return _name;
				}

				QUuid getSalesId() const
				{
					return _salesId;
				}				

				QString getSalesCode() const
				{
					return _salesCode;
				}

				QUuid getAgentId() const
				{
					return _agentId;
				}

				bool getIsMultiCurrency() const
				{
					return _isMultiCurrency;
				}

				int getDisplayAlert() const
				{
					return _displayAlert;
				}

				bool getBalanceDeficitAllowPay() const
				{
					return _balanceDeficitAllowPay;
				}

				bool getEnableMarginPin() const
				{
					return _enableMarginPin;
				}

				bool  getEnableModifyTelephoneIdentificationCode() const
				{
					return _enableModifyTelephoneIdentificationCode;
				}

				bool getIsDisplayLedger() const
				{
					return _isDisplayLedger;
				}

				Common::Enum::AccountType::AccountType getAccountType() const
				{
					return _accountType;
				}

				QUuid getCurrencyId() const
				{
					return _currencyId;
				}

				QDecNumber getCreditAmount() const
				{
					return _creditAmount;
				}

				bool getAllowedTrade() const
				{
					return _allowedTrade;
				}

				QUuid  getQuotePolicyId()
				{
					return _quotePolicyId;
				}

				QUuid  getPublicQuotePolicyId()
				{
					return _publicQuotePolicyId;
				}

				QUuid  getCustomerId()
				{
					return _customerId;
				}

				QDateTime getBeginTime()
				{
					return _beginTime;
				}

				QDateTime getEndTime()
				{
					return _endTime;
				}

				QSharedPointer<MarginSetting>& getMarginSetting()
				{
					return _marginSetting;
				}

				QSharedPointer<LeverageSetting>& getLeverageSetting()
				{
					return _leverageSetting;
				}

				QSharedPointer<BusinessInstrument> getFirstInstrument()
				{
					return _instruments.first();
				}

				int getForbiddenAlert() const
				{
					return _forbiddenAlert;
				}

				int getCounterPartiesMask() const
				{
					return _counterPartiesMask;
				}

				QString getOrganizationCode() const
				{
					return _organizationCode;
				}

                bool getPrintOrderConfirmation() const
                {
                    return _printOrderConfirmation;
                }

				bool getShowWeightOnly() const
				{
					return _showWeightOnly;
				}

                bool getStartupQuotation() const
                {
                    return _startupQuotation;
                }




				void setAccountType(const Common::Enum::AccountType::AccountType& value)
				{
					_accountType = value;
				}

				void setAllowedTrade(const bool& value)
				{
					_allowedTrade = value;
				}

				void setBalanceDeficitAllowPay(const bool& value)
				{
					_balanceDeficitAllowPay = value;
				}

				void setBeginTime(const QDateTime& value)
				{
					_beginTime = value;
				}

				void setCode(const QString& value)
				{
					_code = value;
				}

				void setCreditAmount(const QDecNumber& value)
				{
					_creditAmount = value;
				}

				void setCurrencyId(const QUuid& value)
				{
					_currencyId = value;
				}

				void setCustomerId(const QUuid& value)
				{
					_customerId = value;
				}

				void setDisplayAlert(const int& value)
				{
					_displayAlert = value;
				}

				void setEnableMarginPin(const bool& value)
				{
					_enableMarginPin = value;
				}

				void setEnableModifyTelephoneIdentificationCode(const bool& value)
				{
					_enableModifyTelephoneIdentificationCode = value;
				}

				void setEndTime(const QDateTime& value)
				{
					_endTime = value;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setIsDisplayLedger(const bool& value)
				{
					_isDisplayLedger = value;
				}

				void setIsMultiCurrency(const bool& value)
				{
					_isMultiCurrency = value;
				}

				void setName(const QString& value)
				{
					_name = value;
				}

				void setPublicQuotePolicyId(const QUuid& value)
				{
					_publicQuotePolicyId = value;
				}

				void setQuotePolicyId(const QUuid& value)
				{
					_quotePolicyId = value;
				}

				void setSales(const QUuid& id, const QString& value)
				{
					_salesId = id;
					_salesCode = value;
				}

				void setAgentId(const QUuid& value)
				{
					_agentId = value;
				}

				void setForbiddenAlert(const int& value)
				{
					_forbiddenAlert = value;
				}

				void setCounterPartiesMask(const int& value)
				{
					_counterPartiesMask = value;
				}

				void setOrganizationCode(const QString& value)
				{
					_organizationCode = value;
				}

                void setPrintOrderConfirmation(const bool& value)
                {
                    _printOrderConfirmation = value;
                }

				void setShowWeightOnly(const bool& value)
				{
					_showWeightOnly = value;
				}

                void setStartupQuotation(const bool& value)
                {
                    _startupQuotation = value;
                }


			};
		}
	}
}

#endif
