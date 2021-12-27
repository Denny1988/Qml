#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class StochasticOscillator : public Indicator
	{
	public:
		StochasticOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~StochasticOscillator();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}