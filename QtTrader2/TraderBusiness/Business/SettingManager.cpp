#include "SettingManager.h"
#include <qDebug>
#include "Common/LogHelper.h"
#include "Common/SignalForwarder.h"
#include "Common/GuidMappingManager.h"
#include "Common/Util/Convert_Common.h"
#include "Common/TraderBaseInfo.h"

namespace Business
{
	SettingManager::SettingManager()
	{
	}

	//---------------------------------------private method--------------------------------------------//
	void SettingManager::addAccount(const QUuid& id, const QSharedPointer<Setting::BusinessAccount>& acccount)
	{
		if (!this->_accounts.contains(id))
		{
			this->_accounts.insert(id, acccount);
		}
		else
		{
			this->_accounts[id] = acccount;
		}
	}

	QSharedPointer<Setting::SettingDataBase> SettingManager::getInstrumentChildItem(const QUuid& accountId, const QUuid& instrumentId, QString methodName)
	{
		QSharedPointer<Setting::BusinessAccount> account = this->getAccount(accountId);
		if (account.isNull())
		{
			QLOG_ERROR( Common::Constants::LogKeyConstants::Business) <<  QString("MethodName:SettingManager::getInstrumentChildItem:: %1 Info:account non-existent! id:%2").arg(methodName, accountId.toString());
			if (methodName == "getInterestRate")
			{
				QSharedPointer<Setting::InterestRate> newValue(Q_NULLPTR);
				return newValue;
			}
			else if (methodName == "getInstrument")
			{
				QSharedPointer<Setting::BusinessInstrument> newValue(Q_NULLPTR);
				return newValue;
			}
			else if (methodName == "getPrepaymentInterestRate")
			{
				QSharedPointer<Setting::InterestRate> newValue(Q_NULLPTR);
				return newValue;
			}
			else if (methodName == "getDeliveryCharge")
			{
				QSharedPointer<Setting::DeliveryCharge> newValue(Q_NULLPTR);
				return newValue;
			}
			else if (methodName == "getPlaceSetting")
			{
				QSharedPointer<Setting::PlaceSetting> newValue(Q_NULLPTR);
				return newValue;
			}
			else if (methodName == "getCalculateSetting")
			{
				QSharedPointer<Setting::CalculateSetting> newValue(Q_NULLPTR);
				return newValue;
			}
			else if (methodName == "getMovePipsInfoSetting")
			{
				QSharedPointer<Setting::MovePipsInfoSetting> newValue(Q_NULLPTR);
				return newValue;
			}
			else if (methodName == "getVolumeNecessary")
			{
				QSharedPointer<Setting::VolumeNecessary> newValue(Q_NULLPTR);
				return newValue;
			}
			else if (methodName == "getInstalmentSetting")
			{
				QSharedPointer<Setting::InstalmentSetting> newValue(Q_NULLPTR);
				return newValue;
			}

		}
		QSharedPointer<Setting::BusinessInstrument> instrument = account->getInstrument(instrumentId);
		if (instrument.isNull())
		{
			QLOG_ERROR( Common::Constants::LogKeyConstants::Business) <<  QString("MethodName:SettingManager::getInstrumentChildItem::%1 Info:Instrument non-existent! accountId:%2 instrumentId:%3").arg(methodName, accountId.toString(), instrumentId.toString());
			if (methodName == "getInterestRate")
			{
				QSharedPointer<Setting::InterestRate> newBusinessInstrument(Q_NULLPTR);
				return newBusinessInstrument;
			}
			else if (methodName == "getInstrument")
			{
				return instrument;
			}
			else if (methodName == "getPrepaymentInterestRate")
			{
				QSharedPointer<Setting::InterestRate> newValue(Q_NULLPTR);
				return newValue;
			}
			else if (methodName == "getDeliveryCharge")
			{
				QSharedPointer<Setting::DeliveryCharge> newValue(Q_NULLPTR);
				return newValue;
			}
			else if (methodName == "getPlaceSetting")
			{
				QSharedPointer<Setting::PlaceSetting> newValue(Q_NULLPTR);
				return newValue;
			}
			else if (methodName == "getCalculateSetting")
			{
				QSharedPointer<Setting::CalculateSetting> newValue(Q_NULLPTR);
				return newValue;
			}
			else if (methodName == "getMovePipsInfoSetting")
			{
				QSharedPointer<Setting::MovePipsInfoSetting> newValue(Q_NULLPTR);
				return newValue;
			}
			else if (methodName == "getVolumeNecessary")
			{
				QSharedPointer<Setting::VolumeNecessary> newValue(Q_NULLPTR);
				return newValue;
			}
			else if (methodName == "getInstalmentSetting")
			{
				QSharedPointer<Setting::InstalmentSetting> newValue(Q_NULLPTR);
				return newValue;
			}
		}

		if (methodName == "getInterestRate")
		{
			return instrument->getInterestRateSetting();
		}
		else if (methodName == "getInstrument")
		{
			return instrument;
		}
		else if (methodName == "getPrepaymentInterestRate")
		{
			return instrument->getPrepaymentInterestRateSetting();
		}
		else if (methodName == "getDeliveryCharge")
		{
			return instrument->getDeliveryCharge();
		}
		else if (methodName == "getPlaceSetting")
		{
			return instrument->getPlaceSetting();
		}
		else if (methodName == "getCalculateSetting")
		{
			return instrument->getCalculateSetting();
		}
		else if (methodName == "getMovePipsInfoSetting")
		{
			return instrument->getMovePipsInfoSetting();
		}
		else if (methodName == "getVolumeNecessary")
		{
			return instrument->getVolumeNecessary();
		}
		else if (methodName == "getInstalmentSetting")
		{
			return instrument->getInstalmentSetting();
		}

		return QSharedPointer<Setting::SettingDataBase>(Q_NULLPTR);
	}

	QSharedPointer<Setting::SettingDataBase> SettingManager::getAccountChildItem(const QUuid& accountId, QString methodName)
	{
		QSharedPointer<Setting::BusinessAccount> account = this->getAccount(accountId);
		if (account.isNull())
		{
			QLOG_ERROR( Common::Constants::LogKeyConstants::Business) <<  QString("MethodName:SettingManager::getAccountChildItem::%1 Info:account non-existent! id:%2").arg(methodName,accountId.toString());
			if (methodName == "getMarginSetting")
			{
				QSharedPointer<Setting::InterestRate> newBusinessInstrument(Q_NULLPTR);
				return newBusinessInstrument;
			}
			else if (methodName == "getLeverageSetting")
			{
				QSharedPointer<Setting::LeverageSetting> newValue(Q_NULLPTR);
				return newValue;
			}
		}
		if (methodName == "getMarginSetting")
		{
			return account->getMarginSetting();
		}
		else if (methodName == "getLeverageSetting")
		{
			return account->getLeverageSetting();
		}
		return QSharedPointer<Setting::SettingDataBase>(Q_NULLPTR);
	}

	//---------------------------------------public method--------------------------------------------//

