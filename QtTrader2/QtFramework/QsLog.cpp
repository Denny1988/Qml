// Copyright (c) 2013, Razvan Petru
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or other
//   materials provided with the distribution.
// * The name of the contributors may not be used to endorse or promote products
//   derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#include "QsLog.h"
#include "QsLogDest.h"
//#ifdef QS_LOG_SEPARATE_THREAD
#include <QThreadPool>
#include <QRunnable>
//#endif
#include <QMutex>
#include <QVector>
#include <QDateTime>
#include <QtGlobal>
#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include <QMap>
#include <QDir>
#include "DateTimeHelper.h"

namespace QsLogging
{
typedef QVector<DestinationPtr> DestinationList;
typedef QMap<QString,DestinationPtr>  DestinationMap;

static const char TraceString[] = "TRACE";
static const char DebugString[] = "DEBUG";
static const char InfoString[]  = "INFO ";
static const char WarnString[]  = "WARN ";
static const char ErrorString[] = "ERROR";
static const char FatalString[] = "FATAL";

// not using Qt::ISODate because we need the milliseconds too

static Logger* sInstance = 0;

static const char* LevelToText(Level theLevel)
{
    switch (theLevel) {
        case TraceLevel:
            return TraceString;
        case DebugLevel:
            return DebugString;
        case InfoLevel:
            return InfoString;
        case WarnLevel:
            return WarnString;
        case ErrorLevel:
            return ErrorString;
        case FatalLevel:
            return FatalString;
        case OffLevel:
            return "";
        default: {
            assert(!"bad log level");
            return InfoString;
        }
    }
}

//#ifdef QS_LOG_SEPARATE_THREAD
class LogWriterRunnable : public QRunnable
{
public:
    LogWriterRunnable(QString message, Level level,const QString& fileName);
    virtual void run();

private:
    QString mMessage;
    Level mLevel;
	QString mFileName;
};
//#endif

class LoggerImpl
{
public:
    LoggerImpl();

//#ifdef QS_LOG_SEPARATE_THREAD
    QThreadPool threadPool;
//#endif
    QMutex logMutex;
    Level level;
   /* DestinationList destList;*/
	//DestinationPtr  destPtr;
	DestinationMap	 destMap;
};

//#ifdef QS_LOG_SEPARATE_THREAD
LogWriterRunnable::LogWriterRunnable(QString message, Level level,const QString& fileName)
    : QRunnable()
    , mMessage(message)
    , mLevel(level)
	, mFileName(fileName)
{
}

void LogWriterRunnable::run()
{
    Logger::instance().write(mMessage, mLevel,mFileName);
}
//#endif


LoggerImpl::LoggerImpl()
    : level(InfoLevel)
{
    // assume at least file + console
    /*destList.reserve(2);*/
//#ifdef QS_LOG_SEPARATE_THREAD
    threadPool.setMaxThreadCount(1);
    threadPool.setExpiryTimeout(-1);
//#endif
}


Logger::Logger()
    : d(new LoggerImpl)
{
}

Logger& Logger::instance()
{
    if (!sInstance)
        sInstance = new Logger;

    return *sInstance;
}

void Logger::destroyInstance()
{
    delete sInstance;
    sInstance = 0;
}

// tries to extract the level from a string log message. If available, conversionSucceeded will
// contain the conversion result.
Level Logger::levelFromLogMessage(const QString& logMessage, bool* conversionSucceeded)
{
    if (conversionSucceeded)
        *conversionSucceeded = true;

    if (logMessage.startsWith(QLatin1String(TraceString)))
        return TraceLevel;
    if (logMessage.startsWith(QLatin1String(DebugString)))
        return DebugLevel;
    if (logMessage.startsWith(QLatin1String(InfoString)))
        return InfoLevel;
    if (logMessage.startsWith(QLatin1String(WarnString)))
        return WarnLevel;
    if (logMessage.startsWith(QLatin1String(ErrorString)))
        return ErrorLevel;
    if (logMessage.startsWith(QLatin1String(FatalString)))
        return FatalLevel;

    if (conversionSucceeded)
        *conversionSucceeded = false;
    return OffLevel;
}

Logger::~Logger()
{
//#ifdef QS_LOG_SEPARATE_THREAD
    d->threadPool.waitForDone();
//#endif
    delete d;
    d = 0;
}

void Logger::addDestination(DestinationPtr destination,const QString& key )
{
    assert(destination.data());
    /*d->destList.push_back(destination);*/
	/*d->destPtr = destination;*/
	d->destMap[key] = destination;
}

void Logger::setLoggingLevel(Level newLevel)
{
    d->level = newLevel;
}

Level Logger::loggingLevel() const
{
    return d->level;
}

//! creates the complete log message and passes it to the logger
void Logger::Helper::writeToLog()
{
    const char* const levelName = LevelToText(level);
    const QString completeMessage(QString("%1 %2 %3")
                                  .arg(levelName)
                                  .arg(DateTimeHelper::toLongDateTimeString(QDateTime::currentDateTime()))
                                  .arg(buffer)
                                  );

    Logger::instance().enqueueWrite(completeMessage, level,fileName);
}

Logger::Helper::~Helper()
{
    try {
        writeToLog();
    }
    catch(std::exception&) {
        // you shouldn't throw exceptions from a sink
        //assert(!"exception in logger helper destructor");
        //throw;
    }
}

//! directs the message to the task queue or writes it directly
void Logger::enqueueWrite(const QString& message, Level level,const QString& fileName)
{
//#ifdef QS_LOG_SEPARATE_THREAD
    LogWriterRunnable *r = new LogWriterRunnable(message, level,fileName);
    d->threadPool.start(r);
//#else
//    write(message, level);
//#endif
}

//! Sends the message to all the destinations. The level for this message is passed in case
//! it's useful for processing in the destination.
void Logger::write(const QString& message, Level level,const QString& fileName)
{
        QMutexLocker lock(&d->logMutex);

	if(d->destMap.contains(fileName))
	{
		d->destMap[fileName]->write(message,level);
	}
}

void Logger::addKeyIfNotExist(const QString& key)
{
	
	if(!d->destMap.contains(key))
	{
		DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
			basePath + key, DisableLogRotation, MaxSizeBytes(512), MaxOldLogCount(2)));
		d->destMap[key] = fileDestination;
	}
}

void Logger::setBasePath(const QString& path)
{
#ifdef RUNTIME_OS_WIN32
    basePath.clear();
	if(path.endsWith('\\') || path.endsWith( '/'))
	{
		basePath = path;
	}
	else
	{
		basePath = path + "\\";
    }
	QDir dir;
    if(!dir.exists(basePath))
	{
        dir.mkpath(basePath);
	}
#endif

#ifdef RUNTIME_OS_ANDROID
    basePath.clear();
    if(path.endsWith( '/'))
    {
        basePath = path;
    }
    else
    {
        basePath = path + "/";
    }
    QDir dir;
    if(!dir.exists(basePath))
    {
        dir.mkpath(basePath);
    }
#endif

#ifdef RUNTIME_OS_IPHONE
    basePath.clear();
    if(path.endsWith( '/'))
    {
        basePath = path;
    }
    else
    {
        basePath = path + "/";
    }
    QDir dir;
    if(!dir.exists(basePath))
    {
        dir.mkpath(basePath);
    }
#endif

#ifdef RUNTIME_OS_MACX
    basePath.clear();
    if(path.endsWith( '/'))
    {
        basePath = path;
    }
    else
    {
        basePath = path + "/";
    }
    QDir dir;
    if(!dir.exists(path))
    {
        dir.mkpath(path);
    }
#endif
}

} // end namespace
