#include "screenmanager.h"
#include "Merchant/merchantmanager.h"
#include "stdafx.h"
#include <QDebug>
#if defined(RUNTIME_OS_ANDROID)
#include<QtAndroidExtras/QAndroidJniObject>
#include<QtAndroidExtras/QAndroidJniEnvironment>
#include<QtAndroidExtras/QtAndroid>
using namespace QtAndroid;
#endif
#include <QStandardPaths>


ScreenManager* ScreenManager:: m_Instance = Q_NULLPTR;
ScreenManager* ScreenManager::getInstance()
{
    if(!m_Instance)
    {
        m_Instance = new ScreenManager;
    }
    return m_Instance;
}

ScreenManager::ScreenManager()
{
#if defined (RUNTIME_OS_IPHONE)
    m_ScreenApiHelper = new ScreenApiHelper();
    m_WKWebViewHelper = new WKWebViewHelper();
#endif
}


void ScreenManager::setVerticalHorizontalScreenChart(bool isHorizontal)
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = androidActivity();
    jint orient = activity.callMethod<jint>( "getRequestedOrientation" );
    if(env->ExceptionCheck())
    {
        qDebug() << "exception occured";
        env->ExceptionClear();
    }
    qDebug() << " changeHorizontalScreenClickByAndroidSlot current orient" << orient;
    if(isHorizontal)
    {
        orient = 0;
        qDebug() << "  orient : 0 横屏"; //横屏
        activity.callMethod<void>("setRequestedOrientation", "(I)V", orient);
        if(env->ExceptionCheck())
        {
            qDebug() << "exception occured";
            env->ExceptionClear();
        }
        qDebug() << "exception occured";
    }
    else
    {
       orient=1;
       qDebug() << "  orient : 1 竖屏";//竖屏
       activity.callMethod<void>("setRequestedOrientation", "(I)V", orient);
       if(env->ExceptionCheck())
       {
           qDebug() << "exception occured";
           env->ExceptionClear();
       }
    }

#else
    m_Instance->m_ScreenApiHelper->setVerticalHorizontalScreenChart(isHorizontal);
#endif
}

void ScreenManager::changeStatusBarStyle(bool isBlackFont)
{
#if defined (RUNTIME_OS_IPHONE)
    m_Instance->m_ScreenApiHelper->changeStatusBarStyle(isBlackFont);
#endif
}

bool ScreenManager::getIsSleepStatus()
{
#if defined(RUNTIME_OS_ANDROID)
    jboolean result = QAndroidJniObject::callStaticMethod<jboolean>("omnicare/AndroidHelper", "getIsSleepStatus");
    //qDebug() << "getAppIsPortraitOrientation : " << resultString.toString();
    return result;
#else
    return m_Instance->m_ScreenApiHelper->getIsSleepStatus();
#endif
}

QString ScreenManager::getAppversionName()
{
#if defined (RUNTIME_OS_IPHONE)
    const char * appversionName = m_Instance->m_ScreenApiHelper->getAppversionName();
    QString appversionNameStr = "";
    if(strlen(appversionName)  > 0 )
        appversionNameStr = QString::fromStdString(appversionName);
    return appversionNameStr;
#else
    return "";
#endif
}

QString ScreenManager::getSerialNumber()
{
#if defined (RUNTIME_OS_IPHONE)
    const char * serialNumber = m_Instance->m_ScreenApiHelper->getSerialNumber();
    QString serialNumberStr = "";
    if(strlen(serialNumber)  > 0 )
        serialNumberStr = QString::fromStdString(serialNumber);
    return serialNumberStr;
#else
    return "";
#endif
}

QString ScreenManager::getDeviceModel()
{
#if defined (RUNTIME_OS_IPHONE)
    const char * deviceModel = m_Instance->m_ScreenApiHelper->getDeviceModel();
    QString deviceModelStr = "";
    if(strlen(deviceModel)  > 0 )
        deviceModelStr = QString::fromStdString(deviceModel);
    return deviceModelStr;
#else
    return "";
#endif
}

QString ScreenManager::getOSType()
{
#if defined (RUNTIME_OS_IPHONE)
    const char * OSVersion = m_Instance->m_ScreenApiHelper->getOSType();
    QString OSVersionStr = "";
    if(strlen(OSVersion)  > 0 )
        OSVersionStr = QString::fromStdString(OSVersion);
    return OSVersionStr;
#else
    return "";
#endif
}

QString ScreenManager::getOSLanguage()
{
#if defined (RUNTIME_OS_IPHONE)
    const char * OSVersion = m_Instance->m_ScreenApiHelper->getOSLanguage();
    QString OSVersionStr = "";
    if(strlen(OSVersion)  > 0 )
        OSVersionStr = QString::fromStdString(OSVersion);
    return OSVersionStr;
#else
    return "";
#endif
}

QString ScreenManager::getOSVersionCode()
{
#if defined (RUNTIME_OS_IPHONE)
    const char * OSVersion = m_Instance->m_ScreenApiHelper->getOSVersionCode();
    QString OSVersionStr = "";
    if(strlen(OSVersion)  > 0 )
        OSVersionStr = QString::fromStdString(OSVersion);
    return OSVersionStr;
#else
    return "";
#endif
}

bool ScreenManager::getIsIPhoneX()
{
    #if defined(RUNTIME_OS_ANDROID)
        return false;
    #else
        return m_Instance->m_ScreenApiHelper->getIsIPhoneX();
    #endif
}

void ScreenManager::openUrl(QString urlAddress)
{
    #if defined(RUNTIME_OS_IPHONE)
        std::string str = urlAddress.toStdString();
        const char * c_Url = str.c_str();
        m_Instance->m_ScreenApiHelper->openUrl(c_Url);
    #endif
}

void ScreenManager::copyTxtToSysytemClipboard(QString content)
{
#if defined(RUNTIME_OS_IPHONE)
    std::string str = content.toStdString();
    const char * c_Content = str.c_str();
    m_Instance->m_ScreenApiHelper->copyTxtToSysytemClipboard(c_Content);
#endif
}

QString ScreenManager::getJsReturnValue()
{
#if defined(RUNTIME_OS_IPHONE)
    const char * jsReturnValue = m_Instance->m_WKWebViewHelper->getJsReturnValue();
    QString jsReturnValueStr = "";
    if(strlen(jsReturnValue)  > 0 )
        jsReturnValueStr = QString::fromStdString(jsReturnValue);
    return jsReturnValueStr;
#else
    return "";
#endif
}

void ScreenManager::openWebViewUrl(QString url, QString title, QString goBackText)
{
#if defined(RUNTIME_OS_IPHONE)
    QString appDir = MerchantManager::getInstance()->getCurrentMerchantPath();
    QString goBackImagePath = appDir + "/Images/back.png";
    int titleHeaderHeight = g_ViewInfo->getHeaderTitleMenuHeight();
    m_Instance->m_WKWebViewHelper->openWebViewUrl(url, title, goBackText, goBackImagePath, titleHeaderHeight);
#endif
}

void ScreenManager::playNotifySound(QString soundType, QString soundPath)
{
    #if defined(RUNTIME_OS_IPHONE)
        std::string str = soundType.toStdString();
        const char * c_SoundType = str.c_str();
        std::string strPath = soundPath.toStdString();
        const char * c_SoundPath = strPath.c_str();
        m_Instance->m_ScreenApiHelper->playNotifySound(c_SoundType, c_SoundPath);
    #endif
}
