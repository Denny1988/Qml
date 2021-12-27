#include "OrderLineManager.h"
#include "PricePanel.h"
#include "../historicalchart.h"
#include <QGraphicsScene>

namespace HistoricalChartSpace
{
	OrderLineManager::OrderLineManager(HistoricalChart* pView) : m_pView(pView),
		m_quickModifier(nullptr),
		m_showExecutedOrder(false),
		m_showPendingOrder(false)
	{
	}

	OrderLineManager::~OrderLineManager()
	{
		if (m_quickModifier) m_quickModifier->removeChart(m_pView);
	}

	QuickModifier* OrderLineManager::getOrderModifier()
	{
		return m_quickModifier;
	}

	void OrderLineManager::setQuickModifier(QuickModifier* modifier)
	{
		m_quickModifier = modifier;
		m_quickModifier->appendChart(m_pView);
	}

	void OrderLineManager::changeInstrument(const QUuid& instrumentId)
	{
		if (m_quickModifier) m_quickModifier->refreshOrderListFor(m_pView);
	}

	void OrderLineManager::changeQuotePolicy(const QUuid& quotePolicyId)
	{
		if (m_quickModifier) m_quickModifier->refreshOrderListFor(m_pView);
	}

	void OrderLineManager::addOrder(const OrderInfo& order)
	{
		bool isExists = false;
		for (int index = m_orderLines.size() - 1; index >= 0; index--)
		{
			if (m_orderLines[index]->getOrderId() == order.m_orderId)
			{
				isExists = true;
				break;
			}
		}
		if (!isExists)
		{
			bool show = order.m_isExecuted && m_showExecutedOrder || !order.m_isExecuted && m_showPendingOrder;
			OrderLine* orderLine = new OrderLine(m_pView->getPricePanel(), this, order, show);
			m_orderLines.append(orderLine);
		}
	}

	void OrderLineManager::removeOrder(const QUuid& orderId)
	{
		if (m_orderLines.size() > 0)
		{
			int index;
			for (index = 0; index < m_orderLines.size(); index++)
			{
				if (m_orderLines[index]->getOrderId() == orderId) break;
			}
			if (index < m_orderLines.size())
			{
				OrderLine* orderLine = m_orderLines[index];
				m_orderLines.removeAt(index);
				m_pView->getScene()->removeItem(orderLine);
				delete orderLine;
			}
		}
	}

	void OrderLineManager::clearOrders()
	{
        foreach (OrderLine* orderLine, m_orderLines)
		{
			m_pView->getScene()->removeItem(orderLine);
			delete orderLine;
		}
		m_orderLines.clear();
	}

	void OrderLineManager::showOrder(bool executed, bool show)
	{
		if (executed)
		{
			m_showExecutedOrder = show;
		}
		else
		{
			m_showPendingOrder = show;
		}
        foreach (OrderLine* orderLine, m_orderLines)
		{
			if (orderLine->isExecutedOrder() == executed)
			{
				if (show)
				{
					orderLine->setParentItem(m_pView->getPricePanel()->getMainPainterPanel());
					orderLine->updateUI();
				}
				else
				{
					m_pView->getScene()->removeItem(orderLine);
				}
			}
		}
	}

	void OrderLineManager::updateUI()
	{
        foreach (OrderLine* orderLine, m_orderLines)
		{
			if (orderLine->isExecutedOrder())
			{
				if (m_showExecutedOrder)
				{
					orderLine->updateUI();
				}
			}
			else
			{
				if (m_showPendingOrder)
				{
					orderLine->updateUI();
				}
			}
		}
	}
}
