#ifndef TRADINGORDERENUM_H
#define TRADINGORDERENUM_H
namespace Common
{
	namespace Enum
	{
		namespace OrderQueryType
		{
			enum OrderQueryType
			{
				All = 0,
				ExecutedCloseOrder = 1,
				ExecutedOpenOrder = 2,
				ExecutedOrder = 3,
				UnconfirmPendingOrder = 4
			};
		}
		namespace OrderViewType
		{
			enum OrderViewType
			{
				OpenOrderViewTable,
				WorkingOrderViewTable,
				OrderQueryViewTable,
			};
		}
		namespace FormatType
		{
			enum FormatType
			{
				Percentage,
				NullValue,
				CrossValue
			};
		}
		
		/*class EnumHelper
		{
		};*/
	}
}
#endif