#include "OrderLine.h"
#include "PricePanel.h"
#include "historicalchart.h"
#include "OrderLineManager.h"
#include "Cursors.h"
#include "SR.h"
#include <QMenu>
#include <QDebug>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

namespace HistoricalChartSpace
{
	OrderLine::OrderLine(PricePanel* pricePanel, OrderLineManager* orderLineManager, OrderInfo order, bool show)
		: QGraphicsLineItem(show ? pricePanel->getMainPainterPanel() : nullptr),
		m_pricePanel(pricePanel),
		m_orderLineManager(orderLineManager),
		m_order(order),
		m_closeOrderAction(nullptr),
		m_modifyOrderAction(nullptr),
		m_cancelOrderAction(nullptr),
		m_hintLine(nullptr)
	{
		// cursor
		QPixmap pixmap(adjust_vertical);
		QCursor cursor(pixmap, 0, 0);
		QGraphicsItem::setCursor(cursor);

		// pen for line
		m_pen.setStyle(Qt::DashDotLine);
		this->setPenColor();

		// tool tip
		QString infoText = QString("%1:%2 %3 %4[%5]")
			.arg(m_order.m_isExecuted ? SR::get("OpenOrder") : SR::get("PendingOrder"))
			.arg(m_order.m_accountCode)
			.arg(m_order.m_isBuy ? SR::get("Buy") : SR::get("Sell"))
			.arg(m_order.m_lots)
			.arg(m_order.m_orderCode);
		QGraphicsItem::setToolTip(infoText);
	}

	OrderLine::~OrderLine()
	{
		if (m_closeOrderAction) delete m_closeOrderAction;
		if (m_modifyOrderAction) delete m_modifyOrderAction;
		if (m_cancelOrderAction) delete m_cancelOrderAction;
	}

	const QUuid& OrderLine::getOrderId() const
	{
		return m_order.m_orderId;
	}

	bool OrderLine::isExecutedOrder() const
	{
		return m_order.m_isExecuted;
	}

	void OrderLine::updateUI()
	{
		this->setPenColor();
		this->prepareLine();
		QGraphicsItem::update();
	}

	void OrderLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
	{
		/* draw line */
		this->prepareLine();
		QGraphicsLineItem::paint(painter, option, widget);

		/* draw text */
		// 1.set pen.
		painter->setPen(m_pricePanel->getForeground());

		// 2.set position
		QRectF rect = option->exposedRect;
		double textHeight = 15.0;
		rect.setTop(rect.top() - textHeight);
		rect.setHeight(textHeight);

		// 3.draw text
		if (m_order.m_isExecuted)
		{
			painter->drawText(rect, QString("%1 %2 %3@%4[%5]")
				.arg(m_order.m_accountCode)
				.arg(m_order.m_isBuy ? SR::get("Buy") : SR::get("Sell"))
				.arg(m_order.m_lots)
				.arg(m_order.m_price)
				.arg(m_order.m_orderCode));
		}
		else
		{
			painter->drawText(rect, QString("%1 %2 %3 %4@%5[%6]")
				.arg(m_order.m_accountCode)
				.arg(m_order.m_isOpen ? SR::get("NewOrder") : SR::get("CloseOrderTitle"))
				.arg(m_order.m_isBuy ? SR::get("Buy") : SR::get("Sell"))
				.arg(m_order.m_lots)
				.arg(m_order.m_price)
				.arg(m_order.m_orderCode));
		}
	}

	void OrderLine::prepareLine()
	{
		QRectF rect = m_pricePanel->rect();
		double y = m_pricePanel->Value2Y(m_order.m_price);
		m_line.setLine(rect.left(), y, rect.left() + rect.width(), y);
		QGraphicsLineItem::setLine(m_line);
	}

	void OrderLine::setPenColor()
	{
		const PriceSettings& priceSettings = m_pricePanel->getPriceSettings();
		m_pen.setColor(m_order.m_isExecuted ? priceSettings.m_openOrderLineColor : priceSettings.m_pendingOrderLineColor);
		QGraphicsLineItem::setPen(m_pen);
	}

