#ifndef SETTINGSTRUCT_H
#define SETTINGSTRUCT_H
#include <QUuid>
#include <qdatetime.h>
#include <QSharedPointer>
#include "QDecNumber.h"
#include "Common/Enum/InstrumentEnum.h"
#include <math.h>
using namespace Common::Enum;

namespace Common
{
	namespace Struct
	{
		struct SettingAccount
		{
			QUuid  _id;
			QString  _code;
			QString  _name;
			QString  _salesCode;
		};

		struct SettingInstrument
		{
			QUuid  _id;
			QString  _originCode;
			QString  _code;
			QString  _description;
			bool _showPriceSuperscript;
			int _enhancedPriceDigit;
			int  _numeratorUnit;
			int  _denominator;		
			bool _isNormal;
			QString _groupName;
			int _sequence;
            int _tmpSequence;
			bool _selected;
			int _guidMappingValue;
			InstrumentCategory::InstrumentCategory _instrumentCategory;
            QString _officialFixingDate;
            int getDecimalPlaces(int defaultValue)
			{
                int decimalPlaces = log10(_denominator);
				if (decimalPlaces < 0)
				{
                    decimalPlaces = defaultValue;
				}
				return decimalPlaces;
			}
		};

		struct SettingQuotePolicy
		{
			QUuid  _id;
			QString  _code;
		};

		struct InstrumentQuotePolicyMapping
		{
			QUuid  _quotePolicyId;
			QUuid  _quotePolicyCode;
		};
	}
}
#endif
