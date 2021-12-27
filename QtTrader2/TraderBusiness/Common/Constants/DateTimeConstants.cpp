#include "DateTimeConstants.h"

namespace Common
{
	namespace Constants
	{
		const QString DateTimeConstants::UTCFormat ="yyyy-MM-ddThh:mm:ss";
		const QString DateTimeConstants::ColonTimeFormat = "HH:mm:ss";
		const QString DateTimeConstants::DateFormat = "yyyy-MM-dd";
		const QString DateTimeConstants::DateTimeFormat = "yyyy-MM-dd hh:mm:ss";
		const QString DateTimeConstants::NoShowSecondDateTimeFormat = "yyyy-MM-dd HH:mm";
		const QString DateTimeConstants::LongDateTimeFormat = "yyyy-MM-dd HH:mm:ss.zzz";		
		const QDateTime DateTimeConstants::MinQDateTime = QDateTime::fromString("1800-01-01 00:00:00","yyyy-MM-dd HH:mm:ss");
	}
}