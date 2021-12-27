#include "HistoricalDataSource.h"
#include "historicalchart.h"

namespace HistoricalChartSpace
{
	HistoricalDataSource::HistoricalDataSource(QObject* parent) : DataSource(parent), m_pView(nullptr)
	{
	}

	HistoricalDataSource::~HistoricalDataSource()
	{
	}

	void HistoricalDataSource::beginGetInitData(const ChartProperty& chartProperty, QSharedPointer<GetChartDataCallback> callback)
	{
		this->beginGetHistoricalData(chartProperty, 0, 0, callback);
	}

	void HistoricalDataSource::bindView(HistoricalChart* view)
	{
		if (view && !m_pView)
		{
			m_pView = view;
			connect(this, SIGNAL(RealTimeDataComeSignal(const PrimRealTimeData&)), view, SLOT(RealTimeDataComeSlot(const PrimRealTimeData&)));
			connect(this, SIGNAL(tradeStateChangedSignal(bool)), view, SLOT(ChangeTradeStateSlot(bool)));
			connect(this, SIGNAL(chartDataNotifySignal(PrimHistoricalData)), view, SLOT(ChartDataNotifySlot(PrimHistoricalData)));
		}
	}

    const QUuid* HistoricalDataSource::getCurrentInstrumentId()
    {
        return m_pView ? &(m_pView->getInstrument().getId()) : nullptr;
	}

    const QUuid* HistoricalDataSource::getCurrentQuotePolicyId()
    {
        return m_pView ? &(m_pView->getQuotePolicy().getId()) : nullptr;
	}

	const QList<Instrument>& HistoricalDataSource::getInstrumentList() const
	{
		return m_listInstrument;
	}

    // Specific subclasses should save all Frequency information
	const QList<Frequency>& HistoricalDataSource::getFrequencyList() const
	{
		return m_listFrequency;
	}

    // There should be a function to judge whether it exists to ensure the reliability of the data
    bool HistoricalDataSource::isInList(const QString& instumentId, const Frequency& frequency) const
	{
		bool containsInstrument = false;
        foreach (Instrument instrument, m_listInstrument)
		{
			if (instrument.getKey() == instumentId)
			{
				containsInstrument = true;
				break;
			}
		}
		return containsInstrument && m_listFrequency.contains(frequency);
	}
}

#include "moc_HistoricalDataSource.cpp"
