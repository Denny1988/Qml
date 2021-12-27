#ifndef COMMON_PRICE_H
#define COMMON_PRICE_H
#include <QString>
#include "Util/Convert_Common.h"
#include <math.h>
//#include "stdafx.h"
#include "QDecNumber.h"

namespace Common
{
	class Price
	{
	public:
		Price(){ }

		Price(QString normalizedPrice, double normalizedValue, int numeratorUnit, int denominator, int decimals)
		{
			if (normalizedPrice.isNull() || normalizedPrice.isEmpty())
			{
				normalizedValue = 0.0;
			}
			this->_normalizedPrice = normalizedPrice;
			this->_normalizedValue = normalizedValue;
			this->_normalizedDecimalValue = normalizedValue;
			this->_numeratorUnit = numeratorUnit;
			this->_denominator = denominator;
			this->_decimals = decimals;
		}

		Price(QString price, int numeratorUnit, int denominator, int decimals)
		{
			this->_decimals = decimals;
			double normalizedValue;
			QString normalizedPrice = Price::normalize(price, numeratorUnit, denominator, decimals, normalizedValue);
			if (normalizedPrice.isNull() || normalizedPrice.isEmpty())
			{
				QString info("Cannot create price object: price=" + price + ";numeratorUnit=" + numeratorUnit + ";denominator=" + denominator);
				//throw new QException(info);
				normalizedValue = 0.0;
			}
			this->_normalizedPrice = normalizedPrice;
			this->_normalizedValue = normalizedValue;
			this->_normalizedDecimalValue = normalizedValue;
			this->_numeratorUnit = numeratorUnit;
			this->_denominator = denominator;
			
		}

		Price(double price, int numeratorUnit, int denominator, int decimals)
		{		
			this->_decimals = decimals;
			double normalizedValue;
			QString normalizedPrice = Price::normalize(QString::number(price, '.', decimals), numeratorUnit, denominator, decimals, normalizedValue);
			if (normalizedPrice.isNull() || normalizedPrice.isEmpty())
			{
				QString info("Cannot create price object: price=" + QString::number(price) + ";numeratorUnit=" + numeratorUnit + ";denominator=" + denominator);
				//throw new ArgumentException(info);
				normalizedValue = 0.0;
			}
			this->_normalizedPrice = normalizedPrice;
			this->_normalizedValue = normalizedValue;
			this->_normalizedDecimalValue = normalizedValue;
			this->_numeratorUnit = numeratorUnit;
			this->_denominator = denominator;
			
		}

		Price(double price, int numeratorUnit, int denominator)
		{		
			this->_decimals = (int)log10((double)denominator);
			double normalizedValue;
			QString normalizedPrice = Price::normalize(QString::number(price), numeratorUnit, denominator, this->_decimals, normalizedValue);
			if (normalizedPrice.isNull() || normalizedPrice.isEmpty())
			{
				QString info("Cannot create price object: price=" + QString::number(price) + ";numeratorUnit=" + numeratorUnit + ";denominator=" + denominator);
				//throw new ArgumentException(info);
				normalizedValue = 0.0;
			}
			this->_normalizedPrice = normalizedPrice;
			this->_normalizedValue = normalizedValue;
			this->_normalizedDecimalValue = normalizedValue;
			this->_numeratorUnit = numeratorUnit;
			this->_denominator = denominator;
		}

	private:
		static const int _MaxPrecision = 28;
		static const int _MaxScale = 13;
		static const int _MaxDenominator = 1000000000;

	public:
		QString _normalizedPrice;
		double _normalizedValue;
		QDecNumber _normalizedDecimalValue;
		int _numeratorUnit;
		int _denominator;
		int _decimals;


	public:
		int compareTo(Price* price)
		{
			if(this->_normalizedValue < price->_normalizedValue)
				return -1;
			else if(this->_normalizedValue == price->_normalizedValue)
				return 0;
			else
				return 1;
		}

		QString toQString()
		{
			return this->_normalizedPrice;
		}

		bool isNullPrice()
		{
			return this->_normalizedPrice.isEmpty() || this->_normalizedPrice.isNull() || this->_normalizedPrice == "";
		}

		double toDouble()
		{
			return this->_normalizedValue;
		}

		QDecNumber toDecimal()
		{
			return this->_normalizedDecimalValue;
		}

		static Price createInstance(QString price, int numeratorUnit, int denominator)
		{
			double normalizedValue;
			int decimals = (int)log10((double)denominator);
			QString normalizedPrice = Price::normalize(price, numeratorUnit, denominator, decimals, normalizedValue);			
			return Price(normalizedPrice, normalizedValue, numeratorUnit, denominator, decimals);
		}

