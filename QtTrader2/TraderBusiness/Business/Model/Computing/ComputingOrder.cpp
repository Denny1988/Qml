#include "ComputingOrder.h"
#include <Math.h>
#include "Business/BusinessManager.h"
#include "OverridedQuotation.h"
#include "Common/LogHelper.h"
#include "Business/BusinessHelper.h"
#include "float.h"

namespace Business
{
	namespace Model
	{
		namespace Computing
		{
			QSharedPointer<DemandComputingResult> ComputingOrder::doCompute()
			{
				//QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  "ComputingOrder::doCompute";
                //qDebug() <<  "ComputingOrder::doCompute start";
				QSharedPointer<OrderComputingResult> orderComputingResult(new OrderComputingResult());
				orderComputingResult->orderId = this->_orderId;
				QSharedPointer<Model::Trading::TradingOrder> tradingOrder = Business::BusinessManager::getInstance()->getTradingManager()->getOrder(this->_orderId);				
                if(tradingOrder.isNull()) return orderComputingResult;
                QSharedPointer<Model::Setting::BusinessInstrument> settingInstrument = Business::BusinessManager::getInstance()->getSettingManager()->getInstrument(this->_accountId, this->_instrumentId);
				QSharedPointer<Model::Setting::BusinessAccount> settingAccount = Business::BusinessManager::getInstance()->getSettingManager()->getAccount(this->_accountId);

				QSharedPointer<OverridedQuotationComputingResult> overridedQuotationComputingResult = Business::BusinessManager::getInstance()->getComputingManager()->getOverridedQuotationComputingResult(this->_accountId, this->_instrumentId);

				if ((tradingOrder->getIsBuy() && !tradingOrder->getIsPayoff()) || !tradingOrder->getIsBuy())
					orderComputingResult->totalPaidAmount = (tradingOrder->getPaidPledgeBalance()).abs();

				if (tradingOrder->getIsBuy() && tradingOrder->getPhysicalOriginValueBalance() != DBL_MIN && !tradingOrder->getPaidPledgeBalance().isZero())
				{
					auto physicalOriginValueBalance = (tradingOrder->getPhysicalOriginValueBalance()).abs();
					auto paidPledgeBalance = (tradingOrder->getPaidPledgeBalance()).abs();
					orderComputingResult->totalUnpaidAmount +=  (physicalOriginValueBalance - paidPledgeBalance).abs();
				}

				if(tradingOrder->getTransaction()->getInstrumentCategory() == Common::Enum::InstrumentCategory::Physical &&
					(tradingOrder->getPhysicalTradeSide() == Common::Enum::PhysicalTradeSide::Buy
					|| tradingOrder->getPhysicalTradeSide() == Common::Enum::PhysicalTradeSide::Deposit))
				{
					this->calculateMarketValue(orderComputingResult, overridedQuotationComputingResult, tradingOrder, settingInstrument, settingAccount);
				}

				this->calculatePLFloat(orderComputingResult, overridedQuotationComputingResult, tradingOrder, settingInstrument, settingAccount);
				this->calculateMargin(orderComputingResult, overridedQuotationComputingResult, tradingOrder, settingInstrument, settingAccount);
				this->calculateCommission(orderComputingResult, tradingOrder, settingInstrument, settingAccount, FeeCalculateType::Commission);
				this->calculateCommission(orderComputingResult, tradingOrder, settingInstrument, settingAccount, FeeCalculateType::EstimateCloseCommission);
				this->calculateCommission(orderComputingResult, tradingOrder, settingInstrument, settingAccount, FeeCalculateType::EstimateCloseLevy);
				return orderComputingResult;
			}

			void ComputingOrder::calculate()
			{
				QSharedPointer<Model::Trading::TradingOrder> tradingOrder = Business::BusinessManager::getInstance()->getTradingManager()->getOrder(this->_orderId);				
                if(tradingOrder.isNull()) return;
                QSharedPointer<Model::Setting::BusinessInstrument> settingInstrument = Business::BusinessManager::getInstance()->getSettingManager()->getInstrument(this->_accountId, this->_instrumentId);
				QSharedPointer<Model::Setting::BusinessAccount> settingAccount = Business::BusinessManager::getInstance()->getSettingManager()->getAccount(this->_accountId);
				QSharedPointer<OrderComputingResult> orderComputingResult(NULL);

				QSharedPointer<OverridedQuotationComputingResult> overridedQuotationComputingResult = Business::BusinessManager::getInstance()->getComputingManager()->getOverridedQuotationComputingResult(this->_accountId, this->_instrumentId);
				this->calculatePLFloat(orderComputingResult, overridedQuotationComputingResult, tradingOrder, settingInstrument, settingAccount, true);
				this->calculateCommission(orderComputingResult, tradingOrder, settingInstrument, settingAccount, FeeCalculateType::EstimateCloseCommission, true);
			}



