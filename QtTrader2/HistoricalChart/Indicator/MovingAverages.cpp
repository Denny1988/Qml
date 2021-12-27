#include "MovingAverages.h"

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
	MovingAverages::MovingAverages(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_isMa = true;
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	MovingAverages::~MovingAverages()
	{
	}

	IndicatorSettings& MovingAverages::initSettings(IndicatorSettings& settings) const
	{
		settings.m_isInPriceChart = true;
		settings.m_hashInputIntData[SC::Period] = 7;
		settings.m_dependency = enCPDClose;
		settings.m_maType = LocalMAType_SMA;

		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::red, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	const QString& MovingAverages::getName()
	{
		QString name("MA");
		switch (m_settings.m_maType)
		{
		case HistoricalChartSpace::LocalMAType_EMA:
			name = "EMA";
			break;
		case HistoricalChartSpace::LocalMAType_TSMA:
			name = "TSMA";
			break;
		case HistoricalChartSpace::LocalMAType_VMA:
			name = "VMA";
			break;
		case HistoricalChartSpace::LocalMAType_TRIMA:
			name = "TRIMA";
			break;
		case HistoricalChartSpace::LocalMAType_WMA:
			name = "WMA";
			break;
		}
		m_lineName.clear(); 
		m_lineName.append(name).append(QString::number(m_settings.m_hashInputIntData[SC::Period]));
		return m_lineName;
	}

	void MovingAverages::calculate(int beginIndex, int endIndex)
	{
		// 1. extend buffer
		int dataSize = extendDataBuffer();
		int period = m_settings.m_hashInputIntData[SC::Period];
#ifdef TA_Lib
		//TA_RetCode TA_MA( int    startIdx,
		// int    endIdx,
		// const double inReal[],
		// int           optInTimePeriod, /* From 1 to 100000 */
		// TA_MAType     optInMAType,
		// int          *outBegIdx,
		// int          *outNBElement,
		// double        outReal[] );
		unique_ptr<double> inputData(new double[dataSize]);
		GetCPDA(m_settings.m_dependency, &m_baseData.getOpens()[0], &m_baseData.getCloses()[0], &m_baseData.getHighs()[0], &m_baseData.getLows()[0], inputData.get(), dataSize);
		int outBegin = 0;
		int outNum = 0;
		// 2. new calculate buffer and calculate
		double* outReal = new double[dataSize];
		TA_MAType optInMAType = (TA_MAType)m_settings.m_maType;
		TA_RetCode retCode = TA_MA(beginIndex, endIndex, inputData, period, optInMAType, &outBegin, &outNum, outReal);

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

		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period));
		unique_ptr<CMovingAverage> ta(new CMovingAverage());
		const char* name = "MA";

		CRecordset* result = nullptr;
		switch (m_settings.m_maType)
		{
		case LocalMAType_SMA:
			result = ta->SimpleMovingAverage(database.get(), m_inputSeries, period, name);
			break;
		case LocalMAType_EMA:
			result = ta->ExponentialMovingAverage(database.get(), m_inputSeries, period, name);
			break;
		case LocalMAType_TSMA:
			result = ta->TimeSeriesMovingAverage(database.get(), m_inputSeries, period, name);
			break;
		case LocalMAType_VMA:
			result = ta->VariableMovingAverage(database.get(), m_inputSeries, period, name);
			break;
		case LocalMAType_TRIMA:
			result = ta->TriangularMovingAverage(database.get(), m_inputSeries, period, name);
			break;
		case LocalMAType_WMA:
			result = ta->WeightedMovingAverage(database.get(), m_inputSeries, period, name);
			break;
			//case LocalMAType_VIDYA:
			//	result = ta->VIDYA(database.get(), inputSeries, period, name);
			//	break;
		default:
			break;
		}

		if (result)
		{
			unique_ptr<CRecordset> cleaner(result);
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
		}
#endif
	}
}