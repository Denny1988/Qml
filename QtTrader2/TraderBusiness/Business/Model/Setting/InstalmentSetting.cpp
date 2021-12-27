#include "InstalmentSetting.h"
#include "Common/Util/Convert_Common.h"
#include "Common/SignalForwarder.h"
#include <QDebug>

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			InstalmentSetting::InstalmentSetting()
			{
			}

			InstalmentSetting::~InstalmentSetting()
			{
			}

			QSharedPointer<InstalmentSettingDetail>& InstalmentSetting::getOrAddInstalmentSettingDetail(Frequence::Frequence frequence, int period)
			{
				if(this->_instalmentSettingDetails.contains(frequence))
				{
					auto instalmentSettingDetails = this->_instalmentSettingDetails[frequence];
					if(!instalmentSettingDetails.contains(period))
					{
						QSharedPointer<InstalmentSettingDetail> instalmentSettingDetail(new InstalmentSettingDetail);
						this->_instalmentSettingDetails[frequence].insert(period, instalmentSettingDetail);
					}
					return this->_instalmentSettingDetails[frequence][period];
				}
				else
				{
					QMap<int,QSharedPointer<InstalmentSettingDetail>> instalmentSettingDetails;
					QSharedPointer<InstalmentSettingDetail> instalmentSettingDetail(new InstalmentSettingDetail);
					instalmentSettingDetails.insert(period, instalmentSettingDetail);
					this->_instalmentSettingDetails.insert(frequence, instalmentSettingDetails);
					return this->_instalmentSettingDetails[frequence][period];
				}				
			}

			QSharedPointer<InstalmentSettingDetail>& InstalmentSetting::getOrAddAdvancePaymentInstalmentSettingDetail()
			{
				if (this->_advancePaymentInstalmentSettingDetail.isNull())
				{
					QSharedPointer<Model::Setting::InstalmentSettingDetail> instalmentSettingDetail(new Model::Setting::InstalmentSettingDetail);
					this->_advancePaymentInstalmentSettingDetail = instalmentSettingDetail;
				}
				return this->_advancePaymentInstalmentSettingDetail;
			}

			QSharedPointer<InstalmentSettingDetail> InstalmentSetting::getInstalmentSettingDetail(Frequence::Frequence frequence, int period)
			{
				if(this->_instalmentSettingDetails.count() > 0 && this->_instalmentSettingDetails.contains(frequence))
				{
					auto instalmentSettingDetails = this->_instalmentSettingDetails[frequence];
					if(instalmentSettingDetails.contains(period))
                    {
                        return instalmentSettingDetails[period];;
					}
				}
				if(frequence == Frequence::Occasional && period == 1)
					return _advancePaymentInstalmentSettingDetail;
				return QSharedPointer<InstalmentSettingDetail>(Q_NULLPTR);
			}

			bool InstalmentSetting::hasInstalmentSettingOrAdvancePaymentInstalment(bool isAdvancePayment)
			{
				if (isAdvancePayment)
				{
					return !this->_advancePaymentInstalmentSettingDetail.isNull();
				}
				else
				{
					if (this->_instalmentSettingDetails.count() == 0) return false;

					foreach (auto parentItem, this->_instalmentSettingDetails.values())
					{
						foreach (auto item, parentItem)
						{
							if (item->getIsActive())
								return true;
						}
					}
					return false;
				}
			}

			void InstalmentSetting::update(const QDomNode& xmlInstalmentSetting)
			{
				QDomNamedNodeMap map = xmlInstalmentSetting.attributes();
				bool isUpdateItem = false;
				for (int i = 0; i < map.count(); i++)
				{
					QDomNode qDomNode = map.item(i);
					QString name = qDomNode.nodeName();
					QString value = qDomNode.nodeValue();
					if(name == "Id")
						this->_id = QUuid(value);
					else if(name == "Code")
						this->_code = value;
					else if(name == "AllowEqualPrincipal")
						this->_allowEqualPrincipal= Common::toBool(value);
					else if(name == "AllowEqualInstalment")
						this->_allowEqualInstalment= Common::toBool(value);
					else if(name == "AllowRecalucateNextMonth")
						this->_allowRecalucateNextMonth= Common::toBool(value);
					else if(name == "AllowRecalucateNextYear")
						this->_allowRecalucateNextYear= Common::toBool(value);
					else if(name == "AllowClose")
					{
						this->_allowClose= static_cast<Common::Enum::AllowCloseInstalment::AllowCloseInstalment>(value.toInt());
						Common::SignalForwarder::getInstance()->sendRefreshPhysicalButtonIsEnable();
					}
					else if(name == "AdvancePayment")
					{
						this->_advancePayment= static_cast<Common::Enum::AdvancePaymentOption::AdvancePaymentOption>(value.toInt());
						Common::SignalForwarder::getInstance()->sendRefreshPhysicalButtonIsEnable();
					}
					else if(name == "IsDownPayAsFirstPay")
						this->_isDownPayAsFirstPay= Common::toBool(value);
					else if(name == "IsUpdateItem")
						isUpdateItem = Common::toBool(value);
				}

				if(isUpdateItem)
				{
					this->_instalmentSettingDetails.clear();
				}

				QDomNodeList childlist = xmlInstalmentSetting.childNodes();  
				for (int i = 0; i < childlist.count(); i++)
				{
					QDomNode itemQDomNode = childlist.item(i);
					QString name = itemQDomNode.nodeName();
					if(name == "InstalmentSettingDetail")
					{
						QString detailKey = itemQDomNode.toElement().attribute("DetailKey");
						Common::Enum::Frequence::Frequence frequence;
						int period;
						if(detailKey.isEmpty() || detailKey.isNull())
						{
							frequence =  static_cast<Common::Enum::Frequence::Frequence>(itemQDomNode.toElement().attribute("Frequence").toInt());
							period =  itemQDomNode.toElement().attribute("Period").toInt();
						}
						else
						{
							int separation = detailKey.indexOf("_", 0);
							QString l_Frequence = detailKey.left(separation);
							QString r_Period = detailKey.right(detailKey.length() - separation -1);
							frequence =  static_cast<Common::Enum::Frequence::Frequence>(l_Frequence.toInt());
							period =  r_Period.toInt();
						}

						if(frequence == Common::Enum::Frequence::Occasional)
						{
							if(this->_advancePaymentInstalmentSettingDetail.isNull())
							{
								QSharedPointer<Model::Setting::InstalmentSettingDetail> instalmentSettingDetail(new Model::Setting::InstalmentSettingDetail);
								instalmentSettingDetail->update(itemQDomNode);
								this->_advancePaymentInstalmentSettingDetail = instalmentSettingDetail;
							}
							else
								this->_advancePaymentInstalmentSettingDetail->update(itemQDomNode);
							this->_advancePaymentInstalmentSettingDetail->setInstalmentPolicyId(this->_id);
						}
						else
						{
							QSharedPointer<Model::Setting::InstalmentSettingDetail> instalmentSettingDetail = this->getOrAddInstalmentSettingDetail(frequence, period);
							instalmentSettingDetail->update(itemQDomNode);
							instalmentSettingDetail->setInstalmentPolicyId(this->_id);
						}

					}
				}
			}
			void Business::Model::Setting::InstalmentSetting::clearDetail()
			{
				_instalmentSettingDetails.clear();
				_advancePaymentInstalmentSettingDetail.clear();
			}

            QSharedPointer<InstalmentSetting> InstalmentSetting::copy()
            {
                QSharedPointer<InstalmentSetting> newObject(new InstalmentSetting);
                newObject->_id = _id;
                newObject->_code = _code;
                newObject->_allowEqualPrincipal = _allowEqualPrincipal;
                newObject->_allowEqualInstalment = _allowEqualInstalment;
                newObject->_allowRecalucateNextMonth = _allowRecalucateNextMonth;
                newObject->_allowRecalucateNextYear = _allowRecalucateNextYear;
                newObject->_allowClose = _allowClose;
                newObject->_advancePayment = _advancePayment;
                newObject->_isDownPayAsFirstPay = _isDownPayAsFirstPay;

                if(this->_instalmentSettingDetails.count() > 0)
                {
                    QMap<Frequence::Frequence, QMap<int,QSharedPointer<InstalmentSettingDetail>>> copyInstalmentSettingDetails;
                    QMapIterator<Frequence::Frequence, QMap<int,QSharedPointer<InstalmentSettingDetail>>> item(this->_instalmentSettingDetails);
                    while (item.hasNext()) {
                        item.next();
                        QMap<int,QSharedPointer<InstalmentSettingDetail>> copyChildInstalmentSettingDetails;
                        QMapIterator<int,QSharedPointer<InstalmentSettingDetail>> childItem(item.value());
                        while (childItem.hasNext()) {
                            childItem.next();
                            copyChildInstalmentSettingDetails.insert(childItem.key(), childItem.value()->copy());
                        }
                        copyInstalmentSettingDetails.insert(item.key(), copyChildInstalmentSettingDetails);
                    }
                    newObject->_instalmentSettingDetails = copyInstalmentSettingDetails;
                }
                if(!this->_advancePaymentInstalmentSettingDetail.isNull())
                {
                        newObject->_advancePaymentInstalmentSettingDetail = this->_advancePaymentInstalmentSettingDetail->copy();
                }
                return newObject;
            }

		}
	}
}
