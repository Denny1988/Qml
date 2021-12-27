TEMPLATE = lib
TARGET = QtCustomWidget

unix:!mac {    
    message("QtFramework unix:Android ")
    QT += core gui widgets
    CONFIG  += staticlib
    DEFINES += ARM QTFRAMEWORK_LIB RUNTIME_OS_ANDROID

    CONFIG(debug, debug|release){
        DESTDIR = ../Debug
        INCLUDEPATH += $$PWD/Unix_Debug/GeneratedFiles \
            . \
            $$PWD/Unix_Debug/GeneratedFiles/Debug
        DEPENDPATH += .
        MOC_DIR += $$PWD/Unix_Debug/GeneratedFiles/Debug
        OBJECTS_DIR += $$PWD/Unix_Debug/Debug
        UI_DIR += $$PWD/Unix_Debug/GeneratedFiles
        RCC_DIR += $$PWD/Unix_Debug/GeneratedFiles
    } else {
        DESTDIR = ../Release
        INCLUDEPATH += $$PWD/Unix/GeneratedFiles \
            . \
            $$PWD/Unix/GeneratedFiles/Release
        DEPENDPATH += .
        MOC_DIR += $$PWD/Unix/GeneratedFiles/Release
        OBJECTS_DIR += $$PWD/Unix/Release
        UI_DIR += $$PWD/Unix/GeneratedFiles
        RCC_DIR += $$PWD/Unix/GeneratedFiles
    }
}

unix:mac:!macx {
    DESTDIR = $$PWD/../IOS
    message("QtFramework unix:Iphone ")
    QT += core gui widgets
    CONFIG  += staticlib
    DEFINES += ARM QTFRAMEWORK_LIB BUILD_STATIC RUNTIME_OS_IPHONE

    INCLUDEPATH += $$PWD/IOS/GeneratedFiles \
        . \
        $$PWD/IOS/GeneratedFiles/Release
    DEPENDPATH += .
    MOC_DIR += $$PWD/IOS/GeneratedFiles/Release
    OBJECTS_DIR += $$PWD/IOS/Release
    UI_DIR += $$PWD/IOS/GeneratedFiles
    RCC_DIR += $$PWD/IOS/GeneratedFiles
}

include(QtCustomWidget.pri)



