#pragma  once
#include "Indicator.h"

namespace HistoricalChartSpace
{
	class MovingAverages : public Indicator
	{
	public:
		MovingAverages(IndicatorSettings& settings, BaseData& baseData, bool needInitialize);
		~MovingAverages();

		// Inherited via Indicator
		virtual const QString& getName() override;

	protected:
		IndicatorSettings& initSettings(IndicatorSettings& settings) const;

	private:
		virtual void calculate(int beginIndex, int endIndex) override;
		QString m_lineName;
	};
}