	void SettingManager::updateSettingData(const QDomNode& xmlSettingData)
	{
		QDomNodeList childlist = xmlSettingData.childNodes();
		for (int i = 0; i < childlist.count(); i++)
		{
			QDomNode itemQDomNode = childlist.item(i);
			QString name = itemQDomNode.nodeName();
			if (name == "InstrumentStatuses")
			{
				//this->_instrumentStatus.clear();
				QDomNodeList mappingChildlist = itemQDomNode.childNodes();
				for (int j = 0; j < mappingChildlist.count(); j++)
				{
					QDomNode instrumentStatusesItemQDomNode = mappingChildlist.item(j);
					QString instrumentStatuItemName = instrumentStatusesItemQDomNode.nodeName();
					if (instrumentStatuItemName == "InstrumentStatus")
					{
						QUuid instrumentId = QUuid(instrumentStatusesItemQDomNode.toElement().attribute("Id"));
						QString statuses = instrumentStatusesItemQDomNode.toElement().attribute("Value");
						QList<InstrumentStatus::InstrumentStatus> instrumentStatusList;
						QStringList strList = statuses.split(";");
						foreach(QString item, strList)
						{
							if (item != "")
							{
								QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  QString("InstrumentStatuses Update: id:%1  statuses:%2").arg(instrumentId.toString(), statuses);
								instrumentStatusList.append(static_cast<InstrumentStatus::InstrumentStatus>(item.toInt()));
								Common::SignalForwarder::getInstance()->sendInstrumentStatusChangedSignal(instrumentId, item.toInt());
							}
						}
						if(this->_instrumentStatus.contains(instrumentId))
							this->_instrumentStatus[instrumentId] = instrumentStatusList;
						else
							this->_instrumentStatus.insert(instrumentId, instrumentStatusList);
					}
				}
			}
			else if (name == "Currencies")
			{
				QDomNodeList itemChildlist = itemQDomNode.childNodes();
				for (int j = 0; j < itemChildlist.count(); j++)
				{
					QDomNode itemChildQDomNode = itemChildlist.item(j);
					QString itemName = itemChildQDomNode.nodeName();
					if (itemName == "Currency")
					{
						QUuid id = QUuid(itemChildQDomNode.toElement().attribute("Id"));
						QSharedPointer<Setting::Currency> currency = this->getOrAddCurrency(id);
						currency->update(itemChildQDomNode);
					}
				}
			}
			else if (name == "CurrencyRates")
			{
				this->setCurrencyRate(itemQDomNode);
			}
			else if (name == "InterestRates")
			{
				QDomNodeList itemChildlist = itemQDomNode.childNodes();
                //int c = itemChildlist.count();
				for (int j = 0; j < itemChildlist.count(); j++)
				{
					QDomNode itemChildQDomNode = itemChildlist.item(j);
					QString itemName = itemChildQDomNode.nodeName();
					if (itemName == "InterestRate")
					{
						QUuid id = QUuid(itemChildQDomNode.toElement().attribute("InterestRateId"));
						if (!this->_interestRates.contains(id))
						{
							QSharedPointer<Setting::InterestRate> interestRate(new Setting::InterestRate);
							this->_interestRates.insert(id, interestRate);
						}						
						this->_interestRates[id]->update(itemChildQDomNode);
						
					}
				}
			}
		}
	}

	void SettingManager::setQuotePolicyAccountMappingAndAccountExpireNotify(bool isInit)
	{
		QString expiredAccountCodes = "";
		QString willExpireAccountCodes = "";
		QDateTime now = QDateTime::fromString(Common::TraderBaseInfo::getInstance()->getServerDateTime(), Common::Constants::DateTimeConstants::DateTimeFormat);
		foreach(auto account, this->_accounts)
		{
			QUuid accountId = account->getId();
			QString quotePolicyId = account->getQuotePolicyId().toString();
			QString publicQuotePolicyId = account->getPublicQuotePolicyId().toString();
			foreach(QUuid instrumentId, account->getInstrumentIds())
			{
				QString key = instrumentId.toString() + quotePolicyId;
				if (!this->_quotePolicyAccountMapping.contains(key))
				{
					this->_quotePolicyAccountMapping.insert(key, accountId);
					qDebug() << "setQuotePolicyAccountMappingAndAccountExpireNotify: key:" << key << " accountId:" << accountId;
				}
				QString publickey = instrumentId.toString() + publicQuotePolicyId;
				if (!this->_quotePolicyAccountMapping.contains(publickey))
				{
					this->_quotePolicyAccountMapping.insert(publickey, accountId);
					qDebug() << "setQuotePolicyAccountMappingAndAccountExpireNotify: publickey:" << publickey << " accountId:" << accountId;
				}
			}

			if (isInit)
			{
				if (now.daysTo(account->getEndTime()) == 1)
				{
					if (willExpireAccountCodes != "")
						willExpireAccountCodes.append(";");
					willExpireAccountCodes.append(account->getCode());
				}
				if (account->getEndTime() < now)
				{
					if (expiredAccountCodes != "")
						expiredAccountCodes.append(";");
					expiredAccountCodes.append(account->getCode());
				}
			}
		}

		if (isInit)
		{
			Common::SignalForwarder::getInstance()->sendAccoutWillExpireOrAccountExpireNotifySignal(willExpireAccountCodes, expiredAccountCodes);

			QList<QUuid> hitItems;
			QList<QUuid> expiredItems;
			foreach(QSharedPointer<PriceAlert> priceAlert, this->_priceAlerts.values())
			{
				if (priceAlert->getState() == Common::Enum::PriceAlertState::Hit || priceAlert->getHitPrice() != "")
				{
					hitItems.append(priceAlert->getId());
				}
			}
			if (hitItems.count() > 0)
			{
				Common::SignalForwarder::getInstance()->sendNotifyPriceAlertSignal(hitItems, expiredItems);
			}
		}
	}

	void SettingManager::sendUpdateQuotePolicy()
	{
		if (this->_accounts.contains(Common::TraderBaseInfo::getInstance()->getCurrentAccountId()))
		{
			auto currentAccount = this->_accounts[Common::TraderBaseInfo::getInstance()->getCurrentAccountId()];
			if (!currentAccount.isNull())
			{
				Common::TraderBaseInfo::getInstance()->setCurrentQuotePolicyId(currentAccount->getQuotePolicyId());
			}
			Common::SignalForwarder::getInstance()->sendRefreshQuotePolicySignal();
		}
	}

	QUuid SettingManager::getAccountIdByQuotePolicyId(QUuid quotePolicyId, QUuid instrumentId)
	{
		QString key = instrumentId.toString() + quotePolicyId.toString();
		if (this->_quotePolicyAccountMapping.contains(key))
		{
			return this->_quotePolicyAccountMapping.value(key);
		}
		else
			return QUuid();
	}
		
	void SettingManager::initSystemSetting(const QDomNode& xmlInitializeData)
	{
		QSharedPointer<Setting::SystemSetting>  systemSetting(new Setting::SystemSetting);
		this->_systemSetting = systemSetting;

		QDomNodeList childlist = xmlInitializeData.childNodes();
        //int count = childlist.count();
		for (int i = 0; i < childlist.count(); i++)
		{
			QDomNode itemQDomNode = childlist.item(i);
			QString name = itemQDomNode.nodeName();
			QString xmlvalue = itemQDomNode.nodeValue();
			if (name == "SystemSetting")
			{
				this->_systemSetting->update(itemQDomNode);
			}
			else if (name == "Currencies")
			{
				QDomNodeList itemChildlist = itemQDomNode.childNodes();
				for (int j = 0; j < itemChildlist.count(); j++)
				{
					QDomNode itemChildQDomNode = itemChildlist.item(j);
					QString itemName = itemChildQDomNode.nodeName();
					if (itemName == "Currency")
					{
						QUuid id = QUuid(itemChildQDomNode.toElement().attribute("Id"));
						QSharedPointer<Setting::Currency> currency = this->getOrAddCurrency(id);
						currency->update(itemChildQDomNode);
					}
				}
			}
			else if (name == "CurrencyRates")
			{
				this->setCurrencyRate(itemQDomNode);
			}
			else if (name == "InterestRates")
			{
				QDomNodeList itemChildlist = itemQDomNode.childNodes();
                //int c = itemChildlist.count();
				for (int j = 0; j < itemChildlist.count(); j++)
				{
					QDomNode itemChildQDomNode = itemChildlist.item(j);
					QString itemName = itemChildQDomNode.nodeName();
					if (itemName == "InterestRate")
					{
						QSharedPointer<Setting::InterestRate> interestRate(new Setting::InterestRate);
						interestRate->update(itemChildQDomNode);
						this->_interestRates.insert(interestRate->getInterestRateId(), interestRate);
					}
				}
			}
			else if (name == "ScrapInstruments")
			{
				QDomNodeList itemChildlist = itemQDomNode.childNodes();
				for (int j = 0; j < itemChildlist.count(); j++)
				{
					QDomNode itemChildQDomNode = itemChildlist.item(j);
					QString itemName = itemChildQDomNode.nodeName();
					if (itemName == "ScrapInstrument")
					{
						QUuid id = QUuid(itemChildQDomNode.toElement().attribute("Id"));
						QSharedPointer<Setting::ScrapInstrument> scrapInstrument = this->getOrAddScrapInstrument(id);
						scrapInstrument->update(itemChildQDomNode);
					}
				}
			}
			else if (name == "PaymentInstructionIntegrality")
			{
				if (this->_paymentInstructionIntegralitySetting.isNull())
				{
					QSharedPointer<Setting::PaymentInstructionIntegralitySetting> paymentInstructionIntegralitySetting(new Setting::PaymentInstructionIntegralitySetting);
					this->_paymentInstructionIntegralitySetting = paymentInstructionIntegralitySetting;
				}
				this->_paymentInstructionIntegralitySetting->update(itemQDomNode);
			}
			else if (name == "DeliveryHolidays")
			{
				this->_deliveryHolidays.clear();
				QDomNodeList itemChildlist = itemQDomNode.childNodes();
				for (int j = 0; j < itemChildlist.count(); j++)
				{
					QDomNode itemChildQDomNode = itemChildlist.item(j);
					QString itemName = itemChildQDomNode.nodeName();
					if (itemName == "DeliveryHoliday")
					{
						QSharedPointer<Setting::DeliveryHoliday> deliveryHoliday(new Setting::DeliveryHoliday);
						deliveryHoliday->update(itemChildQDomNode);
						this->_deliveryHolidays.append(deliveryHoliday);
					}
				}
			}
			
		}
	}

