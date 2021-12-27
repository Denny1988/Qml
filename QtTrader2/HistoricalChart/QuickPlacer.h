#pragma once
#include <QString>
#include <QWidget>
#include <QUuid>
#include "historicalchart_global.h"

namespace HistoricalChartSpace
{
	class HistoricalChart;
	class HISTORICALCHART_EXPORT QuickPlacer
	{
	public:
		QuickPlacer();
		virtual ~QuickPlacer();

		virtual void setInstrument(QUuid instrumentId) = 0;
		virtual void setQuotePolicy(QUuid quotePolicyId) = 0;

		virtual bool canQuickPlace() = 0;

		virtual bool canBuyLimit(double price) = 0;
		virtual void buyLimit(double price) = 0;
		virtual bool canSellStop(double price) = 0;
		virtual void sellStop(double price) = 0;

		virtual bool canSellLimit(double price) = 0;
		virtual void sellLimit(double price) = 0;
		virtual bool canBuyStop(double price) = 0;
		virtual void buyStop(double price) = 0;

		virtual double getDefaultLots() = 0;

        // Open the order interface (Instrument is the current instrument)
		virtual void newOrder() = 0;

		virtual QWidget* getWidget() = 0;
	};
}
