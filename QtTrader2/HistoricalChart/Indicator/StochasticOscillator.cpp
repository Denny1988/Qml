#include "StochasticOscillator.h"

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
	StochasticOscillator::StochasticOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas["K"] = new vector < double >;
		m_indicatorDatas["D"] = new vector < double >;
		m_indicatorDatas["J"] = new vector < double >;
	}

	StochasticOscillator::~StochasticOscillator()
	{
	}

	IndicatorSettings& StochasticOscillator::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData["%K Fast"] = 9;
		settings.m_hashInputIntData["%K Slow"] = 3;
		settings.m_hashInputIntData["%D"] = 9;
		settings.m_maType = LocalMAType_SMA;
		settings.m_arrayNum = 3;
		settings.m_plotNum = 3;

		IndicatorLineSettings lineSettings = { "K", Qt::blue, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);

		IndicatorLineSettings lineSettings2 = { "D", Qt::green, 1, Qt::DotLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings2);

		IndicatorLineSettings lineSettings3 = { "J", Qt::darkMagenta, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings3);
		return settings;
	}

	void StochasticOscillator::calculate(int beginIndex, int endIndex)
	{
		// 1. extend buffer and prepare input arguments
		int dataSize = extendDataBuffer();
		beginIndex = 0;
		endIndex = dataSize - 1;

		int fastKperiod = m_settings.m_hashInputIntData["%K Fast"];
		int slowKperiod = m_settings.m_hashInputIntData["%K Slow"];
		int Dperiod = m_settings.m_hashInputIntData["%D"];
		int maxPeriod = fastKperiod > slowKperiod ? fastKperiod : slowKperiod;
		if (maxPeriod < Dperiod) maxPeriod = Dperiod;
#ifdef TA_Lib
		//TA_RetCode TA_STOCH( int    startIdx,
		// int    endIdx,
		// const double inHigh[],
		// const double inLow[],
		// const double inClose[],
		// int           optInFastK_Period, /* From 1 to 100000 */
		// int           optInSlowK_Period, /* From 1 to 100000 */
		// TA_MAType     optInSlowK_MAType,
		// int           optInSlowD_Period, /* From 1 to 100000 */
		// TA_MAType     optInSlowD_MAType,
		// int          *outBegIdx,
		// int          *outNBElement,
		// double        outSlowK[],
		// double        outSlowD[] );
		int outBegin = 0;
		int outNum = 0;
		// 2. new calculate buffer and calculate
		TA_MAType optInMAType = (TA_MAType)m_settings.m_maType;
		double* outSlowK = new double[dataSize];
		double* outSlowD = new double[dataSize];
		TA_RetCode retCode = TA_STOCH(beginIndex, endIndex, &m_baseData.getHighs()[0], &m_baseData.getLows()[0], &m_baseData.getCloses()[0],
			fastKperiod, slowKperiod, optInMAType, Dperiod, optInMAType,
			&outBegin, &outNum, outSlowK, outSlowD);

		// 3. update indicator data
		vector<double>* dataK = m_indicatorDatas["K"];
		vector<double>* dataD = m_indicatorDatas["D"];
		for (int i = 0; i < outNum; i++)
		{
			(*dataK)[i + outBegin] = outSlowK[i];
			(*dataD)[i + outBegin] = outSlowD[i];
		}

		// 4. cleaning
		delete[] outSlowK;
		delete[] outSlowD;
#else
		/************************************************************************/
		/* Use TA-SDK                                                           */
		/************************************************************************/

		const char* name = "KD";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, maxPeriod, "HLC"));
		unique_ptr<COscillator> ta(new COscillator());
		unique_ptr<CRecordset> result(ta->StochasticOscillator(database.get(), database->getRecordset(), fastKperiod, slowKperiod, Dperiod, m_settings.m_maType));

		vector<double>& dataK = *m_indicatorDatas["K"];
		vector<double>& dataD = *m_indicatorDatas["D"];
		vector<double>& dataJ = *m_indicatorDatas["J"];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			bool hasK, hasD;
			hasK = hasD = false;
			int dataIndex = m_dataStartIndex + bufferIndex;
			if (dataIndex < fastKperiod)
			{
				dataK[dataIndex] = numeric_limits<double>::quiet_NaN();
			}
			else if (bufferIndex >= fastKperiod)
			{
				dataK[dataIndex] = result->getValue("%K", bufferIndex + 1);
				hasK = true;
			}

			if (dataIndex < Dperiod)
			{
				dataD[dataIndex] = numeric_limits<double>::quiet_NaN();
			}
			else if (bufferIndex >= Dperiod)
			{
				dataD[dataIndex] = result->getValue("%D", bufferIndex + 1);
				hasD = true;
			}

			if (hasK && hasD)
			{
				dataJ[dataIndex] = 3 * dataK[dataIndex] - 2 * dataD[dataIndex];
			}
		}
#endif
	}
}