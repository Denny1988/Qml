#ifndef BUSINESS_MODEL_SETTING_DELIVAERYCHARGE_H
#define BUSINESS_MODEL_SETTING_DELIVAERYCHARGE_H
//#include <quuid.h>
#include "Common/Enum/PhysicalEnum.h"
#include "QDecNumber.h"
#include <QDomNode>
#include "SettingDataBase.h"
#include <QSharedPointer>

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class DeliveryCharge: public SettingDataBase
			{
			private:
				/*QUuid  _id;
				QString  _code;*/
				Common::Enum::PhysicalChargeBasis::PhysicalChargeBasis _chargeBasis;
				QDecNumber  _chargeRate;
				QDecNumber  _minCharge;
			public:
				void update(const QDomNode& xmlDeliveryCharge)
				{
					QDomNamedNodeMap map = xmlDeliveryCharge.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "ChargeRate")
							this->_chargeRate = QDecNumber::fromQString(value);
						else if(name == "ChargeBasis")
							this->_chargeBasis = static_cast<Common::Enum::PhysicalChargeBasis::PhysicalChargeBasis>(value.toInt());
						else if(name == "MinCharge")
							this->_minCharge = QDecNumber::fromQString(value);
					}
				}

                QSharedPointer<DeliveryCharge> copy()
                {
                    QSharedPointer<DeliveryCharge> newObject(new DeliveryCharge);
                    newObject->_chargeBasis = _chargeBasis;
                    newObject->_chargeRate = _chargeRate;
                    newObject->_minCharge = _minCharge;
                    return newObject;
                }
            public:

				/*QUuid getId() const
				{
				return _id;
				}

				void setId(const QUuid& value)
				{
				_id = value;
				}

				QString getCode() const
				{
				return _code;
				}

				void setCode(const QString& value)
				{
				_code = value;
				}*/

				Common::Enum::PhysicalChargeBasis::PhysicalChargeBasis  getChargeBasis() const
				{
					return _chargeBasis;
				}

				QDecNumber getChargeRate() const
				{
					return _chargeRate;
				}

				QDecNumber getMinCharge() const
				{
					return _minCharge;
				}


				void setChargeBasis(const Common::Enum::PhysicalChargeBasis::PhysicalChargeBasis& value)
				{
					_chargeBasis = value;
				}

				void setChargeRate(const QDecNumber& value)
				{
					_chargeRate = value;
				}

				void setMinCharge(const QDecNumber& value)
				{
					_minCharge = value;
				}





			};
		}
	}
}
#endif
