#include "RandomDataSource.h"
#include <QTimer>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "JulianDate.h"
namespace HistoricalChartSpace
{
	QString GBPUSD_ID("93A80A1E-CCA1-4DD9-9AA4-4DD2D87693C7");
	QString USDJPY_ID("D86C319F-3D66-40BB-9781-44E35D8CE2E2");
	QString PolicyDef_ID("9459DD19-33EF-437C-98AD-2B55069095A1");
	QString PolicyForex_ID("1944540F-90E5-44DC-A9C9-1107D3B185E2");

	RandomDataSource::RandomDataSource() : HistoricalDataSource(NULL), m_basePrice(1200), m_volatilityRange(3)
	{
		m_pTimer = new QTimer(this);
		connect(m_pTimer, SIGNAL(timeout()), this, SLOT(NewDataComeSlot()));
		m_pTimer->start(10000);
		srand(time(NULL));
		this->init();
		m_lastBar.open = 0;
	}

	void RandomDataSource::init()
	{
		//m_listInstrument << Instrument("1", "EURUSD") << Instrument("2", "GBPUSD") << Instrument("3", "USDCHF")
		//	<< Instrument("4", "USDJPY") << Instrument("5", "USDCAD") << Instrument("6", "AUDUSD");

		//m_listFrequency << Frequency(Minute) << Frequency(Minute, 5) << Frequency(Minute, 15) << Frequency(Minute, 30)
		//	<< Frequency(Hour) << Frequency(Hour, 2) << Frequency(Hour, 3) << Frequency(Day) << Frequency(Month);

		this->m_listInstrument.append(Instrument("C3D34552-96F4-4116-B392-05DE41707B60", "AUDUSD", 2));
		this->m_listInstrument.append(Instrument("93A80A1E-CCA1-4DD9-9AA4-4DD2D87693C7", "GBPUSD", 4, false));
		this->m_listInstrument.append(Instrument("99CCA313-DE2E-40D7-8A1A-9CD84EB471F6", "USDCHF", 4));
		this->m_listInstrument.append(Instrument("D86C319F-3D66-40BB-9781-44E35D8CE2E2", "USDJPY", 2));
		this->m_listInstrument.append(Instrument("6D0AC9BF-B187-4999-B786-1011FA2AFCBE", "USDCAD", 4));
		this->m_listInstrument.append(Instrument("5D9F1143-8648-4E2B-BE8E-1C26D441D1E5", "EURUSD", 4));

		this->m_listFrequency.append(Frequency(Minute));
		this->m_listFrequency.append(Frequency(Minute, 5));
		this->m_listFrequency.append(Frequency(Minute, 15));
		this->m_listFrequency.append(Frequency(Minute, 30));
		this->m_listFrequency.append(Frequency(Hour));
		this->m_listFrequency.append(Frequency(Hour, 2));
		this->m_listFrequency.append(Frequency(Hour, 3));
		this->m_listFrequency.append(Frequency(Day));
		this->m_listFrequency.append(Frequency(Month));
	}

	void RandomDataSource::beginGetHistoricalData(const ChartProperty& chartProperty, double endTime, int size, QSharedPointer<GetChartDataCallback> callback)
	{
		if (endTime < ORIGIN_JULIAN && size == 0)
		{
			size = 2000;
			endTime = JulianDate::GetCurrentJDate();
		}
		size = 50;
        //double timeSpan = chartProperty.frequency.getJDateTimeSpan();  // The accuracy of using timeSpan is not enough! ! ! Use instead: QDateTime

		double timeBeforeAdjust = endTime;
		this->adjustBegin(endTime, chartProperty.frequency);

		QString beforeAdjust = JulianDate::FromJDateToString(timeBeforeAdjust);
		QString adjusted = JulianDate::FromJDateToString(endTime);
		
		QDateTime dateTime = JulianDate::JDateToQTDate(endTime);

		vector<PrimHistoricalData> vecData;
		//for (int i = 0; i < size; ++i, endTime -= timeSpan)
		for (int i = 0; i < size; ++i)
		{
			PrimHistoricalData data;
			data.date = JulianDate::QTDateToJDate(dateTime);
			genHistoricalData(&data);
			vecData.push_back(data);

			switch (chartProperty.frequency.getType())
			{
			case HistoricalChartSpace::Minute:
				dateTime = dateTime.addSecs(-chartProperty.frequency.getUint() * 60);
				break;
			case HistoricalChartSpace::Hour:
				dateTime = dateTime.addSecs(-chartProperty.frequency.getUint() * 60 * 60);
				break;
			case HistoricalChartSpace::Day:
				dateTime = dateTime.addDays(-chartProperty.frequency.getUint());
				break;
			case HistoricalChartSpace::Week:
				dateTime = dateTime.addDays(-chartProperty.frequency.getUint() * 7);
				break;
			case HistoricalChartSpace::Month:
				dateTime = dateTime.addMonths(-chartProperty.frequency.getUint());
				break;
			default:

				break;
			}
		}
		
		QSharedPointer<vector<PrimHistoricalData>> datas(new vector<PrimHistoricalData>());
		for (int i = vecData.size() - 1; i >= 0; --i)
		{
			datas->push_back(vecData[i]);
		}
		//emit UpdateHistoricalDataSignal(callId, datas);
		(*callback.data())(datas);
	}

