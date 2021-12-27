#include "DrawingTriangle.h"
#include "ChartPanel.h"
namespace HistoricalChartSpace
{
	DrawingTriangle::DrawingTriangle(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state) : NodeControl(settings, parent), m_triangleItem(NULL)
	{
		initDrawElement();
		m_stage = state;
		updateSettings();
	}

	DrawingTriangle::~DrawingTriangle()
	{
		if(m_triangleItem != NULL)
			delete m_triangleItem;
	}

	void DrawingTriangle::drawLine()
	{
		if(m_vecNode.size() != 3) return;

		const QPointF& beginPos = m_vecNode[0]->pos();
		const QPointF& endPos = m_vecNode[1]->pos();
		const QPointF& onPos = m_vecNode[2]->pos();
		const QPointF& center = m_centerNode->pos();
		
		 QPolygonF polygon;
		 polygon << beginPos - center << endPos - center << onPos - center;
		 m_triangleItem->setPolygon(polygon);
		 m_triangleItem->setPos(center);
	}
	void DrawingTriangle::initDrawElement()
	{
		NodeControl::initDrawElement();
		m_triangleItem = new PolygonShape(m_parentPanel->getMainPainterPanel());
	}
	void DrawingTriangle::setVisible(bool flag)
	{
		NodeControl::setVisible(flag);
		m_triangleItem->setVisible(flag);
	}
	void DrawingTriangle::setPen(const QPen& pen)
	{
		NodeControl::setPen(pen);
		m_triangleItem->setPen(pen);
	}
	
	void DrawingTriangle::setAsBackgroundProperty(double i)
	{
		NodeControl::setAsBackgroundProperty(i);
		m_triangleItem->setZValue(i);
	}
	void DrawingTriangle::setBrushToFill(const QColor& color)
	{
		NodeControl::setBrushToFill(color);
		m_triangleItem->setBrush(color);
		m_triangleItem->setFillRule(Qt::OddEvenFill);
	}
}