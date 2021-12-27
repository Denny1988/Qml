#ifndef BUSINESS_MODEL_SETTING_LOTSETTING_H
#define BUSINESS_MODEL_SETTING_LOTSETTING_H
#include "Common/Enum/PlaceEnum.h"
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include "traderbusiness_global.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class TRADERBUSINESS_EXPORT LotSetting
			{
			private:
				bool	_isOpen;
				Common::Enum::OrderType::OrderType	_orderType;
				QDecNumber  _step;
				QDecNumber  _min;
				QDecNumber  _max;
				QDecNumber  _default;
				int  _decimals;

			public:
				void update(const QDomNode& xmlLotSetting)
				{
					QDomNamedNodeMap map = xmlLotSetting.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "IsOpen")
							this->_isOpen = Common::toBool(value);
						else if(name == "OrderType")
							this->_orderType = static_cast<Common::Enum::OrderType::OrderType>(value.toInt());
						else if(name == "Min")
							this->_min= QDecNumber::fromQString(value);
						else if(name == "Max")
							this->_max= QDecNumber::fromQString(value);
						else if(name == "Default")
							this->_default= QDecNumber::fromQString(value);
						else if(name == "Decimals")
							this->_decimals= value.toInt();
						else if(name == "Step")
							this->_step= QDecNumber::fromQString(value);
					}
				}

                QSharedPointer<LotSetting> copy()
                {
                    QSharedPointer<LotSetting> newObject(new LotSetting);
                    newObject->_isOpen = _isOpen;
                    newObject->_orderType = _orderType;
                    newObject->_step = _step;
                    newObject->_min = _min;
                    newObject->_max = _max;
                    newObject->_default = _default;
                    newObject->_decimals = _decimals;
                    return newObject;
                }

			public:	
				bool getIsOpen() const
				{
					return _isOpen;
				}

				Common::Enum::OrderType::OrderType getOrderType() const
				{
					return _orderType;
				}

				QDecNumber getStep() const
				{
					return _step;
				}

				QDecNumber getMin() const
				{
					return _min;
				}

				QDecNumber getMax() const
				{
					return _max;
				}

				QDecNumber getDefault() const
				{
					return _default;
				}

				int getDecimals() const
				{
					return _decimals;
				}


				void setDecimals(const int& value)
				{
					_decimals = value;
				}

				void setDefault(const QDecNumber& value)
				{
					_default = value;
				}

				void setIsOpen(const bool& value)
				{
					_isOpen = value;
				}

				void setMax(const QDecNumber& value)
				{
					_max = value;
				}

				void setMin(const QDecNumber& value)
				{
					_min = value;
				}

				void setOrderType(const Common::Enum::OrderType::OrderType& value)
				{
					_orderType = value;
				}

				void setStep(const QDecNumber& value)
				{
					_step = value;
				}



			};
		}
	}
}
#endif

