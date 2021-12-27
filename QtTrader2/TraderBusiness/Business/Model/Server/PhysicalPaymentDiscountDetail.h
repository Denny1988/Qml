#ifndef BUSINESS_MODEL_SERVER_PHYSICALPAYMENTDISCOUNTDETAIL_H
#define BUSINESS_MODEL_SERVER_PHYSICALPAYMENTDISCOUNTDETAIL_H
#include "QDecNumber.h"
#include <QUuid>
#include <QXmlStreamReader>

namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class PhysicalPaymentDiscountDetail
			{
			private:
				QDecNumber _discountValue;
				QDecNumber _from;
				QUuid _id;
				QUuid _physicalPaymentDiscountId;


			public:
				QDecNumber getDiscountValue() const
				{
					return _discountValue;
				}

				QDecNumber getFrom() const
				{
					return _from;
				}

				QUuid getId() const
				{
					return _id;
				}

				QUuid getPhysicalPaymentDiscountId() const
				{
					return _physicalPaymentDiscountId;
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

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_discountValue = QDecNumber::fromQString(attributes.value("DiscountValue").toString());
					this->_from = QDecNumber::fromQString(attributes.value("From").toString());
					this->_id = QUuid(attributes.value("Id").toString());
					this->_physicalPaymentDiscountId = QUuid(attributes.value("PhysicalPaymentDiscountId").toString());
				}

				void update(const QDomNode& xmlNode)
				{
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if (name == "DiscountValue")
							this->_discountValue = QDecNumber::fromQString(value);
						else if (name == "From")
							this->_from = QDecNumber::fromQString(value);
						else if (name == "Id")
							this->_id = QUuid(value);
						else if (name == "PhysicalPaymentDiscountId")
							this->_physicalPaymentDiscountId = QUuid(value);
					}
				}


			};

		}
	}
}
#endif

