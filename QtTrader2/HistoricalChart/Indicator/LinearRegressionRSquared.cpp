#include "LinearRegressionRSquared.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	LinearRegressionRSquared::LinearRegressionRSquared(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	LinearRegressionRSquared::~LinearRegressionRSquared()
	{
	}

	IndicatorSettings& LinearRegressionRSquared::initSettings(IndicatorSettings& settings) const
	{
		settings.m_dependency = enCPDClose;
		settings.m_hashInputIntData[SC::Period] = 14;
		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::green, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void LinearRegressionRSquared::calculate(int beginIndex, int endIndex)
	{
		int dataSize = extendDataBuffer();
		int period = m_settings.m_hashInputIntData[SC::Period];

		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period));
		unique_ptr<CLinearRegression> ta(new CLinearRegression());
		unique_ptr<CRecordset> result(ta->Regression(database.get(), m_inputSeries, period));

		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			vectOutReal[dataIndex] = result->getValue("RSquared", bufferIndex + 1);
		}
	}
}
