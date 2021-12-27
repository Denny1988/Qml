#include "SystemTime.h"

namespace HistoricalChartSpace
{
	SystemTime::SystemTime(double systemBaseTime) : m_systemBaseTime(systemBaseTime)
	{
		m_elapsedTimer.start();
	}

	SystemTime::~SystemTime()
	{
	}

	double SystemTime::now()
	{
		double now_ = m_systemBaseTime;
		now_ += m_elapsedTimer.elapsed() / 1000.0 / 60.0 / 60.0 / 24.0;
		return now_;
	}
}