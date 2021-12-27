#include "galleryhelper.h"
#include "screenmanager.h"

#if defined(RUNTIME_OS_ANDROID)
#include <QtAndroidExtras>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroid>

QString selectedFileName;
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_omnicare_MainActivity_fileSelected(JNIEnv */*env*/,
                                                             jobject /*obj*/,
                                                             jstring results)
{
    selectedFileName = QAndroidJniObject(results).toString();
}

#ifdef __cplusplus
}
#endif

#else

#endif


GalleryHelper::GalleryHelper(){}

QString GalleryHelper::getGalleryImagePath(){
#if defined(RUNTIME_OS_ANDROID)
    selectedFileName = "#";
    QAndroidJniObject::callStaticMethod<void>("omnicare/MainActivity",
                                              "openAnImage",
                                              "()V");

    while(selectedFileName == "#")
        qApp->processEvents();

    qDebug()<<"file name:"<<selectedFileName;
    return selectedFileName == ":(" ? "" : selectedFileName;
#else
    return "";
#endif
}

bool GalleryHelper::saveImageToGalleryByPath(const QString& imgPath)
{
#if defined(RUNTIME_OS_ANDROID)
    QAndroidJniObject QPath = QAndroidJniObject::fromString(imgPath);
    jstring aPath =  QPath.object<jstring>();
    jboolean result = QAndroidJniObject::callStaticMethod<jboolean>("omnicare/MainActivity",
                                              "saveImageToGalleryByPath",
                                              "(Ljava/lang/String;)Z",
                                               aPath);
    return result;
#else
    return false;
#endif
}
