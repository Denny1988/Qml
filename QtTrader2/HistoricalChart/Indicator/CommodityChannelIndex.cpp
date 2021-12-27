#include "CommodityChannelIndex.h"

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

using namespace std;
namespace HistoricalChartSpace
{
	CommodityChannelIndex::CommodityChannelIndex(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	CommodityChannelIndex::~CommodityChannelIndex()
	{
	}

	IndicatorSettings& CommodityChannelIndex::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData[SC::Period] = 7;

		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::blue, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void CommodityChannelIndex::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData[SC::Period];
		int period2 = period * 1.8;
		if (endIndex < period2) return;

		// 1. extend buffer
		int dataSize = extendDataBuffer();

#ifdef TA_Lib
		/************************************************************************/
		/* Use TA-Lib                                                           */
		/************************************************************************/
		//TA_RetCode TA_CCI( int    startIdx,
		// int    endIdx,
		// const double inHigh[],
		// const double inLow[],
		// const double inClose[],
		// int           period, /* From 2 to 100000 */
		// int          *outBegIdx,
		// int          *outNBElement,
		// double        outReal[] );

		int outBegin = 0;
		int outNum = 0;

		/************************************************************************/
		/* Use TA-Lib                                                           */
		/************************************************************************/
		double* outReal = new double[dataSize];
		TA_RetCode retCode = TA_CCI(beginIndex, endIndex, &m_baseData.getHighs()[0], &m_baseData.getLows()[0], &m_baseData.getCloses()[0], period, &outBegin, &outNum, outReal);
		if (retCode == TA_SUCCESS)
		{
			// 3. update indicator data
			vector<double>* vectOutReal = m_indicatorDatas[SC::OutReal];
			for (int i = 0; i < outNum; i++)
			{
				(*vectOutReal)[i + outBegin] = outReal[i];
			}
		}
		else
		{
            //_ASSERT(false);
		}

		// 4. cleaning
		delete[] outReal;
#else
		/************************************************************************/
		/* Use TA-SDK                                                           */
		/************************************************************************/

		const char* name = "CCI";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period * 2, "OHLC"));
		unique_ptr<CIndex> ta(new CIndex());
		unique_ptr<CRecordset> result(ta->CommodityChannelIndex(database.get(), database->getRecordset(), period, name));

		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		for (int bufferIndex = 0; bufferIndex < m_inputDataBufferSize; bufferIndex++)
		{
			int dataIndex = m_dataStartIndex + bufferIndex;
			if (dataIndex < period2)
			{
				vectOutReal[dataIndex] = numeric_limits<double>::quiet_NaN();
			}
			else if (bufferIndex >= period2)
			{
				vectOutReal[dataIndex] = result->getValue(name, bufferIndex + 1);
			}
		}
#endif // TA_Lib
	}
}
