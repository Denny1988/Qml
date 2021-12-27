#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class RainbowOscillator : public Indicator
	{
	public:
		RainbowOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~RainbowOscillator();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}