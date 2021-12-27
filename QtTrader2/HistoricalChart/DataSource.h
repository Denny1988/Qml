#pragma once
#include <QObject>
#include "Common.h"
#include <vector>
#include "Instrument.h"
#include <QList>
#include <QUuid>
using namespace std;

namespace HistoricalChartSpace
{
	class HISTORICALCHART_EXPORT DataSource : public QObject
	{
		Q_OBJECT

		signals :
		void RealTimeDataComeSignal(const PrimRealTimeData&);
	
	public:
		static int uniqueCallId;

	public:
		DataSource(QObject* parent = 0);
		virtual ~DataSource();
	};
}