		static Price createInstance(double price, int numeratorUnit, int denominator, int decimals)
		{
			return Price(QString::number(price, '.', decimals), numeratorUnit, denominator, decimals);
		}

		static void update(Price basePrice, QString price, int numeratorUnit, int denominator)
		{
			double normalizedValue;
			QString normalizedPrice = Price::normalize(price, numeratorUnit, denominator, basePrice._decimals, normalizedValue);
			basePrice._normalizedPrice = normalizedPrice;
			basePrice._normalizedValue = normalizedValue;
			basePrice._numeratorUnit = numeratorUnit;
			basePrice._denominator = denominator;
		}

		static void update(Price basePrice, double price, int numeratorUnit, int denominator)
		{
			return Price::update(basePrice, QString::number(price), numeratorUnit, denominator);
		}

		static int compare(Price* price1, Price* price2, bool isReciprocal)
		{
			if (Q_NULLPTR==price1 && Q_NULLPTR==price2)
			{
				return 0;
			}
			else if (Q_NULLPTR!=price1)
			{
				return price1->compareTo(price2) * (isReciprocal ? -1 : 1);
			}
			else
			{
				return price2->compareTo(price1) * (isReciprocal ? 1 : -1);
			}			
		}

		static Price avg(Price price1, Price price2)
		{
			if (price1._numeratorUnit != price2._numeratorUnit || price1._denominator != price2._denominator)
			{
				//throw new ArgumentException("Prices is not the same type");
			}

			return Price::createInstance((price1._normalizedValue + price2._normalizedValue) / 2, price1._numeratorUnit, price1._denominator, price1._decimals);
		}

		static QString normalize(double price, int numeratorUnit, int denominator, int decimals)
		{
			return Price::normalize(QString::number(price), numeratorUnit, denominator, decimals, price);
		}

		static QString normalize(QString price, int numeratorUnit, int denominator, int decimals, double& normalizedValue)
		{   
			QDecNumber normalizedPrice;
			if (!Price::tryParse(price, numeratorUnit, denominator, decimals,  normalizedPrice))
			{
				return "";
			}
			else
			{
				normalizedValue = normalizedPrice.toDouble();
				return Price::toQString(normalizedValue, numeratorUnit, denominator);
			}
		}

		static bool isValid(int numeratorUnit, int denominator)
		{
			if (numeratorUnit <= 0)
			{
				return false;
			}

			if (denominator <= 0 || denominator > Price::_MaxDenominator)
			{
				return false;
			}

			/*if (denominator > 1 && numeratorUnit >= denominator)
			{
				return false;
			}*/
			return true;
		}

		static bool tryParse(QString priceString, int numeratorUnit, int denominator, int decimals, QDecNumber& normalizedPrice)
		{
			if (priceString.isNull() || priceString.isEmpty() || !Price::isValid(numeratorUnit, denominator))
			{
				return false;
			}

			/* QLocale qLocale(Common::getLanguagebyQString(g_Languages.GetCurrentLanguage().m_qtLanguageEnum), Common::getCountrybyQString(g_Languages.GetCurrentLanguage().m_qtCountryEnum));
			if (qLocale.decimalPoint() != '.')
			{
			priceString = priceString.replace(qLocale.decimalPoint(), ".");
			}*/

			QDecNumber priceValue = Common::roundToDecimal(priceString.toDouble(), decimals);
			QDecNumber numeratorUnitDec = Common::roundToDecimal((double)numeratorUnit, decimals);
			QDecNumber denominatorDec = Common::roundToDecimal((double)denominator, decimals);
			if (denominator == 1)
			{
				//normalize value to multiple of numeratorUnit
				normalizedPrice = (Common::roundToDecimal(priceValue / numeratorUnitDec, 0)) * numeratorUnitDec;
			}
			else if (numeratorUnit > denominator)
			{
				QDecNumber unit = (QDecNumber)numeratorUnit / (QDecNumber)denominator;
				normalizedPrice = Common::roundToDecimal(priceValue / unit, 0) * unit;
			}
			else
			{
//				int integral = priceString.toInt();
//				QDecNumber integralPart = integral;
//				QDecNumber numerator = (priceValue - integralPart) * denominatorDec;
//				//normalize numerator to multiple of numeratorUnit
//				QDecNumber normalizedNumerator = Common::roundToDecimal(numerator / numeratorUnitDec, 0) * numeratorUnitDec;
//				normalizedPrice = integralPart + normalizedNumerator / denominatorDec;

                QDecNumber numerator = priceValue * denominatorDec;
                //normalize numerator to multiple of numeratorUnit
                QDecNumber normalizedNumerator = Common::roundToDecimal(numerator / numeratorUnitDec, 0) * numeratorUnitDec;
                normalizedPrice = normalizedNumerator / denominatorDec;
			}

			normalizedPrice = Common::roundToDecimal(normalizedPrice, decimals);

			return true;// normalizedPrice > 0;
		}

