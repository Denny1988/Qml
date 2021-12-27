#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class VolumeROC : public Indicator
	{
	public:
		VolumeROC(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~VolumeROC();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}