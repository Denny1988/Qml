#pragma once
#include "BaseDrawing.h"
namespace HistoricalChartSpace
{
	class DrawingHorizontal : public BaseDrawing
	{
	public:
		DrawingHorizontal(const DrawingObjectSettings& settings, ChartPanel* parentPanel = NULL);
		~DrawingHorizontal();
		void drawToPos(const QPointF& scenePosPoint);
		void updateIfNeed();
		bool handleMouseMove(QGraphicsItem* item,QGraphicsSceneMouseEvent *event);
		void initDrawElement();
		void setSelectProperty(bool flag);
	    void setAsBackgroundProperty(double i);
		void setVisible(bool flag);
		void setPen(const QPen& pen);
		
	private:
		void drawLine(double y);
		
	private:
		DrawingElementLine*	m_lineElement;
		DrawingElementText*	m_textRect;
	};
}