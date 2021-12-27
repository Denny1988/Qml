#pragma once
#include "BaseDrawing.h"
namespace HistoricalChartSpace
{
	class DrawingVertical : public BaseDrawing
	{
	public:
		DrawingVertical(const DrawingObjectSettings& settings, ChartPanel* parentPanel = NULL);
		~DrawingVertical();
		void drawToPos(const QPointF& scenePosPoint);
		void updateIfNeed();
		bool handleMouseMove(QGraphicsItem* item,QGraphicsSceneMouseEvent *event);
		void initDrawElement();
		void setSelectProperty(bool flag);
		void setAsBackgroundProperty(double i);
		void setVisible(bool flag);
		void setPen(const QPen& pen);
	private:
		void drawLine(double x);

	private:
        DrawingElementLine*	m_lineElement;	// Vertical line
        DrawingElementNode*	m_centerNode;	// Mark point
        DrawingElementText*	m_textRect;		// Text box
        QGraphicsLineItem*	m_lineGlobal;	// Global line
	};

}
