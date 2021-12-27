#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class BollingerBands : public Indicator
	{
	public:
		BollingerBands(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~BollingerBands();
        virtual void getParameterRange(const QString& key, double& min, double& max, double& decimals) override;

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
	};
}
