#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class WilliamsAccumulationDistribution : public Indicator
	{
	public:
		WilliamsAccumulationDistribution(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~WilliamsAccumulationDistribution();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}