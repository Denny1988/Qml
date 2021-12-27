#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class EaseOfMovement : public Indicator
	{
	public:
		EaseOfMovement(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~EaseOfMovement();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}