	void RandomDataSource::beginGetFixPeriodData(const ChartProperty& chartProperty, double beginTime, double endTime, QSharedPointer<GetChartDataCallback> callback)
	{
		double timeSpan = chartProperty.frequency.getJDateTimeSpan();

		QSharedPointer<vector<PrimHistoricalData>> datas(new vector<PrimHistoricalData>());
		for (double time = beginTime; time <= endTime; time += timeSpan)
		{
			PrimHistoricalData data;
			if (endTime < ORIGIN_JULIAN) return;
			data.date = endTime;
			genHistoricalData(&data);
			datas->push_back(data);
		}
		(*callback.data())(datas);
	}

	void RandomDataSource::beginGetFixPeriodData(const ChartProperty& chartProperty, int periodType, QSharedPointer<GetChartDataCallback> callback)
	{

	}

	void RandomDataSource::beginGetLastBars(const ChartProperty& chartProperty, double lastDataDate, QSharedPointer<GetLastBarCallback> callback)
	{
		if (isInList(chartProperty.instrumentId, chartProperty.frequency))
		{
			PrimHistoricalData data;
			data.date = lastDataDate;
			this->genHistoricalData(&data);

			m_lastBar.date = lastDataDate + chartProperty.frequency.getJDateTimeSpan();

			QSharedPointer<vector<PrimHistoricalData>> datas(new vector<PrimHistoricalData>());
			datas->push_back(data);
			datas->push_back(m_lastBar);
			(*callback.data())(datas);
			
			m_lastBar.open = m_lastBar.high = m_lastBar.low = m_lastBar.close;
			m_lastBar.volume = 125;
		}
	}

	void RandomDataSource::getRealTimeData(QList<PrimRealTimeData> *datas)
	{
		PrimRealTimeData data;
		data.date = JulianDate::GetCurrentJDate();
		genRealTimeData(data);
		datas->append(data);

		for (int i = 1; i < 2000; ++i)
		{
			data.date = data.date - 1.0 / 24 / 60 / 60;
			genRealTimeData(data);
			datas->push_front(data);
		}
	}

	void RandomDataSource::NewDataComeSlot()
	{
		static int count = 0;
		++count;
		PrimRealTimeData data;
		data.date = JulianDate::GetCurrentJDate();
		genRealTimeData(data);
		emit RealTimeDataComeSignal(data);


		// form last bar:
		if (m_lastBar.open == 0) m_lastBar.open = m_lastBar.high = m_lastBar.low = m_lastBar.close = data.bid;
		m_lastBar.close = data.bid;
		if (data.bid > m_lastBar.high) m_lastBar.high = data.bid;
		if (data.bid < m_lastBar.low) m_lastBar.low = data.bid;
	}

	void RandomDataSource::genHistoricalData(PrimHistoricalData* data) const
	{
		//1235 + rand()/RAND_MAX
		double volatility = m_volatilityRange * rand() / RAND_MAX;
		data->low = m_basePrice - volatility;
		data->high = m_basePrice + volatility;
		data->open = data->low + 2 * volatility * rand() / RAND_MAX;
		data->close = data->low + 2 * volatility * rand() / RAND_MAX;

		//data->low = rand() % 130000 / 1000.0;
		//data->close = data->low + rand() % 10;
		//data->open = data->low + rand() % 10;
		//data->high = data->low + 10 + rand() % 10;
		//data->volume = rand() % 1000;
	}

	void RandomDataSource::genRealTimeData(PrimRealTimeData& data) const
	{
		double volatility = m_volatilityRange * rand() / RAND_MAX;
		double volatility2 = m_volatilityRange * rand() / RAND_MAX;
		data.bid = m_basePrice - volatility / 2 + volatility2 / 2;
		data.ask = data.bid + m_volatilityRange / 60.0;

		//data.bid = rand() % 130000 / 1000.0;
		//data.ask = data.bid + rand() % 40;
	}

	void RandomDataSource::beginGetCurrentTime(QSharedPointer<GetSystemTimeCallback> callback)
	{
		double now = JulianDate::GetCurrentJDate();
		(*callback.data())(now);
	}

	void RandomDataSource::setTimeInterval(int interval)
	{
		m_pTimer->setInterval(interval);
	}

	void RandomDataSource::setBasePrice(int basePrice)
	{
		m_basePrice = basePrice;
	}

	void RandomDataSource::setVolatilityRange(int volatilityRange)
	{
		m_volatilityRange = volatilityRange;
	}

	void RandomDataSource::beginGetQuotePolicies(QString instumentId, QSharedPointer<GetQuotePoliciesCallback> callback)
	{
		QList<KeyDescription> quotePolicies;
		quotePolicies << KeyDescription(PolicyDef_ID, "Def");
		if (instumentId != GBPUSD_ID)  // "GBPUSD"
		{
			quotePolicies << KeyDescription(PolicyForex_ID, "Forex");
		}
			
		(*callback.data())(quotePolicies);
	}

