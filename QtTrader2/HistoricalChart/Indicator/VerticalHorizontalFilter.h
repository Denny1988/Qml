#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class VerticalHorizontalFilter : public Indicator
	{
	public:
		VerticalHorizontalFilter(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~VerticalHorizontalFilter();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}