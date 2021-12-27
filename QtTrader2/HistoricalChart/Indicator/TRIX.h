#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class TRIX : public Indicator
	{
	public:
		TRIX(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~TRIX();

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}
