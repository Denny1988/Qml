#include "VIDYA.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	VIDYA::VIDYA(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	VIDYA::~VIDYA()
	{
	}

	IndicatorSettings& VIDYA::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData[SC::Period] = 7;
		settings.m_hashInputDoubleData["R2 Scale"] = 0.65;
		settings.m_dependency = enCPDClose;

		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::green, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void VIDYA::calculate(int beginIndex, int endIndex)
	{
		int dataSize = extendDataBuffer();
		int period = m_settings.m_hashInputIntData[SC::Period];
		if (period <= 0) period = 1;
		double r2Scale = m_settings.m_hashInputDoubleData["R2 Scale"];

		const char* name = "VIDYA";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period));
		unique_ptr<CMovingAverage> ta(new CMovingAverage());
		unique_ptr<CRecordset> result(ta->VIDYA(database.get(), m_inputSeries, period, r2Scale, name));

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