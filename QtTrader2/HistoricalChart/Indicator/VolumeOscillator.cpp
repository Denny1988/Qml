#include "VolumeOscillator.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	VolumeOscillator::VolumeOscillator(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	VolumeOscillator::~VolumeOscillator()
	{
	}

	IndicatorSettings& VolumeOscillator::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData["Long Term"] = 22;
		settings.m_hashInputIntData["Short Term"] = 14;
		settings.m_hashInputIntData["Points(1) or Percent(2)"] = 1;
		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::green, 1, Qt::SolidLine, enIndicatorDrawLine };
		settings.m_vectLineSettings.push_back(lineSettings);

		return settings;
	}

	void VolumeOscillator::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData["Long Term"];
		if (endIndex < period) return;

		extendDataBuffer();
		int shortTerm = m_settings.m_hashInputIntData["Short Term"];
		int pointsOrPercent = m_settings.m_hashInputIntData["Points(1) or Percent(2)"];
		if (pointsOrPercent != 1 && pointsOrPercent != 2)
		{
			pointsOrPercent = 1;
			m_settings.m_hashInputIntData["Points(1) or Percent(2)"] = pointsOrPercent;
		}

		const char* name = "VO";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period, "V"));
		unique_ptr<COscillator> ta(new COscillator());
		unique_ptr<CRecordset> result(ta->VolumeOscillator(database.get(), database->getRecordset()->getField(SC::Volume), shortTerm, period, pointsOrPercent, name));

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

	void VolumeOscillator::getParameterRange(const QString& key, int& min, int& max)
	{
		if (key == "Points(1) or Percent(2)")
		{
			min = 1;
			max = 2;
		}
		else
		{
			Indicator::getParameterRange(key, min, max);
		}
	}

}