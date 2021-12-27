#ifndef BUSINESS_MODEL_SETTING_PHYSICALPAYMENTDISCOUNTSETTINGDETAIL_H
#define BUSINESS_MODEL_SETTING_PHYSICALPAYMENTDISCOUNTSETTINGDETAIL_H
#include <quuid.h>
#include "Common/Enum/InstalmentEnum.h"
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include "traderbusiness_global.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class TRADERBUSINESS_EXPORT PhysicalPaymentDiscountSettingDetail
			{
			private:
				QUuid  _id;
				QUuid  _physicalPaymentDiscountId;
				QDecNumber _from;
				QDecNumber _discountValue;

			public:
				void update(const QDomNode& xmlphysicalPaymentDiscountPolicyDetail)
				{
					QDomNamedNodeMap map = xmlphysicalPaymentDiscountPolicyDetail.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "Id")
							this->_id = QUuid(value);
						else if(name == "PhysicalPaymentDiscountId")
							this->_physicalPaymentDiscountId = QUuid(value);
						else if(name == "From")
							this->_from = QDecNumber::fromQString(value);
						else if(name == "DiscountValue")
							this->_discountValue = QDecNumber::fromQString(value);
					}
				}

                QSharedPointer<PhysicalPaymentDiscountSettingDetail> copy()
                {
                    QSharedPointer<PhysicalPaymentDiscountSettingDetail> newObject(new PhysicalPaymentDiscountSettingDetail);
                    newObject->_id = _id;
                    newObject->_physicalPaymentDiscountId = _physicalPaymentDiscountId;
                    newObject->_from = _from;
                    newObject->_discountValue = _discountValue;
                    return newObject;
                }

			public:
				QUuid getId() const
				{
					return _id;
				}

				QUuid getPhysicalPaymentDiscountId() const
				{
					return _physicalPaymentDiscountId;
				}

				QDecNumber getFrom() const
				{
					return _from;
				}

				QDecNumber getDiscountValue() const
				{
					return _discountValue;
				}
					


				void setDiscountValue(const QDecNumber& value)
				{
					_discountValue = value;
				}

				void setFrom(const QDecNumber& value)
				{
					_from = value;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setPhysicalPaymentDiscountId(const QUuid& value)
				{
					_physicalPaymentDiscountId = value;
				}


			};
		}
	}
}
#endif
