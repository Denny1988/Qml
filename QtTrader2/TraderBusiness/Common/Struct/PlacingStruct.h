#ifndef PLACINGSTRUCT_H
#define PLACINGSTRUCT_H
#include <QUuid>
#include <qdatetime.h>
#include <QSharedPointer>
#include "QDecNumber.h"
#include <QMap>
#include "Common/Enum/InstrumentEnum.h"
#include "Common/Enum/TransactionEnum.h"
#include "Common/Enum/PlaceEnum.h"
#include "Common/Enum/PhysicalEnum.h"
#include "Common/Enum/InstalmentEnum.h"
#include "Common/Enum/BOEnum.h"

using namespace Common::Enum;

namespace Common
{
	namespace Struct
	{
		struct NumericUpDownParam
		{
			QDecNumber  _step;
			QDecNumber  _min;
			QDecNumber  _max;
			QDecNumber  _default;
			int  _decimals;
			bool _isMovePipsNeeded;
			QDecNumber  _closeMin;
		};

		struct PaymentTypeData
		{
			bool  _showPaymentType;
			bool  _showFullPaymentButton;
			bool  _showAdvancePaymentButton;
			bool  _showInstalmentButton;
			bool  _isSelectFullPayment;
			bool  _isSelectAdvancePayment;
			bool  _isSelectInstalment;
		};

		struct LotNumericUpDownParam
		{
			QSharedPointer<NumericUpDownParam> _lotNumericUpDown;
			bool _isPlaceBuyFiFo;
			bool _isPlaceSellFiFo;
			bool _closeByFIFO;
			bool _isBuyNet;//true = Buy net / false = Sell net			
		};

		struct CloseOrder
		{
                        QString _orderCode;
			bool  _isClose;
			QUuid  _orderId;
			double  _closeLot;
			double  _lotBalance;
			int  _movePips;
			QDateTime _executeTradeDay;
			bool  _isBuy;
			QString _executePrice; 
			QString _physicalRemark;
			int  _decimals;
			QString _avarageLimitClosePrice;
			QString _avarageStopClosePrice;			
			QString _tradePLFloat;
			QSharedPointer<NumericUpDownParam> _lotNumericUpDown;
			QUuid  _accountId;
			QString _accountCode;
            double  _remainderLimitLot;
            double  _remainderStopLot;
            double _closeLimitLot;
            double _closeStopLot;
            QString _lotBalanceFormat;

			QSharedPointer<CloseOrder> copy()
			{
				QSharedPointer<CloseOrder> newCloseOrder(new CloseOrder);
                                newCloseOrder->_orderCode = _orderCode;
                                newCloseOrder->_isClose = _isClose;
				newCloseOrder->_orderId = _orderId;
				newCloseOrder->_closeLot = _closeLot;
				newCloseOrder->_lotBalance = _lotBalance;
				newCloseOrder->_movePips = _movePips;
				newCloseOrder->_executeTradeDay = _executeTradeDay;
				newCloseOrder->_isBuy = _isBuy;
				newCloseOrder->_executePrice = _executePrice;
				newCloseOrder->_physicalRemark = _physicalRemark;
				newCloseOrder->_decimals = _decimals;
				newCloseOrder->_avarageLimitClosePrice = _avarageLimitClosePrice;
				newCloseOrder->_avarageStopClosePrice = _avarageStopClosePrice;
				newCloseOrder->_tradePLFloat = _tradePLFloat;
				newCloseOrder->_lotNumericUpDown = _lotNumericUpDown;
				newCloseOrder->_accountId = _accountId;
				newCloseOrder->_accountCode = _accountCode;
                                newCloseOrder->_remainderLimitLot = _remainderLimitLot;
                                newCloseOrder->_remainderStopLot = _remainderStopLot;
                                newCloseOrder->_closeLimitLot = _closeLimitLot;
                                newCloseOrder->_closeStopLot = _closeStopLot;
				return newCloseOrder;
			}
		};

		struct PriceUpDownParam
		{
			double  _step;
			double  _min;
			double  _max;
			double  _default;
			int  _decimals;
		};

