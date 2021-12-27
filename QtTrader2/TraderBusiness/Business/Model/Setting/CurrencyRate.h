#ifndef BUSINESS_MODEL_SETTING_CURRENCYRATE_H
#define BUSINESS_MODEL_SETTING_CURRENCYRATE_H
#include <quuid.h>
#include "Common/Util/Convert_Common.h"
#include "QDecNumber.h"
#include "Currency.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class CurrencyRate
			{
			private:
				QDecNumber  _rateIn;
				QDecNumber  _rateOut;
				QSharedPointer<Currency> _sourceCurrency;
				QSharedPointer<Currency> _targetCurrency;

			public:
				QUuid getSourceCurrencyId() const
				{
					return this->_sourceCurrency->getId();
				}
				
				QUuid getTargetCurrencyId() const
				{
					return this->_targetCurrency->getId();
				}
				

				QDecNumber getRateIn() const
				{
					return _rateIn;
				}

				void setRateIn(QDecNumber value)
				{
					_rateIn = value;
				}
				
				QDecNumber getRateOut() const
				{
					return _rateOut;
				}

				void setRateOut(QDecNumber value)
				{
					_rateOut = value;
				}

				QSharedPointer<Currency> getSourceCurrency() const
				{
					return _sourceCurrency;
				}

				void setSourceCurrency(QSharedPointer<Currency> value)
				{
					_sourceCurrency = value;
				}

				QSharedPointer<Currency> getTargetCurrency() const
				{
					return _targetCurrency;
				}

				void setTargetCurrency(QSharedPointer<Currency> value)
				{
					_targetCurrency = value;
				}


				QDecNumber exchange(QDecNumber amount)
				{
					QDecNumber newAmount = Common::roundToDecimal(Common::roundToDecimal(amount, this->_sourceCurrency->getDecimals()) * (amount.isNegative() ? this->_rateOut : this->_rateIn), this->_targetCurrency->getDecimals());
					//double a = newAmount.toDouble();
					return newAmount;
				}

				QDecNumber exchange(QDecNumber amount, bool isUseRateout)
				{
					QDecNumber newAmount = Common::roundToDecimal(Common::roundToDecimal(amount, this->_sourceCurrency->getDecimals()) * (isUseRateout ? this->_rateOut : this->_rateIn), this->_targetCurrency->getDecimals());
					//double a = newAmount.toDouble();
					return newAmount;
				}

				QDecNumber exchangeByRateOut(QDecNumber amount)
				{
					QDecNumber newAmount = Common::roundToDecimal(Common::roundToDecimal(amount, this->_sourceCurrency->getDecimals()) * this->_rateOut, this->_targetCurrency->getDecimals());
					//double a = newAmount.toDouble();
					return newAmount;
				}

                QDecNumber exchangeByRateIn(QDecNumber amount)
                {
                    QDecNumber newAmount = Common::roundToDecimal(Common::roundToDecimal(amount, this->_sourceCurrency->getDecimals()) * this->_rateIn, this->_targetCurrency->getDecimals());
                    //double a = newAmount.toDouble();
                    return newAmount;
                }
			};
		}
	}
}
#endif
