#pragma once
#include <QObject>
#include "Common.h"
#include "Instrument.h"
#include <QList>
#include "DataSource.h"

namespace HistoricalChartSpace
{
	class RealTimeChart;
	class HISTORICALCHART_EXPORT RealtimeDataSource : public DataSource
	{
		Q_OBJECT

		signals :
		void UpdateRealTimeDataSignal(const QList<PrimRealTimeData>&);

	public:
		RealtimeDataSource(QString instrumentId, QString quotePolicyId, int decimalPlaces, QObject* parent = 0);
		virtual ~RealtimeDataSource();

		void OnRealTimeChartStarted(RealTimeChart* view);
		int getDecimalPlaces();

	protected:
		virtual void beginGetInitialData() = 0;
		void endGetInitialData(QList<PrimRealTimeData>& datas);

	private:
		int m_decimalPlaces;
	};
}
