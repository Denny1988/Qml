#include "PlaceSetting.h"
#include "Common/Util/Convert_Common.h"
#include "Common/SignalForwarder.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			PlaceSetting::PlaceSetting()
			{
				this->_isAutoClose = false;
				this->_allowNewOpenBuy = false;
				this->_allowNewOpenSell = false;
				this->_priceValidTime = -1;
				this->_quoteTimeout = 0;
				this->_allowFullPayment = false;
				this->_allowAdvancePayment = false;
				this->_allowInstalment = false;
				this->_allowDelivery = false;
				this->_allowShortSell = true;
				this->_closingUseCustomerQuotePolicy = false;
			}

			PlaceSetting::~PlaceSetting()
			{
			}

			QSharedPointer<PlaceSettingDetail>& PlaceSetting::getOrAddPlaceSettingDetail(Common::Enum::OrderType::OrderType orderType)
			{
				if(!this->_placeSettingDetails.contains(orderType))
				{
					QSharedPointer<PlaceSettingDetail> placeSettingDetail(new PlaceSettingDetail);
					placeSettingDetail->init();
					this->_placeSettingDetails.insert(orderType, placeSettingDetail);					
				}
				return this->_placeSettingDetails[orderType];
			}

			QSharedPointer<PlaceSettingDetail> PlaceSetting::getPlaceSettingDetail(Common::Enum::OrderType::OrderType orderType)
			{
				return this->_placeSettingDetails.value(orderType);
			}

			void PlaceSetting::addPlaceSettingDetail(Common::Enum::OrderType::OrderType orderType, const QSharedPointer<PlaceSettingDetail>& placeSettingDetail)
			{
				if(!this->_placeSettingDetails.contains(orderType))
				{
					this->_placeSettingDetails.insert(orderType, placeSettingDetail);
				}
			}

			bool PlaceSetting::containsPlaceSettingDetail(Common::Enum::OrderType::OrderType orderType)
			{
				return this->_placeSettingDetails.contains(orderType);
			}

			QList<QSharedPointer<PlaceSettingDetail>> PlaceSetting::getPlaceSettingDetails()
			{
				return this->_placeSettingDetails.values();
			}

			void PlaceSetting::update(const QDomNode& xmlPlaceSetting)
			{
				QDomNamedNodeMap map = xmlPlaceSetting.attributes();
				for (int i = 0; i < map.count(); i++)
				{
					QDomNode qDomNode = map.item(i);
					QString name = qDomNode.nodeName();
					QString value = qDomNode.nodeValue();
					if(name == "IsAutoClose")
						this->_isAutoClose= Common::toBool(value);
					else if(name == "AllowNewOpenBuy")
						this->_allowNewOpenBuy= Common::toBool(value);
					else if(name == "AllowNewOpenSell")
						this->_allowNewOpenSell= Common::toBool(value);
					else if(name == "PriceValidTime")
					{
						if(this->_priceValidTime == -1)
						{
							this->_priceValidTime = value.toInt();
						}
						else
						{
							if(this->_priceValidTime!=value.toInt())
							{
								int lastPriceValidTime = this->_priceValidTime;
								this->_priceValidTime = value.toInt();
								if((lastPriceValidTime == 0 && value.toInt()>0)
									|| (lastPriceValidTime > 0 && value.toInt()== 0))
								{
									Common::SignalForwarder::getInstance()->sendRefeshEnableFastTradeSignal();
								}
							}
						}
					}
					else if(name == "QuoteTimeout")
						this->_quoteTimeout= value.toInt();
					else if(name == "AllowFullPayment")
						this->_allowFullPayment= Common::toBool(value);
					else if(name == "AllowAdvancePayment")
						this->_allowAdvancePayment= Common::toBool(value);
					else if(name == "AllowInstalment")
						this->_allowInstalment= Common::toBool(value);
					else if(name == "AllowDelivery")
					{
						this->_allowDelivery= Common::toBool(value);
						Common::SignalForwarder::getInstance()->sendRefreshPhysicalButtonIsEnable();
					}
					else if(name == "AllowShortSell")
					{
						this->_allowShortSell= Common::toBool(value);
					}
					else if(name == "ClosingUseCustomerQuotePolicy")
						this->_closingUseCustomerQuotePolicy= Common::toBool(value);
					else if(name == "ContractSize")
						this->_contractSize= QDecNumber::fromQString(value);
				}

				QDomNodeList childlist = xmlPlaceSetting.childNodes();  
				for (int i = 0; i < childlist.count(); i++)
				{
					QDomNode itemQDomNode = childlist.item(i);
					QString name = itemQDomNode.nodeName();
					if(name == "PlaceSettingDetail")
					{
						Common::Enum::OrderType::OrderType orderType =  static_cast<Common::Enum::OrderType::OrderType>(itemQDomNode.toElement().attribute("OrderType").toInt());
						QSharedPointer<Model::Setting::PlaceSettingDetail> placeSettingDetail = this->getOrAddPlaceSettingDetail(orderType);
						placeSettingDetail->update(itemQDomNode);
					}
				}
			}

            QSharedPointer<PlaceSetting> PlaceSetting::copy()
            {
                QSharedPointer<PlaceSetting> newObject(new PlaceSetting);
                newObject->_isAutoClose = _isAutoClose;
                newObject->_allowNewOpenBuy = _allowNewOpenBuy;
                newObject->_allowNewOpenSell = _allowNewOpenSell;
                newObject->_priceValidTime = _priceValidTime;
                newObject->_quoteTimeout = _quoteTimeout;
                newObject->_allowFullPayment = _allowFullPayment;
                newObject->_allowAdvancePayment = _allowAdvancePayment;
                newObject->_allowInstalment = _allowInstalment;
                newObject->_allowDelivery = _allowDelivery;
                newObject->_allowShortSell = _allowShortSell;
                newObject->_closingUseCustomerQuotePolicy = _closingUseCustomerQuotePolicy;
                newObject->_contractSize = _contractSize;

                if(this->_placeSettingDetails.count() > 0)
                {
                    QMap<Common::Enum::OrderType::OrderType, QSharedPointer<PlaceSettingDetail>> copyPlaceSettingDetails;
                    QMapIterator<Common::Enum::OrderType::OrderType, QSharedPointer<PlaceSettingDetail>> item(this->_placeSettingDetails);
                    while (item.hasNext()) {
                        item.next();
                        copyPlaceSettingDetails.insert(item.key(), item.value()->copy());
                    }
                    newObject->_placeSettingDetails = copyPlaceSettingDetails;
                }
                return newObject;
            }
        }
	}
}
