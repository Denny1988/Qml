#pragma once
#include <QString>
#include <QUuid>
#include <QList>
#include <QMap>
#include <QSharedPointer>
#include "historicalchart_global.h"

namespace HistoricalChartSpace
{
	struct OrderInfo
	{
		QUuid m_instrumentId;
		QUuid m_orderId;
        QUuid m_quotePolicyId;  // Used to display only the orders with the same quotePolicyId on the graph
		QString m_accountCode;
		QString m_orderCode;
		bool m_isBuy;
        bool m_isOpen;        // Used to display information about pending orders
        bool m_isExecuted;   // Whether to trade, used to judge whether it is a pending order or a traded order
		double m_lots;
        double m_price;       // The pending order is SetPrice, and the transaction order is ExecutePrice.

		bool operator==(const OrderInfo &other) const
		{
			return this->m_orderId == other.m_orderId;
		}

		bool operator!=(const OrderInfo &orig) const
		{
			return !(*this == orig);
		}
	};

	class HistoricalChart;
	class OrderLineManager;
	class HISTORICALCHART_EXPORT QuickModifier
	{
		friend class OrderLineManager;
	public:
		QuickModifier();
		virtual ~QuickModifier();

        // The following are the methods that need to be implemented on the Trader side:
		// Whether the pending order can be modified, include Limit and stop order.
		virtual bool canModify(QUuid orderID, double newPrice) = 0;
		// modify pending Limit and stop order.
		virtual void modify(QUuid orderID, double newPrice) = 0;
		// modify pending or open order, show modify window.
		virtual void showModifyWindow(QUuid orderID) = 0;
		// close order
		virtual void closeOrder(QUuid orderID) = 0;
		// cancel pending order
		virtual void cancelPendingOrder(QUuid orderID) = 0;

		// Whether stop loss order can be placed.
		virtual bool canSetStopLoss(QUuid openOrderID, double price) = 0;
		// place stop loss order
		virtual void setStopLoss(QUuid openOrderID, double price) = 0;

		// Whether take profit order can be placed.
		virtual bool canSetTakeProfit(QUuid openOrderID, double price) = 0;
		// place take profit order
		virtual void setTakeProfit(QUuid openOrderID, double price) = 0;

        // Methods to be called on the Trader side:
		void addOrder(OrderInfo order);
		void removeOrder(QUuid instrumentId, QUuid orderId);
		void clearOrder();

	private:
        // Called by OrderManager
		void appendChart(HistoricalChart* pChart);
		void removeChart(HistoricalChart* pChart);
		void refreshOrderListFor(HistoricalChart* pChart);

	private:
		// order map: InstrumentId - orders
		QMap<QUuid, QSharedPointer<QList<OrderInfo>>> m_orders;
		QList<HistoricalChart*> m_pCharts;
	};
}
