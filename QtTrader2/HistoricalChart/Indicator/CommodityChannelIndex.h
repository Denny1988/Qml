#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class CommodityChannelIndex : public Indicator
	{
	public:
		CommodityChannelIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~CommodityChannelIndex();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}