#include "MainPainterPanel.h"
#include "Cursors.h"
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QPixmap>
#include <QDebug>
#include "historicalchart.h"
#include "ToolBar.h"

namespace HistoricalChartSpace
{
	MainPainterPanel::MainPainterPanel(ChartPanel *parent) :QGraphicsRectItem(parent), m_parent(parent)
	{
		this->setFlags(ItemIsSelectable | ItemClipsChildrenToShape);
	}

	void MainPainterPanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
	{
		m_parent->paintMainPanel(painter, option, widget);
	}

	QRectF MainPainterPanel::boundingRect() const
	{
		QRectF rect;
		QRectF rectParent = m_parent->rect();
		double adjust = Helper::GetMarginSpaceValue(enMarginAdjustBound) * 2;
		double yCoorPanelWidth = Helper::GetMarginSpaceValue(enMarginYCoordinate);
		rect.setWidth(rectParent.width() - yCoorPanelWidth - adjust);
		rect.setHeight(rectParent.height() - adjust);
		return rect;
	}

	void MainPainterPanel::mousePressEvent(QGraphicsSceneMouseEvent *event)
	{
		//qDebug() << QString("MainPainterPanel::mouseMoveEvent pos:[%1,%2]").arg(event->pos().x()).arg(event->pos().y());
		if (event->type() == QEvent::GraphicsSceneMousePress)
		{
			if (event->button() == Qt::LeftButton)
			{
				m_dragStartPos = m_mouseStartPos = event->pos();
                qDebug() << " MainPainterPanel::mousePressEvent m_mouseStartPos:" << m_mouseStartPos;
				m_parent->handleMousePressEvent(event);
			}
			QGraphicsRectItem::mousePressEvent(event);

			if (event->button() == Qt::LeftButton)
			{
				if (!m_parent->getView()->getToolbar()->isInZoomIn())
				{
					// load adjust_horizontal cursor:
					QPixmap pixmap(adjust_horizontal);
					QCursor cursor(pixmap, 0, 0);
					this->setCursor(cursor);
				}
			}
		}
		else
		{
			QGraphicsRectItem::mousePressEvent(event);
		}
	}

	void MainPainterPanel::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
	{
		if (m_parent->getView()->getToolbar()->isInZoomIn())
		{
			m_parent->handleMouseReleaseEvent(event);
		}
		else
		{
			this->setCursor(Qt::ArrowCursor);
		}

		QGraphicsRectItem::mouseReleaseEvent(event);
	}

	void MainPainterPanel::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
	{
		if (event->buttons() & Qt::LeftButton)
		{
			if (m_parent->getView()->getToolbar()->isInZoomIn())
			{
				m_parent->handleMouseMoveEvent(event);
			}
			else
			{
				int pixels = event->pos().x() - m_mouseStartPos.x();
				if (pixels != 0)
				{
					m_parent->moveChart(pixels);
					m_mouseStartPos = event->pos();
				}
			}
		}
	}

	void MainPainterPanel::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
	{
		m_parent->handleHoverMoveEvent(event);
	}
	void MainPainterPanel::wheelEvent(QGraphicsSceneWheelEvent *event)
	{
		m_parent->handleWheelEvent(event);
	}

	QPainterPath MainPainterPanel::shape() const
	{
		QPainterPath path;
		path.addRect(boundingRect());
		return path;
	}
	QPointF MainPainterPanel::dragStartPos() const
	{
		return m_dragStartPos;
	}
	void MainPainterPanel::showZoomInCoursor()
	{
		// load adjust_horizontal cursor:
		//QPixmap pixmap(zoomIn_cursor);
		//QPixmap pixmap("d:/temp/aa.png");
		QPixmap pixmap("Resources/ChartImages/zoomin_cussor.png");
		QCursor cursor(pixmap, 0, 0);
		this->setCursor(cursor);
	}
	void MainPainterPanel::showNormalCoursor()
	{
		this->setCursor(Qt::ArrowCursor);
	}
}
