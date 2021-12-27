#include "MovingAverageEnvelope.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	MovingAverageEnvelope::MovingAverageEnvelope(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas["Envelope Top"] = new vector < double > ;
		m_indicatorDatas["Envelope Bottom"] = new vector < double > ;

		if (m_settings.m_maType < LocalMAType_SMA || m_settings.m_maType >= LocalMAType_Unknown)
		{
			m_settings.m_maType = LocalMAType_SMA;
		}
	}

	MovingAverageEnvelope::~MovingAverageEnvelope()
	{
	}

	IndicatorSettings& MovingAverageEnvelope::initSettings(IndicatorSettings& settings) const
	{
		settings.m_isInPriceChart = true;
		settings.m_hashInputIntData[SC::Period] = 14;
		settings.m_hashInputDoubleData["Shift"] = 0.5;
		settings.m_arrayNum = 2;
		settings.m_plotNum = 2;
		settings.m_maType = LocalMAType_SMA;
		settings.m_dependency = enCPDClose;

		IndicatorLineSettings lineSettings = { "Envelope Top", Qt::green, 1, Qt::SolidLine, enIndicatorDrawLine };
		settings.m_vectLineSettings.push_back(lineSettings);

		IndicatorLineSettings lineSettings2 = { "Envelope Bottom", Qt::red, 1, Qt::SolidLine, enIndicatorDrawLine };
		settings.m_vectLineSettings.push_back(lineSettings2);
		return settings;
	}

	void MovingAverageEnvelope::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData[SC::Period];
		if (endIndex < period) return;

		extendDataBuffer();
		double shift = m_settings.m_hashInputDoubleData["Shift"];

		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period));
		unique_ptr<CBands> ta(new CBands());
		unique_ptr<CRecordset> result(ta->MovingAverageEnvelope(database.get(), m_inputSeries, period, m_settings.m_maType, shift));

		vector<double>& vectFractalHigh = *m_indicatorDatas["Envelope Top"];
		vector<double>& vectFractalLow = *m_indicatorDatas["Envelope Bottom"];
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
				vectFractalHigh[dataIndex] = result->getValue("Envelope Top", bufferIndex + 1);
				vectFractalLow[dataIndex] = result->getValue("Envelope Bottom", bufferIndex + 1);
			}
		}
	}
}