	double RandomDataSource::GetPeriodBeginTime(int rangeType)
	{
		return 0;
	}
	double RandomDataSource::GetPeriodBeginTime(int periodType, const QString& instrumentId)
	{
		return 0;
	}
    #if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
    MobileMarketQuotation getMobileMarketQuotation(QString instumentIdStr)
    {
        MobileMarketQuotation mobileMarketQuotation;
        return mobileMarketQuotation;
    }
    #endif

	void RandomDataSource::adjustBegin(double& begin, const Frequency& frequency)
	{
		StructDate structDate = JulianDate::FromJDate(begin);
		structDate.Millisecond = 0;

		FrequencyType type = frequency.getType();
		int uint = frequency.getUint();
		switch (type)
		{
		case HistoricalChartSpace::Second:
			break;
		case HistoricalChartSpace::Minute:
		{
			structDate.Second = 0;
			structDate.Minute -= structDate.Minute % uint;
		}
		break;
		case HistoricalChartSpace::Hour:
		{
			structDate.Minute = 0;
			structDate.Second = 0;
			structDate.Hour -= structDate.Hour % uint;
		}
		break;
		case HistoricalChartSpace::Day:
		{
			structDate.Hour = 0;
			structDate.Minute = 0;
			structDate.Second = 0;
		}
		break;
		case HistoricalChartSpace::Week:
			break;
		case HistoricalChartSpace::Month:
			break;
		case HistoricalChartSpace::Year:
			break;
		default:
			break;
		}

		begin = JulianDate::ToJulianDate(structDate);
	}


    RandomDataSource* GetDataSource()
	{
		RandomDataSource *pDataSource = new RandomDataSource;
		return pDataSource;
	}

    RealtimeDataSource*  GetRealtimeDataSource()
	{
		return new RandomRealtimeDataSource;
	}

    BORealtimeDataSource*  GetBORealtimeDataSource()
	{
		return new RandomBORealtimeDataSource;
	}

	RandomRealtimeDataSource::RandomRealtimeDataSource() : RealtimeDataSource("607DD4F5-210F-4C99-8E48-E340EC48184C", "479D4D90-C20A-49F9-9266-C44543379536", 3, NULL)
	{
		m_pTimer = new QTimer(this);
		connect(m_pTimer, SIGNAL(timeout()), this, SLOT(NewDataComeSlot()));
		m_pTimer->start(1000);
		srand(time(NULL));
	}

	void RandomRealtimeDataSource::beginGetInitialData()
{
		QList<PrimRealTimeData> datas;
		PrimRealTimeData data;
		data.date = JulianDate::GetCurrentJDate();
		genRealTimeData(data);
		datas.append(data);

		for (int i = 1; i < 120; ++i)
		{
			data.date = data.date - 1.0 / 24 / 60 / 60;
			genRealTimeData(data);
			datas.push_front(data);
		}

		this->endGetInitialData(datas);
	}

	void RandomRealtimeDataSource::NewDataComeSlot()
	{
		static int count = 0;
		++count;
		PrimRealTimeData data;
		data.date = JulianDate::GetCurrentJDate();
		genRealTimeData(data);
		emit RealTimeDataComeSignal(data);
	}

	void RandomRealtimeDataSource::genRealTimeData(PrimRealTimeData& data) const
	{
		data.bid = rand() % 130000 / 1000.0;
		data.ask = data.bid + rand() % 40;
	}


	RandomBORealtimeDataSource::RandomBORealtimeDataSource() : BORealtimeDataSource("607DD4F5-210F-4C99-8E48-E340EC48184C", "479D4D90-C20A-49F9-9266-C44543379536", 3, NULL)
	{
		m_pTimer = new QTimer(this);
		connect(m_pTimer, SIGNAL(timeout()), this, SLOT(NewDataComeSlot()));
		m_pTimer->start(1000);
		srand(time(NULL));
	}

	void RandomBORealtimeDataSource::beginGetInitialData()
{
		QList<PrimBORealTimeData> datas;
		PrimBORealTimeData data;
		data.date = JulianDate::GetCurrentJDate();
		genRealTimeData(data);
		datas.append(data);

		for (int i = 1; i < 120; ++i)
		{
			data.date = data.date - 1.0 / 24 / 60 / 60;
			genRealTimeData(data);
			datas.push_front(data);
		}

		this->endGetInitialData(datas);
	}

	void RandomBORealtimeDataSource::NewDataComeSlot()
	{
		static int count = 0;
		++count;
		PrimBORealTimeData data;
		data.date = JulianDate::GetCurrentJDate();
		genRealTimeData(data);
		emit RealTimeDataComeSignal(data);
	}

	void RandomBORealtimeDataSource::genRealTimeData(PrimBORealTimeData& data) const
	{
		double bid = rand() % 130000 / 1000.0;
		double ask = bid + rand() % 40;
		data.barStroke = bid;
	}
}


#include "moc_RandomDataSource.cpp"
