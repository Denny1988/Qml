#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class MomentumOscillator : public Indicator
	{
	public:
		MomentumOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~MomentumOscillator();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}
