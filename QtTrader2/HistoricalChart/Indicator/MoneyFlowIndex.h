#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class MoneyFlowIndex : public Indicator
	{
	public:
		MoneyFlowIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~MoneyFlowIndex();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}