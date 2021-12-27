#include "NegativeVolumeIndex.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	NegativeVolumeIndex::NegativeVolumeIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	NegativeVolumeIndex::~NegativeVolumeIndex()
	{
	}

	IndicatorSettings& NegativeVolumeIndex::initSettings(IndicatorSettings& settings) const
	{
		settings.m_dependency = enCPDClose;

		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::green, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void NegativeVolumeIndex::calculate(int beginIndex, int endIndex)
	{
		extendDataBuffer();

		const char* name = "NVI";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, 1));
		
		unique_ptr<CField> volumes(new CField(m_inputDataBufferSize, "Volume"));
		for (int i = 0; i < m_inputDataBufferSize; i++)
		{
			volumes->setValue(i + 1, m_baseData.getVolumes()[m_dataStartIndex + i]);  // TASDK use 1 based array.
		}

		unique_ptr<CIndex> ta(new CIndex());
		unique_ptr<CRecordset> result(ta->NegativeVolumeIndex(database.get(), m_inputSeries, volumes.get(), name));

		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			vectOutReal[dataIndex] = result->getValue(name, bufferIndex + 1);
		}
	}
}
