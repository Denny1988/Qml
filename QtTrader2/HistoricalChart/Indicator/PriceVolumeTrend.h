#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class PriceVolumeTrend : public Indicator
	{
	public:
		PriceVolumeTrend(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~PriceVolumeTrend();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}
