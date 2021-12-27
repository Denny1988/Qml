#ifndef BUSINESS_MODEL_SERVER_INSTRUMENTUNIT_H
#define BUSINESS_MODEL_SERVER_INSTRUMENTUNIT_H
#include <QUuid>
#include <QXmlStreamReader>

namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class InstrumentUnit
			{
			private:
				QUuid _id;
				int _weight;


			public:
				QUuid getId() const
				{
					return _id;
				}

				int getWeight() const
				{
					return _weight;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setWeight(const int& value)
				{
					_weight = value;
				}

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_id = QUuid(attributes.value("Id").toString());
					this->_weight = attributes.value("Weight").toInt();
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
						else if (name == "Weight")
							this->_weight = value.toInt();
					}
				}

			};

		}
	}
}
#endif

