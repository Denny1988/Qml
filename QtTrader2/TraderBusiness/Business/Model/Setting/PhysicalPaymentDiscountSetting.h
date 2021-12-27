#ifndef BUSINESS_MODEL_SETTING_PHYSICALPAYMENTDISCOUNTSETTING_H
#define BUSINESS_MODEL_SETTING_PHYSICALPAYMENTDISCOUNTSETTING_H
#include <quuid.h>
#include "Common/Enum/PhysicalEnum.h"
#include "PhysicalPaymentDiscountSettingDetail.h"
#include <QMap>
#include <QSharedPointer>
#include <QDomNode>
#include "SettingDataBase.h"
#include "traderbusiness_global.h"

using namespace Common::Enum;

namespace Business
{
	namespace Model
	{
		namespace Setting
		{			
			class TRADERBUSINESS_EXPORT PhysicalPaymentDiscountSetting: public SettingDataBase
			{
			public:
				PhysicalPaymentDiscountSetting();
				~PhysicalPaymentDiscountSetting();
			private:
				QUuid  _id;
				QString  _code;
				AllowedPaymentForm::AllowedPaymentFormFlags _allowedPaymentForm;
				DiscountBasis::DiscountBasis _discountBasis;
				DiscountOption::DiscountOption _discountOption;
				QMap<QUuid, QSharedPointer<PhysicalPaymentDiscountSettingDetail>> _physicalPaymentDiscountSettingDetails;

			public:
				QSharedPointer<PhysicalPaymentDiscountSettingDetail> getOrAddPhysicalPaymentDiscountSettingDetail(QUuid id);
				QSharedPointer<PhysicalPaymentDiscountSettingDetail> getPhysicalPaymentDiscountSettingDetail(QUuid id);
				void addPhysicalPaymentDiscountSettingDetail(QUuid id, const QSharedPointer<PhysicalPaymentDiscountSettingDetail>& physicalPaymentDiscountSettingDetail);
				bool containsPhysicalPaymentDiscountSettingDetail(QUuid id);				
				void update(const QDomNode& xmlPhysicalPaymentDiscountSetting);
				QDecNumber calculateDiscount(double lot, QDecNumber marketValue, AllowedPaymentForm::AllowedPaymentForm paymentForm);
				void clearDetail();
                QSharedPointer<PhysicalPaymentDiscountSetting> copy();


			private:
				static bool caseInsensitiveLessThan(QSharedPointer<PhysicalPaymentDiscountSettingDetail> v1, QSharedPointer<PhysicalPaymentDiscountSettingDetail> v2);
				QSharedPointer<PhysicalPaymentDiscountSettingDetail> findLastDiscountDetail(QList<QSharedPointer<PhysicalPaymentDiscountSettingDetail>> details, QDecNumber lot);

			public:
				QUuid getId() const
				{
					return _id;
				}

				QString getCode() const
				{
					return _code;
				}
								
				AllowedPaymentForm::AllowedPaymentFormFlags getAllowedPaymentForm() const
				{
					return _allowedPaymentForm;
				}
								
				DiscountBasis::DiscountBasis getDiscountBasis() const
				{
					return _discountBasis;
				}

				DiscountOption::DiscountOption getDiscountOption() const
				{
					return _discountOption;
				}
				
				QList<QSharedPointer<PhysicalPaymentDiscountSettingDetail>> getPhysicalPaymentDiscountSettingDetails() const
				{
					return _physicalPaymentDiscountSettingDetails.values();
				}

				void setAllowedPaymentForm(const AllowedPaymentForm::AllowedPaymentFormFlags& value)
				{
					_allowedPaymentForm = value;
				}

				void setCode(const QString& value)
				{
					_code = value;
				}

				void setDiscountBasis(const DiscountBasis::DiscountBasis& value)
				{
					_discountBasis = value;
				}

				void setDiscountOption(const DiscountOption::DiscountOption& value)
				{
					_discountOption = value;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}





			};
		}
	}
}
#endif
