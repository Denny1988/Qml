#ifndef BUSINESS_MODEL_SERVER_SERVER_VOLUMENECESSARYDETAIL_H
#define BUSINESS_MODEL_SERVER_SERVER_VOLUMENECESSARYDETAIL_H
#include "QDecNumber.h"
#include <QUuid>
#include <QXmlStreamReader>

namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class VolumeNecessaryDetail
			{
			private:
				QDecNumber _from;
				QUuid _id;
				QDecNumber _marginD;
				QDecNumber _marginO;
				QUuid _volumeNecessaryId;


			public:
				QDecNumber getFrom() const
				{
					return _from;
				}

				QUuid getId() const
				{
					return _id;
				}

				QDecNumber getMarginD() const
				{
					return _marginD;
				}

				QDecNumber getMarginO() const
				{
					return _marginO;
				}

				QUuid getVolumeNecessaryId() const
				{
					return _volumeNecessaryId;
				}

				void setFrom(const QDecNumber& value)
				{
					_from = value;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setMarginD(const QDecNumber& value)
				{
					_marginD = value;
				}

				void setMarginO(const QDecNumber& value)
				{
					_marginO = value;
				}

				void setVolumeNecessaryId(const QUuid& value)
				{
					_volumeNecessaryId = value;
				}

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_from = QDecNumber::fromQString(attributes.value("From").toString());
					this->_id = QUuid(attributes.value("Id").toString());
					this->_marginD = QDecNumber::fromQString(attributes.value("MarginD").toString());
					this->_marginO = QDecNumber::fromQString(attributes.value("MarginO").toString());
					this->_volumeNecessaryId = QUuid(attributes.value("VolumeNecessaryId").toString());
				}

				void update(const QDomNode& xmlNode)
				{
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if (name == "From")
							this->_from = QDecNumber::fromQString(value);
						else if (name == "Id")
							this->_id = QUuid(value);
						else if (name == "MarginD")
							this->_marginD = QDecNumber::fromQString(value);
						else if (name == "MarginO")
							this->_marginO = QDecNumber::fromQString(value);
						else if (name == "VolumeNecessaryId")
							this->_volumeNecessaryId = QUuid(value);
					}
				}

			};			
		}
	}
}
#endif

