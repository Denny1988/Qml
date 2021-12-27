#pragma once
#include <QPen>
#include <QLineF>
#include <QAction>
#include <QGraphicsLineItem>
#include "QuickModifier.h"
#include "HintLine.h"

namespace HistoricalChartSpace
{
	class PricePanel;
	class OrderLineManager;
	class OrderLine : public QGraphicsLineItem
	{
	public:
		OrderLine(PricePanel* pricePanel, OrderLineManager* orderLineManager, OrderInfo order, bool show);
		~OrderLine();

		const QUuid& getOrderId() const;
		bool isExecutedOrder() const;
		void updateUI();

	protected:
		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
		virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

	private:
		void OnDragExecutedOrderLine(QuickModifier* quickModifier, double y, double price);
		void OnDragPendingOrderLine(QuickModifier* quickModifier, double y, double price);
		void prepareLine();
		void setPenColor();

	private:
		QPen m_pen;
		QLineF m_line;
		OrderInfo m_order;
		PricePanel* m_pricePanel;
		OrderLineManager* m_orderLineManager;
		HintLine* m_hintLine;
		QAction* m_closeOrderAction;
		QAction* m_cancelOrderAction;
		QAction* m_modifyOrderAction;
	};
}