#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class RelativeStrengthIndex : public Indicator
	{
	public:
		RelativeStrengthIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~RelativeStrengthIndex();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}
