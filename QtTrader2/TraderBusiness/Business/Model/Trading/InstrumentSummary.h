#ifndef BUSINESS_MODEL_TRADING_INSTRUMENTSUMMARY_H
#define BUSINESS_MODEL_TRADING_INSTRUMENTSUMMARY_H

#include "QDecNumber.h"
#include <QUuid>
#include <QSharedPointer>
#include <QMap>
#include "Common/Util/Convert_Common.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class BusinessInstrument;
		}
		namespace Trading
		{
			class TradingOrder;
			class InstrumentAccountSummary;
			class InstrumentSummaryPLFloat;
			class InstrumentSummary
			{				
			public:
				InstrumentSummary(QMap<QUuid, QSharedPointer<InstrumentSummaryPLFloat>> instrumentSummaryPLFloats);
				~InstrumentSummary(); 

			private:
				bool _isMultipleAccount;
				QUuid _id;
				QUuid _accountId;
				QString _description;
				QDecNumber _buyLot;
				QString _averageBuyPrice;
				QDecNumber _sellLot;
				QString _averageSellPrice;
				QString _balanceLot;
				QMap<QUuid, QSharedPointer<InstrumentAccountSummary>> _instrumentAccountSummaries;
				QDecNumber _totalBuyPirces;
				QDecNumber _totalSellPirces;
				QMap<QUuid, QSharedPointer<InstrumentSummaryPLFloat>> _instrumentSummaryPLFloats;

				int _decimals;
				bool _isShow;

			public:
				void addPLFloat(QUuid currencyId, QDecNumber plfloat);
				void clearPLFloat();
				void UpdateBy(QSharedPointer<TradingOrder> order, QSharedPointer<Setting::BusinessInstrument> setttingInstrument, bool isAdded);
				void UpdateByFloat(QSharedPointer<TradingOrder> order);
				QMap<QUuid, QSharedPointer<InstrumentAccountSummary>> getInstrumentAccountSummaries() const
				{
					return _instrumentAccountSummaries;
				}

				QSharedPointer<InstrumentAccountSummary> getInstrumentAccountSummary(QUuid id)
				{
					return _instrumentAccountSummaries.value(id);
				}
								
				bool isClose(bool isBuy)
				{
					if(this->_instrumentAccountSummaries.count()>1) return false;
					return isBuy ? _buyLot.toDouble()>0 : _sellLot.toDouble()>0;
				}

			public:
				bool getIsMultipleAccount()
				{
					return _isMultipleAccount;
				}

				QUuid getId()
				{
					return _id;
				}

				QUuid getAccountId()
				{
					return _accountId;
				}

				QString getDescription()
				{
					return _description;
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

				void setIsMultipleAccount(bool value)
				{
					_isMultipleAccount = value;
				}

				void setId(QUuid value)
				{
					_id = value;
				}

				void setAccountId(QUuid value)
				{
					_accountId = value;
				}

				void setDescription(QString value)
				{
					_description = value;
				}
							
				QString getBalanceLot()
				{
					return _isShow ? Common::roundToString((_buyLot - _sellLot).toDouble(), 2) : "--";
				}

				QMap<QUuid, QSharedPointer<InstrumentSummaryPLFloat>> getInstrumentSummaryPLFloats()
				{
					return _instrumentSummaryPLFloats;
				}

			};
		}
	}
}

#endif
