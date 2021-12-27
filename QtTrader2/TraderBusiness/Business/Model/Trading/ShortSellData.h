#ifndef BUSINESS_MODEL_TRADING_SHORTSELLDATA_H
#define BUSINESS_MODEL_TRADING_SHORTSELLDATA_H

#include "Common/Struct/PlacingStruct.h"
#include <QList>
#include <QMap>
#include "Common/Enum/PlaceEnum.h"
#include "TradingOrder.h"
#include "Business/Model/Computing/ComputingResult.h"
#include "traderbusiness_global.h"

using namespace Common::Enum;
using namespace Common::Struct;

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			class TRADERBUSINESS_EXPORT ShortSellData
			{
			private:
                bool _canClose; // set default == true for macx
				QString _currency;
				QSharedPointer<Business::Model::Computing::OrderComputingResult> _orderComputingResult;
				QSharedPointer<TradingOrder> _tradingOrder;
			
            public:
                ShortSellData();
                void setCanClose(bool enable);
				QString getQuantity();
				QString getUnit();
				QString getAccountCode();
				QString getInstrumentCode();
				void setButtonIsEnabled();

			public:

                bool getCanClose()
                {
                    return _canClose;
                }

				QString getCurrency()
				{
					return _currency;
				}
								
				QSharedPointer<Business::Model::Computing::OrderComputingResult> getOrderComputingResult()
				{
					return _orderComputingResult;
				}

				QSharedPointer<TradingOrder> getTradingOrder()
				{
					return _tradingOrder;
				}

				void setOrderComputingResult(QSharedPointer<Business::Model::Computing::OrderComputingResult> value)
				{
					_orderComputingResult = value;
				}

				void setTradingOrder(QSharedPointer<TradingOrder> value)
				{
					_tradingOrder = value;
				}
								
				void setCurrency(QString value)
				{
					_currency = value;
				}
			};
		}
	}
}

#endif
