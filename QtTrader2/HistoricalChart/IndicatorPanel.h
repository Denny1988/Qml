#pragma  once
#include "ChartPanel.h"
#include "Settings.h"
#include <QHash>
#include <QPolygonF>


namespace HistoricalChartSpace
{
	class HistoricalChart;
	class IndicatorPanel : public ChartPanel
	{
		Q_OBJECT
	public:

		IndicatorPanel(HistoricalChart* pView, Config& config, BaseData& baseData);
		IndicatorPanel(HistoricalChart* pView, Config& config, BaseData& baseData, Indicator* indicator);
        virtual ~IndicatorPanel() override;

		//void setRange(int beginIndex, int endIndex, double barDistance);
		//void updateBarDistance(double barDistance);
		//void redrawAll();
        //const QString getPosStrInfo(const QPointF& scenePoint);
		virtual void refresh() override;

	private:
		void paintMainPanel(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
		void paintYCoorPanel(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);

	protected:
		double m_bottomMargin;
	};
}