	bool SettingManager::containAccount(const QUuid& id)
	{
		return this->_accounts.contains(id);
	}

	QSharedPointer<Setting::BusinessAccount> SettingManager::getAccount(const QUuid& id) const
	{
		if (!this->_accounts.contains(id))
		{
			QLOG_ERROR( Common::Constants::LogKeyConstants::Business) <<  QString("MethodName:SettingManager::getAccount  Info:account non-existent! id:%1").arg(id.toString());
			QSharedPointer<Setting::BusinessAccount> newAccount(Q_NULLPTR);
			return newAccount;
		}
		else
			return this->_accounts.value(id);
	}

	QSharedPointer<Setting::BusinessAccount> SettingManager::getFirstAccount()
	{
		return this->_accounts.first();
	}

	QSharedPointer<Setting::MarginSetting> SettingManager::getMarginSetting(const QUuid& accountId)
	{
		return qSharedPointerCast<Setting::MarginSetting, Setting::SettingDataBase>(this->getAccountChildItem(accountId, "getMarginSetting"));
	}

	QSharedPointer<Setting::LeverageSetting> SettingManager::getLeverageSetting(const QUuid& accountId)
	{
		return qSharedPointerCast<Setting::LeverageSetting, Setting::SettingDataBase>(this->getAccountChildItem(accountId, "getLeverageSetting"));
	}

	QSharedPointer<Setting::BusinessInstrument> SettingManager::getInstrument(const QUuid& accountId, const QUuid& instrumentId)
	{
		auto instrument = this->getInstrumentChildItem(accountId, instrumentId, "getInstrument");
		if (!instrument.isNull())
		{
			return qSharedPointerCast<Setting::BusinessInstrument, Setting::SettingDataBase>(instrument);
		}
		QSharedPointer<Setting::BusinessInstrument> newValue(Q_NULLPTR);
		return newValue;
	}

	QSharedPointer<Setting::InterestRate> SettingManager::getInterestRate(const QUuid& accountId, const QUuid& instrumentId)
	{
		return qSharedPointerCast<Setting::InterestRate, Setting::SettingDataBase>(this->getInstrumentChildItem(accountId, instrumentId, "getInterestRate"));
	}

	QSharedPointer<Setting::InterestRate> SettingManager::getPrepaymentInterestRate(const QUuid& accountId, const QUuid& instrumentId)
	{
		return qSharedPointerCast<Setting::InterestRate, Setting::SettingDataBase>(this->getInstrumentChildItem(accountId, instrumentId, "getPrepaymentInterestRate"));
	}

	QSharedPointer<Setting::DeliveryCharge> SettingManager::getDeliveryCharge(const QUuid& accountId, const QUuid& instrumentId)
	{
		return qSharedPointerCast<Setting::DeliveryCharge, Setting::SettingDataBase>(this->getInstrumentChildItem(accountId, instrumentId, "getDeliveryCharge"));
	}

	QSharedPointer<Setting::PlaceSetting> SettingManager::getPlaceSetting(const QUuid& accountId, const QUuid& instrumentId)
	{
		return qSharedPointerCast<Setting::PlaceSetting, Setting::SettingDataBase>(this->getInstrumentChildItem(accountId, instrumentId, "getPlaceSetting"));
	}

	QSharedPointer<Setting::PlaceSettingDetail> SettingManager::getPlaceSettingDetail(const QUuid& accountId, const QUuid& instrumentId, Common::Enum::OrderType::OrderType orderType)
	{
		auto placeSetting = this->getPlaceSetting(accountId, instrumentId);
		if (placeSetting.isNull())
		{
			QLOG_ERROR( Common::Constants::LogKeyConstants::Business) <<  "MethodName:SettingManager::getPlaceSettingDetail  Info:PlaceSetting non-existent!";
			QSharedPointer<Setting::PlaceSettingDetail> newValue(Q_NULLPTR);
			return newValue;
		}
		return placeSetting->getPlaceSettingDetail(orderType);
	}

	QSharedPointer<Setting::CalculateSetting> SettingManager::getCalculateSetting(const QUuid& accountId, const QUuid& instrumentId)
	{
		return qSharedPointerCast<Setting::CalculateSetting, Setting::SettingDataBase>(this->getInstrumentChildItem(accountId, instrumentId, "getCalculateSetting"));
	}

	QSharedPointer<Setting::MovePipsInfoSetting> SettingManager::getMovePipsInfoSetting(const QUuid& accountId, const QUuid& instrumentId)
	{
		return qSharedPointerCast<Setting::MovePipsInfoSetting, Setting::SettingDataBase>(this->getInstrumentChildItem(accountId, instrumentId, "getMovePipsInfoSetting"));
	}

	QSharedPointer<Setting::VolumeNecessary> SettingManager::getVolumeNecessary(const QUuid& accountId, const QUuid& instrumentId)
	{
		return qSharedPointerCast<Setting::VolumeNecessary, Setting::SettingDataBase>(this->getInstrumentChildItem(accountId, instrumentId, "getVolumeNecessary"));
	}

	QSharedPointer<Setting::InstalmentSetting> SettingManager::getInstalmentSetting(const QUuid& accountId, const QUuid& instrumentId)
	{
		return qSharedPointerCast<Setting::InstalmentSetting, Setting::SettingDataBase>(this->getInstrumentChildItem(accountId, instrumentId, "getInstalmentSetting"));
	}

	QSharedPointer<Setting::SystemSetting> SettingManager::getSystemSetting()
	{
		return this->_systemSetting;
	}

	QList<QUuid> SettingManager::getInstrumentIds(const QUuid& accountId)
	{
		return this->_accounts.value(accountId)->getInstrumentIds();
	}

