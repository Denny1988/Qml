#ifndef BUSINESS_MODEL_SERVER_SERVER_ADMINISTRATIVECHARGEDETAIL_H
#define BUSINESS_MODEL_SERVER_SERVER_ADMINISTRATIVECHARGEDETAIL_H
#include "QDecNumber.h"
#include <QUuid>
#include <QXmlStreamReader>

namespace Business
{
	namespace Model
	{
		namespace Server
		{
            class AdministrativeChargeDetail
			{
            private:
                QUuid _administrativeChargeId;
                QDecNumber _quantity;
                QDecNumber _chargeRate;

            public:
                QUuid getAdministrativeChargeId() const
				{
                    return _administrativeChargeId;
				}

                QDecNumber getQuantity() const
				{
                    return _quantity;
				}

                QDecNumber getChargeRate() const
				{
                    return _chargeRate;
				}



                void setAdministrativeChargeId(const QUuid& value)
				{
                    _administrativeChargeId = value;
				}

                void setQuantity(const QDecNumber& value)
				{
                    _quantity = value;
				}

                void setChargeRate(const QDecNumber& value)
				{
                    _chargeRate = value;
                }

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
                    QXmlStreamAttributes attributes = xmlStreamReader.attributes();
                    this->_administrativeChargeId = QUuid(attributes.value("AdministrativeChargeId").toString());
                    this->_quantity = QDecNumber::fromQString(attributes.value("Quantity").toString());
                    this->_chargeRate = QDecNumber::fromQString(attributes.value("ChargeRate").toString());
				}

				void update(const QDomNode& xmlNode)
				{
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
                        if (name == "AdministrativeChargeId")
                            this->_administrativeChargeId = QUuid(value);
                        else if (name == "Quantity")
                            this->_quantity = QDecNumber::fromQString(value);
                        else if (name == "ChargeRate")
                            this->_chargeRate = QDecNumber::fromQString(value);
					}
				}

			};			
		}
	}
}
#endif

