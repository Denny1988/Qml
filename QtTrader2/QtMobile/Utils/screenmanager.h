#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#if defined (RUNTIME_OS_IPHONE)
    #include "IOSLib/ScreenApiHelper.h"
    #include "IOSLib/WKWebViewHelper.h"
#endif
#include <QString>

class ScreenManager
{
public:
    static ScreenManager* getInstance();

protected:
    ScreenManager();

private:
    static ScreenManager* m_Instance;
#if defined (RUNTIME_OS_IPHONE)
    ScreenApiHelper* m_ScreenApiHelper;
    WKWebViewHelper* m_WKWebViewHelper;
#endif


public:    
    static void setVerticalHorizontalScreenChart(bool isHorizontal);
    static void changeStatusBarStyle(bool isBlackFont);
    static bool getIsSleepStatus();
    static QString getAppversionName();
    static QString getSerialNumber();
    static QString getDeviceModel();
    static QString getOSType();
    static QString getOSLanguage();
    static QString getOSVersionCode();
    static bool getIsIPhoneX();
    static void openUrl(QString urlAddress);
    static void copyTxtToSysytemClipboard(QString content);
    static QString getJsReturnValue();
    static void openWebViewUrl(QString url, QString title, QString goBackText);
    static void playNotifySound(QString soundType, QString soundPath);
};


#endif
