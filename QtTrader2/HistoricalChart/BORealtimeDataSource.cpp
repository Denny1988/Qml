#include "BORealtimeDataSource.h"
#include "BORealTimeChart.h"

namespace HistoricalChartSpace
{
	BORealtimeDataSource::BORealtimeDataSource(QString instrumentId, QString quotePolicyId, int decimalPlaces, QObject* parent)
		: DataSource(parent), m_decimalPlaces(decimalPlaces)
	{
	}

	BORealtimeDataSource::~BORealtimeDataSource()
	{
	}

	void BORealtimeDataSource::OnRealTimeChartStarted(BORealTimeChart* view)
	{
		if (view)
		{
			connect(this, SIGNAL(RealTimeDataComeSignal(const PrimBORealTimeData&)),
				view, SLOT(RealTimeDataComeSlot(const PrimBORealTimeData&)));

			connect(this, SIGNAL(UpdateRealTimeDataSignal(const QList<PrimBORealTimeData>&)),
				view, SLOT(UpdateRealTimeDataSlot(const QList<PrimBORealTimeData>&)));

			this->beginGetInitialData();
		}
	}

	int BORealtimeDataSource::getDecimalPlaces()
	{
		return m_decimalPlaces;
	}

	void BORealtimeDataSource::setDecimalPlaces(int value)
	{
		m_decimalPlaces = value;
	}

	void BORealtimeDataSource::endGetInitialData(QList<PrimBORealTimeData>& datas)
	{
		emit UpdateRealTimeDataSignal(datas);
	}
}


#include "moc_BORealtimeDataSource.cpp"
