#ifndef BUSINESS_MODEL_COMPUTING_OVERRIDEDQUOTATION_H
#define BUSINESS_MODEL_COMPUTING_OVERRIDEDQUOTATION_H
#include <quuid.h>
#include "DemandComputing.h"
#include "ComputingResult.h"
#include "Business/Model/Setting/BusinessAccount.h"
#include "Business/Model/Setting/CurrencyRate.h"
#include "Business/Model/Trading/TradingOrder.h"

namespace Business
{
	namespace Model
	{
		namespace Computing
		{
			class OverridedQuotation : public DemandComputing
			{
			public:
				int _numeratorUnit;
				int _denominator;
				bool _caculateChangeWithDenominator;
				QUuid _quotePolicyId;
				QUuid _instrumentId;
                PriceType::PriceType _priceType;

			protected:
				QSharedPointer<DemandComputingResult> doCompute();

			private:
				QSharedPointer<OverridedQuotationComputingResult> innerCompute();
			};
		}
	}
}
#endif
