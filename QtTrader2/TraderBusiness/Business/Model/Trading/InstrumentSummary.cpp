#include "InstrumentSummary.h"
#include "TradingOrder.h"
#include "Transaction.h"
#include "InstrumentAccountSummary.h"
#include "Business/Model/Setting/BusinessInstrument.h"

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			InstrumentSummary::InstrumentSummary(QMap<QUuid, QSharedPointer<InstrumentSummaryPLFloat>> instrumentSummaryPLFloats)
			{
				this->_instrumentSummaryPLFloats = instrumentSummaryPLFloats;
				this->_buyLot = 0.0;
				this->_sellLot = 0.0;
			}

			InstrumentSummary::~InstrumentSummary()
			{
			}

			void InstrumentSummary::addPLFloat(QUuid currencyId, QDecNumber plfloat)
			{
				if(this->_instrumentSummaryPLFloats.contains(currencyId))
				{
					this->_instrumentSummaryPLFloats[currencyId]->addPLFloat(plfloat);
				}
			}

			void InstrumentSummary::clearPLFloat()
			{
				foreach (auto item, this->_instrumentSummaryPLFloats)
				{
					item->clearPLFloat();
				}
				if (this->_isMultipleAccount)
                {
					foreach (auto item, this->_instrumentAccountSummaries)
					{
						item->clearPLFloat();
					}
				}
			}

			void InstrumentSummary::UpdateByFloat(QSharedPointer<TradingOrder> order)
			{
				QUuid currencyId = order->getTransaction()->getInstrumentSummaryPLFloatCurrnecyId();
				if (this->_instrumentAccountSummaries.contains(order->getAccountId()))
                {
					QSharedPointer<InstrumentAccountSummary> instrumentAccountSummary = this->_instrumentAccountSummaries[order->getAccountId()];					
					if (!instrumentAccountSummary.isNull())
					{
						if (this->_isMultipleAccount)
						{
							instrumentAccountSummary->addPLFloat(currencyId, order->getTradePLFloat());
						}					
					}
				}
				this->addPLFloat(currencyId, order->getTradePLFloat());
			}

			void InstrumentSummary::UpdateBy(QSharedPointer<TradingOrder> order, QSharedPointer<Setting::BusinessInstrument> setttingInstrument, bool isAdded)
			{
				QSharedPointer<InstrumentAccountSummary> instrumentAccountSummary;
				this->_isShow = true; // !this._SettingsManager.Customer.IsNoShowAccountStatus;
				this->_decimals = setttingInstrument->getLotSetting(true, OrderType::SpotTrade)->getDecimals();
				if (this->_isMultipleAccount)
                {
					if (!this->_instrumentAccountSummaries.contains(order->getAccountId()))
                    {
						QMap<QUuid, QSharedPointer<InstrumentSummaryPLFloat>> instrumentSummaryPLFloats;						
						auto item = this->_instrumentSummaryPLFloats.constBegin();
						while (item != this->_instrumentSummaryPLFloats.constEnd()) {				
							instrumentSummaryPLFloats.insert(item.key(), item.value()->Clone());
							++item;
						}
                        QSharedPointer<InstrumentAccountSummary> newInstrumentAccountSummary(new InstrumentAccountSummary(this->_decimals, this->_isShow, instrumentSummaryPLFloats));
						instrumentAccountSummary = newInstrumentAccountSummary;
						instrumentAccountSummary->setId(order->getAccountId());
						instrumentAccountSummary->setInstrumentId(order->getInstrumentId());
						instrumentAccountSummary->setAccountCode(order->getTransaction()->getAccountCode());
						this->_instrumentAccountSummaries.insert(order->getAccountId(), newInstrumentAccountSummary);
                    }
					else
						instrumentAccountSummary = this->_instrumentAccountSummaries[order->getAccountId()];
                }

				int numeratorUnit = setttingInstrument->getCalculateSetting()->getNumeratorUnit();
                int denominator = setttingInstrument->getCalculateSetting()->getDenominator();
				QUuid currencyId = order->getTransaction()->getInstrumentSummaryPLFloatCurrnecyId();
				int decimal = order->getExecutePrice()._decimals;
				if (order->getIsBuy())
                {
					this->_totalBuyPirces += (order->getLotBalance() * order->getExecutePrice().toDecimal() * (isAdded ? 1 : -1));
					this->_buyLot += (order->getLotBalance() * (isAdded ? 1 : -1));
                    double avarage = this->_buyLot.isZero()? 0.0 : (this->_totalBuyPirces / this->_buyLot).toDouble();
                    this->_averageBuyPrice = Common::Price::normalize(avarage, numeratorUnit, denominator, decimal);
                    if (this->_isMultipleAccount)
                    {
						instrumentAccountSummary->addBuyOrSell(true, order->getLotBalance() * (isAdded ? 1 : -1));
						instrumentAccountSummary->addPirces(true, (order->getLotBalance() * order->getExecutePrice().toDecimal() * (isAdded ? 1 : -1)));
						double accountSummaryAvarage = instrumentAccountSummary->getAvaragePirce(true);
                        instrumentAccountSummary->setAverageBuyPrice(Common::Price::normalize(accountSummaryAvarage, numeratorUnit, denominator, decimal));
						instrumentAccountSummary->addPLFloat(currencyId, order->getTradePLFloat());
                    }					
                }
                else
                {
					this->_totalSellPirces += (order->getLotBalance() * order->getExecutePrice().toDecimal() * (isAdded ? 1 : -1));
                    this->_sellLot += (order->getLotBalance() * (isAdded ? 1 : -1));
                    double avarage = this->_sellLot.isZero()? 0.0 : (this->_totalSellPirces / this->_sellLot).toDouble();
                    this->_averageSellPrice = Common::Price::normalize(avarage, numeratorUnit, denominator, decimal);
                    if (this->_isMultipleAccount)
                    {
                        instrumentAccountSummary->addBuyOrSell(false, order->getLotBalance() * (isAdded ? 1 : -1));
						instrumentAccountSummary->addPirces(false, (order->getLotBalance() * order->getExecutePrice().toDecimal() * (isAdded ? 1 : -1)));
						double accountSummaryAvarage = instrumentAccountSummary->getAvaragePirce(false);
                        instrumentAccountSummary->setAverageSellPrice(Common::Price::normalize(accountSummaryAvarage, numeratorUnit, denominator, decimal));
						instrumentAccountSummary->addPLFloat(currencyId, order->getTradePLFloat());
                    }
                }
				this->addPLFloat(currencyId, order->getTradePLFloat());
				
			}
			
		}
	}
}
