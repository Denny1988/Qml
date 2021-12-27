#pragma once
#include "NodeControl.h"
namespace HistoricalChartSpace
{
	class DrawingRect : public NodeControl
	{
	public:
		DrawingRect(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state);
		~DrawingRect();
		void drawLine();
		void initDrawElement();
		void setAsBackgroundProperty(double i);
		void setVisible(bool flag);
		void setPen(const QPen& pen);
		void setBrushToFill(const QColor& color);
	private:
		RectShape*		m_rectItem;
	};
}
