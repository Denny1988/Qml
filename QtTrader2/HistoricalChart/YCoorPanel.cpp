#include "YCoorPanel.h"
#include "PricePanel.h"
#include <QGraphicsSceneMouseEvent>
namespace HistoricalChartSpace
{
	YCoorPanel::YCoorPanel(ChartPanel *parent) :QGraphicsRectItem(parent), m_chartPanel(parent)
	{
		this->setFlags(ItemIsSelectable | ItemClipsChildrenToShape);
	}

	void YCoorPanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
    {
		m_chartPanel->paintYCoorPanel(painter, option, widget);
	}

	QRectF YCoorPanel::boundingRect() const
	{
		QRectF rect;
		QRectF rectParent  = m_chartPanel->rect();
		double adjust = Helper::GetMarginSpaceValue(enMarginAdjustBound) * 2;
		double wide =  Helper::GetMarginSpaceValue(enMarginYCoordinate);
		rect.setWidth(wide - adjust);
		rect.setHeight(rectParent.height() -adjust);
		return rect;
	}

	QPainterPath YCoorPanel::shape() const
	{
		QPainterPath path;
		path.addRect(boundingRect());
		return path;
	}

	void YCoorPanel::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
	{
        //#if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
		if (event->buttons() == Qt::LeftButton && m_chartPanel->getPanelType() == enPanelPrice)
		{
			double moveRatio = (event->pos().y() - m_mouseStartPos.y()) / m_chartPanel->rect().height();
			PricePanel* pricePanel = dynamic_cast<PricePanel*>(m_chartPanel);
			pricePanel->adjustYScale(moveRatio);
			m_mouseStartPos = event->pos();
		}
        //#endif
	}

	void YCoorPanel::mousePressEvent(QGraphicsSceneMouseEvent *event)
	{
       //#if defined (RUNTIME_OS_WIN32) || defined (RUNTIME_OS_MACX)
		if (event->buttons() == Qt::LeftButton && m_chartPanel->getPanelType() == enPanelPrice)
		{
			m_mouseStartPos = event->pos();
		}
		return QGraphicsRectItem::mousePressEvent(event);
        //#endif
	}
}
