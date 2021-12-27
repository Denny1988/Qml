#include "BaseData.h"
#include <stdio.h>
#include "math.h"
#include <algorithm>
using namespace std;

using namespace HistoricalChartSpace;

BaseData::BaseData()
	: m_beginIndex(0), m_endIndex(0), m_barDistance(5), m_isTailTruncated(false)
{
	m_realTimeAsk = std::numeric_limits<double>::quiet_NaN();
}

BaseData::~BaseData()
{
}

int BaseData::size() const
{
	return m_dates.size();
}

void BaseData::appendData(const PrimHistoricalData& data)
{
	m_opens.push_back(data.open);
	m_highs.push_back(data.high);
	m_lows.push_back(data.low);
	m_closes.push_back(data.close);
	m_volumns.push_back(data.volume);
	m_dates.push_back(data.date);
}

void BaseData::insertData(const PrimHistoricalData& data)
{
	m_opens.insert(m_opens.begin(), data.open);
	m_highs.insert(m_highs.begin(), data.high);
	m_lows.insert(m_lows.begin(), data.low);
	m_closes.insert(m_closes.begin(), data.close);
	m_volumns.insert(m_volumns.begin(), data.volume);
	m_dates.insert(m_dates.begin(), data.date);
}

void BaseData::joinDatasAtFront(vector<PrimHistoricalData>* datas)
{
    //_ASSERT(datas->at(datas->size() - 1).date == m_dates[0]);
	datas->pop_back();

	vector<PrimHistoricalData> oldDatas;
	for (size_t i = 0; i < m_dates.size(); i++)
	{
		PrimHistoricalData data;
		data.close = m_closes[i];
		data.date = m_dates[i];
		data.high = m_highs[i];
		data.low = m_lows[i];
		data.open = m_opens[i];
		data.volume = m_volumns[i];
		oldDatas.push_back(data);
	}

	this->clear();
	for (size_t i = 0; i < datas->size(); i++)
	{
		this->appendData(datas->at(i));
	}
	for (size_t i = 0; i < oldDatas.size(); i++)
	{
		this->appendData(oldDatas[i]);
	}
}

void BaseData::joinDatasAtEnd(vector<PrimHistoricalData>* datas)
{
	this->update(this->size() - 1, datas->at(0));

	for (size_t i = 1; i < datas->size(); i++)
	{
		this->appendData(datas->at(i));
	}
}

void BaseData::clear()
{
	m_opens.clear();
	m_highs.clear();
	m_lows.clear();
	m_closes.clear();
	m_volumns.clear();
	m_dates.clear();
	m_isTailTruncated = false;
}

double BaseData::getDate(int index) const
{
	double jdate = -1.0;
	if (index < m_dates.size())
		jdate = m_dates[index];
	return jdate;
}

int BaseData::timeToIndex(double time) const
{
	auto lower = std::lower_bound(m_dates.begin(), m_dates.end(), time);
	return lower - m_dates.begin();
}

void BaseData::getBidAsk(double &bid, double &ask) const
{
	if (m_closes.size() > 0)
	{
		bid = m_closes[m_closes.size() - 1];
        #if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
        ask = qIsNaN(m_realTimeAsk) ? bid : m_realTimeAsk;
        #endif
        #ifdef RUNTIME_OS_WIN32
        ask = std::isnan(m_realTimeAsk) ? bid : m_realTimeAsk;
        #endif
        #ifdef RUNTIME_OS_MACX
        ask = isnan(m_realTimeAsk) ? bid : m_realTimeAsk;
        #endif
	}
	else
	{
		ask = bid = 0;
	}
}

const vector<double>& BaseData::getOpens() const
{
	return m_opens;
}

const vector<double>& BaseData::getHighs() const
{
	return m_highs;
}

const vector<double>& BaseData::getLows() const
{
	return m_lows;
}

