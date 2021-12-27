#ifndef BUSINESS_MODEL_TRADING_PENDINGINVENTORYDATA_H
#define BUSINESS_MODEL_TRADING_PENDINGINVENTORYDATA_H

#include "Common/Struct/PlacingStruct.h"
#include "Common/Enum/PlaceEnum.h"
#include "Common/Enum/PhysicalEnum.h"
#include "DeliveryRequest.h"
#include "ScrapDeposit.h"

using namespace Common::Enum;
using namespace Common::Struct;

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			class PendingInventoryData
			{				
			public:
				PendingInventoryData();
				~PendingInventoryData(); 
				void update(QSharedPointer<DeliveryRequest> deliveryRequest);
				void update(QSharedPointer<ScrapDeposit> scrapDeposit);
				void updateScrapDepositStatus(ScrapDepositStatus::ScrapDepositStatus status);
				void updateDeliveryRequestStatus(DeliveryRequestStatus::DeliveryRequestStatus status);
				void updateQuantity(QSharedPointer<ScrapDeposit> scrapDeposit);

			private:
				QUuid _id;
				QUuid _accountId;
				QUuid _instrumentId;
				QString _code;
				QString _accountCode;
				QString _description;
				QString _quantity;
				QString _unit;
				QDateTime _submitTime;
				QDateTime _settlementTime;
				InventoryInProcessStatus::InventoryInProcessStatus _status;
				
			public:
				QUuid getId()
				{
					return _id;
				}

				QUuid getInstrumentId()
				{
					return _instrumentId;
				}

				void setAccountId(QUuid value)
				{
					_accountId = value;
				}


				QUuid getAccountId()
				{
					return _accountId;
				}

				void setAccountCode(QString value)
				{
					_accountCode = value;
				}

				QString getCode()
				{
					return _code;
				}

				QString getAccountCode()
				{
					return _accountCode;
				}

				QString getDescription()
				{
					return _description;
				}

				QString getQuantity()
				{
					return _quantity;
				}

				QString getUnit()
				{
					return _unit;
				}

				QDateTime getSubmitTime()
				{
					return _submitTime;
				}

				QDateTime getSettlementTime()
				{
					return _settlementTime;
				}

				InventoryInProcessStatus::InventoryInProcessStatus getStatus()
				{
					return _status;
				}
			};
		}
	}
}

#endif
