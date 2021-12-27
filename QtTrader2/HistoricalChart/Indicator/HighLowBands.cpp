#include "HighLowBands.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	HighLowBands::HighLowBands(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::HighLowBandsTop] = new vector < double > ;
		m_indicatorDatas[SC::HighLowBandsMedian] = new vector < double > ;
		m_indicatorDatas[SC::HighLowBandsBottom] = new vector < double > ;
	}

	HighLowBands::~HighLowBands()
	{
	}

	IndicatorSettings& HighLowBands::initSettings(IndicatorSettings& settings) const
	{
		settings.m_isInPriceChart = true;
		settings.m_hashInputIntData[SC::Period] = 14;
		settings.m_arrayNum = 3;
		settings.m_plotNum = 3;

		IndicatorLineSettings lineSettings = { SC::HighLowBandsTop, Qt::green, 1, Qt::SolidLine, enIndicatorDrawLine };
		settings.m_vectLineSettings.push_back(lineSettings);

		IndicatorLineSettings lineSettings2 = { SC::HighLowBandsMedian, Qt::yellow, 1, Qt::SolidLine, enIndicatorDrawLine };
		settings.m_vectLineSettings.push_back(lineSettings2);

		IndicatorLineSettings lineSettings3 = { SC::HighLowBandsBottom, Qt::red, 1, Qt::SolidLine, enIndicatorDrawLine };
		settings.m_vectLineSettings.push_back(lineSettings3);
		return settings;
	}

	void HighLowBands::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData[SC::Period];
		if (endIndex < period) return;

		extendDataBuffer();

		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period, "HLC"));
		unique_ptr<CBands> ta(new CBands());
		unique_ptr<CRecordset> result(ta->HighLowBands(database.get(), 
			database->getRecordset()->getField(SC::High), 
			database->getRecordset()->getField(SC::Low),
			database->getRecordset()->getField(SC::Close), period));

        if(result==NULL || result== nullptr) return;

		vector<double>& vectTop = *m_indicatorDatas[SC::HighLowBandsTop];
		vector<double>& vectMedian = *m_indicatorDatas[SC::HighLowBandsMedian];
		vector<double>& vectLow = *m_indicatorDatas[SC::HighLowBandsBottom];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			if (dataIndex < period)
			{
				vectTop[dataIndex] = numeric_limits<double>::quiet_NaN();
				vectMedian[dataIndex] = numeric_limits<double>::quiet_NaN();
				vectLow[dataIndex] = numeric_limits<double>::quiet_NaN();
			}
			else if (bufferIndex >= period)
			{
				vectTop[dataIndex] = result->getValue(SC::HighLowBandsTop, bufferIndex + 1);
				vectMedian[dataIndex] = result->getValue(SC::HighLowBandsMedian, bufferIndex + 1);
				vectLow[dataIndex] = result->getValue(SC::HighLowBandsBottom, bufferIndex + 1);
			}
		}
	}

    void HighLowBands::getParameterRange(const QString& key, int& min, int& max)
    {
        min = 6;
        max = 1000;
    }

}
