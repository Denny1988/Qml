#include "VolumeNecessary.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			VolumeNecessary::VolumeNecessary()
			{
			}

			VolumeNecessary::~VolumeNecessary()
			{
			}

			QSharedPointer<VolumeNecessaryDetail> VolumeNecessary::getOrAddVolumeNecessaryDetail(const QUuid& id)
			{
				if(this->_volumeNecessaryDetails.contains(id))
				{
					return this->_volumeNecessaryDetails.value(id);
				}
				QSharedPointer<VolumeNecessaryDetail> volumeNecessaryDetail(new VolumeNecessaryDetail);
				this->_volumeNecessaryDetails.insert(id, volumeNecessaryDetail);
				return volumeNecessaryDetail;
			}

			QList<QSharedPointer<VolumeNecessaryDetail>> VolumeNecessary::getVolumeNecessaryDetails() const
			{
				return this->_volumeNecessaryDetails.values();;
			}

			void VolumeNecessary::addVolumeNecessaryDetail(const QUuid& id, const QSharedPointer<VolumeNecessaryDetail>& volumeNecessaryDetail)
			{
				if (!this->_volumeNecessaryDetails.contains(id))
				{
					this->_volumeNecessaryDetails.insert(id, volumeNecessaryDetail);
				}
				else
					this->_volumeNecessaryDetails[id] = volumeNecessaryDetail;
			}

			bool VolumeNecessary::containsVolumeNecessaryDetail(const QUuid& id)
			{
				return this->_volumeNecessaryDetails.contains(id);
			}

			void VolumeNecessary::clearDetail()
			{
				this->_volumeNecessaryDetails.clear();
			}

			void VolumeNecessary::update(const QDomNode& xmlVolumeNecessary)
			{
				QDomNamedNodeMap map = xmlVolumeNecessary.attributes();
				for (int i = 0; i < map.count(); i++)
				{
					QDomNode qDomNode = map.item(i);
					QString name = qDomNode.nodeName();
					QString value = qDomNode.nodeValue();
					if(name == "Id")
					{
						QUuid newId = QUuid(value);
						if (this->_id != newId)
							this->_volumeNecessaryDetails.clear();
						this->_id = newId;
					}
					else if(name == "Option")
						this->_option = static_cast<Common::Enum::VolumeNecessaryOption::VolumeNecessaryOption>(value.toInt());
				}
				QDomNodeList childlist = xmlVolumeNecessary.childNodes();  
				for (int i = 0; i < childlist.count(); i++)
				{
					QDomNode itemQDomNode = childlist.item(i);
					QString name = itemQDomNode.nodeName();
					if(name == "VolumeNecessaryDetail")
					{
						QUuid id =  QUuid(itemQDomNode.toElement().attribute("Id"));
						QSharedPointer<Model::Setting::VolumeNecessaryDetail> volumeNecessaryDetail = this->getOrAddVolumeNecessaryDetail(id);
						volumeNecessaryDetail->update(itemQDomNode);
					}
				}
			}

            QSharedPointer<VolumeNecessary> VolumeNecessary::copy()
            {
                QSharedPointer<VolumeNecessary> newObject(new VolumeNecessary);
                newObject->_id = _id;
                newObject->_option = _option;
                QMap<QUuid, QSharedPointer<VolumeNecessaryDetail>> copyVolumeNecessaryDetails;
                QMapIterator<QUuid, QSharedPointer<VolumeNecessaryDetail>> item(this->_volumeNecessaryDetails);
                while (item.hasNext()) {
                    item.next();
                    copyVolumeNecessaryDetails.insert(item.key(), item.value()->copy());
                }
                newObject->_volumeNecessaryDetails = copyVolumeNecessaryDetails;
                return newObject;
            }
        }
	}
}
