#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class VIDYA : public Indicator
	{
	public:
		VIDYA(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~VIDYA();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}