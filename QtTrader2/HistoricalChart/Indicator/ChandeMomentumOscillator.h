#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class ChandeMomentumOscillator : public Indicator
	{
	public:
		ChandeMomentumOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~ChandeMomentumOscillator();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}