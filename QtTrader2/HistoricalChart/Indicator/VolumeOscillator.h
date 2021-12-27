#pragma once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class VolumeOscillator : public Indicator
	{
	public:
		VolumeOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~VolumeOscillator();

		virtual void getParameterRange(const QString& key, int& min, int& max) override;

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}