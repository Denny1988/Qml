#include "TradeHelper/TradeHelper.h"
#include "../historicalchart.h"
#include "SR.h"
#include "PriceAlert/PriceAlertAdapter.h"
#include "ToolBar.h"

namespace HistoricalChartSpace
{
	TradeHelper::TradeHelper(HistoricalChart* pView) : m_pView(pView), m_quickPlacer(nullptr)
	{
		m_buyLimitAction = new QAction(pView);
		m_sellStopAction = new QAction(pView);
		m_sellLimitAction = new QAction(pView);
		m_buyStopAction = new QAction(pView);

		m_tradingMenu = new QMenu(SR::get("Trading"), pView);
		m_addAlertAction = new QAction(pView);
		m_newOrderAction = new QAction(SR::get("NewOrder"), pView);

		QObject::connect(m_buyLimitAction, SIGNAL(triggered()), SLOT(BuyLimitSlot()));
		QObject::connect(m_sellStopAction, SIGNAL(triggered()), SLOT(SellStopSlot()));
		QObject::connect(m_sellLimitAction, SIGNAL(triggered()), SLOT(SellLimitSlot()));
		QObject::connect(m_buyStopAction, SIGNAL(triggered()), SLOT(BuyStopSlot()));

		QObject::connect(m_addAlertAction, SIGNAL(triggered()), SLOT(AddAlertSlot()));
		QObject::connect(m_newOrderAction, SIGNAL(triggered()), SLOT(NewOrderSlot()));
	}

	TradeHelper::~TradeHelper()
	{
	}

	void TradeHelper::setQuickPlacer(QuickPlacer* placer)
	{
        m_quickPlacer = placer;
		m_quickPlacer->getWidget()->setParent(m_pView);
		m_quickPlacer->setInstrument(m_pView->m_instrument.getKey());
		m_quickPlacer->setQuotePolicy(m_pView->m_quotePolicy.getKey());
        this->setPlacerPosition();
	}


	QuickPlacer* TradeHelper::getQuickPlacer()
	{
		return m_quickPlacer;
	}

	void TradeHelper::changeInstrument(const QUuid& instrumentId)
	{
		if (m_quickPlacer) m_quickPlacer->setInstrument(instrumentId);
	}

	void TradeHelper::changeQuotePolicy(const QUuid& quotePolicyId)
	{
		if (m_quickPlacer) m_quickPlacer->setQuotePolicy(quotePolicyId);
	}

	void TradeHelper::setPlacerPosition()
	{
		if (m_quickPlacer)
		{
			const int margin = 4;
			int x = margin, y = margin;
			m_quickPlacer->getWidget()->move(x, y);
		}
	}

	void TradeHelper::showQuickPlacer(bool isShow)
    {
		if (m_quickPlacer && m_quickPlacer->canQuickPlace())
        {
			m_quickPlacer->getWidget()->setVisible(isShow);
			if (isShow)
			{
				//m_pView->m_toolbar->OnChartResize(m_pView->width());
			}
		}
	}

	void TradeHelper::appendQuickTradeMenu(QMenu& menu, double price, double bid, double ask)
	{
		m_tradingMenu->clear();
		if (m_quickPlacer == nullptr) return;

		QString priceString = m_pView->m_instrument.getPriceString(price);
		price = priceString.toDouble();

		// add one click place main action
		double defaultLots = m_quickPlacer->getDefaultLots();
		if (price > ask || price < bid)
		{
			bool isAbove = price > ask;
			if (isAbove != m_pView->m_instrument.isNormal())
			{
				if (m_quickPlacer->canBuyLimit(price))
				{
					m_buyLimitAction->setText(SR::get("BuyLimit").arg(defaultLots).arg(priceString));
					m_buyLimitAction->setData(price);
					menu.addAction(m_buyLimitAction);
					menu.addSeparator();

					m_tradingMenu->addAction(m_buyLimitAction);
					m_tradingMenu->addSeparator();
					if (m_quickPlacer->canSellStop(price))
					{
						m_sellStopAction->setText(SR::get("SellStop").arg(defaultLots).arg(priceString));
						m_sellStopAction->setData(price);
						m_tradingMenu->addAction(m_sellStopAction);
					}
				}
			}
			else
			{
				if (m_quickPlacer->canSellLimit(price))
				{
					m_sellLimitAction->setText(SR::get("SellLimit").arg(defaultLots).arg(priceString));
					m_sellLimitAction->setData(price);
					menu.addAction(m_sellLimitAction);
					menu.addSeparator();

					m_tradingMenu->addAction(m_sellLimitAction);
					m_tradingMenu->addSeparator();
					if (m_quickPlacer->canBuyStop(price))
					{
						m_buyStopAction->setText(SR::get("BuyStop").arg(defaultLots).arg(priceString));
						m_buyStopAction->setData(price);
						m_tradingMenu->addAction(m_buyStopAction);
					}
				}
			}
		}

		// add trading sub menu
		m_addAlertAction->setText(SR::get("Alert").arg(priceString));
		m_addAlertAction->setData(price);
		m_tradingMenu->addAction(m_addAlertAction);
		m_tradingMenu->addSeparator();
		m_tradingMenu->addAction(m_newOrderAction);
		menu.addMenu(m_tradingMenu);
	}

	void TradeHelper::BuyLimitSlot()
	{
		QAction *action = qobject_cast<QAction*>(sender());
        //_ASSERT(action);
		double price = action->data().toDouble();
		m_quickPlacer->buyLimit(price);
	}

	void TradeHelper::SellStopSlot()
	{
		QAction *action = qobject_cast<QAction*>(sender());
        //_ASSERT(action);
		double price = action->data().toDouble();
		m_quickPlacer->sellStop(price);
	}

	void TradeHelper::SellLimitSlot()
	{
		QAction *action = qobject_cast<QAction*>(sender());
        //_ASSERT(action);
		double price = action->data().toDouble();
		m_quickPlacer->sellLimit(price);
	}

	void TradeHelper::BuyStopSlot()
	{
		QAction *action = qobject_cast<QAction*>(sender());
        //_ASSERT(action);
		double price = action->data().toDouble();
		m_quickPlacer->buyStop(price);
	}

	void TradeHelper::AddAlertSlot()
	{
		double price = m_addAlertAction->data().toDouble();
		PriceAlertStruct alert;
		alert.ID = QUuid::createUuid();
		alert.InstrumentId = m_pView->getInstrument().getKey();
		alert.QuotePolicyId = m_pView->getQuotePolicyId();
		alert.Color = "Red";
		
		const int BidLessThan = 0;
		const int BidGreaterThan = 1;
		BaseData& baseData = m_pView->getBaseData();
        //_ASSERT(baseData.size() > 0);
		double bid = baseData.getCloses()[baseData.size() - 1];
		alert.Condition = price > bid ? BidGreaterThan : BidLessThan;
		alert.Price = m_pView->getInstrument().getPriceString(price);

		m_pView->getPriceAlertManager()->addAlertSignal(alert);
	}

	void TradeHelper::NewOrderSlot()
	{
		m_quickPlacer->newOrder();
	}

}

#include "moc_TradeHelper.cpp"
