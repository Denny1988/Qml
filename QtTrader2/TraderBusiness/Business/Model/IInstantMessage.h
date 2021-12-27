#ifndef BUSINESS_MODEL_IINSTANTMESSAGE_H
#define BUSINESS_MODEL_IINSTANTMESSAGE_H
#include <quuid.h>
#include <QDomNode>
#include <qdatetime.h>
#include "Common/Constants/DateTimeConstants.h"
#include "Common/Util/Convert_Common.h"
#include "DateTimeHelper.h"
using namespace Common::Constants;

namespace Business
{
	namespace Model
	{
		class IInstantMessage
		{
		protected:
			QUuid  _id;		
			QString _title;
			QString _content;
			QString _publishTimeStr;
			bool _canDelete;
			bool _isRead;
			QDateTime _publishTime;

		public:
			void update(const QDomNode& xmlChat)
			{
				QDomNamedNodeMap map = xmlChat.attributes();
				for (int i = 0; i < map.count(); i++)
				{
					QDomNode qDomNode = map.item(i);
					QString name = qDomNode.nodeName();
					QString value = Common::fix(qDomNode.nodeValue());
					if(name == "Id")
						this->_id = QUuid(value);
					else if(name == "Title")
						this->_title = value;
					else if(name == "Content")
						this->_content = value;
					else if (name == "PublishTime")
					{
						this->_publishTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
						this->_publishTimeStr = DateTimeHelper::toDateTimeString(this->_publishTime);
					}
					else if (name == "IsRead")
						this->_isRead = Common::toBool(value);
				}
			}

		public:
			QUuid getId() const
			{
				return _id;
			}

			QString getTitle() const
			{
				return _title;
			}

			bool isLoadContent()
			{
				return !_content.isNull() && !_content.isEmpty() && _content!="";
			}

			QString getContent() const
			{
				return _content;
			}

			QString getPublishTime() const
			{
				return _publishTimeStr;
			}

			QDateTime getPublishQDateTime() const
			{
				return _publishTime;
			}

			bool getCanDelete() const
			{
				return _canDelete;
			}

			bool getIsRead() const
			{
				return _isRead;
			}

			void setContent(QString value)
			{
				_content = value;
			}

			void setIsRead(bool value)
			{
				_isRead = value;
			}
		};
	}
}
#endif