	QMap<QUuid, QSharedPointer<Common::Struct::SettingQuotePolicy>> SettingManager::getQuotePolicies(QList<QUuid> instrumentIds)
	{
		QMap<QUuid, QSharedPointer<Common::Struct::SettingQuotePolicy>> settingQuotePolicies;
		QMap<QUuid, QSharedPointer<Common::Struct::SettingQuotePolicy>> publicSettingQuotePolicies;
		if (this->_accounts.count() > 1 && Common::TraderBaseInfo::getInstance()->getCheckedAccountCount() > 1)
        {
            QList<QUuid> ids = Common::TraderBaseInfo::getInstance()->getCheckedAccounts();
			QList<QUuid> privateQuotePolicyIds;
			QUuid defaultQuotePolicyId;
			for (int i = 0; i < ids.length(); i++)
			{
				auto settingAccount = this->_accounts[ids[i]];
				if (!settingAccount.isNull() && !privateQuotePolicyIds.contains(settingAccount->getQuotePolicyId()))
                {
					privateQuotePolicyIds.append(settingAccount->getQuotePolicyId());
					defaultQuotePolicyId = settingAccount->getPublicQuotePolicyId();
				}
			}


            QMap<QUuid, QMap<QUuid, QString>>::const_iterator instrumentItem = this->_instrumentQuotePolicyMappings.constBegin();
			while (instrumentItem != this->_instrumentQuotePolicyMappings.constEnd()) {
				if (instrumentIds.contains(instrumentItem.key()))
				{                    
					if (instrumentItem.value().count() > 0)
					{
						QMap<QUuid, QString>::const_iterator quotePolicyItem = instrumentItem.value().constBegin();
						while (quotePolicyItem != instrumentItem.value().constEnd()) {
							if (!settingQuotePolicies.contains(quotePolicyItem.key()))
                            {
								if (privateQuotePolicyIds.contains(quotePolicyItem.key()))
								{
									QSharedPointer<Common::Struct::SettingQuotePolicy> settingQuotePolicy(new Common::Struct::SettingQuotePolicy);
									settingQuotePolicy->_id = quotePolicyItem.key();
									settingQuotePolicy->_code = quotePolicyItem.value();
                                    settingQuotePolicies.insert(settingQuotePolicy->_id, settingQuotePolicy);
								}
								else
								{
									if (!publicSettingQuotePolicies.contains(defaultQuotePolicyId) && defaultQuotePolicyId == quotePolicyItem.key())
									{
										QSharedPointer<Common::Struct::SettingQuotePolicy> settingQuotePolicy(new Common::Struct::SettingQuotePolicy);
										settingQuotePolicy->_id = quotePolicyItem.key();
										settingQuotePolicy->_code = quotePolicyItem.value();
                                        publicSettingQuotePolicies.insert(settingQuotePolicy->_id, settingQuotePolicy);
									}
								}
							}
							++quotePolicyItem;
						}
					}
				}
				++instrumentItem;
			}
		}
		return settingQuotePolicies.count() > 0 ? settingQuotePolicies : publicSettingQuotePolicies;
	}

	QList<QSharedPointer<Common::Struct::SettingQuotePolicy>> SettingManager::getQuotePolicies(QUuid instrumentId)
	{
		QList<QSharedPointer<Common::Struct::SettingQuotePolicy>> settingQuotePolicies;
		QMap<QUuid, QSharedPointer<Common::Struct::SettingQuotePolicy>> publicSettingQuotePolicies;
		QList<QUuid> ids = Common::TraderBaseInfo::getInstance()->getTotalAccounts();
		QList<QUuid> privateQuotePolicyIds;
		QUuid defaultQuotePolicyId;
		for (int i = 0; i < ids.length(); i++)
		{
			auto settingAccount = this->_accounts[ids[i]];
			if (!settingAccount.isNull() && !privateQuotePolicyIds.contains(settingAccount->getQuotePolicyId()))
			{
				privateQuotePolicyIds.append(settingAccount->getQuotePolicyId());
				defaultQuotePolicyId = settingAccount->getPublicQuotePolicyId();
			}
		}

		QMap<QUuid, QMap<QUuid, QString>>::const_iterator instrumentItem = this->_instrumentQuotePolicyMappings.constBegin();
		while (instrumentItem != this->_instrumentQuotePolicyMappings.constEnd()) {
			if (instrumentItem.key() == instrumentId)
			{
				QMap<QUuid, QString>::const_iterator quotePolicyItem = instrumentItem.value().constBegin();
				while (quotePolicyItem != instrumentItem.value().constEnd()) {
					if (privateQuotePolicyIds.contains(quotePolicyItem.key()))
					{
						QSharedPointer<Common::Struct::SettingQuotePolicy> settingQuotePolicy(new Common::Struct::SettingQuotePolicy);
						settingQuotePolicy->_id = quotePolicyItem.key();
						settingQuotePolicy->_code = quotePolicyItem.value();
						settingQuotePolicies.append(settingQuotePolicy);
					}
					else
					{
						//if (!publicSettingQuotePolicies.contains(defaultQuotePolicyId) && defaultQuotePolicyId == quotePolicyItem.key())
						if (!publicSettingQuotePolicies.contains(defaultQuotePolicyId))
						{
							QSharedPointer<Common::Struct::SettingQuotePolicy> settingQuotePolicy(new Common::Struct::SettingQuotePolicy);
							settingQuotePolicy->_id = quotePolicyItem.key();
							settingQuotePolicy->_code = quotePolicyItem.value();
							publicSettingQuotePolicies.insert(settingQuotePolicy->_id, settingQuotePolicy);
						}
					}
					++quotePolicyItem;
				}
			}
			++instrumentItem;
		}
		if (settingQuotePolicies.count() > 0)
			return settingQuotePolicies;
		else
		{
			if (publicSettingQuotePolicies.count() > 0)
				return publicSettingQuotePolicies.values();
			else
			{
				QSharedPointer<Common::Struct::SettingQuotePolicy> settingQuotePolicy(new Common::Struct::SettingQuotePolicy);
				settingQuotePolicy->_id = defaultQuotePolicyId;				
				publicSettingQuotePolicies.insert(settingQuotePolicy->_id, settingQuotePolicy);
				return publicSettingQuotePolicies.values();
			}
		}
	}

	bool SettingManager::isAllowChangePassword()
	{
		bool isAllow = true;
		foreach(auto account, this->_accounts)
		{
			if (!account->getMarginSetting()->getAllowChangePasswordInTrader())
			{
				isAllow = false;
				break;
			}
		}
		return isAllow;
	}

	QList<QSharedPointer<Common::Struct::SettingAccount>> SettingManager::getAccountsByMarginPin()
	{
		QList<QSharedPointer<Common::Struct::SettingAccount>> settingAccounts;
		foreach(auto account, this->_accounts)
		{
			if (account->getEnableMarginPin())
			{
				QSharedPointer<Common::Struct::SettingAccount> settingAccount(new Common::Struct::SettingAccount);
				settingAccount->_id = account->getId();
				settingAccount->_code = account->getCode();
				settingAccounts.append(settingAccount);
			}
		}
		return settingAccounts;
	}

	QList<QSharedPointer<Common::Struct::SettingAccount>> SettingManager::getAccountsByModifyTelephoneIdentificationCode()
	{
		QList<QSharedPointer<Common::Struct::SettingAccount>> settingAccounts;
		foreach(auto account, this->_accounts)
		{
			if (account->getEnableModifyTelephoneIdentificationCode())
			{
				QSharedPointer<Common::Struct::SettingAccount> settingAccount(new Common::Struct::SettingAccount);
				settingAccount->_id = account->getId();
				settingAccount->_code = account->getCode();
				settingAccounts.append(settingAccount);
			}
		}
		return settingAccounts;
	}


	bool SettingManager::getNeedDeal()
	{
		if (this->getAccountCount() > 1)
			return this->_systemSetting->getMultiAccountsOrderType() == Common::Enum::AccountOrderType::DealWhilePlacing;
		else
			return this->_systemSetting->getSingleAccountOrderType() == Common::Enum::AccountOrderType::DealWhilePlacing;
	}

	QSharedPointer<Setting::TradeDay>  SettingManager::getTradeDay()
	{
		return this->_tradeDay;
	}

	QSharedPointer<Setting::InstrumentParameter>  SettingManager::getInstrumentParameter(QUuid instrumentId)
	{
		return this->_instrumentParameters.value(instrumentId);
	}

	QList<InstrumentStatus::InstrumentStatus>  SettingManager::getInstrumentStatus(QUuid instrumentId)
	{
		return this->_instrumentStatus.value(instrumentId);
	}
	
