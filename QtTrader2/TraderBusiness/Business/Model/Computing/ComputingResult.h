#ifndef BUSINESS_MODEL_COMPUTING_COMPUTINGRESULT_H
#define BUSINESS_MODEL_COMPUTING_COMPUTINGRESULT_H
#include <quuid.h>
#include <qdatetime.h>
#include "ChangeFund.h"
#include <QMap>
#include <QSharedPointer>
#include "QDecNumber.h"
#include "Common/Price.h"
#include "Common/Enum/PlaceEnum.h"
#include "Common/Enum/FrameworkEnum.h"

namespace Business
{
	namespace Model
	{
		namespace Computing
		{
			class DemandComputingResult
			{
			};

			class AccountComputingResult : public DemandComputingResult
			{
			public:
				QUuid accountId;
				//QString accountCode;
				//bool isSelect;
				//bool isNoShowAccountStatus;
				QSharedPointer<ChangeFund> fund;
				QMap<QUuid, QSharedPointer<ChangeFund>> subFund;
			};

			class InstrumentComputingResult : public DemandComputingResult
			{
			public:
				InstrumentComputingResult()
				{
					necessary = 0.0;
					partialPaymentPhysicalNecessary = 0.0;
					interestPLFloat = 0.0;
					storagePLFloat = 0.0;
					tradePLFloat = 0.0;
					pledgeAmount = 0.0;
					totalPaidAmount = 0.0;
					totalUnpaidAmount = 0.0;
					estimateCloseCommission = 0.0;
					estimateCloseLevy = 0.0;
				}
				QUuid currencyId;
				QDecNumber necessary;
				QDecNumber partialPaymentPhysicalNecessary;

				QDecNumber  interestPLFloat;
				QDecNumber  storagePLFloat;
				QDecNumber  tradePLFloat;

				QDecNumber  pledgeAmount;
				QDecNumber  totalPaidAmount;
				QDecNumber  totalUnpaidAmount;

				QDecNumber  estimateCloseCommission;
				QDecNumber  estimateCloseLevy;
			};


			class OrderComputingResult : public DemandComputingResult
			{
			public:
				QUuid orderId;
				QDecNumber buyMarginSum;
				QDecNumber sellMarginSum;
				QDecNumber buySum;
				QDecNumber sellSum;
				QDecNumber physcialUnPaidBuyMarginSum;
				QDecNumber physcialUnPaidBuySum;
				QDecNumber physcialPaidPledgeSellMarginSum;
				QDecNumber physcialPaidPledgeSellSum;
				
				QDecNumber  marketValue;
				QDecNumber  marketValueInStock;
				QString  marketValueInStockInFormat;
				QDecNumber  unpaidAmount;
				QString  unpaidAmountInFormat;
				QDecNumber  pledgeAmount;
				QString  pledgeAmountInFormat;
				QDecNumber  totalPaidAmount;
				QString  totalPaidAmountInFormat;
				QDecNumber  totalUnpaidAmount;
				QString  totalUnpaidAmountInFormat;

				QDecNumber  interestPLFloat;
				QString  interestPLFloatInFormat;
				QDecNumber  storagePLFloat;
				QString  storagePLFloatInFormat;
				QDecNumber  tradePLFloat;
				QString  tradePLFloatInFormat;

				QDecNumber  estimateCloseCommission;
				QDecNumber  estimateCloseLevy;
				
			};

			class OverridedQuotationComputingResult : public DemandComputingResult
			{
			public:
				int priceTrend;
				Common::Price  ask;
				Common::Price  bid;
				QString  singlePrice;
				QString  open;
                QString  askOpen;
                QString  bidOpen;
				QString  high;
				QString  low;
				QString  last;
				QString  prevClose;
				QString netChange;
				QString  interestRateBuy;
				QString  interestRateSell;
				QString  volume;
				QString  totalVolume;
				QDateTime  timestamp;

			};

			class WorkingOrderComputingResult : public DemandComputingResult
			{
			public:
				QUuid      orderId;
				Common::Enum::Phase::Phase  phase;
				QDateTime  submitTime;
				QDateTime  expireTime;
				QString    accountCode;
				QString    instrumentCode;
				QDecNumber lot;
				bool       openClose;
				bool       isBuy;
				QString    price;
				QString    executedPrice;
				Common::Enum::OrderType::OrderType  orderType;
				QDecNumber tradePL;
				QDecNumber fee;
				QDecNumber levy;
				QString    remarks;

			};
			class InstrumentResult : public DemandComputingResult
			{
			public:
				QUuid      instrumentId;
				QString    instrumentCode;
				QDateTime  timeStamp;
				QString    bid;
				QString    ask;
				QString    hight;
				QString    low;
				QString    openPrice;
				QString    lastOpenPrice;
				QString    closePrice;
				QDecNumber change;
				QDecNumber buyRate;
				QDecNumber sellRate;
			};
		}
	}
}
#endif