		struct InstalmentDetailsCalculateResult
		{
			QString  _period;
			QString  _principal;
			QString  _interest;
			QString  _instalmentAmount;
			QString  _currencyName;
		};

		struct InstalmentCalculateResult
		{
			QList<QSharedPointer<InstalmentDetailsCalculateResult>>  _instalmentDetailsResults;
			QString  _totalPrincipalInFormat;
			QString  _totalInterestInFormat;
			QString  _totalInstalmentAmountInFormat;
		};

		struct Order
		{
			QUuid _id;
			QUuid _transactionId;
			TradeOption::TradeOption _tradeOption;
			bool _isOpen;
			bool _isBuy;
			QString _setPrice;
			QString _setPrice2;
			int _DQMaxMove;
			QString _lot;
			QString _minLot;
			QString _maxShow;
			QDateTime _priceTimestamp;
			bool _priceIsQuote;
			QString _originalLot;
						
			PhysicalTradeSide::PhysicalTradeSide _physicalTradeSide;
			QUuid _instalmentPolicyId;
			PhysicalInstalmentType::PhysicalInstalmentType _physicalInstalmentType;
			RecalculateRateType::RecalculateRateType _recalculateRateType;
			int _period;
			double _downPayment;
			Frequence::Frequence _instalmentFrequence;
			QString _advanceAmount;
			int _downPaymentBasis;
			bool _isShowShortSelling;
			QString _interestRate;
			
		
			QString _BOSetPrice;
			QString _BOSettleTime;
			QUuid _BOBetTypeId;
			int _BOFrequency;
			QString _BOOdds;
			int _BOOption;
			QString _marketBOPrice;
			BOBetOption::BOBetOption _BetOption;
			int _lastOrderTimeSpan;

			QString _limitPriceForIfDone;
            QString _stopPriceForIfDone;
		};

		struct OrderRelation
		{
			OrderRelationType::OrderRelationType _relationType;
			QUuid _openOrderId;
			QUuid _closeOrderId;
			QString _closeLot;
			QString _openOrderInfo;
			QString _openOrderPrice;
			QString _openOrderTime;
			int _lotDecimal;
		};

		struct Transaction
		{
			QUuid _accountId;
			QUuid _instrumentId;
			QString _accountCode;
			QString _description;
			QUuid _id;
			QString _code;
			PlacingSettingType::PlacingSettingType _placingSettingType;
			TransactionType::TransactionType _type;
			TransactionSubType::TransactionSubType _subType;
			Phase::Phase _phase;
			QString _cancelReason;			 
			PendingOrderExpireType::PendingOrderExpireType _expireType;
			QDateTime _expireTime;
			/*QDateTime BeginTime;
			QDateTime EndTime;
			QDateTime SubmitTime;
			QDateTime ExecuteTime;*/
			QDateTime _endTime;
			OrderType::OrderType _orderType;
			double _contractSize;
			QUuid _submitorId;
			QUuid _assigningOrderId;
			InstrumentCategory::InstrumentCategory _instrumentCategory;
			QList<QSharedPointer<OrderRelation>>  _orderRelations;
			QList<QSharedPointer<Order>>  _orders;
			QString _fee;
			QString _commission;
			bool _isPercentage;
		};

		struct QuickPlacingDetail
		{
			QString _timeStamp;	
			QString	_prevClose;
			QString	_netChange;
			QString _buyRate;
			QString	_sellRate;
			QString _high;
			QString _low;
			QString _bestBuy;
			QString _bestSell;
		};

		struct PhysicalSetting
		{
			QUuid _instalmentPolicyId;
			QDecNumber _minDownPayment;
			QDecNumber _maxDownPayment;
			int _downPaymentBasis;
			QDecNumber _interestRate;
			QDecNumber _administratioFee;
			bool _isPercentage;
		};

		struct InstalmentSettingParameter
		{
			QList<PhysicalInstalmentType::PhysicalInstalmentType> _physicalInstalmentTypes;
			QList<RecalculateRateType::RecalculateRateType> _recalculateRateTypes;
			QList<Frequence::Frequence> _instalmentFrequences;
			QMap<Frequence::Frequence, QList<int>> _periods;

		};
	}
}
#endif
