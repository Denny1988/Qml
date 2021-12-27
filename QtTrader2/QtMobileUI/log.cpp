#include "log.h"

void Log::e(const char * msg)
{
    QLoggingCategory qcategory("qt.log");
    qCCritical(qcategory) <<  msg;
}

void Log::i(const char * msg)
{
    QLoggingCategory qcategory("qt.log");
    qCInfo(qcategory) <<  msg;
}

void Log::d(const char * msg)
{
    QLoggingCategory qcategory("qt.log");
    qCDebug(qcategory) <<  msg;
}

Log::Log()
{
}
