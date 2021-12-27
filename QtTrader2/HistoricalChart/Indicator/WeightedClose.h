#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class WeightedClose : public Indicator
	{
	public:
		WeightedClose(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~WeightedClose();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}