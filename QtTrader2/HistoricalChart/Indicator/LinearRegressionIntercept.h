#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class LinearRegressionIntercept : public Indicator
	{
	public:
		LinearRegressionIntercept(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~LinearRegressionIntercept();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}