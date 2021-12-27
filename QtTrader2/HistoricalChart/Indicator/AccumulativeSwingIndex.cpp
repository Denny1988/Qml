#include "AccumulativeSwingIndex.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	AccumulativeSwingIndex::AccumulativeSwingIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector <double>;
	}

	AccumulativeSwingIndex::~AccumulativeSwingIndex()
	{
	}

	IndicatorSettings& AccumulativeSwingIndex::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData["Limit Move Value"] = 12;
		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::green, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void AccumulativeSwingIndex::calculate(int beginIndex, int endIndex)
	{
		int dataSize = extendDataBuffer();
		int period = 1;
		const char* name = "ASI";
		double limitMoveValue = m_settings.m_hashInputIntData["Limit Move Value"];
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period, "OHLC"));
		unique_ptr<CIndex> ta(new CIndex());
		unique_ptr<CRecordset> result(ta->AccumulativeSwingIndex(database.get(), database->getRecordset(), limitMoveValue, name));

		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			vectOutReal[dataIndex] = result->getValue(name, bufferIndex + 1);
		}
	}
}
