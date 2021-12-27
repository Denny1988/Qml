#include "DrawingTrend.h"
#include "ChartPanel.h"
#include <QCursor>

namespace HistoricalChartSpace
{
	DrawingTrend::DrawingTrend(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state)
		: NodeControl(settings, parent), m_lineElement(NULL)
	{
		initDrawElement();
		//m_state = DrawLineFinished;
		m_stage = state;
		updateSettings();
	}

	DrawingTrend::~DrawingTrend()
	{
		delete m_lineElement;
	}

	void DrawingTrend::drawLine()
	{
        //_ASSERT(m_vecNode.size() >= 2);
		QPointF beginPos = m_vecNode[0]->pos();
		QPointF endPos = m_vecNode[1]->pos();
		this->computeRayPos(beginPos, endPos);
		m_lineElement->setLine(QLineF(beginPos, endPos));
	}

	void DrawingTrend::initDrawElement()
	{
		NodeControl::initDrawElement();
		m_lineElement = new DrawingElementLine(this);
		m_lineElement->setCursor(Qt::ArrowCursor);
		m_lineElement->setFlag(QGraphicsItem::ItemIsSelectable,false);
	}

	void DrawingTrend::setAsBackgroundProperty(double i)
	{
		NodeControl::setAsBackgroundProperty(i);
		m_lineElement->setZValue(i);
	}

	void DrawingTrend::setVisible(bool flag)
	{
		NodeControl::setVisible(flag);
		m_lineElement->setVisible(flag);
	}

	void DrawingTrend::setPen(const QPen& pen)
	{
		NodeControl::setPen(pen);
		m_lineElement->setPen(pen);
	}

	void DrawingTrend::computeRayPos(QPointF& beginPos, QPointF& endPos)
	{
		bool rayLeft = m_settings.m_intRayFlags & RayShow_LeftSelected;
		bool rayRight = m_settings.m_intRayFlags & RayShow_RightSelected;

		if (rayLeft || rayRight)
		{
            // List the linear equation y = kx + b
			double k = 0.0;
			double b = beginPos.y();
			QRectF rect = m_parentPanel->getMainPainterPanel()->boundingRect();
			if (endPos.x() != beginPos.x())
			{
				k = (beginPos.y() - endPos.y()) / (beginPos.x() - endPos.x());
				b = beginPos.y() - k * beginPos.x();
			}

			if (rayLeft)
			{
				double xLeft = rect.left();
				if (beginPos.x() > endPos.x())
				{
					xLeft = rect.left() + rect.width();
				}
				double yLeft = k * xLeft + b;
				beginPos.setX(xLeft);
				beginPos.setY(yLeft);
			}

			if (rayRight)
			{
				double xRight = rect.left() + rect.width();
				if (beginPos.x() > endPos.x())
				{
					xRight = rect.left();
				}
				double yRight = k * xRight + b;
				endPos.setX(xRight);
				endPos.setY(yRight);
			}
		}
	}
}
