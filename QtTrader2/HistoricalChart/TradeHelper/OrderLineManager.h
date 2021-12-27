#pragma once
#include <QObject>
#include <QList>
#include <QAction>
#include "TradeHelper/OrderLine.h"
#include "QuickModifier.h"

namespace HistoricalChartSpace
{
	class HistoricalChart;
	class OrderLineManager
	{
	public:
		OrderLineManager(HistoricalChart* pView);
		~OrderLineManager();

		QuickModifier* getOrderModifier();
		void setQuickModifier(QuickModifier* modifier);
		void changeInstrument(const QUuid& instrumentId);
		void changeQuotePolicy(const QUuid& quotePolicyId);

		void addOrder(const OrderInfo& order);
		void removeOrder(const QUuid& orderId);
		void clearOrders();
		void showOrder(bool executed, bool show);
		void updateUI();

	private:
        QList<OrderLine*> m_orderLines;  // OrderLine* => Consider using QSharedPointer

		HistoricalChart* m_pView;
		QuickModifier* m_quickModifier;

		bool m_showExecutedOrder;
		bool m_showPendingOrder;
	};
}