			void ComputingOrder::calculateCommission(QSharedPointer<OrderComputingResult>& orderComputingResult,
				QSharedPointer<Model::Trading::TradingOrder>& tradingOrder,
				QSharedPointer<Model::Setting::BusinessInstrument>& settingInstrument,
				QSharedPointer<Model::Setting::BusinessAccount>& settingAccount,
				FeeCalculateType::FeeCalculateType feeCalculateType, bool isInit)
			{
				QDecNumber commission = 0.0;
				bool isCommission = (feeCalculateType == FeeCalculateType::Commission || feeCalculateType == FeeCalculateType::EstimateCloseCommission);

				auto calculateSetting  = settingInstrument->getCalculateSetting();
				FeeFormula::FeeFormula commissionFormula = isCommission ? static_cast<FeeFormula::FeeFormula>(calculateSetting->getCommissionFormula()) : static_cast<FeeFormula::FeeFormula>(calculateSetting->getLevyFormula());
				short tradePLFormula = calculateSetting->getTradePLFormula();

				QDecNumber rateCommission = isCommission ? calculateSetting->getRateCommission() : calculateSetting->getRateLevy();
                //qDebug() << "rateCommission:" << rateCommission.toString();
				QDecNumber contractSize = tradingOrder->getTransaction()->getContractSize();
				QDecNumber commissionCloseD = isCommission ? calculateSetting->getCommissionCloseD() : calculateSetting->getLevyClose();
				QDecNumber commissionCloseO = isCommission ? calculateSetting->getCommissionCloseO() : calculateSetting->getLevyClose();
				QDecNumber commissionClose = 0;

				QDecNumber pairRelationFactor = 1;
                if (tradingOrder->getTransaction()->getType() == TransactionType::Pair && calculateSetting->getPairRelationFactor() != -1.0)
				{
					pairRelationFactor = calculateSetting->getPairRelationFactor();
				}

				auto tradeDay = Business::BusinessManager::getInstance()->getSettingManager()->getTradeDay();
				if(!tradeDay.isNull())
				{
					if (tradingOrder->getTransaction()->getExecuteTime() < tradeDay->getBeginTime())
					{
						commissionClose = commissionCloseO * pairRelationFactor;
					}
					else
					{
						commissionClose = commissionCloseD * pairRelationFactor;
					}
				}

				QDecNumber lotBalance = tradingOrder->getLotBalance();
				QDecNumber commission2 = commissionClose * rateCommission;
				QDecNumber tradePLFloat = tradingOrder->getTradePLFloat();
				QDecNumber executePrice = tradingOrder->getExecutePrice().toDecimal();
				QDecNumber buy = 0.0;
				QDecNumber sell = 0.0;
				QDecNumber close = 0.0;	
//                qDebug() << "commissionCloseO:" << commissionCloseO.toString();
//                qDebug() << "pairRelationFactor:" << commissionClose.toString();
//                qDebug() << "commissionCloseD:" << commissionCloseD.toString();
//                qDebug() << "commissionClose:" << pairRelationFactor.toString();
//                qDebug() << "rateCommission:" << rateCommission.toString();
//                qDebug() << "commission2:" << commission2.toString();
//                qDebug() << "lotBalance:" << lotBalance.toString();
				switch (commissionFormula)
				{
				case FeeFormula::FixedAmount:
					commission = commission2 * lotBalance;					
					break;
				case FeeFormula::CS:
					commission = commission2 * lotBalance * contractSize;
					break;
				case FeeFormula::CSDividePrice:
					commission = commission2 * lotBalance * contractSize / executePrice;
					break;
				case FeeFormula::CSMultiplyPrice:
					commission = commission2 * lotBalance * contractSize * executePrice;
					break;
				case FeeFormula::Pips:											
					if (tradePLFormula == 2)
					{
						buy = (tradingOrder->getExecutePrice() + commissionClose.toInt32()).toDecimal();
						sell = executePrice;
						close = executePrice;
					}
					else
					{
						buy = executePrice;
						sell = (tradingOrder->getExecutePrice() + commissionClose.toInt32()).toDecimal();
						close = executePrice;
					}
					commission = this->calculateTradePL(tradePLFormula, lotBalance, contractSize, buy, sell, close);
					break;
				case FeeFormula::PricePips:
					commission = 0;
					break;
				case FeeFormula::RealizedLoss:						
					commission = (tradePLFloat < 0.0 ? commission2 * tradePLFloat : 0);
					break;
				case FeeFormula::RealizedProfit:
					commission = (tradePLFloat > 0.0 ? commission2 * tradePLFloat: 0);
					break;
				case FeeFormula::RealizedPL:
					commission = commission2 * tradePLFloat.abs();
					break;
				case FeeFormula::SharedPL:
					commission = commission2 * tradePLFloat;
					break;
				}
				if (isCommission)
				{
					if (commissionFormula < FeeFormula::PricePips)//5--->Price Pips
					{
						if (commission >= 0.0)
						{
							QDecNumber minCommission = calculateSetting->getMinCommissionClose();
                            commission = commission.getMax(minCommission);
						}
					}
					if (feeCalculateType == FeeCalculateType::Commission)
					{
						tradingOrder->setOpenOrderCommission(commission);
					}
					else if((int)commissionFormula <  (int)FeeFormula::RealizedLoss || (int)commissionFormula > (int)FeeFormula::SharedPL)
					{
						if(!isInit) 
							orderComputingResult->estimateCloseCommission = commission;
						tradingOrder->setOpenOrderCommission(commission);
					}	
					else
						tradingOrder->setOpenOrderCommission(0.0);
				}
				else
				{
					if ((int)commissionFormula < (int)FeeFormula::RealizedLoss || (int)commissionFormula > (int)FeeFormula::SharedPL)
					{
						orderComputingResult->estimateCloseLevy = commission;
                        //qDebug() << "ComputingOrder::calculateCommission estimateCloseLevy:" << commission.toString();
                        //qDebug() << "ComputingOrder::calculateCommission estimateCloseLevy_d:" << commission.toDouble();
					}
				}
			}

