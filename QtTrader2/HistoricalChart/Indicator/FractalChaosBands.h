#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class FractalChaosBands : public Indicator
	{
	public:
		FractalChaosBands(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~FractalChaosBands();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}