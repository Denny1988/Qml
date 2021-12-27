#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class PerformanceIndex : public Indicator
	{
	public:
		PerformanceIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~PerformanceIndex();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}