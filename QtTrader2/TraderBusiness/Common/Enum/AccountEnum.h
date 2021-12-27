#ifndef ACCOUNTTYPE_H
#define ACCOUNTTYPE_H
namespace Common
{
	namespace Enum
	{
		namespace AccountType
		{
			enum AccountType
			{
				Common,
				Agent,
				Company,
				Transit,//used by trader, to hide history open order and disable close order
				BlackList//Is same as AccountType.Common except add notify message
			};
		}

		namespace AccountHierarchicalWrapperItemType
		{
			enum AccountHierarchicalWrapperItemType
			{
				Root,
				AccountCode,
				AccountName,
				Currency,
				Balance,
				Necessary,
				TradePLFloat,
				TotalUnrealisedSwap,
				UnrealisedPL,
				Equity,
				NetAsset,
				Usable,
				Unclear,
				CreditAmount,
				Ratio,
				AlertLevel,
				PledgeAmount,
				FrozenFund,
				TotalPaidAmount,
				PartialPaymentPhysicalNecessary,
				TotalUnpaidAmount,
				SalesCode,
				EstimateCloseCommission,
				EstimateCloseLevy
			};
		}
	}
}
#endif