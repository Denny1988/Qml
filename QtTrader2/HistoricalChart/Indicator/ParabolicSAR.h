#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class ParabolicSAR : public Indicator
	{
	public:
		ParabolicSAR(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~ParabolicSAR();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}