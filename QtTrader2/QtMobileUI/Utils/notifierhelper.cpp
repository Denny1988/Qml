#include "notifierhelper.h"
#include <QDebug>
#include <QByteArray>

#if defined(RUNTIME_OS_ANDROID)
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroid>
#endif


NotifierHelper::NotifierHelper()
{

}

void NotifierHelper::notify(QString contentStr)
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject javaNotification = QAndroidJniObject::fromString(contentStr);
    QAndroidJniObject::callStaticMethod<void>("omnicare/NotifierHelper",
                                       "notify",
                                       "(Ljava/lang/String;)V",
                                       javaNotification.object<jstring>());
#endif
}
