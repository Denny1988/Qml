#ifndef QUOTATIONCONSTANTS_H
#define QUOTATIONCONSTANTS_H

#include <QString>
#include <QDateTime>

namespace Common 
{
	namespace Constants
	{
		class QuotationConstants
		{
		public:
			static const char _BitsToChar[];
			static QDateTime _OrginTime;	
			static const char _InnerSeparator;
			static const char _StartSeparator;
			static const char _OutterSeparator;
			static const char _SequenceSeparator;
		};
	}
}
#endif

