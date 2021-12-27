#ifndef BUSINESS_MODEL_SETTING_TRADEDAY_H
#define BUSINESS_MODEL_SETTING_TRADEDAY_H
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
			class TradeDay
			{
			private:
				QDateTime  _currentDay;
				QDateTime  _beginTime;
				QDateTime  _endTime;
				QDateTime  _lastDay;			

			public:
				void update(const QDomNode& xmlTradeDay)
				{
					QDomNamedNodeMap map = xmlTradeDay.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "CurrentDay")
							this->_currentDay = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
						else if(name == "BeginTime")
							this->_beginTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
						else if(name == "EndTime")
							this->_endTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
						else if(name == "LastDay")
							this->_lastDay = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
					}
				}

			public:

				QDateTime getCurrentDay() const
				{
					return _currentDay;
				}

				QDateTime getBeginTime() const
				{
					return _beginTime;
				}

				QDateTime getEndTime() const
				{
					return _endTime;
				}

				QDateTime getLastDay() const
				{
					return _lastDay;
				}

			};
		}
	}
}
#endif
