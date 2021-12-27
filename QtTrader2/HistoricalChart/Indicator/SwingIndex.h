#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class SwingIndex : public Indicator
	{
	public:
		SwingIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~SwingIndex();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}