#include "FractalChaosBands.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	FractalChaosBands::FractalChaosBands(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::FractalHigh] = new vector < double > ;
		m_indicatorDatas[SC::FractalLow] = new vector < double >;
	}

	FractalChaosBands::~FractalChaosBands()
	{
	}

	IndicatorSettings& FractalChaosBands::initSettings(IndicatorSettings& settings) const
	{
		settings.m_isInPriceChart = true;
		settings.m_hashInputIntData[SC::Period] = 14;
		settings.m_arrayNum = 2;
		settings.m_plotNum = 2;

		IndicatorLineSettings lineSettings = { SC::FractalHigh, Qt::green, 1, Qt::SolidLine, enIndicatorDrawLine };
		settings.m_vectLineSettings.push_back(lineSettings);

		IndicatorLineSettings lineSettings2 = { SC::FractalLow, Qt::red, 1, Qt::SolidLine, enIndicatorDrawLine };
		settings.m_vectLineSettings.push_back(lineSettings2);
		return settings;
	}

	void FractalChaosBands::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData[SC::Period];
		if (endIndex < period) return;

		extendDataBuffer();

		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period, "HL"));
		unique_ptr<CBands> ta(new CBands());
		unique_ptr<CRecordset> result(ta->FractalChaosBands(database.get(), database->getRecordset(), period));

		vector<double>& vectFractalHigh = *m_indicatorDatas[SC::FractalHigh];
		vector<double>& vectFractalLow = *m_indicatorDatas[SC::FractalLow];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			if (dataIndex < period)
			{
				vectFractalHigh[dataIndex] = numeric_limits<double>::quiet_NaN();
				vectFractalLow[dataIndex] = numeric_limits<double>::quiet_NaN();
			}
			else if (bufferIndex >= period)
			{
				vectFractalHigh[dataIndex] = result->getValue(SC::FractalHigh, bufferIndex + 1);
				vectFractalLow[dataIndex] = result->getValue(SC::FractalLow, bufferIndex + 1);
			}
		}
	}
}