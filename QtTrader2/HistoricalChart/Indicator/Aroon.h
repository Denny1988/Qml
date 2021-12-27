#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class Aroon : public Indicator
	{
	public:
		Aroon(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~Aroon();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}