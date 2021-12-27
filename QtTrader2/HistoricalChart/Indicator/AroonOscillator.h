#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class AroonOscillator : public Indicator
	{
	public:
		AroonOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~AroonOscillator();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}