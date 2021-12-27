#ifndef BUSINESS_MODEL_SERVER_QUOTEPOLICYDETAIL_H
#define BUSINESS_MODEL_SERVER_QUOTEPOLICYDETAIL_H
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include "Common/Enum/InstrumentEnum.h"
#include <QUuid>
#include <QXmlStreamReader>

using namespace Common::Enum;
namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class QuotePolicyDetail
			{
			private:
				int _autoAdjustPoints;
				QUuid _instrumentId;
				Common::Enum::PriceType::PriceType _priceType;
				QUuid _quotePolicyId;
				int _spreadPoints;


			public:
				int getAutoAdjustPoints() const
				{
					return _autoAdjustPoints;
				}

				QUuid getInstrumentId() const
				{
					return _instrumentId;
				}

				Common::Enum::PriceType::PriceType getPriceType() const
				{
					return _priceType;
				}

				QUuid getQuotePolicyId() const
				{
					return _quotePolicyId;
				}

				int getSpreadPoints() const
				{
					return _spreadPoints;
				}
				
				void setAutoAdjustPoints(const int& value)
				{
					_autoAdjustPoints = value;
				}

				void setInstrumentId(const QUuid& value)
				{
					_instrumentId = value;
				}

				void setPriceType(const Common::Enum::PriceType::PriceType& value)
				{
					_priceType = value;
				}

				void setQuotePolicyId(const QUuid& value)
				{
					_quotePolicyId = value;
				}

				void setSpreadPoints(const int& value)
				{
					_spreadPoints = value;
				}

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();
					this->_autoAdjustPoints = attributes.value("AutoAdjustPoints").toInt();
					this->_instrumentId = QUuid(attributes.value("InstrumentId").toString());
					this->_priceType = static_cast<PriceType::PriceType>(attributes.value("PriceType").toInt());
					this->_quotePolicyId = QUuid(attributes.value("QuotePolicyId").toString());
					this->_spreadPoints = attributes.value("SpreadPoints").toInt();
				}

				void update(const QDomNode& xmlNode)
				{
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if (name == "AutoAdjustPoints")
							this->_autoAdjustPoints = value.toInt();
						else if (name == "InstrumentId")
							this->_instrumentId = QUuid(value);
						else if (name == "PriceType")
							this->_priceType = static_cast<PriceType::PriceType>(value.toInt());
						else if (name == "QuotePolicyId")
							this->_quotePolicyId = QUuid(value);
						else if (name == "SpreadPoints")
							this->_spreadPoints = value.toInt();
					}
				}
			};
		}
	}
}
#endif