const vector<double>& BaseData::getCloses() const
{
	return m_closes;
}
const vector<double>& BaseData::getVolumes() const
{
	return m_volumns;
}
const vector<double>& BaseData::getDates() const
{
	return m_dates;
}

void BaseData::update(const int index, const PrimHistoricalData & data)
{
	//if (m_dates[index] != data.date)
	//{
	//	qDebug() << "basedata_update: ";
	//	qDebug() << m_dates[index];
	//	qDebug() << data.date;
	//}
	m_opens[index] = data.open;
	m_highs[index] = data.high;
	m_lows[index] = data.low;
	m_closes[index] = data.close;
	m_volumns[index] = data.volume;
	m_dates[index] = data.date;
}

void BaseData::realTimeDataCome(const PrimRealTimeData& data, double timeSpan)
{
	int lastBarIndex = m_closes.size() - 1;
	if (lastBarIndex >= 0)
	{
		m_realTimeAsk = data.ask;
		if (data.date < m_dates[lastBarIndex] + timeSpan)
		{
			m_closes[lastBarIndex] = data.bid;
			if (data.bid > m_highs[lastBarIndex]) m_highs[lastBarIndex] = data.bid;
			if (data.bid < m_lows[lastBarIndex]) m_lows[lastBarIndex] = data.bid;
			//m_volumns[sync_start_index] += data.v  // real time data should containt volumn if need update here.
		}
	}
}

size_t BaseData::getRangeDataCount(double beginTime, double endTime)
{
	size_t dataCount = 0;
	if (m_dates.size() > 0)
	{
		auto lower = std::lower_bound(m_dates.begin(), m_dates.end(), beginTime);
		auto upper = std::upper_bound(m_dates.begin(), m_dates.end(), endTime);
		dataCount = upper - lower;
	}
	return dataCount;
}

void BaseData::truncateBefore(double beginTime)
{
	if (beginTime > m_dates[m_dates.size() - 1])
	{
		this->clear();
	}
	else
	{
		int index = 0;
		for (; index < m_dates.size(); index++)
		{
			if (beginTime <= m_dates[index])
			{
				break;
			}
		}
        //_ASSERT(index < m_dates.size());

		this->truncateBefore(m_opens, index);
		this->truncateBefore(m_highs, index);
		this->truncateBefore(m_lows, index);
		this->truncateBefore(m_closes, index);
		this->truncateBefore(m_volumns, index);
		this->truncateBefore(m_dates, index);
	}
}

void BaseData::truncateBefore(vector<double>& vect, int index)
{
	vector<double> vectTemp;
	for (int i = index; i < vect.size(); i++)
	{
		vectTemp.push_back(vect[i]);
	}
	vect.clear();
	for (int i = 0; i < vectTemp.size(); i++)
	{
		vect.push_back(vectTemp[i]);
	}
}

//void BaseData::truncateAfter(double endTime)
//{
//	if (endTime < m_dates[0])
//	{
//		this->clear();
//	}
//	else
//	{
//		auto upper = std::upper_bound(m_dates.begin(), m_dates.end(), endTime);
//		if (upper != m_dates.end())
//		{
//			size_t index = upper - m_dates.begin();
//
//			m_opens.resize(index);
//			m_highs.resize(index);
//			m_lows.resize(index);
//			m_closes.resize(index);
//			m_volumns.resize(index);
//			m_dates.resize(index);
//
//			m_isTailTruncated = true;
//		}
//	}
//}
//bool BaseData::isTailTruncated()
//{
//	return m_isTailTruncated;
//}
bool BaseData::isValid()
{
	bool valid = m_opens.size() == m_highs.size()
		&& m_highs.size() == m_lows.size()
		&& m_lows.size() == m_closes.size()
		&& m_closes.size() == m_volumns.size()
		&& m_volumns.size() == m_dates.size();
	if (valid)
	{
		valid = valid && m_beginIndex <= m_endIndex;
	}
	if (valid)
	{
		valid = valid && m_endIndex < m_opens.size();
	}
	return valid;
}

