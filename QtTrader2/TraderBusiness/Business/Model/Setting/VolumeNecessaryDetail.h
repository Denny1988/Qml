#ifndef BUSINESS_MODEL_SETTING_VOLUMENECESSARYDETAIL_H
#define BUSINESS_MODEL_SETTING_VOLUMENECESSARYDETAIL_H
//#include <quuid.h>
#include "QDecNumber.h"
#include <QDomNode>
#include <QSharedPointer>

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class VolumeNecessaryDetail
			{
			private:
				//QUuid  _volumeNecessaryId;
				QUuid  _id;
				QDecNumber  _from;
				QDecNumber  _marginD;
				QDecNumber  _marginO;

			public:
				void update(const QDomNode& xmlVolumeNecessaryDetail)
				{
					QDomNamedNodeMap map = xmlVolumeNecessaryDetail.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "Id")
							this->_id = QUuid(value);
						else if(name == "From")
							this->_from= QDecNumber::fromQString(value);
						else if(name == "MarginD")
							this->_marginD= QDecNumber::fromQString(value);
						else if(name == "MarginO")
							this->_marginO= QDecNumber::fromQString(value);
					}
				}

                QSharedPointer<VolumeNecessaryDetail> copy()
                {
                    QSharedPointer<VolumeNecessaryDetail> newObject(new VolumeNecessaryDetail);
                    newObject->_id = _id;
                    newObject->_from = _from;
                    newObject->_marginD = _marginD;
                    newObject->_marginO = _marginO;
                    return newObject;
                }

			public:

				/*QUuid getVolumeNecessaryId() const
				{
				return _volumeNecessaryId;
				}

				void setVolumeNecessaryId(const QUuid& value)
				{
				_volumeNecessaryId = value;
				}*/
				

				QUuid getId() const
				{
					return _id;
				}

				QDecNumber getFrom() const
				{
					return _from;
				}

				QDecNumber getMarginD() const
				{
					return _marginD;
				}

				QDecNumber getMarginO() const
				{
					return _marginO;
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





			};
		}
	}
}
#endif
