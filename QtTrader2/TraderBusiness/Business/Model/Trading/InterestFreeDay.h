#ifndef BUSINESS_MODEL_TRADING_INTERESTFREEDAY_H
#define BUSINESS_MODEL_TRADING_INTERESTFREEDAY_H
#include <QString>

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			class InterestFreeDay
			{
			private:
				QString  _code;
				QString  _tradeDay;
				QString  _lotBalance;
				QString  _executePrice;
				QString  _interestFreeDays;
				QString  _interestRate;
			
			public:

				QString getCode() const
				{
					return _code;
				}

				void setCode(QString value)
				{
					_code = value;
				}

				QString getTradeDay() const
				{
					return _tradeDay;
				}

				void setTradeDay(QString value)
				{
					_tradeDay = value;
				}

				QString getLotBalance() const
				{
					return _lotBalance;
				}

				void setLotBalance(QString value)
				{
					_lotBalance = value;
				}

				QString getExecutePrice() const
				{
					return _executePrice;
				}

				void setExecutePrice(QString value)
				{
					_executePrice = value;
				}

				QString getInterestFreeDays() const
				{
					return _interestFreeDays;
				}

				void setInterestFreeDays(QString value)
				{
					_interestFreeDays = value;
				}

				QString getInterestRate() const
				{
					return _interestRate;
				}

				void setInterestRate(QString value)
				{
					_interestRate = value;
				}
			};
		}
	}
}
#endif
