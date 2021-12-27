#include "QuotationManager.h"
#include <QDebug>
#include "BusinessManager.h"
#include "Common/LogHelper.h"
#include "Common/Constants/DateTimeConstants.h"
#include "Common/Constants/QuotationConstants.h"
#include "Common/GuidMappingManager.h"
#include "DateTimeHelper.h"

using namespace Common::Constants;
namespace Business
{
	QuotationManager::QuotationManager()
	{
        _isWriteLog = Common::AppSetting::getInstance()->getWriteLog();
	}

	void QuotationManager::updateInitializeData(const QDomNode& xmlInitializeData)
	{
		int maxLength = 0;
		QDomNodeList childlist = xmlInitializeData.childNodes();
		for (int i = 0; i < childlist.count(); i++)
		{
			QDomNode itemQDomNode = childlist.item(i);
			QString name = itemQDomNode.nodeName();
			QString xmlvalue = itemQDomNode.nodeValue();
			if (name == "Quotations")
			{
				QDomNodeList quotationChildlist = itemQDomNode.childNodes();
				for (int j = 0; j < quotationChildlist.count(); j++)
				{
					QSharedPointer<Model::InitialQuotation> newInitialQuotation(new Model::InitialQuotation);
					newInitialQuotation->update(quotationChildlist.item(j));
					QString key = newInitialQuotation->getQuotePolicyId().toString() + newInitialQuotation->getInstrumentId().toString();
					this->_overridedQuotations.insert(key, newInitialQuotation);
					maxLength = qMax(maxLength, newInitialQuotation->getMaxPriceLength());
				}
			}
		}
		Common::TraderBaseInfo::getInstance()->setExceedCommonPriceNumber(maxLength);
	}

	QList<QSharedPointer<Model::InitialQuotation>> QuotationManager::getOverridedQuotations()
	{
		return this->_overridedQuotations.values();
	}

	void QuotationManager::setQuotePolicy(QMap<QUuid, QMap<QUuid, QString>> instrumentQuotePolicyMappings)
	{
		QMap<QString, QSharedPointer<Model::InitialQuotation>> overridedQuotations;
		QMap<QString, QSharedPointer<Model::InitialQuotation>>::const_iterator item = this->_overridedQuotations.constBegin();
		while (item != this->_overridedQuotations.constEnd()) {
			QUuid instrumentId = QUuid(item.key());
			if (instrumentQuotePolicyMappings.contains(instrumentId))
			{
				QMapIterator<QUuid, QString> instrumentQuotePolicyItem(instrumentQuotePolicyMappings[instrumentId]);
				while (instrumentQuotePolicyItem.hasNext()) {
					instrumentQuotePolicyItem.next();

					QSharedPointer<Model::InitialQuotation> newInitialQuotation(new Model::InitialQuotation);
					QString key = instrumentQuotePolicyItem.key().toString() + item.key();
					newInitialQuotation->updateQuotation(item.value());
					newInitialQuotation->setQuotePolicyID(instrumentQuotePolicyItem.key());
					overridedQuotations.insert(key, newInitialQuotation);
				}
			}
			++item;
		}

		this->_overridedQuotations.clear();
		QMapIterator<QString, QSharedPointer<Model::InitialQuotation>> overridedQuotationItem(overridedQuotations);
		while (overridedQuotationItem.hasNext()) {
			overridedQuotationItem.next();
			this->_overridedQuotations.insert(overridedQuotationItem.key(), overridedQuotationItem.value());
		}
	}

	QSharedPointer<Model::InitialQuotation> QuotationManager::getOverridedQuotation(QUuid quotePolicyId, QUuid instrumentId)
	{
		QString key = quotePolicyId.toString() + instrumentId.toString();
		if (this->_overridedQuotations.contains(key))
			return this->_overridedQuotations[key];
		QString key2 = instrumentId.toString();
		if (this->_overridedQuotations.contains(key2))
			return this->_overridedQuotations[key2];
		return QSharedPointer<Model::InitialQuotation>(Q_NULLPTR);
	}


