#ifndef COMMONENUMS_H
#define COMMONENUMS_H
namespace Common
{
	namespace Enum
	{
		namespace ExchangeSystem
		{
			enum ExchangeSystem
			{
				Local = 0,
				Bursa = 1,
			};
		}

		namespace VolumeNecessaryOption
		{
			enum VolumeNecessaryOption
			{
				Progessive = 0,
				Flat
			};
		}

		namespace UpdateType
		{
			enum UpdateType
			{
				Account,
				Instrument,
				OpenOrder,
				WorkOrder,
				Inventory,
				PendingInventory,
				ShortSell
			};
		}
		namespace FunctionModelMenuType
		{
			enum FunctionModelMenuType
			{
				QuotationCard,
				QuotationList,
				BOQuotationCard,
				AccountInfor,
				Order,
				Chart,				
				RealTimeChart
			};
		}

		namespace SpinBoxType
		{
			enum SpinBoxType
			{
				Lot,
				MovePips,
				BOAmount
			};
		}

		namespace AlertLevel
		{
			enum AlertLevel
			{
				Level0,
				Level1,
				Level2, 
				Level3
			};
		}

		namespace OrderTabType
		{
			enum OrderTabType
			{
				OpenOrder,
				WorkingOrder,
				BinaryOption,
				Position,
				Physical,
				OrderQuery,
				Log,
				Chat,
				News,				
				PriceAlert
			};
		}

		namespace UpdateAction
		{
			enum UpdateAction
			{
				Init,
				Add,
				Modify,
				Delete
			};
		}
	}
}
#endif