	void SettingManager::updateChatOrNews(const QDomNode& xmlInitializeData)
	{
		QString name = xmlInitializeData.nodeName();
		if (name == "Messages" || name == "Newses")
		{
			if (name == "Messages") Common::SignalForwarder::getInstance()->sendPlaySoundsSignal(Common::Enum::SoundsEnum::News);
			QDomNodeList itemChildlist = xmlInitializeData.childNodes();
			for (int j = 0; j < itemChildlist.count(); j++)
			{
				QDomNode itemChildQDomNode = itemChildlist.item(j);
				QString itemName = itemChildQDomNode.nodeName();
				if (itemName == "Message")
				{
					QSharedPointer<Model::Chat> chat(new Model::Chat());
					chat->update(itemChildQDomNode);
					this->_chats.insert(chat->getId(), chat);
					Common::SignalForwarder::getInstance()->sendAddNewsOrChatSignal(chat->getId(), Common::Enum::InformationCenterEnum::Chat);
					InformationStruct informationStruct;
					informationStruct._id = chat->getId();
					informationStruct._title = chat->getTitle();
					informationStruct._informationCenterEnum = InformationCenterEnum::Chat;
					informationStruct._isRead = chat->getIsRead();
					if (this->_informationStructs.count() > 4)
					{
						this->_informationStructs.removeFirst();
						this->_informationStructs.append(informationStruct);
					}
					else
					{
						this->_informationStructs.append(informationStruct);
					}

				}
				else if (itemName == "News")
				{
					QSharedPointer<Model::News> news(new Model::News());
					news->update(itemChildQDomNode);
					this->_newses.insert(news->getId(), news);
					Common::SignalForwarder::getInstance()->sendAddNewsOrChatSignal(news->getId(), Common::Enum::InformationCenterEnum::News);
					InformationStruct informationStruct;
					informationStruct._id = news->getId();
					informationStruct._title = news->getTitle();
					informationStruct._informationCenterEnum = InformationCenterEnum::News;
					informationStruct._isRead = news->getIsRead();
					if (this->_informationStructs.count() > 4)
					{
						this->_informationStructs.removeFirst();
						this->_informationStructs.append(informationStruct);
					}
					else
					{
						this->_informationStructs.append(informationStruct);
					}
				}
			}
		}
	}

	void SettingManager::updatePriceAlerts(const QDomNode& xmlInitializeData)
	{
		QString name = xmlInitializeData.nodeName();
		if (name == "PriceAlerts")
		{
			QList<QUuid> hitItems;
			QList<QUuid> expiredItems;
			QDomNodeList itemChildlist = xmlInitializeData.childNodes();
			for (int j = 0; j < itemChildlist.count(); j++)
			{
				QDomNode itemChildQDomNode = itemChildlist.item(j);
				QString itemName = itemChildQDomNode.nodeName();
				if (itemName == "PriceAlert")
				{
					QUuid id = QUuid(itemChildQDomNode.toElement().attribute("Id"));
					if (this->_priceAlerts.contains(id))
					{
						this->_priceAlerts[id]->update(itemChildQDomNode);
						auto priceAlert = this->_priceAlerts.value(id);
						if (priceAlert->getState() == Common::Enum::PriceAlertState::Hit || priceAlert->getHitPrice() != "")
						{
							hitItems.append(id);
						}
						else
						{
							expiredItems.append(id);
						}
					}
				}
			}

			if (hitItems.count() > 0 || expiredItems.count() > 0)
			{
				Common::SignalForwarder::getInstance()->sendNotifyPriceAlertSignal(hitItems, expiredItems);

			}

			if (hitItems.count() > 0)
				Common::SignalForwarder::getInstance()->sendPlaySoundsSignal(Common::Enum::SoundsEnum::Alert);
		}
	}

	void SettingManager::addLog(QUuid id, const QDomNode& xmlNotifyData)
	{
		QSharedPointer<Model::Log> log(new Model::Log());
		log->setId(id);
		log->update(xmlNotifyData);
		this->_logs.insert(id, log);
	}

    void SettingManager::addQtAppLog(QUuid id, const QDomNode& xmlNotifyData)
    {
        QSharedPointer<Model::Log> log(new Model::Log());
        log->setId(id);
        log->update(xmlNotifyData);
        this->_qtAppLogs.insert(id, log);
    }

    bool SettingManager::isContainLog(QUuid id, const QDomNode& xmlNotifyData, bool isQtApp)
    {
        QSharedPointer<Model::Log> log(new Model::Log());
        log->setId(id);
        log->update(xmlNotifyData);
        bool isContain = false;
        if(isQtApp)
        {
            foreach (auto logItem, _qtAppLogs.values()) {
                if(logItem->getAction() == log->getAction())
                {
                    isContain = true;
                    break;
                }
            }
        }
        else
        {
            foreach (auto logItem, _logs.values()) {
                if(logItem->getAction() == log->getAction())
                {
                    isContain = true;
                    break;
                }
            }
        }
        return isContain;
    }

	QList<QSharedPointer<Model::Chat>> SettingManager::getChats()
	{
		return this->_chats.values();
	}

	QList<QSharedPointer<Model::News>> SettingManager::getNewses()
	{
		return this->_newses.values();
	}

	void SettingManager::setIsRead(QUuid id, bool value, InformationCenterEnum::InformationCenterEnum informationCenterEnum)
	{
		if (informationCenterEnum == InformationCenterEnum::News)
		{
			this->_newses[id]->setIsRead(value);
		}
		else if (informationCenterEnum == InformationCenterEnum::Chat)
		{
			this->_chats[id]->setIsRead(value);
		}
		for (int i = 0; i < this->_informationStructs.count(); i++)
		{
			if (this->_informationStructs[i]._id == id)
			{
				this->_informationStructs[i]._isRead = value;
			}
		}
	}

	void SettingManager::setContent(QUuid id, QString value, InformationCenterEnum::InformationCenterEnum informationCenterEnum)
	{
		if (informationCenterEnum == InformationCenterEnum::News)
		{
			this->_newses[id]->setContent(value);
		}
		else if (informationCenterEnum == InformationCenterEnum::Chat)
		{
			this->_chats[id]->setContent(value);
		}
	}

	void SettingManager::removeNewsOrChats(QUuid id, InformationCenterEnum::InformationCenterEnum informationCenterEnum)
	{
		if (informationCenterEnum == InformationCenterEnum::News)
		{
			this->_newses.remove(id);
		}
		else if (informationCenterEnum == InformationCenterEnum::Chat)
		{
			this->_chats.remove(id);
		}
	}

	QSharedPointer<Model::Chat> SettingManager::getChat(QUuid id)
	{
		if (this->_chats.contains(id))
		{
			return this->_chats.value(id);
		}
		return QSharedPointer<Model::Chat>(Q_NULLPTR);
	}

	QSharedPointer<Model::News> SettingManager::getNews(QUuid id)
	{
		if (this->_newses.contains(id))
		{
			return this->_newses.value(id);
		}
		return QSharedPointer<Model::News>(Q_NULLPTR);
	}

	QList<QSharedPointer<Model::PriceAlert>> SettingManager::getPriceAlerts()
	{
		return this->_priceAlerts.values();
	}

	QList<QSharedPointer<Model::PriceAlert>> SettingManager::getPriceAlerts(QList<QUuid> ids)
	{
		QList<QSharedPointer<Model::PriceAlert>> priceAlertList;
		foreach(QUuid id, ids)
		{
			if (this->_priceAlerts.contains(id))
			{
				priceAlertList.append(this->_priceAlerts.value(id));
			}
		}
		return priceAlertList;
	}

	QSharedPointer<Model::PriceAlert> SettingManager::getPriceAlert(QUuid id)
	{
		if (this->_priceAlerts.contains(id))
		{
			return this->_priceAlerts.value(id);
		}
		return QSharedPointer<Model::PriceAlert>(Q_NULLPTR);
	}

	void SettingManager::deletePriceAlert(QUuid id)
	{
		if (this->_priceAlerts.contains(id))
		{
			this->_priceAlerts.remove(id);
		}
	}

	void SettingManager::deletePriceAlerts(QList<QUuid> ids)
	{
		foreach(QUuid id, ids)
		{
			if (this->_priceAlerts.contains(id))
			{
				this->_priceAlerts.remove(id);
			}
		}
	}

