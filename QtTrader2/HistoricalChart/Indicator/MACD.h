#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class MACD : public Indicator
	{
	public:
		MACD(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~MACD();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}