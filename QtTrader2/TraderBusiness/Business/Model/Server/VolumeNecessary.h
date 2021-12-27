#ifndef BUSINESS_MODEL_SERVER_SERVER_VOLUMENECESSARY_H
#define BUSINESS_MODEL_SERVER_SERVER_VOLUMENECESSARY_H
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include "Common/Enum/CommonEnum.h"
#include <QUuid>
#include <QXmlStreamReader>

using namespace Common::Enum;
namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class VolumeNecessary
			{
			private:
				QUuid _id;
				VolumeNecessaryOption::VolumeNecessaryOption _option;


			public:
				QUuid getId() const
				{
					return _id;
				}

				VolumeNecessaryOption::VolumeNecessaryOption getOption() const
				{
					return _option;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setOption(const VolumeNecessaryOption::VolumeNecessaryOption& value)
				{
					_option = value;
				}

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_id = QUuid(attributes.value("Id").toString());
					this->_option = static_cast<VolumeNecessaryOption::VolumeNecessaryOption>(attributes.value("Option").toInt());
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
						else if (name == "Option")
							this->_option = static_cast<VolumeNecessaryOption::VolumeNecessaryOption>(value.toInt());
					}
				}
			};			
		}
	}
}
#endif

