#include "DetrendedPriceOscillator.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{

	DetrendedPriceOscillator::DetrendedPriceOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	DetrendedPriceOscillator::~DetrendedPriceOscillator()
	{
	}

	IndicatorSettings& DetrendedPriceOscillator::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData[SC::Period] = 14;

		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;
		settings.m_dependency = enCPDClose;
		settings.m_maType = LocalMAType_SMA;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::blue, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void DetrendedPriceOscillator::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData[SC::Period];
		if (endIndex < period) return;

		extendDataBuffer();
		const char* name = "DPO";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period * 2));
		unique_ptr<COscillator> ta(new COscillator());
		unique_ptr<CRecordset> result(ta->DetrendedPriceOscillator(database.get(), m_inputSeries, period, m_settings.m_maType, name));

		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			if (dataIndex < period * 2)
			{
				vectOutReal[dataIndex] = numeric_limits<double>::quiet_NaN();
			}
			else if (bufferIndex >= period * 2)
			{
				vectOutReal[dataIndex] = result->getValue(name, bufferIndex + 1);
			}
		}
	}

}