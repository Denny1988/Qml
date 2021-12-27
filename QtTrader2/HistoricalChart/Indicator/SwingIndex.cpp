#include "SwingIndex.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	SwingIndex::SwingIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	SwingIndex::~SwingIndex()
	{
	}

	IndicatorSettings& SwingIndex::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputDoubleData["Limit Move Value"] = 12;
		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::green, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void SwingIndex::calculate(int beginIndex, int endIndex)
	{
		extendDataBuffer();
		double limitMoveValue = m_settings.m_hashInputDoubleData["Limit Move Value"];
		const char* name = "SW";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, 1, "OHLC"));
		unique_ptr<CIndex> ta(new CIndex());
		unique_ptr<CRecordset> result(ta->SwingIndex(database.get(), database->getRecordset(), limitMoveValue, name));

		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			vectOutReal[dataIndex] = result->getValue(name, bufferIndex + 1);
		}
	}
}