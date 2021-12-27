#ifndef BUSINESS_MODEL_SETTING_DELIVERYHOLIDAY_H
#define BUSINESS_MODEL_SETTING_DELIVERYHOLIDAY_H
#include "QDateTime.h"
#include <QDomNode>
#include "Common/Constants/DateTimeConstants.h"
using namespace Common::Constants;

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class DeliveryHoliday
			{
			private:
				QDate _beginDate;
				QDate  _endDate;

			public:
				void update(const QDomNode& xmlDeliveryHoliday)
				{
					QDomNamedNodeMap map = xmlDeliveryHoliday.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "BeginDate")
							this->_beginDate = QDate::fromString(value, DateTimeConstants::DateFormat);
						else if(name == "EndDate")
							this->_endDate = QDate::fromString(value, DateTimeConstants::DateFormat);
					}
				}
			public:
				QDate getBeginDate() const
				{
					return _beginDate;
				}

				QDate getEndDate() const
				{
					return _endDate;
				}
			};
		}
	}
}
#endif
