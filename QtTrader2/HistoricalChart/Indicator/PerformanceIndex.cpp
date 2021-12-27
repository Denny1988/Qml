#include "PerformanceIndex.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	PerformanceIndex::PerformanceIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	PerformanceIndex::~PerformanceIndex()
	{
	}

	IndicatorSettings& PerformanceIndex::initSettings(IndicatorSettings& settings) const
	{
		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;
		settings.m_dependency = enCPDClose;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::blue, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void PerformanceIndex::calculate(int beginIndex, int endIndex)
	{
		extendDataBuffer();

		const char* name = "PI";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, 1));
		unique_ptr<CIndex> ta(new CIndex());
		unique_ptr<CRecordset> result(ta->Performance(database.get(), m_inputSeries, name));

		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			vectOutReal[dataIndex] = result->getValue(name, bufferIndex + 1);
		}
	}
}