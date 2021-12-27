#include "PendingInventoryData.h"
#include "Transaction.h"

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			PendingInventoryData::PendingInventoryData()
			{
			}

			PendingInventoryData::~PendingInventoryData()
			{
			}

			void PendingInventoryData::update(QSharedPointer<DeliveryRequest> deliveryRequest)
			{
				this->_id = deliveryRequest->getId();
				this->_code = deliveryRequest->getCode();
				this->_description = deliveryRequest->getInstrumentDesc();
				this->_instrumentId = deliveryRequest->getInstrumentId();
				this->_quantity = deliveryRequest->getRequireQuantity();
				if (!deliveryRequest->getSubmitTime().isNull()) this->_submitTime = deliveryRequest->getSubmitTime();
				if (deliveryRequest->getStatus() == DeliveryRequestStatus::Accepted || deliveryRequest->getStatus() == DeliveryRequestStatus::Approved)
				{
					if (!deliveryRequest->getDeliveryTime().isNull()) this->_settlementTime = deliveryRequest->getDeliveryTime();
				}
				else
				{
					if (!deliveryRequest->getAvailableTime().isNull()) this->_settlementTime = deliveryRequest->getAvailableTime();
				}
				this->_unit = deliveryRequest->getUnit();
				this->updateDeliveryRequestStatus(deliveryRequest->getStatus());
			}

			void PendingInventoryData::updateDeliveryRequestStatus(DeliveryRequestStatus::DeliveryRequestStatus status)
			{
				switch (status)
				{
					case DeliveryRequestStatus::Accepted:
						this->_status = InventoryInProcessStatus::Submitted;
						break;
					case DeliveryRequestStatus::Approved:
						this->_status  = InventoryInProcessStatus::Approved;
						break;
					case DeliveryRequestStatus::Stocked:
						this->_status  = InventoryInProcessStatus::StockIsAvailable;
						break;
					case DeliveryRequestStatus::Delivered:
					case DeliveryRequestStatus::OrderCreated:
						this->_status  = InventoryInProcessStatus::DeliveryCompletede;
						break;                    
					case DeliveryRequestStatus::Canceled:
						this->_status  = InventoryInProcessStatus::Canceled;
						break;
				}
			}

			void PendingInventoryData::update(QSharedPointer<ScrapDeposit> scrapDeposit)
			{
				this->_id = scrapDeposit->getId();
				this->_code = scrapDeposit->getCode();
				this->_description = scrapDeposit->getScrapInstrumentCode();
				this->_instrumentId = scrapDeposit->getTradeInstrumentId();
				if (!scrapDeposit->getSubmitTime().isNull()) this->_submitTime = scrapDeposit->getSubmitTime();
				if (!scrapDeposit->getAcceptTime().isNull()) this->_settlementTime = scrapDeposit->getAcceptTime();
				this->_unit = scrapDeposit->getUnit();
				this->updateQuantity(scrapDeposit);
				this->updateScrapDepositStatus(scrapDeposit->getStatus());
			}

			void PendingInventoryData::updateQuantity(QSharedPointer<ScrapDeposit> scrapDeposit)
			{
				if (!scrapDeposit->getFinalQuantity().toDouble() == 0.0)
					this->_quantity = scrapDeposit->getFinalQuantity();
				else
					this->_quantity = scrapDeposit->getRawQuantity();
			}

			void PendingInventoryData::updateScrapDepositStatus(ScrapDepositStatus::ScrapDepositStatus status)
			{
				switch (status)
				{
					case ScrapDepositStatus::ExaminationInProcess:
						this->_status = InventoryInProcessStatus::ExaminationInProcess;
						break;
					case ScrapDepositStatus::ExaminationCompleted:
						this->_status = InventoryInProcessStatus::ExaminationCompleted;
						break;
					case ScrapDepositStatus::Canceled:
						this->_status = InventoryInProcessStatus::Canceled;
						break;
				}            
			}
		}
	}
}
