#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class FractalChaosOscillator : public Indicator
	{
	public:
		FractalChaosOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~FractalChaosOscillator();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}