#pragma once
#include "NodeControl.h"

//Note: The trend line can also be used as the base class of other control lines, such as Fibbo
namespace HistoricalChartSpace
{
	class DrawingTrend : public NodeControl
	{
	public:
		DrawingTrend(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state);
		virtual ~DrawingTrend();
		virtual void drawLine();
		void initDrawElement();
		void setAsBackgroundProperty(double i);
		void setVisible(bool flag);
		void setPen(const QPen& pen);
	
	protected:
		void computeRayPos(QPointF& beginPos, QPointF& endPos);
	
	protected:
		DrawingElementLine*		m_lineElement;
	};
}
