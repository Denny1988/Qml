#ifndef BUSINESS_MODEL_SETTING_DELIVAERYSPECIFICATIONDETAIL_H
#define BUSINESS_MODEL_SETTING_DELIVAERYSPECIFICATIONDETAIL_H
#include <quuid.h>
#include "QDecNumber.h"
#include <QSharedPointer>

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class DeliverySpecificationDetail
			{
			private:
				QUuid  _deliveryRequestId;
				QString  _size;
                QString  _deliveryCharge;
                QString _productName;
                QString  _premium;

			public:
                QSharedPointer<DeliverySpecificationDetail> copy()
                {
                    QSharedPointer<DeliverySpecificationDetail> newObject(new DeliverySpecificationDetail);
                    newObject->_deliveryRequestId = _deliveryRequestId;
                    newObject->_size = _size;
                    newObject->_deliveryCharge = _deliveryCharge;
                    newObject->_productName = _productName;
                    newObject->_premium = _premium;
                    return newObject;
                }

				QUuid getDeliveryRequestId() const
				{
					return _deliveryRequestId;
				}

				void setDeliveryRequestId(const QUuid& value)
				{
					_deliveryRequestId = value;
				}


				QString getSize() const
				{
					return _size;
				}

				void setSize(QString value)
				{
					_size = value;
				}

				QString getDeliveryCharge() const
				{
					return _deliveryCharge;
				}

				void setDeliveryCharge(QString value)
				{
					_deliveryCharge = value;
				}

                QString getProductName() const
                {
                    return _productName;
                }

                void setProductName(const QString& value)
                {
                    _productName = value;
                }

                QString getPremium() const
                {
                    return _premium;
                }

                void setPremium(QString value)
                {
                    _premium = value;
                }
			};
		}
	}
}
#endif
