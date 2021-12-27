#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class TypicalPrice : public Indicator
	{
	public:
		TypicalPrice(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~TypicalPrice();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}