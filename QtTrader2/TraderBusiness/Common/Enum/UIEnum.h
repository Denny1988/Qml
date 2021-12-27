#ifndef UIENUM_H
#define UIENUM_H

namespace Common
{
	namespace Enum
	{
		namespace MenuType
		{
			enum MenuType
			{
				THEME_MENU= 0,
				SYTEM_MENU
			};
		}
		namespace ReportType
		{
			enum ReportType
			{
				Ledger,
				Statement,
				AccountSummary,
				StockDelivery,
				WidthdrawalNotice,
				DepositCombined,
				DepositIn,
				DepositOut,
				OrderChecking,
				TransactionClearingListing,
				OpenPosition
			};
		}

		namespace MainSytemMenu
		{
			enum MainSytemMenu
			{
				Margin,
				ChangeLeverage,
				SptReconfirm,

				SystemMenu,
				ThemeMenu,
				ChangePassword,
				UserCenter,
				OwnerRegistration,
				BankAccount

			};
		}
		
	}
}
#endif  // ENUM_H
