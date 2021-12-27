#include "InitializeManager.h"
#include "SettingManager.h"
#include "Common/SignalForwarder.h"
#include "Common/GuidMappingManager.h"
#include "Common/Constants/DateTimeConstants.h"

namespace Business
{
	InitializeManager::InitializeManager()
	{
	}

	void InitializeManager::updateInitializeData(const QDomNode& xmlInitializeData, QSharedPointer<SettingManager>& settingManager)
	{
		settingManager->initSystemSetting(xmlInitializeData);
		QDomNodeList childlist = xmlInitializeData.childNodes();
		int count = childlist.count();
		for (int i = 0; i < childlist.count(); i++)
		{
			QDomNode itemQDomNode = childlist.item(i);
			QString name = itemQDomNode.nodeName();
			QString xmlvalue = itemQDomNode.nodeValue();
			if (name == "Accounts")
			{
				QDomNodeList itemChildlist = itemQDomNode.childNodes();
				for (int j = 0; j < itemChildlist.count(); j++)
				{
					QDomNode itemChildQDomNode = itemChildlist.item(j);
					QString itemName = itemChildQDomNode.nodeName();
					if (itemName == "Account")
					{
						QSharedPointer<Common::Struct::SettingAccount>  account(new Common::Struct::SettingAccount);
						QDomNamedNodeMap map = itemChildQDomNode.attributes();
						for (int k = 0; k < map.count(); k++)
						{
							QDomNode qDomNode = map.item(k);
							QString fieldName = qDomNode.nodeName();
							QString fieldvalue = qDomNode.nodeValue();
							if (fieldName == "Id")
								account->_id = QUuid(fieldvalue);
							else if (fieldName == "Code")
								account->_code = fieldvalue;
							else if (fieldName == "Name")
								account->_name = fieldvalue;
						}
						this->_accounts.insert(account->_id, account);
					}
				}
			}
			else if (name == "Instruments")
			{
				Common::GuidMappingManager::getInstance()->clear();
				QDomNodeList itemChildlist = itemQDomNode.childNodes();
				for (int j = 0; j < itemChildlist.count(); j++)
				{
					QDomNode itemChildQDomNode = itemChildlist.item(j);
					QString itemName = itemChildQDomNode.nodeName();
					if (itemName == "Instrument")
					{
                        QSharedPointer<Common::Struct::SettingInstrument>  instrument(new Common::Struct::SettingInstrument);
						QDomNamedNodeMap map = itemChildQDomNode.attributes();
						for (int k = 0; k < map.count(); k++)
						{
							QDomNode qDomNode = map.item(k);
							QString fieldName = qDomNode.nodeName();
							QString fieldvalue = qDomNode.nodeValue();
							if (fieldName == "Id")
								instrument->_id = QUuid(fieldvalue);
							else if (fieldName == "Sequence")
                            {
								instrument->_sequence = fieldvalue.toInt();
                            }
							else if (fieldName == "Code")
								instrument->_code = fieldvalue;
							else if (fieldName == "OriginCode")
								instrument->_originCode = fieldvalue;
							else if (fieldName == "Description")
								instrument->_description = fieldvalue;
							else if (fieldName == "Category")
								instrument->_instrumentCategory = static_cast<InstrumentCategory::InstrumentCategory>(fieldvalue.toInt());
							else if (fieldName == "NumeratorUnit")
								instrument->_numeratorUnit = fieldvalue.toInt();
							else if (fieldName == "Denominator")
								instrument->_denominator = fieldvalue.toInt();
							else if (fieldName == "GuidMappingValue")
								instrument->_guidMappingValue = fieldvalue.toInt();
                            else if (fieldName == "OfficialFixingDate")
                                instrument->_officialFixingDate = fieldvalue;
						}
						this->_instruments.insert(instrument->_id, instrument);
						Common::GuidMappingManager::getInstance()->addInstrumentGuidMapping(instrument->_guidMappingValue, instrument->_id);
					}
				}
			}
			else if (name == "Messages" || name == "Newses")
			{
				QDateTime maxPublishQDateTime = Common::Constants::DateTimeConstants::MinQDateTime;
				QUuid showId = QUuid();
				QDomNodeList itemChildlist = itemQDomNode.childNodes();
				int c = itemChildlist.count();
				for (int j = 0; j < itemChildlist.count(); j++)
				{
					QDomNode itemChildQDomNode = itemChildlist.item(j);
					QString itemName = itemChildQDomNode.nodeName();
					if (itemName == "Message")
					{
						QSharedPointer<Model::Chat> chat(new Model::Chat());
						chat->update(itemChildQDomNode);
						settingManager->_chats.insert(chat->getId(), chat);
						if (chat->getPublishQDateTime() > maxPublishQDateTime && !chat->getIsRead())
						{
							maxPublishQDateTime = chat->getPublishQDateTime();
							showId = chat->getId();
						}
						InformationStruct informationStruct;
						informationStruct._id = chat->getId();
						informationStruct._title = chat->getTitle();
						informationStruct._informationCenterEnum = InformationCenterEnum::Chat;
						informationStruct._isRead = chat->getIsRead();
						if (settingManager->_informationStructs.count() < 6)
						{
							settingManager->_informationStructs.append(informationStruct);
						}

					}
					else if (itemName == "News")
					{
						QSharedPointer<Model::News> news(new Model::News());
						news->update(itemChildQDomNode);
						settingManager->_newses.insert(news->getId(), news);
						InformationStruct informationStruct;
						informationStruct._id = news->getId();
						informationStruct._title = news->getTitle();
						informationStruct._informationCenterEnum = InformationCenterEnum::News;
						informationStruct._isRead = news->getIsRead();
						if (settingManager->_informationStructs.count() < 6)
						{
							settingManager->_informationStructs.append(informationStruct);
						}
					}
				}

				if (!showId.isNull())
				{
					Common::SignalForwarder::getInstance()->sendShowNewsOrChatSignal(showId, Common::Enum::InformationCenterEnum::Chat);
				}
			}
			else if (name == "PriceAlerts")
			{
				QDomNodeList itemChildlist = itemQDomNode.childNodes();
                //int c = itemChildlist.count();
				for (int j = 0; j < itemChildlist.count(); j++)
				{
					QDomNode itemChildQDomNode = itemChildlist.item(j);
					QString itemName = itemChildQDomNode.nodeName();
					if (itemName == "Mapping")
					{
						QSharedPointer<Model::PriceAlert> priceAlert(new Model::PriceAlert());
						priceAlert->update(itemChildQDomNode);
						if (this->_instruments.contains(priceAlert->getInstrumentId()))
						{
							priceAlert->setInstrumentDescription(this->_instruments[priceAlert->getInstrumentId()]->_description);
							settingManager->_priceAlerts.insert(priceAlert->getId(), priceAlert);
						}
					}
				}
			}
			else if (name == "QuotePolicyGuidMappings")
			{
				QDomNodeList itemChildlist = itemQDomNode.childNodes();
                //int c = itemChildlist.count();
				for (int j = 0; j < itemChildlist.count(); j++)
				{
					QDomNode itemChildQDomNode = itemChildlist.item(j);
					QString itemName = itemChildQDomNode.nodeName();
					if (itemName == "Mapping")
					{
						QUuid Id = QUuid(itemChildQDomNode.toElement().attribute("Id"));
						int mappingValue = itemChildQDomNode.toElement().attribute("Value").toInt();
						Common::GuidMappingManager::getInstance()->addQuotePolicyGuidMapping(mappingValue, Id);
					}
				}
			}
			else if (name == "TradingListColumnsVisibilities")
			{
				this->_tradingListColumnsHides.clear();
				QDomNodeList mappingChildlist = itemQDomNode.childNodes();
				for (int j = 0; j < mappingChildlist.count(); j++)
				{
					QDomNode tradingListColumnsVisibilityQDomNode = mappingChildlist.item(j);
					QString name = tradingListColumnsVisibilityQDomNode.nodeName();
					if (name == "TradingListColumnsVisibility")
					{
						QSharedPointer<TradingListColumnsVisibility> tradingListColumnsVisibility(new TradingListColumnsVisibility);
						tradingListColumnsVisibility->update(tradingListColumnsVisibilityQDomNode);
						if (!tradingListColumnsVisibility->getIsDisplay())
						{
							if (this->_tradingListColumnsHides.contains(tradingListColumnsVisibility->getListName()))
							{
								this->_tradingListColumnsHides[tradingListColumnsVisibility->getListName()].append(tradingListColumnsVisibility);
							}
							else
							{
								QList<QSharedPointer<TradingListColumnsVisibility>> tradingListColumnsVisibilityList;
								tradingListColumnsVisibilityList.append(tradingListColumnsVisibility);
								this->_tradingListColumnsHides.insert(tradingListColumnsVisibility->getListName(), tradingListColumnsVisibilityList);
							}
						}
					}
				}
			}
			else if (name == "TradeDay")
			{
				if (settingManager->_tradeDay.isNull())
				{
					QSharedPointer<Setting::TradeDay>  tradeDay(new Setting::TradeDay);
					settingManager->_tradeDay = tradeDay;
				}
				settingManager->_tradeDay->update(itemQDomNode);
			}
			else if (name == "InstrumentParameters")
			{
				settingManager->_instrumentParameters.clear();
				QDomNodeList mappingChildlist = itemQDomNode.childNodes();
				for (int j = 0; j < mappingChildlist.count(); j++)
				{
					QDomNode mappingItemQDomNode = mappingChildlist.item(j);
					QString mappingItemName = mappingItemQDomNode.nodeName();
					if (mappingItemName == "InstrumentParameter")
					{
						QSharedPointer<Setting::InstrumentParameter> instrumentParameter(new Setting::InstrumentParameter);
						instrumentParameter->update(mappingItemQDomNode);
						settingManager->_instrumentParameters.insert(instrumentParameter->getId(), instrumentParameter);
					}
				}
			}
			else if (name == "InstrumentStatuses")
			{
				settingManager->_instrumentStatus.clear();
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
								instrumentStatusList.append(static_cast<InstrumentStatus::InstrumentStatus>(item.toInt()));
								Common::SignalForwarder::getInstance()->sendInstrumentStatusChangedSignal(instrumentId, item.toInt());
							}
						}
						settingManager->_instrumentStatus.insert(instrumentId, instrumentStatusList);
					}
				}
			}
		}
		this->_settingManager = settingManager;
	}


    bool variantLessThanAccount(QSharedPointer<Common::Struct::SettingAccount> v1, QSharedPointer<Common::Struct::SettingAccount> v2)
    {
        return v1->_code.toLower()  < v2->_code.toLower();
    }

    bool variantLessThanInstrument(QSharedPointer<Common::Struct::SettingInstrument> v1, QSharedPointer<Common::Struct::SettingInstrument> v2)
    {
        return v1->_description.toLower() < v2->_description.toLower();
    }

    bool variantLessThanInstrument2(QSharedPointer<Common::Struct::SettingInstrument> v1, QSharedPointer<Common::Struct::SettingInstrument> v2)
    {
        if(v1->_sequence == v2->_sequence)
            return v1->_description.toLower() < v2->_description.toLower();
        else
            return v1->_sequence < v2->_sequence;
    }

    bool variantLessThanInstrument3(QSharedPointer<Common::Struct::SettingInstrument> v1, QSharedPointer<Common::Struct::SettingInstrument> v2)
    {
        if(v1->_tmpSequence == v2->_tmpSequence)
            return v1->_description.toLower() < v2->_description.toLower();
        else
            return v1->_tmpSequence < v2->_tmpSequence;
    }


	QList<QSharedPointer<Common::Struct::SettingAccount>> InitializeManager::getAccountsBySortAccountCode()
	{
		QList<QSharedPointer<Common::Struct::SettingAccount>> accounts = this->_accounts.values();
        qSort(accounts.begin(), accounts.end(), variantLessThanAccount);
		return accounts;
	}

	QList<QSharedPointer<Common::Struct::SettingAccount>> InitializeManager::getAccounts()
	{
		return this->_accounts.values();
	}

	QList<QSharedPointer<Common::Struct::SettingInstrument>> InitializeManager::getInstruments()
	{
		return this->_instruments.values();
    }

	QList<QSharedPointer<Common::Struct::SettingInstrument>> InitializeManager::getInstrumentsBySortCode()
	{
		QList<QSharedPointer<Common::Struct::SettingInstrument>> instruments = this->_instruments.values();
		qSort(instruments.begin(), instruments.end(), variantLessThanInstrument);
		return instruments;
	}

    QList<QSharedPointer<Common::Struct::SettingInstrument>> InitializeManager::getInstrumentsBySortSequenceAndCode()
    {
        QList<QSharedPointer<Common::Struct::SettingInstrument>> instruments = this->_instruments.values();
        qSort(instruments.begin(), instruments.end(), variantLessThanInstrument2);
        return instruments;
    }

    QList<QSharedPointer<Common::Struct::SettingInstrument>> InitializeManager::getNewAddInstruments(QList<QUuid> oldInstrumentIds)
    {
        QList<QSharedPointer<Common::Struct::SettingInstrument>> instruments = QList<QSharedPointer<Common::Struct::SettingInstrument>>();
        auto keys = this->_instruments.keys();
        for (int i=0; i< keys.count(); i++)
        {
            if (!oldInstrumentIds.contains(keys[i]))
            {
                instruments.append(this->_instruments[keys[i]]);
            }
        }
		qSort(instruments.begin(), instruments.end(), variantLessThanInstrument2);
        return instruments;
    }

    QList<QSharedPointer<Common::Struct::SettingInstrument>> InitializeManager::getLayoutInstruments(QList<QUuid> layoutInstrumentIds)
    {
        QList<QSharedPointer<Common::Struct::SettingInstrument>> instruments;
        for(int i = 0; i < layoutInstrumentIds.count() ; i++)
        {
            if (this->_instruments.contains(layoutInstrumentIds[i]))
            {
                instruments.append(this->_instruments.value(layoutInstrumentIds[i]));
            }
        }
        return instruments;
    }

    QList<QSharedPointer<Common::Struct::SettingInstrument>> InitializeManager::getAllInstrumentsBySortSequenceAndCode(QList<QUuid> layoutInstrumentIds)
    {
        QList<QSharedPointer<Common::Struct::SettingInstrument>> instruments = QList<QSharedPointer<Common::Struct::SettingInstrument>>();
        QMapIterator<QUuid, QSharedPointer<Common::Struct::SettingInstrument>> instrumentItem(this->_instruments);
        while (instrumentItem.hasNext()) {
            instrumentItem.next();
            QUuid id = instrumentItem.key();
            if (layoutInstrumentIds.contains(id))
            {
                this->_instruments[id]->_tmpSequence = layoutInstrumentIds.indexOf(id);
            }
            else
            {
                if(instrumentItem.value()->_sequence < 0)
                    this->_instruments[id]->_tmpSequence = 9999;
                else
                    this->_instruments[id]->_tmpSequence = instrumentItem.value()->_sequence;
            }
            qDebug() << "InitializeManager::getAllInstrumentsBySortSequenceAndCode: id:" << id.toString()
                     << " desc:" << instrumentItem.value()->_description
                     << " temSequence:" << this->_instruments[id]->_tmpSequence
                     << " sequence:" << this->_instruments[id]->_sequence;
            instruments.append(this->_instruments[id]);
        }
        qSort(instruments.begin(), instruments.end(), variantLessThanInstrument3);
        return instruments;
    }

	QList<QSharedPointer<Common::Struct::SettingInstrument>> InitializeManager::getUnCheckInstruments(QList<QString> notContainInstrumentIds)
	{
		QList<QSharedPointer<Common::Struct::SettingInstrument>> instruments;
		for (auto item = this->_instruments.cbegin(); item != this->_instruments.cend(); ++item)
		{
			if (!notContainInstrumentIds.contains(item.key().toString()))
			{
				instruments.append(item.value());
			}
		}
		return instruments;
	}

	QSharedPointer<Common::Struct::SettingInstrument> InitializeManager::getInstrument(QUuid instrumentId)
	{
		if (this->_instruments.contains(instrumentId))
		{
			return this->_instruments.value(instrumentId);
		}
		return QSharedPointer<Common::Struct::SettingInstrument>(Q_NULLPTR);
	}

	QSharedPointer<Common::Struct::SettingAccount> InitializeManager::getAccount(QUuid accountId)
	{
		return this->_accounts.value(accountId);
	}

	bool InitializeManager::isContainsAccount(QUuid id)
	{
		return this->_accounts.contains(id);
	}

	int InitializeManager::getPhysicalCount()
	{
		int physicalCount = 0;
		foreach(auto item, this->_instruments.values())
		{
			if (item->_instrumentCategory == InstrumentCategory::Physical)
				physicalCount++;
		}
		return physicalCount;
	}

	bool InitializeManager::isExistAccountOrInstrument()
	{
		return this->_accounts.count() > 0 && this->_instruments.count() > 0;
	}

	bool InitializeManager::isExistInstrument()
	{
		return this->_instruments.count() > 0;
	}

	int InitializeManager::getBOInstrumentCount()
	{
		QList<QUuid> instrumentIds;
		QMapIterator<QUuid, QSharedPointer<Setting::BusinessAccount>> accountItem(this->_settingManager->_accounts);
		while (accountItem.hasNext())
		{
			accountItem.next();
			foreach(auto instrumentItem, accountItem.value()->getInstruments())
			{
				if (instrumentItem->isExistBOPolicySetting() && !instrumentIds.contains(instrumentItem->getId()))
					instrumentIds.append(instrumentItem->getId());
			}
		}
		return instrumentIds.count();
	}

	int InitializeManager::getInstrumentCount()
	{
		return this->_instruments.count();
	}

    void InitializeManager::removeNoActiveInstrument(QUuid instrumentId)
	{
		this->_instruments.remove(instrumentId);
	}

    bool InitializeManager::isExistInstrument(QUuid instrumentId)
	{
		return this->_instruments.contains(instrumentId);
	}

    bool InitializeManager::isShowOfficialFixing()
    {
        bool isShow = false;
        foreach(auto item, this->_instruments.values())
        {
            if (item->_officialFixingDate != "")
                isShow = true;
        }
        return isShow;
    }

    bool InitializeManager::isShowOfficialFixingByInstrument(QUuid instrumentId)
    {
        return this->_instruments.value(instrumentId)->_officialFixingDate != "";
    }

    QString InitializeManager::getFixingDateByInstrument(QUuid instrumentId)
    {
        return this->_instruments.value(instrumentId)->_officialFixingDate;
    }

	int InitializeManager::getNotBOInstrumentCount()
	{
		QList<QUuid> instrumentIds;
		QMapIterator<QUuid, QSharedPointer<Setting::BusinessAccount>> accountItem(this->_settingManager->_accounts);
		while (accountItem.hasNext())
		{
			accountItem.next();
			foreach(auto instrumentItem, accountItem.value()->getInstruments())
			{
				if (!instrumentIds.contains(instrumentItem->getId()) && !instrumentItem->isExistBOPolicySetting())
					instrumentIds.append(instrumentItem->getId());
			}
		}
		return instrumentIds.count();
	}

	QList<QString> InitializeManager::getTradingListColumnsHides(TradingListNameEnum::TradingListNameEnum tradingListNameEnum)
	{
		QList<QString> hideColumns;
		auto list = this->_tradingListColumnsHides.value(tradingListNameEnum);
		foreach (auto item, list)
		{
			hideColumns.append(item->getColumnName());
		}
		return hideColumns;
	}

    QList<QString> InitializeManager::getInstrumentIds()
    {
        QList<QString> instrumentIds;
        for (auto item = this->_instruments.cbegin(); item != this->_instruments.cend(); ++item)
        {
            instrumentIds.append(item.value()->_id.toString());
        }
        return instrumentIds;
    }


    QMap<QUuid, bool> InitializeManager::getEnableClickTradeList()
    {
        QMap<QUuid, bool> enableClickTradeList;
        bool enableClickTrade = this->_settingManager->getSystemSetting()->getEnableClickTrade();
        if(enableClickTrade)
        {
            auto keys = this->_instruments.keys();
            for (int i=0; i< keys.count(); i++)
            {
                enableClickTradeList.insert(keys[i], true);
            }
        }
        return enableClickTradeList;
    }

    QList<QUuid> InitializeManager::getInstrumentKeyIds()
    {
        return this->_instruments.keys();
    }

}
