#ifndef BUSINESS_MODEL_SETTING_CURRENCY_H
#define BUSINESS_MODEL_SETTING_CURRENCY_H
#include <quuid.h>
#include <qstring.h>
#include "QDecNumber.h"
#include <QDomNode>

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class Currency
			{
			private:
				QUuid  _id;
				QString  _code;
				QString  _name;
				QString  _alias;
				int  _decimals;
				QDecNumber  _minDeposit;			

			public:
				void update(const QDomNode& xmlCurrency)
				{
					QDomNamedNodeMap map = xmlCurrency.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "Id")
							this->_id = QUuid(value);
						else if(name == "Name")
							this->_name = value;
						else if(name == "Code")
							this->_code = value;
						else if(name == "Alias")
							this->_alias = value;
						else if(name == "Decimals")
							this->_decimals = value.toInt();
						else if(name == "MinDeposit")
							this->_minDeposit = QDecNumber::fromQString(value);
					}
				}

			public:

				QUuid getId() const
				{
					return _id;
				}

				QString getCode() const
				{
					return _code;
				}

				int getDecimals() const
				{
					return _decimals;
				}

				QDecNumber getMinDeposit() const
				{
					return _minDeposit;
				}

				QString getName() const
				{
					return _name;
				}

				QString getAlias() const
				{
					return _alias;
				}

			};
		}
	}
}
#endif
