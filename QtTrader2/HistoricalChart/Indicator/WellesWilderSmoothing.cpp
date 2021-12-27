#include "WellesWilderSmoothing.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	WellesWilderSmoothing::WellesWilderSmoothing(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	WellesWilderSmoothing::~WellesWilderSmoothing()
	{
	}

	IndicatorSettings& WellesWilderSmoothing::initSettings(IndicatorSettings& settings) const
	{
		settings.m_isInPriceChart = true;
		settings.m_hashInputIntData[SC::Period] = 14;

		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;
		settings.m_dependency = enCPDClose;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::green, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void WellesWilderSmoothing::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData[SC::Period];
		int extPeriod = period * 2;
		if (endIndex < extPeriod) return;

		int dataSize = extendDataBuffer();
        // The value of this indicator at the initial stage is too small (the same is true for silverlight graphics, which may be a bug), so the indicator must be calculated from 0.
		beginIndex = 0;
		endIndex = dataSize - 1;

		const char* name = "WWS";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, extPeriod));
		unique_ptr<CMovingAverage> ta(new CMovingAverage());
		unique_ptr<CRecordset> result(ta->WellesWilderSmoothing(m_inputSeries, period, name));

		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			if (dataIndex < extPeriod)
			{
				vectOutReal[dataIndex] = numeric_limits<double>::quiet_NaN();
			}
			else if (bufferIndex >= extPeriod)
			{
				vectOutReal[dataIndex] = result->getValue(name, bufferIndex + 1);
			}
		}
	}
}
