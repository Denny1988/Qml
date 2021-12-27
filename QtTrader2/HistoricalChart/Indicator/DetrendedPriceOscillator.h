#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class DetrendedPriceOscillator : public Indicator
	{
	public:
		DetrendedPriceOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~DetrendedPriceOscillator();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}