			void ComputingOrder::calculateMarketValue(QSharedPointer<OrderComputingResult>& orderComputingResult,
				QSharedPointer<OverridedQuotationComputingResult>& overridedQuotationComputingResult,
				QSharedPointer<Model::Trading::TradingOrder>& tradingOrder,
				QSharedPointer<Model::Setting::BusinessInstrument>& settingInstrument,
				QSharedPointer<Model::Setting::BusinessAccount>& settingAccount)
			{	
				//QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "ComputingOrder::calculateMarketValue";
				Common::Price ask;
				Common::Price bid;
				if(!overridedQuotationComputingResult.isNull())
				{
					ask = overridedQuotationComputingResult->ask;
					bid = overridedQuotationComputingResult->bid;
				}
				Common::Price buy = settingInstrument->getIsNormal() ? bid : ask;
				Common::Price sell = settingInstrument->getIsNormal() ? ask : bid;

				QDecNumber pledgeAmount = 0;
				QDecNumber value = 0;
				QDecNumber unpaidAmount = 0;
				if (Q_NULLPTR == &buy && Q_NULLPTR == &sell)
				{
					orderComputingResult->marketValue = 0;
				}
				else
				{
					if (tradingOrder->getIsBuy())
					{
						tradingOrder->setLivePrice(buy);
					}
					else
					{
						tradingOrder->setLivePrice(sell);
					}
                    QDecNumber lot = tradingOrder->getLotBalance();
                    int lotInteger = Common::stringToInt(lot.toString());
                    QDecNumber lotIntegerDecimal = QDecNumber(lotInteger);
                    QDecNumber lotRemainder = lot - lotIntegerDecimal;
					Common::Price livePrice = tradingOrder->getLivePrice();
					QDecNumber price = Common::roundToDecimal(livePrice.toDouble(), livePrice._decimals);
					value = BusinessHelper::getInstance()->calculateValue(settingInstrument->getCalculateSetting()->getTradePLFormula(), tradingOrder->getContractSize(), lotIntegerDecimal, lotRemainder, price, settingInstrument->getCalculateSetting()->getDiscountOfOdd());
				}

				if (tradingOrder->getIsBuy() && !tradingOrder->getIsPayoff() && tradingOrder->getPhysicalOriginValueBalance() != DBL_MIN && tradingOrder->getPaidPledgeBalance() != DBL_MIN)
					unpaidAmount = tradingOrder->getPhysicalOriginValueBalance() - (tradingOrder->getPaidPledgeBalance()).abs();

				int decimals = 0;
				if (settingAccount->getIsMultiCurrency())
				{
					QUuid sourceCurrencyId = settingInstrument->getCurrencyId();
					QSharedPointer<Model::Setting::Currency> currency = Business::BusinessManager::getInstance()->getSettingManager()->getCurrency(sourceCurrencyId);
					if (!currency.isNull())
						decimals = currency->getDecimals();
					orderComputingResult->marketValueInStock = Common::roundToDecimal(value, decimals);
					orderComputingResult->marketValueInStockInFormat= Common::roundToString(orderComputingResult->marketValueInStock.toDouble(), decimals, true);
					unpaidAmount = Common::roundToDecimal(unpaidAmount, decimals);
				}
				else
				{
					QUuid sourceCurrencyId = settingInstrument->getCurrencyId();
					QUuid targetCurrencyId = settingAccount->getCurrencyId();
					QSharedPointer<Model::Setting::CurrencyRate> currencyRate = Business::BusinessManager::getInstance()->getSettingManager()->getCurrencyRate(sourceCurrencyId, targetCurrencyId);
					if(!currencyRate.isNull())
					{				
						decimals = currencyRate->getTargetCurrency()->getDecimals();
						orderComputingResult->marketValueInStock = currencyRate->exchange(value);
						orderComputingResult->marketValueInStockInFormat= Common::roundToString(orderComputingResult->marketValueInStock.toDouble(), decimals, true);
						unpaidAmount = Common::roundToDecimal(unpaidAmount, decimals);
					}
					else
					{
						QSharedPointer<Model::Setting::Currency> currency = Business::BusinessManager::getInstance()->getSettingManager()->getCurrency(sourceCurrencyId);
						if(!currency.isNull())
						{
							decimals = currency->getDecimals();
							orderComputingResult->marketValueInStock = Common::roundToDecimal(value, decimals);
							orderComputingResult->marketValueInStockInFormat= Common::roundToString(orderComputingResult->marketValueInStock.toDouble(), decimals, true);
							unpaidAmount = Common::roundToDecimal(unpaidAmount, decimals);
						}
					}
				}
				if (tradingOrder->getIsPayoff())
				{
					pledgeAmount = orderComputingResult->marketValueInStock * settingInstrument->getCalculateSetting()->getValueDiscountAsMargin();
				}
				pledgeAmount = Common::roundToDecimal(pledgeAmount, decimals);
				orderComputingResult->unpaidAmount = unpaidAmount;
				orderComputingResult->unpaidAmountInFormat = Common::roundToString(unpaidAmount.toDouble(), decimals, true);
				orderComputingResult->marketValue = value;
				if (Q_NULLPTR!=&buy && Q_NULLPTR!=&sell)
				{
					orderComputingResult->pledgeAmount = pledgeAmount;
					orderComputingResult->pledgeAmountInFormat = Common::roundToString(pledgeAmount.toDouble(), decimals, true);
				}
			}

