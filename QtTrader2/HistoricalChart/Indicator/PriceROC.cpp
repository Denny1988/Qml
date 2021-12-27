#include "PriceROC.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	PriceROC::PriceROC(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	PriceROC::~PriceROC()
	{
	}

	IndicatorSettings& PriceROC::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData[SC::Period] = 12;

		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;
		settings.m_dependency = enCPDClose;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::green, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void PriceROC::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData[SC::Period];
		if (endIndex < period) return;

		extendDataBuffer();

		const char* name = "PROC";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period));
		unique_ptr<CGeneral> ta(new CGeneral());
		unique_ptr<CRecordset> result(ta->PriceROC(database.get(), m_inputSeries, period, name));

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