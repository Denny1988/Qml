#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class MovingAverageEnvelope : public Indicator
	{
	public:
		MovingAverageEnvelope(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~MovingAverageEnvelope();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}