#include "VerticalHorizontalFilter.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	VerticalHorizontalFilter::VerticalHorizontalFilter(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	VerticalHorizontalFilter::~VerticalHorizontalFilter()
	{
	}

	IndicatorSettings& VerticalHorizontalFilter::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData[SC::Period] = 14;
		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;
		settings.m_dependency = enCPDClose;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::green, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void VerticalHorizontalFilter::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData[SC::Period];
		if (endIndex < period) return;

		extendDataBuffer();
		const char* name = "VHF";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period));
		unique_ptr<COscillator> ta(new COscillator());
		unique_ptr<CRecordset> result(ta->VerticalHorizontalFilter(database.get(), m_inputSeries, period, name));

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