	void OrderLine::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
	{
		QuickModifier* quickModifier = m_orderLineManager->getOrderModifier();
		if (quickModifier)
		{
			QMenu menu(m_pricePanel->getView());
			if (m_order.m_isExecuted)
			{
				if (!m_closeOrderAction)
				{
					m_closeOrderAction = new QAction(SR::get("CloseOrder"), m_pricePanel);
					QObject::connect(m_closeOrderAction, &QAction::triggered, [=]()
					{
						quickModifier->closeOrder(this->getOrderId());
					});
				}
				menu.addAction(m_closeOrderAction);
			}
			else
			{
				if (!m_modifyOrderAction)
				{
					m_modifyOrderAction = new QAction(SR::get("ModifyOrder"), m_pricePanel);
					QObject::connect(m_modifyOrderAction, &QAction::triggered, [=]()
					{
						quickModifier->showModifyWindow(this->getOrderId()); 
					});
				}
				if (!m_cancelOrderAction)
				{
					m_cancelOrderAction = new QAction(SR::get("CancelOrder"), m_pricePanel);
					QObject::connect(m_cancelOrderAction, &QAction::triggered, [=]()
					{
						quickModifier->cancelPendingOrder(this->getOrderId());
					});
				}
				menu.addAction(m_modifyOrderAction);
				menu.addAction(m_cancelOrderAction);
			}

			menu.exec(QCursor::pos());
		}
	}

	void OrderLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
	{
		// to insure mouseMoveEvent work
	}

	void OrderLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
	{
		QuickModifier* quickModifier = m_orderLineManager->getOrderModifier();
		if (quickModifier)
		{
			if (event->button() | Qt::LeftButton)
			{
				double y = event->pos().y();
				double price = m_pricePanel->Y2Value(y);
				if (price != m_order.m_price)
				{
					if (m_order.m_isExecuted)
					{
						this->OnDragExecutedOrderLine(quickModifier, y, price);
					}
					else
					{
						this->OnDragPendingOrderLine(quickModifier, y, price);
					}
				}
			}
		}
	}

	void OrderLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
	{
		QuickModifier* quickModifier = m_orderLineManager->getOrderModifier();
		if (quickModifier)
		{
			if (m_hintLine && m_hintLine->isShowOnChart())
			{
				m_hintLine->hide();
				if (m_order.m_isExecuted)
				{
					if (m_hintLine->m_isStopLoss)
					{
						quickModifier->setStopLoss(m_order.m_orderId, m_hintLine->m_price);
					}
					else
					{
						quickModifier->setTakeProfit(m_order.m_orderId, m_hintLine->m_price);
					}
				}
				else
				{
					quickModifier->modify(m_order.m_orderId, m_hintLine->m_price);
				}
			}
		}
	}

	void OrderLine::OnDragExecutedOrderLine(QuickModifier* quickModifier, double y, double price)
	{
		if (m_order.m_isBuy != price > m_order.m_price)
		{
			//if (quickModifier->canSetStopLoss(m_order.m_orderId, price))
			//{
				if (!m_hintLine) m_hintLine = new HintLine(m_pricePanel, Qt::red);

				QString infoText = QString("sl@ %1 for [%2]").arg(price).arg(m_order.m_orderCode);
				m_hintLine->moveTo(y, price, infoText, true);
			//}
		}
		else
		{
			//if (quickModifier->canSetTakeProfit(m_order.m_orderId, price))
			//{
				if (!m_hintLine) m_hintLine = new HintLine(m_pricePanel, Qt::red);

				QString infoText = QString("tp@ %1 for [%2]").arg(price).arg(m_order.m_orderCode);
				m_hintLine->moveTo(y, price, infoText, false);
			//}
		}
	}

	void OrderLine::OnDragPendingOrderLine(QuickModifier* quickModifier, double y, double price)
	{
		if (quickModifier->canModify(m_order.m_orderId, price))
		{
			if (!m_hintLine) m_hintLine = new HintLine(m_pricePanel, Qt::darkGreen);

			QString infoText = QString("%1 %2 %3[%4]@%5")
				.arg(m_order.m_accountCode)
				.arg(m_order.m_isBuy ? SR::get("Buy") : SR::get("Sell"))
				.arg(m_order.m_lots)
				.arg(m_order.m_orderCode)
				.arg(price);
			m_hintLine->moveTo(y, price, infoText);
		}
	}
}