			void ComputingOrder::calculatePLFloat(QSharedPointer<OrderComputingResult>& orderComputingResult,
				QSharedPointer<OverridedQuotationComputingResult>& overridedQuotationComputingResult,
				QSharedPointer<Model::Trading::TradingOrder>& tradingOrder,
				QSharedPointer<Model::Setting::BusinessInstrument>& settingInstrument,
				QSharedPointer<Model::Setting::BusinessAccount>& settingAccount, bool isInit)
			{
				//QLOG_INFO(Common::Constants::LogKeyConstants::Business ) << "ComputingOrder::calculatePLFloat";
				if (tradingOrder->getIsOpen() && !tradingOrder->getLotBalance().isNegative() && !tradingOrder->getLotBalance().isZero())
				{	
					Common::Price ask;
					Common::Price bid;
					if(!overridedQuotationComputingResult.isNull())
					{
						if(!overridedQuotationComputingResult->ask.isNullPrice()) ask = overridedQuotationComputingResult->ask;
						if(!overridedQuotationComputingResult->bid.isNullPrice()) bid = overridedQuotationComputingResult->bid;
					}
					Common::Price buy = settingInstrument->getIsNormal() ? bid : ask;
					Common::Price sell = settingInstrument->getIsNormal() ? ask : bid;

					if (Q_NULLPTR == &buy && Q_NULLPTR == &sell)
						return;

					QDecNumber executePrice = tradingOrder->getExecutePrice().toDecimal();
					if (tradingOrder->getExecutePriceOrigin().length() > 0)
					{
                        executePrice = Common::roundToDecimal(tradingOrder->getExecutePriceOrigin().toDouble(), tradingOrder->getExecutePrice()._decimals);
					}
					QDecNumber closeValue;
					QDecNumber buyValue;
					QDecNumber sellValue;
					Common::Price close;
					if (tradingOrder->getIsBuy())
					{
						close = buy;
						closeValue = buy.toDecimal();
						buyValue = executePrice;
						sellValue = closeValue;
					}
					else
					{
						close = sell;
						closeValue = sell.toDecimal();
						buyValue = closeValue;
						sellValue = executePrice;
					}
					tradingOrder->setLivePrice(close);

                    double lotbalance = tradingOrder->getLotBalance().toDouble();
					double interestPerLot = tradingOrder->getInterestPerLot().toDouble();
					double storagePerLot = tradingOrder->getStoragePerLot().toDouble();
					/*qDebug()<< "lotbalance:" << lotbalance;
					qDebug()<< "interestPerLot:" << interestPerLot;
					qDebug()<< "storagePerLot:" << storagePerLot;*/
					QDecNumber interestPLFloat =  lotbalance* interestPerLot;
					QDecNumber storagePLFloat = lotbalance * storagePerLot;

					if (Q_NULLPTR==&buy || buy.isNullPrice()) buyValue = executePrice;
					if (Q_NULLPTR==&sell || sell.isNullPrice()) sellValue = executePrice;
					if (Q_NULLPTR==&close || close.isNullPrice()) closeValue = executePrice;
					QDecNumber tradePLFloat = 0;
                    QDecNumber tradePLFloatForPending = 0;

					if (tradingOrder->getTransaction()->getInstrumentCategory() == Common::Enum::InstrumentCategory::Forex
						|| (tradingOrder->getPhysicalTradeSide() == Common::Enum::PhysicalTradeSide::ShortSell)
						|| (tradingOrder->getTransaction()->getInstrumentCategory() == Common::Enum::InstrumentCategory::Physical && !tradingOrder->getIsPayoff()))
					{
						tradePLFloat = this->calculateTradePL(settingInstrument->getCalculateSetting()->getTradePLFormula(), tradingOrder->getLotBalance(), tradingOrder->getContractSize(), buyValue, sellValue, closeValue);
                        tradePLFloatForPending = this->calculateTradePL(settingInstrument->getCalculateSetting()->getTradePLFormula(), qMax(tradingOrder->getLotBalanceForLimitOrderType(), tradingOrder->getLotBalanceForStopOrderType()),
                                                                        tradingOrder->getContractSize(), buyValue, sellValue, closeValue);
					}
                    //if(settingInstrument->getCode() == "EURJPY" && tradingOrder->getCode() == "DEM2021031600070")
                    //    qDebug() << "Code:" + tradingOrder->getCode() << " tradePLFloat:" << tradePLFloat.toString()<< " tradePLFloatForPending:" << tradePLFloatForPending.toString();
					int decimals = 0;
					if (settingAccount->getIsMultiCurrency())
					{
						QSharedPointer<Model::Setting::Currency> currency = Business::BusinessManager::getInstance()->getSettingManager()->getCurrency(settingInstrument->getCurrencyId());
						if (!currency.isNull())
							decimals = currency->getDecimals();

						interestPLFloat = Common::roundToDecimal(interestPLFloat, decimals);
						storagePLFloat = Common::roundToDecimal(storagePLFloat, decimals);
						tradePLFloat = Common::roundToDecimal(tradePLFloat, decimals);
                        tradePLFloatForPending = Common::roundToDecimal(tradePLFloatForPending, decimals);
                        //if(settingInstrument->getCode() == "EURJPY" && tradingOrder->getCode() == "DEM2021031600070")
                        //    qDebug() << "222 tradePLFloat:" << tradePLFloat.toString() << " tradePLFloatForPending:"<< tradePLFloatForPending.toString() << " decimals:"<< decimals;
					}
					else
					{
						QUuid sourceCurrencyId = settingInstrument->getCurrencyId();
						QUuid targetCurrencyId = settingAccount->getCurrencyId();
						QSharedPointer<Model::Setting::CurrencyRate> currencyRate = Business::BusinessManager::getInstance()->getSettingManager()->getCurrencyRate(sourceCurrencyId, targetCurrencyId);
						if(!currencyRate.isNull())
						{
							interestPLFloat = currencyRate->exchange(interestPLFloat);
							storagePLFloat = currencyRate->exchange(storagePLFloat);
							tradePLFloat = currencyRate->exchange(tradePLFloat); 
                            tradePLFloatForPending = currencyRate->exchange(tradePLFloatForPending);
							decimals = currencyRate->getTargetCurrency()->getDecimals();
							/*qDebug() << "OOOcurrencyRate:" << currencyRate->getRateIn() <<  currencyRate->getRateOut();
							qDebug() << "OOOtradePLFloat:" << tradePLFloat.toString();*/
                           //qDebug() << "Code:" + tradingOrder->getCode() << "tradePLFloat333:" << tradePLFloat.toString();
                            //if(settingInstrument->getCode() == "EURJPY" && tradingOrder->getCode() == "DEM2021031600070")
                             //   qDebug() << "333 tradePLFloat:" << tradePLFloat.toString() << " tradePLFloatForPending:"<< tradePLFloatForPending.toString();
						}
					}

					if(!isInit)
					{
						orderComputingResult->interestPLFloat = interestPLFloat;
						orderComputingResult->storagePLFloat = storagePLFloat;
						orderComputingResult->tradePLFloat = tradePLFloat;
						orderComputingResult->interestPLFloatInFormat = Common::roundToString(interestPLFloat.toDouble(), decimals, true);
						orderComputingResult->storagePLFloatInFormat = Common::roundToString(storagePLFloat.toDouble(), decimals, true);
						orderComputingResult->tradePLFloatInFormat = Common::roundToString(tradePLFloat.toDouble(), decimals, true);
					}
					tradingOrder->setTradePLFloat(tradePLFloat);
                    tradingOrder->setTradePLFloatForPending(tradePLFloatForPending);
					tradingOrder->setInterestPLFloat(interestPLFloat);
					tradingOrder->setStoragePLFloat(storagePLFloat);
                    //if(settingInstrument->getCode() == "EURJPY" && tradingOrder->getCode() == "DEM2021031600070")
                    //    qDebug() << "444 Code:" + tradingOrder->getCode() << " tradePLFloat:" << tradePLFloat.toString()<< " tradePLFloatForPending:" << tradePLFloatForPending.toString();
					//QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "Code:" + tradingOrder->getCode() << "tradePLFloat:" << tradePLFloat.toDouble();
                    //qDebug() << "Code:" + tradingOrder->getCode() << "tradePLFloat:" << tradePLFloat.toDouble();
				}
			}

