#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class PositiveVolumeIndex : public Indicator
	{
	public:
		PositiveVolumeIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~PositiveVolumeIndex();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}
