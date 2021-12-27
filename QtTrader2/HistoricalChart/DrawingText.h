#pragma once
#include "BaseDrawing.h"
#include <QTextEdit>

namespace HistoricalChartSpace
{
	class DrawingText : public BaseDrawing, public QObject
	{
	public:
		DrawingText(const DrawingObjectSettings& settings, ChartPanel* parent, DrawStage state);
		virtual ~DrawingText();

		virtual void drawToPos(const QPointF& scenePosPoint) override;
		virtual void updateIfNeed() override;
		virtual void setVisible(bool flag) override;
		virtual void setPen(const QPen& pen) override;
		virtual void setSelectProperty(bool flag) override;
		virtual void setAsBackgroundProperty(double i) override;

	private:
		void draw(QPointF& pos);

		virtual bool handleMouseMove(QGraphicsItem* item, QGraphicsSceneMouseEvent *event) override;
		virtual bool handleMouseDoubleClick(QGraphicsItem* item, QGraphicsSceneMouseEvent* event) override;
		virtual bool eventFilter(QObject *, QEvent *) override;

	private:
		DrawingElementNode* m_positionNode;
		DrawingElementText* m_textElement;
		RectShape* m_rectShape;
		QTextEdit* m_editor;
	};
}