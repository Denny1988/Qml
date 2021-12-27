#include "biometricprompthelper.h"
#include <QDebug>
#include <QByteArray>
#include "stdafx.h"

#if defined(RUNTIME_OS_ANDROID)
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroid>
#else
#include "IOSLib/BiometricPromptApiHelper.h"
#endif


BiometricPromptHelper::BiometricPromptHelper()
{

}

QString BiometricPromptHelper::openFingerLogin(){
    #if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject resultString = QAndroidJniObject::callStaticObjectMethod<jstring>("omnicare/BiometricHelper", "openFingerLogin");
    return resultString.toString();
#else
    BiometricPromptApiHelper helper;
    helper.fingerLogin();
    return "";
#endif
}

QString BiometricPromptHelper::fingerLogin(){
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject resultString = QAndroidJniObject::callStaticObjectMethod<jstring>("omnicare/BiometricHelper", "fingerLogin");
    return resultString.toString();
#else
    BiometricPromptApiHelper helper;
    helper.fingerLogin();
    return "";
#endif
}

QString BiometricPromptHelper::checkHardware()
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject resultString = QAndroidJniObject::callStaticObjectMethod<jstring>("omnicare/BiometricHelper", "checkHardware");
    return resultString.toString();
#else
    return "";
#endif
}

int BiometricPromptHelper::getOpenFingerLoginStatus()
{
#if defined(RUNTIME_OS_ANDROID)
    jint resultString  = QAndroidJniObject::callStaticMethod<jint>("omnicare/BiometricHelper", "getOpenFingerLoginStatus", "()I");
    return (int)resultString;
#else
    BiometricPromptApiHelper helper;
    return helper.getFingerLoginStatus();
#endif
}

int BiometricPromptHelper::getFingerLoginStatus()
{
#if defined(RUNTIME_OS_ANDROID)
    jint resultString  = QAndroidJniObject::callStaticMethod<jint>("omnicare/BiometricHelper", "getFingerLoginStatus", "()I");
    return (int)resultString;
#else
    BiometricPromptApiHelper helper;
    return helper.getFingerLoginStatus();
#endif
}

void BiometricPromptHelper::resetOpenFingerLoginStatus()
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject::callStaticMethod<jboolean>("omnicare/BiometricHelper", "resetOpenFingerLoginStatus", "()Z");
#else
    BiometricPromptApiHelper helper;
    helper.resetFingerLoginStatus();
#endif
}

void BiometricPromptHelper::resetFingerLoginStatus()
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject::callStaticMethod<jboolean>("omnicare/BiometricHelper", "resetFingerLoginStatus", "()Z");
#else
    BiometricPromptApiHelper helper;
    helper.resetFingerLoginStatus();
#endif
}

QString BiometricPromptHelper::getBiometricPromptIV()
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject resultString = QAndroidJniObject::callStaticObjectMethod<jstring>("omnicare/BiometricHelper", "getBiometricPromptIV");
    return resultString.toString();
#else
    return "";
#endif
}

void BiometricPromptHelper::setBiometricPromptIV(QString iv)
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject ivMessage = QAndroidJniObject::fromString(iv);
    jstring message =  ivMessage.object<jstring>();
    jboolean result = QAndroidJniObject::callStaticMethod<jboolean>("omnicare/BiometricHelper", "setBiometricPromptIV", "(Ljava/lang/String;)Z", message);
#else
    return;
#endif
}


QString BiometricPromptHelper::getFingerLoginStatusStr(int status)
{
    if(status == -1)
        return g_Languages.Tr("SystemVersionNotSupportTouchID", "DialogWindow");
    else if(status == -2)
            return g_Languages.Tr("AuthorizationFailed", "DialogWindow");
    else if(status == -3)
            return g_Languages.Tr("UserCancelVerificationOfTouchID", "DialogWindow");
    else if(status == -4)
            return g_Languages.Tr("UserChoosesEnterPassword", "DialogWindow");
    else if(status == -5)
            return g_Languages.Tr("CancelAuthorizationForOtherApplication", "DialogWindow");
    else if(status == -6)
            return g_Languages.Tr("TheDeviceSystemNoPassword", "DialogWindow");
    else if(status == -7)
            return g_Languages.Tr("TouchIDNotSetOnTheDevice", "DialogWindow");
    else if(status == -8)
            return g_Languages.Tr("UserNotEnterFingerprint", "DialogWindow");
    else if(status == -9)
            return g_Languages.Tr("TouchIDLockedAndRequiresEnterPassword", "DialogWindow");
    else if(status == -10)
            return g_Languages.Tr("APPSuspendedWhenUserCannotControl", "DialogWindow");
    else if(status == -11)
            return g_Languages.Tr("LAContextHasInvalidated", "DialogWindow");
    else if(status == -12)
            return g_Languages.Tr("InOtherCasesSwitchMainThreadProcessing", "DialogWindow");
    else if(status == -13)
            return g_Languages.Tr("AuthenticationNoStartTouchIDNotEnrolledFingers", "DialogWindow");
    else if(status == -14)
            return g_Languages.Tr("AuthenticationNoStartPasswordNotSet", "DialogWindow");
    else if(status == -15)
            return g_Languages.Tr("TouchIDNotAvailable", "DialogWindow");
}
