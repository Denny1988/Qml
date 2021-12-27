#include "DrawingRect.h"
#include "ChartPanel.h"


namespace HistoricalChartSpace
{
	DrawingRect::DrawingRect(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state) : NodeControl(settings, parent), m_rectItem(NULL)
	{
		initDrawElement();
		m_stage = state;
		updateSettings();
	}

	DrawingRect::~DrawingRect()
	{
		if(m_rectItem != NULL)
			delete m_rectItem;
	}

	void DrawingRect::drawLine()
	{
		if(m_vecNode.size() != 2) return;

		const QPointF& beginPos = m_vecNode[0]->pos();
		const QPointF& endPos = m_vecNode[1]->pos();
	
		
		QPointF diff = endPos - beginPos;
		
		m_rectItem->setRect(0,0,diff.x(),diff.y());
		m_rectItem->setPos(beginPos);
	}

	void DrawingRect::initDrawElement()
	{
		NodeControl::initDrawElement();
		m_rectItem = new RectShape(m_parentPanel->getMainPainterPanel());
	}

	void DrawingRect::setAsBackgroundProperty(double i)
	{
		NodeControl::setAsBackgroundProperty(i);
		m_rectItem->setZValue(i);
	}
	void DrawingRect::setVisible(bool flag)
	{
		NodeControl::setVisible(flag);
		m_rectItem->setVisible(flag);
	}
	void DrawingRect::setPen(const QPen& pen)
	{
		NodeControl::setPen(pen);
		m_rectItem->setPen(pen);
	}

	void DrawingRect::setBrushToFill(const QColor& color)
	{
		NodeControl::setBrushToFill(color);
		
		m_rectItem->setBrush(color);

	}
}