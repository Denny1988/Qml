#ifndef BUSINESS_MODEL_SERVER_COUNTERPARTY_H
#define BUSINESS_MODEL_SERVER_COUNTERPARTY_H
#include <QUuid>
#include <QXmlStreamReader>

namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class CounterParty
			{
			private:
				QString _agreementUrl;
				QString _iP;
				int _id;
				QString _name;
				int _port;
				
			public:
				QString getAgreementUrl() const
				{
					return _agreementUrl;
				}

				QString getIP() const
				{
					return _iP;
				}

				int getId() const
				{
					return _id;
				}

				QString getName() const
				{
					return _name;
				}

				int getPort() const
				{
					return _port;
				}
				
				void setAgreementUrl(const QString& value)
				{
					_agreementUrl = value;
				}

				void setIP(const QString& value)
				{
					_iP = value;
				}

				void setId(const int& value)
				{
					_id = value;
				}

				void setName(const QString& value)
				{
					_name = value;
				}

				void setPort(const int& value)
				{
					_port = value;
				}

				
			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_agreementUrl = attributes.value("AgreementUrl").toString();
					this->_iP = attributes.value("IP").toString();
					this->_id = attributes.value("Id").toInt();
					this->_name = attributes.value("Name").toString();
					this->_port = attributes.value("Port").toInt();
				}

				void update(const QDomNode& xmlNode)
				{
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if (name == "AgreementUrl")
							this->_agreementUrl = value;
						else if (name == "IP")
							this->_iP = value;
						else if (name == "Id")
							this->_id = value.toInt();
						else if (name == "Name")
							this->_name = value;
						else if (name == "Port")
							this->_port = value.toInt();
					}
				}
			};
		}
	}
}
#endif

