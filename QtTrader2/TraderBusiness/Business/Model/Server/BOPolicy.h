#ifndef BUSINESS_MODEL_SERVER_BOPOLICY_H
#define BUSINESS_MODEL_SERVER_BOPOLICY_H
#include "QDecNumber.h"
#include <QUuid>
#include <QXmlStreamReader>

namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class BOPolicy
			{
			private:
				QString _code;
				QUuid _id;
				int _maxOrderCount;
				QDecNumber _totalBetLimit;


			public:
				QString getCode() const
				{
					return _code;
				}

				QUuid getId() const
				{
					return _id;
				}

				int getMaxOrderCount() const
				{
					return _maxOrderCount;
				}

				QDecNumber getTotalBetLimit() const
				{
					return _totalBetLimit;
				}

				void setCode(const QString& value)
				{
					_code = value;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setMaxOrderCount(const int& value)
				{
					_maxOrderCount = value;
				}

				void setTotalBetLimit(const QDecNumber& value)
				{
					_totalBetLimit = value;
				}


			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_code = attributes.value("Code").toString();
					this->_id = QUuid(attributes.value("Id").toString());
					this->_maxOrderCount = attributes.value("MaxOrderCount").toInt();
					this->_totalBetLimit = QDecNumber::fromQString(attributes.value("TotalBetLimit").toString());
				}

				void update(const QDomNode& xmlNode)
				{
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if (name == "Code")
							this->_code = value;
						else if (name == "Id")
							this->_id = QUuid(value);
						else if (name == "MaxOrderCount")
							this->_maxOrderCount = value.toInt();
						else if (name == "TotalBetLimit")
							this->_totalBetLimit = QDecNumber::fromQString(value);
					}
				}


			};
		}
	}
}
#endif

