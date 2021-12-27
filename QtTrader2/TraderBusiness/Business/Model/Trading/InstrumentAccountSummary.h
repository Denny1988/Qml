#ifndef BUSINESS_MODEL_TRADING_INSTRUMENTACCOUNTSUMMARY_H
#define BUSINESS_MODEL_TRADING_INSTRUMENTACCOUNTSUMMARY_H

#include "QDecNumber.h"
#include <QUuid>
#include <QSharedPointer>
#include <QMap>
#include "Common/Util/Convert_Common.h"
#include "Business/Model/Setting/Currency.h"

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			class InstrumentSummaryPLFloat
			{
			public:
				InstrumentSummaryPLFloat(QSharedPointer<Model::Setting::Currency> currency)
				{
					_currency = currency;
					_PLFloat = 0.0;
				};
				~InstrumentSummaryPLFloat(){}; 

			private:
				QDecNumber _PLFloat;
				QSharedPointer<Model::Setting::Currency> _currency;
			
			public:
				void addPLFloat(QDecNumber plfloat)
				{
					_PLFloat += plfloat;
				}

				void clearPLFloat()
				{
					_PLFloat = 0.0;
				}

				QString getCurrencyCode()
				{
					return _currency->getCode();
				}

				QString getPLFloatInFormatString()
				{
					return _PLFloat.isZero() ? "" : Common::roundToString(_PLFloat.toDouble(), _currency->getDecimals(), true);
				}

				QSharedPointer<InstrumentSummaryPLFloat> Clone()
				{
					QSharedPointer<InstrumentSummaryPLFloat> instrumentSummaryPLFloat(new InstrumentSummaryPLFloat(_currency));
					return instrumentSummaryPLFloat;
				}
			};

			class InstrumentAccountSummary
			{				
			public:
				InstrumentAccountSummary(int decimals, bool isShow, QMap<QUuid, QSharedPointer<InstrumentSummaryPLFloat>> instrumentSummaryPLFloats)
				{
					_decimals = decimals;
					_isShow = isShow;
					_buyLot = 0.0;
					_sellLot = 0.0;
					_instrumentSummaryPLFloats = instrumentSummaryPLFloats;
				};
				~InstrumentAccountSummary(){}; 

			private:
				QUuid _id;
				QUuid _instrumentId;
				QString _accountCode;
				QDecNumber _buyLot;
				QString _averageBuyPrice;
				QDecNumber _sellLot;
				QString _averageSellPrice;
				QDecNumber _totalBuyPirces;
				QDecNumber _totalSellPirces;

				QMap<QUuid, QSharedPointer<InstrumentSummaryPLFloat>> _instrumentSummaryPLFloats;

				int _decimals;
				bool _isShow;

			public:
				bool isClose(bool isBuy)
				{
					return isBuy ? _buyLot.toDouble()>0 : _sellLot.toDouble()>0;
				}

				void clearPLFloat()
				{
					foreach (auto item, _instrumentSummaryPLFloats)
					{
						item->clearPLFloat();
					}
				}
				
			public:				
				QUuid getId()
				{
					return _id;
				}

				QUuid getInstrumentId()
				{
					return _instrumentId;
				}

				QString getAccountCode()
				{
					return _accountCode;
				}

				QString getBuyLotFormat()
				{
					return _isShow ? Common::roundToString(_buyLot.toDouble(), 2) : "--";
				}

				QString getAverageBuyPrice()
				{
					return _isShow ? _averageBuyPrice : "--";
				}

				QString getSellLotFormat()
				{
					return _isShow ? Common::roundToString(_sellLot.toDouble(), 2) : "--";
				}

				QString getAverageSellPrice()
				{
					return _isShow ? _averageSellPrice : "--";
				}

				QString getBalanceLot()
				{
					return _isShow ? Common::roundToString((_buyLot - _sellLot).toDouble(), 2) : "--";
				}

				void setId(QUuid value)
				{
					_id = value;
				}

				void setInstrumentId(QUuid value)
				{
					_instrumentId = value;
				}
								
				void setAccountCode(QString value)
				{
					_accountCode = value;
				}
				
				void addBuyOrSell(bool isBuy, QDecNumber lot)
				{
					if(isBuy)
					{
						this->_buyLot += lot;
					}
					else
					{
						this->_sellLot  += lot;
					}
				}

				void addPirces(bool isBuy, QDecNumber price)
				{
					if (isBuy)
					{
						this->_totalBuyPirces += price;
					}
					else
					{
						this->_totalSellPirces += price;
					}
				}

				double getAvaragePirce(bool isBuy)
				{
					if (isBuy)
					{
						return this->_buyLot.isZero() ? 0.0 : (this->_totalBuyPirces / this->_buyLot).toDouble();
					}
					else
					{
						return this->_sellLot.isZero() ? 0.0 : (this->_totalSellPirces / this->_sellLot).toDouble();
					}
				}

				void setAverageBuyPrice(QString value)
				{
					_averageBuyPrice = value;
				}

				void setAverageSellPrice(QString value)
				{
					_averageSellPrice = value;
				}

				QMap<QUuid, QSharedPointer<InstrumentSummaryPLFloat>> getInstrumentSummaryPLFloats()
				{
					return _instrumentSummaryPLFloats;
				}
								
				void addPLFloat(QUuid currencyId, QDecNumber plfloat)
				{
					if(_instrumentSummaryPLFloats.contains(currencyId))
					{
						_instrumentSummaryPLFloats.value(currencyId)->addPLFloat(plfloat);
					}
				}
			};
		}
	}
}

#endif
