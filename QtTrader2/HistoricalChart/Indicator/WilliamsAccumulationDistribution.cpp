#include "WilliamsAccumulationDistribution.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	WilliamsAccumulationDistribution::WilliamsAccumulationDistribution(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	WilliamsAccumulationDistribution::~WilliamsAccumulationDistribution()
	{
	}

	IndicatorSettings& WilliamsAccumulationDistribution::initSettings(IndicatorSettings& settings) const
	{
		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::green, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void WilliamsAccumulationDistribution::calculate(int beginIndex, int endIndex)
	{
		extendDataBuffer();

		const char* name = "WAD";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, 1, "HLC"));
		unique_ptr<COscillator> ta(new COscillator());
		unique_ptr<CRecordset> result(ta->WilliamsAccumulationDistribution(database.get(), database->getRecordset(), name));

		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			vectOutReal[dataIndex] = result->getValue(name, bufferIndex + 1);
		}
	}
}