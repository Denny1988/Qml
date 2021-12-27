#include "VolumeROC.h"
#include "TASDK.h"

namespace HistoricalChartSpace
{
	VolumeROC::VolumeROC(IndicatorSettings& settings, BaseData& baseData, bool needInitialize)
		: Indicator(needInitialize ? initSettings(settings) : settings, baseData)
	{
		m_indicatorDatas[SC::OutReal] = new vector < double > ;
	}

	VolumeROC::~VolumeROC()
	{
	}

	IndicatorSettings& VolumeROC::initSettings(IndicatorSettings& settings) const
	{
		settings.m_hashInputIntData[SC::Period] = 22;
		settings.m_arrayNum = 1;
		settings.m_plotNum = 1;

		IndicatorLineSettings lineSettings = { SC::OutReal, Qt::green, 1, Qt::SolidLine, enIndicatorDrawLine };
		settings.m_vectLineSettings.push_back(lineSettings);

		return settings;
	}

	void VolumeROC::calculate(int beginIndex, int endIndex)
	{
		int period = m_settings.m_hashInputIntData[SC::Period];
		if (endIndex < period) return;

		extendDataBuffer();

		const char* name = "VROC";
		unique_ptr<CDatabase> database(prepare(beginIndex, endIndex, period, "V"));
		unique_ptr<CGeneral> ta(new CGeneral());
		unique_ptr<CRecordset> result(ta->VolumeROC(database.get(), database->getRecordset()->getField(SC::Volume), period, name));

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
}