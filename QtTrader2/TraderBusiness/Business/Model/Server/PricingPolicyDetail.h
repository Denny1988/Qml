#ifndef BUSINESS_MODEL_SERVER_PRICINGPOLICYDETAIL_H
#define BUSINESS_MODEL_SERVER_PRICINGPOLICYDETAIL_H
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include <QUuid>
#include <QXmlStreamReader>

using namespace Common::Enum;
namespace Business
{
	namespace Model
	{
		namespace Server
		{
            class PricingPolicyDetail
			{
			private:
                QUuid _id;
                QUuid _pricingPolicyId;
                QDecNumber _price;
                int _numerator;
                QDecNumber _spreadFactor;
                QDecNumber _autoPointFactor;
                QDecNumber _orderVariationFactor;

			public:                
                QUuid getId() const
                {
                    return _id;
                }

                QUuid getPricingPolicyId() const
                {
                    return _pricingPolicyId;
                }

                QDecNumber getPrice() const
				{
                    return _price;
				}

                int getNumerator() const
				{
                    return _numerator;
                }

                QDecNumber getSpreadFactor() const
				{
                    return _spreadFactor;
				}

                QDecNumber getAutoPointFactor() const
				{
                    return _autoPointFactor;
				}

                QDecNumber getOrderVariationFactor() const
				{
                    return _orderVariationFactor;
				}


                void setId(const QUuid& value)
                {
                    _id = value;
                }

                void setPricingPolicyId(const QUuid& value)
                {
                    _pricingPolicyId = value;
                }
				
                void setPrice(const QDecNumber& value)
				{
                    _price = value;
				}

                void setNumerator(const int& value)
				{
                    _numerator = value;
				}

                void setSpreadFactor(const QDecNumber& value)
				{
                    _spreadFactor = value;
				}

                void setAutoPointFactor(const QDecNumber& value)
				{
                    _autoPointFactor = value;
				}

                void setOrderVariationFactor(const QDecNumber& value)
				{
                    _orderVariationFactor = value;
				}


				public:
					void update(QXmlStreamReader& xmlStreamReader)
					{
						QXmlStreamAttributes attributes = xmlStreamReader.attributes();
                        this->_id = QUuid(attributes.value("ID").toString());
                        this->_pricingPolicyId = QUuid(attributes.value("PricingPolicyId").toString());
                        this->_price = QDecNumber::fromQString(attributes.value("Price").toString());
                        this->_numerator = attributes.value("Numerator").toInt();
                        this->_spreadFactor = QDecNumber::fromQString(attributes.value("SpreadFactor").toString());
                        this->_autoPointFactor = QDecNumber::fromQString(attributes.value("AutoPointFactor").toString());
                        this->_orderVariationFactor = QDecNumber::fromQString(attributes.value("OrderVariationFactor").toString());
					}

					void update(const QDomNode& xmlNode)
					{
						QDomNamedNodeMap map = xmlNode.attributes();
						for (int i = 0; i < map.count(); i++)
						{
							QDomNode qDomNode = map.item(i);
							QString name = qDomNode.nodeName();
							QString value = qDomNode.nodeValue();
                            if (name == "ID")
                                 this->_id = QUuid(value);
                            else if (name == "PricingPolicyId")
                                this->_pricingPolicyId = QUuid(value);
                            else if (name == "Price")
                                this->_price = QDecNumber::fromQString(value);
                            else if (name == "Numerator")
                                this->_numerator = value.toInt();
                            else if (name == "SpreadFactor")
                                this->_spreadFactor = QDecNumber::fromQString(value);
                            else if (name == "AutoPointFactor")
                                this->_autoPointFactor = QDecNumber::fromQString(value);
                            else if (name == "OrderVariationFactor")
                                this->_orderVariationFactor = QDecNumber::fromQString(value);
						}
					}
			};
		}
	}
}
#endif

