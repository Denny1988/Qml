#pragma  once
#include <QGraphicsRectItem>
#include "Settings.h"
#include "Config.h"


namespace HistoricalChartSpace
{
	class ChartPanel;
	class SplitPanel : public QGraphicsRectItem
	{
	public:
		SplitPanel(Config& config);
		~SplitPanel();
		void setPrePanel(ChartPanel* panel);
		void setNextPanel(ChartPanel* panel);
		ChartPanel* getPrePanel() const;
		ChartPanel* getNextPanel() const;
	protected:
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		
	private:
		Config&			m_config;
		const CommonSettings&	m_commonSettings;
		ChartPanel*				m_prePanel;
		ChartPanel*				m_nextPanel;
		QPointF					m_mouseStartPos;
	};
}