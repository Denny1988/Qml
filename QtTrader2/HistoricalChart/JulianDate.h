
#pragma once
#include <QString>
#include "Common.h"
#include "historicalchart_global.h"
#include <ctime>
#include <QDateTime>

namespace HistoricalChartSpace
{

	class HISTORICALCHART_EXPORT JulianDate
	{
	public:

		static bool inline IsLeap(const int year)
		{
			return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
		}


		static int DaysFromDate(const int year, const int month, const int day)
		{
			int i, d = 0;
			// not very efficient, but shouldn't be a problem for any realistic date
			for (i = ORIGIN_YEAR; i < year; i++)
			{
				d += 365;
				if (IsLeap(i))
					d++;
			}
			if (IsLeap(year) && month > 2)
				d++;
			for (i = 1; i < month; i++)
				d += monthDays[i - 1];
			d += day;
			return d + ORIGIN_JULIAN;
		}

		// computes the year month and day, given the julian date for noon that day
		static void DaysToDate(const int julian_days, short &year, short& month, short &day)
		{
			int d, y, m, tmp;
			d = julian_days - ORIGIN_JULIAN;              // days from 11/31/1582
			y = ORIGIN_YEAR;
			while (d > (tmp = 365 + (IsLeap(y) ? 1 : 0)))
			{
				d -= tmp;
				y++;
			}
			m = 1;
			while (d > (tmp = monthDays[m - 1] + ((IsLeap(y) && m == 2) ? 1 : 0)))
			{
				d -= tmp;
				m++;
			}
			year = y;
			month = m;
			day = d;
		}


		// converts a date into a julian date
		static double ToJulianDate(const int year, const int month, const int day, const int hour = 0, const int minute = 0, const int second = 0, const int millisecond = 0)
		{
			double julian;
			int ms = millisecond;
			if (millisecond > 995) ms = 0;
			// date component
			julian = (double)DaysFromDate(year, month, day);
			// time component
			julian += (((((double)ms + 0.5) / 1000.0 + (double)second) / 60.0 + (double)minute) / 60.0 + (double)hour) / 24.0;
			// origin is at noon
			return julian - 0.5;
		}

		static double ToJulianDate(StructDate& ret)
		{
			return ToJulianDate(ret.Year, ret.Month, ret.Day, ret.Hour, ret.Minute, ret.Second, ret.Millisecond);
		}

		// converts a julian date into a date.
		static StructDate FromJDate(const double julian)
		{

			int jdays;
			double remainder, hour, minute, second, millisec;

			// separate the integer and decimal part of the julian date
			jdays = (int)julian;
			remainder = julian - (double)jdays;

			// make adjustment to account for the origin being at noon
			if (remainder < 0.5)
			{
				remainder += 0.5;
			}
			else
			{
				jdays++;
				remainder -= 0.5;
			}

			StructDate ret;

			// compute the date
			DaysToDate(jdays, ret.Year, ret.Month, ret.Day);

			// compute the time
			hour = remainder * 24.0;
			ret.Hour = (int)hour;
			remainder = hour - (double)ret.Hour;

			minute = remainder * 60.0;
			ret.Minute = (int)minute;
			remainder = minute - (double)ret.Minute;

			second = remainder * 60.0;
			ret.Second = (int)second;
			remainder = second - (double)ret.Second;

			millisec = remainder * 1000.0;
			if (millisec > 995) millisec = 0; //???
			ret.Millisecond = (int)millisec;

			return ret;
		}

		static QString FromJDateToString(const double julian)
		{
			StructDate date = FromJDate(julian);

			QString str(QString::number(date.Year));
			str.append(".").append(QString::number(date.Month)).append(".").append(QString::number(date.Day)).append("  ").append(QString::number(date.Hour)).append(":").append(QString::number(date.Minute)).append(":").append(QString::number(date.Second));

			return str;
		}

		static QString JDateItemToString(const double julian, int nType, int nlen = 0)
		{
			StructDate date = FromJDate(julian);
			QString str;
			for (int i = enShowYear; i < enShowDateUnknown; i <<= 1)
			{
				QString tempstr("");
				switch (nType & i)
				{
				case HistoricalChartSpace::enShowYear:
					tempstr.sprintf("%04d", date.Year);
					break;
				case HistoricalChartSpace::enShowMonth:
					tempstr = " " + DateMonthToString(date.Month, nlen);
					break;
				case HistoricalChartSpace::enShowDay:
					tempstr.sprintf("%02d", date.Day);
					break;
				case HistoricalChartSpace::enShowTime:
					tempstr.sprintf(" %02d:%02d", date.Hour, date.Minute);
					break;
				case HistoricalChartSpace::enShowSecond:
					tempstr.sprintf(":%02d", date.Second);
					break;
				case HistoricalChartSpace::enShowDateUnknown:
					break;
				default:
					break;
				}
				str.append(tempstr);
			}
			return str;
		}

		static QString JDateToString(double julian, bool isShowDate, bool isShowTime)
		{
			StructDate date = FromJDate(julian);
			QString str;
			QString tempstr;
			if (isShowDate)
			{
				tempstr.sprintf("%04d-%02d-%02d", date.Year, date.Month, date.Day);
				str.append(tempstr);
			}
			if (isShowTime)
			{
				const char* format = isShowDate ? " %02d:%02d:%02d" : "%02d:%02d:%02d";
				tempstr.sprintf(format, date.Hour, date.Minute, date.Second);
				str.append(tempstr);
			}
			return str;
		}

