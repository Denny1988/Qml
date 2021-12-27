#ifndef BUSINESS_MODEL_TRADING_SUBFUND_H
#define BUSINESS_MODEL_TRADING_SUBFUND_H
#include <quuid.h>
#include "QDecNumber.h"
#include <QDomNode>
#include <QXmlStreamReader>
#include "Business/Model/Computing/ComputingResult.h"
#include "Common/TraderBaseInfo.h"
#include "Common/LogHelper.h"

using namespace Business::Model::Computing;

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			class SubFund
			{
			public:
				SubFund()
				{
					this->_decimals = 0;
					this->_creditAmount = 0.0;
				}

				~SubFund(){}

			private:
				QUuid  _accountId;
				QUuid  _currencyId;
				QString _currencyCode;
				QString _currencyName;
				int		_decimals;
				QDecNumber  _balance;
				QDecNumber  _necessary;
				QDecNumber  _uiTradePLFloat;
				QDecNumber  _pledgeAmount;
				QDecNumber  _frozenFund;
				QDecNumber  _totalPaidAmount;
				QDecNumber  _totalUnPaidAmount;
				QDecNumber  _partialPaymentPhysicalNecessary;
				QDecNumber  _totalNecessary;
				QDecNumber  _estimateCloseCommission;
				QDecNumber  _estimateCloseLevy;

				
				QDecNumber  _interestPLNotValued;
				QDecNumber  _storagePLNotValued;
				QDecNumber  _tradePLNotValued;
				QDecNumber  _interestPLFloat;
				QDecNumber  _storagePLFloat;
				QDecNumber  _tradePLFloat;
				QDecNumber  _unclearAmount;
				QDecNumber _creditAmount;
				
			public:
				void update(const QXmlStreamReader& xmlStreamReader)
				{	
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();  
					this->_currencyId = QUuid(attributes.value("CurrencyID").toString());
					this->_balance = attributes.value("Balance").toDouble();
					this->_totalNecessary = attributes.value("Necessary").toDouble();
					this->_pledgeAmount= attributes.value("ValueAsMargin").toDouble();
					this->_frozenFund= attributes.value("FrozenFund").toDouble();
					this->_totalPaidAmount= attributes.value("TotalPaidAmount").toDouble();
					this->_totalUnPaidAmount= attributes.value("TotalUnPaidAmount").toDouble();
					this->_partialPaymentPhysicalNecessary= attributes.value("PartialPaymentPhysicalNecessary").toDouble();
					this->_interestPLNotValued= attributes.value("InterestPLNotValued").toDouble();
					this->_storagePLNotValued= attributes.value("StoragePLNotValued").toDouble();
					this->_tradePLNotValued= attributes.value("TradePLNotValued").toDouble();
					this->_interestPLFloat= attributes.value("InterestPLFloat").toDouble();
					this->_storagePLFloat= attributes.value("StoragePLFloat").toDouble();
					this->_tradePLFloat= attributes.value("TradePLFloat").toDouble();
					this->_unclearAmount= attributes.value("UnclearAmount").toDouble();
					this->_estimateCloseCommission -= (attributes.value("EstimateCloseCommission").toDouble());
					this->_estimateCloseLevy -= (attributes.value("EstimateCloseLevy").toDouble());
					_necessary = _totalNecessary - _partialPaymentPhysicalNecessary;
				}

				void update(const QDomNode& xmlAccountCurrency)
				{					
					QDomNamedNodeMap map = xmlAccountCurrency.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "CurrencyID")
							this->_currencyId = QUuid(value);
						else if(name == "Balance")
							this->_balance = value.toDouble();
						else if(name == "Necessary")
							this->_totalNecessary = value.toDouble();
						else if(name == "ValueAsMargin")
							this->_pledgeAmount= value.toDouble();
						else if(name == "FrozenFund")
							this->_frozenFund= value.toDouble();
						else if(name == "TotalPaidAmount")
							this->_totalPaidAmount= value.toDouble();
						else if(name == "TotalUnPaidAmount")
							this->_totalUnPaidAmount= value.toDouble();
						else if(name == "PartialPaymentPhysicalNecessary")
							this->_partialPaymentPhysicalNecessary= value.toDouble();
					    else if(name == "InterestPLNotValued")
							this->_interestPLNotValued= value.toDouble();
						else if(name == "StoragePLNotValued")
							this->_storagePLNotValued= value.toDouble();
						else if(name == "TradePLNotValued")
							this->_tradePLNotValued= value.toDouble();
						else if(name == "InterestPLFloat")
							this->_interestPLFloat= value.toDouble();
						else if(name == "StoragePLFloat")
							this->_storagePLFloat= value.toDouble();
						else if(name == "TradePLFloat")
							this->_tradePLFloat= value.toDouble();
						else if(name == "UnclearAmount")
							this->_unclearAmount= value.toDouble();
						else if(name == "EstimateCloseCommission")
							this->_estimateCloseCommission= value.toDouble();
						else if(name == "EstimateCloseLevy")
							this->_estimateCloseLevy= value.toDouble();
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
				
				QDecNumber getTotalUnrealisedSwap()
				{
					return Common::roundToDecimal(_interestPLNotValued + _storagePLNotValued, _decimals);
				}

				QDecNumber getUnrealisedPL()
				{
					return _tradePLNotValued;
				}

				void setAccountId(const QUuid& value)
				{
					_accountId = value;
				}

				void setCurrencyCode(QString value, QString name)
				{
					_currencyCode = value;
					_currencyName = name;
				}

				QDecNumber getEquity()
				{
					return Common::roundToDecimal(_balance.toDouble() + _interestPLFloat.toDouble() + _storagePLFloat.toDouble() + _tradePLFloat.toDouble()
							+ _tradePLNotValued.toDouble() + _interestPLNotValued.toDouble() + _storagePLNotValued.toDouble()
							+ _pledgeAmount.toDouble() + _totalPaidAmount.toDouble() + _estimateCloseCommission.toDouble() + _estimateCloseLevy.toDouble(), 
							_decimals);
				}

				QDecNumber getNetAsset()
				{
					return Common::roundToDecimal(_creditAmount + getEquity(), _decimals);
				}

				QDecNumber getUsable()
				{
					if(_totalNecessary.isZero())
						return 0.0;
					else
						return Common::roundToDecimal((Common::TraderBaseInfo::getInstance()->getEnableNetAsset() ? getNetAsset() : getEquity()) - _totalNecessary, _decimals);
				}




				
				int getDecimals()
				{
					return _decimals;
				}

				void addBalance(QDecNumber value)
				{
					_balance += value;
				}

				bool canPlacePayment(QDecNumber targetAmount, bool allowPayWhenBalanceDeficit)
                {
                    QDecNumber equity = getEquity();
                    QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "SubFund::canPlacePayment "
                           << " targetAmount:"<< targetAmount.toDouble()
                           << " equity:"<< equity.toDouble()
                           << " totalNecessary:"<< _totalNecessary.toDouble()
                           << " unclearAmount:"<< _unclearAmount.toDouble()
                           << " partialPaymentPhysicalNecessary:"<< _partialPaymentPhysicalNecessary.toDouble()
                           << " balance:"<< _balance.toDouble()
                           << " allowPayWhenBalanceDeficit:" << allowPayWhenBalanceDeficit;
					if (((equity - _totalNecessary - _unclearAmount) - targetAmount) < 0.0 
						|| (!allowPayWhenBalanceDeficit &&
                        ((_balance - (_totalNecessary - _partialPaymentPhysicalNecessary) - _unclearAmount) - targetAmount) < 0.0))
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
					return Common::roundToString(_necessary.toDouble(), _decimals, true);
				}

				QString getPartialPaymentPhysicalNecessaryFormat()
				{
					return Common::roundToString(_partialPaymentPhysicalNecessary.toDouble(), _decimals, true);
				}

				QString getTotalUnrealisedSwapFormat()
				{
					return Common::roundToString(getTotalUnrealisedSwap().toDouble(), _decimals, true);
				}

				QString getUITradePLFloatFormat()
				{
					return Common::roundToString(getUITradePLFloat().toDouble(), _decimals, true);
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
					return Common::roundToString(getEquity().toDouble(), _decimals, true);
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
					if(_totalNecessary.isZero())
						return "0.0";
					else
						return Common::roundToString(getUsable().toDouble(), _decimals, true);
				}

				QString getPledgeAmountFormat()
				{
					return Common::roundToString(_pledgeAmount.toDouble(), _decimals, true);
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


				QString getEstimateCloseCommissionFormat()
				{
					return Common::roundToString(_estimateCloseCommission.toDouble(), _decimals, true);
				}

				QString getEstimateCloseLevyFormat()
				{
					return Common::roundToString(_estimateCloseLevy.toDouble(), _decimals, true);
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

				int getDecimals() const
				{
					return _decimals;
				}
								
				QDecNumber getBalance() const
				{
					return _balance;
				}

				QDecNumber getNecessary() const
				{
					return Common::TraderBaseInfo::getInstance()->getOnlyExistsBOInstrument() ? "0.0" : _necessary;
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
					return Common::TraderBaseInfo::getInstance()->getOnlyExistsBOInstrument() ? "0.0" : _tradePLFloat;
				}

				QDecNumber getUnclearAmount() const
				{
					return _unclearAmount;
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
					return Common::TraderBaseInfo::getInstance()->getOnlyExistsBOInstrument() ? "0.0" : _partialPaymentPhysicalNecessary;
				}

				QDecNumber getUITradePLFloat() const
				{
					return Common::TraderBaseInfo::getInstance()->getOnlyExistsBOInstrument() ? "0.0" : Common::roundToDecimal(_tradePLFloat + _interestPLFloat + _storagePLFloat, _decimals);
				}

				QDecNumber getEstimateCloseCommission() const
				{
					return Common::TraderBaseInfo::getInstance()->getOnlyExistsBOInstrument() ? "0.0" : _estimateCloseCommission;
				}

				QDecNumber getEstimateCloseLevy() const
				{
					return Common::TraderBaseInfo::getInstance()->getOnlyExistsBOInstrument() ? "0.0" : _estimateCloseLevy;
				}


				QDecNumber getCreditAmount() const
				{
					return _creditAmount;
				}

				void setDecimals(int value)
				{
					_decimals = value;
				}

				void setCreditAmount(QDecNumber value)
				{
					_creditAmount = value;
				}

			};
		}
	}
}
#endif
