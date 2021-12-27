#include "RealtimeDataSource.h"
#include "RealTimeChart.h"

namespace HistoricalChartSpace
{
	RealtimeDataSource::RealtimeDataSource(QString instrumentId, QString quotePolicyId, int decimalPlaces, QObject* parent)
		: DataSource(parent), m_decimalPlaces(decimalPlaces)
	{
	}

	RealtimeDataSource::~RealtimeDataSource()
	{
	}

	void RealtimeDataSource::OnRealTimeChartStarted(RealTimeChart* view)
	{
		if (view)
		{
			connect(this, SIGNAL(RealTimeDataComeSignal(const PrimRealTimeData&)),
				view, SLOT(RealTimeDataComeSlot(const PrimRealTimeData&)));

			connect(this, SIGNAL(UpdateRealTimeDataSignal(const QList<PrimRealTimeData>&)),
				view, SLOT(UpdateRealTimeDataSlot(const QList<PrimRealTimeData>&)));

			this->beginGetInitialData();
		}
	}

	int RealtimeDataSource::getDecimalPlaces()
	{
		return m_decimalPlaces;
	}

	void RealtimeDataSource::endGetInitialData(QList<PrimRealTimeData>& datas)
	{
		emit UpdateRealTimeDataSignal(datas);
	}
}


#include "moc_RealtimeDataSource.cpp"