	std::vector<std::string> QuotationManager::parseCommon(const std::string& input, char delimeter)
	{
		std::vector<std::string> result;
		auto i = input.cbegin();
		while (i < input.cend())
		{
			auto j = std::find(i, input.cend(), delimeter);
			result.push_back(std::string(i, j));
			if (j == input.cend())
			{
				break;
			}
			i = j + 1;
		}
		return result;
	}

	void QuotationManager::updateDailyCloseQuotation(const QDomNode & xmlData, QSharedPointer<SettingManager> settingManager)
	{
		QDomNodeList itemChildlist = xmlData.childNodes();
		for (int j = 0; j < itemChildlist.count(); j++)
		{
			QDomNode itemChildQDomNode = itemChildlist.item(j);
			QString itemName = itemChildQDomNode.nodeName();
			if (itemName == "InstrumentDailyCloseQuotation")
			{
				QUuid instrumentId = QUuid(itemChildQDomNode.toElement().attribute("InstrumentId"));
				QUuid quotePolicyId = QUuid(itemChildQDomNode.toElement().attribute("QuotePolicyId"));
				QString key = quotePolicyId.toString() + instrumentId.toString();
				QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  " key:" << key;
				if (this->_overridedQuotations.contains(key))
				{
					QDateTime tradeDay = QDateTime::fromString(itemChildQDomNode.toElement().attribute("TradeDay"), DateTimeConstants::DateTimeFormat);
					QDateTime currentTradeDay = settingManager->getTradeDay()->getCurrentDay();
                    QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  QString(" tradeDay:%1 LastDay:%2 currentTradeDay:%3").arg(
                        DateTimeHelper::toDateTimeString(tradeDay),
                        DateTimeHelper::toDateTimeString(settingManager->getTradeDay()->getLastDay()),
                        DateTimeHelper::toDateTimeString(currentTradeDay));
                    if (tradeDay >= currentTradeDay || tradeDay.date() >= currentTradeDay.date())
					{
						QString open = itemChildQDomNode.toElement().attribute("Open");
						if (!open.isEmpty() && open != "")
							this->_overridedQuotations[key]->setOpen(open);
						QString askOpen = itemChildQDomNode.toElement().attribute("AskOpen");
						if (!askOpen.isEmpty() && askOpen != "")
							this->_overridedQuotations[key]->setAskOpen(askOpen);
						QString close = itemChildQDomNode.toElement().attribute("Close");
						if (!close.isEmpty() && close != "")
							this->_overridedQuotations[key]->setPrevClose(close);
						QString prevAsk = itemChildQDomNode.toElement().attribute("PrevAsk");
						if (!prevAsk.isEmpty() && prevAsk != "")
							this->_overridedQuotations[key]->setPrevAsk(prevAsk);
						QString prevBid = itemChildQDomNode.toElement().attribute("PrevBid");
						if (!prevBid.isEmpty() && prevBid != "")
							this->_overridedQuotations[key]->setPrevBid(prevBid);
						QString ask = itemChildQDomNode.toElement().attribute("Ask");
						if (!ask.isEmpty() && ask != "")
							this->_overridedQuotations[key]->setAsk(ask);
						QString bid = itemChildQDomNode.toElement().attribute("Bid");
						if (!bid.isEmpty() && bid != "")
							this->_overridedQuotations[key]->setBid(bid);						
						QString high = itemChildQDomNode.toElement().attribute("High");
						if (!high.isEmpty() && high != "")
							this->_overridedQuotations[key]->setHigh(high);
						QString low = itemChildQDomNode.toElement().attribute("Low");
						if (!low.isEmpty() && low != "")
							this->_overridedQuotations[key]->setLow(low);
						QString askHigh = itemChildQDomNode.toElement().attribute("AskHigh");
						if (!askHigh.isEmpty() && askHigh != "")
							this->_overridedQuotations[key]->setAskHigh(askHigh);
						QString askLow = itemChildQDomNode.toElement().attribute("AskLow");
						if (!askLow.isEmpty() && askLow != "")
							this->_overridedQuotations[key]->setAskLow(askLow);
					}
					else
					{						
						//修改上一个交易日的数据，如果没有开市，则需要更新Ask Bid High Low饰
						if (tradeDay.date() == settingManager->getTradeDay()->getLastDay().date())
						{
							QString close = itemChildQDomNode.toElement().attribute("Close");							
							if (!close.isEmpty() && close != "")
								this->_overridedQuotations[key]->setPrevClose(close);
							QString prevAsk = itemChildQDomNode.toElement().attribute("PrevAsk");
							if (!prevAsk.isEmpty() && prevAsk != "")
								this->_overridedQuotations[key]->setPrevAsk(prevAsk);
							QString prevBid = itemChildQDomNode.toElement().attribute("PrevBid");
							if (!prevBid.isEmpty() && prevBid != "")
								this->_overridedQuotations[key]->setPrevBid(prevBid);
							
							if (this->_overridedQuotations[key]->getTimestamp() <= settingManager->getTradeDay()->getBeginTime())
							{
								QString ask = itemChildQDomNode.toElement().attribute("Ask");
								if (!ask.isEmpty() && ask != "")
									this->_overridedQuotations[key]->setAsk(ask);
								QString bid = itemChildQDomNode.toElement().attribute("Bid");
								if (!bid.isEmpty() && bid != "")
									this->_overridedQuotations[key]->setBid(bid);
								QString high = itemChildQDomNode.toElement().attribute("High");
								if (!high.isEmpty() && high != "")
									this->_overridedQuotations[key]->setHigh(high);
								QString low = itemChildQDomNode.toElement().attribute("Low");
								if (!low.isEmpty() && low != "")
									this->_overridedQuotations[key]->setLow(low);
								QString askHigh = itemChildQDomNode.toElement().attribute("AskHigh");
								if (!askHigh.isEmpty() && askHigh != "")
									this->_overridedQuotations[key]->setAskHigh(askHigh);
								QString askLow = itemChildQDomNode.toElement().attribute("AskLow");
								if (!askLow.isEmpty() && askLow != "")
									this->_overridedQuotations[key]->setAskLow(askLow);
							}
						}
					}
					BusinessManager::getInstance()->getComputingManager()->computeQuotationOrAddComputingQuotetation(this->_overridedQuotations[key]);
				}
                else
                   QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<  " overridedQuotations not exist key:" << key;
			}
		}
	}


