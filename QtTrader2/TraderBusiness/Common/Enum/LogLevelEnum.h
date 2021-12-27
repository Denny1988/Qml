#ifndef LOGLEVEL_H
#define LOGLEVEL_H

namespace Common
{
	enum LogLevel
	{
		TraceLevel = 0,
		DebugLevel,
		InfoLevel,
		WarnLevel,
		ErrorLevel,
		FatalLevel,
		OffLevel
	};
}
#endif 
