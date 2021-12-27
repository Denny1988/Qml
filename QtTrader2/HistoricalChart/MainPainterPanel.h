#pragma once
#include "ChartPanel.h"
namespace HistoricalChartSpace
{
	class MainPainterPanel : public QGraphicsRectItem
	{
	public:
		MainPainterPanel(ChartPanel *parent);
		QRectF boundingRect() const;
		QPainterPath shape() const;
		QPointF dragStartPos() const;
		void showZoomInCoursor();
		void showNormalCoursor();

	protected:
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

		virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
		virtual void wheelEvent(QGraphicsSceneWheelEvent *event);

	private:
		ChartPanel*	m_parent;
		QPointF m_dragStartPos;
		QPointF m_mouseStartPos;
	};
}