		static QString DateMonthToString(short nMonth, int nlen)
		{
			QString szRet;
			if (nlen <= 1){ //J
				switch (nMonth){
				case 1:
					szRet = "J";
					break;
				case 2:
					szRet = "F";
					break;
				case 3:
					szRet = "M";
					break;
				case 4:
					szRet = "A";
					break;
				case 5:
					szRet = "M";
					break;
				case 6:
					szRet = "J";
					break;
				case 7:
					szRet = "J";
					break;
				case 8:
					szRet = "A";
					break;
				case 9:
					szRet = "S";
					break;
				case 10:
					szRet = "O";
					break;
				case 11:
					szRet = "N";
					break;
				case 12:
					szRet = "D";
					break;
				}
			}
			else if (nlen == 3){ //Jan
				switch (nMonth){
				case 1:
					szRet = "Jan";
					break;
				case 2:
					szRet = "Feb";
					break;
				case 3:
					szRet = "Mar";
					break;
				case 4:
					szRet = "Apr";
					break;
				case 5:
					szRet = "May";
					break;
				case 6:
					szRet = "Jun";
					break;
				case 7:
					szRet = "Jul";
					break;
				case 8:
					szRet = "Aug";
					break;
				case 9:
					szRet = "Sep";
					break;
				case 10:
					szRet = "Oct";
					break;
				case 11:
					szRet = "Nov";
					break;
				case 12:
					szRet = "Dec";
					break;
				}
			}
			else if (nlen > 3){ //January
				switch (nMonth){
				case 1:
					szRet = "January";
					break;
				case 2:
					szRet = "February";
					break;
				case 3:
					szRet = "March";
					break;
				case 4:
					szRet = "April";
					break;
				case 5:
					szRet = "May";
					break;
				case 6:
					szRet = "June";
					break;
				case 7:
					szRet = "July";
					break;
				case 8:
					szRet = "August";
					break;
				case 9:
					szRet = "September";
					break;
				case 10:
					szRet = "October";
					break;
				case 11:
					szRet = "November";
					break;
				case 12:
					szRet = "December";
					break;
				}
			}
			return szRet;
		}
		static double GetCurrentJDate()
		{
			time_t currentPrim = time(NULL);
			tm* currentTemp = localtime(&currentPrim);

			return ToJulianDate(currentTemp->tm_year + 1900, currentTemp->tm_mon + 1, currentTemp->tm_mday, currentTemp->tm_hour, currentTemp->tm_min, currentTemp->tm_sec, 0);
		}


		static QDateTime JDateToQTDate(double jDate)
		{
			StructDate structTime = JulianDate::FromJDate(jDate);
			QDate date(structTime.Year, structTime.Month, structTime.Day);
			QTime time(structTime.Hour, structTime.Minute, structTime.Second, structTime.Millisecond);
			return QDateTime(date, time);
		}

		static double QTDateToJDate(const QDateTime& qDate)
		{
            QDate date = qDate.date();
            QTime time = qDate.time();
			StructDate structTime;
			structTime.Year = date.year();
			structTime.Month = date.month();
			structTime.Day = date.day();
			structTime.Hour = time.hour();
			structTime.Minute = time.minute();
			structTime.Second = time.second();
			structTime.Millisecond = time.msec();
			return JulianDate::ToJulianDate(structTime);
		}

		// accurate to seconds
		static bool isEqual(const double& date1, const double& date2)
		{
			StructDate structDate1 = FromJDate(date1);
			StructDate structDate2 = FromJDate(date2);
			return structDate1.Year == structDate2.Year && structDate1.Month == structDate2.Month && structDate1.Day == structDate2.Day
				&& structDate1.Hour == structDate2.Hour && structDate1.Minute == structDate2.Minute && structDate1.Second == structDate2.Second;
		}

		// if data1 lessThan data2, accurate to seconds.
		static bool lessThan(const double& date1, const double& date2)
		{
			StructDate structDate1 = FromJDate(date1);
			StructDate structDate2 = FromJDate(date2);
			return structDate1.Year < structDate2.Year || 
				structDate1.Year == structDate2.Year && structDate1.Month < structDate2.Month ||
				structDate1.Year == structDate2.Year && structDate1.Month == structDate2.Month && structDate1.Day < structDate2.Day ||
				structDate1.Year == structDate2.Year && structDate1.Month == structDate2.Month && structDate1.Day == structDate2.Day && structDate1.Hour < structDate2.Hour  ||
				structDate1.Year == structDate2.Year && structDate1.Month == structDate2.Month && structDate1.Day == structDate2.Day && structDate1.Hour == structDate2.Hour && structDate1.Minute < structDate2.Minute ||
				structDate1.Year == structDate2.Year && structDate1.Month == structDate2.Month && structDate1.Day == structDate2.Day && structDate1.Hour == structDate2.Hour && structDate1.Minute == structDate2.Minute && structDate1.Second < structDate2.Second;
		}
	};
}
