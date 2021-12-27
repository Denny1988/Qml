#ifndef BUSINESS_MODEL_TRADING_MARKETINGGROUPFUND_H
#define BUSINESS_MODEL_TRADING_MARKETINGGROUPFUND_H
#include <quuid.h>
#include "QDecNumber.h"
#include "SubFund.h"
#include "Fund.h"
#include "Common/Util/Convert_Common.h"
#include "Business/Model/Setting/Currency.h"
#include <QSharedPointer>
#include "Common/TraderBaseInfo.h"

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			class MarketingGroupFund
			{
			public:
				MarketingGroupFund()
				{
                    this->_currencyName = "";
					this->_decimals = 2;
					this->clear();
				}

				~MarketingGroupFund() {}

			private:
				QUuid _salesId;
				QString _salesCode;
				QString _salesName;
				QString _managerCode;
				QList<QUuid>  _accountIds;
				QMap<QUuid, QString> _currencies;
				QUuid _currencyId;
                QString _currencyName;
				int		_decimals;

				QDecNumber  _balance;
				QDecNumber  _totalPaidAmount;
				QDecNumber  _totalUnPaidAmount;
				QDecNumber  _pledgeAmount;
				QDecNumber  _necessary;
				QDecNumber  _partialPaymentPhysicalNecessary;
				QDecNumber  _frozenFund;
				QDecNumber  _unclearAmount;
				QDecNumber  _creditAmount;
				QDecNumber  _estimateCloseCommission;
				QDecNumber  _estimateCloseLevy;
				QDecNumber _totalUnrealisedSwap;
				QDecNumber _unrealisedPL;
				QDecNumber _UITradePLFloat;
				QDecNumber _equity;
				QDecNumber _netAsset;
				QDecNumber _usable;

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_salesId = QUuid(attributes.value("SalesId").toString());
					this->_salesCode = attributes.value("SalesCode").toString();
					this->_salesName = attributes.value("SalesName").toString();
					this->_managerCode = attributes.value("ManagerCode").toString();

					if (xmlStreamReader.isStartElement()) {
						while (!xmlStreamReader.atEnd()) {
							if (xmlStreamReader.isEndElement()) {
								xmlStreamReader.readNext();//Account end
                                qDebug() << "MarketingGroupFund update: "<< xmlStreamReader.name();
                                if (xmlStreamReader.name() != "Account")
                                    break;
							}
							if (xmlStreamReader.name() == "Account") {
								QUuid accountId = QUuid(xmlStreamReader.attributes().value("Id").toString());
								this->_accountIds.append(accountId);
                                xmlStreamReader.readNext();
                                xmlStreamReader.readNext();
							}
							else
								xmlStreamReader.readNext();
						}
					}
				}

				void clear()
				{
					this->_balance = 0.0;
					this->_totalPaidAmount = 0.0;
					this->_totalUnPaidAmount = 0.0;
					this->_pledgeAmount = 0.0;
					this->_necessary = 0.0;
					this->_partialPaymentPhysicalNecessary = 0.0;
					this->_frozenFund = 0.0;
					this->_unclearAmount = 0.0;
					this->_creditAmount = 0.0;
					this->_estimateCloseCommission = 0.0;
					this->_estimateCloseLevy = 0.0;
					this->_unrealisedPL = 0.0;
					this->_totalUnrealisedSwap = 0.0;
					this->_UITradePLFloat = 0.0;
					this->_equity = 0.0;
					this->_netAsset = 0.0;
					this->_usable = 0.0;
					this->_currencies.clear();
				}

                void clearAccountList()
                {
                    this->_accountIds.clear();
                }

                void addAccountId(QUuid accountId)
                {
                    this->_accountIds.append(accountId);
                }

				void addSubFund(QSharedPointer<SubFund> fund)
				{
                    if(this->_accountIds.contains(fund->getAccountId()))
                    {
                        this->_balance += fund->getBalance();
                        this->_totalPaidAmount += fund->getTotalPaidAmount();
                        this->_totalUnPaidAmount += fund->getTotalUnPaidAmount();
                        this->_pledgeAmount += fund->getPledgeAmount();
                        this->_necessary += fund->getNecessary();
                        this->_partialPaymentPhysicalNecessary += fund->getPartialPaymentPhysicalNecessary();
                        this->_frozenFund += fund->getFrozenFund();
                        this->_unclearAmount += fund->getUnclearAmount();
                        this->_creditAmount += fund->getCreditAmount();
                        this->_estimateCloseCommission += fund->getEstimateCloseCommission();
                        this->_estimateCloseLevy += fund->getEstimateCloseLevy();
                        this->_totalUnrealisedSwap += fund->getTotalUnrealisedSwap();
                        this->_UITradePLFloat += fund->getUITradePLFloat();
                        this->_equity += fund->getEquity();
                        this->_netAsset += fund->getNetAsset();
                        this->_usable += fund->getUsable();
                        this->_unrealisedPL += fund->getUnrealisedPL();
                    }
				}

				void addFund(QSharedPointer<Fund> fund)
				{
                    if(this->_accountIds.contains(fund->getAccountId()))
                    {
                        this->_balance += fund->getBalance();
                        this->_totalPaidAmount += fund->getTotalPaidAmount();
                        this->_totalUnPaidAmount += fund->getTotalUnPaidAmount();
                        this->_pledgeAmount += fund->getPledgeAmount();
                        this->_necessary += fund->getNecessary();
                        this->_partialPaymentPhysicalNecessary += fund->getPartialPaymentPhysicalNecessary();
                        this->_frozenFund += fund->getFrozenFund();
                        this->_unclearAmount += fund->getUnclearAmount();
                        this->_creditAmount += fund->getCreditAmount();
                        this->_estimateCloseCommission += fund->getEstimateCloseCommission();
                        this->_estimateCloseLevy += fund->getEstimateCloseLevy();
                        this->_totalUnrealisedSwap += fund->getTotalUnrealisedSwap();
                        this->_UITradePLFloat += fund->getUITradePLFloat();
                        this->_equity += fund->getEquity();
                        this->_netAsset += fund->getNetAsset();
                        this->_usable += fund->getUsable();
                        this->_unrealisedPL += fund->getUnrealisedPL();
                    }
				}

				void addCurrency(QUuid currencyId, QString code)
				{
					if (!_currencies.contains(currencyId))
						_currencies.insert(currencyId, code);
				}

				QMap<QUuid, QString>  getCurrencies()
				{
					return _currencies;
				}

				int currenciesCount()
				{
					return _currencies.count();
				}

				QUuid getSalesId() const
				{
					return _salesId;
				}

				QString getSalesCode() const
				{
					return _salesCode;
				}

				QString getSalesName() const
				{
					return _salesName;
				}

				QString getManagerCode() const
				{
					return _managerCode;
				}

				QList<QUuid> getAccountIds() const
				{
					return _accountIds;
				}

				QString getMarketingGroupInfo() const
				{
					if (Common::TraderBaseInfo::getInstance()->getUserId() == this->_salesId)
					{
						return QString("%1 : %2").arg(this->_salesCode, QString::number(this->_accountIds.count()));
					}
					else
					{
						return QString("%1 > %2 (%3) : %4").arg(this->_managerCode, this->_salesCode, this->_salesName, QString::number(this->_accountIds.count()));
					}
				}

                void setCurrency(QUuid currencyId, QString name, int decimals)
				{
                    _currencyName = name;
					_decimals = decimals;
					_currencyId = currencyId;
				}

                void setSaleInfo(QUuid salesId, QString salesCode, QString salesName, QString managerCode)
                {
                    _salesId = salesId;
                    _salesCode = salesCode;
                    _salesName = salesName;
                    _managerCode = managerCode;
                }

				void setBalance(QDecNumber value)
				{
					_balance = value;
				}

                void setCurrencyId(QUuid value)
                {
                    _currencyId = value;
                }

				void setTotalPaidAmount(QDecNumber value)
				{
					_totalPaidAmount = value;
				}

				void setTotalUnPaidAmount(QDecNumber value)
				{
					_totalUnPaidAmount = value;
				}

				void setPledgeAmount(QDecNumber value)
				{
					_pledgeAmount = value;
				}

				void setNecessary(QDecNumber value)
				{
					_necessary = value;
				}

				void setPartialPaymentPhysicalNecessary(QDecNumber value)
				{
					_partialPaymentPhysicalNecessary = value;
				}

				void setFrozenFund(QDecNumber value)
				{
					_frozenFund = value;
				}

				void setUnclearAmount(QDecNumber value)
				{
					_unclearAmount = value;
				}

				void setCreditAmount(QDecNumber value)
				{
					_creditAmount = value;
				}

				void setEstimateCloseCommission(QDecNumber value)
				{
					_estimateCloseCommission = value;
				}

				void setEstimateCloseLevy(QDecNumber value)
				{
					_estimateCloseLevy = value;
				}

				void setUnrealisedPL(QDecNumber value)
				{
					_unrealisedPL = value;
				}

				void setTotalUnrealisedSwap(QDecNumber value)
				{
					_totalUnrealisedSwap = value;
				}

				void setUITradePLFloat(QDecNumber value)
				{
					_UITradePLFloat = value;
				}

				void setEquity(QDecNumber value)
				{
					_equity = value;
				}

				void setNetAsset(QDecNumber value)
				{
					_netAsset = value;
				}


				void setUsable(QDecNumber value)
				{
					_usable = value;
				}


				QUuid getCurrencyId()
				{
					return _currencyId;
				}

                QString getCurrencyName()
				{
                    return _currencyName;
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
					return Common::roundToString(_totalUnrealisedSwap.toDouble(), _decimals, true);
				}

				QString getUITradePLFloatFormat()
				{
					return Common::roundToString(_UITradePLFloat.toDouble(), _decimals, true);
				}

				QString getUnrealisedPLFormat()
				{
					return Common::roundToString(_unrealisedPL.toDouble(), _decimals, true);
				}

				QString getUnclearAmountFormat()
				{
					return Common::roundToString(_unclearAmount.toDouble(), _decimals, true);
				}

				QString getEquityFormat()
				{
					return Common::roundToString(_equity.toDouble(), _decimals, true);
				}

				QString getNetAssetFormat()
				{
					return Common::roundToString(_netAsset.toDouble(), _decimals, true);
				}


				QString getNetAsset()
				{
					return Common::roundToString(_netAsset.toDouble(), _decimals, true);
				}

				QString getUsableFormat()
				{
					return Common::roundToString(_usable.toDouble(), _decimals, true);
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

				QString getEstimateCloseCommissionFormat()
				{
					return Common::roundToString(_estimateCloseCommission.toDouble(), _decimals, true);
				}

				QString getEstimateCloseLevyFormat()
				{
					return Common::roundToString(_estimateCloseLevy.toDouble(), _decimals, true);
				}



			};
		}
	}
}
#endif
