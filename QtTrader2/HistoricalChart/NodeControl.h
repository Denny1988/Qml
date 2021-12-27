#pragma once
#include "BaseDrawing.h"
namespace HistoricalChartSpace
{
	
	class NodeControl : public BaseDrawing
	{
	public:
		NodeControl(const DrawingObjectSettings& settings, ChartPanel* parentPanel);
		virtual ~NodeControl();

		void drawToPos(const QPointF& scenePosPoint);
		void updateIfNeed();

		bool handleMouseMove(QGraphicsItem* item, QGraphicsSceneMouseEvent *event);
		void handleMouseRelease(QGraphicsItem* item, QGraphicsSceneMouseEvent* event);
        // When new drawing elements appear, the following functions can be implemented as needed
		virtual void initDrawElement();
		virtual void setSelectProperty(bool flag);
		virtual void setAsBackgroundProperty(double i);
		virtual void setVisible(bool flag);
		virtual void setPen(const QPen& pen);
		virtual void setBrushToFill(const QColor& color);
	protected:
		bool nonCenterNodeMove(QGraphicsSceneMouseEvent *event, DrawingElementNode* node);
		bool centerNodeMove(QGraphicsSceneMouseEvent *event);
		void updatePosData(DrawingElementNode* node);
		virtual void updateCenterNode();
        virtual void drawLine() = 0;	// Virtual methods that must be implemented
	protected:
		QVector<DrawingElementNode*>		m_vecNode;
		DrawingElementNode*				m_centerNode;

	};
}
