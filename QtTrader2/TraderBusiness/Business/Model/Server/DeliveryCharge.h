#ifndef BUSINESS_MODEL_SERVER_DELIVERYCHARGE_H
#define BUSINESS_MODEL_SERVER_DELIVERYCHARGE_H
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include "Common/Enum/PhysicalEnum.h"
#include <QUuid>
#include <QXmlStreamReader>

using namespace Common::Enum;
namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class DeliveryCharge
			{
			private:
				PhysicalChargeBasis::PhysicalChargeBasis _chargeBasis;
				QDecNumber _chargeRate;
				QUuid _id;
				QDecNumber _minCharge;


			public:
				PhysicalChargeBasis::PhysicalChargeBasis getChargeBasis() const
				{
					return _chargeBasis;
				}

				QDecNumber getChargeRate() const
				{
					return _chargeRate;
				}

				QUuid getId() const
				{
					return _id;
				}

				QDecNumber getMinCharge() const
				{
					return _minCharge;
				}
				
				void setChargeBasis(const PhysicalChargeBasis::PhysicalChargeBasis& value)
				{
					_chargeBasis = value;
				}

				void setChargeRate(const QDecNumber& value)
				{
					_chargeRate = value;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setMinCharge(const QDecNumber& value)
				{
					_minCharge = value;
				}

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_chargeBasis = static_cast<PhysicalChargeBasis::PhysicalChargeBasis>(attributes.value("ChargeBasis").toInt());
					this->_chargeRate = QDecNumber::fromQString(attributes.value("ChargeRate").toString());
					this->_id = QUuid(attributes.value("Id").toString());
					this->_minCharge = QDecNumber::fromQString(attributes.value("MinCharge").toString());
				}

			public:
				void update(const QDomNode& xmlNode)
				{
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if (name == "ChargeBasis")
							this->_chargeBasis = static_cast<PhysicalChargeBasis::PhysicalChargeBasis>(value.toInt());
						else if (name == "ChargeRate")
							this->_chargeRate = QDecNumber::fromQString(value);
						else if (name == "Id")
							this->_id = QUuid(value);
						else if (name == "MinCharge")
							this->_minCharge = QDecNumber::fromQString(value);
					}
				}



			};
		}
	}
}
#endif

