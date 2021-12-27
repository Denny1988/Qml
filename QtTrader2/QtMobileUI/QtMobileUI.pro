QT += qml quick core gui xml network xmlpatterns quickcontrols2 gui-private webview webchannel websockets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets quickwidgets

CONFIG+= warn_off

#develop on Android
unix:!mac {
    message("unix:!mac：Android develp on windows")
    TARGET = QtMobileUI
    QT  += androidextras
    DEFINES += ARM RUNTIME_OS_ANDROID APPPUBLISH

    CONFIG(debug, debug|release){
            LIBS += -L$$PWD/../Unix_Debug/Debug/ -lHistoricalChart
            INCLUDEPATH += $$PWD/../HistoricalChart
            DEPENDPATH += $$PWD/../HistoricalChart
            PRE_TARGETDEPS += $$PWD/../Unix_Debug/Debug/libHistoricalChart.a

            LIBS += -L$$PWD/../Unix_Debug/Debug/ -lTraderBusiness
            INCLUDEPATH += $$PWD/../TraderBusiness
            DEPENDPATH += $$PWD/../TraderBusiness
            PRE_TARGETDEPS += $$PWD/../Unix_Debug/Debug/libTraderBusiness.a

            LIBS += -L$$PWD/../Unix_Debug/Debug/ -lqdecimal
            INCLUDEPATH += $$PWD/../QDecimal
            DEPENDPATH += $$PWD/../QDecimal
            PRE_TARGETDEPS += $$PWD/../Unix_Debug/Debug/libqdecimal.a

            LIBS += -L$$PWD/../Unix_Debug/Debug/ -lTA-SDK
            INCLUDEPATH += $$PWD/../TA-SDK
            DEPENDPATH += $$PWD/../TA-SDK
            PRE_TARGETDEPS += $$PWD/../Unix_Debug/Debug/libTA-SDK.a

            LIBS += -L$$PWD/../Unix_Debug/Debug/ -lQtFramework
            INCLUDEPATH += $$PWD/../QtFramework
            DEPENDPATH += $$PWD/../QtFramework
            PRE_TARGETDEPS += $$PWD/../Unix_Debug/Debug/libQtFramework.a

            LIBS += -L$$PWD/../Unix_Debug/Debug/ -lQtCustomWidget
            PRE_TARGETDEPS += $$PWD/../Unix_Debug/Debug/libQtCustomWidget.a
            INCLUDEPATH += $$PWD/../QtCustomWidget
            DEPENDPATH += $$PWD/../QtCustomWidget
    } else {
            LIBS += -L$$PWD/../Unix/Release/ -lHistoricalChart
            INCLUDEPATH += $$PWD/../HistoricalChart
            DEPENDPATH += $$PWD/../HistoricalChart
            PRE_TARGETDEPS += $$PWD/../Unix/Release/libHistoricalChart.a

            LIBS += -L$$PWD/../Unix/Release/ -lTraderBusiness
            INCLUDEPATH += $$PWD/../TraderBusiness
            DEPENDPATH += $$PWD/../TraderBusiness
            PRE_TARGETDEPS += $$PWD/../Unix/Release/libTraderBusiness.a

            LIBS += -L$$PWD/../Unix/Release/ -lqdecimal
            INCLUDEPATH += $$PWD/../QDecimal
            DEPENDPATH += $$PWD/../QDecimal
            PRE_TARGETDEPS += $$PWD/../Unix/Release/libqdecimal.a

            LIBS += -L$$PWD/../Unix/Release/ -lTA-SDK
            INCLUDEPATH += $$PWD/../TA-SDK
            DEPENDPATH += $$PWD/../TA-SDK
            PRE_TARGETDEPS += $$PWD/../Unix/Release/libTA-SDK.a

            LIBS += -L$$PWD/../Unix/Release/ -lQtFramework
            INCLUDEPATH += $$PWD/../QtFramework
            DEPENDPATH += $$PWD/../QtFramework
            PRE_TARGETDEPS += $$PWD/../Unix/Release/libQtFramework.a

            LIBS += -L$$PWD/../Unix/Release/ -lQtCustomWidget
            PRE_TARGETDEPS += $$PWD/../Unix/Release/libQtCustomWidget.a
            INCLUDEPATH += $$PWD/../QtCustomWidget
            DEPENDPATH += $$PWD/../QtCustomWidget
    }
}