	void SettingManager::updatePriceAlert(QUuid id, QSharedPointer<Model::PriceAlert> priceAlert)
	{
		if (this->_priceAlerts.contains(id))
		{
			this->_priceAlerts[id]->updatePriceAlert(priceAlert);
		}
	}

	void SettingManager::addPriceAlert(QUuid id, QSharedPointer<Model::PriceAlert> priceAlert)
	{
		if (this->_priceAlerts.contains(id))
		{
			this->_priceAlerts[id]->updatePriceAlert(priceAlert);
		}
		else
		{
			this->_priceAlerts.insert(id, priceAlert);
		}
	}

	int SettingManager::getPriceAlertCountByInstrument(QUuid instrumentId)
	{
		int count = 0;
		foreach(auto item, this->_priceAlerts.values())
		{
			if (item->getInstrumentId() == instrumentId)
			{
				count++;
			}
		}
		return count;
	}

	bool SettingManager::hasDuplicatedPriceAlert(QUuid instrumentId, int condition, QString Price, QUuid id)
	{
		bool hasDuplicated = false;
		foreach(auto item, this->_priceAlerts.values())
		{
			if (item->getInstrumentId() == instrumentId
				&& item->getCondition() == condition
				&& item->getPrice() == Price
				&& item->getId() != id)
			{
				hasDuplicated = true;
			}
		}
		return hasDuplicated;
	}

	void SettingManager::changeAlertPrice(QUuid id, QString price)
	{
		this->_priceAlerts[id]->setPrice(price);
	}

	QSharedPointer<Model::Log> SettingManager::getLog(QUuid id)
	{
		if (this->_logs.contains(id))
		{
			return this->_logs.value(id);
		}
		return QSharedPointer<Model::Log>(Q_NULLPTR);
	}

	QList<QSharedPointer<Model::Log>> SettingManager::getLogs()
	{
		return this->_logs.values();
	}

    QSharedPointer<Model::Log> SettingManager::getQtAppLog(QUuid id)
    {
        if (this->_qtAppLogs.contains(id))
        {
            return this->_qtAppLogs.value(id);
        }
        return QSharedPointer<Model::Log>(Q_NULLPTR);
    }

    QList<QSharedPointer<Model::Log>> SettingManager::getQtAppLogs()
    {
        return this->_qtAppLogs.values();
    }

    void SettingManager::deleteLog(QUuid id)
    {
        if(this->_logs.contains(id))
            this->_logs.remove(id);
        if(this->_qtAppLogs.contains(id))
            this->_qtAppLogs.remove(id);
    }

	void SettingManager::deleteInformationStruct(QUuid id)
	{
		for (int i = this->_informationStructs.count() - 1; i >= 0; i--)
		{
			if (this->_informationStructs[i]._id == id)
			{
				this->_informationStructs.removeAt(i);
			}
		}
	}


	bool SettingManager::isShowSptReconfirm()
	{
		int sptReconfirmDuration = this->_systemSetting->getSptReconfirmDuration();
		bool hasZeroPriceValidTime = false;
		foreach(auto account, this->_accounts)
		{
			foreach(auto instrument, account->getInstruments())
			{
				int priceValidTime = instrument->getPlaceSetting()->getPriceValidTime();
				if (priceValidTime == 0)
				{
					hasZeroPriceValidTime = true;
					break;
				}
			}
		}
        return hasZeroPriceValidTime && sptReconfirmDuration > 0 && this->_systemSetting->getAllowTraderDQReconfirm();
	}

	bool SettingManager::isExistQuotePolicyInInstrumentMapping(QUuid instrumentId, QUuid quotePolicyId)
	{
		if (this->_instrumentQuotePolicyMappings.contains(instrumentId))
		{
			if (this->_instrumentQuotePolicyMappings[instrumentId].contains(quotePolicyId))
				return true;
		}
		return false;
	}

	bool SettingManager::isExistQuotePolicyAccountMapping(QUuid instrumentId, QUuid quotePolicyId)
	{
		QString key = instrumentId.toString() + quotePolicyId.toString();		
		return this->_quotePolicyAccountMapping.contains(key);
	}

	bool SettingManager::setCurrentAccountAndQuotePolicyId()
	{
		// set Current QuotePolicyId
		auto currentAccount = this->getAccount(Common::TraderBaseInfo::getInstance()->getCurrentAccountId());
		if (!currentAccount.isNull())
		{
			Common::TraderBaseInfo::getInstance()->setCurrentQuotePolicyId(currentAccount->getQuotePolicyId());
		}
		else
		{
			auto firstAccount = this->_accounts.first();
			if (firstAccount.isNull())
			{
				return false;
			}
			Common::TraderBaseInfo::getInstance()->setCurrentAccountId(firstAccount->getId());
			Common::TraderBaseInfo::getInstance()->setCurrentQuotePolicyId(firstAccount->getQuotePolicyId());
		}
		return true;
	}

	void SettingManager::updateLeverage(QUuid accountId, int leverage)
	{
		if (this->_accounts.contains(accountId))
		{
			this->_accounts[accountId]->updateLeverage(leverage);
		}
	}

	bool SettingManager::isMutilQuotePolicy()
	{
		QList<QUuid> accountIdList;
		QMapIterator<QUuid, QSharedPointer<Setting::BusinessAccount>> accountItem(this->_accounts);
		while (accountItem.hasNext())
		{
			accountItem.next();
			QUuid quotePolicyId = accountItem.value()->getQuotePolicyId();
			if (!accountIdList.contains(quotePolicyId))
			{
				accountIdList.append(quotePolicyId);
			}
		}
		return accountIdList.count() > 1;
	}

	bool SettingManager::isContainQuotePolicy(QUuid quotePolicyId)
	{

		QMapIterator<QUuid, QSharedPointer<Setting::BusinessAccount>> accountItem(this->_accounts);
		while (accountItem.hasNext())
		{
			accountItem.next();
			if (quotePolicyId == accountItem.value()->getQuotePolicyId()) return true;
		}
		return false;

	}

	bool SettingManager::hasConsumeCode()
	{
		QMapIterator<QUuid, QSharedPointer<Setting::BusinessAccount>> accountItem(this->_accounts);
		while (accountItem.hasNext())
		{
			accountItem.next();
			if (!Common::isNullOrEmpty(accountItem.value()->getMarginSetting()->getConsumerCode())) return true;
		}
		return false;
	}


	bool SettingManager::canSubmitMagin()
	{
		QMapIterator<QUuid, QSharedPointer<Setting::BusinessAccount>> accountItem(this->_accounts);
		while (accountItem.hasNext())
		{
			accountItem.next();
            auto marginSetting = accountItem.value()->getMarginSetting();
            bool isAgent = false;
            if (!accountItem.value()->getAgentId().isNull())
            {
                isAgent = (accountItem.value()->getAgentId() == Common::TraderBaseInfo::getInstance()->getUserId()
                           && accountItem.value()->getCustomerId()!= Common::TraderBaseInfo::getInstance()->getUserId());
            }
            if (!this->_systemSetting->getIsEmployee() && !isAgent)
            {
				if (accountItem.value()->canSubmitMargin())
					return true;
			}
			else
			{
                if (marginSetting->getEnablePI() && marginSetting->getAllowSalesPayment())
                    return true;
                if (marginSetting->getEnablePIInterACTransfer() && marginSetting->getAllowSalesPayment())
                    return true;
			}
            //judgment deposit
            if((!Common::isNullOrEmpty(this->_systemSetting->getConsumerCode()) &&
               !Common::isNullOrEmpty(marginSetting->getConsumerCode()) &&
               !this->_systemSetting->getCnyCurrencyId().isNull() &&
                this->_systemSetting->getPaymentPath().length() > 0)
                || marginSetting->getOnlineTransferCount()>0)
            {
                if (this->_systemSetting->getIsEmployee())
                {
                    if (marginSetting->getAllowSalesDeposit())
                        return true;
                }
                else
                    return true;
            }

            //Compatible with the old version for traderserver
            if(marginSetting->getOnlineTransferCount()==-1)
            {
                if(!Common::isNullOrEmpty(this->_systemSetting->getConsumerCode()) &&
                   !Common::isNullOrEmpty(marginSetting->getConsumerCode()))
                {
                    if (this->_systemSetting->getIsEmployee())
                    {
                        if (marginSetting->getAllowSalesDeposit())
                            return true;
                    }
                    else
                        return true;
                }
            }
		}
		return false;
	}


