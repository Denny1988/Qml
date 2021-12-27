#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class HighMinusLow : public Indicator
	{
	public:
		HighMinusLow(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~HighMinusLow();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}