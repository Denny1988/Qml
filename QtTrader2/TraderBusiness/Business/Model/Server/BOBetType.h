#ifndef BUSINESS_MODEL_SERVER_BOBETTYPE_H
#define BUSINESS_MODEL_SERVER_BOBETTYPE_H
#include "Common/Enum/BOEnum.h"
#include <QUuid>
#include <QXmlStreamReader>

using namespace Common::Enum;
namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class BOBetType
			{
			private:
				QUuid _id;
				int _lastOrderTimeSpan;
				BOBetOption::BOBetOption _option;


			public:
				QUuid getId() const
				{
					return _id;
				}

				int getLastOrderTimeSpan() const
				{
					return _lastOrderTimeSpan;
				}

				BOBetOption::BOBetOption getOption() const
				{
					return _option;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setLastOrderTimeSpan(const int& value)
				{
					_lastOrderTimeSpan = value;
				}

				void setOption(const BOBetOption::BOBetOption& value)
				{
					_option = value;
				}

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_id = QUuid(attributes.value("Id").toString());
					this->_lastOrderTimeSpan = attributes.value("LastOrderTimeSpan").toInt();
					this->_option = static_cast<BOBetOption::BOBetOption>(attributes.value("Option").toInt());
				}

				void update(const QDomNode& xmlNode)
				{
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if (name == "Id")
							this->_id = QUuid(value);
						else if (name == "LastOrderTimeSpan")
							this->_lastOrderTimeSpan = value.toInt();
						else if (name == "Option")
							this->_option = static_cast<BOBetOption::BOBetOption>(value.toInt());
					}
				}


			};
		}
	}
}
#endif

