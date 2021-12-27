#include "commonhelper.h"
#include <QCoreApplication>
#include <QDir>
namespace QtFramwork
{
    CommonHelper::CommonHelper()
    {

    }

    QString CommonHelper::getAppDataDirPath()
    {
    #ifdef RUNTIME_OS_WIN32
        return QCoreApplication::applicationDirPath();
    #elif RUNTIME_OS_MACX
        auto appDirPath = QCoreApplication::applicationDirPath();
        QString before = ".app/Contents/MacOS";
        QString after = ".app/Contents/Data";
        QString fullPath = appDirPath.replace(before, after, Qt::CaseSensitive);
        QDir dir(fullPath);
        if(!dir.exists())
        {//for debug
            appDirPath = QCoreApplication::applicationDirPath();
            before = "Trader.app/Contents/MacOS";
            QString after = "Data";
            fullPath = appDirPath.replace(before, after, Qt::CaseSensitive);
        }
        return fullPath;
    #endif
    }
}

