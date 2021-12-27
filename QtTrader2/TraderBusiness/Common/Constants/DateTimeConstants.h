#ifndef DATETIMECONSTANTS_H
#define DATETIMECONSTANTS_H
#include <qstring.h>
#include <QDateTime>
#include "traderbusiness_global.h"

namespace Common
{
	namespace Constants
	{
		class TRADERBUSINESS_EXPORT DateTimeConstants
		{
		private:
			DateTimeConstants(){}
		public:
			static const QString UTCFormat;
			static const QString ColonTimeFormat;
			static const QString DateFormat;
			static const QString DateTimeFormat;
			static const QString NoShowSecondDateTimeFormat;
			static const QDateTime MinQDateTime;			
			static const QString LongDateTimeFormat;
		};
	}
}
#endif
