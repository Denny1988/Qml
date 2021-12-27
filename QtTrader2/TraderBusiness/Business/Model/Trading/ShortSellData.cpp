#include "ShortSellData.h"
#include "Transaction.h"
#include "Business/Model/Setting/BusinessInstrument.h"
#include "Business/Model/Setting/BusinessAccount.h"
#include "Common/Util/Convert_Common.h"


namespace Business
{
	namespace Model
	{
		namespace Trading
        {

             ShortSellData::ShortSellData()
             {
                  _canClose = true;
             }

             void ShortSellData::setCanClose(bool enable)
             {
                  _canClose = enable;
             }

			QString ShortSellData::getQuantity()
			{
				return Common::roundToString((_tradingOrder->getLotBalance() * 
					this->_tradingOrder->getTransaction()->getContractSize() * 
					this->_tradingOrder->getTransaction()->getBusinessInstrument()->getWeight()).toDouble(),
					this->_tradingOrder->getTransaction()->getBusinessInstrument()->getPhysicalLotDecimal());
			}

			QString ShortSellData::getUnit()
			{
				return this->_tradingOrder->getTransaction()->getBusinessInstrument()->getUnit();
			}

			QString ShortSellData::getAccountCode()
			{
				return	this->_tradingOrder->getTransaction()->getAccountCode();
			}

			QString ShortSellData::getInstrumentCode()
			{
				return	this->_tradingOrder->getTransaction()->getBusinessInstrument()->getDescription();
                        }

			void ShortSellData::setButtonIsEnabled()
			{
				if (_tradingOrder.isNull())
				{
					_canClose = false;
					return;
				}
				if (!this->_tradingOrder->getTransaction()->getBusinessAccount()->getAllowedTrade()
					|| this->_tradingOrder->getTransaction()->getBusinessAccount()->getAccountType() == AccountType::Transit
					|| this->_tradingOrder->getTransaction()->getBusinessInstrument()->getExpired())
				{
					_canClose = false;
					return;
				}
				_canClose = true;
			}
		}
	}
}
