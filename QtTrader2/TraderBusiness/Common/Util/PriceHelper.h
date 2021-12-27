#ifndef PRICEHELPER_H
#define PRICEHELPER_H
#include <QString>
#include "Common/Price.h"
#include "BusinessManager.h"


namespace Common
{
    class PriceHelper
    {
    public:
        static Price toPrice(QString value, QSharedPointer<Business::Model::Setting::BusinessInstrument> settingInstrument);
        static Price toPrice(QString value, int numeratorUnit, int denominator);
       /* static Price toPrice(double value, QSharedPointer<Business::Model::Setting::BusinessInstrument> settingInstrument)
        {
            return Price::createInstance(value, settingInstrument->getCalculateSetting()->getNumeratorUnit(), settingInstrument->getCalculateSetting()->getDenominator());
        }

        static Price toPrice(double value,  int numeratorUnit, int denominator)
        {
            return Price::createInstance(value,  numeratorUnit, denominator);
        }*/
    };
}
#endif
