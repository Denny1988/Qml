#include "DataSource.h"
#include "historicalchart.h"
#include "RealTimeChart.h"

namespace HistoricalChartSpace
{
	int DataSource::uniqueCallId = 1;

	DataSource::DataSource(QObject* parent) : QObject(parent)
	{
	}

	DataSource::~DataSource()
	{
	}
}


#include "moc_DataSource.cpp"
