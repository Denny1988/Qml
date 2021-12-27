#ifndef BUSINESS_MODEL_TRADING_DELIVERYREQUESTORDERRELATION_H
#define BUSINESS_MODEL_TRADING_DELIVERYREQUESTORDERRELATION_H
#include <quuid.h>
#include "QDecNumber.h"
#include <QDomNode>
#include <QXmlStreamReader>

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			class DeliveryRequestOrderRelation
			{
			private:
				QUuid  _openOrderId;
				QUuid  _deliveryRequestId;
				QDecNumber  _deliveryQuantity;
				QDecNumber  _deliveryLot;
			public:
				void update(const QXmlStreamReader& xmlStreamReader)
				{	
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();  
					this->_openOrderId = QUuid(attributes.value("OpenOrderId").toString());
					this->_deliveryRequestId = QUuid(attributes.value("DeliveryRequestId").toString());
					this->_deliveryQuantity = QDecNumber::fromQString(attributes.value("DeliveryQuantity").toString());
					this->_deliveryLot = QDecNumber::fromQString(attributes.value("DeliveryLot").toString());
				}

				void update(const QDomNode& xmlDeliveryRequestOrderRelation)
				{
					QDomNamedNodeMap map = xmlDeliveryRequestOrderRelation.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "OpenOrderId")
							this->_openOrderId = QUuid(value);
						else if(name == "DeliveryRequestId")
							this->_deliveryRequestId = QUuid(value);
						else if(name == "DeliveryQuantity")
							this->_deliveryQuantity= QDecNumber::fromQString(value);
						else if(name == "DeliveryLot")
							this->_deliveryLot= QDecNumber::fromQString(value);
					}
				}

			public:
				QUuid getOpenOrderId() const
				{
					return _openOrderId;
				}
				

				QUuid getDeliveryRequestId() const
				{
					return _deliveryRequestId;
				}
				

				QDecNumber getDeliveryQuantity() const
				{
					return _deliveryQuantity;
				}


				QDecNumber getDeliveryLot() const
				{
					return _deliveryLot;
				}

			};
		}
	}
}
#endif
