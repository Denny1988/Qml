#pragma once
#include "ChartPanel.h"
namespace HistoricalChartSpace
{
	class YCoorPanel : public QGraphicsRectItem
	{
	public:
		YCoorPanel(ChartPanel *parent);
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
		QRectF boundingRect() const;
		QPainterPath shape() const;
	protected:
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
	private:
		ChartPanel*			m_chartPanel;
		QPointF				m_mouseStartPos;
	};
}