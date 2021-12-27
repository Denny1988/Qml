#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class HighLowBands : public Indicator
	{
	public:
		HighLowBands(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~HighLowBands();
        void getParameterRange(const QString& key, int& min, int& max);

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;

	};
}
