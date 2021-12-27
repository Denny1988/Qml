#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class PriceROC : public Indicator
	{
	public:
		PriceROC(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~PriceROC();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}