	bool SettingManager::isShowMagin()
	{        
//        return ((!Common::isNullOrEmpty(this->_systemSetting->getConsumerCode()) && this->hasConsumeCode()) ||
//                this->canSubmitMagin()) ? true : false;
        return this->canSubmitMagin();
	}

	bool SettingManager::checkIsBOInstrument(QUuid instrumentId)
	{
		int boCount = 0;
		QMapIterator<QUuid, QSharedPointer<Setting::BusinessAccount>> accountItem(this->_accounts);
		while (accountItem.hasNext())
		{
			accountItem.next();
			auto instrument = accountItem.value()->getInstrument(instrumentId);
			if (!instrument.isNull())
			{
				if (instrument->isExistBOPolicySetting())
					boCount++;
			}
		}
		return boCount == this->_accounts.count();
	}

	QList<QUuid> SettingManager::getPlaceAccountIdList(QUuid instrumentId)
	{
		auto accountIds = Common::TraderBaseInfo::getInstance()->getCheckedAccounts();
		QList<QUuid> accountIdList;
		for (int i = 0; i < accountIds.count(); i++)
		{
			auto instrument = this->getInstrument(accountIds[i], instrumentId);
			if (instrument.isNull() || instrument->isExistBOPolicySetting()) continue;
			accountIdList.append(accountIds[i]);
		}
		return accountIdList;
	}

	void SettingManager::updatCurrencyRates(const QDomNode& xmlData)
	{
		this->setCurrencyRate(xmlData);
	}

	QSharedPointer<Setting::Currency> SettingManager::getOrAddCurrency(const QUuid& id)
	{
		if (!this->_currencies.contains(id))
		{
			QSharedPointer<Setting::Currency> currency(new Setting::Currency);
			this->_currencies.insert(id, currency);
		}
		return this->_currencies.value(id);
	}

	QSharedPointer<Setting::Currency> SettingManager::getCurrency(const QUuid& id) const
	{
		return this->_currencies.value(id);
	}

	void SettingManager::addCurrency(const QUuid& id, const QSharedPointer<Setting::Currency>& currency)
	{
		if (!this->_currencies.contains(id))
		{
			this->_currencies.insert(id, currency);
		}
	}

	QMap<QUuid, QSharedPointer<Setting::Currency>> SettingManager::getCurrencies() const
	{
		return this->_currencies;
	}

	QSharedPointer<Setting::CurrencyRate> SettingManager::getOrAddCurrencyRate(const QUuid& sourceCurrencyId, const QUuid& targetCurrencyId)
	{
		if (this->_currencyRates.contains(sourceCurrencyId))
		{
			QMap<QUuid, QSharedPointer<Setting::CurrencyRate>> sourceCurrencyRates = this->_currencyRates[sourceCurrencyId];
			if (this->_currencyRates.value(sourceCurrencyId).contains(targetCurrencyId))
			{
				return sourceCurrencyRates[targetCurrencyId];
			}
			else
			{
				QSharedPointer<Setting::CurrencyRate> targetCurrencyRate(new Setting::CurrencyRate);
				this->_currencyRates[sourceCurrencyId].insert(targetCurrencyId, targetCurrencyRate);
				return targetCurrencyRate;
			}
		}
		else
		{
			QSharedPointer<Setting::CurrencyRate> targetCurrencyRate(new Setting::CurrencyRate);
			QMap<QUuid, QSharedPointer<Setting::CurrencyRate>> sourceCurrencyRates = QMap<QUuid, QSharedPointer<Setting::CurrencyRate>>();
			sourceCurrencyRates.insert(targetCurrencyId, targetCurrencyRate);
			this->_currencyRates.insert(sourceCurrencyId, sourceCurrencyRates);
			return targetCurrencyRate;
		}

	}

	QSharedPointer<Setting::CurrencyRate> SettingManager::getCurrencyRate(const QUuid& sourceCurrencyId, const QUuid& targetCurrencyId) const
	{
		if (this->_currencyRates.contains(sourceCurrencyId))
		{
			QMap<QUuid, QSharedPointer<Setting::CurrencyRate>> sourceCurrencyRates = this->_currencyRates.value(sourceCurrencyId);
			if (sourceCurrencyRates.contains(targetCurrencyId))
			{
				return sourceCurrencyRates.value(targetCurrencyId);
			}
		}
		QSharedPointer<Setting::CurrencyRate> currencyRate(Q_NULLPTR);
		return currencyRate;
	}

	void SettingManager::addCurrencyRate(const QUuid& sourceCurrencyId, const QUuid& targetCurrencyId, const QSharedPointer<Setting::CurrencyRate>& currencyRate)
	{
		if (!this->_currencyRates.contains(sourceCurrencyId))
		{
			QMap<QUuid, QSharedPointer<Setting::CurrencyRate>> sourceCurrencyRates = QMap<QUuid, QSharedPointer<Setting::CurrencyRate>>();
			sourceCurrencyRates.insert(targetCurrencyId, currencyRate);
			this->_currencyRates.insert(sourceCurrencyId, sourceCurrencyRates);
		}
		else
		{
			QMap<QUuid, QSharedPointer<Setting::CurrencyRate>> sourceCurrencyRates = this->_currencyRates.value(sourceCurrencyId);
			if (!sourceCurrencyRates.contains(targetCurrencyId))
			{
				sourceCurrencyRates.insert(targetCurrencyId, currencyRate);
			}
		}
	}

	QSharedPointer<Setting::ScrapInstrument> SettingManager::getOrAddScrapInstrument(const QUuid& id)
	{
		if (this->_scrapInstruments.contains(id))
		{
			return this->_scrapInstruments.value(id);
		}
		QSharedPointer<Setting::ScrapInstrument> scrapInstrument(new Setting::ScrapInstrument);
		this->_scrapInstruments.insert(id, scrapInstrument);
		return scrapInstrument;
	}

	QSharedPointer<Setting::ScrapInstrument> SettingManager::getScrapInstrument(const QUuid& id) const
	{
		if (this->_scrapInstruments.contains(id))
		{
			return _scrapInstruments.value(id);
		}
		QSharedPointer<Setting::ScrapInstrument> scrapInstrument(Q_NULLPTR);
		return scrapInstrument;
	}

	void SettingManager::addScrapInstrument(const QUuid& id, const QSharedPointer<Setting::ScrapInstrument>& scrapInstrument)
	{
		if (!this->_scrapInstruments.contains(id))
		{
			this->_scrapInstruments.insert(id, scrapInstrument);
		}
	}

	QSharedPointer<Setting::CalculateQuotationDefaultSetting> SettingManager::getOrAddCalculateQuotationDefaultSetting(const QUuid& instrumentId)
	{
		if (this->_calculateQuotationDefaultSettings.contains(instrumentId))
		{
			return this->_calculateQuotationDefaultSettings.value(instrumentId);
		}
		QSharedPointer<Setting::CalculateQuotationDefaultSetting> calculateQuotationDefaultSetting(new Setting::CalculateQuotationDefaultSetting);
		this->_calculateQuotationDefaultSettings.insert(instrumentId, calculateQuotationDefaultSetting);
		return calculateQuotationDefaultSetting;
	}

	QSharedPointer<Setting::DeliveryHoliday> SettingManager::getDeliveryHoliday(QDate deliveryTime)
	{
		QSharedPointer<Setting::DeliveryHoliday> deliveryHoliday;
		foreach(auto item, this->_deliveryHolidays)
		{
			if (deliveryTime >= item->getBeginDate() && deliveryTime <= item->getEndDate())
			{
				deliveryHoliday = item;
			}
		}
		return deliveryHoliday;
	}