#develop on IPHONE
unix:mac {
    message("unix:mac：develop on mac")
    TARGET = iExPro

    DEFINES += RUNTIME_OS_IPHONE APPPUBLISH

    LIBS += -L$$PWD/../IOS/ -lHistoricalChart
    INCLUDEPATH += $$PWD/../HistoricalChart
    DEPENDPATH += $$PWD/../HistoricalChart
    PRE_TARGETDEPS += $$PWD/../IOS/libHistoricalChart.a

    LIBS += -L$$PWD/../IOS/ -lTraderBusiness
    INCLUDEPATH += $$PWD/../TraderBusiness
    DEPENDPATH += $$PWD/../TraderBusiness
    PRE_TARGETDEPS += $$PWD/../IOS/libTraderBusiness.a

    LIBS += -L$$PWD/../IOS/ -lqdecimal
    INCLUDEPATH += $$PWD/../QDecimal
    DEPENDPATH += $$PWD/../QDecimal
    PRE_TARGETDEPS += $$PWD/../IOS/libqdecimal.a

    LIBS += -L$$PWD/../IOS/ -lTA-SDK
    INCLUDEPATH += $$PWD/../TA-SDK
    DEPENDPATH += $$PWD/../TA-SDK
    PRE_TARGETDEPS += $$PWD/../IOS/libTA-SDK.a

    LIBS += -L$$PWD/../IOS/ -lQtFramework
    INCLUDEPATH += $$PWD/../QtFramework
    DEPENDPATH += $$PWD/../QtFramework
    PRE_TARGETDEPS += $$PWD/../IOS/libQtFramework.a

    LIBS += -L$$PWD/../IOS/ -lQtCustomWidget
    PRE_TARGETDEPS += $$PWD/../IOS/libQtCustomWidget.a
    INCLUDEPATH += $$PWD/../QtCustomWidget
    DEPENDPATH += $$PWD/../QtCustomWidget
}

TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
MOC_DIR += $$PWD/GeneratedFiles
CONFIG(debug, debug|release){
    OBJECTS_DIR += $$PWD/GeneratedFiles/Debug
} else {
    OBJECTS_DIR += $$PWD/GeneratedFiles/Release
}
UI_DIR += $$PWD/GeneratedFiles
RCC_DIR += $$PWD/GeneratedFiles

include(QtMobileUI.pri)

CONFIG += mobility
CONFIG += c++11
MOBILITY =


#develop on IPHONE
unix:mac {
    message("unix:mac：develop on mac")
    TARGET = iExPro

    DEFINES += RUNTIME_OS_IPHONE APPPUBLISH

    LIBS += -L$$PWD/../IOS/ -lHistoricalChart
    INCLUDEPATH += $$PWD/../HistoricalChart
    DEPENDPATH += $$PWD/../HistoricalChart
    PRE_TARGETDEPS += $$PWD/../IOS/libHistoricalChart.a

    LIBS += -L$$PWD/../IOS/ -lTraderBusiness
    INCLUDEPATH += $$PWD/../TraderBusiness
    DEPENDPATH += $$PWD/../TraderBusiness
    PRE_TARGETDEPS += $$PWD/../IOS/libTraderBusiness.a

    LIBS += -L$$PWD/../IOS/ -lqdecimal
    INCLUDEPATH += $$PWD/../QDecimal
    DEPENDPATH += $$PWD/../QDecimal
    PRE_TARGETDEPS += $$PWD/../IOS/libqdecimal.a

    LIBS += -L$$PWD/../IOS/ -lTA-SDK
    INCLUDEPATH += $$PWD/../TA-SDK
    DEPENDPATH += $$PWD/../TA-SDK
    PRE_TARGETDEPS += $$PWD/../IOS/libTA-SDK.a

    LIBS += -L$$PWD/../IOS/ -lQtFramework
    INCLUDEPATH += $$PWD/../QtFramework
    DEPENDPATH += $$PWD/../QtFramework
    PRE_TARGETDEPS += $$PWD/../IOS/libQtFramework.a

    LIBS += -L$$PWD/../IOS/ -lQtCustomWidget
    PRE_TARGETDEPS += $$PWD/../IOS/libQtCustomWidget.a
    INCLUDEPATH += $$PWD/../QtCustomWidget
    DEPENDPATH += $$PWD/../QtCustomWidget
}



#langauge
TRANSLATIONS += \
  res/tr_eng.ts \
  res/tr_chs.ts

#lupdate_only {
#    SOURCES = *.qml \
#}


unix:mac {
    message("unix:mac：add ios files for mac build")

    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
    QMAKE_CXXFLAGS += -std=c++14 -stdlib=libc++
    #QMAKE_LFLAGS += -stdlib=libc++ #-Objc -all_load
    ##QMAKE_LFLAGS += -ObjC -force_load

    QMAKE_TARGET_BUNDLE_PREFIX = hk.com.omnicare.www
    #QMAKE_DEVELOPMENT_TEAM = omnicare
    #QMAKE_PROVISIONING_PROFILE = xxx (XCode managed profile)

    LIBS += -lz -lsqlite3 -framework UIKit -framework WebKit -framework Foundation -framework CFNetwork -framework Security -framework CoreTelephony -framework SystemConfiguration -framework LocalAuthentication -framework NotificationCenter -framework Photos -framework AudioToolbox\
                $$PWD/IOSLib/libWeChatSDK.a

    LIBS += -F$$PWD/ios/ -framework
    INCLUDEPATH += $$PWD/ios
    DEPENDPATH += $$PWD/ios
    INCLUDEPATH += $$PWD/IOSLib
    DEPENDPATH += $$PWD/IOSLib

    #DISTFILES += ios/Bugly.framework/HEADERS/*


}
