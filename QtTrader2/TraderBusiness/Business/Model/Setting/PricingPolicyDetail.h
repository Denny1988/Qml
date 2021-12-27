#ifndef BUSINESS_MODEL_SETTING_PRICINGPOLICYDETAIL_H
#define BUSINESS_MODEL_SETTING_PRICINGPOLICYDETAIL_H
#include "QDecNumber.h"
#include <QUuid>

using namespace Common::Enum;
namespace Business
{
	namespace Model
	{
        namespace Setting
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
                QSharedPointer<PricingPolicyDetail> copy()
                {
                    QSharedPointer<PricingPolicyDetail> newObject(new PricingPolicyDetail);
                    newObject->_id = _id;
                    newObject->_pricingPolicyId = _pricingPolicyId;
                    newObject->_price = _price;
                    newObject->_numerator = _numerator;
                    newObject->_spreadFactor = _spreadFactor;
                    newObject->_autoPointFactor = _autoPointFactor;
                    newObject->_orderVariationFactor = _orderVariationFactor;
                    return newObject;
                }

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

