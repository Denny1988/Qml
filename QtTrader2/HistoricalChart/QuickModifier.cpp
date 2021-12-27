#include "QuickModifier.h"
#include "historicalchart.h"

namespace HistoricalChartSpace
{
	QuickModifier::QuickModifier()
	{
	}

	QuickModifier::~QuickModifier()
	{
	}

	void QuickModifier::addOrder(OrderInfo order)
	{
		QSharedPointer<QList<OrderInfo>> orders;
		if (m_orders.contains(order.m_instrumentId))
		{
			orders = m_orders[order.m_instrumentId];
		}
		else
		{
			orders = QSharedPointer<QList<OrderInfo>>(new QList<OrderInfo>());
			m_orders[order.m_instrumentId] = orders;
		}
		orders->append(order);

		// notify chart with order.m_instrumentId
        foreach (HistoricalChart* pChart, m_pCharts)
		{
			if (QUuid(pChart->getInstrument().getKey()) == order.m_instrumentId && pChart->getQuotePolicyId() == order.m_quotePolicyId)
			{
				pChart->addOrder(order);
			}
		}
	}

	void QuickModifier::removeOrder(QUuid instrumentId, QUuid orderId)
	{
		if (m_orders.contains(instrumentId))
		{
			QSharedPointer<QList<OrderInfo>> orders = m_orders[instrumentId];
            foreach (OrderInfo order, *orders.data())
			{
				if (order.m_orderId == orderId)
				{
					orders->removeOne(order);
                    foreach (HistoricalChart* pChart, m_pCharts)
					{
						if (QUuid(pChart->getInstrument().getKey()) == order.m_instrumentId)
						{
							pChart->removeOrder(order.m_orderId);
						}
					}
					break;
				}
			}
		}
	}

	void QuickModifier::clearOrder()
	{
		m_orders.clear();
	}

	void QuickModifier::appendChart(HistoricalChart* pChart)
	{
		m_pCharts.push_back(pChart);
		this->refreshOrderListFor(pChart);
	}

	void QuickModifier::removeChart(HistoricalChart* pChart)
	{
		m_pCharts.removeOne(pChart);
	}

	void QuickModifier::refreshOrderListFor(HistoricalChart* pChart)
	{
		pChart->clearOrders();
		QUuid instrumnetId = pChart->getInstrument().getKey();
		if (m_orders.contains(instrumnetId))
		{
			QSharedPointer<QList<OrderInfo>> orders = m_orders[instrumnetId];
			for (int i = 0; i < orders->size(); i++)
			{
				if (orders->at(i).m_quotePolicyId == pChart->getQuotePolicyId())
				{
					pChart->addOrder(orders->at(i));
				}
			}
		}
	}
}
