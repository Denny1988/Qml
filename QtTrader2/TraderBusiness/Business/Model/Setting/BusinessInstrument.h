#ifndef BUSINESS_MODEL_SETTING_BUSINESSINSTRUMENT_H
#define BUSINESS_MODEL_SETTING_BUSINESSINSTRUMENT_H
#include <quuid.h>
#include <qstring.h>
#include <qdatetime.h>
#include "Common/Enum/InstrumentEnum.h"
#include "InterestRate.h"
#include "DeliveryCharge.h"
#include "LotSetting.h"
#include "MovePipsInfoSetting.h"
#include "VolumeNecessary.h"
#include "CalculateSetting.h"
#include "PlaceSetting.h"
#include "InstalmentSetting.h"
#include "BOPolicySetting.h"
#include "PhysicalPaymentDiscountSetting.h"
#include "DeliverySpecificationDetail.h"
#include "PricingPolicyDetail.h"
#include <QList>
#include "SettingDataBase.h"
#include <QDomNode>
#include <QReadWriteLock>
#include "traderbusiness_global.h"
using namespace Common::Enum;

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class TRADERBUSINESS_EXPORT BusinessInstrument: public SettingDataBase
			{
			public:
				BusinessInstrument();
				~BusinessInstrument();
			private:
				QUuid  _id;
				QString  _originCode;
				QString  _code;
				bool  _isSinglePrice;
				QString  _instrumentDescription;	
				QString  _narrative;
				QString  _quoteDescription;
				QUuid  _unitId;
				QString  _unit;
				QUuid  _currencyId;
				int  _placeConfirmMinTime;
				int  _maxAlertPricePerInstrument;
				int  _exceptionEnquiryOutTime;
				int  _enquiryOutTime;
				QUuid  _deliveryPointGroupId;
				InstrumentCategory::InstrumentCategory _instrumentCategory;
				int _spreadPoints;
				int _autoAdjustPoints;
				bool _showPriceSuperscript;
				int _enhancedPriceDigit;
				bool _isNormal;
				int _physicalLotDecimal;
				int _weight;
				bool _expired;
				int _deliveryTimeBeginDay;
				int _deliveryTimeEndDay;
				int _buyInterestValueDay;
				int _sellInterestValueDay;
				QDateTime _beginTime;
				QDateTime _endTime;
				PriceType::PriceType _priceType;
                int _deliveryMode;
                QUuid _administrativeChargeId;
                QDecNumber _administrativeChargeConvertRate;


				QSharedPointer<InterestRate> _interestRateSetting;
				QSharedPointer<InterestRate> _prepaymentInterestRateSetting;
				QSharedPointer<DeliveryCharge> _deliveryChargeSetting;			
				QSharedPointer<PlaceSetting> _placeSetting;
				QSharedPointer<CalculateSetting> _calculateSetting;			
				QSharedPointer<MovePipsInfoSetting> _movePipsInfoSetting;			
				QSharedPointer<VolumeNecessary> _volumeNecessarySetting;
				QSharedPointer<InstalmentSetting> _instalmentSettingSetting;
				QSharedPointer<PhysicalPaymentDiscountSetting> _physicalPaymentDiscountSetting;
				QSharedPointer<BOPolicySetting> _BOPolicySetting;
				QMap<QString, QSharedPointer<LotSetting>> _lotSettings;
				QMap<QString, QSharedPointer<DeliverySpecificationDetail>> _deliverySpecificationDetails;
                QMap<QDecNumber, QDecNumber> _administrativeChargeDetails;                
                QMap<QDecNumber, QSharedPointer<PricingPolicyDetail>> _pricingPolicyDetails;

			public:
				QSharedPointer<LotSetting>& getLotSetting(bool isOpen, Common::Enum::OrderType::OrderType orderType);
				QSharedPointer<DeliverySpecificationDetail>& getOrAddDeliverySpecificationDetail(QString size);
				void update(const QDomNode& xmlLeverageSetting);
				QSharedPointer<LotSetting>& getOrAddLotSetting(bool isOpen, Common::Enum::OrderType::OrderType orderType);
				QSharedPointer<LotSetting>& getOrAddLotSetting(const QString& key);
				bool allowPlaceBO();
				QSharedPointer<InterestRate>& getOrAddInterestRateSetting();
				QSharedPointer<InterestRate>& getOrAddPrepaymentInterestRateSetting();
				QSharedPointer<DeliveryCharge>& getOrAddDeliveryChargeSetting();
				QSharedPointer<PlaceSetting>& getOrAddPlaceSetting();
				QSharedPointer<CalculateSetting>& getOrAddCalculateSetting();
				QSharedPointer<MovePipsInfoSetting>& getOrAddMovePipsInfoSetting();
				QSharedPointer<VolumeNecessary>& getOrAddVolumeNecessarySetting();
				QSharedPointer<InstalmentSetting>& getOrAddInstalmentSettingSetting();
				QSharedPointer<PhysicalPaymentDiscountSetting>& getOrAddPhysicalPaymentDiscountSetting();
				QSharedPointer<BOPolicySetting>& getOrAddBOPolicySetting();
                void clearDeliverySpecificationDetails() { _deliverySpecificationDetails.clear(); }
                void addOrUpdateAdministrativeChargeDetail(QDecNumber quantity, QDecNumber chargeRate);
                QMap<QDecNumber, QDecNumber> getAdministrativeChargeDetails();
                void clearPricingPolicyDetails()  {  _pricingPolicyDetails.clear(); }
                void addPricingPolicyDetail(QDecNumber price, QSharedPointer<PricingPolicyDetail> pricingPolicyDetail);
                QSharedPointer<PricingPolicyDetail> getPricingPolicyDetail(QDecNumber marketPrice);
                bool isExistPricingPolicy();
                QSharedPointer<BusinessInstrument> copy();

			public:
				
				QUuid getId() const
				{
					return _id;
				}

				QString getOriginCode() const
				{
					return _originCode;
				}

				QString getCode() const
				{
					return _code;
				}

				bool getIsSinglePrice() const
				{
					return _isSinglePrice;
				}

				QString getDescription() const
				{
					return _instrumentDescription;
				}

				QString getNarrative() const
				{
					return _narrative;
				}

				QString getQuoteDescription() const
				{
					return _quoteDescription;
				}

				QUuid getUnitId() const
				{
					return _unitId;
				}

				QString getUnit() const
				{
					return _unit;
				}
				

				QUuid getCurrencyId() const
				{
					return _currencyId;
				}

				int getPlaceConfirmMinTime() const
				{
					return _placeConfirmMinTime;
				}

				int getMaxAlertPricePerInstrument() const
				{
					return _maxAlertPricePerInstrument;
				}

				int getExceptionEnquiryOutTime() const
				{
					return _exceptionEnquiryOutTime;
				}

				int getEnquiryOutTime() const
				{
					return _enquiryOutTime;
				}

				QUuid getDeliveryPointGroupId() const
				{
					return _deliveryPointGroupId;
				}

				InstrumentCategory::InstrumentCategory getInstrumentCategory() const
				{
					return _instrumentCategory;
				}
						
				int getSpreadPoints() const
				{
					return _spreadPoints;
				}

				int getAutoAdjustPoints() const
				{
					return _autoAdjustPoints;
				}

				bool getShowPriceSuperscript() const
				{
					return _showPriceSuperscript;
				}

				int getEnhancedPriceDigit() const
				{
					return _enhancedPriceDigit;
				}
								
				bool getIsNormal() const
				{
					return _isNormal;
				}

				int getPhysicalLotDecimal() const
				{
					return _physicalLotDecimal;
				}		

				int getWeight() const
				{
					return _weight;
				}		
								
				bool getExpired() const
				{
					return _expired;
				}	
				
				PriceType::PriceType getPriceType() const
				{
					return _priceType;
				}
						
				int getDeliveryTimeBeginDay()
				{
					return _deliveryTimeBeginDay;
				}

				int getDeliveryTimeEndDay()
				{
					return _deliveryTimeEndDay;
				}

				int getBuyInterestValueDay()
				{
					return _buyInterestValueDay;
				}

				int getSellInterestValueDay()
				{
					return _sellInterestValueDay;
				}

				QDateTime getBeginTime()
				{
					return _beginTime;
				}

				QDateTime getEndTime()
				{
					return _endTime;
				}

                int getDeliveryMode()
                {
                    return _deliveryMode;
                }

                QUuid getAdministrativeChargeId()
                {
                    return _administrativeChargeId;
                }

                QDecNumber getAdministrativeChargeConvertRate()
                {
                    return _administrativeChargeConvertRate;
                }



				
				QSharedPointer<InterestRate>& getInterestRateSetting() 
				{
					return _interestRateSetting;
				}

				QSharedPointer<InterestRate>& getPrepaymentInterestRateSetting()
				{
					return _prepaymentInterestRateSetting;
				}

				QSharedPointer<DeliveryCharge>& getDeliveryCharge()
				{
					return _deliveryChargeSetting;
				}

				QSharedPointer<PlaceSetting>& getPlaceSetting()
				{
					return _placeSetting;
				}

				QSharedPointer<CalculateSetting>& getCalculateSetting()
				{
					return _calculateSetting;
				}

				QSharedPointer<MovePipsInfoSetting>& getMovePipsInfoSetting()
				{
					return _movePipsInfoSetting;
				}

				QSharedPointer<VolumeNecessary>& getVolumeNecessary()
				{
					return _volumeNecessarySetting;
				}

				bool VolumeNecessaryIsNull()
				{
					return _volumeNecessarySetting.isNull();
				}

				QSharedPointer<InstalmentSetting>& getInstalmentSetting()
				{
					return _instalmentSettingSetting;
				}

				QSharedPointer<PhysicalPaymentDiscountSetting>& getPhysicalPaymentDiscountSetting()
				{
					return _physicalPaymentDiscountSetting;
				}
				
				QList<QSharedPointer<DeliverySpecificationDetail>>  getDeliverySpecificationDetails()
				{
					return _deliverySpecificationDetails.values();
				}
				
				QSharedPointer<BOPolicySetting>& getBOPolicySetting()
				{
					return _BOPolicySetting;
				}

				bool isExistBOPolicySetting()
				{
					return !_BOPolicySetting.isNull() && _BOPolicySetting->getBOPolicyDetailSettingsCount() > 0;
				}
				
				bool isExistBOPolicyDetailSetting(QString frequencyKey)
				{
					return !_BOPolicySetting.isNull() && _BOPolicySetting->isExistBOPolicyDetailSetting(frequencyKey);
				}




				void setAutoAdjustPoints(const int& value)
				{
					_autoAdjustPoints = value;
				}

				void setBeginTime(const QDateTime& value)
				{
					_beginTime = value;
				}

				void setBuyInterestValueDay(const int& value)
				{
					_buyInterestValueDay = value;
				}

				void setCode(const QString& value)
				{
					_code = value;
				}

				void setCurrencyId(const QUuid& value)
				{
					_currencyId = value;
				}

				void setDeliveryPointGroupId(const QUuid& value)
				{
					_deliveryPointGroupId = value;
				}

				void setDeliveryTimeBeginDay(const int& value)
				{
					_deliveryTimeBeginDay = value;
				}

				void setDeliveryTimeEndDay(const int& value)
				{
					_deliveryTimeEndDay = value;
				}

				void setEndTime(const QDateTime& value)
				{
					_endTime = value;
				}

				void setEnhancedPriceDigit(const int& value)
				{
					_enhancedPriceDigit = value;
				}

				void setEnquiryOutTime(const int& value)
				{
					_enquiryOutTime = value;
				}

				void setExceptionEnquiryOutTime(const int& value)
				{
					_exceptionEnquiryOutTime = value;
				}

				void setExpired(const bool& value)
				{
					_expired = value;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setInstrumentCategory(const InstrumentCategory::InstrumentCategory& value)
				{
					_instrumentCategory = value;
				}

				void setInstrumentDescription(const QString& value)
				{
					_instrumentDescription = value;
				}

				void setIsNormal(const bool& value)
				{
					_isNormal = value;
				}

				void setIsSinglePrice(const bool& value)
				{
					_isSinglePrice = value;
				}

				void setMaxAlertPricePerInstrument(const int& value)
				{
					_maxAlertPricePerInstrument = value;
				}

				void setNarrative(const QString& value)
				{
					_narrative = value;
				}

				void setOriginCode(const QString& value)
				{
					_originCode = value;
				}

				void setPhysicalLotDecimal(const int& value)
				{
					_physicalLotDecimal = value;
				}

				void setPlaceConfirmMinTime(const int& value)
				{
					_placeConfirmMinTime = value;
				}

				void setPriceType(const PriceType::PriceType& value)
				{
					_priceType = value;
				}

				void setQuoteDescription(const QString& value)
				{
					_quoteDescription = value;
				}

				void setSellInterestValueDay(const int& value)
				{
					_sellInterestValueDay = value;
				}

				void setShowPriceSuperscript(const bool& value)
				{
					_showPriceSuperscript = value;
				}

				void setSpreadPoints(const int& value)
				{
					_spreadPoints = value;
				}

				void setUnit(const QString& value)
				{
					_unit = value;
				}

				void setUnitId(const QUuid& value)
				{
					_unitId = value;
				}

				void setWeight(const int& value)
				{
					_weight = value;
				}

                void setDeliveryMode(const int& value)
                {
                    _deliveryMode = value;
                }

                void setAdministrativeChargeId(const QUuid& value)
                {
                    _administrativeChargeId = value;
                }

                void setAdministrativeChargeConvertRate(const QDecNumber& value)
                {
                    _administrativeChargeConvertRate = value;
                }


			};
		}
	}
}
#endif
