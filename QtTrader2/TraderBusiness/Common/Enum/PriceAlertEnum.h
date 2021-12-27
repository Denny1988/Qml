#ifndef PRICEALERTENUM_H
#define PRICEALERTENUM_H
namespace Common
{
	namespace Enum
	{
		namespace PriceAlertState
		{
			enum PriceAlertState
			{
				//0=Pending, 1:Hit, 2:Processed, 3: Expired
				Pending,
				Hit,
				Processed,
				Expired
			};
		}

		namespace PriceAlertModifyFlag
		{
			enum PriceAlertModifyFlag
			{
				//0/null:Add, 1: Update, 2: Remove
				Add,
				Update,
				Remove
			};
		}

		namespace ConditionState
		{
			enum ConditionState
			{
				BidLessThan = 0,
				BidGreaterThan = 1,
				AskLessthan = 2,
				AskGreaterThan = 3
			};
		}
	}
}
#endif