#ifndef BUSINESS_MODEL_TRADING_DELIVAERYREQUEST_H
#define BUSINESS_MODEL_TRADING_DELIVAERYREQUEST_H
#include <quuid.h>
#include <QDateTime.h>
#include "Common/Enum/PhysicalEnum.h"
#include "DeliveryRequestOrderRelation.h"
#include "QDecNumber.h"
#include <QDomNode>
#include "Common/Constants/DateTimeConstants.h"
#include <QXmlStreamReader>
using namespace Common::Constants;

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			class DeliveryRequest
			{
			private:
				QUuid  _id;
				QUuid  _accountId;
				QUuid  _instrumentId;
				QUuid  _chargeCurrencyId;
				QString  _charge;
				QString _instrumentDesc;
				QString  _requireLot;
				QString  _requireQuantity;
				QString _code;
				QString _unit;
				QDateTime _availableTime;
				QDateTime _deliveryTime;
				Common::Enum::DeliveryRequestStatus::DeliveryRequestStatus _status;
				QDateTime _submitTime;
				QUuid _deliveryAddressId;

				QList<QSharedPointer<DeliveryRequestOrderRelation>> _deliveryRequestOrderRelations;		
				QMap<QUuid, QDecNumber> _orderDelivaryLockLots;		

			public:
				void update(QXmlStreamReader& xmlStreamReader)
				{	
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();  
					this->_id = QUuid(attributes.value("Id").toString());
					this->_instrumentId = QUuid(attributes.value("InstrumentId").toString());
					this->_code = attributes.value("Code").toString();
					this->_requireQuantity = attributes.value("RequireQuantity").toString();
					this->_unit = attributes.value("Unit").toString();					
                    this->_availableTime = QDateTime::fromString(attributes.value("AvailableDeliveryTime").toString(), DateTimeConstants::DateTimeFormat);
					this->_status = static_cast<Common::Enum::DeliveryRequestStatus::DeliveryRequestStatus>(attributes.value("Status").toInt());
					this->_deliveryTime = QDateTime::fromString(attributes.value("DeliveryTime").toString(), DateTimeConstants::DateTimeFormat);
					this->_submitTime = QDateTime::fromString(attributes.value("SubmitTime").toString(), DateTimeConstants::DateTimeFormat);

					xmlStreamReader.readNext();
					while (!xmlStreamReader.atEnd()) {
						if (xmlStreamReader.isEndElement()) {
							xmlStreamReader.readNext();
							break;
						}
						if (xmlStreamReader.isStartElement()) {
							if (xmlStreamReader.name() == "DeliveryRequestOrderRelations") {
								xmlStreamReader.readNext();
								while (!xmlStreamReader.atEnd()) {
									if (xmlStreamReader.isEndElement()) {
										xmlStreamReader.readNext();//DeliveryRequestOrderRelations end
										break;
									}
									if (xmlStreamReader.isStartElement()) {
										while (!xmlStreamReader.atEnd()) {
											if (xmlStreamReader.isEndElement()) {
												xmlStreamReader.readNext();//DeliveryRequestOrderRelation end
												break;
											}
											QSharedPointer<DeliveryRequestOrderRelation> deliveryRequestOrderRelation(new DeliveryRequestOrderRelation);
											deliveryRequestOrderRelation->update(xmlStreamReader);
											this->_deliveryRequestOrderRelations.append(deliveryRequestOrderRelation);	
											QUuid openOrderId = deliveryRequestOrderRelation->getOpenOrderId();
											if(this->_orderDelivaryLockLots.contains(openOrderId))
											{
												this->_orderDelivaryLockLots[openOrderId] = this->_orderDelivaryLockLots.value(openOrderId) + deliveryRequestOrderRelation->getDeliveryLot();
											}
											else
											{
												this->_orderDelivaryLockLots.insert(openOrderId, deliveryRequestOrderRelation->getDeliveryLot());
											}
											xmlStreamReader.readNext();
										}
									} else {
										xmlStreamReader.readNext();
									}
								}
									
								xmlStreamReader.readNext();
							}
							else if (xmlStreamReader.name() == "DeliveryRequestSpecifications")  
							{
								xmlStreamReader.readNext();
								while (!xmlStreamReader.atEnd()) {
									if (xmlStreamReader.isEndElement()) {
										xmlStreamReader.readNext();//DeliveryRequestOrderRelations end
										break;
									}
									if (xmlStreamReader.isStartElement()) {
										while (!xmlStreamReader.atEnd()) {
											if (xmlStreamReader.isEndElement()) {
												xmlStreamReader.readNext();//DeliveryRequestSpecification end
												break;
											}
											xmlStreamReader.readNext();											
										}
									} else {
										xmlStreamReader.readNext();
									}
								}
							}
							else if (xmlStreamReader.name() == "Bills")  
							{
								xmlStreamReader.readNext();
								while (!xmlStreamReader.atEnd()) {
									if (xmlStreamReader.isEndElement()) {
										xmlStreamReader.readNext();//Bills end
										break;
									}
									if (xmlStreamReader.isStartElement()) {
										while (!xmlStreamReader.atEnd()) {
											if (xmlStreamReader.isEndElement()) {
												xmlStreamReader.readNext();//Bill end
												break;
											}
											xmlStreamReader.readNext();											
										}
									} else {
										xmlStreamReader.readNext();
									}
								}
							}
						}
						else
							xmlStreamReader.readNext();
					}
				}

				void update(const QDomNode& xmlDeliveryRequest)
				{
					QDomNamedNodeMap map = xmlDeliveryRequest.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "Id")
							this->_id = QUuid(value);
						else if(name == "InstrumentId")
							this->_instrumentId = QUuid(value);
						else if(name == "Code")
							this->_code = value;
						else if(name == "RequireQuantity")
							this->_requireQuantity = value;
						else if(name == "Unit")
							this->_unit = value;
                        else if(name == "AvailableDeliveryTime")
							this->_availableTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
						else if(name == "Status")
							this->_status = static_cast<Common::Enum::DeliveryRequestStatus::DeliveryRequestStatus>(value.toInt());
						else if(name == "DeliveryTime")
							this->_deliveryTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
						else if(name == "SubmitTime")
							this->_submitTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
					}

					QDomNodeList childlist = xmlDeliveryRequest.childNodes();  
					for (int i = 0; i < childlist.count(); i++)
					{
						QDomNode itemQDomNode = childlist.item(i);
						QString name = itemQDomNode.nodeName();
						if(name == "DeliveryRequestOrderRelations")
						{
							QDomNodeList orderRelationChildlist = itemQDomNode.childNodes(); 
							for (int k = 0; k < orderRelationChildlist.count(); k++)
							{
								QDomNode deliveryRequestOrderRelationItemQDomNode = orderRelationChildlist.item(k);
								QSharedPointer<DeliveryRequestOrderRelation> deliveryRequestOrderRelation(new DeliveryRequestOrderRelation);
								deliveryRequestOrderRelation->update(deliveryRequestOrderRelationItemQDomNode);
								this->_deliveryRequestOrderRelations.append(deliveryRequestOrderRelation);								
							}
						}
					}
				}

			public:

				QUuid getId() const
				{
					return _id;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				QUuid getAccountId() const
				{
					return _accountId;
				}

				void setAccountId(const QUuid& value)
				{
					_accountId = value;
				}

				QUuid getInstrumentId() const
				{
					return _instrumentId;
				}

				void setInstrumentId(const QUuid& value)
				{
					_instrumentId = value;
				}

				QUuid getChargeCurrencyId() const
				{
					return _chargeCurrencyId;
				}

				void setChargeCurrencyId(const QUuid& value)
				{
					_chargeCurrencyId = value;
				}


				QString getCharge() const
				{
					return _charge;
				}

				void setCharge(QString value)
				{
					_charge = value;
				}


				QString getInstrumentDesc() const
				{
					return _instrumentDesc;
				}

				void setInstrumentDesc(const QString& value)
				{
					_instrumentDesc = value;
				}


				QString getRequireLot() const
				{
					return _requireLot;
				}

				void setRequireLot(QString value)
				{
					_requireLot = value;
				}

				QString getRequireQuantity() const
				{
					return _requireQuantity;
				}

				void setRequireQuantity(QString value)
				{
					_requireQuantity = value;
				}

				QString getCode() const
				{
					return _code;
				}

				void setCode(const QString& value)
				{
					_code = value;
				}

				QString getUnit() const
				{
					return _unit;
				}

				void setUnit(const QString& value)
				{
					_unit = value;
				}

				QDateTime getAvailableTime() const
				{
					return _availableTime;
				}

				void setAvailableTime(const QDateTime& value)
				{
					_availableTime = value;
				}


				QDateTime getDeliveryTime() const
				{
					return _deliveryTime;
				}

				void setDeliveryTime(const QDateTime& value)
				{
					_deliveryTime = value;
				}

				Common::Enum::DeliveryRequestStatus::DeliveryRequestStatus getStatus() const
				{
					return _status;
				}

				void setStatus(int value)
				{
					_status = static_cast<Common::Enum::DeliveryRequestStatus::DeliveryRequestStatus>(value);
				}

				QDateTime getSubmitTime() const
				{
					return _submitTime;
				}

				void setSubmitTime(const QDateTime& value)
				{
					_submitTime = value;
				}

				QUuid getDeliveryAddressId() const
				{
					return _deliveryAddressId;
				}

				void setDeliveryAddressId(const QUuid& value)
				{
					_deliveryAddressId = value;
				}

				QList<QSharedPointer<DeliveryRequestOrderRelation>> getDeliveryRequestOrderRelations()
				{
					return _deliveryRequestOrderRelations;
				}

				QMap<QUuid, QDecNumber> getOrderDelivaryLockLots()
				{
					return _orderDelivaryLockLots;
				}
			};
		}
	}
}
#endif
