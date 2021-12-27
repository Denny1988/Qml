#ifndef BUSINESS_MODEL_SETTING_MOVEPIPSINFOSETTING_H
#define BUSINESS_MODEL_SETTING_MOVEPIPSINFOSETTING_H
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include "SettingDataBase.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class MovePipsInfoSetting: public SettingDataBase
			{
			private:
				QDecNumber  _step;
				QDecNumber  _min;
				QDecNumber  _max;
				QDecNumber  _default;
				int  _decimals;
				bool _isMovePipsNeeded;

			public:
				void update(const QDomNode& xmlMovePipsInfoSetting)
				{
					QDomNamedNodeMap map = xmlMovePipsInfoSetting.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "Step")
							this->_step= QDecNumber::fromQString(value);
						else if(name == "Min")
							this->_min= QDecNumber::fromQString(value);
						else if(name == "Max")
							this->_max= QDecNumber::fromQString(value);
						else if(name == "Default")
							this->_default= QDecNumber::fromQString(value);
						else if(name == "Decimals")
							this->_decimals= value.toInt();
						else if(name == "IsMovePipsNeeded")
							this->_isMovePipsNeeded = Common::toBool(value);
					}
				}

				void unavailable()
				{
					this->_max = 0;
					this->_step = 0;
					this->_decimals = 0;
					this->_default = 0; 
					this->_min = 0;
					this->_isMovePipsNeeded = false;
				}

                QSharedPointer<MovePipsInfoSetting> copy()
                {
                    QSharedPointer<MovePipsInfoSetting> newObject(new MovePipsInfoSetting);
                    newObject->_step = _step;
                    newObject->_min = _min;
                    newObject->_max = _max;
                    newObject->_default = _default;
                    newObject->_decimals = _decimals;
                    newObject->_isMovePipsNeeded = _isMovePipsNeeded;
                    return newObject;
                }

			public:			
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

				bool getIsMovePipsNeeded() const
				{
					return _isMovePipsNeeded;
				}


				void setDecimals(const int& value)
				{
					_decimals = value;
				}

				void setDefault(const QDecNumber& value)
				{
					_default = value;
				}

				void setIsMovePipsNeeded(const bool& value)
				{
					_isMovePipsNeeded = value;
				}

				void setMax(const QDecNumber& value)
				{
					_max = value;
				}

				void setMin(const QDecNumber& value)
				{
					_min = value;
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

