#ifndef BUSINESS_MODEL_SETTING_INTERESTRATE_H
#define BUSINESS_MODEL_SETTING_INTERESTRATE_H
#include "QDecNumber.h"
#include <QDomNode>
#include <QUuid>
#include "SettingDataBase.h"
#include <QSharedPointer>

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class InterestRate: public SettingDataBase
			{
			public:
                InterestRate()
				{
					this->_buy = 0.0;
					this->_sell = 0.0;
				}
			private:
				QUuid _interestRateId;
				QDecNumber  _buy;
				QDecNumber  _sell;

			public:
				void update(const QDomNode& xmlInterestRate)
				{
					QDomNamedNodeMap map = xmlInterestRate.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if (name == "Buy")
							this->_buy = QDecNumber::fromQString(value);
						else if (name == "Sell")
							this->_sell = QDecNumber::fromQString(value);
						else if (name == "InterestRateId")
							this->_interestRateId = QUuid(value);
					}
				}

                QSharedPointer<InterestRate> copy()
                {
                    QSharedPointer<InterestRate> newObject(new InterestRate);
                    newObject->_interestRateId = _interestRateId;
                    newObject->_buy = _buy;
                    newObject->_sell = _sell;
                    return newObject;
                }

			public:
				QUuid getInterestRateId() const
				{
					return _interestRateId;
				}

				QDecNumber getBuy() const
				{
					return _buy;
				}

				QDecNumber getSell() const
				{
					return _sell;
				}

				void setBuy(const QDecNumber& value)
				{
					_buy = value;
				}

				void setInterestRateId(const QUuid& value)
				{
					_interestRateId = value;
				}

				void setSell(const QDecNumber& value)
				{
					_sell = value;
				}



			};
		}
	}
}
#endif
