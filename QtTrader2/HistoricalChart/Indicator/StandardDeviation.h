#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class StandardDeviation : public Indicator
	{
	public:
		StandardDeviation(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~StandardDeviation();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}