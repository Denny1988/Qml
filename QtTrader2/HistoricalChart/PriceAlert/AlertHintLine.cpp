#include "AlertHintLine.h"
#include "Cursors.h"
#include <QCursor>
#include <QStyleOptionGraphicsItem>
#include "PricePanel.h"
#include "historicalchart.h"

namespace HistoricalChartSpace
{
	AlertHintLine::AlertHintLine(PricePanel* pricePanel, const QPen& pen, int lineLength)
		:QGraphicsLineItem(pricePanel->getMainPainterPanel()),
		m_pricePanel(pricePanel),
		m_pen(pen),
		m_lineLength(lineLength)
	{
		QGraphicsLineItem::setPen(m_pen);

		// cursor
		QPixmap pixmap(adjust_vertical);
		QCursor cursor(pixmap, 0, 0);
		QGraphicsItem::setCursor(cursor);
	}

	AlertHintLine::~AlertHintLine()
	{
	}

	void AlertHintLine::moveTo(double y)
	{
		QRectF rect = m_pricePanel->getMainPainterPanel()->boundingRect();
		double x2 = rect.left() + rect.width();
		double x1 = x2 - m_lineLength;
		m_line.setLine(x1, y, x2, y);
		QGraphicsLineItem::setLine(m_line);
		m_pricePanel->getMainPainterPanel()->update();
	}

	void AlertHintLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
	{
		/* draw line */
		QGraphicsLineItem::paint(painter, option, widget);

		/* draw arrow */
		painter->drawPixmap(m_line.x2() - 4, m_line.y2() - 3, QPixmap(right_arrow));
	}

}