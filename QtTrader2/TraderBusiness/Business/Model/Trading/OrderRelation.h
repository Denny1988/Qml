#ifndef BUSINESS_MODEL_TRADING_ORDERRELATION_H
#define BUSINESS_MODEL_TRADING_ORDERRELATION_H
#include <quuid.h>
#include "Common/Enum/PlaceEnum.h"
#include "QDecNumber.h"
#include <QDomNode>
#include <QXmlStreamReader>

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			class OrderRelation
			{
			private:
				QUuid  _orderId1;
				QUuid  _orderId2;
				QDecNumber  _lot;
                QString  _lotStr;
				Common::Enum::OrderRelationType::OrderRelationType _relationType;
				QDecNumber  _tradePL;
				QDecNumber  _interestPL;
				QDecNumber  _storagePL;
				QDecNumber  _physicalTradePL;
				QDecNumber  _payBackPledge;
				QDecNumber  _closedPhysicalValue;
				QString _openOrderInfo;

			public:
				void update(const QXmlStreamReader& xmlStreamReader)
				{	
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();  
					this->_orderId1 = QUuid(attributes.value("OpenOrderID").toString());
					this->_orderId2 = QUuid(attributes.value("CloseOrderID").toString());
					this->_lot = QDecNumber::fromQString(attributes.value("ClosedLot").toString());
                    this->_lotStr = attributes.value("ClosedLot").toString();
					if(attributes.hasAttribute("TradePL")) this->_tradePL = QDecNumber::fromQString(attributes.value("TradePL").toString());
				}

                void update(QSharedPointer<Trading::OrderRelation> otherOrderRelation)
                {
                    if(otherOrderRelation->_lotStr.length() > 0)
                    {
                        this->_lot = otherOrderRelation->_lot;
                        this->_lotStr = otherOrderRelation->_lotStr;
                    }
                    this->_tradePL = otherOrderRelation->_tradePL;
                    this->_interestPL = otherOrderRelation->_interestPL;
                    this->_storagePL = otherOrderRelation->_storagePL;
                    this->_physicalTradePL = otherOrderRelation->_physicalTradePL;
                    this->_payBackPledge = otherOrderRelation->_payBackPledge;
                    this->_closedPhysicalValue = otherOrderRelation->_closedPhysicalValue;
                }

				void update(const QDomNode& xmlOrderRelation)
				{
					this->_relationType = Common::Enum::OrderRelationType::Close;
					QDomNamedNodeMap map = xmlOrderRelation.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "OpenOrderID")
							this->_orderId1 = QUuid(value);
						else if(name == "CloseOrderID")
							this->_orderId2 = QUuid(value);
						else if(name == "ClosedLot")
                        {
							this->_lot= QDecNumber::fromQString(value);
                            this->_lotStr = value;
                        }
						else if(name == "TradePL")
							this->_tradePL= QDecNumber::fromQString(value);
						/*else if(name == "RelationType")
							this->_relationType = static_cast<Common::Enum::OrderRelationType::OrderRelationType>(value.toInt());
						else if(name == "TradePL")
							this->_tradePL= QDecNumber::fromQString(value);
						else if(name == "InterestPL")
							this->_interestPL= QDecNumber::fromQString(value);
						else if(name == "StoragePL")
							this->_storagePL= QDecNumber::fromQString(value);
						else if(name == "PhysicalTradePL")
							this->_physicalTradePL= QDecNumber::fromQString(value);
						else if(name == "PayBackPledge")
							this->_payBackPledge= QDecNumber::fromQString(value);
						else if(name == "ClosedPhysicalValue")
							this->_closedPhysicalValue= QDecNumber::fromQString(value);*/
					}
				}
			public:

				QUuid getOrderId1() const
				{
					return _orderId1;
				}
				

				QUuid getOrderId2() const
				{
					return _orderId2;
				}
				

				QDecNumber getLot() const
				{
					return _lot;
				}
				
                QString getLotInFormat() const
                {
                    return _lotStr;
                }

				Common::Enum::OrderRelationType::OrderRelationType getRelationType() const
				{
					return _relationType;
				}

				void setRelationType(int value)
				{
					_relationType = static_cast<Common::Enum::OrderRelationType::OrderRelationType>(value);
				}



				QDecNumber getTradePL() const
				{
					return _tradePL;
				}

				void setTradePL(QDecNumber value)
				{
					_tradePL = value;
				}



				QDecNumber getInterestPL() const
				{
					return _interestPL;
				}

				void setInterestPL(QDecNumber value)
				{
					_interestPL = value;
				}



				QDecNumber getStoragePL() const
				{
					return _storagePL;
				}

				void setStoragePL(QDecNumber value)
				{
					_storagePL = value;
				}



				QDecNumber getPhysicalTradePL() const
				{
					return _physicalTradePL;
				}

				void setPhysicalTradePL(QDecNumber value)
				{
					_physicalTradePL = value;
				}



				QDecNumber getPayBackPledge() const
				{
					return _payBackPledge;
				}

				void setPayBackPledge(QDecNumber value)
				{
					_payBackPledge = value;
				}



				QDecNumber getClosedPhysicalValue() const
				{
					return _closedPhysicalValue;
				}

				void setClosedPhysicalValue(QDecNumber value)
				{
					_closedPhysicalValue = value;
				}

				QString getOpenOrderInfo()
				{
					return _openOrderInfo;
				}

				void setOpenOrderInfo(QString value)
				{
					_openOrderInfo = value;
				}

			};
		}
	}
}
#endif
