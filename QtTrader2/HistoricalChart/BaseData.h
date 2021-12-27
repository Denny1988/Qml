#pragma  once
#include <vector>
#include "Common.h"
#include <QMutex>
#include <math.h>
using namespace std;

namespace HistoricalChartSpace
{
	class HISTORICALCHART_EXPORT BaseData
	{
	public:
		BaseData();
		~BaseData();
		
		QMutex m_mutex;

		int size() const;
		void appendData(const PrimHistoricalData& data);
		void insertData(const PrimHistoricalData& data);
		void clear();

		double getDate(int index) const;
		int timeToIndex(double time) const;
		void getBidAsk(double &bid, double &ask) const;

		const vector<double>& getOpens() const;
		const vector<double>& getHighs() const;
		const vector<double>& getLows() const;
		const vector<double>& getCloses() const;
		const vector<double>& getVolumes() const;
		const vector<double>& getDates() const;

		void update(const int index, const PrimHistoricalData& data);
		void realTimeDataCome(const PrimRealTimeData& data, double timeSpan);
		void truncateBefore(double beginTime);
		//void truncateAfter(double endTime);
		//bool isTailTruncated();
		size_t getRangeDataCount(double beginTime, double endTime);
		void joinDatasAtFront(vector<PrimHistoricalData>* datas);
		void joinDatasAtEnd(vector<PrimHistoricalData>* datas);

		// basic paint parameters:
		int getBeginIndex(){ return m_beginIndex; }
		void setBeginIndex(int index)
		{
			m_beginIndex = index;
		}

		int getEndIndex(){ return m_endIndex; }
		void setEndIndex(int index)
		{
			int maxIndex = this->size() - 1;
			m_endIndex = index > maxIndex ? maxIndex : index;
		}

		double getBarDistance(){ return m_barDistance; }
		void setBarDistance(double barDistance)
		{
			m_barDistance = barDistance;
		}

		bool isValid();

	private:
		void truncateBefore(vector<double>& vect, int index);

	private:
		vector<double> m_opens;
		vector<double> m_highs;
		vector<double> m_lows;
		vector<double> m_closes;
		vector<double> m_volumns;
		vector<double> m_dates;
		double m_realTimeAsk;

		int m_beginIndex;
		int m_endIndex;
		double m_barDistance;
		bool m_isTailTruncated;
	};
}
