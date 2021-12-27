#include "DrawingVertical.h"
#include "historicalchart.h"
#include "ChartPanel.h"
#include <QGraphicsSceneMouseEvent>
#include "JulianDate.h"
namespace HistoricalChartSpace
{
	DrawingVertical::DrawingVertical(const DrawingObjectSettings& settings, ChartPanel* parentPanel)
		: BaseDrawing(parentPanel, settings),
		m_centerNode(NULL),
		m_lineElement(NULL),
		m_textRect(NULL)
	{
		m_lineGlobal = new QGraphicsLineItem(m_parentPanel->getView()->getXCoordinate());
		if (parentPanel != NULL)
		{
			initDrawElement();
			m_stage = DrawFinished;
			updateSettings();
		}
	}

	DrawingVertical::~DrawingVertical()
	{
		if(m_centerNode != NULL) delete m_centerNode;
		if(m_lineElement != NULL) delete m_lineElement;
		if(m_textRect != NULL) delete m_textRect;
		if(m_lineGlobal != NULL) delete m_lineGlobal;
	}

	void DrawingVertical::drawToPos(const QPointF& scenePosPoint)
	{
		QPointF localPoint = m_parentPanel->getMainPainterPanel()->mapFromScene(scenePosPoint);
        //_ASSERT(m_settings.m_vecDate.size() == 1);
		m_settings.m_vecDate[0] = m_parentPanel->Xpos2Time(localPoint);
		updateSettings();
		drawLine(localPoint.x());
	}

	void DrawingVertical::updateIfNeed()
	{
		if(m_stage != DrawFinished) return;
		if(m_settings.m_vecDate.size() != 1) return;
		drawLine(m_parentPanel->Time2Xpos(m_settings.m_vecDate[0]));
	}

	bool DrawingVertical::handleMouseMove(QGraphicsItem* item, QGraphicsSceneMouseEvent *event)
	{
		DrawingElementLine* line = dynamic_cast<DrawingElementLine*>(item);
		if (line == NULL || line != m_lineElement) return false;

		QPointF point = event->pos();
		QRectF rect = m_parentPanel->getMainPainterPanel()->boundingRect();
		double margin = Helper::GetMarginSpaceValue(enMarginDrawingObjectToBound);
		double xMax = rect.left() + rect.width() - margin;
		double xCurrent = point.x();

		if (point.x() < rect.left()) point.setX(rect.left());
		if (point.x() > xMax) point.setX(xMax);
        // Prevent m_settings from being accidentally modified
		if (m_settings.m_vecDate.size() != 1)  return false;
		m_settings.m_vecDate[0] = m_parentPanel->Xpos2Time(point);

		drawLine(point.x());
		return true;
	}

	void DrawingVertical::initDrawElement()
	{
		m_lineElement = new DrawingElementLine(this);
		m_centerNode = new DrawingElementNode(this);
		m_textRect = new DrawingElementText(this, m_parentPanel->getView()->getXCoordinate());

		//const double width = Helper::GetMarginSpaceValue(enMarginXTextWSpace);
		//const double hight = Helper::GetMarginSpaceValue(enMarginXTextHSpace);

		//m_textRect->setRect(0, 0, width, hight);
		m_centerNode->setZValue(-1);
	}

	void DrawingVertical::setSelectProperty(bool flag)
	{
		m_lineElement->setFlag(QGraphicsItem::ItemIsSelectable,flag);
	}
	void DrawingVertical::setAsBackgroundProperty(double i)
	{
		m_lineElement->setZValue(i);
	}
	void DrawingVertical::setVisible(bool flag)
	{
		m_lineElement->setVisible(flag);
		m_centerNode->setVisible(flag);
		m_textRect->setVisible(flag);
		m_isVisible = flag;
	}
	void DrawingVertical::setPen(const QPen& pen)
	{
		m_lineElement->setPen(pen);
		m_centerNode->setPen(pen);
		m_textRect->setColor(m_settings.m_colorLine, m_parentPanel->getView()->getConfig().getCommonSettings().m_background);
	}

	void DrawingVertical::drawLine(double x)
	{
		if (!m_isVisible) return;

        // Determine whether it is in the display area
		QRectF rect = m_parentPanel->getMainPainterPanel()->boundingRect();
		if( x < rect.left() || x > rect.left() + rect.width())
		{
			m_lineElement->setVisible(false);
			m_lineGlobal->setVisible(false);
			m_centerNode->setVisible(false);
			m_textRect->setVisible(false);
			return;
		}
		else
		{
			m_lineElement->setVisible(true);
			m_centerNode->setVisible(true);
		}

        // Draw area lines
		double top = rect.top();
		double bottom = top + m_parentPanel->rect().height();
		m_lineElement->setLine(x,top,x,bottom);
		m_centerNode->setPos(x,(top + bottom) / 2);

        // Does it have diffraction properties
		if(m_settings.m_intRayFlags & RayShow_SingleSelected)
		{
            // Draw coordinate lines
			double margin = Helper::GetMarginSpaceValue(enMarginOutGridStandardSpace);
			double xCoordinateHigh = Helper::GetMarginSpaceValue(enMarginXCoordinate); 
			//const double textWidth = Helper::GetMarginSpaceValue(enMarginXTextWSpace);

			QRectF globalRect = m_parentPanel->getView()->getViewPortRect();
			double globalGraphHigh = globalRect.top() + globalRect.height() - xCoordinateHigh; 

			m_lineGlobal->setPen(m_lineElement->pen());
			m_lineGlobal->setLine(x,globalRect.top(),x,globalGraphHigh);
			m_lineGlobal->setVisible(true);
			m_lineGlobal->setZValue(-1.0);

			if(m_settings.m_vecDate.size() != 1) return;
			
			QString strDate = JulianDate::FromJDateToString(m_settings.m_vecDate[0]);
			m_textRect->setText(strDate);
			m_textRect->setVisible(true);

			const double textWidth = m_textRect->getTextWidth();
			double textLeft = x - textWidth / 2;
			textLeft = textLeft > 0 ? textLeft : 0;
			m_textRect->setPos(textLeft, globalGraphHigh + margin);
			m_lineGlobal->setVisible(true);
			m_textRect->setVisible(true);
		}
		else
		{
			m_textRect->setVisible(false);
			m_lineGlobal->setVisible(false);
		}
	}
}
