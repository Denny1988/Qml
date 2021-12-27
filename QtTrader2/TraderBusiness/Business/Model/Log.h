#ifndef BUSINESS_MODEL_LOG_H
#define BUSINESS_MODEL_LOG_H
#include <quuid.h>
#include <QDomNode>
#include <qdatetime.h>
#include "Common/Constants/DateTimeConstants.h"
#include "Common/Util/Convert_Common.h"
using namespace Common::Constants;

namespace Business
{
	namespace Model
	{
		class Log
		{
		protected:
			QUuid  _id;		
			QDateTime _time;
			QString _action;

		public:
			void update(const QDomNode& xmlChat)
			{
				QString n1= xmlChat.nodeName();
				QDomNamedNodeMap map = xmlChat.attributes();
				for (int i = 0; i < map.count(); i++)
				{
					QDomNode qDomNode = map.item(i);
					QString name = qDomNode.nodeName();
					QString value = Common::fix(qDomNode.nodeValue());
					if(name == "Time")
						this->_time = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
					else if(name == "Action")
						this->_action = value;
				}
			}

		public:
			QUuid getId() const
			{
				return _id;
			}

			void setId(QUuid id)
			{
				_id = id;;
			}
			
			QString getAction() const
			{
				return _action;
			}
			
			QDateTime getTime() const
			{
				return _time;
			}
		};
	}
}
#endif