			QDecNumber ComputingOrder::calculateTradePL(int tradePLFormula, QDecNumber lotBalance, QDecNumber contractSize, QDecNumber buy, QDecNumber sell, QDecNumber close)
			{
				//QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "ComputingOrder::calculateTradePL";
				QDecNumber tradePL = 0;
				QDecNumber divisor = 1;
				switch (tradePLFormula)
				{
				case 0:

                    tradePL = lotBalance * (sell - buy) * contractSize;
					break;
				case 1:
					tradePL = lotBalance * (sell - buy) * contractSize / close;
					break;
				case 2:
					tradePL = lotBalance * (divisor / sell - divisor / buy) * contractSize;
					break;
				case 3:     // Added 2007-11-20
					QDecNumber openPrice = (close == buy) ? sell : buy;
					tradePL = lotBalance * (sell - buy) * contractSize / openPrice;
					break;
				}

//                qDebug() << "calculateTradePL:" << "lotBalance" << lotBalance.toString();
//                qDebug() << "calculateTradePL:" << "sell" << sell.toString();
//                qDebug() << "calculateTradePL:" << "buy" << buy.toString();
//                qDebug() << "calculateTradePL:" << "contractSize" << contractSize.toString();
//                qDebug() << "tradePL:" << "tradePL" << tradePL.toString();
#if defined (RUNTIME_OS_IPHONE)
                //Some values are actually 7.775, and when converted to double type, it becomes 7.7749999999999999, so complete, otherwise the correct value is 7.78 after rounding, but the actual value is 7.77
                if(tradePL > 0.0)
                    tradePL = tradePL.add(0.00000001);
                else
                    tradePL = tradePL.subtract(0.00000001);
#else
                if(tradePL > 0.0)
                    tradePL = tradePL + 0.00000001;
                else
                    tradePL = tradePL - 0.00000001;
#endif
                //qDebug() << "ddddtradePL:" << "tradePL2222222" << tradePL.toString();
                return tradePL;
			}

