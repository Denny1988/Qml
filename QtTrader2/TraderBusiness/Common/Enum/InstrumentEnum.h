#ifndef INSTRUMENTENUM_H
#define INSTRUMENTENUM_H
namespace Common
{
	namespace Enum
	{
		namespace PriceType
		{
			enum PriceType
			{
				WatchOnly,
				ReferenceOnly,
				DealingEnable,
				OriginEnable,
				PriceType3
			};
		}

		namespace InstrumentCategory
		{
			enum InstrumentCategory
			{
				Forex=10,
				Physical=20
			};
		}

		namespace SecurityTradingStatus
		{
			enum SecurityTradingStatus
			{
				TradingHalt = 2,
				PriceIndication = 5,
				ReadyToTrade = 17,
				NotAvailableForTrading = 18,
				UnknownOrInvalid = 20,
				PreOpen = 21,
				OpeningRotation = 22,
				PreCross = 24,
				Cross = 25,
				NoCancel = 26
			};
		}

		namespace PriceTrend
		{
			enum PriceTrend
			{
				NoChange,
				Up,
				Down
			};
		}
		namespace GridItemEnum
        {
			enum GridItemEnum
			{  
                GRAP = 2
			};
		}

		namespace QuickPlacingType
		{
			enum QuickPlacingType
			{  
				Font,
				Back
			};
		}

		namespace InstrumentStatus
		{
			enum InstrumentStatus
			{
				None = 0,
				DayOpen = 1,
				DayClose = 2,
				SessionOpen = 3,
				SessionClose = 4,
				StopPlace = 5,
				NotInTrading = 6,
				DayOpening = 7,
				DayClosing = 8,
				SessionOpening = 9,
				SessionClosing = 10,
				MOOOpen = 11,
				MOOClosed = 12,
				MOCOpen = 13,
				MOCClosed = 14,
                DayCloseQuotationReceived = 15,
                AfterDayCloseBeforeDayOpen = 16
			};
		}

		namespace FeeFormula
		{ 
			enum FeeFormula
			{

				FixedAmount = 0,
				CS = 1,
				CSDividePrice = 2,
				CSMultiplyPrice = 3,
				Pips = 4,
				PricePips = 5,
				RealizedLoss = 6,
				RealizedProfit = 7,
				RealizedPL = 8,
				SharedPL = 9
			}; 
		}


		namespace FeeCalculateType
		{ 
			enum FeeCalculateType
			{
				Commission,
				Levy,
				EstimateCloseCommission,
				EstimateCloseLevy
			};
		}
	}
}
#endif
