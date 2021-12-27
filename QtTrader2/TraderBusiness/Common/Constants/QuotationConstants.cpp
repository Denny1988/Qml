#include "Common/Constants/QuotationConstants.h"
#include <QDate>
#include <QTime>
#include <QTimeZone>

namespace Common 
{
	namespace Constants
	{
		const char QuotationConstants::_BitsToChar[] =  { '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';' };
		QDateTime  QuotationConstants::_OrginTime = QDateTime(QDate(2011, 4, 1), QTime(0, 0, 0), QTimeZone::utc());
		const char QuotationConstants::_InnerSeparator = ':';
		const char QuotationConstants::_StartSeparator = '/';
		const char QuotationConstants::_OutterSeparator = ';';
		const char QuotationConstants::_SequenceSeparator = '-';
	}
}