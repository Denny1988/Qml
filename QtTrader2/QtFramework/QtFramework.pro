# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = lib
TARGET = QtFramework

win32 {
    DESTDIR = ../Release
    message("QtFramework win32")
    QT += core gui axserver axcontainer widgets
    CONFIG  += staticlib

    DEFINES += WIN64 WIN32 UNICODE QTFRAMEWORK_LIB QT_WIDGETS_LIB RUNTIME_OS_WIN32
    INCLUDEPATH += $$PWD/Win32/GeneratedFiles \
        . \
        $(QTDIR)\include\ActiveQt \
        $$PWD/Win32/GeneratedFiles
    DEPENDPATH += .
    MOC_DIR += $$PWD/Win32/GeneratedFiles
    OBJECTS_DIR += $$PWD/Win32/Release
    UI_DIR += $$PWD/Win32/GeneratedFiles
    RCC_DIR += $$PWD/Win32/GeneratedFiles

    HEADERS += $$PWD/browserWrapper.h \
        $$PWD/globelmacro.h \
        $$PWD/QsLog.h \
        $$PWD/QsLogDest.h \
        $$PWD/QsLogDestFile.h \
        $$PWD/QsLogDisableForThisFile.h \
        $$PWD/QsLogLevel.h
    SOURCES += $$PWD/browserWrapper.cpp \
        $$PWD/QsLog.cpp \
        $$PWD/QsLogDest.cpp \
        $$PWD/QsLogDestFile.cpp
}

unix:!mac {    
    message("QtFramework unix:Android ")
    QT += core
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

    HEADERS += $$PWD/globelmacro.h \
        $$PWD/QsLog.h \
        $$PWD/QsLogDest.h \
        $$PWD/QsLogDestFile.h \
        $$PWD/QsLogDisableForThisFile.h \
        $$PWD/QsLogLevel.h
    SOURCES += $$PWD/QsLog.cpp \
        $$PWD/QsLogDest.cpp \
        $$PWD/QsLogDestFile.cpp
}

unix:mac:!macx {
    DESTDIR = $$PWD/../IOS
    message("QtFramework unix:Iphone ")
    QT += core
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

    HEADERS += $$PWD/globelmacro.h \
        $$PWD/QsLog.h \
        $$PWD/QsLogDest.h \
        $$PWD/QsLogDestFile.h \
        $$PWD/QsLogDisableForThisFile.h \
        $$PWD/QsLogLevel.h
    SOURCES += $$PWD/QsLog.cpp \
        $$PWD/QsLogDest.cpp \
        $$PWD/QsLogDestFile.cpp
}

unix:mac:macx  {
    DESTDIR = ../Release
    message("QtFramework unix:mac:macx  ")
    QT += core webenginewidgets
    CONFIG  += staticlib
    DEFINES += ARM QTFRAMEWORK_LIB BUILD_STATIC RUNTIME_OS_MACX

    INCLUDEPATH += $$PWD/Macx/GeneratedFiles \
        . \
        $$PWD/Macx/GeneratedFiles/Release
    DEPENDPATH += .
    MOC_DIR += $$PWD/Macx/GeneratedFiles/Release
    OBJECTS_DIR += $$PWD/Macx/Release
    UI_DIR += $$PWD/Macx/GeneratedFiles
    RCC_DIR += $$PWD/Macx/GeneratedFiles

    HEADERS += $$PWD/browserWrapper.h \
        $$PWD/globelmacro.h \
        $$PWD/QsLog.h \
        $$PWD/QsLogDest.h \
        $$PWD/QsLogDestFile.h \
        $$PWD/QsLogDisableForThisFile.h \
        $$PWD/QsLogLevel.h
    SOURCES += $$PWD/browserWrapper.cpp \
        $$PWD/QsLog.cpp \
        $$PWD/QsLogDest.cpp \
        $$PWD/QsLogDestFile.cpp
}

HEADERS += \
    DateTimeHelper.h \
    commonhelper.h

SOURCES += \
    commonhelper.cpp




