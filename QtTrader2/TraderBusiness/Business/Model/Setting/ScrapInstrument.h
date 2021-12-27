#ifndef BUSINESS_MODEL_SETTING_SCRAPINSTRUMENT_H
#define BUSINESS_MODEL_SETTING_SCRAPINSTRUMENT_H
#include <quuid.h>
namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class ScrapInstrument
			{
			private:
				QUuid  _id;			
				int _quantityDecimalDigits;
				QString _unit;
				QString _description;

			public:
				void update(const QDomNode& xmlScrapInstrument)
				{
					QDomNamedNodeMap map = xmlScrapInstrument.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = Common::fix(qDomNode.nodeValue());
						if(name == "Id")
							this->_id = QUuid(value);
						else if(name == "Unit")
							this->_unit = value;
						else if(name == "Description")
							this->_description = value;
						else if(name == "QuantityDecimalDigits")
							this->_quantityDecimalDigits = value.toInt();
					}
				}

			public:
				QUuid getId() const
				{
					return _id;
				}
				
				QString getUnit() const
				{
					return _unit;
				}

				int getQuantityDecimalDigits() const
				{
					return _quantityDecimalDigits;
				}

				QString getDescription() const
				{
					return _description;
				}
			};
		}
	}
}
#endif
