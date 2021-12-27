#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class TrueRange : public Indicator
	{
	public:
		TrueRange(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~TrueRange();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}