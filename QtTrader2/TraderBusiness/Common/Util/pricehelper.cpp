#include "PriceHelper.h"
#include "QDecNumber.h"

namespace Common
{
    Price PriceHelper::toPrice(QString value, QSharedPointer<Business::Model::Setting::BusinessInstrument> settingInstrument)
    {
        int numeratorUnit = settingInstrument->getCalculateSetting()->getNumeratorUnit();
        if (settingInstrument->isExistPricingPolicy())
        {
            QDecNumber basePrice = Common::roundToDecimal(value.toDouble(), Common::getDecimal(value));
            auto pricingPolicyDetail = settingInstrument->getPricingPolicyDetail(basePrice);
            if (!pricingPolicyDetail.isNull())
            {
                numeratorUnit = pricingPolicyDetail->getNumerator();
            }
        }
        return Price::createInstance(value, numeratorUnit, settingInstrument->getCalculateSetting()->getDenominator());
    }

    Price PriceHelper::toPrice(QString value, int numeratorUnit, int denominator)
    {
        return Price::createInstance(value, numeratorUnit, denominator);
    }

   /* static Price toPrice(double value, QSharedPointer<Business::Model::Setting::BusinessInstrument> settingInstrument)
    {
        return Price::createInstance(value, settingInstrument->getCalculateSetting()->getNumeratorUnit(), settingInstrument->getCalculateSetting()->getDenominator());
    }

    static Price toPrice(double value,  int numeratorUnit, int denominator)
    {
        return Price::createInstance(value,  numeratorUnit, denominator);
    }*/
}
