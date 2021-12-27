#ifndef CANCELREASONENUM_H
#define CANCELREASONENUM_H

namespace Common
{
	namespace Enum
	{
		namespace CancelReason
		{
			enum CancelReason
			{
				None = -1,
				CustomerCanceled = 0,
				DealerCanceled = 1,
				RiskMonitorCanceled = 2,
				MooMocNewPositionNotAllowed = 3,
				InitialOrderCanNotBeAmended = 4,
				OrderExpired = 5,
				InvalidPrice = 6,
				RiskMonitorDelete = 7,
				AccountResetFailed = 8,
				NecessaryIsNotWithinThreshold = 9,
				MarginIsNotEnough = 10,
				AccountIsNotTrading = 11,
				InstrumentIsNotAccepting = 12,
				TimingIsNotAcceptable = 13,
				OrderTypeIsNotAcceptable = 14,
				HasNoAccountsLocked = 15,
				IsLockedByAgent = 16,
				LossExecutedOrderInOco = 17,
				ExceedOpenLotBalance = 18,
				OneCancelOther = 19,
				AccountIsInAlerting = 20,
				LimitStopAddPositionNotAllowed = 21,
				TransactionCannotBeBooked = 22,
				OnlySptMktIsAllowedForPreCheck = 23,
				InvalidTransactionPhase = 24,
				TransactionExpired = 25,
				OtherReason = 26,
				PriceChanged = 27,
				OpenOrderIsClosed = 28,
				ReplacedWithMaxLot = 29,
				SplittedForHasShortSell = 30,
				AdjustedToFullPaidOrderForHasShortSell = 31,
				TimeSpanBetweenOrders = 32,
				ExceedHighLowPrice = 33,
				ExceedDQVariation = 34,
				NotPaidoffInstalmentOrderCanNotBeClosed = 35,
				MarketPausedOrClosed = 1003,
				PriceMustBeGreaterThanZero = 1012,
				MarketPriceOrdersNotSupportedByOppositeLimit = 2013,
				SellOrderStopPriceMustBeBelowLastTradePrice = 2060,
				BuyOrderStopPriceMustBeAboveLastTradePrice = 2061,
				OrderTypeNotPermittedInPreOpen = 2130,
				OrderPriceOutsideTheLimits = 2137,
				OrderPriceOutsideBands = 2179,
				OrderTypeNotPermittedForGroup = 2311,
				OrderCannotBeModifiedInNoCancel = 7024,
			};
		}


	}
}
#endif
