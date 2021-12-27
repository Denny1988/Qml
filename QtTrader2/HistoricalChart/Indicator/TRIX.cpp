#include "TRIX.h"

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
	TRIX::TRIX(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	TRIX::~TRIX()
	{
	}

	IndicatorSettings& TRIX::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData[SC::Period] = 7;
		settings.m_dependency = enCPDClose;

		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::blue, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void TRIX::calculate(int beginIndex, int endIndex)
	{
		// 1. extend buffer
		int dataSize = extendDataBuffer();
		int period = m_settings.m_hashInputIntData[SC::Period];
		if (period * 4 > dataSize) return;

#ifdef TA_Lib
		//TA_RetCode TA_TRIX( int    startIdx,
		//	int    endIdx,
		//	const double inReal[],
		//	int           optInTimePeriod, /* From 1 to 100000 */
		//	int          *outBegIdx,
		//	int          *outNBElement,
		//	double        outReal[] );
		unique_ptr<double> inputData(new double[dataSize]);
		GetCPDA(m_settings.m_dependency, &m_baseData.getOpens()[0], &m_baseData.getCloses()[0], &m_baseData.getHighs()[0], &m_baseData.getLows()[0], inputData.get(), dataSize);

		int outBegin = 0;
		int outNum = 0;

		// 2. new calculate buffer and calculate
		double* outReal = new double[dataSize];
		TA_RetCode retCode = TA_TRIX(beginIndex, endIndex, inputData.get(), period, &outBegin, &outNum, outReal);

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

		const char* name = "TRIX";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period * 4));
		unique_ptr<COscillator> ta(new COscillator());
		unique_ptr<CRecordset> result(ta->TRIX(database.get(), m_inputSeries, period, name));

		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			if (dataIndex < period * 4)
			{
				vectOutReal[dataIndex] = numeric_limits<double>::quiet_NaN();
			}
			else if (bufferIndex >= period * 4)
			{
				vectOutReal[dataIndex] = result->getValue(name, bufferIndex + 1);
			}
		}
#endif
	}
}