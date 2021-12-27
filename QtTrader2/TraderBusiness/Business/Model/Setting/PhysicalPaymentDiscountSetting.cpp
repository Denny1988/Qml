#include "PhysicalPaymentDiscountSetting.h"
#include "Common/LogHelper.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{			
			PhysicalPaymentDiscountSetting::PhysicalPaymentDiscountSetting()
			{
			}

			PhysicalPaymentDiscountSetting::~PhysicalPaymentDiscountSetting()
			{
			}

			QSharedPointer<PhysicalPaymentDiscountSettingDetail> PhysicalPaymentDiscountSetting::getOrAddPhysicalPaymentDiscountSettingDetail(QUuid id)
			{
				if(this->_physicalPaymentDiscountSettingDetails.contains(id))
				{
					return this->_physicalPaymentDiscountSettingDetails.value(id);
				}
				QSharedPointer<PhysicalPaymentDiscountSettingDetail> physicalPaymentDiscountSettingDetail(new PhysicalPaymentDiscountSettingDetail);
				this->_physicalPaymentDiscountSettingDetails.insert(id, physicalPaymentDiscountSettingDetail);
				return physicalPaymentDiscountSettingDetail;
			}

			QSharedPointer<PhysicalPaymentDiscountSettingDetail> PhysicalPaymentDiscountSetting::getPhysicalPaymentDiscountSettingDetail(QUuid id)
			{
				return this->_physicalPaymentDiscountSettingDetails.value(id);
			}

			void PhysicalPaymentDiscountSetting::addPhysicalPaymentDiscountSettingDetail(QUuid id, const QSharedPointer<PhysicalPaymentDiscountSettingDetail>& physicalPaymentDiscountSettingDetail)
			{
				if(!this->_physicalPaymentDiscountSettingDetails.contains(id))
				{
					this->_physicalPaymentDiscountSettingDetails.insert(id, physicalPaymentDiscountSettingDetail);
				}
			}

			bool PhysicalPaymentDiscountSetting::containsPhysicalPaymentDiscountSettingDetail(QUuid id)
			{
				return this->_physicalPaymentDiscountSettingDetails.contains(id);
			}


			void PhysicalPaymentDiscountSetting::update(const QDomNode& xmlPhysicalPaymentDiscountSetting)
			{
				QDomNamedNodeMap map = xmlPhysicalPaymentDiscountSetting.attributes();
				for (int i = 0; i < map.count(); i++)
				{
					QDomNode qDomNode = map.item(i);
					QString name = qDomNode.nodeName();
					QString value = qDomNode.nodeValue();
					if(name == "Id")
						this->_id = QUuid(value);
					else if(name == "Code")
						this->_code = value;
					else if(name == "AllowedPaymentForm")
						this->_allowedPaymentForm= static_cast<AllowedPaymentForm::AllowedPaymentFormFlags>(value.toInt());
					else if(name == "DiscountBasis")
						this->_discountBasis= static_cast<DiscountBasis::DiscountBasis>(value.toInt());
					else if(name == "DiscountOption")
						this->_discountOption= static_cast<DiscountOption::DiscountOption>(value.toInt());
				}

				QDomNodeList childlist = xmlPhysicalPaymentDiscountSetting.childNodes();  
				for (int i = 0; i < childlist.count(); i++)
				{
					QDomNode itemQDomNode = childlist.item(i);
					QString name = itemQDomNode.nodeName();
					if(name == "PhysicalPaymentDiscountSettingDetail")
					{
						QUuid id =  QUuid(itemQDomNode.toElement().attribute("Id"));
						QSharedPointer<PhysicalPaymentDiscountSettingDetail> physicalPaymentDiscountSettingDetail = this->getOrAddPhysicalPaymentDiscountSettingDetail(id);
						physicalPaymentDiscountSettingDetail->update(itemQDomNode);
					}
				}
			}

			QDecNumber PhysicalPaymentDiscountSetting::calculateDiscount(double lot, QDecNumber marketValue, AllowedPaymentForm::AllowedPaymentForm paymentForm)
			{
				if ((paymentForm & this->_allowedPaymentForm) != paymentForm || this->_physicalPaymentDiscountSettingDetails.count() == 0) return 0;
				QList<QSharedPointer<PhysicalPaymentDiscountSettingDetail>> details = this->_physicalPaymentDiscountSettingDetails.values();
				qSort(details.begin(), details.end(), caseInsensitiveLessThan);
				QDecNumber decimalLot = QDecNumber(lot);
				if (this->_discountOption == DiscountOption::Flat)
				{
					QSharedPointer<PhysicalPaymentDiscountSettingDetail> detail = this->findLastDiscountDetail(details, lot);
					QDecNumber discount = 0;
					if (!detail.isNull())
					{
						discount = this->_discountBasis == DiscountBasis::UnitAmount ? decimalLot * detail->getDiscountValue() : marketValue * detail->getDiscountValue();
					}
					return discount;
				}
				else if (this->_discountOption == DiscountOption::Progressive)
				{
					QDecNumber discount = 0;
					int index = 0;
					while (index < details.count() && decimalLot > details[index]->getFrom())
					{
						QSharedPointer<PhysicalPaymentDiscountSettingDetail> detail = details[index];
						QDecNumber calculateLot = decimalLot - detail->getFrom();
						if (index < details.count() - 1)
						{
							QDecNumber range = (details[index + 1]->getFrom() - detail->getFrom());
							calculateLot = qMin(calculateLot, range);
						}
						QDecNumber calculateValue = (marketValue * calculateLot) / lot;
						discount += this->_discountBasis == DiscountBasis::UnitAmount ? calculateLot * detail->getDiscountValue() : calculateValue * detail->getDiscountValue();
						index++;
					}

					return discount;
				}
				else
				{
					QLOG_INFO(Common::Constants::LogKeyConstants::Business) << QString("PhysicalPaymentDiscountSetting::calculateDiscount Option = %1 is not supported").arg((int)this->_discountOption);
					return 0;
				}
			}

			void PhysicalPaymentDiscountSetting::clearDetail()
			{
				this->_physicalPaymentDiscountSettingDetails.clear();
			}

            QSharedPointer<PhysicalPaymentDiscountSetting> PhysicalPaymentDiscountSetting::copy()
            {
                QSharedPointer<PhysicalPaymentDiscountSetting> newObject(new PhysicalPaymentDiscountSetting);
                newObject->_id = _id;
                newObject->_code = _code;
                newObject->_allowedPaymentForm = _allowedPaymentForm;
                newObject->_discountBasis = _discountBasis;
                newObject->_discountOption = _discountOption;

                QMap<QUuid, QSharedPointer<PhysicalPaymentDiscountSettingDetail>> copyPhysicalPaymentDiscountSettingDetails;
                QMapIterator<QUuid, QSharedPointer<PhysicalPaymentDiscountSettingDetail>> item(this->_physicalPaymentDiscountSettingDetails);
                while (item.hasNext()) {
                    item.next();
                    copyPhysicalPaymentDiscountSettingDetails.insert(item.key(), item.value()->copy());
                }
                newObject->_physicalPaymentDiscountSettingDetails = copyPhysicalPaymentDiscountSettingDetails;
                return newObject;
            }


			////////--------------private----------------------/////
			bool PhysicalPaymentDiscountSetting::caseInsensitiveLessThan(QSharedPointer<PhysicalPaymentDiscountSettingDetail> v1, QSharedPointer<PhysicalPaymentDiscountSettingDetail> v2)
			{
				return v1->getFrom() < v2->getFrom();
			}

			QSharedPointer<PhysicalPaymentDiscountSettingDetail> PhysicalPaymentDiscountSetting::findLastDiscountDetail(QList<QSharedPointer<PhysicalPaymentDiscountSettingDetail>> details, QDecNumber lot)
			{
				for (int i = details.count() - 1; i >= 0; i--)
				{
					if (details[i]->getFrom() < lot) return details[i];
				}
				return QSharedPointer<PhysicalPaymentDiscountSettingDetail>(Q_NULLPTR);
			}

		}
	}
}
