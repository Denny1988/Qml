#ifndef QTCUSTOMWIDGET_GLOBAL_H
#define QTCUSTOMWIDGET_GLOBAL_H

#include <QtCore/qglobal.h>
#ifdef BUILD_DLL
    #ifdef QTCUSTOMWIDGET_LIB
    # define QTCUSTOMWIDGET_EXPORT Q_DECL_EXPORT
    #else
    # define QTCUSTOMWIDGET_EXPORT Q_DECL_IMPORT
    #endif
#else
    # define QTCUSTOMWIDGET_EXPORT
#endif

#endif // QTCUSTOMWIDGET_GLOBAL_H
