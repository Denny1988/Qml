#include "ChandeMomentumOscillator.h"

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
	ChandeMomentumOscillator::ChandeMomentumOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	ChandeMomentumOscillator::~ChandeMomentumOscillator()
	{
	}

	IndicatorSettings& ChandeMomentumOscillator::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData[SC::Period] = 7;
		settings.m_dependency = enCPDClose;

		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::blue, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void ChandeMomentumOscillator::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData[SC::Period];
		if (endIndex < period) return;

		// 1. extend buffer
		int dataSize = extendDataBuffer();

#ifdef TA_Lib
		//TA_RetCode TA_CMO( int    startIdx,
		// int    endIdx,
		// const double inReal[],
		// int           optInTimePeriod, /* From 2 to 100000 */
		// int          *outBegIdx,
		// int          *outNBElement,
		// double        outReal[] );

		int outBegin = 0;
		int outNum = 0;
		unique_ptr<double> inputData(new double[dataSize]);
		GetCPDA(m_settings.m_dependency, &m_baseData.getOpens()[0], &m_baseData.getCloses()[0], &m_baseData.getHighs()[0], &m_baseData.getLows()[0], inputData.get(), dataSize);

		// 2. new calculate buffer and calculate
		double* outReal = new double[dataSize];
		TA_RetCode retCode = TA_CMO(beginIndex, endIndex, inputData, period, &outBegin, &outNum, outReal);

		// 3. update indicator data
		vector<double>* vectOutReal = m_indicatorDatas[SC::OutReal];
		for (int i = 0; i < outNum; i++)
		{
			(*vectOutReal)[i + outBegin] = outReal[i];
		}

		// 4. cleaning
		delete[] outReal;
		delete[] inputData;
#else
		/************************************************************************/
		/* Use TA-SDK                                                           */
		/************************************************************************/

		const char* name = "CMO";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period));
		unique_ptr<COscillator> ta(new COscillator());
		unique_ptr<CRecordset> result(ta->ChandeMomentumOscillator(database.get(), m_inputSeries, period, name));

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