#include "TradeVolumeIndex.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	TradeVolumeIndex::TradeVolumeIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	TradeVolumeIndex::~TradeVolumeIndex()
	{
	}

	IndicatorSettings& TradeVolumeIndex::initSettings(IndicatorSettings& settings) const
	{
		settings.m_dependency = enCPDClose;
		settings.m_hashInputDoubleData["Min Tick Val"] = 0.25;

		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::green, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void TradeVolumeIndex::calculate(int beginIndex, int endIndex)
	{
		extendDataBuffer();
		double minTickVal = m_settings.m_hashInputDoubleData["Min Tick Val"];

		const char* name = "TVI";
		int bufferSize = endIndex - beginIndex + 1;
		unique_ptr<double> volume(new double[bufferSize]);
		unique_ptr<CField> volumes(new CField(bufferSize, "Volume"));
		for (int i = 0; i < bufferSize; i++)
		{
			volumes->setValue(i, m_baseData.getVolumes()[beginIndex + i]);
		}

		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, 1));
		unique_ptr<CIndex> ta(new CIndex());
		unique_ptr<CRecordset> result(ta->TradeVolumeIndex(database.get(), m_inputSeries, volumes.get(), minTickVal, name));

		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			vectOutReal[dataIndex] = result->getValue(name, bufferIndex + 1);
		}
	}
}
