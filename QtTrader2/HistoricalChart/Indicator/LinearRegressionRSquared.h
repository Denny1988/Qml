#pragma once
#include "Indicator.h"
namespace HistoricalChartSpace
{
	class LinearRegressionRSquared : public Indicator
	{
	public:
		LinearRegressionRSquared(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~LinearRegressionRSquared();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}