#pragma once
#include <QElapsedTimer>

namespace HistoricalChartSpace
{
	class SystemTime
	{
	public:
		SystemTime(double systemBaseTime);
		~SystemTime();

		double now();

	private:
		double m_systemBaseTime;
		QElapsedTimer m_elapsedTimer;
	};
}