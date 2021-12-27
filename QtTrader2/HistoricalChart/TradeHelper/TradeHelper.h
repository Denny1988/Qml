#pragma once
#include <QObject>
#include <QMenu>
#include "QuickPlacer.h"
//#include "QuickModifier.h"

namespace HistoricalChartSpace
{
	class HistoricalChart;
	class TradeHelper : public QObject
	{
		Q_OBJECT
	public:
		TradeHelper(HistoricalChart* pView);
		~TradeHelper();

		void setQuickPlacer(QuickPlacer* placer);
		QuickPlacer* getQuickPlacer();
		//void setQuickModifier(QuickModifier* modifier);
		void changeInstrument(const QUuid& instrumentId);
		void changeQuotePolicy(const QUuid& quotePolicyId);
		void setPlacerPosition();
		void showQuickPlacer(bool isShow);
		void appendQuickTradeMenu(QMenu& menu, double price, double bid, double ask);

	private slots:
		void BuyLimitSlot();
		void SellStopSlot();
		void SellLimitSlot();
		void BuyStopSlot();

		void AddAlertSlot();
		void NewOrderSlot();

	private:
		HistoricalChart* m_pView;
		QuickPlacer* m_quickPlacer;

		QAction* m_buyLimitAction;
		QAction* m_sellStopAction;

		QAction* m_sellLimitAction;
		QAction* m_buyStopAction;

		QMenu* m_tradingMenu;
		QAction* m_addAlertAction;
		QAction* m_newOrderAction;

		//QuickModifier* m_quickModifier;
	};
}