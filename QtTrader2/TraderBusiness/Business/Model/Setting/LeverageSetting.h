#ifndef BUSINESS_MODEL_SETTING_LEVERAGESETTING_H
#define BUSINESS_MODEL_SETTING_LEVERAGESETTING_H
#include <QDomNode>
#include "Common/Util/Convert_Common.h"
#include "SettingDataBase.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class LeverageSetting: public SettingDataBase
			{
			public:
				LeverageSetting()
				{
					_leverage=0;
					_enableModify  = false;
				    _min = 0;
				    _max = 0;
					_step =  0;
				}

			private:
				int  _leverage;
				bool _enableModify;
				int  _min;
				int  _max;
				int  _step;
			public:
				void update(const QDomNode& xmlLeverageSetting)
				{
					QDomNamedNodeMap map = xmlLeverageSetting.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "Leverage")
							this->_leverage = value.toInt();
						else if(name == "EnableModify")
							this->_enableModify = Common::toBool(value);
						else if(name == "Min")
							this->_min = value.toInt();
						else if(name == "Max")
							this->_max = value.toInt();
						else if(name == "Step")
							this->_step = value.toInt();
					}
				}
                QSharedPointer<LeverageSetting> copy()
                {
                    QSharedPointer<LeverageSetting> newObject(new LeverageSetting);
                    newObject->_leverage = _leverage;
                    newObject->_enableModify = _enableModify;
                    newObject->_min = _min;
                    newObject->_max = _max;
                    newObject->_step = _step;
                    return newObject;
                }


			public:			
				int getLeverage() const
				{
					return _leverage;
				}

				bool getEnableModify() const
				{
					return _enableModify;
				}

				int getMin() const
				{
					return _min;
				}

				int getMax() const
				{
					return _max;
				}

				int getStep() const
				{
					return _step;
				}
				

				void setEnableModify(const bool& value)
				{
					_enableModify = value;
				}

				void setLeverage(const int& value)
				{
					_leverage = value;
				}

				void setMax(const int& value)
				{
					_max = value;
				}

				void setMin(const int& value)
				{
					_min = value;
				}

				void setStep(const int& value)
				{
					_step = value;
				}




			};
		}
	}
}
#endif

