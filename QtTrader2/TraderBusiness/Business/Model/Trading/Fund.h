#ifndef BUSINESS_MODEL_TRADING_FUND_H
#define BUSINESS_MODEL_TRADING_FUND_H
#include <quuid.h>
#include "QDecNumber.h"
#include <QDomNode>
#include "Common/Enum/CommonEnum.h"
#include "Business/Model/Computing/ComputingResult.h"
#include "Business/Model/Setting/CurrencyRate.h"
#include <QXmlStreamReader>
#include "Common/TraderBaseInfo.h"
#include "traderbusiness_global.h"
#include "Common/LogHelper.h"

using namespace Business::Model::Computing;
using namespace Common::Enum;
namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			class TRADERBUSINESS_EXPORT Fund
			{
			public:
				Fund()
				{
					this->_decimals = 0;
					this->_alertLevel = AlertLevel::Level0;
				}

				~Fund() {}

			private:
				QUuid  _accountId;
				QUuid  _currencyId;
				QString _currencyCode;
				QString _currencyName;
				int		_decimals;
				QDecNumber  _balance;
				QDecNumber  _totalPaidAmount;
				QDecNumber  _totalUnPaidAmount;
				QDecNumber  _pledgeAmount;
				QDecNumber  _necessary;
				QDecNumber  _partialPaymentPhysicalNecessary;
				QDecNumber  _totalNecessary;
				QDecNumber  _frozenFund;
				QDecNumber  _unclearAmount;
				QDecNumber  _creditAmount;
				QDecNumber  _estimateCloseCommission;
				QDecNumber  _estimateCloseLevy;

				QDecNumber  _interestPLNotValued;
				QDecNumber  _storagePLNotValued;
				QDecNumber  _tradePLNotValued;
				QDecNumber  _interestPLFloat;
				QDecNumber  _storagePLFloat;
				QDecNumber  _tradePLFloat;
				AlertLevel::AlertLevel  _alertLevel;

				bool _ratioIncludeCredit;
				int _displayAlert;

			public:
				void update(const QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_accountId = QUuid(attributes.value("ID").toString());
					QString b = attributes.value("Balance").toString();
					this->_balance = attributes.value("Balance").toDouble();
					this->_totalNecessary = attributes.value("Necessary").toDouble();
					this->_pledgeAmount = attributes.value("ValueAsMargin").toDouble();
					this->_frozenFund = attributes.value("FrozenFund").toDouble();
					this->_totalPaidAmount = attributes.value("TotalPaidAmount").toDouble();
					this->_totalUnPaidAmount = attributes.value("TotalUnPaidAmount").toDouble();
					this->_partialPaymentPhysicalNecessary = attributes.value("PartialPaymentPhysicalNecessary").toDouble();
					this->_interestPLNotValued = attributes.value("InterestPLNotValued").toDouble();
					this->_storagePLNotValued = attributes.value("StoragePLNotValued").toDouble();
					this->_tradePLNotValued = attributes.value("TradePLNotValued").toDouble();
					this->_interestPLFloat = attributes.value("InterestPLFloat").toDouble();
					this->_storagePLFloat = attributes.value("StoragePLFloat").toDouble();
					this->_tradePLFloat = attributes.value("TradePLFloat").toDouble();
					this->_alertLevel = static_cast<AlertLevel::AlertLevel>(attributes.value("AlertLevel").toInt());
					this->_unclearAmount = attributes.value("UnclearAmount").toDouble();
					this->_estimateCloseCommission -= (attributes.value("EstimateCloseCommission").toDouble());
					this->_estimateCloseLevy -= (attributes.value("EstimateCloseLevy").toDouble());
					_necessary = _totalNecessary - _partialPaymentPhysicalNecessary;
				}

				void update(const QDomNode& xmlAccountBalance)
				{
					QDomNamedNodeMap map = xmlAccountBalance.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if (name == "ID")
							this->_accountId = QUuid(value);
						else if (name == "Balance")
							this->_balance = value.toDouble();
						else if (name == "Necessary")
							this->_totalNecessary = value.toDouble();
						else if (name == "ValueAsMargin")
							this->_pledgeAmount = value.toDouble();
						else if (name == "FrozenFund")
							this->_frozenFund = value.toDouble();
						else if (name == "TotalPaidAmount")
							this->_totalPaidAmount = value.toDouble();
						else if (name == "TotalUnPaidAmount")
							this->_totalUnPaidAmount = value.toDouble();
						else if (name == "PartialPaymentPhysicalNecessary")
							this->_partialPaymentPhysicalNecessary = value.toDouble();
						else if (name == "InterestPLNotValued")
							this->_interestPLNotValued = value.toDouble();
						else if (name == "StoragePLNotValued")
							this->_storagePLNotValued = value.toDouble();
						else if (name == "TradePLNotValued")
							this->_tradePLNotValued = value.toDouble();
						else if (name == "InterestPLFloat")
							this->_interestPLFloat = value.toDouble();
						else if (name == "StoragePLFloat")
							this->_storagePLFloat = value.toDouble();
						else if (name == "TradePLFloat")
							this->_tradePLFloat = value.toDouble();
						else if (name == "AlertLevel")
							this->_alertLevel = static_cast<AlertLevel::AlertLevel>(value.toInt());
						else if (name == "UnclearAmount")
							this->_unclearAmount = value.toDouble();
						else if (name == "EstimateCloseCommission")
							this->_estimateCloseCommission = value.toDouble();
						else if (name == "EstimateCloseLevy")
							this->_estimateCloseLevy = value.toDouble();
					}
					_necessary = _totalNecessary - _partialPaymentPhysicalNecessary;
				}

				void updateChangeFund(QSharedPointer<ChangeFund> fund)
				{
					_necessary = fund->necessary;
					_partialPaymentPhysicalNecessary = fund->partialPaymentPhysicalNecessary;
					_totalNecessary = _necessary + _partialPaymentPhysicalNecessary;
					_interestPLFloat = fund->interestPLFloat;
					_storagePLFloat = fund->storagePLFloat;
					_tradePLFloat = fund->tradePLFloat;
					_pledgeAmount = fund->pledgeAmount;
					_totalPaidAmount = fund->totalPaidAmount;
					_totalUnPaidAmount = fund->totalUnpaidAmount;
					_estimateCloseCommission = fund->estimateCloseCommission;
					_estimateCloseLevy = fund->estimateCloseLevy;
				}

				QDecNumber getUITradePLFloat()
				{
					return Common::roundToDecimal(_tradePLFloat.toDouble() + _interestPLFloat.toDouble() + _storagePLFloat.toDouble(), _decimals);
				}

				QDecNumber getTotalUnrealisedSwap()
				{
					return Common::roundToDecimal(_interestPLNotValued.toDouble() + _storagePLNotValued.toDouble(), _decimals);
				}

				QDecNumber getUnrealisedPL()
				{
					return _tradePLNotValued;
				}

				QDecNumber getTotalNecessary()
				{
					return Common::roundToDecimal(_totalNecessary, _decimals);
				}

				QDecNumber getEquity()
				{
					QDecNumber newEquity = _balance.toDouble() + _interestPLFloat.toDouble() + _storagePLFloat.toDouble() + _tradePLFloat.toDouble()
						+ _tradePLNotValued.toDouble() + _interestPLNotValued.toDouble() + _storagePLNotValued.toDouble()
						+ _pledgeAmount.toDouble() + _totalPaidAmount.toDouble() + _estimateCloseCommission.toDouble() + _estimateCloseLevy.toDouble();

					/*double a = newEquity.toDouble();
					double balance = _balance.toDouble();
					double interestPLFloat = _interestPLFloat.toDouble();
					double storagePLFloat = _storagePLFloat.toDouble();
					double tradePLFloat = _tradePLFloat.toDouble();
					double tradePLNotValued = _tradePLNotValued.toDouble();
					double interestPLNotValued = _interestPLNotValued.toDouble();
					double storagePLNotValued = _storagePLNotValued.toDouble();
					double pledgeAmount = _pledgeAmount.toDouble();
					double totalPaidAmount = _totalPaidAmount.toDouble();
					double estimateCloseCommission = _estimateCloseCommission.toDouble();
					double estimateCloseLevy = _estimateCloseLevy.toDouble();*/

					return Common::roundToDecimal(newEquity, _decimals);
				}

				QDecNumber getNetAsset()
				{
					return Common::roundToDecimal(_creditAmount + getEquity(), _decimals);
				}

				QDecNumber getUsable()
				{
                    return Common::roundToDecimal((Common::TraderBaseInfo::getInstance()->getEnableNetAsset() ? getNetAsset() : getEquity()) - getTotalNecessary(), _decimals);
				}


				AlertLevel::AlertLevel getAlertLevel()
				{
					return _alertLevel;
				}

				int getDecimals()
				{
					return _decimals;
				}

				QDecNumber getRatio()
				{
					QDecNumber equity = getEquity();
					if (this->_ratioIncludeCredit)
					{
						equity += this->_creditAmount;
					}
					return getTotalNecessary() == 0.0 ? 0.0 : (equity / getTotalNecessary()).toDouble();
				}

				bool needAlert()
				{
					return needAlert(_displayAlert);
				}

				bool needAlert(int displayAlert)
				{
					if (displayAlert != 0)
					{
						if (((_alertLevel == AlertLevel::Level1) && (displayAlert == 1 || displayAlert == 2 || displayAlert == 3 || displayAlert == 4))
							|| ((_alertLevel == AlertLevel::Level2) && (displayAlert == 2 || displayAlert == 4 || displayAlert == 5 || displayAlert == 6))
							|| ((_alertLevel == AlertLevel::Level3) && (displayAlert == 3 || displayAlert == 4 || displayAlert == 6 || displayAlert == 7)))
						{
							return true;
						}
					}
					return false;
				}

				

				void setCreditAmount(QDecNumber value)
				{
					_creditAmount = value;
				}

				void setRatioIncludeCredit(bool value)
				{
					_ratioIncludeCredit = value;
				}

				void setCurrencyCode(QUuid id, QString value, QString name)
				{
					_currencyId = id;
					_currencyCode = value;
					_currencyName = name;
				}

				void setDecimals(int value)
				{
					_decimals = value;
				}

				void setDisplayAlert(int value)
				{
					_displayAlert = value;
				}

				void addBalance(QDecNumber value)
				{
					_balance += value;
				}

				bool hasSufficientUsableMargin(bool allowPayWhenBalanceDeficit, QSharedPointer<Business::Model::Setting::CurrencyRate> currencyRate, QDecNumber amount)
				{
					QDecNumber equity = getEquity();
					if (!currencyRate.isNull())
					{
						amount = currencyRate->exchange(amount);
					}
                    QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "Fund::hasSufficientUsableMargin "
                           << " amount:"<< amount.toDouble()
                           << " equity:"<< equity.toDouble()
                           << " TotalNecessary:"<< getTotalNecessary().toDouble()
                           << " unclearAmount:"<< _unclearAmount.toDouble()
                           << " partialPaymentPhysicalNecessary:"<< _partialPaymentPhysicalNecessary.toDouble()
                           << " balance:"<< _balance.toDouble()
                           << " allowPayWhenBalanceDeficit:" << allowPayWhenBalanceDeficit;
					/*double a1 = amount.toDouble();
					double a2 = equity.toDouble();
					double a3 = getTotalNecessary().toDouble();
					double a4 = _unclearAmount.toDouble();
					double a5 = _balance.toDouble();
					double a6 = getTotalNecessary().toDouble();
					double a7 =_partialPaymentPhysicalNecessary.toDouble();*/
					if ((equity - getTotalNecessary() - _unclearAmount) - amount < 0.0 ||
						(!allowPayWhenBalanceDeficit &&
						(_balance - (getTotalNecessary() - _partialPaymentPhysicalNecessary) - _unclearAmount) - amount < 0.0))
					{
						return false;
					}
					else
					{
						return true;
					}
				}

				QString getBalanceFormat()
				{
					return Common::roundToString(_balance.toDouble(), _decimals, true);
				}

				QString getNecessaryFormat()
				{
					return Common::TraderBaseInfo::getInstance()->getOnlyExistsBOInstrument() ? "0.0" : Common::roundToString(_necessary.toDouble(), _decimals, true);
				}

				QString getPartialPaymentPhysicalNecessaryFormat()
				{
					return Common::TraderBaseInfo::getInstance()->getOnlyExistsBOInstrument() ? "0.0" : Common::roundToString(_partialPaymentPhysicalNecessary.toDouble(), _decimals, true);
				}

				QString getTotalUnrealisedSwapFormat()
				{
					return Common::roundToString(getTotalUnrealisedSwap().toDouble(), _decimals, true);
				}

				QString getUITradePLFloatFormat()
				{
					return Common::TraderBaseInfo::getInstance()->getOnlyExistsBOInstrument() ? "0.0" : Common::roundToString(getUITradePLFloat().toDouble(), _decimals, true);
				}

				QString getUnrealisedPLFormat()
				{
					return Common::roundToString(_tradePLNotValued.toDouble(), _decimals, true);
				}

				QString getUnclearAmountFormat()
				{
					return Common::roundToString(_unclearAmount.toDouble(), _decimals, true);
				}

				QString getEquityFormat()
				{
					return Common::TraderBaseInfo::getInstance()->getOnlyExistsBOInstrument() ? "0.0" : Common::roundToString(getEquity().toDouble(), _decimals, true);
				}

				QString getNetAssetFormat()
				{
					if (!Common::TraderBaseInfo::getInstance()->getEnableNetAsset())
						return "0.0";
					else
						return Common::roundToString(getNetAsset().toDouble(), _decimals, true);
				}

				QString getUsableFormat()
				{
                    if (Common::TraderBaseInfo::getInstance()->getOnlyExistsBOInstrument())
						return "0.0";
					else
						return Common::roundToString(getUsable().toDouble(), _decimals, true);
				}

				QString getPledgeAmountFormat()
				{
					return Common::roundToString(_pledgeAmount.toDouble(), _decimals, true);
				}

				QString getCreditAmountFormat()
				{
					return Common::roundToString(_creditAmount.toDouble(), _decimals, true);
				}

				QString getFrozenFundFormat()
				{
					return Common::roundToString(_frozenFund.toDouble(), _decimals, true);
				}

				QString getTotalPaidAmountFormat()
				{
					return Common::roundToString(_totalPaidAmount.toDouble(), _decimals, true);
				}

				QString getTotalUnPaidAmountFormat()
				{
					return Common::roundToString(_totalUnPaidAmount.toDouble(), _decimals, true);
				}

				QString getRatioFormat()
				{
					return Common::TraderBaseInfo::getInstance()->getOnlyExistsBOInstrument() ? "0.0" : Common::roundToPercentageString(this->getRatio().toDouble(), _decimals);
				}

				QString getEstimateCloseCommissionFormat()
				{
					return Common::TraderBaseInfo::getInstance()->getOnlyExistsBOInstrument() ? "0.0" : Common::roundToString(_estimateCloseCommission.toDouble(), _decimals, true);
				}

				QString getEstimateCloseLevyFormat()
				{
					return Common::TraderBaseInfo::getInstance()->getOnlyExistsBOInstrument() ? "0.0" : Common::roundToString(_estimateCloseLevy.toDouble(), _decimals, true);
				}

			public:
                QUuid getAccountId() const
                {
                    return _accountId;
                }

				QUuid getCurrencyId() const
				{
					return _currencyId;
				}

				QString getCurrencyCode() const
				{
					return _currencyCode;
				}

				QString getCurrencyName() const
				{
					return _currencyName;
				}

				QDecNumber getBalance() const
				{
					return _balance;
				}

				QDecNumber getNecessary() const
				{
					return _necessary;
				}

				QDecNumber getPledgeAmount() const
				{
					return _pledgeAmount;
				}

				QDecNumber getFrozenFund() const
				{
					return _frozenFund;
				}

				QDecNumber getTotalPaidAmount() const
				{
					return _totalPaidAmount;
				}

				QDecNumber getTotalUnPaidAmount() const
				{
					return _totalUnPaidAmount;
				}

				QDecNumber getPartialPaymentPhysicalNecessary() const
				{
					return _partialPaymentPhysicalNecessary;
				}

				QDecNumber getInterestPLNotValued() const
				{
					return _interestPLNotValued;
				}

				QDecNumber getStoragePLNotValued() const
				{
					return _storagePLNotValued;
				}

				QDecNumber getTradePLNotValued() const
				{
					return _tradePLNotValued;
				}

				QDecNumber getInterestPLFloat() const
				{
					return _interestPLFloat;
				}

				QDecNumber getStoragePLFloat() const
				{
					return _storagePLFloat;
				}

				QDecNumber getTradePLFloat() const
				{
					return _tradePLFloat;
				}

				QDecNumber getUnclearAmount() const
				{
					return _unclearAmount;
				}

				QDecNumber getCreditAmount() const
				{
					return _creditAmount;
				}

				QDecNumber getEstimateCloseCommission()
				{
					return _estimateCloseCommission;
				}

				QDecNumber getEstimateCloseLevy()
				{
					return _estimateCloseLevy;
				}

			};
		}
	}
}
#endif
