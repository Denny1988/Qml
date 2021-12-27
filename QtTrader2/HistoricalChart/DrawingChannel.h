#pragma once
#include "DrawingTrend.h"

namespace HistoricalChartSpace
{
	class DrawingChannel : public DrawingTrend
	{
	public:
		DrawingChannel(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state);
		~DrawingChannel();

		virtual void drawLine() override;
		virtual void updateCenterNode() override;
		virtual void hindToPos(const QPointF& scenePosPoint) override;
        void setSecondLineIsVisible(bool isVisible);

	private:
		QGraphicsLineItem* m_secondLine;
	};
}
