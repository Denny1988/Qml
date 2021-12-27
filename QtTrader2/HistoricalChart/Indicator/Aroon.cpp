#include "Aroon.h"

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
	Aroon::Aroon(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::AroonUp] = new vector < double > ;
		m_indicatorDatas[SC::AroonDown] = new vector < double > ;
	}

	Aroon::~Aroon()
	{
	}

	IndicatorSettings& Aroon::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData[SC::Period] = 14;
		settings.m_arrayNum = 2;
		settings.m_plotNum = 2;

		IndicatorLineSettings lineSettings = { SC::AroonUp, Qt::red, 1, Qt::SolidLine, enIndicatorDrawLine };
		settings.m_vectLineSettings.push_back(lineSettings);

		IndicatorLineSettings lineSettings2 = { SC::AroonDown, Qt::green, 1, Qt::SolidLine, enIndicatorDrawLine };
		settings.m_vectLineSettings.push_back(lineSettings2);
		return settings;
	}

	void Aroon::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData[SC::Period];
		if (period <= 0) period = 1;
		if (endIndex < period) return;

		// 1. extend buffer
		int dataSize = extendDataBuffer();

#ifdef TA_Lib
		//TA_RetCode TA_AROON( int    startIdx,
		// int    endIdx,
		// const double inHigh[],
		// const double inLow[],
		// int           optInTimePeriod, /* From 2 to 100000 */
		// int          *outBegIdx,
		// int          *outNBElement,
		// double        outAroonDown[],
		// double        outAroonUp[] );

		int outBegin = 0;
		int outNum = 0;
		// 2. new calculate buffer and calculate
		double* aroonDown = new double[dataSize];
		double* aroonUp = new double[dataSize];

		TA_RetCode retCode = TA_AROON(beginIndex, endIndex, &m_baseData.getHighs()[0], &m_baseData.getLows()[0], period, &outBegin, &outNum, aroonDown, aroonUp);

		// 3. update indicator data
		vector<double>* vectAroonDown = m_indicatorDatas[SC::AroonDown];
		vector<double>* vectAroonUp = m_indicatorDatas[SC::AroonUp];
		for (int bufferIndex = 0; bufferIndex < outNum; bufferIndex++)
		{
			(*vectAroonDown)[bufferIndex + outBegin] = aroonDown[bufferIndex];
			(*vectAroonUp)[bufferIndex + outBegin] = aroonUp[bufferIndex];
		}

		// 4. cleaning
		delete[] aroonDown;
		delete[] aroonUp;
#else
		/************************************************************************/
		/* Use TA-SDK                                                           */
		/************************************************************************/

		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period, "HL"));
		unique_ptr<COscillator> ta(new COscillator());
		unique_ptr<CRecordset> result(ta->Aroon(database.get(), database->getRecordset(), period));

		vector<double>& vectAroonUp = *m_indicatorDatas[SC::AroonUp];
		vector<double>& vectAroonDown = *m_indicatorDatas[SC::AroonDown];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			if (dataIndex < period)
			{
				vectAroonUp[dataIndex] = numeric_limits<double>::quiet_NaN();
				vectAroonDown[dataIndex] = numeric_limits<double>::quiet_NaN();
			}
            else if (bufferIndex >= period) // bufferIndex >= period Only when there is data.
			{
				vectAroonUp[dataIndex] = result->getValue("Aroon Up", bufferIndex + 1);
				vectAroonDown[dataIndex] = result->getValue("Aroon Down", bufferIndex + 1);
			}
		}
#endif
	}
}
