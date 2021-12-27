#pragma once
#include "NodeControl.h"
namespace HistoricalChartSpace
{
	class DrawingTriangle : public NodeControl
	{
	public:
		DrawingTriangle(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state);
		~DrawingTriangle();

		void drawLine();
		void initDrawElement();
		void setVisible(bool flag);
		void setPen(const QPen& pen);
		void setAsBackgroundProperty(double i);
		void setBrushToFill(const QColor& color); 
	private:
		PolygonShape*			m_triangleItem;
	};
}