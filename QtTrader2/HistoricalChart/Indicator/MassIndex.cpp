#include "MassIndex.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	MassIndex::MassIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	MassIndex::~MassIndex()
	{
	}

	IndicatorSettings& MassIndex::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData[SC::Period] = 8;

		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::green, 1, Qt::SolidLine, enIndicatorDrawLine };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void MassIndex::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData[SC::Period];
		int validPeriod = period * 3;
		if (endIndex < validPeriod) return;

		extendDataBuffer();
		const char* name = "MI";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, validPeriod, "HL"));
		unique_ptr<CIndex> ta(new CIndex());
		unique_ptr<CRecordset> result(ta->MassIndex(database.get(), database->getRecordset(), period, name));

		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			if (dataIndex < validPeriod)
			{
				vectOutReal[dataIndex] = numeric_limits<double>::quiet_NaN();
			}
			else if (bufferIndex >= validPeriod)
			{
				vectOutReal[dataIndex] = result->getValue(name, bufferIndex + 1);
			}
		}
	}
}