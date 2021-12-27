#pragma once
#include <QPen>
#include <QLineF>
#include <QAction>
#include <QGraphicsLineItem>
#include "PriceAlertAdapter.h"

namespace HistoricalChartSpace
{
	class PricePanel;
	class AlertHintLine;
	class AlertLine : public QGraphicsLineItem
	{
	public:
		AlertLine(PricePanel* pricePanel, PriceAlertAdapter* manager, const PriceAlertStruct& priceAlert);
		~AlertLine();

		void updateParameter(const PriceAlertStruct& priceAlert);
		void ensureInView();

	protected:
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
		virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

	private:
		void hide();
		void prepareLine();
		void placeLine();

	private:
		QPen m_pen;
		QLineF m_line;
		int m_lineLength;
		PriceAlertStruct m_priceAlert;
		PricePanel* m_pricePanel;
		PriceAlertAdapter* m_manager;
		AlertHintLine* m_hintLine;
		QAction* m_modifyAlertAction;
		QAction* m_deleteAlertAction;
	};
}