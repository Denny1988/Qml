#ifndef ORDERENUM_H
#define ORDERENUM_H
namespace Common
{
	namespace Enum
	{
		namespace TradeOption
		{
			enum TradeOption
			{
				Invalid,
				Stop,
				Better
			};
		}

		namespace BuySell
		{
			enum BuySell
			{
				Buy,
				Sell,
				NotSet,
				CloseOrderBuySellDirection
			};
		}

		namespace OpenClose
		{
			enum OpenClose
			{
				Open,
				Close
			};
		}

		namespace OrderTypeCategory
		{
			enum OrderTypeCategory
			{
				DQ,
				Pending
			};
		}

		namespace OrderRelationType
		{
			enum OrderRelationType
			{
				Close,
				OCO,
				Assignment,
				IfDone,
				ChangeToOCO
			};
		}

		namespace PlacingType
		{
			enum PlacingType
			{
				None = 0,
				SpotTrade,
				LimitStop,

				Market,         //execute by next price
				MarketOnOpen,
				MarketOnClose,
				OneCancelOther,
				IfDone,
				NewLimit, //not use
				NewStop, //not use
				NewLimitStopSameTime,
				NewOCO,
				ChangePlacedOrder,
				MultipleClose,

				Limit_AcceptHedge,
				Limit_AcceptNew,
				Limit_AcceptUnlock,

				Stop_AcceptHedge,
				Stop_AcceptNew,
				Stop_AcceptUnlock,

				BinaryOption
			};
		}

		namespace PendingOrderExpireType
		{
			enum PendingOrderExpireType
			{
				GoodTillDate,
				GoodTillFriday,
				GoodTillMonth,
				GoodTillSession,
				GoodTillDay
			};
		}

		namespace ExpireType
		{
			enum ExpireType
			{
				Day,
				GTC,
				IOC,
				GTD,
				Session,
				FillOrKill,
				FillAndKill
			};
		}

		namespace Phase
		{
			enum Phase
			{
				None = -1,
				Placing = 255,
				Placed = 0,
				Canceled,
				Executed,
				Completed,
				Deleted
			};
		}

		namespace PriceError
		{
			enum PriceError
			{
				None,
				BetweenAskAndBid,
				TooCloseMarket,
				InvalidPrice,
				TooFarAwayMarket
			};
		}

		namespace LimitOrStop
		{ 
			enum LimitOrStop
			{
				Limit,
				Stop,
				Null = 10
			};
		}

		namespace PaymentForm
		{ 
			enum PaymentForm
			{
				FullPayment = 1,
				AdvancePayment = 2,
				Instalment = 4
			};
		}

		namespace OrderType
		{
			enum OrderType
			{
				SpotTrade,
				Limit,
				Market,
				MarketOnOpen,
				MarketOnClose,
				OneCancelOther,
				Risk,
				Stop,
				MultipleClose,
				MarketToLimit,
				StopLimit,
				FAK_Market,
				BinaryOption
			};
		}

		namespace PlacingSettingType
		{
			enum PlacingSettingType
			{
				Margin,
				Physical,
				BinaryOption
			};
		}

		namespace ConfirmStatus
		{
			enum ConfirmStatus
			{
				None,
				Placing,
				Placed,
				Canceled,
				Executed,
				PlaceTimeout
			};
		}

		namespace BSStatus
		{
			enum BSStatus
			{
				None = -1,
				SellOnly = 0,
				BuyOnly = 1,
				Both = 2,
			};
		}

		namespace SingleMutiOrder
		{ 
			enum SingleMutiOrder
			{
				SingleOrder,
				MutiOrder
			};
		}

		namespace PlacingTabType
		{ 
			enum PlacingTabType
			{
				SingleSpot,
				MultiSpot,
				Pending,
				BOCountDown,
				BOTriple
			};
		}

		namespace PhysicalStockType
		{ 
			enum PhysicalStockType
			{
				Inventory = 1,
				PendingInventory = 2,
				ShortSell = 3
			};
		}

		namespace OrderTypeMask
		{
			enum OrderTypeMask
			{
				None = 0,
				SpotTrade = 1,
				LimitStop = 2,

				Market = 4,         //execute by next price
				MarketOnOpen = 8,
				MarketOnClose = 16,
				OneCancelOther = 32,
				BinaryOption = 128
			};
		}


		namespace AllowedNewTradeSides
		{
			enum AllowedNewTradeSides
			{
				DisallowAll = 0,
				AllowNewOpenBuyOnly = 1,
				AllowNewOpenSellOnly = 2,
				AllowAll = 3
			};
		}

	}
}
#endif