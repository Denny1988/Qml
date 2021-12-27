#include "ServerSourceManager.h"
#include "SettingManager.h"
#include "InitializeManager.h"
#include <qDebug>
#include "Common/SignalForwarder.h"
#include "Common/GuidMappingManager.h"
#include "Common/Util/Convert_Common.h"
#include "Common/Enum/PlaceEnum.h"
#include "Common/Enum/PhysicalEnum.h"
#include "Business/Model/Setting/InterestRate.h"
#include "Business/Model/Setting/CalculateQuotationDefaultSetting.h"
#include "Common/TraderBaseInfo.h"
#include "Common/LogHelper.h"

namespace Business
{
	ServerSourceManager::ServerSourceManager(QSharedPointer<SettingManager>& settingManager, QSharedPointer<InitializeManager>& initializeManager)
	{
		this->_initializeManager = initializeManager;
		this->_settingManager = settingManager;
	}

	//---------------------------------------public method--------------------------------------------//
	bool ServerSourceManager::initSettingSourceData(const QByteArray&  dataByteArray)
	{
		bool isSuessce = false;
		QXmlStreamReader xmlStreamReader;
		xmlStreamReader.addData(dataByteArray);

		xmlStreamReader.readNext();
		while (!xmlStreamReader.atEnd()) {
			if (xmlStreamReader.isStartElement()) {
				if (xmlStreamReader.name() == "SettingSource") {
					xmlStreamReader.readNext();
					while (!xmlStreamReader.atEnd()) {
						if (xmlStreamReader.isEndElement()) {
							xmlStreamReader.readNext();// SettingSource end
							break;
						}
						if (xmlStreamReader.isStartElement()) {
							QString name = xmlStreamReader.name().toString();
                            QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  QString("ServerSourceManager::initSettingSourceData name:%1").arg(name);
							if (name == "Customer") {
								if (this->_customer.isNull())
								{
									QSharedPointer<Server::Customer> newCustomer(new Server::Customer);
									this->_customer = newCustomer;
								}
								this->_customer->update(xmlStreamReader);
								xmlStreamReader.readNext();// Customer end
								xmlStreamReader.readNext();
							}
							else if (name == "SystemParameter")
							{
								if (this->_systemParameter.isNull())
								{
									QSharedPointer<Server::SystemParameter> newSystemParameter(new Server::SystemParameter);
									this->_systemParameter = newSystemParameter;
								}
								this->_systemParameter->update(xmlStreamReader);
								xmlStreamReader.readNext();// SystemParameter end
								xmlStreamReader.readNext();
							}
							else if (name == "InstrumentUnits")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "InstrumentUnit");
							}
							else if (name == "DealingPolicyDetails")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "DealingPolicyDetail");
							}
							else if (name == "Instruments")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "Instrument");
							}
							else if (name == "Accounts")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "Account");
							}
							else if (name == "TradePolicyDetails")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "TradePolicyDetail");
							}
							else if (name == "QuotePolicies")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "QuotePolicy");
							}
							else if (name == "QuotePolicyDetails")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "QuotePolicyDetail");
							}
							else if (name == "InstalmentPolicies")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "InstalmentPolicy");
							}
							else if (name == "InstalmentPolicyDetails")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "InstalmentPolicyDetail");
							}
							else if (name == "DeliveryCharges")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "DeliveryCharge");
							}
							else if (name == "VolumeNecessaries")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "VolumeNecessary");
							}
							else if (name == "VolumeNecessaryDetails")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "VolumeNecessaryDetail");
							}
							else if (name == "DeliverySpecificationDetails")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "DeliverySpecificationDetail");
							}
							else if (name == "PhysicalPaymentDiscounts")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "PhysicalPaymentDiscount");
							}
							else if (name == "PhysicalPaymentDiscountDetails")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "PhysicalPaymentDiscountDetail");
							}
							else if (name == "BOPolicies")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "BOPolicy");
							}
							else if (name == "BOPolicyDetails")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "BOPolicyDetail");
							}
							else if (name == "BOBetTypes")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "BOBetType");
							}
                            else if (name == "AdministrativeCharges")
                            {
                                xmlStreamReader.readNext();
                                this->readDoubleStructureElement(xmlStreamReader, "AdministrativeCharge");
                            }
                            else if (name == "AdministrativeChargeDetails")
                            {
                                xmlStreamReader.readNext();
                                this->readDoubleStructureElement(xmlStreamReader, "AdministrativeChargeDetail");
                            }
                            else if (name == "Organizations")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "Organization");
							}
							else if (name == "CounterParties")
							{
								xmlStreamReader.readNext();
								this->readDoubleStructureElement(xmlStreamReader, "CounterParty");
							}
							else if (name == "InstrumentQuotePolicyMappings")
							{
                                xmlStreamReader.readNext();
								this->setInstrumentQuotePolicyMappings(xmlStreamReader);
							}
							else if (name == "AccountForMarketingGroups")
							{
                                xmlStreamReader.readNext();
								this->setAccountForMarketingGroups(xmlStreamReader);
							}
                            else if (name == "PricingPolicyDetails")
                            {
                                xmlStreamReader.readNext();
                                this->setPricingPolicyDetails(xmlStreamReader);
                            }
                            else if (name == "ReportGroupDetails")
                            {
                                xmlStreamReader.readNext();
                                this->readDoubleStructureElement(xmlStreamReader, "ReportGroupDetail");
                            }
							else
								xmlStreamReader.readNext();
						}
						else
							xmlStreamReader.readNext();
					}

					xmlStreamReader.readNext();
					isSuessce = true;
				}
				else {
					xmlStreamReader.raiseError(QObject::tr("Not a initSettingSourceData xml"));
				}
			}
			else {
				xmlStreamReader.readNext();
			}
		}
		this->calculate();

		return isSuessce;
	}

	void ServerSourceManager::updateSettingSourceData(const QDomNode& xmlSettingData)
    {
		UpdateAction::UpdateAction updateAction = static_cast<UpdateAction::UpdateAction>(xmlSettingData.toElement().attribute("UpdateAction").toInt());
		QDomNodeList childlist = xmlSettingData.childNodes();
		for (int i = 0; i < childlist.count(); i++)
		{
			QDomNode itemQDomNode = childlist.item(i);
			QString name = itemQDomNode.nodeName();
			if (name == "Customer")
			{
				QUuid id = QUuid(itemQDomNode.toElement().attribute("Id"));
				if (this->_customer->getId() == id)
					this->_customer->update(itemQDomNode);
                else
                {
                    QMap<QUuid, QSharedPointer<Business::Model::Server::Account>>::const_iterator accountItem = this->_accounts.constBegin();
                    while (accountItem != this->_accounts.constEnd()) {
                      if(accountItem.value()->getCustomerId() == id)
                      {
                          QDomNamedNodeMap map = itemQDomNode.attributes();
                          for (int k = 0; k < map.count(); k++)
                          {
                              QDomNode qDomNode = map.item(k);
                              QString name = qDomNode.nodeName();
                              if (name == "PrivateQuotePolicyId")
                              {
                                  Common::SignalForwarder::getInstance()->sendRefreshSystemSignal();
                                  break;
                              }
                          }
                      }
                      ++accountItem;
                    }

                }
			}
			else if (name == "Accounts")
				this->updateDoubleStructureElement(itemQDomNode, "Account", updateAction);
			else if (name == "Instruments")
				this->updateDoubleStructureElement(itemQDomNode, "Instrument", updateAction);
			else if (name == "VolumeNecessaries")
				this->updateDoubleStructureElement(itemQDomNode, "VolumeNecessary", updateAction);
			else if (name == "VolumeNecessaryDetails")
				this->updateDoubleStructureElement(itemQDomNode, "VolumeNecessaryDetail", updateAction);
			else if (name == "InstalmentPolicies")
				this->updateDoubleStructureElement(itemQDomNode, "InstalmentPolicy", updateAction);
			else if (name == "InstalmentPolicyDetails")
				this->updateDoubleStructureElement(itemQDomNode, "InstalmentPolicyDetail", updateAction);
			else if (name == "PhysicalPaymentDiscounts")
				this->updateDoubleStructureElement(itemQDomNode, "PhysicalPaymentDiscount", updateAction);
			else if (name == "PhysicalPaymentDiscountDetails")
				this->updateDoubleStructureElement(itemQDomNode, "PhysicalPaymentDiscountDetail", updateAction);
			else if (name == "TradePolicyDetails")
				this->updateDoubleStructureElement(itemQDomNode, "TradePolicyDetail", updateAction);
			else if (name == "DealingPolicyDetails")
				this->updateDoubleStructureElement(itemQDomNode, "DealingPolicyDetail", updateAction);
			else if (name == "QuotePolicies")
				this->updateDoubleStructureElement(itemQDomNode, "QuotePolicy", updateAction);
			else if (name == "QuotePolicyDetails")
				this->updateDoubleStructureElement(itemQDomNode, "QuotePolicyDetail", updateAction);
			else if (name == "DeliveryCharges")
				this->updateDoubleStructureElement(itemQDomNode, "DeliveryCharge", updateAction);
			else if (name == "DeliverySpecificationDetails")
				this->updateDoubleStructureElement(itemQDomNode, "DeliverySpecificationDetail", updateAction);
			else if (name == "InstrumentUnits")
				this->updateDoubleStructureElement(itemQDomNode, "InstrumentUnit", updateAction);
			else if (name == "BOPolicies")
				this->updateDoubleStructureElement(itemQDomNode, "BOPolicy", updateAction);
			else if (name == "BOPolicyDetails")
				this->updateDoubleStructureElement(itemQDomNode, "BOPolicyDetail", updateAction);
			else if (name == "BOBetTypes")
				this->updateDoubleStructureElement(itemQDomNode, "BOBetType", updateAction);
            else if (name == "AdministrativeCharges")
                this->updateDoubleStructureElement(itemQDomNode, "AdministrativeCharge", updateAction);
            else if (name == "AdministrativeChargeDetails")
                this->updateDoubleStructureElement(itemQDomNode, "AdministrativeChargeDetail", updateAction);
			else if (name == "SystemParameter")
				this->_systemParameter->update(itemQDomNode);
            else if (name == "PricingPolicyDetails")
                this->updateDoubleStructureElement(itemQDomNode, "PricingPolicyDetail", updateAction);
            else if(name == "ReportGroupDetails")
                this->updateDoubleStructureElement(itemQDomNode, "ReportGroupDetail", updateAction);

		}
	}

	void ServerSourceManager::calculate()
	{
		this->setSystemSetting();
        this->setAccountSetting();
	}

	int ServerSourceManager::getRearPrice(const QUuid & id)
	{
		if (this->_instruments.contains(id))
			return this->_instruments[id]->getRearPrice();
		return 0;
	}

	bool ServerSourceManager::isBVI(const QUuid & id)
	{
		if (this->_instruments.contains(id))
		{
			int counterPartyId = this->_instruments[id]->getCounterPartyId();
			if (counterPartyId > 0 && this->_counterParties.contains(counterPartyId)) return true;
		}
		return false;
	}

	QSharedPointer<Server::CounterParty> ServerSourceManager::getCounterParty(const int & id)
	{
		if (this->_counterParties.contains(id))
			return this->_counterParties[id];
		QSharedPointer<Server::CounterParty> newObject(NULL);
		return newObject;
	}

	int ServerSourceManager::getCounterPartyId(const QUuid & instrumnetId)
	{
		if (this->_instruments.contains(instrumnetId))
		{
			return this->_instruments[instrumnetId]->getCounterPartyId();
		}
		return 0;
	}

	bool ServerSourceManager::getAllowEmployeeTrading(const QUuid & accountId)
	{
		auto accountSource = this->_accounts.value(accountId);
		bool allowManagerTrading = accountSource->getAllowManagerTrading();
		int userRelation = accountSource->getUserRelation();
		bool allowSalesTrading = accountSource->getAllowSalesTrading();
		QUuid salesID = accountSource->getSalesId();		
		QUuid customerId = this->_customer->getId();
		return (customerId != salesID || (customerId == salesID && allowSalesTrading)) &&
				(userRelation != 2 || (userRelation == 2 && allowManagerTrading));
	}

	bool ServerSourceManager::getAllowEmployeeTrading()
	{
		bool isAllowManagerTrading =  false;
		QUuid customerId = this->_customer->getId();
		foreach (QUuid accountId, Common::TraderBaseInfo::getInstance()->getCheckedAccounts())
		{
			auto accountSource = this->_accounts.value(accountId);
			bool allowManagerTrading = accountSource->getAllowManagerTrading();
			int userRelation = accountSource->getUserRelation();
			bool allowSalesTrading = accountSource->getAllowSalesTrading();
			QUuid salesID = accountSource->getSalesId();
			if((customerId != salesID || (customerId == salesID && allowSalesTrading)) &&
				(userRelation != 2 || (userRelation == 2 && allowManagerTrading)))
				isAllowManagerTrading = true;
		}
		return isAllowManagerTrading;
	}

    QSharedPointer<Business::Model::Server::InstrumentUnit> ServerSourceManager::getInstrumentUnit(const QUuid& id)
    {
        if (this->_instrumentUnits.contains(id))
            return this->_instrumentUnits[id];
        else
        {
            QSharedPointer<Server::InstrumentUnit> newObject(nullptr);
            return newObject;
        }
    }

	///-------------------------------------------------------getOrAdd methods-----------------------------------------------------------///
	QSharedPointer<Server::InstrumentUnit> ServerSourceManager::getOrAddInstrumentUnit(const QUuid & id)
	{
		if (this->_instrumentUnits.contains(id))
			return this->_instrumentUnits[id];
		QSharedPointer<Server::InstrumentUnit> newObject(new Server::InstrumentUnit);
		this->_instrumentUnits.insert(id, newObject);
		return newObject;
    }

	QSharedPointer<Server::DealingPolicyDetail> ServerSourceManager::getOrAddDealingPolicyDetail(const QString & key)
	{
		if (this->_dealingPolicyDetails.contains(key))
			return this->_dealingPolicyDetails[key];
		QSharedPointer<Server::DealingPolicyDetail> newObject(new Server::DealingPolicyDetail);
		this->_dealingPolicyDetails.insert(key, newObject);
		return newObject;
	}

	QSharedPointer<Server::Instrument> ServerSourceManager::getOrAddInstrument(const QUuid & id)
	{
		if (this->_instruments.contains(id))
			return this->_instruments[id];
		QSharedPointer<Server::Instrument> newObject(new Server::Instrument);
		this->_instruments.insert(id, newObject);
		return newObject;
	}

	QSharedPointer<Server::Account> ServerSourceManager::getOrAddAccount(const QUuid & id)
	{
		if (this->_accounts.contains(id))
			return this->_accounts[id];
		QSharedPointer<Server::Account> newObject(new Server::Account);
		this->_accounts.insert(id, newObject);
		return newObject;
	}

	QSharedPointer<Server::TradePolicyDetail> ServerSourceManager::getOrAddTradePolicyDetail(const QString & key)
	{
		if (this->_tradePolicyDetails.contains(key))
			return this->_tradePolicyDetails[key];
		QSharedPointer<Server::TradePolicyDetail> newObject(new Server::TradePolicyDetail);
		this->_tradePolicyDetails.insert(key, newObject);
		return newObject;
	}

	QSharedPointer<Server::QuotePolicy> ServerSourceManager::getOrAddQuotePolicy(const QUuid & id)
	{
		if (this->_quotePolicies.contains(id))
			return this->_quotePolicies[id];
		QSharedPointer<Server::QuotePolicy> newObject(new Server::QuotePolicy);
		this->_quotePolicies.insert(id, newObject);
		return newObject;
	}

	QSharedPointer<Server::QuotePolicyDetail> ServerSourceManager::getOrAddQuotePolicyDetail(const QString & key)
	{
		if (this->_quotePolicyDetails.contains(key))
			return this->_quotePolicyDetails[key];
		QSharedPointer<Server::QuotePolicyDetail> newObject(new Server::QuotePolicyDetail);
		this->_quotePolicyDetails.insert(key, newObject);
		return newObject;
	}

	QSharedPointer<Server::InstalmentPolicy> ServerSourceManager::getOrAddInstalmentPolicy(const QUuid & id)
	{
		if (this->_instalmentPolicies.contains(id))
			return this->_instalmentPolicies[id];
		QSharedPointer<Server::InstalmentPolicy> newObject(new Server::InstalmentPolicy);
		this->_instalmentPolicies.insert(id, newObject);
		return newObject;
	}

	QSharedPointer<Server::InstalmentPolicyDetail> ServerSourceManager::getOrAddInstalmentPolicyDetail(const QString & key)
	{
		if (this->_instalmentPolicyDetails.contains(key))
			return this->_instalmentPolicyDetails[key];
		QSharedPointer<Server::InstalmentPolicyDetail> newObject(new Server::InstalmentPolicyDetail);
		this->_instalmentPolicyDetails.insert(key, newObject);
		return newObject;
	}

	QSharedPointer<Server::DeliveryCharge> ServerSourceManager::getOrAddDeliveryCharge(const QUuid & id)
	{
		if (this->_deliveryCharges.contains(id))
			return this->_deliveryCharges[id];
		QSharedPointer<Server::DeliveryCharge> newObject(new Server::DeliveryCharge);
		this->_deliveryCharges.insert(id, newObject);
		return newObject;
	}

	QSharedPointer<Server::VolumeNecessary> ServerSourceManager::getOrAddVolumeNecessary(const QUuid & id)
	{
		if (this->_volumeNecessaries.contains(id))
			return this->_volumeNecessaries[id];
		QSharedPointer<Server::VolumeNecessary> newObject(new Server::VolumeNecessary);
		this->_volumeNecessaries.insert(id, newObject);
		return newObject;
	}

	QSharedPointer<Server::VolumeNecessaryDetail> ServerSourceManager::getOrAddVolumeNecessaryDetail(const QUuid & id)
	{
		if (this->_volumeNecessaryDetails.contains(id))
			return this->_volumeNecessaryDetails[id];
		QSharedPointer<Server::VolumeNecessaryDetail> newObject(new Server::VolumeNecessaryDetail);
		this->_volumeNecessaryDetails.insert(id, newObject);
		return newObject;
	}

	QSharedPointer<Server::DeliverySpecificationDetail> ServerSourceManager::getOrAddDeliverySpecificationDetail(const QString & key)
	{
		if (this->_deliverySpecificationDetails.contains(key))
			return this->_deliverySpecificationDetails[key];
		QSharedPointer<Server::DeliverySpecificationDetail> newObject(new Server::DeliverySpecificationDetail);
		this->_deliverySpecificationDetails.insert(key, newObject);
		return newObject;
	}

	QSharedPointer<Server::PhysicalPaymentDiscount> ServerSourceManager::getOrAddPhysicalPaymentDiscount(const QUuid & id)
	{
		if (this->_physicalPaymentDiscounts.contains(id))
			return this->_physicalPaymentDiscounts[id];
		QSharedPointer<Server::PhysicalPaymentDiscount> newObject(new Server::PhysicalPaymentDiscount);
		this->_physicalPaymentDiscounts.insert(id, newObject);
		return newObject;
	}

	QSharedPointer<Server::PhysicalPaymentDiscountDetail> ServerSourceManager::getOrAddPhysicalPaymentDiscountDetail(const QUuid & id)
	{
		if (this->_physicalPaymentDiscountDetails.contains(id))
			return this->_physicalPaymentDiscountDetails[id];
		QSharedPointer<Server::PhysicalPaymentDiscountDetail> newObject(new Server::PhysicalPaymentDiscountDetail);
		this->_physicalPaymentDiscountDetails.insert(id, newObject);
		return newObject;
	}

	QSharedPointer<Server::BOPolicy> ServerSourceManager::getOrAddBOPolicy(const QUuid & id)
	{
		if (this->_BOPolicies.contains(id))
			return this->_BOPolicies[id];
		QSharedPointer<Server::BOPolicy> newObject(new Server::BOPolicy);
		this->_BOPolicies.insert(id, newObject);
		return newObject;
	}

	QSharedPointer<Server::BOPolicyDetail> ServerSourceManager::getOrAddBOPolicyDetail(const QString & key)
	{
		if (this->_BOPolicyDetails.contains(key))
			return this->_BOPolicyDetails[key];
		QSharedPointer<Server::BOPolicyDetail> newObject(new Server::BOPolicyDetail);
		this->_BOPolicyDetails.insert(key, newObject);
		return newObject;
	}

    QSharedPointer<Business::Model::Server::AdministrativeCharge> ServerSourceManager::getOrAddAdministrativeCharge(const QUuid& id)
    {
        if (this->_AdministrativeCharges.contains(id))
            return this->_AdministrativeCharges[id];
        QSharedPointer<Server::AdministrativeCharge> newObject(new Server::AdministrativeCharge);
        this->_AdministrativeCharges.insert(id, newObject);
        return newObject;
    }

    QSharedPointer<Business::Model::Server::AdministrativeChargeDetail> ServerSourceManager::getOrAddAdministrativeChargeDetail(const QString& key)
    {
        if (this->_AdministrativeChargeDetails.contains(key))
            return this->_AdministrativeChargeDetails[key];
        QSharedPointer<Server::AdministrativeChargeDetail> newObject(new Server::AdministrativeChargeDetail);
        this->_AdministrativeChargeDetails.insert(key, newObject);
        return newObject;
    }

	QSharedPointer<Server::BOBetType> ServerSourceManager::getOrAddBOBetType(const QUuid & id)
	{
		if (this->_BOBetTypes.contains(id))
			return this->_BOBetTypes[id];
		QSharedPointer<Server::BOBetType> newObject(new Server::BOBetType);
		this->_BOBetTypes.insert(id, newObject);
		return newObject;
	}

	QSharedPointer<Server::Organization> ServerSourceManager::getOrAddOrganization(const QUuid & id)
	{
		if (this->_organizations.contains(id))
			return this->_organizations[id];
		QSharedPointer<Server::Organization> newObject(new Server::Organization);
		this->_organizations.insert(id, newObject);
		return newObject;
	}

	QSharedPointer<Server::CounterParty> ServerSourceManager::getOrAddCounterParty(const int & id)
	{
		if (this->_counterParties.contains(id))
			return this->_counterParties[id];
		QSharedPointer<Server::CounterParty> newObject(new Server::CounterParty);
		this->_counterParties.insert(id, newObject);
		return newObject;
	}

    QSharedPointer<Business::Model::Server::PricingPolicyDetail> ServerSourceManager::getOrAddPricingPolicyDetail(const QUuid& key)
    {
        if (this->_PricingPolicyDetails.contains(key))
            return this->_PricingPolicyDetails[key];
        QSharedPointer<Server::PricingPolicyDetail> newObject(new Server::PricingPolicyDetail);
        this->_PricingPolicyDetails.insert(key, newObject);
        return newObject;
    }
	

	///-------------------------------------------------------QXmlStreamReader methods-----------------------------------------------------------///
	void ServerSourceManager::readDoubleStructureElement(QXmlStreamReader& xmlStreamReader, QString name)
	{
		while (!xmlStreamReader.atEnd()) {
			if (xmlStreamReader.isEndElement()) {
				xmlStreamReader.readNext();//Parent end
				break;
			}
			if (xmlStreamReader.isStartElement()) {
				while (!xmlStreamReader.atEnd()) {
					if (xmlStreamReader.isEndElement()) {
						xmlStreamReader.readNext();//Item end
						break;
					}
					QString rName = xmlStreamReader.name().toString();
                    QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  QString("ServerSourceManager::readDoubleStructureElement rName:%1").arg(rName);
					if (rName == name && name == "InstrumentUnit") {
						QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
						QSharedPointer<Server::InstrumentUnit> sourceObject = this->getOrAddInstrumentUnit(id);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "DealingPolicyDetail") {
						QString key = QString("%1_%2").arg(xmlStreamReader.attributes().value("DealingPolicyId").toString(),
							xmlStreamReader.attributes().value("InstrumentId").toString());
						QSharedPointer<Server::DealingPolicyDetail> sourceObject = this->getOrAddDealingPolicyDetail(key);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "Instrument") {
						QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
						QSharedPointer<Server::Instrument> sourceObject = this->getOrAddInstrument(id);						
						sourceObject->update(xmlStreamReader);
						/*if (!sourceObject->getIsActive())
							this->_initializeManager->removeNoActiveInstrument(id);*/
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "Account") {
						QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
						QSharedPointer<Server::Account> sourceObject = this->getOrAddAccount(id);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "TradePolicyDetail") {
						QString key = QString("%1_%2").arg(xmlStreamReader.attributes().value("TradePolicyId").toString(),
							xmlStreamReader.attributes().value("InstrumentId").toString());
						QSharedPointer<Server::TradePolicyDetail> sourceObject = this->getOrAddTradePolicyDetail(key);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "QuotePolicy") {
						QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
						QSharedPointer<Server::QuotePolicy> sourceObject = this->getOrAddQuotePolicy(id);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "QuotePolicyDetail") {
						QString key = QString("%1_%2").arg(xmlStreamReader.attributes().value("QuotePolicyId").toString(),
							xmlStreamReader.attributes().value("InstrumentId").toString());
						QSharedPointer<Server::QuotePolicyDetail> sourceObject = this->getOrAddQuotePolicyDetail(key);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "InstalmentPolicy") {
						QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
						QSharedPointer<Server::InstalmentPolicy> sourceObject = this->getOrAddInstalmentPolicy(id);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "InstalmentPolicyDetail") {
						QString key = QString("%1_%2_%3").arg(
							xmlStreamReader.attributes().value("InstalmentPolicyId").toString(),
							xmlStreamReader.attributes().value("Period").toString(),
							xmlStreamReader.attributes().value("Frequence").toString());
						QSharedPointer<Server::InstalmentPolicyDetail> sourceObject = this->getOrAddInstalmentPolicyDetail(key);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "DeliveryCharge") {
						QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
						QSharedPointer<Server::DeliveryCharge> sourceObject = this->getOrAddDeliveryCharge(id);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "VolumeNecessary") {
						QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
						QSharedPointer<Server::VolumeNecessary> sourceObject = this->getOrAddVolumeNecessary(id);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "VolumeNecessaryDetail") {
						QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
						QSharedPointer<Server::VolumeNecessaryDetail> sourceObject = this->getOrAddVolumeNecessaryDetail(id);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "DeliverySpecificationDetail") {
						QString key = QString("%1_%2").arg(
							xmlStreamReader.attributes().value("DeliverySpecificationId").toString(),
							xmlStreamReader.attributes().value("Size").toString());
						QSharedPointer<Server::DeliverySpecificationDetail> sourceObject = this->getOrAddDeliverySpecificationDetail(key);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "PhysicalPaymentDiscount") {
						QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
						QSharedPointer<Server::PhysicalPaymentDiscount> sourceObject = this->getOrAddPhysicalPaymentDiscount(id);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "PhysicalPaymentDiscountDetail") {
						QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
						QSharedPointer<Server::PhysicalPaymentDiscountDetail> sourceObject = this->getOrAddPhysicalPaymentDiscountDetail(id);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "BOPolicy") {
						QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
						QSharedPointer<Server::BOPolicy> sourceObject = this->getOrAddBOPolicy(id);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "BOPolicyDetail") {
						QString key = QString("%1_%2_%3").arg(
							xmlStreamReader.attributes().value("BOPolicyId").toString(),
							xmlStreamReader.attributes().value("BOBetTypeId").toString(),
							xmlStreamReader.attributes().value("Frequency").toString());
						QSharedPointer<Server::BOPolicyDetail> sourceObject = this->getOrAddBOPolicyDetail(key);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "BOBetType") {
						QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
						QSharedPointer<Server::BOBetType> sourceObject = this->getOrAddBOBetType(id);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "Organization") {
						QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
						QSharedPointer<Server::Organization> sourceObject = this->getOrAddOrganization(id);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
					else if (rName == name && name == "CounterParty") {
						int id = xmlStreamReader.attributes().value("Id").toInt();
						QSharedPointer<Server::CounterParty> sourceObject = this->getOrAddCounterParty(id);
						sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
					}
                    else if (rName == name && name == "AdministrativeCharge") {
                        QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
                        QSharedPointer<Server::AdministrativeCharge> sourceObject = this->getOrAddAdministrativeCharge(id);
                        sourceObject->update(xmlStreamReader);
                        xmlStreamReader.readNext();
                    }
                    else if (rName == name && name == "AdministrativeChargeDetail") {
                        QString key = QString("%1_%2").arg(
                            xmlStreamReader.attributes().value("AdministrativeChargeId").toString(),
                            xmlStreamReader.attributes().value("Quantity").toString());
                        QSharedPointer<Server::AdministrativeChargeDetail> sourceObject = this->getOrAddAdministrativeChargeDetail(key);
                        sourceObject->update(xmlStreamReader);
                        xmlStreamReader.readNext();
                    }
                    else if (rName == name && name == "PricingPolicyDetail") {
                        QUuid id = QUuid(xmlStreamReader.attributes().value("ID").toString());
                        QSharedPointer<Server::PricingPolicyDetail> sourceObject = this->getOrAddPricingPolicyDetail(id);
                        sourceObject->update(xmlStreamReader);
                        xmlStreamReader.readNext();
                    }
                    else if (rName == name && name == "ReportGroupDetail") {
                        QString name = xmlStreamReader.attributes().value("Name").toString();
                        if(!this->_reportGroupDetails.contains(name))
                            this->_reportGroupDetails.append(name);
                        xmlStreamReader.readNext();
                    }
					else {
						xmlStreamReader.readNext();
					}
				}
			}
			else {
				xmlStreamReader.readNext();
			}
		}
	}

	void ServerSourceManager::updateDoubleStructureElement(QDomNode & xmlData, QString name, UpdateAction::UpdateAction updateAction)
	{
		QDomNodeList secondChildlist = xmlData.childNodes();
		for (int j = 0; j < secondChildlist.count(); j++)
		{
			QDomNode secondItemQDomNode = secondChildlist.item(j);
			QString nodeName = secondItemQDomNode.nodeName();
			QDomElement secondItemElement = secondItemQDomNode.toElement();
            //qDebug() << "ServerSourceManager::updateDoubleStructureElement nodeName:" << nodeName << " name:" << name;
			if (nodeName == name && name == "Account")
			{
				QUuid id = QUuid(secondItemElement.attribute("Id"));
				if (this->_accounts.contains(id))
					this->_accounts[id]->update(secondItemQDomNode);
			}
			else if (nodeName == name && name == "Instrument")
			{
				QUuid id = QUuid(secondItemElement.attribute("Id"));
				if (this->_instruments.contains(id))
					this->_instruments[id]->update(secondItemQDomNode);
			}
			else if (nodeName == name && name == "VolumeNecessary")
			{
				QUuid id = QUuid(secondItemElement.attribute("Id"));
				if (updateAction == UpdateAction::Delete)
					this->_volumeNecessaries.remove(id);
				else
				{
					auto sourceObject = this->getOrAddVolumeNecessary(id);
					sourceObject->update(secondItemQDomNode);
				}
			}
			else if (nodeName == name && name == "VolumeNecessaryDetail")
			{
				QUuid id = QUuid(secondItemElement.attribute("Id"));
				if (updateAction == UpdateAction::Delete)
					this->_volumeNecessaryDetails.remove(id);
				else
				{
					auto sourceObject = this->getOrAddVolumeNecessaryDetail(id);
					sourceObject->update(secondItemQDomNode);
				}
			}
			else if (nodeName == name && name == "InstalmentPolicy")
			{
				QUuid id = QUuid(secondItemElement.attribute("Id"));
				if (updateAction == UpdateAction::Delete)
					this->_instalmentPolicies.remove(id);
				else
				{
					auto sourceObject = this->getOrAddInstalmentPolicy(id);
					sourceObject->update(secondItemQDomNode);
				}
			}
			else if (nodeName == name && name == "InstalmentPolicyDetail")
			{
				QString key = QString("%1_%2_%3").arg(
					secondItemElement.attribute("InstalmentPolicyId"),
					secondItemElement.attribute("Period"),
					secondItemElement.attribute("Frequence"));
				if (updateAction == UpdateAction::Delete)
					this->_instalmentPolicyDetails.remove(key);
				else
				{
					auto sourceObject = this->getOrAddInstalmentPolicyDetail(key);
					sourceObject->update(secondItemQDomNode);
				}
			}
			else if (nodeName == name && name == "PhysicalPaymentDiscount")
			{
				QUuid id = QUuid(secondItemElement.attribute("Id"));
				if (updateAction == UpdateAction::Delete)
					this->_physicalPaymentDiscounts.remove(id);
				else
				{
					auto sourceObject = this->getOrAddPhysicalPaymentDiscount(id);
					sourceObject->update(secondItemQDomNode);
				}
			}
			else if (nodeName == name && name == "PhysicalPaymentDiscountDetail")
			{
				QUuid id = QUuid(secondItemElement.attribute("Id"));
				if (updateAction == UpdateAction::Delete)
					this->_physicalPaymentDiscountDetails.remove(id);
				else
				{
					auto sourceObject = this->getOrAddPhysicalPaymentDiscountDetail(id);
					sourceObject->update(secondItemQDomNode);
				}
			}
            else if (nodeName == name && name == "TradePolicyDetail") {
				if (updateAction == UpdateAction::Delete)
                {
                    bool isRefresh = false;
                    QUuid tradePolicyId = QUuid(secondItemElement.attribute("TradePolicyId"));
                    QMapIterator<QUuid, QSharedPointer<Business::Model::Server::Account>> accountItem(_accounts);
                    while (accountItem.hasNext()) {
                        accountItem.next();
                        if(tradePolicyId == accountItem.value()->getTradePolicyId())
                        {
                            isRefresh = true;
                            break;
                        }
                    }
                    if (isRefresh)
                        Common::SignalForwarder::getInstance()->sendRefreshSystemSignal();
                    //this->_tradePolicyDetails.remove(key);
                }
				else
				{
                    QString key = QString("%1_%2").arg(
                        secondItemElement.attribute("TradePolicyId"),
                        secondItemElement.attribute("InstrumentId"));
					auto sourceObject = this->getOrAddTradePolicyDetail(key);
					sourceObject->update(secondItemQDomNode);
                    qDebug() << "  key:" <<  key << " DQMaxMove:"<< sourceObject->getDQMaxMove();
				}
			}
			else if (nodeName == name && name == "DealingPolicyDetail") {
				QString key = QString("%1_%2").arg(
					secondItemElement.attribute("DealingPolicyId"),
					secondItemElement.attribute("InstrumentId"));
				if (updateAction == UpdateAction::Delete)
					this->_dealingPolicyDetails.remove(key);
				else
				{
					auto sourceObject = this->getOrAddDealingPolicyDetail(key);
					sourceObject->update(secondItemQDomNode);
				}
			}
			else if (nodeName == name && name == "QuotePolicy") {
				QUuid id = QUuid(secondItemElement.attribute("Id"));
				if (updateAction == UpdateAction::Delete)
					this->_quotePolicies.remove(id);
				else
				{
					auto sourceObject = this->getOrAddQuotePolicy(id);
					sourceObject->update(secondItemQDomNode);
				}
			}
			else if (nodeName == name && name == "QuotePolicyDetail") {
				QString key = QString("%1_%2").arg(
					secondItemElement.attribute("QuotePolicyId"),
					secondItemElement.attribute("InstrumentId"));
				if (updateAction == UpdateAction::Delete)
					this->_quotePolicyDetails.remove(key);
				else
				{
					auto sourceObject = this->getOrAddQuotePolicyDetail(key);
					sourceObject->update(secondItemQDomNode);
				}
				if (updateAction == UpdateAction::Delete || updateAction == UpdateAction::Add)
				{
					QUuid instrumentId = secondItemElement.attribute("InstrumentId");
                    QUuid quotePolicyId = secondItemElement.attribute("QuotePolicyId");
                    bool isContainQuotePolicy = false;
                    QList<QSharedPointer<Business::Model::Server::Account>> accounts =_accounts.values();
                    foreach(QSharedPointer<Business::Model::Server::Account> account, accounts)
                    {
                        if(account->getUseQuotePolicyId() == quotePolicyId)
                            isContainQuotePolicy = true;
                    }
                    if (isContainQuotePolicy && _instruments.contains(instrumentId))
						Common::SignalForwarder::getInstance()->sendRefreshSystemSignal();
				}
			}

			else if (nodeName == name && name == "BOPolicy") {
				QUuid id = QUuid(secondItemElement.attribute("Id"));
				if (updateAction == UpdateAction::Delete)
					this->_BOPolicies.remove(id);
				else
				{
					auto sourceObject = this->getOrAddBOPolicy(id);
					sourceObject->update(secondItemQDomNode);
				}
			}
			else if (nodeName == name && name == "BOPolicyDetail") {
				QString key = QString("%1_%2_%3").arg(
					secondItemElement.attribute("BOPolicyId"),
					secondItemElement.attribute("BOBetTypeId"),
					secondItemElement.attribute("Frequency"));
				if (updateAction == UpdateAction::Delete)
					this->_BOPolicyDetails.remove(key);
				else
				{
					auto sourceObject = this->getOrAddBOPolicyDetail(key);
					sourceObject->update(secondItemQDomNode);
				}
			}
			else if (nodeName == name && name == "BOBetType") {
				QUuid id = QUuid(secondItemElement.attribute("Id"));
				if (updateAction == UpdateAction::Delete)
					this->_BOBetTypes.remove(id);
				else
				{
					auto sourceObject = this->getOrAddBOBetType(id);
					sourceObject->update(secondItemQDomNode);
				}
			}
			else if (nodeName == name && name == "DeliveryCharge") {
				QUuid id = QUuid(secondItemElement.attribute("Id"));
				if (updateAction == UpdateAction::Delete)
					this->_deliveryCharges.remove(id);
				else
				{
					auto sourceObject = this->getOrAddDeliveryCharge(id);
					sourceObject->update(secondItemQDomNode);
				}
			}
			else if (nodeName == name && name == "DeliverySpecificationDetail") {
				QString key = QString("%1_%2").arg(
					secondItemElement.attribute("DeliverySpecificationId"),
					secondItemElement.attribute("Size"));
				if (updateAction == UpdateAction::Delete)
					this->_deliverySpecificationDetails.remove(key);
				else
				{
					auto sourceObject = this->getOrAddDeliverySpecificationDetail(key);
					sourceObject->update(secondItemQDomNode);
				}
			}
			else if (nodeName == name && name == "InstrumentUnit") {
				QUuid id = QUuid(secondItemElement.attribute("Id"));
				if (updateAction == UpdateAction::Delete)
					this->_instrumentUnits.remove(id);
				else
				{
					auto sourceObject = this->getOrAddInstrumentUnit(id);
					sourceObject->update(secondItemQDomNode);
				}
			}
			else if (nodeName == name && name == "CounterParty") {
				int id = secondItemElement.attribute("Id").toInt();
				if (updateAction == UpdateAction::Delete)
					this->_counterParties.remove(id);
				else
				{
					auto sourceObject = this->getOrAddCounterParty(id);
					sourceObject->update(secondItemQDomNode);
				}
			}
            else if (nodeName == name && name == "AdministrativeCharge") {
                QUuid id = QUuid(secondItemElement.attribute("Id"));
                if (updateAction == UpdateAction::Delete)
                    this->_AdministrativeCharges.remove(id);
                else
                {
                    auto sourceObject = this->getOrAddAdministrativeCharge(id);
                    sourceObject->update(secondItemQDomNode);
                }
            }
            else if (nodeName == name && name == "AdministrativeChargeDetail") {
                QString key = QString("%1_%2").arg(
                    secondItemElement.attribute("AdministrativeChargeId"),
                    secondItemElement.attribute("Quantity"));
                if (updateAction == UpdateAction::Delete)
                    this->_AdministrativeChargeDetails.remove(key);
                else
                {
                    auto sourceObject = this->getOrAddAdministrativeChargeDetail(key);
                    sourceObject->update(secondItemQDomNode);
                }
            }            
            else if (nodeName == name && name == "PricingPolicyDetail") {
                QUuid key = QUuid(secondItemElement.attribute("ID"));
                if (updateAction == UpdateAction::Delete)
                    this->_PricingPolicyDetails.remove(key);
                else
                {
                    auto sourceObject = this->getOrAddPricingPolicyDetail(key);
                    sourceObject->update(secondItemQDomNode);
                }
            }
            else if (nodeName == name && name == "ReportGroupDetail") {
                QString name = secondItemElement.attribute("Name");
                if (updateAction == UpdateAction::Delete)
                    this->_reportGroupDetails.removeOne(name);
                else
                {
                    if(!this->_reportGroupDetails.contains(name))
                        this->_reportGroupDetails.append(name);
                }
            }
		}
	}

    void ServerSourceManager::setIsSptReconfirm(bool value)
    {
        this->_customer->setIsSptReconfirm(value);
        this->_settingManager->_systemSetting->setIsSptReconfirm(value);
    }

    void ServerSourceManager::setSystemSetting()
	{
		this->_settingManager->_systemSetting->setShowLog(this->_customer->getShowLog());
		this->_settingManager->_systemSetting->setDQOrderOutTime(this->_customer->getDQOrderOutTime());
		this->_settingManager->_systemSetting->setEnableFastTrade(this->_customer->getEnableFastTrade());
        this->_settingManager->_systemSetting->setIsSptReconfirm(this->_systemParameter->getAllowTraderDQReconfirm() ? this->_customer->getIsSptReconfirm(): false);
		this->_settingManager->_systemSetting->setSingleAccountOrderType(this->_customer->getSingleAccountOrderType());
		this->_settingManager->_systemSetting->setMultiAccountsOrderType(this->_customer->getMultiAccountsOrderType());
		this->_settingManager->_systemSetting->setDisallowTrade(this->_customer->getDisallowTrade());
		this->_settingManager->_systemSetting->setIsNoShowAccountStatus(this->_customer->getIsNoShowAccountStatus());
		this->_settingManager->_systemSetting->setIsEmployee(this->_customer->getIsEmployee());
		this->_settingManager->_systemSetting->setDQMaxLotApplyAccount(this->_systemParameter->getDQMaxLotApplyAccount());
		this->_settingManager->_systemSetting->setBankAccountOnly(this->_systemParameter->getBankAccountOnly());
		this->_settingManager->_systemSetting->setShowPriceChangedBeforevCloseConfirmWindow(this->_systemParameter->getShowPriceChangedBeforeCloseConfirmWindow());
		this->_settingManager->_systemSetting->setHighBid(this->_systemParameter->getHighBid());
		this->_settingManager->_systemSetting->setLowBid(this->_systemParameter->getLowBid());
		this->_settingManager->_systemSetting->setEnableTrendSheetChart(this->_systemParameter->getEnableTrendSheetChart());
		this->_settingManager->_systemSetting->setEnableResetTelephonePin(this->_systemParameter->getEnableResetTelephonePin());
		this->_settingManager->_systemSetting->setEnableModifyLeverage(this->_systemParameter->getEnableModifyLeverage());
		this->_settingManager->_systemSetting->setTimeOptionInTraderLogAndConfirmWindow(this->_systemParameter->getTimeOptionInTraderLogAndConfirmWindow());
		this->_settingManager->_systemSetting->setShowHundredChart(this->_systemParameter->getShowHundredChart());
		this->_settingManager->_systemSetting->setSptReconfirmDuration(this->_systemParameter->getSptReconfirmDuration());
		this->_settingManager->_systemSetting->setPriceRow(this->_systemParameter->getPriceRow());
		this->_settingManager->_systemSetting->setMaxCustomerBankNo(this->_systemParameter->getMaxCustomerBankNo());
		this->_settingManager->_systemSetting->setAllowEditBankAccountInTrader(this->_systemParameter->getAllowEditBankAccountInTrader());
		this->_settingManager->_systemSetting->setRatioIncludeCredit(this->_systemParameter->getRatioIncludeCredit());
		this->_settingManager->_systemSetting->setIncludeFeeOnRiskAction(this->_systemParameter->getIncludeFeeOnRiskAction());
		this->_settingManager->_systemSetting->setBOIncludePrincipal(this->_systemParameter->getBOIncludePrincipal());
		this->_settingManager->_systemSetting->setOpenPositionShowTime(this->_systemParameter->getOpenPositionShowTime());
		this->_settingManager->_systemSetting->setCnyCurrencyId(this->_systemParameter->getCnyCurrencyId());
		this->_settingManager->_systemSetting->setInterestFreeAlertDay(this->_systemParameter->getInterestFreeAlertDay());
		this->_settingManager->_systemSetting->setConsumerCode(this->_systemParameter->getConsumerCode());
		this->_settingManager->_systemSetting->setPaymentPath(this->_systemParameter->getPaymentPath());
		this->_settingManager->_systemSetting->setIsCheckAcceptDQVariation(this->_systemParameter->getIsCheckAcceptDQVariation());
		this->_settingManager->_systemSetting->setNonQuoteVerificationUiDelay(this->_systemParameter->getNonQuoteVerificationUiDelay());
		this->_settingManager->_systemSetting->setIsAllowLimitInSpread(this->_systemParameter->getIsAllowLimitInSpread());
		this->_settingManager->_systemSetting->setAllowTraderResetAlertLevel(this->_systemParameter->getAllowTraderResetAlertLevel());
		this->_settingManager->_systemSetting->setEnableNetAsset(this->_systemParameter->getEnableNetAsset());
        this->_settingManager->_systemSetting->setAllowTraderDQReconfirm(this->_systemParameter->getAllowTraderDQReconfirm());
        this->_settingManager->_systemSetting->setReportUrl(this->_systemParameter->getReportUrl());
        this->_settingManager->_systemSetting->setEnableClickTrade(this->_systemParameter->getEnableClickTrade());        
        this->_settingManager->_systemSetting->setTradePLFloatColumnNumber(this->_systemParameter->getTradePLFloatColumnNumber());
        this->_settingManager->_systemSetting->setAccountCodeColumnNumber(this->_systemParameter->getAccountCodeColumnNumber());
        this->_settingManager->_systemSetting->setOrderCodeColumnNumber(this->_systemParameter->getOrderCodeColumnNumber());
        this->_settingManager->_systemSetting->setClientBookTraderUrl(this->_systemParameter->getClientBookTraderUrl());
        this->_settingManager->_systemSetting->setIexCrmUserCenterUrl(this->_systemParameter->getIexCrmUserCenterUrl());
        this->_settingManager->_systemSetting->setIsIexCrmUser(this->_customer->getIsIexCrmUser());
        this->_settingManager->_systemSetting->setIsShowNews(this->_systemParameter->getIsShowNews());
        this->_settingManager->_systemSetting->setIsUploadClientLog(this->_customer->getIsUploadClientLog());

		Common::TraderBaseInfo::getInstance()->setEnableNetAsset(this->_settingManager->_systemSetting->getEnableNetAsset());

		QUuid defaultQuotePolicyId;
		foreach(auto item, this->_quotePolicies)
		{
			if (item->getIsDefault())
				defaultQuotePolicyId = item->getId();
		}
		QList<QSharedPointer<Server::QuotePolicyDetail>> defaultQuotePolicyDetails;
		foreach(auto item, this->_quotePolicyDetails)
		{
			if (item->getQuotePolicyId() == defaultQuotePolicyId)
				defaultQuotePolicyDetails.append(item);
		}
		foreach(auto item, defaultQuotePolicyDetails)
		{
			if (this->_instruments.contains(item->getInstrumentId()))
			{
				QSharedPointer<Setting::CalculateQuotationDefaultSetting> calculateQuotationDefaultSetting = this->_settingManager->getOrAddCalculateQuotationDefaultSetting(item->getInstrumentId());
				auto instrument = this->_instruments[item->getInstrumentId()];
				calculateQuotationDefaultSetting->setInstrumentId(item->getInstrumentId());
				calculateQuotationDefaultSetting->setSpreadPoints(item->getSpreadPoints());
				calculateQuotationDefaultSetting->setAutoAdjustPoints(item->getAutoAdjustPoints());
				calculateQuotationDefaultSetting->setPriceType(item->getPriceType());
				calculateQuotationDefaultSetting->setNumeratorUnit(instrument->getNumeratorUnit());
				calculateQuotationDefaultSetting->setDenominator(instrument->getDenominator());
				calculateQuotationDefaultSetting->setCaculateChangeWithDenominator(this->_systemParameter->getCaculateChangeWithDenominator());
				this->_settingManager->AddOrUpdateCalculateQuotationDefaultSetting(calculateQuotationDefaultSetting);
			}
		}
	}

	void ServerSourceManager::setAccountSetting()
	{
		QMap<QUuid, QSharedPointer<Setting::BusinessAccount>> newAccountMaps;
		QMapIterator<QUuid, QSharedPointer<Server::Account>> accountItem(_accounts);
		while (accountItem.hasNext())
		{
			accountItem.next();
			QUuid accountId = accountItem.key();
			auto accountSource = accountItem.value();

			QSharedPointer<Setting::BusinessAccount> account(new Setting::BusinessAccount);

			account->setId(accountId);
			account->setCode(accountSource->getCode());
			account->setAccountType(accountSource->getType());
			account->setAllowedTrade(!this->_customer->getDisallowTrade() && accountSource->getIsTradingAllowed());
			account->setBalanceDeficitAllowPay(this->_systemParameter->getBalanceDeficitAllowPay());
			account->setBeginTime(accountSource->getBeginTime());
			account->setCode(accountSource->getCode());
			account->setCreditAmount(accountSource->getCreditAmount());
			account->setCurrencyId(accountSource->getCurrencyId());
			account->setCustomerId(accountSource->getCustomerId());
			account->setDisplayAlert(this->_customer->getDisplayAlert());
			account->setEnableMarginPin(this->_systemParameter->getEnableMarginPin());
			account->setEnableModifyTelephoneIdentificationCode(this->_systemParameter->getEnableModifyTelephoneIdentificationCode());
			account->setEndTime(accountSource->getEndTime());
			account->setIsDisplayLedger(this->_customer->getIsDisplayLedger());
			account->setIsMultiCurrency(accountSource->getIsMultiCurrency());
			account->setName(accountSource->getName());
			account->setPublicQuotePolicyId(accountSource->getPublicQuotePolicyId());
			account->setQuotePolicyId(accountSource->getUseQuotePolicyId());
			account->setSales(accountSource->getSalesId(), accountSource->getSalesCode());
            account->setAgentId(accountSource->getAgentId());
			account->setForbiddenAlert(accountSource->getForbiddenAlert());
			account->setCounterPartiesMask(accountSource->getCounterPartiesMask());
			account->setOrganizationCode(accountSource->getOrganizationCode());
            account->setPrintOrderConfirmation(accountSource->getPrintOrderConfirmation());
            account->setShowWeightOnly(accountSource->getShowWeightOnly());
            account->setStartupQuotation(accountSource->getStartupQuotation());

			//MarginSetting
			auto marginSetting = account->getMarginSetting();
			marginSetting->setAgentCode(accountSource->getAgentCode());
			marginSetting->setAgentEmail(accountSource->getAgentEmail());
			marginSetting->setAgentName(accountSource->getAgentName());
			marginSetting->setAllowChangePasswordInTrader(accountSource->getAllowChangePasswordInTrader());
			marginSetting->setBankAccountDefaultCountryId(accountSource->getBankAccountDefaultCountryId());
            marginSetting->setDefaultPaymentCurrencyId(accountSource->getDefaultPaymentCurrencyId());
			marginSetting->setBankAccountNameMustSameWithAccountName(this->_systemParameter->getBankAccountNameMustSameWithAccountName());
			marginSetting->setConsumerCode(accountSource->getConsumerCode());
			marginSetting->setCustomerEmail(accountSource->getCustomerEmail());
			marginSetting->setEnableAccountTransfer(accountSource->getEnableAccountTransfer());
			marginSetting->setEnableAgentRegistration(accountSource->getEnableAgentRegistration());
			marginSetting->setEnableCMExtension(accountSource->getEnableCMExtension());
			marginSetting->setEnableOwnerRegistration(accountSource->getEnableOwnerRegistration());
			marginSetting->setEnablePI(accountSource->getEnablePI());
			marginSetting->setEnablePICash(accountSource->getEnablePICash());
			marginSetting->setEnablePIInterACTransfer(accountSource->getEnablePIInterACTransfer());
			marginSetting->setNeedBeneficiaryInPaymentInstruction(accountSource->getNeedBeneficiaryInPaymentInstruction());
			marginSetting->setPaymentInstructionRemark(accountSource->getPaymentInstructionRemark());
			marginSetting->setOrganizationPhone(accountSource->getOrganizationPhone());
			if (this->_organizations.contains(accountSource->getOrganizationId()))
			{
				auto organization = this->_organizations[accountSource->getOrganizationId()];
                marginSetting->setOrganizationId(organization->getId());
                marginSetting->setOrganizationCode(organization->getCode());
				marginSetting->setOrganizationName(organization->getName());
			}
            marginSetting->setAllowSalesPayment(accountSource->getAllowSalesPayment());
            marginSetting->setAllowSalesDeposit(accountSource->getAllowSalesDeposit());
            marginSetting->setUploadBankCard(accountSource->getUploadBankCard());
            marginSetting->setOnlineTransferCount(accountSource->getOnlineTransferCount());

			//LeverageSetting
			auto leverageSetting = account->getLeverageSetting();
			leverageSetting->setEnableModify(this->_systemParameter->getEnableModifyLeverage());
			leverageSetting->setLeverage(accountSource->getLeverage());
			leverageSetting->setMax(this->_systemParameter->getMaxLeverage());
			leverageSetting->setMin(this->_systemParameter->getMinLeverage());
			leverageSetting->setStep(this->_systemParameter->getLeverageStep());

			//InstrumentSettings
			this->setInstrumentSettings(account, accountSource);

			newAccountMaps.insert(accountId, account);
		}

		this->_settingManager->_accounts = newAccountMaps;

	}

	void ServerSourceManager::setInstrumentSettings(QSharedPointer<Setting::BusinessAccount>& account, QSharedPointer<Server::Account>& accountSource)
	{
		QMapIterator<QUuid, QSharedPointer<Server::Instrument>> instrumentItem(_instruments);
		while (instrumentItem.hasNext())
		{
			instrumentItem.next();
			QUuid instrumentId = instrumentItem.key();
			QString instrumentIdStr = instrumentId.toString().mid(1, 36);
			auto instrumentSource = instrumentItem.value();

			QString traderPolicyKey = QString("%1_%2").arg(
				accountSource->getTradePolicyId().toString().mid(1, 36),
				instrumentIdStr);
			if (this->_tradePolicyDetails.contains(traderPolicyKey))
			{
				auto traderPolicyDetailSource = this->_tradePolicyDetails[traderPolicyKey];
				auto dealingPolicyDetailSource = this->getDealingPolicyDetail(
					this->_customer->getDealingPolicyId(), instrumentId);
				QString quotePolicyKey = QString("%1_%2").arg(
					accountSource->getUseQuotePolicyId().toString().mid(1, 36),
					instrumentIdStr);
				if (this->_quotePolicyDetails.contains(quotePolicyKey))
				{
					auto quotePolicyDetailSource = this->_quotePolicyDetails[quotePolicyKey];
					auto instrument = account->getOrAddInstrument(instrumentId);
					this->setInstrumentSettingItem(instrument, accountSource, instrumentSource,
						traderPolicyDetailSource, dealingPolicyDetailSource, quotePolicyDetailSource);
				}
				else
				{
					quotePolicyKey = QString("%1_%2").arg(
						accountSource->getPublicQuotePolicyId().toString().mid(1, 36),
						instrumentIdStr);
					if (this->_quotePolicyDetails.contains(quotePolicyKey))
					{
						auto quotePolicyDetailSource = this->_quotePolicyDetails[quotePolicyKey];
						auto instrument = account->getOrAddInstrument(instrumentId);
						this->setInstrumentSettingItem(instrument, accountSource, instrumentSource,
							traderPolicyDetailSource, dealingPolicyDetailSource, quotePolicyDetailSource);
					}
				}
			}
		}
	}

    void ServerSourceManager::setInstrumentSettingItem(
        QSharedPointer<Setting::BusinessInstrument>& instrument,
        QSharedPointer<Server::Account>& accountSource,
        QSharedPointer<Server::Instrument>& instrumentSource,
        QSharedPointer<Server::TradePolicyDetail>& tradePolicyDetailSource,
        QSharedPointer<Server::DealingPolicyDetail>& dealingPolicyDetailSource,
        QSharedPointer<Server::QuotePolicyDetail>& quotePolicyDetailSource)
	{
		////BusinessInstrument
		instrument->setAutoAdjustPoints(quotePolicyDetailSource->getAutoAdjustPoints());
		instrument->setBeginTime(instrumentSource->getBeginTime());
		instrument->setBuyInterestValueDay(tradePolicyDetailSource->getBuyInterestValueDay());
		instrument->setCode(instrumentSource->getCode());
		instrument->setCurrencyId(instrumentSource->getCurrencyId());
		instrument->setDeliveryPointGroupId(tradePolicyDetailSource->getDeliveryPointGroupId());
		instrument->setDeliveryTimeBeginDay(instrumentSource->getDeliveryTimeBeginDay());
		instrument->setDeliveryTimeEndDay(instrumentSource->getDeliveryTimeEndDay());
		instrument->setEndTime(instrumentSource->getEndTime());
		instrument->setEnhancedPriceDigit(instrumentSource->getEnhancedPriceDigit());
		instrument->setEnquiryOutTime(this->_systemParameter->getEnquiryOutTime());
		instrument->setExceptionEnquiryOutTime(this->_systemParameter->getExceptionEnquiryOutTime());
		instrument->setId(instrumentSource->getId());
		instrument->setInstrumentCategory(instrumentSource->getCategory());
		instrument->setInstrumentDescription(instrumentSource->getDescription());
		instrument->setIsNormal(instrumentSource->getIsNormal());
		instrument->setIsSinglePrice(instrumentSource->getIsSinglePrice());
		instrument->setMaxAlertPricePerInstrument(this->_systemParameter->getMaxAlertPricePerInstrument());
		instrument->setNarrative(instrumentSource->getNarrative());
		instrument->setOriginCode(instrumentSource->getOriginCode());
		instrument->setPhysicalLotDecimal(instrumentSource->getPhysicalLotDecimal());
		instrument->setPlaceConfirmMinTime(this->_systemParameter->getPlaceConfirmMinTime());
		instrument->setPriceType(quotePolicyDetailSource->getPriceType());
		instrument->setQuoteDescription(instrumentSource->getQuoteDescription());
		instrument->setSellInterestValueDay(tradePolicyDetailSource->getSellInterestValueDay());
		instrument->setShowPriceSuperscript(instrumentSource->getShowPriceSuperscript());
		instrument->setSpreadPoints(quotePolicyDetailSource->getSpreadPoints());
		instrument->setUnit(instrumentSource->getUnit());
		instrument->setUnitId(instrumentSource->getUnitId());
		if (this->_instrumentUnits.contains(instrumentSource->getUnitId()))
		{
			auto instrumenUnit = this->_instrumentUnits[instrumentSource->getUnitId()];
			instrument->setWeight(instrumenUnit->getWeight());
		}
        else
           instrument->setWeight(1);
        instrument->setDeliveryMode(tradePolicyDetailSource->getDeliveryMode());
        instrument->setAdministrativeChargeId(tradePolicyDetailSource->getAdministrativeChargeId());
        instrument->setAdministrativeChargeConvertRate(tradePolicyDetailSource->getAdministrativeChargeConvertRate());
        if(!tradePolicyDetailSource->getAdministrativeChargeId().isNull())
        {
            QString administrativeChargeIdStr = tradePolicyDetailSource->getAdministrativeChargeId().toString().mid(1, 36);
            QMapIterator<QString, QSharedPointer<Business::Model::Server::AdministrativeChargeDetail>> item(this->_AdministrativeChargeDetails);
            while (item.hasNext()) {
              item.next();
              if(item.key().contains(administrativeChargeIdStr))
              {
                 instrument->addOrUpdateAdministrativeChargeDetail(item.value()->getQuantity(), item.value()->getChargeRate());
              }
            }
        }

		////interestRateSetting
		auto interestRateSetting = instrument->getOrAddInterestRateSetting();
		auto interestRateSource = this->_settingManager->getInterestRateSource(tradePolicyDetailSource->getInterestRateId());
		if (!interestRateSource.isNull())
		{
			interestRateSetting->setBuy(interestRateSource->getBuy());
			interestRateSetting->setSell(interestRateSource->getSell());
		}
		////prepaymentInterestRateSetting
		auto prepaymentInterestRateSetting = instrument->getOrAddPrepaymentInterestRateSetting();
		if (!tradePolicyDetailSource->getPrepaymentInterestRateId().isNull())
		{
			auto interestRateSource2 = this->_settingManager->getInterestRateSource(tradePolicyDetailSource->getInterestRateId());
			prepaymentInterestRateSetting->setBuy(interestRateSource2->getBuy());
			prepaymentInterestRateSetting->setSell(interestRateSource2->getSell());
		}
		else
			prepaymentInterestRateSetting.clear();

		////deliveryChargeSetting
		auto deliveryChargeSetting = instrument->getOrAddDeliveryChargeSetting();
		if (!tradePolicyDetailSource->getDeliveryChargeId().isNull() && this->_deliveryCharges.contains(tradePolicyDetailSource->getDeliveryChargeId()))
		{
			auto deliveryChargeSource = this->_deliveryCharges[tradePolicyDetailSource->getDeliveryChargeId()];
			deliveryChargeSetting->setChargeBasis(deliveryChargeSource->getChargeBasis());
			deliveryChargeSetting->setChargeRate(deliveryChargeSource->getChargeRate());
			deliveryChargeSetting->setMinCharge(deliveryChargeSource->getMinCharge());
		}
		else
			deliveryChargeSetting.clear();

		////calculateSetting
		auto calculateSetting = instrument->getOrAddCalculateSetting();
		this->setCalculateSetting(calculateSetting, accountSource, instrumentSource, tradePolicyDetailSource);

		////movePipsInfoSetting
		auto movePipsInfoSetting = instrument->getOrAddMovePipsInfoSetting();
		this->setMovePipsInfoSetting(movePipsInfoSetting, instrumentSource, tradePolicyDetailSource);

		////placeSetting
		auto placeSetting = instrument->getOrAddPlaceSetting();
		this->setPlaceSettingItem(placeSetting, accountSource, instrumentSource, tradePolicyDetailSource, dealingPolicyDetailSource);

		////volumeNecessarySetting
		auto volumeNecessarySetting = instrument->getOrAddVolumeNecessarySetting();
		QUuid volumeNecessaryId = tradePolicyDetailSource->getVolumeNecessaryId();
		if (!volumeNecessaryId.isNull())
		{
			auto volumeNecessarySource = this->_volumeNecessaries[volumeNecessaryId];
			volumeNecessarySetting->clearDetail();
			volumeNecessarySetting->setId(volumeNecessaryId);
			volumeNecessarySetting->setOption(volumeNecessarySource->getOption());

			foreach(auto volumeNecessaryDetailSource, this->_volumeNecessaryDetails)
			{
				if (volumeNecessaryDetailSource->getVolumeNecessaryId() == volumeNecessaryId)
				{
					auto volumeNecessaryDetail = volumeNecessarySetting->getOrAddVolumeNecessaryDetail(volumeNecessaryDetailSource->getId());
					volumeNecessaryDetail->setFrom(volumeNecessaryDetailSource->getFrom());
					volumeNecessaryDetail->setId(volumeNecessaryDetailSource->getId());
					volumeNecessaryDetail->setMarginD(volumeNecessaryDetailSource->getMarginD());
					volumeNecessaryDetail->setMarginO(volumeNecessaryDetailSource->getMarginO());
				}
			}

		}
		else
			volumeNecessarySetting.clear();


		////physicalPaymentDiscountSetting
		auto physicalPaymentDiscountSetting = instrument->getOrAddPhysicalPaymentDiscountSetting();
		QUuid physicalPaymentDiscountId = tradePolicyDetailSource->getPhysicalPaymentDiscountId();
		if (!physicalPaymentDiscountId.isNull())
		{
			auto physicalPaymentDiscountSource = this->_physicalPaymentDiscounts[physicalPaymentDiscountId];
			physicalPaymentDiscountSetting->clearDetail();
			physicalPaymentDiscountSetting->setId(physicalPaymentDiscountId);
			physicalPaymentDiscountSetting->setAllowedPaymentForm(physicalPaymentDiscountSource->getAllowedPaymentForm());
			physicalPaymentDiscountSetting->setCode(physicalPaymentDiscountSource->getCode());
			physicalPaymentDiscountSetting->setDiscountBasis(physicalPaymentDiscountSource->getDiscountBasis());
			physicalPaymentDiscountSetting->setDiscountOption(physicalPaymentDiscountSource->getOption());

			foreach(auto physicalPaymentDiscountDetailSource, this->_physicalPaymentDiscountDetails)
			{
				if (physicalPaymentDiscountDetailSource->getPhysicalPaymentDiscountId() == physicalPaymentDiscountId)
				{
					auto physicalPaymentDiscountDetail = physicalPaymentDiscountSetting->getOrAddPhysicalPaymentDiscountSettingDetail(physicalPaymentDiscountDetailSource->getId());
					physicalPaymentDiscountDetail->setId(physicalPaymentDiscountDetailSource->getId());
					physicalPaymentDiscountDetail->setPhysicalPaymentDiscountId(physicalPaymentDiscountId);
					physicalPaymentDiscountDetail->setDiscountValue(physicalPaymentDiscountDetailSource->getDiscountValue());
					physicalPaymentDiscountDetail->setFrom(physicalPaymentDiscountDetailSource->getFrom());
				}
			}

		}
		else
			physicalPaymentDiscountSetting.clear();


		////BOPolicySetting
		auto BOPolicySetting = instrument->getOrAddBOPolicySetting();
		QUuid BOPolicyId = tradePolicyDetailSource->getBOPolicyId();
		if (!BOPolicyId.isNull())
		{
			auto BOPolicySource = this->_BOPolicies[BOPolicyId];
			BOPolicySetting->clearDetail();
			BOPolicySetting->setId(BOPolicyId);
			BOPolicySetting->setCode(BOPolicySource->getCode());
			BOPolicySetting->setMaxOrderCount(BOPolicySource->getMaxOrderCount());
			BOPolicySetting->setTotalBetLimit(BOPolicySource->getTotalBetLimit());

			foreach(auto BOPolicyDetailSource, this->_BOPolicyDetails)
			{
				if (BOPolicyDetailSource->getBOPolicyId() == BOPolicyId)
				{
					QString key = QString("%1_%2").arg(BOPolicyDetailSource->getBOBetTypeId().toString().mid(1, 36), QString::number(BOPolicyDetailSource->getFrequency()));
					auto BOPolicyDetailSetting = BOPolicySetting->getOrAddBOPolicyDetail(key);
					BOPolicyDetailSetting->setBOBetTypeId(BOPolicyDetailSource->getBOBetTypeId());
					BOPolicyDetailSetting->setFrequency(BOPolicyDetailSource->getFrequency());
					BOPolicyDetailSetting->setOdds(BOPolicyDetailSource->getOdds());
					BOPolicyDetailSetting->setMaxOrderCount(BOPolicyDetailSource->getMaxOrderCount());
					BOPolicyDetailSetting->setTotalBetLimit(BOPolicyDetailSource->getTotalBetLimit());
					BOPolicyDetailSetting->setOddsDecimals(BOPolicyDetailSource->getOddsDecimals());

					auto BOBetTypeSource = this->_BOBetTypes[BOPolicyDetailSource->getBOBetTypeId()];
					BOPolicyDetailSetting->setOption(BOBetTypeSource->getOption());
					BOPolicyDetailSetting->setLastOrderTimeSpan(BOBetTypeSource->getLastOrderTimeSpan());

					QDecNumber maxBet = BOPolicyDetailSource->getMaxBet();
					QDecNumber minBet = BOPolicyDetailSource->getMinBet();
					QDecNumber stepBet = BOPolicyDetailSource->getStepBet();
					BOPolicyDetailSetting->setMax(maxBet);
					BOPolicyDetailSetting->setMin(minBet);
					BOPolicyDetailSetting->setStep(stepBet);
					BOPolicyDetailSetting->setDefault(minBet);
					int decimalPalces = 0;
					decimalPalces = qMax(decimalPalces, Common::getDecimalPlaces(minBet.toDouble()));
					decimalPalces = qMax(decimalPalces, Common::getDecimalPlaces(stepBet.toDouble()));
					decimalPalces = qMax(decimalPalces, Common::getDecimalPlaces(maxBet.toDouble()));
					BOPolicyDetailSetting->setDecimals(decimalPalces);
				}
			}

		}
		else
			BOPolicySetting.clear();

		////instalmentSetting
		auto instalmentSetting = instrument->getOrAddInstalmentSettingSetting();
		QUuid instalmentPolicyId = tradePolicyDetailSource->getInstalmentPolicyId();
		if (!instalmentPolicyId.isNull())
		{
			auto instalmentPolicySource = this->_instalmentPolicies[instalmentPolicyId];
			instalmentSetting->clearDetail();
			instalmentSetting->setId(instalmentPolicySource->getId());
			instalmentSetting->setCode(instalmentPolicySource->getCode());
			instalmentSetting->setAdvancePayment(instalmentPolicySource->getAdvancePayment());
			instalmentSetting->setAllowClose(instalmentPolicySource->getAllowClose());
			instalmentSetting->setIsDownPayAsFirstPay(instalmentPolicySource->getIsDownPayAsFirstPay());
			int allowedInstalmentTypes = instalmentPolicySource->getAllowedInstalmentTypes();
			instalmentSetting->setAllowEqualInstalment(Common::contains(allowedInstalmentTypes, (int)PhysicalInstalmentType::EqualInstalment));
			instalmentSetting->setAllowEqualPrincipal(Common::contains(allowedInstalmentTypes, (int)PhysicalInstalmentType::EqualPrincipal));
			int recalculateRateTypes = instalmentPolicySource->getRecalculateRateTypes();
			instalmentSetting->setAllowRecalucateNextMonth(Common::contains(recalculateRateTypes, (int)RecalculateRateType::RecalucateNextMonth));
			instalmentSetting->setAllowRecalucateNextYear(Common::contains(recalculateRateTypes, (int)RecalculateRateType::RecalucateNextYear));

			foreach(auto instalmentPolicyDetailSource, this->_instalmentPolicyDetails)
			{
                if (instalmentPolicyDetailSource->getInstalmentPolicyId() == instalmentPolicyId && instalmentPolicyDetailSource->getIsActive())
				{
					QSharedPointer<Setting::InstalmentSettingDetail> instalmentPolicyDetailSetting;
					if (instalmentPolicyDetailSource->getFrequence() == Frequence::Occasional)
						instalmentPolicyDetailSetting = instalmentSetting->getOrAddAdvancePaymentInstalmentSettingDetail();
					else
						instalmentPolicyDetailSetting = instalmentSetting->getOrAddInstalmentSettingDetail(instalmentPolicyDetailSource->getFrequence(), instalmentPolicyDetailSource->getPeriod());
					instalmentPolicyDetailSetting->setContractTerminateFee(instalmentPolicyDetailSource->getContractTerminateFee());
					instalmentPolicyDetailSetting->setContractTerminateType(instalmentPolicyDetailSource->getContractTerminateType());
                    instalmentPolicyDetailSetting->setAdministratioFee(instalmentPolicyDetailSource->getAdministrationFee());
                    instalmentPolicyDetailSetting->setAdministrationFeeBaseType(instalmentPolicyDetailSource->getAdministrationFeeBaseType());
					instalmentPolicyDetailSetting->setDownPaymentBasis(instalmentPolicyDetailSource->getDownPaymentBasis());
					instalmentPolicyDetailSetting->setFrequence(instalmentPolicyDetailSource->getFrequence());
					instalmentPolicyDetailSetting->setInstalmentPolicyId(instalmentPolicyDetailSource->getInstalmentPolicyId());
					instalmentPolicyDetailSetting->setInterestRate(instalmentPolicyDetailSource->getInterestRate());
					instalmentPolicyDetailSetting->setIsActive(instalmentPolicyDetailSource->getIsActive());
					instalmentPolicyDetailSetting->setMaxDownPayment(instalmentPolicyDetailSource->getMaxDownPayment());
					instalmentPolicyDetailSetting->setMinDownPayment(instalmentPolicyDetailSource->getMinDownPayment());
                    instalmentPolicyDetailSetting->setPeriod(instalmentPolicyDetailSource->getPeriod());
				}
			}

		}
		else
			instalmentSetting.clear();

		////LotSettings
		auto spotTradeOpenLotSetting = instrument->getOrAddLotSetting(true, OrderType::SpotTrade);
		auto spotTradeCloseLotSetting = instrument->getOrAddLotSetting(false, OrderType::SpotTrade);
		auto limitOpenLotSetting = instrument->getOrAddLotSetting(true, OrderType::Limit);
		auto limitCloseLotSetting = instrument->getOrAddLotSetting(false, OrderType::Limit);
		this->setLotSetting(spotTradeOpenLotSetting, accountSource, instrumentSource, tradePolicyDetailSource, dealingPolicyDetailSource);
		this->setLotSetting(spotTradeCloseLotSetting, accountSource, instrumentSource, tradePolicyDetailSource, dealingPolicyDetailSource);
		this->setLotSetting(limitOpenLotSetting, accountSource, instrumentSource, tradePolicyDetailSource, dealingPolicyDetailSource);
		this->setLotSetting(limitCloseLotSetting, accountSource, instrumentSource, tradePolicyDetailSource, dealingPolicyDetailSource);


        //deliverySpecificationDetails
		QUuid deliverySpecificationId = instrumentSource->getDeliverySpecificationId();
		if (!deliverySpecificationId.isNull())
		{
			foreach(auto deliverySpecificationDetailSource, this->_deliverySpecificationDetails)
			{
				if (deliverySpecificationDetailSource->getDeliverySpecificationId() == deliverySpecificationId)
				{
					auto deliverySpecificationDetail = instrument->getOrAddDeliverySpecificationDetail(deliverySpecificationDetailSource->getSize());
					deliverySpecificationDetail->setDeliveryRequestId(deliverySpecificationId);
                    deliverySpecificationDetail->setSize(deliverySpecificationDetailSource->getSize());
                    if(Common::TraderBaseInfo::getInstance()->getLanguage() == "ENG")
                        deliverySpecificationDetail->setProductName(deliverySpecificationDetailSource->getENG());
                    else if(Common::TraderBaseInfo::getInstance()->getLanguage() == "CHS")
                        deliverySpecificationDetail->setProductName(deliverySpecificationDetailSource->getCHS());
                    else if(Common::TraderBaseInfo::getInstance()->getLanguage() == "CHT")
                        deliverySpecificationDetail->setProductName(deliverySpecificationDetailSource->getCHT());
                    else if(Common::TraderBaseInfo::getInstance()->getLanguage() == "JPN")
                        deliverySpecificationDetail->setProductName(deliverySpecificationDetailSource->getJPN());
                    else if(Common::TraderBaseInfo::getInstance()->getLanguage() == "VN")
                        deliverySpecificationDetail->setProductName(deliverySpecificationDetailSource->getVN());
                    else if(Common::TraderBaseInfo::getInstance()->getLanguage() == "MAL")
                        deliverySpecificationDetail->setProductName(deliverySpecificationDetailSource->getMAL());
					deliverySpecificationDetail->setDeliveryCharge(deliverySpecificationDetailSource->getDeliveryCharge());
                    deliverySpecificationDetail->setPremium(deliverySpecificationDetailSource->getPremium());
				}
			}

		}
		else
			instrument->clearDeliverySpecificationDetails();


        //PricingPolicyDetails
        instrument->clearPricingPolicyDetails();
        instrumentSource->clearPricingPolicyDetails();
        if(!instrumentSource->getPricingPolicyId().isNull())
        {
            QUuid pricingPolicyId = instrumentSource->getPricingPolicyId();
            foreach(auto pricingPolicyDetailSource, this->_PricingPolicyDetails)
            {
                if(pricingPolicyId == pricingPolicyDetailSource->getPricingPolicyId())
                {
                    QSharedPointer<Setting::PricingPolicyDetail> settingPricingPolicyDetail(new Setting::PricingPolicyDetail);
                    settingPricingPolicyDetail->setId(pricingPolicyDetailSource->getId());
                    settingPricingPolicyDetail->setPricingPolicyId(pricingPolicyDetailSource->getPricingPolicyId());
                    settingPricingPolicyDetail->setPrice(pricingPolicyDetailSource->getPrice());
                    settingPricingPolicyDetail->setNumerator(pricingPolicyDetailSource->getNumerator());
                    settingPricingPolicyDetail->setSpreadFactor(pricingPolicyDetailSource->getSpreadFactor());
                    settingPricingPolicyDetail->setAutoPointFactor(pricingPolicyDetailSource->getAutoPointFactor());
                    settingPricingPolicyDetail->setOrderVariationFactor(pricingPolicyDetailSource->getOrderVariationFactor());
                    instrument->addPricingPolicyDetail(pricingPolicyDetailSource->getPrice(), settingPricingPolicyDetail);


                    instrumentSource->addPricingPolicyDetail(pricingPolicyDetailSource->getPrice(), pricingPolicyDetailSource);
                }
            }
        }


	}


    void ServerSourceManager::setCalculateSetting(QSharedPointer<Setting::CalculateSetting>& calculateSetting,
		QSharedPointer<Server::Account>& accountSource,
		QSharedPointer<Server::Instrument>& instrumentSource,
		QSharedPointer<Server::TradePolicyDetail>& tradePolicyDetailSource)
	{
		calculateSetting->setCaculateChangeWithDenominator(this->_systemParameter->getCaculateChangeWithDenominator());
		calculateSetting->setCommissionCloseD(tradePolicyDetailSource->getCommissionCloseD());
		calculateSetting->setCommissionCloseO(tradePolicyDetailSource->getCommissionCloseO());
		calculateSetting->setCommissionFormula(instrumentSource->getCommissionFormula());
		calculateSetting->setDenominator(instrumentSource->getDenominator());
		calculateSetting->setDiscountOfOdd(tradePolicyDetailSource->getDiscountOfOdd());
		calculateSetting->setIsCalculateFloat(this->_customer->getIsCalculateFloat());
		calculateSetting->setLevyClose(tradePolicyDetailSource->getLevyClose());
		calculateSetting->setLevyFormula(instrumentSource->getLevyFormula());
		calculateSetting->setMarginD(tradePolicyDetailSource->getMarginD());
		calculateSetting->setMarginFormula(instrumentSource->getMarginFormula());
		calculateSetting->setMarginLockedD(tradePolicyDetailSource->getMarginLockedD());
		calculateSetting->setMarginLockedO(tradePolicyDetailSource->getMarginLockedO());
		calculateSetting->setMarginO(tradePolicyDetailSource->getMarginO());
		calculateSetting->setMinCommissionClose(tradePolicyDetailSource->getMinCommissionClose());
		calculateSetting->setMinCommissionOpen(tradePolicyDetailSource->getMinCommissionOpen());
        calculateSetting->setCommissionOpen(tradePolicyDetailSource->getCommissionOpen());
		calculateSetting->setNecessaryRound(tradePolicyDetailSource->getNecessaryRound());
		calculateSetting->setNumeratorUnit(instrumentSource->getNumeratorUnit());
		calculateSetting->setPairRelationFactor(tradePolicyDetailSource->getPairRelationFactor());
		calculateSetting->setPartPaidPhysicalNecessary(tradePolicyDetailSource->getPartPaidPhysicalNecessary());
		calculateSetting->setPhysicalDeliveryIncremental(tradePolicyDetailSource->getPhysicalDeliveryIncremental());
		calculateSetting->setPhysicalMinDeliveryQuantity(tradePolicyDetailSource->getPhysicalMinDeliveryQuantity());
		calculateSetting->setRateCommission(accountSource->getRateCommission());
		calculateSetting->setRateLevy(accountSource->getRateLevy());
		calculateSetting->setRateMarginD(accountSource->getRateMarginD());
		calculateSetting->setRateMarginLockD(accountSource->getRateMarginLockD());
		calculateSetting->setRateMarginLockO(accountSource->getRateMarginLockO());
		calculateSetting->setRateMarginO(accountSource->getRateMarginO());
		calculateSetting->setTradePLFormula(instrumentSource->getTradePLFormula());
		calculateSetting->setUseNightNecessaryWhenBreak(this->_systemParameter->getUseNightNecessaryWhenBreak());
		calculateSetting->setValueDiscountAsMargin(tradePolicyDetailSource->getValueDiscountAsMargin());
	}

    void ServerSourceManager::setMovePipsInfoSetting(QSharedPointer<Setting::MovePipsInfoSetting>& movePipsInfoSetting,
		QSharedPointer<Server::Instrument>& instrumentSource,
		QSharedPointer<Server::TradePolicyDetail>& tradePolicyDetailSource)
    {
		int numeratorUnit = instrumentSource->getNumeratorUnit();
		int dQMaxMove = tradePolicyDetailSource->getDQMaxMove();
		int orderTypeMask = instrumentSource->getOrderTypeMask();

//        qDebug() << " Code:"<< instrumentSource->getCode() <<
//                    " numeratorUnit:"<< numeratorUnit<<
//                    " DQMaxMove:"<< dQMaxMove;

		if (dQMaxMove > 0 && dQMaxMove > numeratorUnit &&
			(Common::contains(orderTypeMask, (int)(OrderTypeMask::SpotTrade))))
		{
			movePipsInfoSetting->setMax(dQMaxMove);
			movePipsInfoSetting->setStep(numeratorUnit);
			movePipsInfoSetting->setDecimals(0);
			movePipsInfoSetting->setDefault(0);
			movePipsInfoSetting->setMin(0);
			movePipsInfoSetting->setIsMovePipsNeeded(true);
		}
		else
		{
			movePipsInfoSetting->unavailable();
		}
	}

    void ServerSourceManager::setPlaceSettingItem(
		QSharedPointer<Setting::PlaceSetting>& placeSetting,
		QSharedPointer<Server::Account>& accountSource,
		QSharedPointer<Server::Instrument>& instrumentSource,
		QSharedPointer<Server::TradePolicyDetail>& tradePolicyDetailSource,
		QSharedPointer<Server::DealingPolicyDetail>& dealingPolicyDetailSource)
	{
		placeSetting->setClosingUseCustomerQuotePolicy(this->_systemParameter->getClosingUseCustomerQuotePolicy());
		placeSetting->setContractSize(tradePolicyDetailSource->getContractSize());
		placeSetting->setIsAutoClose(accountSource->getIsAutoClose());
		placeSetting->setPriceValidTime(dealingPolicyDetailSource.isNull() ? instrumentSource->getPriceValidTime() : dealingPolicyDetailSource->getPriceValidTime());
		placeSetting->setQuoteTimeout(this->_systemParameter->getEnquiryOutTime());

		bool allowAddNewPosition = accountSource->getAllowAddNewPosition();
		AllowedNewTradeSides::AllowedNewTradeSides allowedNewTradeSides = dealingPolicyDetailSource.isNull() ?
			static_cast<AllowedNewTradeSides::AllowedNewTradeSides>(instrumentSource->getAllowedNewTradeSides()) :
			static_cast<AllowedNewTradeSides::AllowedNewTradeSides>(dealingPolicyDetailSource->getAllowedNewTradeSides());

		if (!allowAddNewPosition || allowedNewTradeSides == AllowedNewTradeSides::DisallowAll)
		{
			placeSetting->setAllowNewOpenBuy(false);
			placeSetting->setAllowNewOpenSell(false);
		}
		else
		{
			placeSetting->setAllowNewOpenBuy(true);
			placeSetting->setAllowNewOpenSell(true);
		}

		int paymentForm = tradePolicyDetailSource->getPaymentForm();
		placeSetting->setAllowFullPayment(Common::contains(paymentForm, (int)PaymentForm::FullPayment));
		placeSetting->setAllowAdvancePayment(Common::contains(paymentForm, (int)PaymentForm::AdvancePayment));
		placeSetting->setAllowInstalment(Common::contains(paymentForm, (int)PaymentForm::Instalment));

		int allowedPhysicalTradeSides = tradePolicyDetailSource->getAllowedPhysicalTradeSides();
		placeSetting->setAllowDelivery(Common::contains(allowedPhysicalTradeSides, (int)PhysicalTradeSide::Delivery));
		placeSetting->setAllowShortSell(Common::contains(allowedPhysicalTradeSides, (int)PhysicalTradeSide::ShortSell));


		bool disallowTrade = this->_customer->getDisallowTrade();
		bool isTradingAllowed = accountSource->getIsTradingAllowed();
		bool allowSalesTrading = accountSource->getAllowSalesTrading();
		QUuid salesID = accountSource->getSalesId();
		QUuid agentID = accountSource->getAgentId();
		bool allowManagerTrading = accountSource->getAllowManagerTrading();
		bool isTradeActive = tradePolicyDetailSource->getIsTradeActive();
		bool isActive = instrumentSource->getIsActive();
		QDateTime beginTime = accountSource->getBeginTime();
		QDateTime endTime = accountSource->getEndTime();
		QDateTime instrumentBeginTime = instrumentSource->getBeginTime();
		QDateTime instrumentEndTime = instrumentSource->getEndTime();
		QDateTime currentDate = Common::TraderBaseInfo::getInstance()->getUtcNow();
		QUuid customerId = this->_customer->getId();
		int userRelation = accountSource->getUserRelation();
		int orderTypeMask = instrumentSource->getOrderTypeMask();
		auto instrumentStatus = this->_settingManager->getInstrumentStatus(instrumentSource->getId());


		////------------------------------------------SpotTrade
		bool multipleCloseAllowed = false;
		bool isSpotTrade = false;
		bool isAllowMarket = false;
		if (!disallowTrade && isTradingAllowed && isTradeActive && isActive
			&& currentDate >= beginTime && currentDate <= endTime
			&& currentDate >= instrumentBeginTime && currentDate <= instrumentEndTime
			&& (customerId != salesID || (customerId == salesID && allowSalesTrading))
			&& (userRelation != 2 || (userRelation == 2 && allowManagerTrading)))
		{
			bool isSessionOpen = instrumentStatus.contains(InstrumentStatus::None) ? false :
				(instrumentStatus.contains(InstrumentStatus::SessionOpen) || instrumentStatus.contains(InstrumentStatus::DayOpen));

			QDecNumber maxDQLot = dealingPolicyDetailSource.isNull() ?
				instrumentSource->getMaxDQLot() :
				dealingPolicyDetailSource->getMaxDQLot();
			if (maxDQLot > 0.0 && isSessionOpen && Common::contains(orderTypeMask, (int)OrderTypeMask::SpotTrade))
			{
				isSpotTrade = true;
			}

			if (maxDQLot > 0.0 && isSessionOpen && Common::contains(orderTypeMask, (int)OrderTypeMask::Market))
			{
				isSpotTrade = true;
				isAllowMarket = true;
			}

			if (isSessionOpen && tradePolicyDetailSource->getMultipleCloseAllowed())
			{
				multipleCloseAllowed = true;
			}
		}
		auto spotTradePlaceSettingDetail = placeSetting->getOrAddPlaceSettingDetail(OrderType::SpotTrade);
		spotTradePlaceSettingDetail->setAllowTrade(isSpotTrade);
		spotTradePlaceSettingDetail->setAllowMarket(isAllowMarket);
		spotTradePlaceSettingDetail->setAllowMultipleClose(multipleCloseAllowed);
		spotTradePlaceSettingDetail->setDQQuoteMinLot(dealingPolicyDetailSource.isNull() ? instrumentSource->getDQQuoteMinLot() : dealingPolicyDetailSource->getDQQuoteMinLot());
		spotTradePlaceSettingDetail->setAcceptDQVariation(dealingPolicyDetailSource.isNull() ? instrumentSource->getAcceptDQVariation() : dealingPolicyDetailSource->getAcceptDQVariation());
        spotTradePlaceSettingDetail->setAllowIfDoneForDQ(tradePolicyDetailSource->getAllowIfDoneForDQ());
        if (dealingPolicyDetailSource.isNull())
            spotTradePlaceSettingDetail->setAcceptIfDoneVariation(instrumentSource->getAcceptIfDoneVariation());
        else
            spotTradePlaceSettingDetail->setAcceptIfDoneVariation(dealingPolicyDetailSource->getAcceptIfDoneVariation());


		////------------------------------------------Limit
		auto limitPlaceSettingDetail = placeSetting->getOrAddPlaceSettingDetail(OrderType::Limit);
		bool allowTrade = false;
		if (!disallowTrade && isTradingAllowed && isTradeActive && isActive
			&& currentDate >= beginTime && currentDate <= endTime
			&& currentDate >= instrumentBeginTime && currentDate <= instrumentEndTime
			&& (customerId != salesID || (customerId == salesID && allowSalesTrading))
			&& (userRelation != 2 || (userRelation == 2 && allowManagerTrading)))
		{

			bool isSessionOpen = instrumentStatus.contains(InstrumentStatus::None) ? false :
				(instrumentStatus.contains(InstrumentStatus::SessionOpen) || instrumentStatus.contains(InstrumentStatus::DayOpen));

			QDecNumber maxOtherLot = dealingPolicyDetailSource.isNull() ?
				instrumentSource->getMaxOtherLot() :
				dealingPolicyDetailSource->getMaxOtherLot();

			if (maxOtherLot > 0.0)
			{
				bool canPlacePendingOrderAtAnyTime = instrumentSource->getCanPlacePendingOrderAtAnyTime();
				if ((isSessionOpen || canPlacePendingOrderAtAnyTime))
				{
					allowTrade = true;
				}
				if ((isSessionOpen || canPlacePendingOrderAtAnyTime) && Common::contains(orderTypeMask, (int)OrderTypeMask::LimitStop))
					limitPlaceSettingDetail->setAllowLimitStop(true);
				else
					limitPlaceSettingDetail->setAllowLimitStop(false);
				if ((isSessionOpen || canPlacePendingOrderAtAnyTime) && Common::contains(orderTypeMask, (int)OrderTypeMask::MarketOnOpen))
					limitPlaceSettingDetail->setAllowMarketOnOpen(true);
				else
					limitPlaceSettingDetail->setAllowMarketOnOpen(false);
				if ((isSessionOpen || canPlacePendingOrderAtAnyTime) && Common::contains(orderTypeMask, (int)OrderTypeMask::MarketOnClose))
					limitPlaceSettingDetail->setAllowMarketOnClose(true);
				else
					limitPlaceSettingDetail->setAllowMarketOnClose(false);
				if ((isSessionOpen || canPlacePendingOrderAtAnyTime) && Common::contains(orderTypeMask, (int)OrderTypeMask::OneCancelOther))
					limitPlaceSettingDetail->setAllowOneCancelOther(true);
				else
					limitPlaceSettingDetail->setAllowOneCancelOther(false);
				if ((isSessionOpen || canPlacePendingOrderAtAnyTime) && tradePolicyDetailSource->getAllowIfDone())
					limitPlaceSettingDetail->setAllowIfDone(true);
				else
					limitPlaceSettingDetail->setAllowIfDone(false);
				if (isSessionOpen || canPlacePendingOrderAtAnyTime)
				{
					int isAcceptNewLimit = tradePolicyDetailSource->getIsAcceptNewLimit();
					//qDebug() << " Code:" << instrumentSource->getCode() <<" isAcceptNewLimit:" << isAcceptNewLimit;
					switch (isAcceptNewLimit)
					{
					case 0:
						limitPlaceSettingDetail->setAllowLimitAcceptHedge(true);
						limitPlaceSettingDetail->setAllowLimitAcceptNew(false);
						limitPlaceSettingDetail->setAllowLimitAcceptUnlock(false);
						break;
					case 1:
						limitPlaceSettingDetail->setAllowLimitAcceptHedge(true);
						limitPlaceSettingDetail->setAllowLimitAcceptNew(true);
						limitPlaceSettingDetail->setAllowLimitAcceptUnlock(true);
						break;
					case 2:
						limitPlaceSettingDetail->setAllowLimitAcceptUnlock(false);
						limitPlaceSettingDetail->setAllowLimitAcceptHedge(false);
						limitPlaceSettingDetail->setAllowLimitAcceptNew(false);
						break;
					case 3:
						limitPlaceSettingDetail->setAllowLimitAcceptUnlock(true);
						limitPlaceSettingDetail->setAllowLimitAcceptHedge(false);
						limitPlaceSettingDetail->setAllowLimitAcceptNew(false);
						break;
					default:
						break;
					}
					int isAcceptNewStop = tradePolicyDetailSource->getIsAcceptNewStop();
					//qDebug() << " Code:" << instrumentSource->getCode() << " isAcceptNewLimit:" << isAcceptNewLimit;
					switch (isAcceptNewStop)
					{
					case 0:
						limitPlaceSettingDetail->setAllowStopAcceptHedge(true);
						limitPlaceSettingDetail->setAllowStopAcceptNew(false);
						limitPlaceSettingDetail->setAllowStopAcceptUnlock(false);
						break;
					case 1:
						limitPlaceSettingDetail->setAllowStopAcceptHedge(true);
						limitPlaceSettingDetail->setAllowStopAcceptNew(true);
						limitPlaceSettingDetail->setAllowStopAcceptUnlock(true);
						break;
					case 2:
						limitPlaceSettingDetail->setAllowStopAcceptUnlock(false);
						limitPlaceSettingDetail->setAllowStopAcceptHedge(false);
						limitPlaceSettingDetail->setAllowStopAcceptNew(false);
						break;
					case 3:
						limitPlaceSettingDetail->setAllowStopAcceptUnlock(true);
						limitPlaceSettingDetail->setAllowStopAcceptHedge(false);
						limitPlaceSettingDetail->setAllowStopAcceptNew(false);
						break;
					default:
						break;
					}
				}
				else
				{
					limitPlaceSettingDetail->setAllowLimitAcceptHedge(false);
					limitPlaceSettingDetail->setAllowLimitAcceptNew(false);
					limitPlaceSettingDetail->setAllowLimitAcceptUnlock(false);
					limitPlaceSettingDetail->setAllowStopAcceptHedge(false);
					limitPlaceSettingDetail->setAllowStopAcceptNew(false);
					limitPlaceSettingDetail->setAllowStopAcceptUnlock(false);
				}
				if ((isSessionOpen || canPlacePendingOrderAtAnyTime) && tradePolicyDetailSource->getAllowNewLimitStopSameTime())
					limitPlaceSettingDetail->setAllowNewLimitStopSameTime(true);
				else
					limitPlaceSettingDetail->setAllowNewLimitStopSameTime(false);
				if ((isSessionOpen || canPlacePendingOrderAtAnyTime) && tradePolicyDetailSource->getAllowNewOCO())
					limitPlaceSettingDetail->setAllowNewOCO(true);
				else
					limitPlaceSettingDetail->setAllowNewOCO(false);
				if ((isSessionOpen || canPlacePendingOrderAtAnyTime) && tradePolicyDetailSource->getChangePlacedOrderAllowed())
					limitPlaceSettingDetail->setAllowChangePlacedOrder(true);
				else
					limitPlaceSettingDetail->setAllowChangePlacedOrder(false);
			}

			limitPlaceSettingDetail->clearExpireType();

            if (tradePolicyDetailSource->getGoodTillDate())
            {
                limitPlaceSettingDetail->setGoodTillDateIndex(tradePolicyDetailSource->getGoodTillDateIndex());
                limitPlaceSettingDetail->addExpireType(tradePolicyDetailSource->getGoodTillDateIndex(), PendingOrderExpireType::GoodTillDate);
            }
            if (tradePolicyDetailSource->getGoodTillMonthGTF())
            {
                limitPlaceSettingDetail->setGoodTillFridayIndex(tradePolicyDetailSource->getGoodTillFridayIndex());
                limitPlaceSettingDetail->addExpireType(tradePolicyDetailSource->getGoodTillFridayIndex(), PendingOrderExpireType::GoodTillFriday);
            }
            if (tradePolicyDetailSource->getGoodTillMonthGTM())
            {
                limitPlaceSettingDetail->setGoodTillMonthIndex(tradePolicyDetailSource->getGoodTillMonthIndex());
                limitPlaceSettingDetail->addExpireType(tradePolicyDetailSource->getGoodTillMonthIndex(), PendingOrderExpireType::GoodTillMonth);
            }
            if (tradePolicyDetailSource->getGoodTillMonthSession())
            {
                limitPlaceSettingDetail->setGoodTillSessionIndex(tradePolicyDetailSource->getGoodTillSessionIndex());
                limitPlaceSettingDetail->addExpireType(tradePolicyDetailSource->getGoodTillSessionIndex(), PendingOrderExpireType::GoodTillSession);
            }
            if (tradePolicyDetailSource->getGoodTillMonthDayOrder())
            {
                limitPlaceSettingDetail->setGoodTillDayIndex(tradePolicyDetailSource->getGoodTillDayIndex());
                limitPlaceSettingDetail->addExpireType(tradePolicyDetailSource->getGoodTillDayIndex(), PendingOrderExpireType::GoodTillDay);
            }
		}
		if (!instrumentStatus.contains(InstrumentStatus::None))
		{
			if (instrumentStatus.contains(InstrumentStatus::MOOClosed))
			{
				limitPlaceSettingDetail->setAllowMarketOnOpen(false);
			}
			if (instrumentStatus.contains(InstrumentStatus::MOCClosed))
			{
				limitPlaceSettingDetail->setAllowMarketOnClose(false);
			}
		}
		limitPlaceSettingDetail->setAllowTrade(allowTrade);
		limitPlaceSettingDetail->setIsAcceptNewMOOMOC(tradePolicyDetailSource->getIsAcceptNewMOOMOC());
		
		if (dealingPolicyDetailSource.isNull())
		{
			limitPlaceSettingDetail->setAcceptLmtVariation(instrumentSource->getAcceptLmtVariation());
			limitPlaceSettingDetail->setCancelLmtVariation(instrumentSource->getCancelLmtVariation());
			limitPlaceSettingDetail->setAcceptCloseLmtVariation(instrumentSource->getAcceptCloseLmtVariation());
			limitPlaceSettingDetail->setAcceptIfDoneVariation(instrumentSource->getAcceptIfDoneVariation());
		}
		else
		{
			limitPlaceSettingDetail->setAcceptLmtVariation(dealingPolicyDetailSource->getAcceptLmtVariation());
			limitPlaceSettingDetail->setCancelLmtVariation(dealingPolicyDetailSource->getCancelLmtVariation());
			limitPlaceSettingDetail->setAcceptCloseLmtVariation(dealingPolicyDetailSource->getAcceptCloseLmtVariation());
			limitPlaceSettingDetail->setAcceptIfDoneVariation(dealingPolicyDetailSource->getAcceptIfDoneVariation());
		}

		////------------------------------------------BinaryOption
		bool isBinaryOption = false;
		if (!disallowTrade && isTradingAllowed && isTradeActive && isActive
			&& currentDate >= beginTime && currentDate <= endTime
			&& currentDate >= instrumentBeginTime && currentDate <= instrumentEndTime
			&& (customerId != salesID || (customerId == salesID && allowSalesTrading))
			&& (userRelation != 2 || (userRelation == 2 && allowManagerTrading)))
		{
			bool isSessionOpen = instrumentStatus.contains(InstrumentStatus::None) ? false :
				(instrumentStatus.contains(InstrumentStatus::SessionOpen) || instrumentStatus.contains(InstrumentStatus::DayOpen));

			QDecNumber maxDQLot = dealingPolicyDetailSource.isNull() ?
				instrumentSource->getMaxDQLot() :
				dealingPolicyDetailSource->getMaxDQLot();
			if (maxDQLot > 0.0 && isSessionOpen && Common::contains(orderTypeMask, (int)OrderTypeMask::BinaryOption))
			{
				isBinaryOption = true;
			}
		}
		auto binaryOptionPlaceSettingDetail = placeSetting->getOrAddPlaceSettingDetail(OrderType::BinaryOption);
		binaryOptionPlaceSettingDetail->setAllowTrade(isBinaryOption);
	}

    void ServerSourceManager::setLotSetting(QSharedPointer<Setting::LotSetting>& lotSetting,
		QSharedPointer<Server::Account>& accountSource,
		QSharedPointer<Server::Instrument>& instrumentSource,
		QSharedPointer<Server::TradePolicyDetail>& tradePolicyDetailSource,
		QSharedPointer<Server::DealingPolicyDetail>& dealingPolicyDetailSource)
	{
		bool isOpen = lotSetting->getIsOpen();
		OrderType::OrderType orderType = lotSetting->getOrderType();

		QDecNumber step = isOpen ?
			accountSource->getRateLotMultiplier() * tradePolicyDetailSource->getOpenMultiplier() :
			accountSource->getRateLotMultiplier() * tradePolicyDetailSource->getCloseMultiplier();
		lotSetting->setStep(step);

		QDecNumber min = isOpen ?
			accountSource->getRateLotMin() * tradePolicyDetailSource->getMinOpen() :
			accountSource->getRateLotMin() * tradePolicyDetailSource->getMinClose();
		lotSetting->setMin(min);

		QDecNumber maxLot = dealingPolicyDetailSource.isNull() ? instrumentSource->getMaxDQLot() : dealingPolicyDetailSource->getMaxDQLot();
		QDecNumber maxOtherLot = dealingPolicyDetailSource.isNull() ? instrumentSource->getMaxOtherLot() : dealingPolicyDetailSource->getMaxOtherLot();
//        qDebug() << " setLotSetting Code" << instrumentSource->getCode() << " isOpen:" << isOpen  << " orderType:" << (orderType == OrderType::SpotTrade ? "0" : "1")
//			<< "  maxLot:" << instrumentSource->getMaxDQLot().toDouble()
//			<< "  MaxOtherLot:" << instrumentSource->getMaxOtherLot().toDouble()
//            << " step:" << step.toDouble()
//            << " min:" << min.toDouble();

		QDecNumber max = Common::integralMultiple((orderType == OrderType::SpotTrade ? maxLot : maxOtherLot), step);
//        qDebug() << "  max:" << max.toDouble();
		lotSetting->setMax(max);

        QDecNumber rateDefaultLot = accountSource->getRateDefaultLot();
        QDecNumber defaultLot = tradePolicyDetailSource->getDefaultLot();
        defaultLot = defaultLot * rateDefaultLot;;
        defaultLot = defaultLot > max ? max : defaultLot;
        defaultLot = defaultLot < min ? min : defaultLot;
        lotSetting->setDefault(defaultLot);

		InstrumentCategory::InstrumentCategory category = instrumentSource->getCategory();
		int physicalLotDecimal = instrumentSource->getPhysicalLotDecimal();
		int decimalPlaces = 0;
		decimalPlaces = qMax(decimalPlaces, Common::getDecimalPlaces(defaultLot.toDouble()));
		decimalPlaces = qMax(decimalPlaces, Common::getDecimalPlaces(step.toDouble()));
		decimalPlaces = qMax(decimalPlaces, Common::getDecimalPlaces(max.toDouble()));
		decimalPlaces = qMax(decimalPlaces, Common::getDecimalPlaces(min.toDouble()));
		if (category == InstrumentCategory::Physical)
			decimalPlaces = qMax(decimalPlaces, physicalLotDecimal);
		lotSetting->setDecimals(decimalPlaces);
	}

    void ServerSourceManager::setInstrumentQuotePolicyMappings(QXmlStreamReader & xmlStreamReader)
	{
		this->_settingManager->_instrumentQuotePolicyMappings.clear();
		this->_instrumentAndQuotePolicyIDs.clear();
		//<InstrumentQuotePolicyMappings>
		//    <Instrument Id=""> 
		//          <QuotePolicy Id="" Code="" />
		//          <QuotePolicy Id="" Code="" />
		//     </Instrument>
		//    <Instrument Id=""> 
		//          <QuotePolicy Id="" Code="" />
		//          <QuotePolicy Id="" Code="" />
		//     </Instrument>
		//</InstrumentQuotePolicyMappings>
        while (!xmlStreamReader.atEnd()) {
			if (xmlStreamReader.isEndElement()) {
				xmlStreamReader.readNext();//InstrumentQuotePolicyMappings end
				break;
			}
			if (xmlStreamReader.isStartElement()) {
                while (!xmlStreamReader.atEnd()) {
                    if (xmlStreamReader.isEndElement()) {
                        xmlStreamReader.readNext();//Instrument end
                        break;
                    }
					//InstrumentId1|QuotePolicyId1,QuotePolicyId1,QuotePolicyId1
					if (xmlStreamReader.name() == "Instrument") {

						QString idStr;
						QString instrumentIdStr = xmlStreamReader.attributes().value("Id").toString();
						QUuid instrumentId = QUuid(instrumentIdStr);
						idStr.append(instrumentIdStr);
						idStr.append("|");
						QMap<QUuid, QString> quotePolicys;

                        if (xmlStreamReader.isStartElement()) {
                            while (!xmlStreamReader.atEnd()) {
                                xmlStreamReader.readNext();
                                if (xmlStreamReader.name() != "QuotePolicy" && xmlStreamReader.name() !="")
                                {
                                    break;
                                }
                                if (xmlStreamReader.name() == "QuotePolicy") {

                                    QString quotePolicyIdStr = xmlStreamReader.attributes().value("Id").toString();
									if (quotePolicyIdStr.length() > 0)
									{
										idStr.append(quotePolicyIdStr);
										idStr.append(",");

										QUuid quotePolicyId = QUuid(quotePolicyIdStr);
										QString quotePolicyCode = xmlStreamReader.attributes().value("Code").toString();
										quotePolicys.insert(quotePolicyId, quotePolicyCode);
                                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << QString("ServerSourceManager::setInstrumentQuotePolicyMappings quotePolicyCode:%1").arg(quotePolicyCode);
									}                                    
                                }

                            }
                        }

                        this->_settingManager->_instrumentQuotePolicyMappings.insert(instrumentId, quotePolicys);
						this->_instrumentAndQuotePolicyIDs.append(idStr);
                    }
                    else
                        xmlStreamReader.readNext();
				}
            }
            else
                xmlStreamReader.readNext();
		}
	}

    void ServerSourceManager::setAccountForMarketingGroups(QXmlStreamReader & xmlStreamReader)
	{
		this->_settingManager->_marketingGroupFunds.clear();
		/*<AccountForMarketingGroups>
			<AccountForMarketingGroup SalesId = "QUuid" SalesCode = "QString" SalesName = "QString" ManagerCode = "QString">
				<Account Id = "QUuid" / >
			< / AccountForMarketingGroup>
		< / AccountForMarketingGroups>*/
		while (!xmlStreamReader.atEnd()) {
			if (xmlStreamReader.isEndElement()) {
				xmlStreamReader.readNext();//AccountForMarketingGroups end
				break;
			}
			if (xmlStreamReader.isStartElement()) {
				while (!xmlStreamReader.atEnd()) {
					//if (xmlStreamReader.isEndElement()) {
					//	xmlStreamReader.readNext();//AccountForMarketingGroup end
					//	break;
					//}
					QString name = xmlStreamReader.name().toString();
					if (name == "AccountForMarketingGroup")
					{
						QLOG_INFO(Common::Constants::LogKeyConstants::Business) << QString("ServerSourceManager::setAccountForMarketingGroups name:%1").arg(name);
						QSharedPointer<Trading::MarketingGroupFund> marketingGroupFund(new Trading::MarketingGroupFund);
						marketingGroupFund->update(xmlStreamReader);

						//use saleId + Account.CurrenyId group
						QList<QUuid> accountLists = marketingGroupFund->getAccountIds();
						marketingGroupFund->clearAccountList();
						foreach(QUuid accountId, accountLists) {
							QUuid currencyId = this->_accounts[accountId]->getCurrencyId();
							QString key = marketingGroupFund->getSalesId().toString() + currencyId.toString();
							if (this->_settingManager->_marketingGroupFunds.contains(key))
								this->_settingManager->_marketingGroupFunds[key]->addAccountId(accountId);
							else
							{
								QSharedPointer<Trading::MarketingGroupFund> newMarketingGroupFund(new Trading::MarketingGroupFund());
								newMarketingGroupFund->setSaleInfo(marketingGroupFund->getSalesId(),
									marketingGroupFund->getSalesCode(),
									marketingGroupFund->getSalesName(),
									marketingGroupFund->getManagerCode());
								newMarketingGroupFund->addAccountId(accountId);
								newMarketingGroupFund->setCurrencyId(currencyId);
								this->_settingManager->_marketingGroupFunds.insert(key, newMarketingGroupFund);
							}
						}
						break;
					}
					else
						xmlStreamReader.readNext();
				}
			}
			else
				xmlStreamReader.readNext();
		}
	}

    void ServerSourceManager::setPricingPolicyDetails(QXmlStreamReader& xmlStreamReader)
    {
        /*<PricingPolicyDetails>
            <PricingPolicyDetail ID = "" PricingPolicyId = "" Price = "" Numerator = "" SpreadFactor="" AutoPointFactor="" OrderVariationFactor=""/>
          </PricingPolicyDetails>*/
        while (!xmlStreamReader.atEnd()) {
            if (xmlStreamReader.isEndElement()) {
                xmlStreamReader.readNext();//PricingPolicyDetails end
                break;
            }
            if (xmlStreamReader.isStartElement()) {
                while (!xmlStreamReader.atEnd()) {
                    if (xmlStreamReader.isEndElement()) {
                        xmlStreamReader.readNext();//PricingPolicyDetail end
                        break;
                    }
                    QString name = xmlStreamReader.name().toString();
                    if (name == "PricingPolicyDetail")
                    {
                        QLOG_INFO(Common::Constants::LogKeyConstants::Business) << QString("ServerSourceManager::setPricingPolicyDetail name:%1").arg(name);
                        QUuid key = QUuid(xmlStreamReader.attributes().value("ID").toString());
                        QSharedPointer<Server::PricingPolicyDetail> sourceObject = this->getOrAddPricingPolicyDetail(key);
                        sourceObject->update(xmlStreamReader);
						xmlStreamReader.readNext();
                    }
                    else
                        xmlStreamReader.readNext();
                }
            }
            else
                xmlStreamReader.readNext();
        }
    }

	QSharedPointer<Server::DealingPolicyDetail> ServerSourceManager::getDealingPolicyDetail(const QUuid& dealingPolicyId, const QUuid& instrumentId)
	{
		if (dealingPolicyId.isNull())
		{
			QSharedPointer<Server::DealingPolicyDetail> nullObject(NULL);
			return nullObject;
		}
		QString key = QString("%1_%2").arg(dealingPolicyId.toString().mid(1, 36), instrumentId.toString().mid(1, 36));
		if (!this->_dealingPolicyDetails.contains(key))
		{
			QSharedPointer<Server::DealingPolicyDetail> nullObject(NULL);
			return nullObject;
		}
		return this->_dealingPolicyDetails[key];
	}
	
    QSharedPointer<Server::PricingPolicyDetail> ServerSourceManager::getPricingPolicyDetail(const QUuid& instrumentId, QDecNumber marketPrice)
    {
        QSharedPointer<Server::PricingPolicyDetail> pricingPolicyDetail;
        if (this->_instruments.contains(instrumentId) && this->_instruments[instrumentId]->isExistPricingPolicy())
                    return this->_instruments[instrumentId]->getPricingPolicyDetail(marketPrice);
        return pricingPolicyDetail;
    }




}
