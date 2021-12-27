#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class WilliamsR : public Indicator
	{
	public:
		WilliamsR(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~WilliamsR();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}