			void ComputingOrder::calculateMargin(QSharedPointer<OrderComputingResult>& orderComputingResult, 
				QSharedPointer<OverridedQuotationComputingResult>& overridedQuotationComputingResult,
				QSharedPointer<Model::Trading::TradingOrder>& tradingOrder,
				QSharedPointer<Model::Setting::BusinessInstrument>& settingInstrument,
				QSharedPointer<Model::Setting::BusinessAccount>& settingAccount)
			{
				//QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "ComputingOrder::calculateMargin";
				QSharedPointer<Model::Setting::CalculateSetting> calculateSetting = settingInstrument->getCalculateSetting();
				QSharedPointer<Model::Setting::CurrencyRate> currencyRate;
				int decimals = 0;
				if (settingAccount->getIsMultiCurrency())
				{
					QSharedPointer<Model::Setting::Currency> currency = Business::BusinessManager::getInstance()->getSettingManager()->getCurrency(settingInstrument->getCurrencyId());
					if(!currency.isNull())
						decimals = currency->getDecimals();
				}
				else
				{
					QUuid sourceCurrencyId = settingInstrument->getCurrencyId();
					QUuid targetCurrencyId = settingAccount->getCurrencyId();
					currencyRate = Business::BusinessManager::getInstance()->getSettingManager()->getCurrencyRate(sourceCurrencyId, targetCurrencyId);
				}

				Common::Price ask;
				Common::Price bid;
				if(!overridedQuotationComputingResult.isNull())
				{
					ask = overridedQuotationComputingResult->ask;
					bid = overridedQuotationComputingResult->bid;
				}

				Common::Price buy = settingInstrument->getIsNormal() ? bid : ask;
				Common::Price sell = settingInstrument->getIsNormal() ? ask : bid;

				Common::Price close;
				if (tradingOrder->getIsBuy())
				{
					close = buy;
				}
				else
				{
					close = sell;
				}

				if (tradingOrder->getPhase() == Common::Enum::Phase::Canceled)
					return;
				QDecNumber marginm = 0;
				if ((calculateSetting->getMarginFormula() == 6 || calculateSetting->getMarginFormula() == 7)
					&& (Q_NULLPTR==&close))
				{
					marginm = 0;
				}
				else
				{
                    QDecNumber price = (calculateSetting->getMarginFormula() == 6 || calculateSetting->getMarginFormula() == 7)
						? Common::roundToDecimal(close.toDouble(), close._decimals)
						: Common::roundToDecimal(tradingOrder->getExecutePrice().toDouble(), tradingOrder->getExecutePrice()._decimals);
					//qDebug() << "1111111111 price:" << price.toString();
					marginm = this->calculateMargin(calculateSetting->getMarginFormula(), tradingOrder->getLotBalance(), tradingOrder->getContractSize(), price, currencyRate, decimals);
				}
				if (tradingOrder->getIsBuy())
				{
					if (tradingOrder->getIsOpen())
					{
						if (tradingOrder->getTransaction()->getInstrumentCategory() == Common::Enum::InstrumentCategory::Physical)
						{
							if (!tradingOrder->getIsPayoff())
							{
								orderComputingResult->physcialUnPaidBuyMarginSum += marginm;
								orderComputingResult->physcialUnPaidBuySum += tradingOrder->getLotBalance();
							}
						}
						else
						{
							orderComputingResult->buyMarginSum += marginm;
							orderComputingResult->buySum += tradingOrder->getLotBalance() * tradingOrder->getContractSize();
						}
					}                    
				}
				else
				{
					if (tradingOrder->getIsOpen())
					{
						if (tradingOrder->getTransaction()->getInstrumentCategory() == Common::Enum::InstrumentCategory::Physical
							&& tradingOrder->getPaidPledgeBalance() != DBL_MIN && !tradingOrder->getPaidPledgeBalance().isZero())
						{
							orderComputingResult->physcialPaidPledgeSellMarginSum += marginm;
							orderComputingResult->physcialPaidPledgeSellSum += tradingOrder->getLotBalance();
						}
						else
						{
							orderComputingResult->sellMarginSum += marginm;
							orderComputingResult->sellSum += tradingOrder->getLotBalance() * tradingOrder->getContractSize();
						}
					}
				}

				/*qDebug() << "sellMarginSum:" << orderComputingResult->sellMarginSum.toString();
				qDebug() << "buyMarginSum:" << orderComputingResult->buyMarginSum.toString();*/

				if ((calculateSetting->getMarginFormula() == 6 || calculateSetting->getMarginFormula() == 7)
					&& (Q_NULLPTR==&close))
				{
					if (tradingOrder->getIsBuy())
					{
						if (tradingOrder->getIsOpen())
						{
							if (tradingOrder->getTransaction()->getInstrumentCategory() == Common::Enum::InstrumentCategory::Physical)
							{
								if (!tradingOrder->getIsPayoff())
								{
									orderComputingResult->physcialUnPaidBuyMarginSum += tradingOrder->getNecessary(); 
								}
							}
							else
							{
								orderComputingResult->buyMarginSum += tradingOrder->getNecessary();
							}
						}   
					}
					else
					{
						if (tradingOrder->getIsOpen())
						{
							if (tradingOrder->getTransaction()->getInstrumentCategory() == Common::Enum::InstrumentCategory::Physical
								&& tradingOrder->getPaidPledgeBalance() != DBL_MIN)
							{
								orderComputingResult->physcialPaidPledgeSellMarginSum += tradingOrder->getNecessary();
							}
							else
								orderComputingResult->sellMarginSum += tradingOrder->getNecessary();
						}
					}

				}

				/*qDebug() << "sellMarginSum_2:" << orderComputingResult->sellMarginSum.toString();
				qDebug() << "buyMarginSum_2:" << orderComputingResult->buyMarginSum.toString();*/
			}

