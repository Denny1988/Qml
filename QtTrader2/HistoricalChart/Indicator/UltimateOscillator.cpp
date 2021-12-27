#include "UltimateOscillator.h"

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
	UltimateOscillator::UltimateOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	UltimateOscillator::~UltimateOscillator()
	{
	}

	IndicatorSettings& UltimateOscillator::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData["Period1"] = 7;
		settings.m_hashInputIntData["Period2"] = 14;
		settings.m_hashInputIntData["Period3"] = 28;

		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::blue, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void UltimateOscillator::calculate(int beginIndex, int endIndex)
	{
		// 1. extend buffer and prepare input arguments
		int dataSize = extendDataBuffer();
		int period1 = m_settings.m_hashInputIntData["Period1"];
		int period2 = m_settings.m_hashInputIntData["Period2"];
		int period3 = m_settings.m_hashInputIntData["Period3"];
#ifdef TA_Lib
		//TA_RetCode TA_ULTOSC( int    startIdx,
		//	int    endIdx,
		//	const double inHigh[],
		//	const double inLow[],
		//	const double inClose[],
		//	int           optInTimePeriod1, /* From 1 to 100000 */
		//	int           optInTimePeriod2, /* From 1 to 100000 */
		//	int           optInTimePeriod3, /* From 1 to 100000 */
		//	int          *outBegIdx,
		//	int          *outNBElement,
		//	double        outReal[] );
		int outBegin = 0;
		int outNum = 0;

		// 2. new calculate buffer and calculate
		double* outReal = new double[dataSize];
		TA_RetCode retCode = TA_ULTOSC(beginIndex, endIndex, &m_baseData.getHighs()[0], &m_baseData.getLows()[0], &m_baseData.getCloses()[0], period1, period2, period3, &outBegin, &outNum, outReal);

		// 3. update indicator data
		vector<double>* vectOutReal = m_indicatorDatas[SC::OutReal];
		for (int i = 0; i < outNum; i++)
		{
			(*vectOutReal)[i + outBegin] = outReal[i];
		}

		// 4. cleaning
		delete[] outReal;
#else
		/************************************************************************/
		/* Use TA-SDK                                                           */
		/************************************************************************/

		int maxPeriod = period1 > period2 ? period1 : period2;
		if (maxPeriod < period3) maxPeriod = period3;

		const char* name = "UO";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, maxPeriod, "HLC"));
		unique_ptr<COscillator> ta(new COscillator());
		unique_ptr<CRecordset> result(ta->UltimateOscillator(database.get(), database->getRecordset(), period1, period2, period3, name));

		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			if (dataIndex < maxPeriod)
			{
				vectOutReal[dataIndex] = numeric_limits<double>::quiet_NaN();
			}
			else if (bufferIndex >= maxPeriod)
			{
				vectOutReal[dataIndex] = result->getValue(name, bufferIndex + 1);
			}
		}
#endif
	}
}