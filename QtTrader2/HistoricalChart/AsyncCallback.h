#pragma once
#include <QList>
#include "Instrument.h"
#include "historicalchart.h"

/*
class HISTORICALCHART_EXPORT AsyncCallback
{
public:
	virtual ~AsyncCallback(){} // insure dynamic_cast
};

template<typename resultType>
class HISTORICALCHART_EXPORT CallBack : public AsyncCallback
{
public:
	typedef void(HistoricalChart::* CallbackFunc)(int, resultType);

public:
	CallBack(int callId, HistoricalChart* chartView, CallbackFunc func) : m_callId(callId), m_chartView(chartView), m_feedbackFunc(func){}
	virtual ~CallBack(){}

	void operator()(resultType result)
	{
		m_chartView->UpdateQuotePolicy(m_callId, result);
		(m_chartView->*m_feedbackFunc)(m_callId, result);
	}

	int callId()
	{
		return m_callId;
	}

private:
	int m_callId;
	HistoricalChart* m_chartView;
	CallbackFunc m_feedbackFunc;
};
*/

namespace HistoricalChartSpace
{

	class HISTORICALCHART_EXPORT AsyncCallback
	{
	public:
		AsyncCallback(int callId) : m_callId(callId){}
		virtual ~AsyncCallback(){} // insure dynamic_cast
		int callId()
		{
			return m_callId;
		}

	protected:
		int m_callId;
        HistoricalChart* m_chartView;  // If there are multiple Callback targets, consider using void*.
	};

	class HISTORICALCHART_EXPORT GetQuotePoliciesCallback : public AsyncCallback
	{
	public:
		GetQuotePoliciesCallback(int callId, HistoricalChart* chartView) : AsyncCallback(callId)
		{
			m_chartView = chartView;
		}
		virtual ~GetQuotePoliciesCallback(){}

		void operator()(QList<KeyDescription>& result)
		{
			m_chartView->UpdateQuotePolicy(m_callId, result);
		}
	};

	class HISTORICALCHART_EXPORT GetChartDataCallback: public AsyncCallback
	{
	public:
		GetChartDataCallback(int callId, HistoricalChart* chartView) : AsyncCallback(callId)
		{
			m_chartView = chartView;
		}
		~GetChartDataCallback(){}

		void operator()(QSharedPointer<vector<PrimHistoricalData>> result)
		{
			m_chartView->UpdateHistoricalData(m_callId, result);
		}
	};

	class HISTORICALCHART_EXPORT GetLastBarCallback : public AsyncCallback
	{
	public:
		GetLastBarCallback(int callId, HistoricalChart* chartView) : AsyncCallback(callId)
		{
			m_chartView = chartView;
		}
		~GetLastBarCallback(){}

		void operator()(QSharedPointer<vector<PrimHistoricalData>> datas)
		{
			m_chartView->LastBarDataCome(m_callId, datas);
		}
	};

	class HISTORICALCHART_EXPORT GetSystemTimeCallback : public AsyncCallback
	{
	public:
		GetSystemTimeCallback(int callId, HistoricalChart* chartView) : AsyncCallback(callId)
		{
			m_chartView = chartView;
		}
		~GetSystemTimeCallback(){}

		void operator()(double currentTime)
		{
			m_chartView->setSystemTime(m_callId, currentTime);
		}
	};
}
