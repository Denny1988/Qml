#ifndef BUSINESS_MODEL_SETTING_INSTRUMENTPARAMETER_H
#define BUSINESS_MODEL_SETTING_INSTRUMENTPARAMETER_H
#include <quuid.h>
#include <qdatetime.h>
#include <QDomNode>
#include "Common/Constants/DateTimeConstants.h"
using namespace Common::Constants;

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class InstrumentParameter
			{
			private:
				QUuid  _id;
				QDateTime  _dayOpenTime;
				QDateTime  _dayCloseTime;
				QDateTime  _nextDayOpenTime;
				QDateTime  _lastTradeDay;		

			public:
				void update(const QDomNode& xmlCurrency)
				{
					QDomNamedNodeMap map = xmlCurrency.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "Id")
							this->_id = QUuid(value);
						else if(name == "DayOpenTime")
							this->_dayOpenTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
						else if(name == "DayCloseTime")
							this->_dayCloseTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
						else if(name == "NextDayOpenTime")
							this->_nextDayOpenTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
						else if(name == "LastTradeDay")
							this->_lastTradeDay = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
					}
				}

			public:

				QUuid getId() const
				{
					return _id;
				}

				QDateTime getDayOpenTime() const
				{
					return _dayOpenTime;
				}

				QDateTime getDayCloseTime() const
				{
					return _dayCloseTime;
				}

				QDateTime getNextDayOpenTime() const
				{
					return _nextDayOpenTime;
				}

				QDateTime getLastTradeDay() const
				{
					return _lastTradeDay;
				}

			};
		}
	}
}
#endif
