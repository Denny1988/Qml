#ifndef BUSINESS_MODEL_SERVER_SERVER_ADMINISTRATIVECHARGE_H
#define BUSINESS_MODEL_SERVER_SERVER_ADMINISTRATIVECHARGE_H
#include <QUuid>
#include <QXmlStreamReader>

namespace Business
{
	namespace Model
	{
		namespace Server
		{
            class AdministrativeCharge
			{
			private:
				QUuid _id;
                QString _code;


			public:
				QUuid getId() const
				{
					return _id;
				}

                QString getCode() const
				{
                    return _code;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

                void setCode(const QString& value)
				{
                    _code = value;
				}

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_id = QUuid(attributes.value("Id").toString());
                    this->_code = attributes.value("Code").toInt();
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
                        else if (name == "Code")
                            this->_code = value;
					}
				}
			};			
		}
	}
}
#endif

