#include "RainbowOscillator.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	RainbowOscillator::RainbowOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	RainbowOscillator::~RainbowOscillator()
	{
	}

	IndicatorSettings& RainbowOscillator::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData["Levels"] = 9;
		settings.m_dependency = enCPDClose;
		settings.m_maType = LocalMAType_SMA;

		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::green, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void RainbowOscillator::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData["Levels"];
		if (endIndex < period) return;

		extendDataBuffer();

		const char* name = "RO";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period));
		unique_ptr<COscillator> ta(new COscillator());
		unique_ptr<CRecordset> result(ta->RainbowOscillator(database.get(), m_inputSeries, period, m_settings.m_maType, name));

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