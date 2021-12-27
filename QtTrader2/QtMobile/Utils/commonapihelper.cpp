#include "commonapihelper.h"
#include "screenmanager.h"
#include "Merchant/merchantmanager.h"
#include "Common/CustomerSetting.h"
#if defined(RUNTIME_OS_ANDROID)
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroid>
using namespace QtAndroid;
#else
#include "IOSLib/ScreenApiHelper.h"
#endif

#include <QStandardPaths>

CommonApiHelper::CommonApiHelper()
{

}

//path存储在卡或者手机外部存储空间中，可通过第三方软件查看文件 //for ANDROID
const QString CommonApiHelper::getAppExCardDirectoryPath()
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject resultString  = QAndroidJniObject::callStaticObjectMethod<jstring>("omnicare/AndroidHelper", "getAppExCardDirectoryPath");
    qDebug() << "getAppExCardDirectoryPath() = " << resultString.toString();
    return resultString.toString();
#else
    return QStandardPaths::standardLocations(QStandardPaths::DataLocation)[0];
#endif
}

//path存储在App Data中，需要root权限才能查看 //for ANDROID
const QString CommonApiHelper::getAppDataDirectoryPath()
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject resultString  = QAndroidJniObject::callStaticObjectMethod<jstring>("omnicare/AndroidHelper", "getAppDataDirectoryPath");
    //qDebug() << "getAppDataDirectoryPath() = " << resultString.toString();
    return resultString.toString();
#else
    return QStandardPaths::standardLocations(QStandardPaths::DataLocation)[0];
#endif
}

//path存储在App Cache中，缓存文件，清理空间时可能被删除 //for ANDROID
const QString CommonApiHelper::getAppCacheDirectoryPath()
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject resultString  = QAndroidJniObject::callStaticObjectMethod<jstring>("omnicare/AndroidHelper", "getAppCacheDirectoryPath");
    //qDebug() << "getAppCacheDirectoryPath()" << resultString.toString();
    return resultString.toString();
#else
    return QStandardPaths::standardLocations(QStandardPaths::CacheLocation)[0];
#endif
}

//app使用的路径 //for ANDROID
const QString CommonApiHelper::getAppDirectoryPath()
{
#ifdef APPPUBLISH
    return CommonApiHelper::getAppDataDirectoryPath(); //看不到文件夹
#else
    return CommonApiHelper::getAppExCardDirectoryPath();//可以在存储空间中看到文件夹
#endif
}

void CommonApiHelper::openUrl(QString url)
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject QUrl = QAndroidJniObject::fromString(url);
    jstring aUrl =  QUrl.object<jstring>();
    jint result = QAndroidJniObject::callStaticMethod<jint>("omnicare.AndroidHelper",
                                                            "openUrl",
                                                            "(Ljava/lang/String;)I",
                                                            aUrl);
    qDebug() << "CommonApiHelper:openUrl result = " << result;
#else
    ScreenManager::getInstance()->openUrl(url);
#endif
}

int CommonApiHelper::getAppVersionCode()
{
#if defined(RUNTIME_OS_ANDROID)
    jint resultString  = QAndroidJniObject::callStaticMethod<jint>("omnicare/AndroidHelper",
                                                                   "getAppVersionCode",
                                                                   "()I");
    return (int)resultString;
#else
    return  0;
#endif
}

const QString CommonApiHelper::getAppversionName()
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject resultString  = QAndroidJniObject::callStaticObjectMethod<jstring>("omnicare/AndroidHelper",
                                                                                         "getAppversionName");
    return resultString.toString();
#else
    return  ScreenManager::getInstance()->getAppversionName();
#endif
}

const QString CommonApiHelper::getSerialNumber()
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject string = QAndroidJniObject::callStaticObjectMethod("omnicare/AndroidHelper",
                                                                         "getSerialNumber",
                                                                         "()Ljava/lang/String;");
    return string.toString();
#else
    return  ScreenManager::getInstance()->getSerialNumber();
#endif
}

const QString CommonApiHelper::getDeviceModel()
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject str = QAndroidJniObject::callStaticObjectMethod("omnicare/AndroidHelper",
                                                                      "getDeviceModel",
                                                                      "()Ljava/lang/String;");
    return str.toString();
#else
    return  ScreenManager::getInstance()->getDeviceModel();
#endif
}

const QString CommonApiHelper::getOSType()
{
#if defined(RUNTIME_OS_ANDROID)
    return "Android";
#else
    return  ScreenManager::getInstance()->getOSType();
#endif
}

const QString CommonApiHelper::getOSLanguage()
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject str = QAndroidJniObject::callStaticObjectMethod("omnicare/AndroidHelper",
                                                                      "getOSLanguage",
                                                                      "()Ljava/lang/String;");
    return str.toString();
