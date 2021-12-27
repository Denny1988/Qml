#ifndef INFORMATIONSTRUCT_H
#define INFORMATIONSTRUCT_H
#include <QUuid>
#include "Common/Enum/InformationCenterEnum.h"
using namespace Common::Enum;

namespace Common
{
	namespace Struct
	{
		struct InformationStruct
		{
			QUuid  _id;
			QString  _title;
			InformationCenterEnum::InformationCenterEnum  _informationCenterEnum;		
			bool _isRead;
		};
	}
}
#endif
