#ifndef HISTORICALCHART_GLOBAL_H
#define HISTORICALCHART_GLOBAL_H

#include <QtCore/qglobal.h>
//#ifndef BUILD_STATIC
//    #ifdef HISTORICALCHART_LIB
//    # define HISTORICALCHART_EXPORT Q_DECL_EXPORT
//    #else
//    # define HISTORICALCHART_EXPORT Q_DECL_IMPORT
//    #endif
//#else
//     # define HISTORICALCHART_EXPORT
//#endif

#ifdef BUILD_DLL
    #ifdef HISTORICALCHART_LIB
    # define HISTORICALCHART_EXPORT Q_DECL_EXPORT
    #else
    # define HISTORICALCHART_EXPORT Q_DECL_IMPORT
    #endif
#else
	# define HISTORICALCHART_EXPORT
#endif

#endif // HISTORICALCHART_GLOBAL_H
