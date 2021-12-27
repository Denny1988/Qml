#ifndef LOGKEYCONSTANTS_H
#define LOGKEYCONSTANTS_H

#include <QString>
#include "traderbusiness_global.h"

namespace Common 
{
	namespace Constants
	{
		class TRADERBUSINESS_EXPORT LogKeyConstants
		{
		public:
			static const QString Application;
			static const QString Business;
			static const QString Communication;
			static const QString QuotationServerCommunication;
			static const QString Service;
			static const QString UpdateQuotation;
			static const QString PlacingOrder;
			static const QString Chart;
			static const QString BreakDump;			
		};
	}
}
#endif

