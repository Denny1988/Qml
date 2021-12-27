#include "MoneyFlowIndex.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	MoneyFlowIndex::MoneyFlowIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	MoneyFlowIndex::~MoneyFlowIndex()
	{
	}

	IndicatorSettings& MoneyFlowIndex::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData[SC::Period] = 14;

		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::blue, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void MoneyFlowIndex::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData[SC::Period];
		if (endIndex < period) return;

		extendDataBuffer();

		const char* name = "MFI";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period, "OHLCV"));
		unique_ptr<CIndex> ta(new CIndex());
		unique_ptr<CRecordset> result(ta->MoneyFlowIndex(database.get(), database->getRecordset(), period, name));

		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			if (dataIndex < period)
			{
				vectOutReal[dataIndex] = numeric_limits<double>::quiet_NaN();
			}
			else if (bufferIndex >= period)
			{
				vectOutReal[dataIndex] = result->getValue(name, bufferIndex + 1);
			}
		}
	}
}