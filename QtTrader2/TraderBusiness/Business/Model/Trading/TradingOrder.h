#ifndef BUSINESS_MODEL_TRADING_TRADINGORDER_H
#define BUSINESS_MODEL_TRADING_TRADINGORDER_H
#include <quuid.h>
#include <qstring.h>
#include <qdatetime.h>
#include "Common/Enum/PlaceEnum.h"
#include "Common/Enum/BOEnum.h"
#include "Common/Enum/PhysicalEnum.h"
#include "Common/Enum/InstalmentEnum.h"
#include "Common/Enum/InstrumentEnum.h"
#include "Common/Enum/CancelReasonEnum.h"
#include "Common/Util/Convert_Common.h"
#include "Common/Price.h"
#include "QDecNumber.h"
#include "OrderRelation.h"
#include "OrderInstalmentData.h"
#include <QDomNode>
#include <qDebug>
#include "Common/Struct/PlacingStruct.h"
#include "Business/Model/Setting/CurrencyRate.h"
#include <QXmlStreamReader>
#include "traderbusiness_global.h"

using namespace Common::Enum;

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			class Transaction;
			class TRADERBUSINESS_EXPORT TradingOrder
			{
			public:
				TradingOrder();
				~TradingOrder();
			private:
				QUuid  _accountId;
				QUuid  _instrumentId;
				QUuid  _id;
				QUuid  _transactionId;
				QString  _code;
				QDecNumber  _lot;
				QDecNumber  _lotBalance;
                //Note: This _lotBalanceDecimals is used in this situation, the number of open positions is 0.1, when the position is closed, when the decimal is set to 0, an integer will be displayed, and 0 will be displayed on the closing page. In this case, we will display 0.1
                int _lotBalanceDecimals;
				QDecNumber  _minLot;
				QDecNumber  _maxShow;
				bool  _isOpen;
				bool  _isBuy;
				QString  _setPrice;
				TradeOption::TradeOption  _tradeOption;
				int  _dQMaxMove;
				QDateTime  _executeTradeDay;                
                QString  _executePriceOrigin;
				Common::Price  _executePrice;
				Common::Price  _livePrice;
				QString  _autoLimitPrice;
				QString  _autoStopPrice;
				Phase::Phase   _phase;
				InstrumentCategory::InstrumentCategory  _instrumentCategory;
				bool _canClose;
				bool _isNewAddOrder;

				QDecNumber  _commissionSum;
				QDecNumber  _rebate;
				QDecNumber  _levySum;
				QDecNumber  _tradePL;
				QDecNumber  _interestPL;
				QDecNumber  _storagePL;
				QDecNumber  _paidPledge;
				QDecNumber  _payBackPledge;
				QDecNumber  _overdueCutPenalty;
				QDecNumber  _closePenalty;
				QDecNumber  _frozenFund;
				QDecNumber  _instalmentAdministrationFee;
				QDecNumber  _storagePerLot;
				QDecNumber  _interestPerLot;
				QList<QDecNumber> _interestPLNotValueds;
			    QList<QDecNumber> _storagePLNotValueds;	
				QDecNumber  _interestPLFloat;
				QDecNumber  _storagePLFloat;
				QDecNumber  _tradePLFloat;
                QDecNumber  _tradePLFloatForPending;
				QDecNumber  _storagePLValued;
				QDecNumber  _physicalPaidAmount;		
				QDecNumber  _physicalOriginValueBalance;
				QDecNumber  _physicalOriginValue;
				QDecNumber  _paidPledgeBalance;
				QDecNumber  _necessary;
				QDecNumber  _interestPLNotValued;
				QDecNumber  _storagePLNotValued;
				QDecNumber  _unrealizedswap;
				QDecNumber _physicalPaymentDiscount;
				QDecNumber _otherFee;
				QDecNumber  _openOrderCommission;
				QDecNumber  _openOrderLevy;

				QDateTime  _priceTimestamp;
				bool  _isQuotedPrice;
				QString  _setPrice2;
				QString  _limitPriceForIfDone;
				QString  _stopPriceForIfDone;

				bool _canLimitOrStop;
				bool _canLiquidation;
				QString _limitCloseOrderSumarry;
				QString _stopCloseOrderSumarry;
				QString _avarageLimitClosePrice;
				QString _avarageStopClosePrice;
				bool  _isPayoff;
				bool  _isInstalmentOverdue;				
				PhysicalType::PhysicalType _physicalType;				
				PhysicalTradeSide::PhysicalTradeSide  _physicalTradeSide;
				QUuid  _physicalRequestId;
				QUuid  _instalmentPolicyId;
				PhysicalInstalmentType::PhysicalInstalmentType  _physicalInstalmentType;
				RecalculateRateType::RecalculateRateType  _recalculateRateType;
				int  _period;
				QDecNumber  _downPayment;
				Frequence::Frequence  _instalmentFrequence;				
				int  _downPaymentBasis;	
				QDecNumber _deliveryQuantityLock;
				QDecNumber _deliveryLockLot;				
				QDecNumber _deliveryQuantity;
				QDecNumber _deliveryLot;
				QString _quantityFormat;
				CancelReason::CancelReason _cancelReason; 
				QString _transactionError;

                QMap<QString, QSharedPointer<OrderRelation>> _orderRelations;
				QMap<QString, QSharedPointer<OrderInstalmentData>> _orderInstalmentDatas;
				QMap<QUuid, QSharedPointer<TradingOrder>> _closeOrders;
				QSharedPointer<Transaction> _transaction;
				QSharedPointer<TradingOrder> _replaceOrder;
				QSharedPointer<TradingOrder> _replacedByOrder;
				QSharedPointer<Business::Model::Setting::CurrencyRate> _currencyRate;
				
				QString _tradePLFloatInFormat;
                QString _tradePLFloatForPendingInFormat;

				QUuid _BOBetTypeId;
				int _BOFrequency;
				QDateTime _BOSettleTime;
				int _BOOption;
				QString _BOOdds;
				QString _hitPrice;
				int _hitCount;
				BOHitResult::BOHitResult _BOHitResult;
				int _BOCountdown;
				QString _BOFloatPLInString;
				Common::Price  _marketBOPrice;
                QUuid _officialFixingRequestId;

                QDateTime  _tradeDay;
				
			public:
				void update(QXmlStreamReader& xmlStreamReader);
				void readBillElement(QXmlStreamReader& xmlStreamReader);
				void readOrderRelationElement(QXmlStreamReader& xmlStreamReader);
                void readOfficialFixingRequestElement(QXmlStreamReader& xmlStreamReader);
				void update(const QDomNode& xmlOrder);
				void updatePlacingOrder(QSharedPointer<Common::Struct::Order> order, QSharedPointer<Common::Struct::Transaction> transaction);
                void setTransaction(QSharedPointer<Business::Model::Trading::Transaction> transaction);
                QSharedPointer<Business::Model::Trading::Transaction> getTransaction();
				QDateTime getExecuteTradeDay();
				QDecNumber getContractSize() const;
				OrderTypeCategory::OrderTypeCategory getCategory(OrderType::OrderType orderType);
				QDecNumber getLotBalanceForDqOrderType();
				QDecNumber getLotBalanceForPendingOrderType();
				QDecNumber getLotBalanceForLimitOrderType();
				QDecNumber getLotBalanceForStopOrderType();
				QDecNumber getLotBalanceForMarketOnOpenType();
				QDecNumber getLotBalanceForMarketOnCloseType();
				QString getPaidPledgeBalanceInFormat();
				QString getPhysicalPaymentDiscountInFormat();
				QString getLotInFormat();
				QString getCloseLotInFormat(QUuid openOrderId);
				QString getLotBalanceInFormat();
				QString getCommissionSumInFormat();
				QString getLevySumInFormat();
				QString getOpenOrderCommissionInFormat();
				QString getOpenOrderLevyInFormat();
				QString getRebateInFormat();
				QString getTradePLInFormat();
				QString getTradePLFloatInFormat();
                QString getTradePLFloatForPendingInFormat();
				QString getTradePLValueInFormat();
				QString getInterestPLFloatInFormat();
				QString getStoragePLFloatInFormat();
				QString getUnrealizedswapInFormat();
				void setDeliveryLot(QDecNumber value);
				QString getDeliveryLotString();
				QString getDeliveryQuantityString();
				void calculateDeliveryLot();
				void setIsPayoff();
				bool getAllowTradeLimitStopOrder();
				bool getAllowTradeSpotTradeOrder();
				QString getCurrencyRateInFormat();
				QString getInterestRateInFormat();

				QString getOpenOrderInfo();
				int getDecimalPalcesForLot();
				bool canClear();
				void caculateLimitSummary();
				void calculate();
				QList<QSharedPointer<TradingOrder>> getPendingCloseOrders();
				int getLotBalanceDecimals(){ return _lotBalanceDecimals; }
				bool getIsNewAddOrder() { return _isNewAddOrder;}
				QDecNumber getCloseLot(QUuid openOrderId);
				void addCloseOrder(QSharedPointer<TradingOrder> closeOrder);
				void removeCloseOrder(QUuid orderId);
				QList<QSharedPointer<TradingOrder>> getCloseOrders();
				QDecNumber getLotBalance();
				void addDeliveryLockLot(const QDecNumber& value);
				QString getBOOddsInFormat();
				int getBOHitcount();
				QString getBOCountdown();
				bool isBOOrder();
				QString getBOBeginTimeStr();
				QString getBOEndTimeStr();
				QDateTime getBOBeginTime();
				QDateTime getBOEndTime();
				bool updateBOCountdown(QDateTime now);
				void setBOFloatPLInString();
				void clearData();
                bool applyOfficialFixing();
                bool officialFixingEnableStatus();

			public:
				QString getLimitCloseOrderSumarry()
				{
					return _limitCloseOrderSumarry;
				}

				QString getStopCloseOrderSumarry()
				{
					return _stopCloseOrderSumarry;
				}

				QSharedPointer<TradingOrder> getCloseOrder(QUuid orderId)
				{
					return _closeOrders[orderId];
				}

				QList<QSharedPointer<OrderRelation>> getOrderRelations()
				{
                    return _orderRelations.values();
				}

				QList<QSharedPointer<OrderInstalmentData>> getOrderInstalmentDatas()
				{
					return _orderInstalmentDatas.values();
				}

				QUuid getAccountId() const
				{
					return _accountId;
				}

				void setAccountId(const QUuid& value)
				{
					_accountId = value;
				}

				QUuid getInstrumentId() const
				{
					return _instrumentId;
				}

				void setInstrumentId(const QUuid& value)
				{
					_instrumentId = value;
				}

				QUuid getId() const
				{
					return _id;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}


				QUuid getTransactionId() const
				{
					return _transactionId;
				}

				void setTransactionId(const QUuid& value)
				{
					_transactionId = value;
				}

				QString getCode() const
				{
					return _code;
				}
			
				QDecNumber getLot() const
				{
					return _lot;
				}

				Common::Price getExecutePrice() const
				{
					return _executePrice;
				}

				void setExecutePrice(const Common::Price& value)
				{
					_executePrice = value;
				}

                QString getExecutePriceOrigin() const
                {
                    return _executePriceOrigin;
                }

                void setExecutePriceOrigin(const QString& value)
                {
                    _executePriceOrigin = value;
                }

				QDecNumber getCommissionSum() const
				{
					return _commissionSum;
				}

				QDecNumber getRebate() const
				{
					return _rebate;
				}
				
				QDecNumber getLevySum() const
				{
					return _levySum;
				}

				QDecNumber getOpenOrderCommission() const
				{
					return _openOrderCommission;
				}
				
				QDecNumber getOpenOrderLevy() const
				{
					return _openOrderLevy;
				}

				QDecNumber getInterestPerLot() const
				{
					return _interestPerLot;
				}

				QDecNumber getStoragePerLot() const
				{
					return _storagePerLot;
				}
								
				QDecNumber getTradePL() const
				{
					return _tradePL;
				}

				QDecNumber getInterestPL() const
				{
					return _interestPL;
				}

				QDecNumber getStoragePL() const
				{
					return _storagePL;
				}

				QDecNumber getTradePLFloat() const
				{
					return _tradePLFloat;
				}

                QDecNumber getTradePLFloatForPending() const
                {
                    return _tradePLFloatForPending;
                }

				QDecNumber getInterestPLFloat() const
				{
					return _interestPLFloat;
				}
				
				QDecNumber getStoragePLFloat() const
				{
					return _storagePLFloat;
				}
								
				QDecNumber getPayBackPledge() const
				{
					return _payBackPledge;
				}

				QDecNumber getOverdueCutPenalty() const
				{
					return _overdueCutPenalty;
				}

				QDecNumber getClosePenalty() const
				{
					return _closePenalty;
				}

				QDecNumber getFrozenFund() const
				{
					return _frozenFund;
				}

				QDecNumber getInstalmentAdministrationFee() const
				{
					return _instalmentAdministrationFee;
				}
				
				QDecNumber getStoragePLValued() const
				{
					return _storagePLValued;
				}
				
				Common::Price getLivePrice() const
				{
					return _livePrice;
				}

				void setLivePrice(const Common::Price& value)
				{
					_livePrice = value;
				}

				QString getAutoLimitPrice() const
				{
					return _autoLimitPrice;
				}
				
				QString getAutoStopPrice() const
				{
					return _autoStopPrice;
				}

				bool getIsPayoff() const
				{
					return _isPayoff;
				}
				
				bool getIsInstalmentOverdue() const
				{
					return _isInstalmentOverdue;
				}
				
				QDecNumber getMinLot() const
				{
					return _minLot;
				}

				QDecNumber getMaxShow() const
				{
					return _maxShow;
				}

				bool getIsOpen() const
				{
					return _isOpen;
				}

				bool getIsBuy() const
				{
					return _isBuy;
				}
				
				QString getSetPrice() const
				{
					return _setPrice;
				}
				
				QDateTime getPriceTimestamp() const
				{
					return _priceTimestamp;
				}

				bool getIsQuotedPrice() const
				{
					return _isQuotedPrice;
				}

				QString getSetPrice2() const
				{
					return _setPrice2;
				}

				QString getLimitPriceForIfDone() const
				{
					return _limitPriceForIfDone;
				}

				QString getStopPriceForIfDone() const
				{
					return _stopPriceForIfDone;
				}
				
				TradeOption::TradeOption getTradeOption() const
				{
					return _tradeOption;
				}
				
				int getDQMaxMove() const
				{
					return _dQMaxMove;
				}

				PhysicalType::PhysicalType getPhysicalType() const
				{
					return _physicalType;
				}				

				QDecNumber getPhysicalPaidAmount() const
				{
					return _physicalPaidAmount;
				}

				PhysicalTradeSide::PhysicalTradeSide getPhysicalTradeSide() const
				{
					return _physicalTradeSide;
				}

				QUuid getPhysicalRequestId() const
				{
					return _physicalRequestId;
				}
				
				QUuid getInstalmentPolicyId() const
				{
					return _instalmentPolicyId;
				}

				PhysicalInstalmentType::PhysicalInstalmentType getPhysicalInstalmentType() const
				{
					return _physicalInstalmentType;
				}

				RecalculateRateType::RecalculateRateType getRecalculateRateType() const
				{
					return _recalculateRateType;
				}
				
				int getPeriod() const
				{
					return _period;
				}

				QDecNumber getDownPayment() const
				{
					return _downPayment;
				}

				Frequence::Frequence getInstalmentFrequence() const
				{
					return _instalmentFrequence;
				}

				QDecNumber getPhysicalOriginValue() const
				{
					return _physicalOriginValue;
				}
				
				QDecNumber getPhysicalOriginValueBalance() const
				{
					return _physicalOriginValueBalance;
				}

				int getDownPaymentBasis() const
				{
					return _downPaymentBasis;
				}
				
				QDecNumber getPaidPledge() const
				{
					return _paidPledge;
				}

				QDecNumber getPaidPledgeBalance() const
				{
					return _paidPledgeBalance;
				}

				QDecNumber getNecessary() const
				{
					return _necessary;
				}

				Phase::Phase getPhase() const
				{
					return _phase;
				}	

				QDecNumber getDeliveryQuantityLock() const
				{
					return _deliveryQuantityLock;
				}

				QDecNumber getDeliveryLockLot() const
				{
					return _deliveryLockLot;
				}

				QDecNumber getDeliveryQuantity() const
				{
					return _deliveryQuantity;
				}

				QDecNumber getDeliveryLot() const
				{
					return _deliveryLot;
				}

				QDecNumber getPhysicalPaymentDiscount() const
				{
					return _physicalPaymentDiscount;
				}	
				
				QDecNumber getOtherFee() const
				{
					return _otherFee;
				}

				QString getQuantityFormat() const
				{
					return _quantityFormat;
				}

				CancelReason::CancelReason getCancelReason() const
				{
					return _cancelReason;
				}
				
				QString getTransactionError() const
				{
					return _transactionError;
				}

				
				void setDeliveryQuantityLock(QDecNumber value)
				{
					_deliveryQuantityLock  = value;
				}
				
				
				
				void setQuantityFormat(QString value)
				{
					_quantityFormat = value;
				}

				void setPhase(Phase::Phase phase)
				{
					this->_phase = phase;
				}
				
				void setTradePLFloat(QDecNumber value);
                void setTradePLFloatForPending(QDecNumber value);

				void setTradePL(QDecNumber value)
				{
					this->_tradePL = value;
				}

				void setInterestPLFloat(QDecNumber value)
				{
					this->_interestPLFloat = value;
				}

				void setStoragePLFloat(QDecNumber value)
				{
					this->_storagePLFloat = value;
				}

				bool getCanLimitOrStop()
				{
                    return _canLimitOrStop && _officialFixingRequestId.isNull();
				}

				bool getCanLiquidation()
				{
                    return _canLiquidation && _officialFixingRequestId.isNull();
				}

				void setCanLimitOrStop(bool value)
				{
					_canLimitOrStop = value;
				}

				void setCanLiquidation(bool value)
				{
					_canLiquidation = value;
				}

				void setReplaceOrder(QSharedPointer<TradingOrder> value)
				{
					_replaceOrder = value;
				}

				void setReplacedByOrder(QSharedPointer<TradingOrder> value)
				{
					_replacedByOrder = value;
				}

				bool getCanClose()
				{
					return _canClose && _isOpen;
				}
				
				void setCanClose(bool value)
				{
					_canClose = value;
				}
				
				void setOpenOrderCommission(QDecNumber value)
				{
					_openOrderCommission = value;
				}
				
				void setOpenOrderLevy(QDecNumber value)
				{
					_openOrderLevy = value;
				}

				QString getAvarageLimitClosePrice()
				{
					return _avarageLimitClosePrice;
				}

				QString getAvarageStopClosePrice()
				{
					return _avarageStopClosePrice;
				}

                void setCancelReason(CancelReason::CancelReason value)
                {
                    _cancelReason = value;
                }

                void setTransactionError(QString value)
				{
					_transactionError = value;
				}

				QUuid getBOBetTypeId()
				{
					return _BOBetTypeId;
				}

				void setBOBetTypeId(QUuid value)
				{
					_BOBetTypeId = value;
				}
				 
				int getBOFrequency()
				{
					return _BOFrequency;
				}

				void setBOFrequency(int value)
				{
					_BOFrequency = value;
				}

				QDateTime getBOSettleTime()
				{
					return _BOSettleTime;
				}

				void setBOSettleTime(QDateTime value)
				{
					_BOSettleTime = value;
				}
				
				int getBOOption()
				{
					return _BOOption;
				}

				void setBOOption(int value)
				{
					_BOOption = value;
				}
				
				int getHitCount()
				{
					return _hitCount;
				}

				QString getHitPrice()
				{
					return _hitCount == 0 ? "" : _hitPrice;
				}

				QString getBOOdds()
				{
					return _BOOdds;
				}
				
				BOHitResult::BOHitResult getBOHitResult()
				{
					return _BOHitResult;
				}

				void setBOCountdown(int value)
				{
					_BOCountdown = value;
				}

				QString getBOFloatPLInString()
				{
					return _BOFloatPLInString;
				}

				void setMarketBOPrice(const Common::Price& value)
				{
					_marketBOPrice = value;
				}

                QDateTime getTradeDay()
                {
                    return _tradeDay;
                }


			};
		}
	}
}
#endif
