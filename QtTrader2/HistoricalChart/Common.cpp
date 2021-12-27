#include "Common.h"
#include <cstring>
#include <QColor>
#include <QRectF>
namespace HistoricalChartSpace
{
	const QString Constants::LogFile = "Chart.log";
	const int Constants::BidLessThan = 0;
	const int Constants::BidGreaterThan = 1;
	const int Constants::AskLessthan = 2;
	const int Constants::AskGreaterThan = 3;

	Frequency::Frequency()
	{
		m_Type = HistoricalChartSpace::Minute;
		m_Unit = 0;
	}
	Frequency::Frequency(FrequencyType type, int unit)
	{
		m_Type = type;
		m_Unit = unit;
	}
	QString Frequency::getName() const
	{
		QString str;
		switch (m_Type)
		{
		case HistoricalChartSpace::Second:
			str = "S";
			break;
		case HistoricalChartSpace::Minute:
			str = "M";
			break;
		case HistoricalChartSpace::Hour:
			str = "H";
			break;
		case HistoricalChartSpace::Day:
			str = (m_Unit == 1 ? "Daily" : "Days");
			break;
		case HistoricalChartSpace::Week:
			str = (m_Unit == 1 ? "Weekly" : "Weeks");
			break;
		case HistoricalChartSpace::Month:
			str = (m_Unit == 1 ? "Monthly" : "Months");
			break;
		case HistoricalChartSpace::Year:
			str = (m_Unit == 1 ? "Yearly" : "Years");
			break;
		default:
			break;
		}

		if (m_Type < Day)
		{
			str += QString::number(m_Unit);
		}
		else
		{
			str = (m_Unit == 1 ? str : QString::number(m_Unit) + str);
		}

		return str;

	}
	double Frequency::getJDateTimeSpan() const
	{
		double nRet = -1.0;
		switch (m_Type)
		{
		case HistoricalChartSpace::Second:
			nRet = 1.0 / 60.0 / 60.0 / 24.0;
			break;
		case HistoricalChartSpace::Minute:
			nRet = 1.0 / 60.0 / 24.0;
			break;
		case HistoricalChartSpace::Hour:
			nRet = 1.0 / 24.0;
			break;
		case HistoricalChartSpace::Day:
			nRet = 1.0;
			break;
		case HistoricalChartSpace::Week:
			nRet = 7.0;
			break;
		case HistoricalChartSpace::Month:
			nRet = 30.0;
			break;
		case HistoricalChartSpace::Year:
			nRet = 365.0;
			break;
		default:
			break;
		}

		return nRet * m_Unit;
	}
	
	int Frequency::getTimeSpanInSecond() const
	{
		int nRet = -1;
		switch (m_Type)
		{
		case HistoricalChartSpace::Second:
			nRet = 1;
			break;
		case HistoricalChartSpace::Minute:
			nRet = 60;
			break;
		case HistoricalChartSpace::Hour:
			nRet = 60 * 60;
			break;
		case HistoricalChartSpace::Day:
			nRet = 24 * 60 * 60;
			break;
		case HistoricalChartSpace::Week:
			nRet = 7 * 24 * 60 * 60;
			break;
		case HistoricalChartSpace::Month:
			nRet = 30 * 24 * 60 * 60;
			break;
		case HistoricalChartSpace::Year:
			nRet = 365 * 24 * 60 * 60;
			break;
		default:
			break;
		}

		return nRet * m_Unit;
	}

	FrequencyType Frequency::getType() const
	{
		return m_Type;
	}
	int Frequency::getUint() const
	{
		return m_Unit;
	}
	bool Frequency::operator==(const Frequency& that) const
	{
		return this->m_Type == that.m_Type && this->m_Unit == that.m_Unit;
	}

	bool Frequency::operator!=(const Frequency& that) const
	{
		return *this == that ? false : true;
	}

	bool Frequency::isPloidyOf(Frequency& other, int& multiple) const
	{
		if (m_Type == HistoricalChartSpace::Minute || m_Type == HistoricalChartSpace::Hour)
		{
			if (other.getType() == HistoricalChartSpace::Minute || other.getType() == HistoricalChartSpace::Hour)
			{
				if (m_Type >= other.getType())
				{
					int levelDiff = m_Type - other.getType();
					if ((levelDiff * 60 + m_Unit) % other.getUint() == 0)
					{
						multiple = (levelDiff * 60 + m_Unit) / other.getUint();
						return true;
					}
				}
			}
		}
		return false;
	}

	showInTime Frequency::getTypicalShowTime() const
	{
		showInTime enumShow(enshowInTime_unknown);
		switch (m_Type)
		{
		case HistoricalChartSpace::Second:
			break;
		case HistoricalChartSpace::Minute:
			if (m_Unit == 1)
			{
				enumShow = enShowInTime_1min;
			}
			else if (m_Unit == 2)
			{
				enumShow = enshowInTime_2min;
			}
			else if (m_Unit == 3)
			{
				enumShow = enshowInTime_3min;
			}
			else if (m_Unit == 4)
			{
				enumShow = enshowInTime_4min;
			}
			else if (m_Unit == 5)
			{
				enumShow = enshowInTime_5min;
			}
			else if (m_Unit == 6)
			{
				enumShow = enshowInTime_6min;
			}
			else if (m_Unit == 10)
			{
				enumShow = enshowInTime_10min;
			}
			else if (m_Unit == 12)
			{
				enumShow = enshowInTime_12min;
			}
			else if (m_Unit == 15)
			{
				enumShow = enshowInTime_15min;
			}
			else if (m_Unit == 20)
			{
				enumShow = enshowInTime_20min;
			}
			else if (m_Unit == 30)
			{
				enumShow = enshowInTime_30min;
			}
			else
			{
				enumShow = enshowInTime_mins;
			}
			break;
		case HistoricalChartSpace::Hour:
			if (m_Unit == 1)
			{
				enumShow = enshowInTime_1hour;
			}
			else if (m_Unit == 2)
			{
				enumShow = enshowInTime_2hour;
			}
			else if (m_Unit == 3)
			{
				enumShow = enshowInTime_3hour;
			}
			else if (m_Unit == 4)
			{
				enumShow = enshowInTime_4hour;
			}
			else if (m_Unit == 6)
			{
				enumShow = enshowInTime_6hour;
			}
			else if (m_Unit == 8)
			{
				enumShow = enshowInTime_8hour;
			}
			else if (m_Unit == 10)
			{
				enumShow = enshowInTime_12hour;
			}
			else
			{
				enumShow = enshowInTime_hours;
			}
			break;
		case HistoricalChartSpace::Day:
			enumShow = enshowInTime_daily;
			break;
		case HistoricalChartSpace::Week:
			enumShow = enshowInTime_weekily;
			break;
		case HistoricalChartSpace::Month:
			enumShow = enShowInTime_monthly;
			break;
		case HistoricalChartSpace::Year:
			break;
		default:
			break;
		}

		return enumShow;
	}

	QString ChartProperty::getKey() const
	{
		return instrumentId + quotePolicyId + frequency.getName();
	}

}

