#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class MassIndex : public Indicator
	{
	public:
		MassIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~MassIndex();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}
