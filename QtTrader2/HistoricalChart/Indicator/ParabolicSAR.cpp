#include "ParabolicSAR.h"

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
	ParabolicSAR::ParabolicSAR(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	ParabolicSAR::~ParabolicSAR()
	{
	}

	IndicatorSettings& ParabolicSAR::initSettings(IndicatorSettings& settings) const
	{
		settings.m_isInPriceChart = true;
		settings.m_hashInputDoubleData["Acceleration"] = 0.02;
		settings.m_hashInputDoubleData["Maximum"] = 0.2;

		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::blue, 1, Qt::SolidLine, enIndicatorDrawDot };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void ParabolicSAR::calculate(int beginIndex, int endIndex)
	{
		// 1. extend buffer and input arguments
		int dataSize = extendDataBuffer();
		double acceleration = m_settings.m_hashInputDoubleData["Acceleration"]; // acceleration factor
		double maxAF = m_settings.m_hashInputDoubleData["Maximum"];
#ifdef TA_Lib
		//TA_RetCode TA_SAR( int    startIdx,
		// int    endIdx,
		// const double inHigh[],
		// const double inLow[],
		// double        optInAcceleration, /* From 0 to TA_REAL_MAX */
		// double        optInMaximum, /* From 0 to TA_REAL_MAX */
		// int          *outBegIdx,
		// int          *outNBElement,
		// double        outReal[] );

		int outBegin = 0;
		int outNum = 0;
		// 2. new calculate buffer and calculate
		double* outReal = new double[dataSize];
		TA_RetCode retCode = TA_SAR(beginIndex, endIndex, &m_baseData.getHighs()[0], &m_baseData.getLows()[0], acceleration, maxAF, &outBegin, &outNum, outReal);

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

		int period = 2;
		const char* name = "SAR";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period, "HL"));
		unique_ptr<COscillator> ta(new COscillator());
		unique_ptr<CRecordset> result(ta->ParabolicSAR(database.get(), database->getRecordset()->getField(SC::High), database->getRecordset()->getField(SC::Low), acceleration, maxAF, name));

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