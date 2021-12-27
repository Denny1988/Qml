#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class ChaikinMoneyFlow : public Indicator
	{
	public:
		ChaikinMoneyFlow(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~ChaikinMoneyFlow();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}