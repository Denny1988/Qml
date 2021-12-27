#ifndef BUSINESS_MODEL_COMPUTING_CHANGEFUND_H
#define BUSINESS_MODEL_COMPUTING_CHANGEFUND_H
#include <quuid.h>
#include "QDecNumber.h"

namespace Business
{
	namespace Model
	{
		namespace Computing
		{
			class ChangeFund
			{
			public:
				QUuid currencyId;
				QDecNumber necessary;
				QDecNumber partialPaymentPhysicalNecessary;
				QDecNumber  interestPLFloat;
				QDecNumber  storagePLFloat;
				QDecNumber  tradePLFloat;
				QDecNumber  pledgeAmount;
				QDecNumber  totalPaidAmount;
				QDecNumber totalUnpaidAmount;
				QDecNumber  estimateCloseCommission;
				QDecNumber  estimateCloseLevy;
			};
		}
	}
}
#endif
