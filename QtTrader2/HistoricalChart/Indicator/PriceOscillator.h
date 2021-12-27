#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class PriceOscillator : public Indicator
	{
	public:
		PriceOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~PriceOscillator();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}