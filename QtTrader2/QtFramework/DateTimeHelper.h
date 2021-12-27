#ifndef DATETIMEHELPER_H
#define DATETIMEHELPER_H

#include <QDateTime>

class DateTimeHelper
{
public:
    static QString toLongDateTimeString(const QDateTime& d)
    {
        if(d.isNull() || d.date().year() == 0) return "";
        QString str;//yyyy-MM-dd hh:mm:ss.zzz
		QDate date = d.date();
		QTime time = d.time();
        str.sprintf("%d-%02d-%02d %02d:%02d:%02d.%03d", date.year(), date.month(), date.day(), time.hour(), time.minute(), time.second(),time.msec());
        return str;
    }

    static QString toDateTimeString(const QDateTime& d)
    {
        if(d.isNull() || d.date().year() == 0) return "";
        QString str;//yyyy-MM-dd hh:mm:ss
		QDate date = d.date();
		QTime time = d.time();
        str.sprintf("%d-%02d-%02d %02d:%02d:%02d",date.year(), date.month(), date.day(), time.hour(), time.minute(), time.second());
        return str;
    }

    static QString toDateString(const QDateTime& d)
    {
        if(d.isNull() || d.date().year() == 0) return "";
        QString str;//yyyy-MM-dd
		QDate date = d.date();
        str.sprintf("%d-%02d-%02d",date.year(), date.month(), date.day());
        return str;
    }

    static QString toDateString(const QDate& d)
    {
        if(d.isNull() || d.year() == 0) return "";
        QString str;//yyyy-MM-dd
        str.sprintf("%d-%02d-%02d",d.year(), d.month(), d.day());
        return str;
    }

    static QString toNoShowYearDateString(const QDateTime& d)
    {
        if(d.isNull() || d.date().year() == 0) return "";
        QString str;//MM-dd
        QDate date = d.date();
        str.sprintf("%02d-%02d", date.month(), date.day());
        return str;
    }

    static QString toNoShowDateString(const QDateTime& d)
    {
        if(d.isNull() || d.date().year() == 0) return "";
        QString str;//HH:mm:ss
		QTime time = d.time();
        str.sprintf("%02d:%02d:%02d",time.hour(), time.minute(), time.second());
        return str;
    }

    static QString toNoShowSecondDateTimeString(const QDateTime& d)
    {
        if(d.isNull() || d.date().year() == 0) return "";
        QString str;//yyyy-MM-dd hh:mm
		QDate date = d.date();
		QTime time = d.time();
        str.sprintf("%d-%02d-%02d %02d:%02d",date.year(), date.month(), date.day(), time.hour(), time.minute());
        return str;
    }

    static QString toNoShowSecondTimeString(const QDateTime& d)
    {
        if(d.isNull() || d.date().year() == 0) return "";
        QString str;//hh:mm
        QTime time = d.time();
        str.sprintf("%02d:%02d", time.hour(), time.minute());
        return str;
    }

    static QString toNoShowYearDateTimeString(const QDateTime& d)
    {
        if(d.isNull() || d.date().year() == 0) return "";
        QString str;//MM-dd hh:mm:ss
        QDate date = d.date();
        QTime time = d.time();
        str.sprintf("%02d-%02d %02d:%02d:%02d", date.month(), date.day(), time.hour(), time.minute(), time.second());
        return str;
    }

};

#endif
