#pragma once
#include <QObject>
#include "Common.h"
#include <vector>
#include "Instrument.h"
#include <QList>
#include "DataSource.h"
#include "AsyncCallback.h"
using namespace std;

namespace HistoricalChartSpace
{
	class HistoricalChart;
	class HISTORICALCHART_EXPORT HistoricalDataSource : public DataSource
	{
		Q_OBJECT

		signals :
		void tradeStateChangedSignal(bool paused);
		void chartDataNotifySignal(PrimHistoricalData data);

	public:
		HistoricalDataSource(QObject* parent = 0);
		virtual ~HistoricalDataSource();

        // Take the initial data corresponding to Instrument, quotePolicy, and frequency determined by chartProperty
		void beginGetInitData(const ChartProperty& chartProperty, QSharedPointer<GetChartDataCallback> callback);

        // Take the specified data, where the order of the data in the vector is in increasing order of time
		virtual void beginGetHistoricalData(const ChartProperty& chartProperty, double endTime, int size, QSharedPointer<GetChartDataCallback> callback) = 0;

        // Take all the data from beginTime to endTime, and the time is arranged in ascending order
		virtual void beginGetFixPeriodData(const ChartProperty& chartProperty, double beginTime, double endTime, QSharedPointer<GetChartDataCallback> callback) = 0;

		virtual void beginGetFixPeriodData(const ChartProperty& chartProperty, int periodType, QSharedPointer<GetChartDataCallback> callback) = 0;

		virtual void beginGetQuotePolicies(QString instumentId, QSharedPointer<GetQuotePoliciesCallback> callback) = 0;

		virtual void beginGetLastBars(const ChartProperty& chartProperty, double lastDataDate, QSharedPointer<GetLastBarCallback> callback) = 0;

        // Get the system time from the data source
		virtual void beginGetCurrentTime(QSharedPointer<GetSystemTimeCallback> callback) = 0;

		// periodType 1: currentDay, 2: current week  3: current month, 4: current quarter  5: current year
		virtual double GetPeriodBeginTime(int periodType) = 0;
		// periodType Day2=6, Day3=7, Day4=8, Day5=9, Day10=10,
		virtual double GetPeriodBeginTime(int periodType, const QString& instrumentId) = 0;

        #if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
        //mobile
        virtual MobileMarketQuotation getMobileMarketQuotation(QString instumentIdStr) = 0;
        #endif

        // Get all Instrument information
		const QList<Instrument>& getInstrumentList() const;

        // Get all Frequency information
		const QList<Frequency>& getFrequencyList() const;

        // There should be a function to judge whether it exists to ensure the reliability of the data
		bool isInList(const QString& instumentId, const Frequency& frequency) const;

		void bindView(HistoricalChart* view);

        const QUuid* getCurrentInstrumentId();
        const QUuid* getCurrentQuotePolicyId();



	protected:
		QList<Instrument>		m_listInstrument;
		QList<Frequency>		m_listFrequency;

		// map for instrumentId to QuotePolicy
		QMap<string, QList<KeyDescription> > m_quotePolicies;
		HistoricalChart* m_pView;
	};
}
