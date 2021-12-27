#include "HintLine.h"
#include "Cursors.h"
#include <QCursor>
#include <QStyleOptionGraphicsItem>
#include "PricePanel.h"
#include "historicalchart.h"

namespace HistoricalChartSpace
{
	HintLine::HintLine(PricePanel* pricePanel, QColor lineColor)
		:QGraphicsLineItem(pricePanel->getMainPainterPanel()), m_pricePanel(pricePanel)
	{
		m_pen.setColor(lineColor);
		m_pen.setStyle(Qt::DashLine);
		QGraphicsLineItem::setPen(m_pen);

		// cursor
		QPixmap pixmap(adjust_vertical);
		QCursor cursor(pixmap, 0, 0);
		QGraphicsItem::setCursor(cursor);
	}

	HintLine::~HintLine()
	{
	}

	void HintLine::moveTo(double y, double price, QString& text, bool isStopLoss /*= false*/)
	{
		m_y = y;
		m_price = price;
		m_text = text;
		m_isStopLoss = isStopLoss;

		QRectF rect = m_pricePanel->rect();
		m_line.setLine(rect.left(), m_y, rect.left() + rect.width(), m_y);
		QGraphicsLineItem::setLine(m_line);

		if (QGraphicsItem::parentItem() == nullptr)
		{
			setParentItem(m_pricePanel->getMainPainterPanel());
		}
		m_pricePanel->getMainPainterPanel()->update();
	}

	bool HintLine::isShowOnChart()
	{
		return QGraphicsItem::parentItem() != nullptr;
	}

	void HintLine::hide()
	{
		m_pricePanel->getView()->getScene()->removeItem(this);
	}

	void HintLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
	{
		/* draw line */
		QGraphicsLineItem::paint(painter, option, widget);

		/* draw text */
		// 1.set pen.
		painter->setPen(m_pricePanel->getForeground());

		// 2.set position
		QRectF rect = option->exposedRect;
		double textHeight = 15.0;
		rect.setTop(rect.top() - textHeight);
		rect.setHeight(textHeight);

		// 3. draw text
		painter->drawText(rect, m_text);
	}

}
