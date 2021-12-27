#include "PriceOscillator.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	PriceOscillator::PriceOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	PriceOscillator::~PriceOscillator()
	{
	}

	IndicatorSettings& PriceOscillator::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData["Long Cycle"] = 22;
		settings.m_hashInputIntData["Short Cycle"] = 14;
		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;
		settings.m_maType = LocalMAType_SMA;
		settings.m_dependency = enCPDClose;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::green, 1, Qt::SolidLine, enIndicatorDrawLine };
		settings.m_vectLineSettings.push_back(lineSettings);

		return settings;
	}

	void PriceOscillator::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData["Long Cycle"];
		if (endIndex < period) return;

		extendDataBuffer();
		int shortCycle = m_settings.m_hashInputIntData["Short Cycle"];

		const char* name = "PO";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period));
		unique_ptr<COscillator> ta(new COscillator());
		unique_ptr<CRecordset> result(ta->PriceOscillator(database.get(), m_inputSeries, period, shortCycle, m_settings.m_maType, name));

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