	QList<QUuid> QuotationManager::quotationReceived(const QString quotations)
	{
		//const QString overridedAttr = "Overrided";
        //QLOG_INFO(Common::Constants::LogKeyConstants::Business) << QString("QuotationManager::quotationReceived:%1").arg(quotations);
		QList<QUuid> instrumentIds;
		if (!quotations.isNull() && !quotations.isEmpty())
        {
			QStringList quotationList = quotations.split(Common::Constants::QuotationConstants::_OutterSeparator);
			foreach(QString quotationItem, quotationList)
			{
				if (quotationItem.isNull() || quotationItem.isEmpty() || quotationItem == "")
					continue;

				auto rowQuotationList = quotationItem.split(Common::Constants::QuotationConstants::_InnerSeparator);
				int i = 0;

				//InstrumentID:Ask:Bid:Timestamp:High:Low:"":Volume:TotalVolume:QuotePolicyID;
				/*QUuid instrumentId = Common::GuidMappingManager::getInstance()->get(rowQuotationList[i++].toInt());
				QString ask = rowQuotationList[i++];
				QString bid = rowQuotationList[i++];
				long totalSeconds = rowQuotationList[i++].toLong();
				QString high = rowQuotationList[i++];
				QString low = rowQuotationList[i++];*/

				//InstrumentID:Ask:Bid:High:Low:"":Timestamp:Volume:TotalVolume:QuotePolicyID
				QUuid instrumentId = Common::GuidMappingManager::getInstance()->getInstrumentGuidMapping(rowQuotationList[i++].toInt());
				if (instrumentId.isNull()) continue;
				QString ask = rowQuotationList[i++];
				QString bid = rowQuotationList[i++];
				QString high = rowQuotationList[i++];
				QString low = rowQuotationList[i++];
				i++;
				qlonglong totalTimes = rowQuotationList[i++].toLongLong();
				QDateTime orginTime = Common::Constants::QuotationConstants::_OrginTime;

				QDateTime timestamp = Common::AppSetting::getInstance()->getContainsMilSInQuotationTime() ? orginTime.addMSecs(totalTimes) : orginTime.addSecs(totalTimes);
				i++;
				i++;
				QUuid quotePolicyId;
				if (rowQuotationList.size() > 9)
				{
					quotePolicyId = Common::GuidMappingManager::getInstance()->getQuotePolicyGuidMapping(rowQuotationList[i++].toInt());
					//if(quotePolicyId.isNull()) quotePolicyId = Common::TraderBaseInfo::getInstance()->getCurrentQuotePolicyId();
				}
				else
				{
					//compatible old Quotation command
					quotePolicyId = Common::TraderBaseInfo::getInstance()->getCurrentQuotePolicyId();
				}

				QString askHigh = "";
				QString askLow = "";
				if (rowQuotationList.size() > 10)
				{
					askHigh = rowQuotationList[i++];
					askLow = rowQuotationList[i++];
				}

				if (quotePolicyId.isNull()) continue;

				QString key = quotePolicyId.toString() + instrumentId.toString();
				if (this->_overridedQuotations.contains(key))
				{
					if (timestamp < this->_overridedQuotations[key]->getUtcTimestamp())
					{
						QLOG_ERROR(Common::Constants::LogKeyConstants::UpdateQuotation) << 
						QString("Error Price: Id:%1  ask:%2  bid:%3  Quotation Time:%4  Last Time:%5  Last Utc Time:%6").arg(
								instrumentId.toString().mid(1, 36),
								ask, bid, 
								DateTimeHelper::toDateTimeString(timestamp),
								DateTimeHelper::toDateTimeString(this->_overridedQuotations[key]->getTimestamp()),
								DateTimeHelper::toDateTimeString(this->_overridedQuotations[key]->getUtcTimestamp()));
					}
					else
					{
						QString lastBid = this->_overridedQuotations[key]->getBid();
						if (bid.toDouble() > lastBid.toDouble())
							this->_overridedQuotations[key]->setPriceTrend(1);
						else if (bid.toDouble() < lastBid.toDouble())
							this->_overridedQuotations[key]->setPriceTrend(2);
						else
							this->_overridedQuotations[key]->setPriceTrend(0);
						this->_overridedQuotations[key]->setUtcTimestamp(timestamp);
						this->_overridedQuotations[key]->updateQuotation(ask, bid, high, low, askHigh, askLow, timestamp);

						BusinessManager::getInstance()->getComputingManager()->computeQuotationOrAddComputingQuotetation(this->_overridedQuotations[key]);
						instrumentIds.append(instrumentId);
//                        QLOG_INFO(Common::Constants::LogKeyConstants::Business)<<
//                                  QString("QuotationManager::quotationReceived:%1 ask:%2 bid:%3 ").arg(instrumentId.toString(),ask,bid);
					}
				}
				else
				{
					QSharedPointer<Business::Model::InitialQuotation> quotation(new Business::Model::InitialQuotation);
					quotation->setInstrumentID(instrumentId);
					quotation->setQuotePolicyID(quotePolicyId);
					quotation->setTimestamp(timestamp);
					quotation->setAsk(ask);
					quotation->setBid(bid);
					quotation->setUtcTimestamp(timestamp);
					if (!high.isEmpty() && !high.isNull())
					{
						quotation->setHigh(high);
					}
					if (!low.isEmpty() && !low.isNull())
					{
						quotation->setLow(low);
					}
					if (!askHigh.isEmpty() && !askHigh.isNull())
					{
						quotation->setAskHigh(askHigh);
					}
					if (!askLow.isEmpty() && !askLow.isNull())
					{
						quotation->setAskLow(askLow);
					}
					this->_overridedQuotations.insert(key, quotation);
					BusinessManager::getInstance()->getComputingManager()->computeQuotationOrAddComputingQuotetation(quotation);
					instrumentIds.append(instrumentId);
//                    QLOG_INFO(Common::Constants::LogKeyConstants::Business) <<
//                         QString("QuotationManager::quotationReceived:%1 ask:%2 bid:%3 ").arg(instrumentId.toString(),ask,bid);
				}
			}


            this->_lastQuotationReceivedTime = QDateTime::currentDateTime();
        }
		return instrumentIds;
	}
}
