#ifndef BUSINESS_MODEL_SERVER_QUOTEPOLICY_H
#define BUSINESS_MODEL_SERVER_QUOTEPOLICY_H
#include <QUuid>
#include <QXmlStreamReader>

namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class QuotePolicy
			{
			private:
				QString _code;
				QString _description;
				QUuid _id;
				bool _isDefault;


			public:
				QString getCode() const
				{
					return _code;
				}

				QString getDescription() const
				{
					return _description;
				}

				QUuid getId() const
				{
					return _id;
				}

				bool getIsDefault() const
				{
					return _isDefault;
				}
				
				void setCode(const QString& value)
				{
					_code = value;
				}

				void setDescription(const QString& value)
				{
					_description = value;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setIsDefault(const bool& value)
				{
					_isDefault = value;
				}

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_code = attributes.value("Code").toString();
					this->_description = attributes.value("Description").toString();
					this->_id = QUuid(attributes.value("Id").toString());
					this->_isDefault = Common::toBool(attributes.value("IsDefault").toString());
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
						else if (name == "Description")
							this->_description = value;
						else if (name == "Id")
							this->_id = QUuid(value);
						else if (name == "IsDefault")
							this->_isDefault = Common::toBool(value);
					}
				}



			};
		}
	}
}
#endif

