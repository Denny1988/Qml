#pragma once
#include <QGraphicsItem>
namespace HistoricalChartSpace
{
	class CursorPosition;
	class HistoricalChart;
	class Crosshair : public QGraphicsItem
	{
	public:
		Crosshair(HistoricalChart* pView, CursorPosition* cursorPosition);
		QRectF boundingRect() const;
		QPainterPath shape() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);

	private:
		CursorPosition* m_cursorPosition;
		HistoricalChart* m_pView;
	};
}