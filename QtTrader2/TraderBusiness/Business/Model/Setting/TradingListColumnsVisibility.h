#ifndef BUSINESS_MODEL_SETTING_TRADINGLISTCOLUMNSVISIBILITY_H
#define BUSINESS_MODEL_SETTING_TRADINGLISTCOLUMNSVISIBILITY_H
#include "Common/Enum/TradingListNameEnum.h"
#include "Common/Util/Convert_Common.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class TradingListColumnsVisibility
			{
			private:
				QString _path;
				Common::Enum::TradingListNameEnum::TradingListNameEnum	_listNameType;
				QString _columnName;
				bool	_iDisplay;

			public:
				void update(const QDomNode& xml)
				{
					QDomNamedNodeMap map = xml.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "Path")
							this->_path = value;
						else if(name == "ListName")
							this->_listNameType = static_cast<Common::Enum::TradingListNameEnum::TradingListNameEnum>(value.toInt());
						else if(name == "ColumnName")
							this->_columnName = value.toUpper();
						else if(name == "IsDisplay")
							this->_iDisplay = Common::toBool(value);
					}
				}

			public:	
				QString getPath() const
				{
					return _path;
				}

				Common::Enum::TradingListNameEnum::TradingListNameEnum getListName() const
				{
					return _listNameType;
				}

				QString getColumnName() const
				{
					return _columnName;
				}

				bool getIsDisplay() const
				{
					return _iDisplay;
				}


			};
		}
	}
}
#endif

