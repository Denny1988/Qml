#include "BusinessInstrument.h"
#include "Common/Util/Convert_Common.h"
#include <QDomNode>
#include <qDebug>
#include "Common/Constants/DateTimeConstants.h"
#include "Common/SignalForwarder.h"
#include "Common/TraderBaseInfo.h"
using namespace Common::Constants;

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			BusinessInstrument::BusinessInstrument()
			{
				this->_expired = false;
			}

			BusinessInstrument::~BusinessInstrument()
			{
			}

			QSharedPointer<LotSetting>& BusinessInstrument::getLotSetting(bool isOpen, Common::Enum::OrderType::OrderType orderType)
			{
				QString key = QString("%1_%2").arg(QString::number((int)orderType), isOpen ? "True" : "False");
				return this->_lotSettings[key];
			}

			QSharedPointer<LotSetting>& BusinessInstrument::getOrAddLotSetting(const QString& key)
			{
				if (!this->_lotSettings.contains(key))
				{
					QSharedPointer<LotSetting> lotSetting(new LotSetting);
					this->_lotSettings.insert(key, lotSetting);
				}				
				return this->_lotSettings[key];
			}


			QSharedPointer<LotSetting>& Model::Setting::BusinessInstrument::getOrAddLotSetting(bool isOpen, Common::Enum::OrderType::OrderType orderType)
			{
				QString key = QString("%1_%2").arg(QString::number((int)orderType), isOpen ? "True" : "False");
				if (!this->_lotSettings.contains(key))
				{
					QSharedPointer<LotSetting> lotSetting(new LotSetting);
					lotSetting->setIsOpen(isOpen);
					lotSetting->setOrderType(orderType);
					this->_lotSettings.insert(key, lotSetting);
				}
				return this->_lotSettings[key];
			}

			QSharedPointer<DeliverySpecificationDetail>& Model::Setting::BusinessInstrument::getOrAddDeliverySpecificationDetail(QString size)
			{
				if (!this->_deliverySpecificationDetails.contains(size))
				{
					QSharedPointer<DeliverySpecificationDetail> newObject(new DeliverySpecificationDetail);
					this->_deliverySpecificationDetails.insert(size, newObject);
				}
				return this->_deliverySpecificationDetails[size];
			}

			void BusinessInstrument::update(const QDomNode& xmlBusinessInstrument)
			{
				QDomNamedNodeMap map = xmlBusinessInstrument.attributes();
				for (int i = 0; i < map.count(); i++)
				{
					QDomNode qDomNode = map.item(i);
					QString name = qDomNode.nodeName();
					QString value = Common::fix(qDomNode.nodeValue());
					if (name == "Id")
						this->_id = QUuid(value);
					else if (name == "OriginCode")
						this->_originCode = value;
					else if (name == "Code")
						this->_code = value;
					else if (name == "Description")
						this->_instrumentDescription = value;
					else if (name == "IsSinglePrice")
						this->_isSinglePrice = Common::toBool(value);
					else if (name == "Narrative")
						this->_narrative = value;
					else if (name == "QuoteDescription")
						this->_quoteDescription = value;
					else if (name == "UnitId")
						this->_unitId = QUuid(value);
					else if (name == "Unit")
						this->_unit = value;
					else if (name == "CurrencyId")
						this->_currencyId = QUuid(value);
					else if (name == "PlaceConfirmMinTime")
						this->_placeConfirmMinTime = value.toInt();
					else if (name == "ExceptionEnquiryOutTime")
						this->_exceptionEnquiryOutTime = value.toInt();
					else if (name == "EnquiryOutTime")
						this->_enquiryOutTime = value.toInt();
					else if (name == "MaxAlertPricePerInstrument")
						this->_maxAlertPricePerInstrument = value.toInt();
					else if (name == "DeliveryPointGroupId")
						this->_deliveryPointGroupId = QUuid(value);
					else if (name == "Category")
						this->_instrumentCategory = static_cast<InstrumentCategory::InstrumentCategory>(value.toInt());
					else if (name == "SpreadPoints")
						this->_spreadPoints = value.toInt();
					else if (name == "AutoAdjustPoints")
						this->_autoAdjustPoints = value.toInt();
					else if (name == "ShowPriceSuperscript")
						this->_showPriceSuperscript = Common::toBool(value);
					else if (name == "EnhancedPriceDigit")
						this->_enhancedPriceDigit = value.toInt();
					else if (name == "IsNormal")
						this->_isNormal = Common::toBool(value);
					else if (name == "PhysicalLotDecimal")
						this->_physicalLotDecimal = value.toInt();
					else if (name == "Weight")
						this->_weight = value.toInt();
					else if (name == "PriceType")
						this->_priceType = static_cast<PriceType::PriceType>(value.toInt());
					else if (name == "DeliveryTimeBeginDay")
						this->_deliveryTimeBeginDay = value.toInt();
					else if (name == "DeliveryTimeEndDay")
						this->_deliveryTimeEndDay = value.toInt();
					else if (name == "BuyInterestValueDay")
						this->_buyInterestValueDay = value.toInt();
					else if (name == "SellInterestValueDay")
						this->_sellInterestValueDay = value.toInt();
					else if (name == "BeginTime")
						this->_beginTime = Common::convertToDateTime(value);
					else if (name == "EndTime")
						this->_endTime = Common::convertToDateTime(value);
				}
				QDomNodeList childlist = xmlBusinessInstrument.childNodes();
				for (int i = 0; i < childlist.count(); i++)
				{
					QDomNode itemQDomNode = childlist.item(i);
					QString name = itemQDomNode.nodeName();
					if (name == "InterestRateSetting")
					{
						if (this->_interestRateSetting.isNull())
						{
							QSharedPointer<Model::Setting::InterestRate> interestRateSetting(new Model::Setting::InterestRate);
							this->_interestRateSetting = interestRateSetting;
						}
						this->_interestRateSetting->update(itemQDomNode);
					}
					else if (name == "PlaceSetting")
					{
						if (this->_placeSetting.isNull())
						{
							QSharedPointer<Model::Setting::PlaceSetting> placeSetting(new Model::Setting::PlaceSetting);
							this->_placeSetting = placeSetting;
						}
						this->_placeSetting->update(itemQDomNode);
						Common::SignalForwarder::getInstance()->sendRefreshBOFrequenciesSignal(this->_id);
					}
					else if (name == "CalculateSetting")
					{
						if (this->_calculateSetting.isNull())
						{
							QSharedPointer<Model::Setting::CalculateSetting> calculateSetting(new Model::Setting::CalculateSetting);
							this->_calculateSetting = calculateSetting;
						}
						this->_calculateSetting->update(itemQDomNode);
						QList<QUuid> instrumentIds;
						instrumentIds.append(this->_id);
						Common::SignalForwarder::getInstance()->sendComputeQuotationSignal(instrumentIds);
						Common::SignalForwarder::getInstance()->sendComputeAllSignal(instrumentIds);
					}
					else if (name == "MovePipsInfoSetting")
					{
						if (this->_movePipsInfoSetting.isNull())
						{
							QSharedPointer<Model::Setting::MovePipsInfoSetting> movePipsInfoSetting(new Model::Setting::MovePipsInfoSetting);
							this->_movePipsInfoSetting = movePipsInfoSetting;
						}
						this->_movePipsInfoSetting->update(itemQDomNode);
					}
					else if (name == "PrepaymentInterestRateSetting")
					{
						QString isDelete = itemQDomNode.toElement().attribute("IsDelete");
						if (isDelete == "False" || isDelete == "")
						{
							if (this->_prepaymentInterestRateSetting.isNull())
							{
								QSharedPointer<Model::Setting::InterestRate> interestRateSetting(new Model::Setting::InterestRate);
								this->_prepaymentInterestRateSetting = interestRateSetting;
							}
							this->_prepaymentInterestRateSetting->update(itemQDomNode);
						}
						else
						{
							this->_prepaymentInterestRateSetting.clear();
						}
					}
					else if (name == "DeliveryChargeSetting")
					{
						QString isDelete = itemQDomNode.toElement().attribute("IsDelete");
						if (isDelete == "False" || isDelete == "")
						{
							if (this->_deliveryChargeSetting.isNull())
							{
								QSharedPointer<Model::Setting::DeliveryCharge> deliveryChargeSetting(new Model::Setting::DeliveryCharge);
								this->_deliveryChargeSetting = deliveryChargeSetting;
							}
							this->_deliveryChargeSetting->update(itemQDomNode);
						}
						else
						{
							this->_deliveryChargeSetting.clear();
						}
					}
					else if (name == "VolumeNecessarySetting")
					{
						QString isDelete = itemQDomNode.toElement().attribute("IsDelete");
						if (isDelete == "False" || isDelete == "")
						{
							if (this->_volumeNecessarySetting.isNull())
							{
								QSharedPointer<Model::Setting::VolumeNecessary> volumeNecessarySetting(new Model::Setting::VolumeNecessary);
								this->_volumeNecessarySetting = volumeNecessarySetting;
							}
							this->_volumeNecessarySetting->update(itemQDomNode);
						}
						else
						{
							this->_volumeNecessarySetting.clear();
						}
						QList<QUuid> instrumentIds;
						instrumentIds.append(this->_id);
						Common::SignalForwarder::getInstance()->sendComputeQuotationSignal(instrumentIds);
						Common::SignalForwarder::getInstance()->sendComputeAllSignal(instrumentIds);
					}
					else if (name == "InstalmentSetting")
					{
						QString isDelete = itemQDomNode.toElement().attribute("IsDelete");
						if (isDelete == "False" || isDelete == "")
						{
							if (this->_instalmentSettingSetting.isNull())
							{
								QSharedPointer<Model::Setting::InstalmentSetting> instalmentSettingSetting(new Model::Setting::InstalmentSetting);
								this->_instalmentSettingSetting = instalmentSettingSetting;
							}
							this->_instalmentSettingSetting->update(itemQDomNode);
						}
						else
						{
							this->_instalmentSettingSetting.clear();
						}
					}
					else if (name == "BOPolicySetting")
					{
						QString isDelete = itemQDomNode.toElement().attribute("IsDelete");
						if (isDelete == "False" || isDelete == "")
						{
							if (itemQDomNode.toElement().hasAttribute("IsClearChildren"))
							{
								QString isClearChildren = itemQDomNode.toElement().attribute("IsClearChildren");
								if (isClearChildren == "True") this->_BOPolicySetting.clear();
							}
							if (this->_BOPolicySetting.isNull())
							{
								QSharedPointer<Model::Setting::BOPolicySetting> BOPolicySetting(new Model::Setting::BOPolicySetting);
								this->_BOPolicySetting = BOPolicySetting;
							}
							this->_BOPolicySetting->update(itemQDomNode);
						}
						else
						{
							this->_BOPolicySetting.clear();
						}
						Common::SignalForwarder::getInstance()->sendRefreshBOFrequenciesSignal(this->_id);
					}
					else if (name == "PhysicalPaymentDiscountSetting")
					{
						QString isDelete = itemQDomNode.toElement().attribute("IsDelete");
						if (isDelete == "False" || isDelete == "")
						{
							if (this->_physicalPaymentDiscountSetting.isNull())
							{
								QSharedPointer<Model::Setting::PhysicalPaymentDiscountSetting> physicalPaymentDiscountSetting(new Model::Setting::PhysicalPaymentDiscountSetting);
								this->_physicalPaymentDiscountSetting = physicalPaymentDiscountSetting;
							}
							this->_physicalPaymentDiscountSetting->update(itemQDomNode);
						}
						else
						{
							this->_physicalPaymentDiscountSetting.clear();
						}
					}
					else if (name == "LotSettings")
					{
						QDomNodeList itemChildlist = itemQDomNode.childNodes();
						for (int j = 0; j < itemChildlist.count(); j++)
						{
							QDomNode itemChildQDomNode = itemChildlist.item(j);
							QString itemName = itemChildQDomNode.nodeName();
							if (itemName == "LotSetting")
							{
								QString orderType = itemChildQDomNode.toElement().attribute("OrderType");
								QString isOpen = itemChildQDomNode.toElement().attribute("IsOpen");
								QString key = QString("%1_%2").arg(orderType, isOpen);
								QSharedPointer<Model::Setting::LotSetting> lotSetting = this->getOrAddLotSetting(key);
								lotSetting->update(itemChildQDomNode);
							}
						}
					}
					else if (name == "DeliverySpecificationDetailSettings")
					{
						QString isDelete = itemQDomNode.toElement().attribute("IsDelete");
						if (isDelete == "False" || isDelete == "")
						{
							QDomNodeList itemChildlist = itemQDomNode.childNodes();
							for (int j = 0; j < itemChildlist.count(); j++)
							{
								QDomNode itemChildQDomNode = itemChildlist.item(j);
								QString itemName = itemChildQDomNode.nodeName();
								if (itemName == "DeliverySpecificationDetail")
								{
									QString size = itemChildQDomNode.toElement().attribute("Size");
									if (this->_deliverySpecificationDetails.contains(size))
									{
										this->_deliverySpecificationDetails[size]->setDeliveryCharge(itemChildQDomNode.toElement().attribute("DeliveryCharge"));
									}
									else
									{
										QSharedPointer<Model::Setting::DeliverySpecificationDetail> deliverySpecificationDetail(new Model::Setting::DeliverySpecificationDetail);
										deliverySpecificationDetail->setSize(size);
										deliverySpecificationDetail->setDeliveryCharge(itemChildQDomNode.toElement().attribute("DeliveryCharge"));                                        
                                        if(Common::TraderBaseInfo::getInstance()->getLanguage() == "ENG")
                                            deliverySpecificationDetail->setProductName(itemChildQDomNode.toElement().attribute("ENG"));
                                        else if(Common::TraderBaseInfo::getInstance()->getLanguage() == "CHS")
                                            deliverySpecificationDetail->setProductName(itemChildQDomNode.toElement().attribute("CHS"));
                                        else if(Common::TraderBaseInfo::getInstance()->getLanguage() == "CHT")
                                            deliverySpecificationDetail->setProductName(itemChildQDomNode.toElement().attribute("CHT"));
                                        else if(Common::TraderBaseInfo::getInstance()->getLanguage() == "JPN")
                                            deliverySpecificationDetail->setProductName(itemChildQDomNode.toElement().attribute("JPN"));
                                        else if(Common::TraderBaseInfo::getInstance()->getLanguage() == "VN")
                                            deliverySpecificationDetail->setProductName(itemChildQDomNode.toElement().attribute("VN"));
                                        else if(Common::TraderBaseInfo::getInstance()->getLanguage() == "MAL")
                                            deliverySpecificationDetail->setProductName(itemChildQDomNode.toElement().attribute("MAL"));
										this->_deliverySpecificationDetails.insert(size, deliverySpecificationDetail);
									}
								}
							}
						}
						else
						{
							this->_deliverySpecificationDetails.clear();
							QDomNodeList itemChildlist = itemQDomNode.childNodes();
							for (int j = 0; j < itemChildlist.count(); j++)
							{
								QDomNode itemChildQDomNode = itemChildlist.item(j);
								QString itemName = itemChildQDomNode.nodeName();
								if (itemName == "DeliverySpecificationDetail")
								{
									QString size = itemChildQDomNode.toElement().attribute("Size");
									QSharedPointer<Model::Setting::DeliverySpecificationDetail> deliverySpecificationDetail(new Model::Setting::DeliverySpecificationDetail);
									deliverySpecificationDetail->setSize(size);
									deliverySpecificationDetail->setDeliveryCharge(itemChildQDomNode.toElement().attribute("DeliveryCharge"));
                                    if(Common::TraderBaseInfo::getInstance()->getLanguage() == "ENG")
                                        deliverySpecificationDetail->setProductName(itemChildQDomNode.toElement().attribute("ENG"));
                                    else if(Common::TraderBaseInfo::getInstance()->getLanguage() == "CHS")
                                        deliverySpecificationDetail->setProductName(itemChildQDomNode.toElement().attribute("CHS"));
                                    else if(Common::TraderBaseInfo::getInstance()->getLanguage() == "CHT")
                                        deliverySpecificationDetail->setProductName(itemChildQDomNode.toElement().attribute("CHT"));
                                    else if(Common::TraderBaseInfo::getInstance()->getLanguage() == "JPN")
                                        deliverySpecificationDetail->setProductName(itemChildQDomNode.toElement().attribute("JPN"));
                                    else if(Common::TraderBaseInfo::getInstance()->getLanguage() == "VN")
                                        deliverySpecificationDetail->setProductName(itemChildQDomNode.toElement().attribute("VN"));
                                    else if(Common::TraderBaseInfo::getInstance()->getLanguage() == "MAL")
                                        deliverySpecificationDetail->setProductName(itemChildQDomNode.toElement().attribute("MAL"));
									this->_deliverySpecificationDetails.insert(size, deliverySpecificationDetail);
								}
							}

						}
					}
				}
			}


			bool BusinessInstrument::allowPlaceBO()
			{
				return this->_placeSetting->getPlaceSettingDetail(Common::Enum::OrderType::BinaryOption)->getAllowTrade();
			}

			QSharedPointer<InterestRate>& Model::Setting::BusinessInstrument::getOrAddInterestRateSetting()
			{
				if (this->_interestRateSetting.isNull())
				{
					QSharedPointer<InterestRate> newObject(new InterestRate);
					this->_interestRateSetting = newObject;
				}
				return this->_interestRateSetting;
			}

			QSharedPointer<InterestRate>& Model::Setting::BusinessInstrument::getOrAddPrepaymentInterestRateSetting()
			{
				if (this->_prepaymentInterestRateSetting.isNull())
				{
					QSharedPointer<InterestRate> newObject(new InterestRate);
					this->_prepaymentInterestRateSetting = newObject;
				}
				return this->_prepaymentInterestRateSetting;
			}

			QSharedPointer<DeliveryCharge>& Model::Setting::BusinessInstrument::getOrAddDeliveryChargeSetting()
			{
				if (this->_deliveryChargeSetting.isNull())
				{
					QSharedPointer<DeliveryCharge> newObject(new DeliveryCharge);
					this->_deliveryChargeSetting = newObject;
				}
				return this->_deliveryChargeSetting;
			}

			QSharedPointer<PlaceSetting>& Model::Setting::BusinessInstrument::getOrAddPlaceSetting()
			{
				if (this->_placeSetting.isNull())
				{
					QSharedPointer<PlaceSetting> newObject(new PlaceSetting);
					this->_placeSetting = newObject;
				}
				return this->_placeSetting;
			}

			QSharedPointer<CalculateSetting>& Model::Setting::BusinessInstrument::getOrAddCalculateSetting()
			{
				if (this->_calculateSetting.isNull())
				{
					QSharedPointer<CalculateSetting> newObject(new CalculateSetting);
					this->_calculateSetting = newObject;
				}
				return this->_calculateSetting;
			}

			QSharedPointer<MovePipsInfoSetting>& Model::Setting::BusinessInstrument::getOrAddMovePipsInfoSetting()
			{
				if (this->_movePipsInfoSetting.isNull())
				{
					QSharedPointer<MovePipsInfoSetting> newObject(new MovePipsInfoSetting);
					this->_movePipsInfoSetting = newObject;
				}
				return this->_movePipsInfoSetting;
			}

			QSharedPointer<VolumeNecessary>& Model::Setting::BusinessInstrument::getOrAddVolumeNecessarySetting()
			{
				if (this->_volumeNecessarySetting.isNull())
				{
					QSharedPointer<VolumeNecessary> newObject(new VolumeNecessary);
					this->_volumeNecessarySetting = newObject;
				}
				return this->_volumeNecessarySetting;
			}

			QSharedPointer<InstalmentSetting>& Model::Setting::BusinessInstrument::getOrAddInstalmentSettingSetting()
			{
				if (this->_instalmentSettingSetting.isNull())
				{
					QSharedPointer<InstalmentSetting> newObject(new InstalmentSetting);
					this->_instalmentSettingSetting = newObject;
				}
				return this->_instalmentSettingSetting;
			}

			QSharedPointer<PhysicalPaymentDiscountSetting>& Model::Setting::BusinessInstrument::getOrAddPhysicalPaymentDiscountSetting()
			{
				if (this->_physicalPaymentDiscountSetting.isNull())
				{
					QSharedPointer<PhysicalPaymentDiscountSetting> newObject(new PhysicalPaymentDiscountSetting);
					this->_physicalPaymentDiscountSetting = newObject;
				}
				return this->_physicalPaymentDiscountSetting;
			}

			QSharedPointer<BOPolicySetting>& Model::Setting::BusinessInstrument::getOrAddBOPolicySetting()
			{
				if (this->_BOPolicySetting.isNull())
				{
					QSharedPointer<BOPolicySetting> newObject(new BOPolicySetting);
					this->_BOPolicySetting = newObject;
				}
				return this->_BOPolicySetting;
			}

            void Model::Setting::BusinessInstrument::addOrUpdateAdministrativeChargeDetail(QDecNumber quantity, QDecNumber chargeRate)
            {
                if(this->_administrativeChargeDetails.contains(quantity))
                    this->_administrativeChargeDetails[quantity] = chargeRate;
                else
                    this->_administrativeChargeDetails.insert(quantity, chargeRate);
            }

            QMap<QDecNumber, QDecNumber> BusinessInstrument::getAdministrativeChargeDetails()
            {
                return this->_administrativeChargeDetails;
            }

            void BusinessInstrument::addPricingPolicyDetail(QDecNumber price, QSharedPointer<PricingPolicyDetail> pricingPolicyDetail)
            {
                this->_pricingPolicyDetails.insert(price, pricingPolicyDetail);
            }

            QSharedPointer<PricingPolicyDetail> BusinessInstrument::getPricingPolicyDetail(QDecNumber marketPrice)
            {
                QSharedPointer<PricingPolicyDetail> maxPrcingPolicy(NULL);
                QMapIterator<QDecNumber, QSharedPointer<PricingPolicyDetail>> item(this->_pricingPolicyDetails);
                while (item.hasNext()) {
                  item.next();
                  maxPrcingPolicy = item.value();
                  qDebug() << " BusinessInstrument::getPricingPolicyDetail: price:"<< item.key().toDouble() << " marketPrice:" << marketPrice.toDouble();                  
                  if(item.key().toDouble() >= marketPrice.toDouble())
                  {
                      break;
                  }				  
                }                
                return maxPrcingPolicy;
            }

            bool BusinessInstrument::isExistPricingPolicy()
            {
                return this->_pricingPolicyDetails.count() > 0;
            }

            QSharedPointer<BusinessInstrument> BusinessInstrument::copy()
            {
                QSharedPointer<BusinessInstrument> newObject(new BusinessInstrument);
                newObject->_id = _id;
                newObject->_originCode = _originCode;
                newObject->_code = _code;
                newObject->_isSinglePrice = _isSinglePrice;
                newObject->_instrumentDescription = _instrumentDescription;
                newObject->_narrative = _narrative;
                newObject->_quoteDescription = _quoteDescription;
                newObject->_unitId = _unitId;
                newObject->_unit = _unit;
                newObject->_currencyId = _currencyId;
                newObject->_placeConfirmMinTime = _placeConfirmMinTime;
                newObject->_maxAlertPricePerInstrument = _maxAlertPricePerInstrument;
                newObject->_exceptionEnquiryOutTime = _exceptionEnquiryOutTime;
                newObject->_enquiryOutTime = _enquiryOutTime;
                newObject->_deliveryPointGroupId = _deliveryPointGroupId;
                newObject->_instrumentCategory = _instrumentCategory;
                newObject->_spreadPoints = _spreadPoints;
                newObject->_autoAdjustPoints = _autoAdjustPoints;
                newObject->_showPriceSuperscript = _showPriceSuperscript;
                newObject->_enhancedPriceDigit = _enhancedPriceDigit;
                newObject->_isNormal = _isNormal;
                newObject->_physicalLotDecimal = _physicalLotDecimal;
                newObject->_weight = _weight;
                newObject->_expired = _expired;
                newObject->_deliveryTimeBeginDay = _deliveryTimeBeginDay;
                newObject->_deliveryTimeEndDay = _deliveryTimeEndDay;
                newObject->_buyInterestValueDay = _buyInterestValueDay;
                newObject->_sellInterestValueDay = _sellInterestValueDay;
                newObject->_beginTime = _beginTime;
                newObject->_endTime = _endTime;
                newObject->_priceType = _priceType;
                newObject->_deliveryMode = _deliveryMode;
                newObject->_administrativeChargeId = _administrativeChargeId;
                newObject->_administrativeChargeConvertRate = _administrativeChargeConvertRate;

                if(!this->_interestRateSetting.isNull())
                {
                    newObject->_interestRateSetting = this->_interestRateSetting->copy();
                }
                if(!this->_prepaymentInterestRateSetting.isNull())
                {
                    newObject->_prepaymentInterestRateSetting = this->_prepaymentInterestRateSetting->copy();
                }
                if(!this->_deliveryChargeSetting.isNull())
                {
                    newObject->_deliveryChargeSetting = this->_deliveryChargeSetting->copy();
                }
                if(!this->_placeSetting.isNull())
                {
                    newObject->_placeSetting = this->_placeSetting->copy();
                }
                if(!this->_calculateSetting.isNull())
                {
                    newObject->_calculateSetting = this->_calculateSetting->copy();
                }
                if(!this->_movePipsInfoSetting.isNull())
                {
                    newObject->_movePipsInfoSetting = this->_movePipsInfoSetting->copy();
                }
                if(!this->_volumeNecessarySetting.isNull())
                {
                    newObject->_volumeNecessarySetting = this->_volumeNecessarySetting->copy();
                }
                if(!this->_instalmentSettingSetting.isNull())
                {
                    newObject->_instalmentSettingSetting = this->_instalmentSettingSetting->copy();
                }
                if(!this->_physicalPaymentDiscountSetting.isNull())
                {
                    newObject->_physicalPaymentDiscountSetting = this->_physicalPaymentDiscountSetting->copy();
                }
                if(!this->_BOPolicySetting.isNull())
                {
                    newObject->_BOPolicySetting = this->_BOPolicySetting->copy();
                }
                if(this->_lotSettings.count() > 0)
                {
                    QMap<QString, QSharedPointer<LotSetting>> copyLotSettings;
                    QMapIterator<QString, QSharedPointer<LotSetting>> item(this->_lotSettings);
                    while (item.hasNext()) {
                        item.next();
                        copyLotSettings.insert(item.key(), item.value()->copy());
                    }
                    newObject->_lotSettings = copyLotSettings;
                }
                if(this->_deliverySpecificationDetails.count() > 0)
                {
                    QMap<QString, QSharedPointer<DeliverySpecificationDetail>> copyDeliverySpecificationDetails;
                    QMapIterator<QString, QSharedPointer<DeliverySpecificationDetail>> item(this->_deliverySpecificationDetails);
                    while (item.hasNext()) {
                        item.next();
                        copyDeliverySpecificationDetails.insert(item.key(), item.value()->copy());
                    }
                    newObject->_deliverySpecificationDetails = copyDeliverySpecificationDetails;
                }
                if(this->_pricingPolicyDetails.count() > 0)
                {
                    QMap<QDecNumber, QSharedPointer<PricingPolicyDetail>> copyPricingPolicyDetails;
                    QMapIterator<QDecNumber, QSharedPointer<PricingPolicyDetail>> item(this->_pricingPolicyDetails);
                    while (item.hasNext()) {
                        item.next();
                        copyPricingPolicyDetails.insert(item.key(), item.value()->copy());
                    }
                    newObject->_pricingPolicyDetails = copyPricingPolicyDetails;
                }
                return newObject;
            }
		}
	}
}
