#include "MACD.h"

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
	MACD::MACD(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas["Main"] = new vector < double > ;
		m_indicatorDatas["Signal"] = new vector < double > ;
	}

	MACD::~MACD()
	{
	}

	IndicatorSettings& MACD::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData[SC::FastEMA] = 12;
		settings.m_hashInputIntData[SC::SlowEMA] = 26;
		settings.m_hashInputIntData[SC::MacdSMA] = 9;

		settings.m_arrayNum = 3;
		settings.m_plotNum = 2;
		settings.m_dependency = enCPDClose;

		IndicatorLineSettings lineSettings = { "Main", Qt::yellow, 1.5, Qt::SolidLine, enIndicatorDrawColumn };
		settings.m_vectLineSettings.push_back(lineSettings);

		IndicatorLineSettings lineSettings2 = { "Signal", Qt::red, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings2);
		return settings;
	}

	void MACD::calculate(int beginIndex, int endIndex)
	{
		// 1. extend buffer, prepare parameters and input datas
		int dataSize = extendDataBuffer();
		beginIndex = 0;
		endIndex = dataSize - 1;

		int fastPeriod = m_settings.m_hashInputIntData[SC::FastEMA];
		int slowPeriod = m_settings.m_hashInputIntData[SC::SlowEMA];
		int signalPeriod = m_settings.m_hashInputIntData[SC::MacdSMA];

		int maxPeriod = slowPeriod < fastPeriod ? fastPeriod : slowPeriod;
		if (dataSize < maxPeriod)
		{
			map<QString, vector<double>*>::iterator it;
			for (it = m_indicatorDatas.begin(); it != m_indicatorDatas.end(); it++)
			{
				it->second->clear();
				it->second->resize(dataSize, std::numeric_limits<double>::quiet_NaN());
			}
            return;  // When dataSize <maxPeriod, TASDK calculation will be abnormal, causing the program to crash. Therefore, special treatment is done here.
		}

#ifdef TA_Lib
		//TA_RetCode TA_MACD( int    startIdx,
		// int    endIdx,
		// const double inReal[],
		// int           optInFastPeriod, /* From 2 to 100000 */
		// int           optInSlowPeriod, /* From 2 to 100000 */
		// int           optInSignalPeriod, /* From 1 to 100000 */
		// int          *outBegIdx,
		// int          *outNBElement,
		// double        outMACD[],
		// double        outMACDSignal[],
		// double        outMACDHist[] );
		unique_ptr<double> inputData(new double[dataSize]);
		GetCPDA(m_settings.m_dependency, &m_baseData.getOpens()[0], &m_baseData.getCloses()[0], &m_baseData.getHighs()[0], &m_baseData.getLows()[0], inputData.get(), dataSize);

		int outBegin = 0;
		int outNum = 0;

		// 2. new calculate buffer and calculate
		double* outMACDHist = new double[dataSize];
		double* _main = new double[dataSize];
		double* signal = new double[dataSize];

		TA_RetCode  retCode = TA_MACD(beginIndex, endIndex, inputData.get(), fastPeriod, slowPeriod, signalPeriod, &outBegin, &outNum, _main, signal, outMACDHist);

		// 3. update indicator data
		//if (beginIndex == 0) m_startIndex = newSize - 1;
		if (outNum > 0)
		{
			//if (beginIndex == 0) m_startIndex = outBegin;
			vector<double>* outMACD = m_indicatorDatas["Main"];
			vector<double>* outMACDSignal = m_indicatorDatas["Signal"];
			for (int i = 0; i < outNum; i++)
			{
				(*outMACD)[i + outBegin] = _main[i];
				(*outMACDSignal)[i + outBegin] = signal[i];
			}
		}

		// 4. cleaning
		delete[] outMACDHist;
		delete[] _main;
		delete[] signal;
#else
		/************************************************************************/
		/* Use TA-SDK                                                           */
		/************************************************************************/

		const char* macd = "MACD";
		const char* signal = "Signal";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, slowPeriod));
		unique_ptr<COscillator> ta(new COscillator());
		unique_ptr<CRecordset> result(ta->MACD(database.get(), m_inputSeries, signalPeriod, slowPeriod, fastPeriod, macd));

		vector<double>& outMACD = *m_indicatorDatas["Main"];
		vector<double>& outMACDSignal = *m_indicatorDatas["Signal"];
		CField* macdField = result->getField(macd);
		CField* signalField = result->getField(signal);
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			if (dataIndex < slowPeriod)
			{
				outMACD[dataIndex] = numeric_limits<double>::quiet_NaN();
			}
			else if (bufferIndex >= slowPeriod)
			{
				outMACD[dataIndex] = macdField->getValue(bufferIndex + 1);
			}

			if (dataIndex < slowPeriod + signalPeriod)
			{
				outMACDSignal[dataIndex] = numeric_limits<double>::quiet_NaN();
			}
			else if (bufferIndex >= slowPeriod + signalPeriod)
			{
				outMACDSignal[dataIndex] = signalField->getValue(bufferIndex + 1);
			}
		}
#endif
	}
}
