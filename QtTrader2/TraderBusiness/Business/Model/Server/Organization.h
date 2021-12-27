#ifndef BUSINESS_MODEL_SERVER_ORGANIZATION_H
#define BUSINESS_MODEL_SERVER_ORGANIZATION_H
#include <QUuid>
#include <QXmlStreamReader>
#include <QDomNode>

namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class Organization
			{
			private:
				QString _code;
				QUuid _id;
				QString _name;
				
			public:
				QString getCode() const
				{
					return _code;
				}

				QUuid getId() const
				{
					return _id;
				}

				QString getName() const
				{
					return _name;
				}

				void setCode(const QString& value)
				{
					_code = value;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setName(const QString& value)
				{
					_name = value;
				}
					

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_code = attributes.value("Code").toString();
					this->_id = QUuid(attributes.value("Id").toString());
					this->_name = attributes.value("Name").toString();
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
						else if (name == "Name")
							this->_name = value;
					}
				}


			};
		}
	}
}
#endif

