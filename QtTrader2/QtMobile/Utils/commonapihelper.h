#ifndef COMMONAPIHELPER_H
#define COMMONAPIHELPER_H


#include <QDebug>

class CommonApiHelper
{
public:
    CommonApiHelper();
    static const QString getAppExCardDirectoryPath();
    static const QString getAppDataDirectoryPath();
    static const QString getAppCacheDirectoryPath();
    static const QString getAppDirectoryPath();
    static void openUrl(QString url);
    static int getAppVersionCode();
    static const QString getAppversionName();
    static const QString getSerialNumber();
    static const QString getDeviceModel();
    static const QString getOSType();
    static const QString getOSLanguage();
    static const QString getOSVersionCode();
    static void setBadgeCount(int count, QString titleStr, QString contentStr);
    static void copyTxtToSysytemClipboard(QString txt);
    static const QString getAppGalleryPath();
    static void startVibrate();
    static void openWebViewUrl(QString url, QString title, QString goBackText);
    static const QString getJsReturnValue();
    static void playSound(QString type);
};

#endif // COMMONAPIHELPER_H
