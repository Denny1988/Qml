#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class UltimateOscillator : public Indicator
	{
	public:
		UltimateOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~UltimateOscillator();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}