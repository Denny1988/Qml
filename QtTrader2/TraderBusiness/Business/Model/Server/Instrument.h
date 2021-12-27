#ifndef BUSINESS_MODEL_SERVER_INSTRUMENT_H
#define BUSINESS_MODEL_SERVER_INSTRUMENT_H
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include "Common/Enum/InstrumentEnum.h"
#include "PricingPolicyDetail.h"
#include <QUuid>
#include <QXmlStreamReader>
#include <qDebug>

using namespace Common::Enum;
namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class Instrument
			{
			private:
				int _acceptCloseLmtVariation;
				int _acceptDQVariation;
				int _acceptIfDoneVariation;
				int _acceptLmtVariation;
				int _allowedNewTradeSides;
				QDateTime _beginTime;
				bool _canPlacePendingOrderAtAnyTime;
				int _cancelLmtVariation;
				InstrumentCategory::InstrumentCategory _category;
				QString _code;
				int _commissionFormula;
				QUuid _currencyId;
				QDecNumber _dQQuoteMinLot;
				QUuid _deliverySpecificationId;
				int _deliveryTimeBeginDay;
				int _deliveryTimeEndDay;
				int _denominator;
				QString _description;
				QDateTime _endTime;
				int _enhancedPriceDigit;
				QUuid _id;
				bool _isActive;
				bool _isNormal;
				bool _isSinglePrice;
				int _levyFormula;
				int _marginFormula;
				QDecNumber _maxDQLot;
				QDecNumber _maxOtherLot;
				QString _narrative;
				int _numeratorUnit;
				int _orderTypeMask;
				QString _originCode;
				int _physicalLotDecimal;
				int _priceValidTime;
				QString _quoteDescription;
				bool _showPriceSuperscript;
				int _tradePLFormula;
				QString _unit;
				QUuid _unitId;
				int _rearPrice;
				int _counterPartyId;
                QUuid _pricingPolicyId;
                QMap<QDecNumber, QSharedPointer<PricingPolicyDetail>> _pricingPolicyDetails;

			public:
				int getAcceptCloseLmtVariation() const
				{
					return _acceptCloseLmtVariation;
				}

				int getAcceptDQVariation() const
				{
					return _acceptDQVariation;
				}

				int getAcceptIfDoneVariation() const
				{
					return _acceptIfDoneVariation;
				}

				int getAcceptLmtVariation() const
				{
					return _acceptLmtVariation;
				}

				int getAllowedNewTradeSides() const
				{
					return _allowedNewTradeSides;
				}

				QDateTime getBeginTime() const
				{
					return _beginTime;
				}

				bool getCanPlacePendingOrderAtAnyTime() const
				{
					return _canPlacePendingOrderAtAnyTime;
				}

				int getCancelLmtVariation() const
				{
					return _cancelLmtVariation;
				}

				InstrumentCategory::InstrumentCategory getCategory() const
				{
					return _category;
				}

				QString getCode() const
				{
					return _code;
				}

				int getCommissionFormula() const
				{
					return _commissionFormula;
				}

				QUuid getCurrencyId() const
				{
					return _currencyId;
				}

				QDecNumber getDQQuoteMinLot() const
				{
					return _dQQuoteMinLot;
				}

				QUuid getDeliverySpecificationId() const
				{
					return _deliverySpecificationId;
				}

				int getDeliveryTimeBeginDay() const
				{
					return _deliveryTimeBeginDay;
				}

				int getDeliveryTimeEndDay() const
				{
					return _deliveryTimeEndDay;
				}

				int getDenominator() const
				{
					return _denominator;
				}

				QString getDescription() const
				{
					return _description;
				}

				QDateTime getEndTime() const
				{
					return _endTime;
				}

				int getEnhancedPriceDigit() const
				{
					return _enhancedPriceDigit;
				}

				QUuid getId() const
				{
					return _id;
				}

				bool getIsActive() const
				{
					return _isActive;
				}

				bool getIsNormal() const
				{
					return _isNormal;
				}

				bool getIsSinglePrice() const
				{
					return _isSinglePrice;
				}

				int getLevyFormula() const
				{
					return _levyFormula;
				}

				int getMarginFormula() const
				{
					return _marginFormula;
				}

				QDecNumber getMaxDQLot() const
				{
					return _maxDQLot;
				}

				QDecNumber getMaxOtherLot() const
				{
					return _maxOtherLot;
				}

				QString getNarrative() const
				{
					return _narrative;
				}

				int getNumeratorUnit() const
				{
					return _numeratorUnit;
				}

				int getOrderTypeMask() const
				{
					return _orderTypeMask;
				}

				QString getOriginCode() const
				{
					return _originCode;
				}

				int getPhysicalLotDecimal() const
				{
					return _physicalLotDecimal;
				}

				int getPriceValidTime() const
				{
					return _priceValidTime;
				}

				QString getQuoteDescription() const
				{
					return _quoteDescription;
				}

				bool getShowPriceSuperscript() const
				{
					return _showPriceSuperscript;
				}

				int getTradePLFormula() const
				{
					return _tradePLFormula;
				}

				QString getUnit() const
				{
					return _unit;
				}

				QUuid getUnitId() const
				{
					return _unitId;
				}

				int getRearPrice() const
				{
					return _rearPrice;
				}
				
				int getCounterPartyId() const
				{
					return _counterPartyId;
				}

                QUuid getPricingPolicyId() const
                {
                    return _pricingPolicyId;
                }

				

				void setAcceptCloseLmtVariation(const int& value)
				{
					_acceptCloseLmtVariation = value;
				}

				void setAcceptDQVariation(const int& value)
				{
					_acceptDQVariation = value;
				}

				void setAcceptIfDoneVariation(const int& value)
				{
					_acceptIfDoneVariation = value;
				}

				void setAcceptLmtVariation(const int& value)
				{
					_acceptLmtVariation = value;
				}

				void setAllowedNewTradeSides(const int& value)
				{
					_allowedNewTradeSides = value;
				}

				void setBeginTime(const QDateTime& value)
				{
					_beginTime = value;
				}

				void setCanPlacePendingOrderAtAnyTime(const bool& value)
				{
					_canPlacePendingOrderAtAnyTime = value;
				}

				void setCancelLmtVariation(const int& value)
				{
					_cancelLmtVariation = value;
				}

				void setCategory(const InstrumentCategory::InstrumentCategory& value)
				{
					_category = value;
				}

				void setCode(const QString& value)
				{
					_code = value;
				}

				void setCommissionFormula(const int& value)
				{
					_commissionFormula = value;
				}

				void setCurrencyId(const QUuid& value)
				{
					_currencyId = value;
				}

				void setDQQuoteMinLot(const QDecNumber& value)
				{
					_dQQuoteMinLot = value;
				}

				void setDeliverySpecificationId(const QUuid& value)
				{
					_deliverySpecificationId = value;
				}

				void setDeliveryTimeBeginDay(const int& value)
				{
					_deliveryTimeBeginDay = value;
				}

				void setDeliveryTimeEndDay(const int& value)
				{
					_deliveryTimeEndDay = value;
				}

				void setDenominator(const int& value)
				{
					_denominator = value;
				}

				void setDescription(const QString& value)
				{
					_description = value;
				}

				void setEndTime(const QDateTime& value)
				{
					_endTime = value;
				}

				void setEnhancedPriceDigit(const int& value)
				{
					_enhancedPriceDigit = value;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setIsActive(const bool& value)
				{
					_isActive = value;
				}

				void setIsNormal(const bool& value)
				{
					_isNormal = value;
				}

				void setIsSinglePrice(const bool& value)
				{
					_isSinglePrice = value;
				}

				void setLevyFormula(const int& value)
				{
					_levyFormula = value;
				}

				void setMarginFormula(const int& value)
				{
					_marginFormula = value;
				}

				void setMaxDQLot(const QDecNumber& value)
				{
					_maxDQLot = value;
				}

				void setMaxOtherLot(const QDecNumber& value)
				{
					_maxOtherLot = value;
				}

				void setNarrative(const QString& value)
				{
					_narrative = value;
				}

				void setNumeratorUnit(const int& value)
				{
					_numeratorUnit = value;
				}

				void setOrderTypeMask(const int& value)
				{
					_orderTypeMask = value;
				}

				void setOriginCode(const QString& value)
				{
					_originCode = value;
				}

				void setPhysicalLotDecimal(const int& value)
				{
					_physicalLotDecimal = value;
				}

				void setPriceValidTime(const int& value)
				{
					_priceValidTime = value;
				}

				void setQuoteDescription(const QString& value)
				{
					_quoteDescription = value;
				}

				void setShowPriceSuperscript(const bool& value)
				{
					_showPriceSuperscript = value;
				}

				void setTradePLFormula(const int& value)
				{
					_tradePLFormula = value;
				}

				void setUnit(const QString& value)
				{
					_unit = value;
				}

				void setUnitId(const QUuid& value)
				{
					_unitId = value;
				}

				void setRearPrice(const int& value)
				{
					_rearPrice = value;
				}
				
				void setCounterPartyId(const int& value)
				{
					_counterPartyId = value;
				}
				
                void setPricingPolicyId(const QUuid& value)
                {
                    _pricingPolicyId = value;
                }



				public:
					void update(QXmlStreamReader& xmlStreamReader)
					{
						QXmlStreamAttributes attributes = xmlStreamReader.attributes();
						this->_acceptCloseLmtVariation = attributes.value("AcceptCloseLmtVariation").toInt();
						this->_acceptDQVariation = attributes.value("AcceptDQVariation").toInt();
						this->_acceptIfDoneVariation = attributes.value("AcceptIfDoneVariation").toInt();
						this->_acceptLmtVariation = attributes.value("AcceptLmtVariation").toInt();
						this->_allowedNewTradeSides = attributes.value("AllowedNewTradeSides").toInt();
						this->_beginTime = QDateTime::fromString(attributes.value("BeginTime").toString(), DateTimeConstants::DateTimeFormat);
						this->_canPlacePendingOrderAtAnyTime = Common::toBool(attributes.value("CanPlacePendingOrderAtAnyTime").toString());
						this->_cancelLmtVariation = attributes.value("CancelLmtVariation").toInt();
						this->_category = static_cast<InstrumentCategory::InstrumentCategory>(attributes.value("Category").toInt());
						this->_code = attributes.value("Code").toString();
						this->_commissionFormula = attributes.value("CommissionFormula").toInt();
						this->_currencyId = QUuid(attributes.value("CurrencyId").toString());
						this->_dQQuoteMinLot = QDecNumber::fromQString(attributes.value("DQQuoteMinLot").toString());
						this->_deliverySpecificationId = QUuid(attributes.value("DeliverySpecificationId").toString());
						this->_deliveryTimeBeginDay = attributes.value("DeliveryTimeBeginDay").toInt();
						this->_deliveryTimeEndDay = attributes.value("DeliveryTimeEndDay").toInt();
						this->_denominator = attributes.value("Denominator").toInt();
						this->_description = attributes.value("Description").toString();
						this->_endTime = QDateTime::fromString(attributes.value("EndTime").toString(), DateTimeConstants::DateTimeFormat);
						this->_enhancedPriceDigit = attributes.value("EnhancedPriceDigit").toInt();
						this->_id = QUuid(attributes.value("Id").toString());
						this->_isActive = Common::toBool(attributes.value("IsActive").toString());
						this->_isNormal = Common::toBool(attributes.value("IsNormal").toString());
						this->_isSinglePrice = Common::toBool(attributes.value("IsSinglePrice").toString());
						this->_levyFormula = attributes.value("LevyFormula").toInt();
						this->_marginFormula = attributes.value("MarginFormula").toInt();
						this->_maxDQLot = QDecNumber::fromQString(attributes.value("MaxDQLot").toString());
						this->_maxOtherLot = QDecNumber::fromQString(attributes.value("MaxOtherLot").toString());
						this->_narrative = attributes.value("Narrative").toString();
						this->_numeratorUnit = attributes.value("NumeratorUnit").toInt();
						this->_orderTypeMask = attributes.value("OrderTypeMask").toInt();
						this->_originCode = attributes.value("OriginCode").toString();
						this->_physicalLotDecimal = attributes.value("PhysicalLotDecimal").toInt();
						this->_priceValidTime = attributes.value("PriceValidTime").toInt();
						this->_quoteDescription = attributes.value("QuoteDescription").toString();
						this->_showPriceSuperscript = Common::toBool(attributes.value("ShowPriceSuperscript").toString());
						this->_tradePLFormula = attributes.value("TradePLFormula").toInt();
						this->_unit = attributes.value("Unit").toString();
						this->_unitId = QUuid(attributes.value("UnitId").toString());
						this->_rearPrice = attributes.value("RearPrice").toInt();
						this->_counterPartyId = attributes.value("CounterPartyId").toInt();
                        this->_pricingPolicyId = attributes.value("PricingPolicyId").toString() != "" ? QUuid(attributes.value("PricingPolicyId").toString()) : QUuid();
					}

					void update(const QDomNode& xmlNode)
					{
						QDomNamedNodeMap map = xmlNode.attributes();
						for (int i = 0; i < map.count(); i++)
						{
							QDomNode qDomNode = map.item(i);
							QString name = qDomNode.nodeName();
							QString value = qDomNode.nodeValue();
							if (name == "AcceptCloseLmtVariation")
								this->_acceptCloseLmtVariation = value.toInt();
							else if (name == "AcceptDQVariation")
								this->_acceptDQVariation = value.toInt();
							else if (name == "AcceptIfDoneVariation")
								this->_acceptIfDoneVariation = value.toInt();
							else if (name == "AcceptLmtVariation")
								this->_acceptLmtVariation = value.toInt();
							else if (name == "AllowedNewTradeSides")
								this->_allowedNewTradeSides = value.toInt();
							else if (name == "BeginTime")
								this->_beginTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
							else if (name == "CanPlacePendingOrderAtAnyTime")
								this->_canPlacePendingOrderAtAnyTime = Common::toBool(value);
							else if (name == "CancelLmtVariation")
								this->_cancelLmtVariation = value.toInt();
							else if (name == "Category")
								this->_category = static_cast<InstrumentCategory::InstrumentCategory>(value.toInt());
							else if (name == "Code")
								this->_code = value;
							else if (name == "CommissionFormula")
								this->_commissionFormula = value.toInt();
							else if (name == "CurrencyId")
								this->_currencyId = QUuid(value);
							else if (name == "DQQuoteMinLot")
								this->_dQQuoteMinLot = QDecNumber::fromQString(value);
							else if (name == "DeliverySpecificationId")
								this->_deliverySpecificationId = QUuid(value);
							else if (name == "DeliveryTimeBeginDay")
								this->_deliveryTimeBeginDay = value.toInt();
							else if (name == "DeliveryTimeEndDay")
								this->_deliveryTimeEndDay = value.toInt();
							else if (name == "Denominator")
								this->_denominator = value.toInt();
							else if (name == "Description")
								this->_description = value;
							else if (name == "EndTime")
								this->_endTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
							else if (name == "EnhancedPriceDigit")
								this->_enhancedPriceDigit = value.toInt();
							else if (name == "Id")
								this->_id = QUuid(value);
							else if (name == "IsActive")
								this->_isActive = Common::toBool(value);
							else if (name == "IsNormal")
								this->_isNormal = Common::toBool(value);
							else if (name == "IsSinglePrice")
								this->_isSinglePrice = Common::toBool(value);
							else if (name == "LevyFormula")
								this->_levyFormula = value.toInt();
							else if (name == "MarginFormula")
								this->_marginFormula = value.toInt();
							else if (name == "MaxDQLot")
								this->_maxDQLot = QDecNumber::fromQString(value);
							else if (name == "MaxOtherLot")
							{
								this->_maxOtherLot = QDecNumber::fromQString(value);
								//qDebug() << " setMaxOtherLot:" << _maxOtherLot.toDouble();
							}
							else if (name == "Narrative")
								this->_narrative = value;
							else if (name == "NumeratorUnit")
								this->_numeratorUnit = value.toInt();
							else if (name == "OrderTypeMask")
								this->_orderTypeMask = value.toInt();
							else if (name == "OriginCode")
								this->_originCode = value;
							else if (name == "PhysicalLotDecimal")
								this->_physicalLotDecimal = value.toInt();
							else if (name == "PriceValidTime")
								this->_priceValidTime = value.toInt();
							else if (name == "QuoteDescription")
								this->_quoteDescription = value;
							else if (name == "ShowPriceSuperscript")
								this->_showPriceSuperscript = Common::toBool(value);
							else if (name == "TradePLFormula")
								this->_tradePLFormula = value.toInt();
							else if (name == "Unit")
								this->_unit = value;
							else if (name == "UnitId")
								this->_unitId = QUuid(value);
							else if (name == "RearPrice")
								this->_rearPrice = value.toInt();
							else if (name == "CounterPartyId")
								this->_counterPartyId = value.toInt();
                            else if (name == "PricingPolicyId")
                                this->_pricingPolicyId = QUuid(value);
						}
					}

                    void clearPricingPolicyDetails()  {  _pricingPolicyDetails.clear(); }

                    void addPricingPolicyDetail(QDecNumber price, QSharedPointer<PricingPolicyDetail> pricingPolicyDetail)
                    {
                        this->_pricingPolicyDetails.insert(price, pricingPolicyDetail);
                    }

                    QSharedPointer<PricingPolicyDetail> getPricingPolicyDetail(QDecNumber marketPrice)
                    {
                        QSharedPointer<PricingPolicyDetail> maxPrcingPolicy(NULL);
                        QMapIterator<QDecNumber, QSharedPointer<PricingPolicyDetail>> item(this->_pricingPolicyDetails);
                        while (item.hasNext()) {
                          item.next();
                          maxPrcingPolicy = item.value();
                          //qDebug() << " ServerInstrument::getPricingPolicyDetail: price:"<< item.key().toDouble() << " marketPrice:" << marketPrice.toDouble();
                          if(item.key().toDouble() >= marketPrice.toDouble())
                          {
                              break;
                          }
                        }
                        return maxPrcingPolicy;
                    }

                    bool isExistPricingPolicy()
                    {
                        return this->_pricingPolicyDetails.count() > 0;
                    }
			};
		}
	}
}
#endif

