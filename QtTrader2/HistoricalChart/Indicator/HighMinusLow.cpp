#include "HighMinusLow.h"

namespace HistoricalChartSpace
{
	HighMinusLow::HighMinusLow(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	HighMinusLow::~HighMinusLow()
	{
	}

	IndicatorSettings& HistoricalChartSpace::HighMinusLow::initSettings(IndicatorSettings& settings) const
	{
		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::blue, 1, Qt::SolidLine, enIndicatorDrawCurve };
		settings.m_vectLineSettings.push_back(lineSettings);
		return settings;
	}

	void HighMinusLow::calculate(int beginIndex, int endIndex)
	{
		// 1. extend buffer
		int dataSize = extendDataBuffer();

		// 2. calculate
		vector<double>& vectOutReal = *m_indicatorDatas[SC::OutReal];
		const vector<double>& highs = m_baseData.getHighs();
		const vector<double>& lows = m_baseData.getLows();
		for (int i = beginIndex; i < endIndex; ++i)
		{
			vectOutReal[i] = highs[i] - lows[i];
		}
	}
}