			QDecNumber ComputingOrder::calculateMargin(int marginFormula, QDecNumber lotBalance, QDecNumber contractSize, QDecNumber price, QSharedPointer<Model::Setting::CurrencyRate>& currencyRate, int decimals)
			{	
				/*qDebug() << "order  marginFormula:" << marginFormula;
				qDebug() << "order  lotBalance:" << lotBalance.toString();
				qDebug() << "order  contractSize:" << contractSize.toString();
				qDebug() << "order  price:" << price.toString();*/
				
				QDecNumber margin = 0;
				QDecNumber negate = -1;
				switch (marginFormula)
				{
				case 0:
					margin = lotBalance;
					margin = currencyRate.isNull() ? Common::roundToDecimal(margin, decimals) : Common::roundToDecimal(margin, currencyRate->getTargetCurrency()->getDecimals());
					break;
				case 1:
					margin = lotBalance * contractSize;
					margin = currencyRate.isNull() ? Common::roundToDecimal(margin, decimals) : currencyRate->exchange(margin * negate) * negate;
					break;
				case 2:
				case 6:
					margin = lotBalance * contractSize / price;
					margin = currencyRate.isNull() ? Common::roundToDecimal(margin, decimals) : currencyRate->exchange(margin * negate) * negate;
					break;
				case 3:
				case 7:
                case 8:
                case 9:
                case 10:
                case 11:
					margin = lotBalance * contractSize * price;
					margin = currencyRate.isNull() ? Common::roundToDecimal(margin, decimals) : currencyRate->exchange(margin * negate) * negate;
					break;
				case 4:
				case 5:
					margin = lotBalance;
					margin = currencyRate.isNull() ? Common::roundToDecimal(margin, decimals) : Common::roundToDecimal(margin, currencyRate->getTargetCurrency()->getDecimals());
					break;
				}
				//qDebug() << "order  margin:" << margin.toString();
				return margin;
			}
		}
	}
}
