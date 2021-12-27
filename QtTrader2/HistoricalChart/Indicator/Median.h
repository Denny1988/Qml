#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class Median : public Indicator
	{
	public:
		Median(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~Median();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}