#else
    return  ScreenManager::getInstance()->getOSLanguage();
#endif
}

const QString CommonApiHelper::getOSVersionCode()
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject str = QAndroidJniObject::callStaticObjectMethod("omnicare/AndroidHelper",
                                                                      "getOSVersionCode",
                                                                      "()Ljava/lang/String;");
    return str.toString();
#else
    return  ScreenManager::getInstance()->getOSVersionCode();
#endif
}


void CommonApiHelper::setBadgeCount(int count, QString titleStr, QString contentStr)
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject QTitleStr = QAndroidJniObject::fromString(titleStr);
    jstring aTitleStr =  QTitleStr.object<jstring>();
    QAndroidJniObject QContentStr = QAndroidJniObject::fromString(contentStr);
    jstring aContentstr =  QContentStr.object<jstring>();
    jint result = QAndroidJniObject::callStaticMethod<jint>("omnicare.AndroidHelper",
                                                            "setBadgeCount",
                                                            "(ILjava/lang/String;Ljava/lang/String;)I",
                                                            count,
                                                            aTitleStr,
                                                            aContentstr);
    qDebug() << "CommonApiHelper:setBadgeCount result = " << result;
#else
    qDebug() << "CommonApiHelper:setBadgeCount result = " << count << titleStr << contentStr;
#endif
}

void CommonApiHelper::copyTxtToSysytemClipboard(QString txt)
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject QTxt = QAndroidJniObject::fromString(txt);
    jstring aTxt =  QTxt.object<jstring>();
    QAndroidJniObject::callStaticMethod<void>("omnicare.AndroidHelper",
                                                "copyTxtToSysytemClipboard",
                                                "(Ljava/lang/String;)V",
                                                aTxt);
#else
    ScreenManager::getInstance()->copyTxtToSysytemClipboard(txt);
#endif
}

const QString CommonApiHelper::getAppGalleryPath()
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject str = QAndroidJniObject::callStaticObjectMethod("omnicare/AndroidHelper",
                                                                      "getAppGalleryPath",
                                                                      "()Ljava/lang/String;");
    return str.toString();
#else
    return  "";
#endif
}

void CommonApiHelper::startVibrate()
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject::callStaticMethod<void>("omnicare/AndroidHelper", "startVibrate", "(I)V", 300);
#else

#endif
}


void CommonApiHelper::openWebViewUrl(QString url, QString title, QString goBackText)
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject QUrl = QAndroidJniObject::fromString(url);
    jstring aUrl =  QUrl.object<jstring>();
    QAndroidJniObject QTitle = QAndroidJniObject::fromString(title);
    jstring aTitle =  QTitle.object<jstring>();
    QAndroidJniObject QGoBackText = QAndroidJniObject::fromString(goBackText);
    jstring aGoBackText=  QGoBackText.object<jstring>();
    QString path = QString("file:///%1/Images/back.png").arg(MerchantManager::getInstance()->getCurrentMerchantPath());
    QAndroidJniObject QPath = QAndroidJniObject::fromString(path);
    jstring aPath=  QPath.object<jstring>();
    jint result = QAndroidJniObject::callStaticMethod<jint>("omnicare.WebViewHelper",
                                                            "openWebView",
                                                            "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I",
                                                            aUrl, aTitle, aGoBackText, aPath);
    qDebug() << "CommonApiHelper:openUrl result = " << result;
#else
    ScreenManager::getInstance()->openWebViewUrl(url, title, goBackText);
#endif
}

const QString CommonApiHelper::getJsReturnValue()
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject string = QAndroidJniObject::callStaticObjectMethod("omnicare/WebViewHelper",
                                                                         "getJsReturnValue",
                                                                         "()Ljava/lang/String;");
    return string.toString();
#else
    return ScreenManager::getInstance()->getJsReturnValue();
#endif
}


void CommonApiHelper::playSound(QString type)
{
    QString enableSoundPromptsStr =  Common::CustomerSetting::getInstance()->getValue("SoundPrompts", "EnableSoundPrompts");
    if(enableSoundPromptsStr.toUpper() == "TRUE")
    {
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject Qtype = QAndroidJniObject::fromString(type);
    jstring aType =  Qtype.object<jstring>();
    jint result = QAndroidJniObject::callStaticMethod<jint>("omnicare.AndroidHelper",
                                                            "playSound",
                                                            "(Ljava/lang/String;)I",
                                                            aType);
    qDebug() << "CommonApiHelper:playSound result = " << result;
#else
    QString soundPath = QStandardPaths::standardLocations(QStandardPaths::DataLocation)[0] + QDir::separator() + "Merchant"+ QDir::separator() +"Sounds"+ QDir::separator() + type + ".wav";
    ScreenManager::getInstance()->playNotifySound(type, soundPath);
#endif
    }
}
