#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class ChaikinVolatility : public Indicator
	{
	public:
		ChaikinVolatility(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~ChaikinVolatility();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}