		static QString toQString(double normalizedValue, int numeratorUnit, int denominator)
		{
			QString priceString;

			if (denominator == 1)
			{
				priceString = Common::roundToString(normalizedValue, 0);
			}
			else
			{
				int integralPart = (int)normalizedValue;
				int numerator = abs((int)Common::round((normalizedValue - integralPart) * denominator));

				double decimalDigits = log10((double)denominator);
				if (decimalDigits == (uint)decimalDigits)
				{
					//10,100,1000
					QDecNumber numerator2 = (double)numerator / denominator;
					QString  numerator2Str = Common::roundToString(numerator2.toDouble(),(int)decimalDigits);
					priceString.append(QString::number(integralPart));
					priceString.append(numerator2Str.mid(1));
				}
				else
				{
					if (numerator == 0)
					{
						priceString = integralPart;
					}
					else
					{
						/*QLocale qLocale(Common::getLanguagebyQString(g_Languages.GetCurrentLanguage().m_qtLanguageEnum), Common::getCountrybyQString(g_Languages.GetCurrentLanguage().m_qtCountryEnum));	
						priceString = integralPart + qLocale.decimalPoint() + numerator + "/" + denominator;*/
					}
				}
			}

			if (normalizedValue < 0 && priceString[0] != '-') priceString = "-" + priceString;
			return priceString;
		}  

	};

	/*inline Price operator *(Price price1, int points)
	{
		double price = price1._normalizedValue * ((double)points / price1._denominator);
		return Price::createInstance(price, price1._numeratorUnit, price1._denominator, price1._decimals);
	}*/

	inline Price operator *(Price price1, double price2)
	{
		double price = price1._normalizedValue * price2;
		return Price::createInstance(price, price1._numeratorUnit, price1._denominator, price1._decimals);
	}

	//inline Price operator *(Price price1, Price price2)
	//{
	//	if (price1._numeratorUnit != price2._numeratorUnit || price1._denominator != price2._denominator)
	//	{
	//		//throw new ArgumentException("Prices is not the same type");
	//	}

	//	return Price(price1._normalizedValue * price2._normalizedValue, price1._numeratorUnit, price1._denominator, price1._decimals);
	//}


	inline Price operator +(Price price1, int points)
	{
        double price = price1._normalizedValue + (double)points / price1._denominator;
		return Price::createInstance(price, price1._numeratorUnit, price1._denominator, price1._decimals);
	}

	inline Price operator +(Price price1, double price2)
	{
		double price = price1._normalizedValue + price2;
		return Price(price, price1._numeratorUnit, price1._denominator, price1._decimals);
	}

	inline Price operator +(Price price1, Price price2)
	{
		if (price1._numeratorUnit != price2._numeratorUnit || price1._denominator != price2._denominator)
		{
			//throw new ArgumentException("Prices is not the same type");
		}

		return Price(price1._normalizedValue + price2._normalizedValue, price1._numeratorUnit, price1._denominator, price1._decimals);
	}

	inline Price operator -(Price price1, int points)
	{			
		return (price1 + (int)(0 - points));
	}

	inline Price operator -(Price price1, double price2)
	{
		double price = price1._normalizedValue - price2;
		return Price(price, price1._numeratorUnit, price1._denominator, price1._decimals);
	}

	inline int operator -(Price price1, Price price2)
	{
		if (price1._numeratorUnit != price2._numeratorUnit || price1._denominator != price2._denominator)
		{
			//throw new ArgumentException("Prices is not the same type");
		}

		return (int)Common::round((price1._normalizedValue - price2._normalizedValue) * price1._denominator);
	}

	inline bool operator ==(Price price1, Price price2)
	{
		return price1._normalizedValue == price2._normalizedValue;
	}

	inline bool operator !=(Price price1, Price price2)
	{
		return !(price1 == price2);
	}

	inline bool operator >(Price price1, Price price2)
	{
		return price1._normalizedValue > price2._normalizedValue;
	}

	inline bool operator >=(Price price1, Price price2)
	{
		return price1._normalizedValue >= price2._normalizedValue;
	}

	inline bool operator <(Price price1, Price price2)
	{
		return price1._normalizedValue < price2._normalizedValue;
	}

	inline bool operator <=(Price price1, Price price2)
	{
		return price1._normalizedValue <= price2._normalizedValue;
	}
}

#endif

