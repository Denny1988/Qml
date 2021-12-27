#pragma once
#include "Indicator.h"
namespace HistoricalChartSpace
{
	class AccumulativeSwingIndex : public Indicator
	{
	public:
		AccumulativeSwingIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~AccumulativeSwingIndex();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}