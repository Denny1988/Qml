#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class WellesWilderSmoothing : public Indicator
	{
	public:
		WellesWilderSmoothing(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~WellesWilderSmoothing();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}