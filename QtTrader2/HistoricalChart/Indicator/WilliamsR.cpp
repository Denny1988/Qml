#include "WilliamsR.h"

#ifdef TA_Lib
#include "ta_libc.h"
#ifndef NDEBUG
#pragma  comment(lib,"ta_libc_cdd.lib")
#else
#pragma  comment(lib,"ta_libc_cmr.lib")
#endif
#else
#include "TASDK.h"
#endif

namespace HistoricalChartSpace
{
	WilliamsR::WilliamsR(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	WilliamsR::~WilliamsR()
	{
	}

	IndicatorSettings& WilliamsR::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData[SC::Period] = 7;
		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::blue, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}
	void WilliamsR::calculate(int beginIndex, int endIndex)
	{
		// 1. extend buffer
		int dataSize = extendDataBuffer();
		int period = m_settings.m_hashInputIntData[SC::Period];
#ifdef TA_Lib
		//TA_RetCode TA_WILLR( int    startIdx,
		//	int    endIdx,
		//	const double inHigh[],
		//	const double inLow[],
		//	const double inClose[],
		//	int           optInTimePeriod, /* From 2 to 100000 */
		//	int          *outBegIdx,
		//	int          *outNBElement,
		//	double        outReal[] );

		int outBegin = 0;
		int outNum = 0;

		// 2. new calculate buffer and calculate
		unique_ptr<double> outReal(new double[dataSize]);
		TA_RetCode retCode = TA_WILLR(beginIndex, endIndex, &m_baseData.getHighs()[0], &m_baseData.getLows()[0], &m_baseData.getCloses()[0], period, &outBegin, &outNum, outReal.get());

		// 3. update indicator data
		vector<double>* vectOutReal = m_indicatorDatas[SC::OutReal];
		for (int i = 0; i < outNum; i++)
		{
			(*vectOutReal)[i + outBegin] = outReal.get()[i];
		}
#else
		/************************************************************************/
		/* Use TA-SDK                                                           */
		/************************************************************************/

		const char* name = "WR";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period, "HLC"));
		unique_ptr<COscillator> ta(new COscillator());
		unique_ptr<CRecordset> result(ta->WilliamsPctR(database.get(), database->getRecordset(), period, name));

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
#endif
	}
}