#pragma once
#include <QMap>
#include <QUuid>
#include <QColor>
#include <QSharedPointer>
#include "historicalchart_global.h"

namespace HistoricalChartSpace
{
	struct HISTORICALCHART_EXPORT PriceAlertStruct
	{
		QUuid ID;
		QUuid InstrumentId;
		QUuid QuotePolicyId;
		QString Color;
		int Condition;
		QString Price;

		bool operator==(const PriceAlertStruct& other) const
		{
			return ID == other.ID;
		}
	};

	class AlertLine;
	class HistoricalChart;
	class HISTORICALCHART_EXPORT PriceAlertAdapter : public QObject
	{
		Q_OBJECT
		signals :
		void addAlertSignal(PriceAlertStruct alert);
		void editAlertSignal(QUuid alertId);
		void changeAlertPriceSignal(QUuid alertId, QString newPrice);
		void removeAlertSignal(QUuid alertId);

	public:
		PriceAlertAdapter(HistoricalChart* pView);
		~PriceAlertAdapter();

		void appendAlert(const PriceAlertStruct& alert);
		void udpateAlert(const PriceAlertStruct& alert);
		void removeAlert(const QUuid& alertId);
		void removeAlerts(const QList<QUuid>& alertIds);
		void clearAlerts();

		void updateUI();

	private:
		HistoricalChart* m_pView;
		QMap<QUuid, QSharedPointer<AlertLine>> m_alertLines;
	};
}