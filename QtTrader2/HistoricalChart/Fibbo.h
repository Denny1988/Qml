#pragma once
#include "DrawingTrend.h"

namespace HistoricalChartSpace
{
	class Fibbo : public DrawingTrend
	{
	public:
		Fibbo(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state);
		~Fibbo();
		void drawLine();
        void setLineAndTextIsVisible(bool isVisible);

	private:
		QList<QGraphicsLineItem*>			m_listLine;
		QList<QGraphicsSimpleTextItem*>		m_listText;
	};
}
