#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class OnBalanceVolume : public Indicator
	{
	public:
		OnBalanceVolume(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~OnBalanceVolume();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}
