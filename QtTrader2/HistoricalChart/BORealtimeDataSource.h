#pragma once
#include <QObject>
#include "Common.h"
#include "Instrument.h"
#include <QList>
#include "DataSource.h"

namespace HistoricalChartSpace
{
	class BORealTimeChart;
	class HISTORICALCHART_EXPORT BORealtimeDataSource : public DataSource
	{
		Q_OBJECT

		signals :
		void UpdateRealTimeDataSignal(const QList<PrimBORealTimeData>&);
		void RealTimeDataComeSignal(const PrimBORealTimeData&);

	public:
		BORealtimeDataSource(QString instrumentId, QString quotePolicyId, int decimalPlaces, QObject* parent = 0);
		virtual ~BORealtimeDataSource();

		void OnRealTimeChartStarted(BORealTimeChart* view);
		int getDecimalPlaces();
		void setDecimalPlaces(int value);

	protected:
		virtual void beginGetInitialData() = 0;
		void endGetInitialData(QList<PrimBORealTimeData>& datas);

	private:
		int m_decimalPlaces;
	};
}
