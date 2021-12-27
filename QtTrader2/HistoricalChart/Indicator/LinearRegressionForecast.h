#pragma once
#include "Indicator.h"
namespace HistoricalChartSpace
{
	class LinearRegressionForecast : public Indicator
	{
	public:
		LinearRegressionForecast(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~LinearRegressionForecast();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}