	void SettingManager::setCurrencyRate(const QDomNode& xmlData)
	{
		QDomNodeList itemChildlist = xmlData.childNodes();
		for (int j = 0; j < itemChildlist.count(); j++)
		{
			QDomNode itemChildQDomNode = itemChildlist.item(j);
			QString itemName = itemChildQDomNode.nodeName();
			if (itemName == "CurrencyRate")
			{
				QString detailKey = itemChildQDomNode.toElement().attribute("DetailKey");
				QUuid sourceCurrencyId;
				QUuid targetCurrencyId;
				QString rateInStr;
				QString rateOutStr;
				if(!(detailKey.isEmpty() || detailKey.isNull()))
				{
					int separation = detailKey.indexOf("_", 0);
					QString l_sourceCurrencyId = detailKey.left(separation);
					QString r_targetCurrencyId = detailKey.right(detailKey.length() - separation - 1);
					sourceCurrencyId = QUuid(l_sourceCurrencyId);
					targetCurrencyId = QUuid(r_targetCurrencyId);
				}
				QDomNamedNodeMap map = itemChildQDomNode.attributes();
				for (int k = 0; k < map.count(); k++)
				{
					QDomNode qDomNode = map.item(k);
					QString name = qDomNode.nodeName();
					QString value = qDomNode.nodeValue();
					if (name == "RateIn")
						rateInStr = value;
					else if (name == "RateOut")
						rateOutStr = value;
					else if (name == "SourceCurrencyID" || name == "SourceCurrencyId")
						sourceCurrencyId = QUuid(value);
					else if (name == "TargetCurrencyID" || name == "TargetCurrencyId")
						targetCurrencyId = QUuid(value);

				}

				QSharedPointer<Model::Setting::CurrencyRate> currencyRate = this->getOrAddCurrencyRate(sourceCurrencyId, targetCurrencyId);
				QSharedPointer<Model::Setting::Currency> sourceCurrency = this->getOrAddCurrency(sourceCurrencyId);
				currencyRate->setSourceCurrency(sourceCurrency);
				QSharedPointer<Model::Setting::Currency> targetCurrency = this->getOrAddCurrency(targetCurrencyId);
				currencyRate->setTargetCurrency(targetCurrency);
				currencyRate->setRateIn(QDecNumber::fromQString(rateInStr));
				currencyRate->setRateOut(QDecNumber::fromQString(rateOutStr));
			}
		}
	}

	void SettingManager::AddOrUpdateCalculateQuotationDefaultSetting(QSharedPointer<Setting::CalculateQuotationDefaultSetting>& calculateQuotationDefaultSetting)
	{
		if (this->_calculateQuotationDefaultSettings.contains(calculateQuotationDefaultSetting->getInstrumentId()))
			this->_calculateQuotationDefaultSettings[calculateQuotationDefaultSetting->getInstrumentId()] = calculateQuotationDefaultSetting;
		else
			this->_calculateQuotationDefaultSettings.insert(calculateQuotationDefaultSetting->getInstrumentId(), calculateQuotationDefaultSetting);
	}

	QSharedPointer<Setting::InterestRate>& SettingManager::getInterestRateSource(const QUuid & id)
	{
		return this->_interestRates[id];
	}

	bool Business::SettingManager::isAllowTraderResetAlertLevel(int forbiddenAlert)
	{
		return this->_systemSetting->getAllowTraderResetAlertLevel() && forbiddenAlert < 1;
	}

	bool Business::SettingManager::isTradeOpenedAccount(QUuid accountId, int counterPartyType)
	{
		return Common::contains(this->_accounts[accountId]->getCounterPartiesMask(), counterPartyType);
	}

	bool Business::SettingManager::isContainFromCurrentUser(QUuid accountId)
	{
		return Common::TraderBaseInfo::getInstance()->getUserId() == this->_accounts[accountId]->getCustomerId();
	}

    QSharedPointer<Trading::MarketingGroupFund> Business::SettingManager::getMarketingGroupFund(QString saleIdAndAccountCurrenyId)
	{
        return this->_marketingGroupFunds.value(saleIdAndAccountCurrenyId);
    }
	
	QString SettingManager::getInstrumentUnit(const QUuid& accountId, const QUuid& instrumentId)
	{
        if (this->_accounts[accountId]->getShowWeightOnly())
        {
            auto instrument = this->getInstrument(accountId, instrumentId);
            return instrument->getUnit();
        }
        return "";
	}

    QString SettingManager::getInstrumentUnitNoUseShowWeightOnly(const QUuid& accountId, const QUuid& instrumentId)
    {
        auto instrument = this->getInstrument(accountId, instrumentId);
        return instrument->getUnit();
    }

	QString SettingManager::getInstrumentUnitByMultiAccount(const QUuid& instrumentId)
	{
		bool isShowWeightOnly = true;
		foreach(auto account, this->_accounts)
		{
			if (!account->getShowWeightOnly())
				isShowWeightOnly = false;
		}
		if (isShowWeightOnly)
		{
			auto instrument = this->getInstrument(this->_accounts.first()->getId(), instrumentId);
			if (!instrument.isNull())
				return instrument->getUnit();
		}
		return "";
	}

	bool SettingManager::isShowWeightOnly()
	{
		bool isShowWeightOnly = true;
		foreach(auto account, this->_accounts)
		{
			if (!account->getShowWeightOnly())
				isShowWeightOnly = false;
		}
		return isShowWeightOnly;
	}


    QSharedPointer<Setting::BusinessAccount> SettingManager::getCopyAccount(const QUuid& id)
    {
        if (!this->_accounts.contains(id))
        {
            QLOG_ERROR( Common::Constants::LogKeyConstants::Business) <<  QString("MethodName:SettingManager::getAccount  Info:account non-existent! id:%1").arg(id.toString());
            QSharedPointer<Setting::BusinessAccount> newAccount(Q_NULLPTR);
            return newAccount;
        }
        else
        {
            auto oldAccount = this->_accounts.value(id).data();
            return oldAccount->copy();
        }
    }

    QList<QSharedPointer<Setting::BusinessAccount>> SettingManager::getAccounts()
    {
        return this->_accounts.values();
    }

    bool variantLessThanAccount(QSharedPointer<Setting::BusinessAccount> v1, QSharedPointer<Setting::BusinessAccount> v2)
    {
        return v1->getCode().toLower()  < v2->getCode().toLower();
    }

    QList<QSharedPointer<Setting::BusinessAccount>> SettingManager::getAccountsForBankAccount()
    {
        QList<QSharedPointer<Setting::BusinessAccount>> accounts;
        auto userId = Common::TraderBaseInfo::getInstance()->getUserId();
        auto userName = Common::TraderBaseInfo::getInstance()->getUserName();
        foreach (QSharedPointer<Setting::BusinessAccount> account, this->_accounts) {
            auto marginSetting = account->getMarginSetting();
            bool isAgent = false;
            if (!account->getAgentId().isNull())
            {
                isAgent = (account->getAgentId() == userId && account->getCustomerId()!= userId);
            }
            if(!this->_systemSetting->getIsEmployee() && !isAgent)
            {
                if(marginSetting->getEnablePI() && account->getCustomerId() == userId)
                {
                    accounts.append(account);
                }
            }
            else if (this->_systemSetting->getIsEmployee())
            {
                if (marginSetting->getEnablePI() && marginSetting->getAllowSalesPayment() && account->getSalesCode() == userName)
                {
                   accounts.append(account);
                }
            }
        }

        if(accounts.count() > 1)
            qSort(accounts.begin(), accounts.end(), variantLessThanAccount);
        return accounts;
    }

    QUuid SettingManager::getFirstAccountIdForClientRegistration()
    {
        auto userId = Common::TraderBaseInfo::getInstance()->getUserId();
        auto accounts = this->_accounts.values();
        if(accounts.count() > 1)
            qSort(accounts.begin(), accounts.end(), variantLessThanAccount);
        foreach (QSharedPointer<Setting::BusinessAccount> account, accounts) {
           if(account->getCustomerId() == userId)
           {
               return account->getId();
           }
        }
        foreach (QSharedPointer<Setting::BusinessAccount> account, accounts) {
           if(account->getAgentId() == userId)
           {
               return account->getId();
           }
        }
    }
}
