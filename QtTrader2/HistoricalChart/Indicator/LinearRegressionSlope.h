#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class LinearRegressionSlope : public Indicator
	{
	public:
		LinearRegressionSlope(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~LinearRegressionSlope();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}
