#ifndef PHYSICALENUM_H
#define PHYSICALENUM_H
namespace Common
{
	namespace Enum
	{
		namespace PhysicalTradeSide
		{
			enum PhysicalTradeSide
			{
				None = 0,
				Buy = 1,
				Sell = 2,
				ShortSell = 4,
				Delivery = 8,
				Deposit = 16
			};
		}

		namespace PhysicalChargeBasis
		{
			enum PhysicalChargeBasis
			{
				DayOpenPrice = 0,
				MkTPrice = 1,
				UnitFixAmount = 2,
				OnSpecification = 3
			};
		}

		namespace DeliveryRequestStatus
		{
			enum DeliveryRequestStatus
            {
				Accepted = 0,
				Approved = 1,
				Stocked = 2,
				Delivered = 3,
				OrderCreated = 4,
				Hedge = 5,
				Canceled = 100
			};
		}

		namespace ScrapDepositStatus
		{
			enum ScrapDepositStatus
            {
				ExaminationInProcess = 0,
				ExaminationCompleted = 1,
				ContractConverted = 2,
				Canceled = 100,
			};
		}

		namespace InventoryInProcessStatus
		{
			enum InventoryInProcessStatus
			{
                ExaminationInProcess,
                ExaminationCompleted,
                ContractConverted,
                Submitted,
                Approved,
                StockIsAvailable,
                DeliveryCompletede,
				Canceled,
			};
		}

		namespace AllowedPaymentForm
		{
			enum AllowedPaymentForm
			{
				FullPayment = 1,
				AdvancePayment = 2,
				Instalment = 4
			};
			Q_DECLARE_FLAGS(AllowedPaymentFormFlags, AllowedPaymentForm)
		}

		namespace DiscountOption
		{
			enum DiscountOption
			{
				Progressive = 0,
				Flat = 1
			};
		}

		namespace DiscountBasis
		{
			enum DiscountBasis
			{
				Percentage = 0,
				UnitAmount = 1
			};
		}
	}
}
#endif
