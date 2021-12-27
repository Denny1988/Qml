#ifndef COMMON_LOGHELPER_H
#define COMMON_LOGHELPER_H
#include <QString>
#include "QsLog.h"
#include "Common/Constants/LogKeyConstants.h"
#include "traderbusiness_global.h"
#include "Common/Enum/LogLevelEnum.h"
#include <qdebug.h>

//namespace Common
//{
//    class TRADERBUSINESS_EXPORT Helper
//    {
//    public:
//        Helper():qtDebug(&buffer){}
//        ~Helper();
//        QDebug& stream(){ return qtDebug; }

//    private:
//        QString buffer;
//        QDebug qtDebug;
//    };

//    class TRADERBUSINESS_EXPORT LogHelper
//	{
//    private:
//        static Helper* helper;

//    public:
//        static QDebug writeLog(LogLevel level, QString key)
//        {
//            if(level == LogLevel::InfoLevel)
//                return QsLogging::Logger::Helper(QsLogging::InfoLevel,key).stream();
//            else if(level == LogLevel::WarnLevel)
//                return QsLogging::Logger::Helper(QsLogging::WarnLevel,key).stream();
//            else if(level == LogLevel::DebugLevel)
//                return QsLogging::Logger::Helper(QsLogging::DebugLevel,key).stream();
//            else if(level == LogLevel::ErrorLevel)
//                return QsLogging::Logger::Helper(QsLogging::ErrorLevel,key).stream();
//            else if(level == LogLevel::FatalLevel)
//                return QsLogging::Logger::Helper(QsLogging::FatalLevel,key).stream();
//            else if(level == LogLevel::TraceLevel)
//                return QsLogging::Logger::Helper(QsLogging::TraceLevel,key).stream();
//        }

//        static void writeLog(LogLevel level, QString key, QString content)
//        {
//            if (level == LogLevel::InfoLevel)
//                QLOG_INFO(key) << content;
//            else if (level == LogLevel::WarnLevel)
//                QLOG_WARN(key) << content;
//            else if (level == LogLevel::DebugLevel)
//                QLOG_DEBUG(key) << content;
//            else if (level == LogLevel::ErrorLevel)
//                QLOG_ERROR(key) << content;
//            else if (level == LogLevel::FatalLevel)
//                QLOG_FATAL(key) << content;
//            else if (level == LogLevel::TraceLevel)
//                QLOG_TRACE(key) << content;
//        }

//		static void writeLog(LogLevel level, QString key, const char content)
//        {
//			if (level == LogLevel::InfoLevel)
//				QLOG_INFO(key) << content;
//			else if (level == LogLevel::WarnLevel)
//				QLOG_WARN(key) << content;
//			else if (level == LogLevel::DebugLevel)
//				QLOG_DEBUG(key) << content;
//			else if (level == LogLevel::ErrorLevel)
//				QLOG_ERROR(key) << content;
//			else if (level == LogLevel::FatalLevel)
//				QLOG_FATAL(key) << content;
//			else if (level == LogLevel::TraceLevel)
//                QLOG_TRACE(key) << content;
//		}

//	};
//}

//
//#ifdef WIN32
//#define LOGHELPER_TRACE(key) \
//    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::TraceLevel) {} \
//    else QsLogging::Logger::Helper(QsLogging::TraceLevel,key).stream()
//#define LOGHELPER_DEBUG(key) \
//	QLOG_DEBUG(key) << QsLogging::Logger::Helper(QsLogging::DebugLevel, key).stream();
//#define LOGHELPER_INFO(key)  \
//    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::InfoLevel) {} \
//    else QsLogging::Logger::Helper(QsLogging::InfoLevel,key).stream()
//#define LOGHELPER_WARN(key)  \
//    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::WarnLevel) {} \
//    else QsLogging::Logger::Helper(QsLogging::WarnLevel,key).stream()
//#define LOGHELPER_ERROR(key) \
//    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::ErrorLevel) {} \
//    else QsLogging::Logger::Helper(QsLogging::ErrorLevel,key).stream()
//#define LOGHELPER_FATAL(key) \
//    if (QsLogging::Logger::instance().loggingLevel() > QsLogging::FatalLevel) {} \
//    else QsLogging::Logger::Helper(QsLogging::FatalLevel,key).stream()
//#endif

#endif
