#include "TrueRange.h"

namespace HistoricalChartSpace
{
	TrueRange::TrueRange(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	TrueRange::~TrueRange()
	{
	}

	IndicatorSettings& TrueRange::initSettings(IndicatorSettings& settings) const
	{
		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::blue, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	/*
		http://stockcharts.com/school
		Wilder started with a concept called True Range(TR), which is defined as the greatest of the following :
		Method 1 : Current High less the current Low
		Method 2 : Current High less the previous Close(absolute value)
		Method 3 : Current Low less the previous Close(absolute value)
		*/
	void TrueRange::calculate(int beginIndex, int endIndex)
	{
		// 1. extend buffer
		int dataSize = extendDataBuffer();

		// 2. calculate
		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		const vector<double>& highs = m_baseData.getHighs();
		const vector<double>& lows = m_baseData.getLows();
		const vector<double>& closes = m_baseData.getCloses();
		int begin = beginIndex > 2 ? beginIndex : 2;
		for (int i = begin; i <= endIndex; ++i)
		{
			double range1 = highs[i] - lows[i];
			double range2 = abs(highs[i] - closes[i - 1]);
			double range3 = abs(lows[i] - closes[i - 1]);

			double trueRange = -1;
			if (range1 > trueRange)
				trueRange = range1;
			if (range2 > trueRange)
				trueRange = range2;
			if (range3 > trueRange)
				trueRange = range3;

			vectOutReal[i] = trueRange;
		}
	}
}