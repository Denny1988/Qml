#include "BollingerBands.h"

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
	BollingerBands::BollingerBands(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas["UpperBand"] = new vector<double>();
		m_indicatorDatas["MiddleBand"] = new vector<double>();
		m_indicatorDatas["LowerBand"] = new vector<double>();

		if (m_settings.m_maType < LocalMAType_SMA || m_settings.m_maType >= LocalMAType_Unknown)
		{
			m_settings.m_maType = LocalMAType_SMA;
		}
	}

	BollingerBands::~BollingerBands()
	{
	}

	IndicatorSettings& BollingerBands::initSettings(IndicatorSettings& settings) const
	{
		settings.m_isInPriceChart = true;
		settings.m_hashInputIntData[SC::Period] = 20;
		settings.m_hashInputDoubleData["Standard Dev"] = 2.0;
		settings.m_maType = LocalMAType_SMA;
		settings.m_dependency = enCPDClose;

		settings.m_arrayNum = 3;
		settings.m_plotNum = 3;

		IndicatorLineSettings lineSettings = { "UpperBand", Qt::red, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);

		IndicatorLineSettings lineSettings2 = { "MiddleBand", Qt::yellow, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings2);

		IndicatorLineSettings lineSettings3 = { "LowerBand", Qt::green, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings3);
		return settings;
	}

	void BollingerBands::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData[SC::Period];
		if (endIndex < period) return;

		// 1. extend buffer and prepare input datas
		int dataSize = extendDataBuffer();
		double standardDev = m_settings.m_hashInputDoubleData["Standard Dev"];
#ifdef TA_Lib
		unique_ptr<double> inputData(new double[dataSize]);
		GetCPDA(m_settings.m_dependency, &m_baseData.getOpens()[0], &m_baseData.getCloses()[0], &m_baseData.getHighs()[0], &m_baseData.getLows()[0], inputData.get(), dataSize);
		// 2. new calculate buffer and calculate
		double* upperBand = new double[dataSize];
		double* middleBand = new double[dataSize];
		double* lowerBand = new double[dataSize];

		//TA_RetCode TA_BBANDS( int    startIdx,
		// int    endIdx,
		// const double inReal[],
		// int           optInTimePeriod, /* From 2 to 100000 */
		// double        optInNbDevUp, /* From TA_REAL_MIN to TA_REAL_MAX */
		// double        optInNbDevDn, /* From TA_REAL_MIN to TA_REAL_MAX */
		// TA_MAType     optInMAType,
		// int          *outBegIdx,
		// int          *outNBElement,
		// double        outRealUpperBand[],
		// double        outRealMiddleBand[],
		// double        outRealLowerBand[] );

		int outBegin = 0;
		int outNum = 0;
		TA_MAType maType = (TA_MAType)m_settings.m_maType;
		TA_RetCode retCode = TA_BBANDS(beginIndex, endIndex, inputData.get(), period, standardDev, standardDev, maType,
			&outBegin, &outNum, upperBand, middleBand, lowerBand);

		// 3. update indicator data
		vector<double>* vectUpperBand = m_indicatorDatas["UpperBand"];
		vector<double>* vectMiddleBand = m_indicatorDatas["MiddleBand"];
		vector<double>* vectLowerBand = m_indicatorDatas["LowerBand"];
		for (int i = 0; i < outNum; i++)
		{
			(*vectUpperBand)[i + outBegin] = upperBand[i];
			(*vectMiddleBand)[i + outBegin] = middleBand[i];
			(*vectLowerBand)[i + outBegin] = lowerBand[i];
		}

		// 4. cleaning
		delete[] upperBand;
		delete[] middleBand;
		delete[] lowerBand;
#else
		/************************************************************************/
		/* Use TA-SDK                                                           */
		/************************************************************************/

		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period));
		unique_ptr<CBands> ta(new CBands());
		unique_ptr<CRecordset> result(ta->BollingerBands(database.get(), m_inputSeries, period, standardDev, m_settings.m_maType));

		vector<double>& vectUpperBand = *m_indicatorDatas["UpperBand"];
		vector<double>& vectMiddleBand = *m_indicatorDatas["MiddleBand"];
		vector<double>& vectLowerBand = *m_indicatorDatas["LowerBand"];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			if (dataIndex < period)
			{
				vectUpperBand[dataIndex] = numeric_limits<double>::quiet_NaN();
				vectMiddleBand[dataIndex] = numeric_limits<double>::quiet_NaN();
				vectLowerBand[dataIndex] = numeric_limits<double>::quiet_NaN();
			}
			else if (bufferIndex >= period)
			{
				vectUpperBand[dataIndex] = result->getValue("Bollinger Band Top", bufferIndex + 1);
				vectMiddleBand[dataIndex] = result->getValue("Bollinger Band Median", bufferIndex + 1);
				vectLowerBand[dataIndex] = result->getValue("Bollinger Band Bottom", bufferIndex + 1);
			}
		}
#endif
	}

	void BollingerBands::getParameterRange(const QString& key, double& min, double& max, double& decimals)
	{
		//"Standard Dev"
		min = 0.01;
		max = 100;
		decimals = 2;
	}

}