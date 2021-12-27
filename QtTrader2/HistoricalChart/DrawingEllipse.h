#pragma once
#include "NodeControl.h"
namespace HistoricalChartSpace
{
	class DrawingEllipse : public NodeControl
	{
	public:
		DrawingEllipse(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state);
		~DrawingEllipse();
		
		void drawLine();
		void initDrawElement();
		void setVisible(bool flag);
		void setPen(const QPen& pen);
		void setAsBackgroundProperty(double i);
		void setBrushToFill(const QColor& color); 
		void updateCenterNode();
	private:
		EllipseShape*			m_ellipseLine;	 
	};
}