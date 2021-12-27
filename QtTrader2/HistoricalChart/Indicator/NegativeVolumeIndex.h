#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class NegativeVolumeIndex : public Indicator
	{
	public:
		NegativeVolumeIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~NegativeVolumeIndex();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}
