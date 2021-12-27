#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class TradeVolumeIndex : public Indicator
	{
	public:
		TradeVolumeIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~TradeVolumeIndex();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}
