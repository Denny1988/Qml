#include "TradingManager.h"
#include "SettingManager.h"
#include "InitializeManager.h"
#include "ComputingManager.h"
#include "Common/Util/PriceHelper.h"
#include "Common/SignalForwarder.h"
#include "Common/AppSetting.h"
#include "Business/Model/Trading/InstrumentAccountSummary.h"
#include "Common/TraderBaseInfo.h"
#include <QMutexLocker>
#include "Common/LogHelper.h"
#include "Common/UtcTimeManager.h"
#include "DateTimeHelper.h"
#include <QDebug>

namespace Business
{
	QMutex TradingManager::m_Mutex(QMutex::Recursive);
	TradingManager::TradingManager(QSharedPointer<SettingManager> settingManager, QSharedPointer<ComputingManager> computingManager, QSharedPointer<InitializeManager> initializeManager)
	{
		this->_settingManager = settingManager;
		this->_computingManager = computingManager;
		this->_initializeManager = initializeManager;
		this->_accountCodes = "";
		this->_resetAlertLevelAccountCodes = "";
	}

	bool TradingManager::initTradingData(const QByteArray&  tradingDataByteArray)
	{
		bool isSuessce = false;				
		QXmlStreamReader xmlStreamReader;
		int o = 0;
		xmlStreamReader.addData(tradingDataByteArray);
		xmlStreamReader.readNext();
		while (!xmlStreamReader.atEnd()) {
			if (xmlStreamReader.isStartElement()) {
				QString name = xmlStreamReader.name().toString();
				if (xmlStreamReader.name() == "Accounts") {
					xmlStreamReader.readNext();
					while (!xmlStreamReader.atEnd()) {
						readAccountElement(xmlStreamReader, this->_accountCodes, this->_resetAlertLevelAccountIds, this->_resetAlertLevelAccountCodes);
						if (o != 0 && o % 5 == 0 && this->_accountCodes != "")
						{
							this->_accountCodes.append(" ");
						}
						o++;
						xmlStreamReader.readNext();
						isSuessce = true;
					}
				}
				else {
					xmlStreamReader.raiseError(QObject::tr("Not a TradingData xml"));
				}
			}
			else {
				xmlStreamReader.readNext();
			}
		}
		
		return isSuessce;
	}

    bool TradingManager::refreshTradingData(const QByteArray&  tradingDataByteArray)
    {
        QMap<QUuid,QSharedPointer<Business::Model::Trading::Transaction>> transactions;
        QMap<QUuid,QSharedPointer<TradingOrder>> orders;
        foreach (QUuid id, TransactionIds) {
            transactions.insert(id, this->_transactions.value(id));
        }
        foreach (QUuid id, OrderIds) {
            orders.insert(id, this->_orders.value(id));
        }
        this->_transactions.clear();
        this->_orders.clear();
        bool isOk = this->initTradingData(tradingDataByteArray);
        foreach (QUuid id, TransactionIds) {
            if(!this->_transactions.contains(id))
            {
                auto transaction = transactions[id];
                this->_transactions.insert(id, transaction);
            }

        }
        foreach (QUuid id, OrderIds) {
            if(!this->_orders.contains(id))
            {
                auto order = orders[id];
                if(order->getIsOpen())
                {
                    order->getTransaction()->setPhase(Phase::Canceled);
                    order->setPhase(Phase::Canceled);
                    order->setCancelReason(CancelReason::DealerCanceled);
                    this->_orders.insert(id, order);
                }
                else
                {

                }
                // transaction->setPhase(Phase::Canceled);
            }
        }
        return isOk;
    }

	void TradingManager::startSetInitData()
	{
		this->setInitData();
		if (this->_accountCodes != "")
		{
            QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "TradingManager::initTradingData accountCodes:" + this->_accountCodes;
			Common::SignalForwarder::getInstance()->sendAccountMarginNotifySignal(this->_accountCodes, this->_resetAlertLevelAccountIds, this->_resetAlertLevelAccountCodes);
		}
		this->_accountCodes = "";
		this->_resetAlertLevelAccountIds.clear();
		this->_resetAlertLevelAccountCodes = "";
	}

	void TradingManager::calculateInstrumentSummary()
	{
		QMutexLocker locker(&TradingManager::m_Mutex);
		bool isMutilAcccount = this->_settingManager->getAccountCount() > 1 ? true : false;
		this->_instrumentSummaries.clear();
		//获得勾选账户,从保存里面拿
		QList<QUuid> accountList = Common::TraderBaseInfo::getInstance()->getCheckedAccounts();
		if (accountList.count() == 0)
		{
			foreach(auto item, this->_initializeManager->getAccounts())
			{
				accountList.append(item->_id);
			}
		}
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (order->getIsOpen() && order->getPhase() == Phase::Executed
				&& !order->getLotBalance().isZero()
				&& order->getTransaction()->getInstrumentCategory() != InstrumentCategory::Physical
				&& accountList.contains(order->getAccountId()))
			{
				QSharedPointer<InstrumentSummary> instrumentSummary;
				if (this->_instrumentSummaries.contains(order->getInstrumentId()))
				{
					instrumentSummary = this->_instrumentSummaries[order->getInstrumentId()];
				}
				else
				{
					QMap<QUuid, QSharedPointer<InstrumentSummaryPLFloat>> instrumentSummaryPLFloats;
					foreach(auto currency, this->_settingManager->getCurrencies())
					{
						QSharedPointer<InstrumentSummaryPLFloat> instrumentSummaryPLFloat(new InstrumentSummaryPLFloat(currency));
						instrumentSummaryPLFloats.insert(currency->getId(), instrumentSummaryPLFloat);
					}
					QSharedPointer<InstrumentSummary> newInstrumentSummary(new InstrumentSummary(instrumentSummaryPLFloats));
					instrumentSummary = newInstrumentSummary;
				}
				auto instrument = this->_initializeManager->getInstrument(order->getInstrumentId());
				instrumentSummary->setId(instrument->_id);
				instrumentSummary->setAccountId(order->getAccountId());
				instrumentSummary->setDescription(instrument->_description);
				this->_instrumentSummaries.insert(instrument->_id, instrumentSummary);

				instrumentSummary->setIsMultipleAccount(isMutilAcccount);
				auto settingInstrument = this->_settingManager->getInstrument(order->getAccountId(), order->getInstrumentId());
				instrumentSummary->UpdateBy(order, settingInstrument, true);
			}
		}
	}

	QList<QUuid> TradingManager::getShowFloatingByCurrency()
	{
		QList<QUuid> currencyIds;
		QList<QUuid> accountList = Common::TraderBaseInfo::getInstance()->getCheckedAccounts();
		if (accountList.count() == 0)
		{
			return currencyIds;
		}
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (order->getIsOpen() && order->getPhase() == Phase::Executed
				&& !order->getLotBalance().isZero()
				&& order->getTransaction()->getInstrumentCategory() != InstrumentCategory::Physical
				&& accountList.contains(order->getAccountId()))
			{
				QUuid id = order->getTransaction()->getInstrumentSummaryPLFloatCurrnecyId();
				if (!currencyIds.contains(id))
					currencyIds.append(id);
			}
		}
		return currencyIds;
	}

	void TradingManager::calculateInstrumentSummaryByInstrument(QUuid instrumentId)
	{
		QMutexLocker locker(&TradingManager::m_Mutex);
		if (!this->_instrumentSummaries.contains(instrumentId) || this->_instrumentSummaries.value(instrumentId).isNull())
			return;
		this->_instrumentSummaries[instrumentId]->clearPLFloat();
		QList<QUuid> accountList = Common::TraderBaseInfo::getInstance()->getCheckedAccounts();
		if (accountList.count() == 0)
		{
			foreach(auto item, this->_initializeManager->getAccounts())
			{
				accountList.append(item->_id);
			}
		}
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (order->getIsOpen() && order->getPhase() == Phase::Executed
				&& !order->getLotBalance().isZero()
				&& order->getTransaction()->getInstrumentCategory() != InstrumentCategory::Physical
				&& accountList.contains(order->getAccountId())
				&& order->getInstrumentId() == instrumentId)
			{
				QSharedPointer<InstrumentSummary> instrumentSummary;
				if (this->_instrumentSummaries.contains(order->getInstrumentId()))
				{
					instrumentSummary = this->_instrumentSummaries[order->getInstrumentId()];
					instrumentSummary->UpdateByFloat(order);
				}
			}
		}
	}

	void TradingManager::refreshOpenOrderButtonIsEnable()
	{
		QList<QUuid> accountList = Common::TraderBaseInfo::getInstance()->getCheckedAccounts();
		if (accountList.count() == 0)
		{
			foreach(auto item, this->_initializeManager->getAccounts())
			{
				accountList.append(item->_id);
			}
		}

		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (order->getIsOpen() && order->getPhase() == Phase::Executed
                && !order->getLotBalance().isZero()
				&& accountList.contains(order->getAccountId()))
			{
				this->setButtonIsEnable(order);
			}
		}
	}

	void TradingManager::setButtonIsEnable(QSharedPointer<TradingOrder> order)
	{
		auto limitStopPlaceSettingDetail = order->getTransaction()->getBusinessInstrument()->getPlaceSetting()->getPlaceSettingDetail(OrderType::Limit);
		if (!limitStopPlaceSettingDetail->getAllowTrade())
		{
			order->setCanLimitOrStop(false);
		}
		else
		{
			order->setCanLimitOrStop(((limitStopPlaceSettingDetail->getAllowLimitStop() &&
				((order->getLotBalanceForLimitOrderType() > 0.0 && order->getLotBalanceForStopOrderType() > 0.0)
					|| (order->getLotBalanceForLimitOrderType() > 0.0 && order->getLotBalanceForStopOrderType().isZero())
					|| (order->getLotBalanceForLimitOrderType().isZero() && order->getLotBalanceForStopOrderType() > 0.0)))
				|| (order->getLotBalanceForPendingOrderType() > 0.0 &&
				(limitStopPlaceSettingDetail->getAllowMarket() || limitStopPlaceSettingDetail->getAllowMarketOnOpen() || limitStopPlaceSettingDetail->getAllowMarketOnClose())
					)));
		}

		auto spotTradePlaceSettingDetail = order->getTransaction()->getBusinessInstrument()->getPlaceSetting()->getPlaceSettingDetail(OrderType::SpotTrade);
		if (!spotTradePlaceSettingDetail->getAllowTrade())
		{
			order->setCanLiquidation(false);
		}
		else
		{
			order->setCanLiquidation(order->getLotBalanceForDqOrderType() > 0.0);
		}
	}

    void TradingManager::refreshOpenOrderButtonIsEnableByAccount(const QString & accountCode, bool isShow)
	{
		if (isShow)
		{
			foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
			{
				if (order->getIsOpen() && order->getPhase() == Phase::Executed
                    && !order->getLotBalance().isZero()
					&& order->getTransaction()->getBusinessAccount()->getCode() == accountCode)
				{
					this->setButtonIsEnable(order);
				}
			}
		}
	}

	void TradingManager::updateTradingDataByNotifyReceived(const QDomNode& xmlTradingData)
	{
		QDomNode accountNode = xmlTradingData;
		QString accountIdStr = accountNode.toElement().attribute("ID");
		QUuid accountId = QUuid(accountIdStr);
		QString leverage = accountNode.toElement().attribute("Leverage");
		QSharedPointer<Fund> fund = this->getOrAddFund(accountId);
		auto oldAlertLevel = fund->getAlertLevel();
		if (!leverage.isNull() && !leverage.isEmpty())
		{
			this->_settingManager->updateLeverage(accountId, leverage.toInt());
		}
		QSharedPointer<Business::Model::Setting::BusinessAccount> settingAccount = this->_settingManager->getAccount(accountId);
		QDomNodeList accountChildlist = accountNode.childNodes();
        //int count = accountChildlist.count();

		QList<QUuid> orderExecuted;
		QList<QUuid> closeOrderForPending;
		QList<QUuid> orderIds;
		QList<QUuid> physicalInstrumentIds;
		bool isCompute = false;
		bool isRefreshPhysical = false;
		QString accountCodes = "";
		QStringList resetAlertLevelAccountIds;
		QString resetAlertLevelAccountCodes = "";
		auto checkAccounts = Common::TraderBaseInfo::getInstance()->getCheckedAccounts();
		QList<QUuid> addOrderToChart;
		QMap<QUuid, QUuid> removeOrderToChart;
		QList<QUuid> removeOrder;
		QList<QUuid> addOrderToWork;
		QList<QUuid> addComputingModel;
		QList<QUuid> addRefreshInstrumentPageFiFoStatus;
        QList<QUuid> todayExecuteOrder;
        QList<QUuid> ordersChanged;

		for (int i = 0; i < accountChildlist.count(); i++)
		{
			QDomNode accountItemQDomNode = accountChildlist.item(i);
			QString name = accountItemQDomNode.nodeName();
			if (name == "Transactions")
			{
				QDomNodeList transactionChildlist = accountItemQDomNode.childNodes();
				//int sdsdsd = transactionChildlist.count();
				for (int j = 0; j < transactionChildlist.count(); j++)
				{
					QUuid tranId = QUuid(transactionChildlist.at(j).toElement().attribute("ID"));
					QUuid instrumentId = QUuid(transactionChildlist.at(j).toElement().attribute("InstrumentID"));

					QSharedPointer<Model::Trading::Transaction> transaction = this->getOrAddTransaction(tranId);
					if (accountId.isNull()) continue;
					transaction->setAccountId(accountId);
					transaction->update(transactionChildlist.item(j));

					QSharedPointer<Business::Model::Setting::BusinessInstrument> settingInstrument = this->_settingManager->getInstrument(accountId, instrumentId);
					if (!settingAccount.isNull())
						transaction->setBusinessAccount(settingAccount);
					if (!settingInstrument.isNull())
						transaction->setBusinessInstrument(settingInstrument);
					else
					{
						settingInstrument = transaction->getBusinessInstrument();
						if (transaction.isNull() || settingInstrument.isNull()) continue;
						if (instrumentId.isNull())
							instrumentId = settingInstrument->getId();
					}

					QDomNodeList ordersChildlist = transactionChildlist.item(j).childNodes();
					QDomNode ordersQDomNode = ordersChildlist.item(0);
					QDomNodeList orderItemsChildlist = ordersQDomNode.childNodes();
					for (int k = 0; k < orderItemsChildlist.count(); k++)
					{
						QDomNode orderItemQDomNode = orderItemsChildlist.item(k);
						QUuid orderId = QUuid(orderItemQDomNode.toElement().attribute("ID"));
						QString isDeleted = orderItemQDomNode.toElement().attribute("IsDeleted");
						QString phaseStr = orderItemQDomNode.toElement().attribute("Phase");
						Phase::Phase phase = static_cast<Common::Enum::Phase::Phase>((phaseStr.isNull() || phaseStr.isEmpty() || phaseStr == "") ? -1 : phaseStr.toInt());
						QString orderCode = orderItemQDomNode.toElement().attribute("Code");
						QString executePrice = orderItemQDomNode.toElement().attribute("ExecutePrice");
						QString livePrice = orderItemQDomNode.toElement().attribute("LivePrice");

                        QLOG_INFO(Common::Constants::LogKeyConstants::Application) << " TradingManager::phaseStr: " + phaseStr;

						if ((isDeleted != "" && !isDeleted.isEmpty() && !isDeleted.isNull() && isDeleted == "True")
							|| phase == Phase::Deleted)
						{
							QSharedPointer<TradingOrder> order = this->getOrAddOrder(orderId);
                            if (!this->_workOrders.contains(orderId) && transaction->getSubmitTime() > _loginTime)
							{
								addOrderToWork.append(orderId);
							}
							order->update(orderItemQDomNode);
							this->deleteOrderFromCommand(orderId);
							if (!accountId.isNull())
								order->setAccountId(accountId);
							if (!instrumentId.isNull())
								order->setInstrumentId(instrumentId);
							order->setTransactionId(tranId);
							if (livePrice != "" && !livePrice.isEmpty() && !livePrice.isNull())
                                order->setLivePrice(Common::PriceHelper::toPrice(livePrice, transaction->getBusinessInstrument()));
							if (executePrice != "" && !executePrice.isEmpty() && !executePrice.isNull())
                                order->setExecutePrice(Common::PriceHelper::toPrice(executePrice, transaction->getBusinessInstrument()));
							order->setTransaction(transaction);
							orderIds.append(orderId);
							removeOrderToChart.insert(order->getId(), order->getInstrumentId());
							removeOrder.append(order->getId());
							continue;
						}

						orderIds.append(orderId);

                        bool isEmitPhaseChange = !this->_orders.contains(orderId);
						QSharedPointer<TradingOrder> order = this->getOrAddOrderByUpdateCommand(orderId);
                        if (isEmitPhaseChange && transaction->getSubmitTime() > _loginTime)
                        {
                            addOrderToWork.append(orderId);
                        }
						if (this->_removeOpenOrders.contains(orderId))
						{
							//if (!addOrderToWork.contains(orderId)) addOrderToWork.append(orderId);
							this->_workOrders.insert(order->getId(), order);
							this->_removeOpenOrders.remove(orderId);
							if (!orderExecuted.contains(order->getId()))
                            {
								orderExecuted.append(order->getId());
                                ordersChanged.append(order->getId());
                            }
						}
						switch (phase)
						{
						case Common::Enum::Phase::Placed:
							Common::SignalForwarder::getInstance()->sendPlaySoundsSignal(Common::Enum::SoundsEnum::Placed);
							break;
						case Common::Enum::Phase::Canceled:
							Common::SignalForwarder::getInstance()->sendPlaySoundsSignal(Common::Enum::SoundsEnum::Cancel);
							break;
						case Common::Enum::Phase::Executed:
							Common::SignalForwarder::getInstance()->sendPlaySoundsSignal(Common::Enum::SoundsEnum::Execute);
                            if(transaction->getExecuteTime().date() == this->_settingManager->getTradeDay()->getCurrentDay().date())
                                todayExecuteOrder.append(orderId);
                            break;
                        case Common::Enum::Phase::Completed:
                            if(transaction->getExecuteTime().date() == this->_settingManager->getTradeDay()->getCurrentDay().date())
                                todayExecuteOrder.append(orderId);
                            break;
						default:
							break;
						}

						if (phase != Common::Enum::Phase::None && phase != order->getPhase())
							isEmitPhaseChange = true;
						if (!accountId.isNull())
							order->setAccountId(accountId);
						if (!instrumentId.isNull())
							order->setInstrumentId(instrumentId);
						order->setTransactionId(tranId);
						if (livePrice != "" && !livePrice.isEmpty() && !livePrice.isNull())
                            order->setLivePrice(Common::PriceHelper::toPrice(livePrice, transaction->getBusinessInstrument()));
						if (executePrice != "" && !executePrice.isEmpty() && !executePrice.isNull())
                            order->setExecutePrice(Common::PriceHelper::toPrice(executePrice, transaction->getBusinessInstrument()));
						order->setTransaction(transaction);
						auto oldLotBalance = order->getLotBalance();
						order->update(orderItemQDomNode);
						order->setIsPayoff();


						if (isEmitPhaseChange)
						{
							Common::SignalForwarder::getInstance()->sendPhaseChangedSignal(order->getId(), phase);
							if (phase == Phase::Executed && order->getIsOpen() && !order->getLotBalance().isZero())
							{
								if (!addRefreshInstrumentPageFiFoStatus.contains(order->getInstrumentId()))
									addRefreshInstrumentPageFiFoStatus.append(order->getInstrumentId());
								if (!orderExecuted.contains(order->getId()))
                                {
									orderExecuted.append(order->getId());
                                    ordersChanged.append(order->getId());
                                }
								if (checkAccounts.contains(order->getAccountId()))
									addOrderToChart.append(order->getId());
							}
							if (checkAccounts.contains(order->getAccountId()))
							{
								if (phase == Phase::Placed && order->getCategory(order->getTransaction()->getOrderType()) == OrderTypeCategory::Pending)
								{
									addOrderToChart.append(order->getId());
								}
								if (phase == Phase::Canceled || phase == Phase::Deleted)
								{
									removeOrderToChart.insert(order->getId(), order->getInstrumentId());
									removeOrder.append(order->getId());
								}
								if (order->getIsOpen() && order->getPhase() == Phase::Executed && order->getLotBalance().isZero())
								{
									removeOrderToChart.insert(order->getId(), order->getInstrumentId());
									removeOrder.append(order->getId());
								}
								if (order->getPhase() == Phase::Executed && order->getLotBalance().isZero())
								{
									if (!addRefreshInstrumentPageFiFoStatus.contains(order->getInstrumentId()))
										addRefreshInstrumentPageFiFoStatus.append(order->getInstrumentId());
								}
								if (!order->getIsOpen() && order->getPhase() == Phase::Executed)
								{
									removeOrder.append(order->getId());
								}
							}
						}
						else
						{
							if (order->getIsOpen() && order->getPhase() == Phase::Executed && order->getLotBalance().isZero())
							{
								removeOrderToChart.insert(order->getId(), order->getInstrumentId());
								removeOrder.append(order->getId());
							}
						}

						if (oldLotBalance.isZero() && !order->getLotBalance().isZero() && phase != Phase::Placing)
						{
							if (!orderExecuted.contains(order->getId()))
                            {
								orderExecuted.append(order->getId());
                                ordersChanged.append(order->getId());
                            }
						}

                        if(oldLotBalance != order->getLotBalance())
                            ordersChanged.append(order->getId());

						if (transaction->getInstrumentCategory() == Common::Enum::InstrumentCategory::Physical
							&& order->getPhase() == Phase::Executed
							&& oldLotBalance.isZero()
							&& !order->getLotBalance().isZero())
						{
							//cancel delivery
							if (!addRefreshInstrumentPageFiFoStatus.contains(order->getInstrumentId()))
								addRefreshInstrumentPageFiFoStatus.append(order->getInstrumentId());
							if (!orderExecuted.contains(order->getId()))
                            {
								orderExecuted.append(order->getId());
                                ordersChanged.append(order->getId());
                            }
							if (checkAccounts.contains(order->getAccountId()) && !addOrderToChart.contains(order->getId()))
								addOrderToChart.append(order->getId());
						}
						if ((phase == Phase::Placed || phase == Phase::Placing)
							&& !order->getIsOpen()
							&& order->getCategory(order->getTransaction()->getOrderType()) == OrderTypeCategory::Pending)
						{
							closeOrderForPending.append(order->getId());
						}
						//if (!this->_workOrders.contains(order->getId()) && (isEmitPhaseChange && order->getIsNewAddOrder()))
						//{
						//	//if (!addOrderToWork.contains(orderId)) addOrderToWork.append(orderId);
						//	if (!this->_workOrders.contains(order->getId()) &&  !order->isBOOrder()) this->_workOrders.insert(order->getId(), order);
						//}
						if (order->isBOOrder() && !this->_boOrders.contains(order->getId()))
						{
							this->_boOrders.insert(order->getId(), order);
						}
						if (phase == Phase::Executed)
							isCompute = true;

						if (order->getIsOpen() && order->getPhase() == Phase::Executed
							&& order->getLotBalance().toDouble() > 0.0)
						{
							addComputingModel.append(order->getId());
						}

						if ((order->getPhase() == Phase::Placing || order->getPhase() == Phase::Placed) &&
							this->_pendingTransactionIds.contains(transaction->getId()))
							this->_pendingTransactionIds.append(transaction->getId());
					}

					if (transaction->getInstrumentCategory() == Common::Enum::InstrumentCategory::Physical)
					{
						isRefreshPhysical = true;
						physicalInstrumentIds.append(instrumentId);
					}
				}
			}
			else if (name == "DeliveryRequests")
			{
				QDomNodeList deliveryRequestChildlist = accountItemQDomNode.childNodes();
				for (int j = 0; j < deliveryRequestChildlist.count(); j++)
				{
					QUuid id = QUuid(deliveryRequestChildlist.item(j).toElement().attribute("Id"));
					QString status = deliveryRequestChildlist.item(j).toElement().attribute("Status");
					bool isAdd = false;
					if (!this->_deliveryRequests.contains(id) && status != "100")
						isAdd = true;
					QSharedPointer<DeliveryRequest> deliveryRequest = this->getOrAddDeliveryRequest(id);
					deliveryRequest->setAccountId(accountId);
					deliveryRequest->update(deliveryRequestChildlist.item(j));
					auto settingInstrument = this->_settingManager->getInstrument(accountId, deliveryRequest->getInstrumentId());
					if (!settingInstrument.isNull())
					{
						deliveryRequest->setUnit(settingInstrument->getUnit());
					}
					this->refreshPendingInventoryFromDeliveryRequest(deliveryRequest);
					if (isAdd)
					{
						Common::SignalForwarder::getInstance()->sendAddPendingInventorySignal(id);
					}
				}
				isCompute = true;
			}
			else if (name == "ScrapDeposits")
			{
				QDomNodeList scrapDepositChildlist = accountItemQDomNode.childNodes();
				for (int j = 0; j < scrapDepositChildlist.count(); j++)
				{
					QUuid id = QUuid(scrapDepositChildlist.item(j).toElement().attribute("Id"));
					QString status = scrapDepositChildlist.item(j).toElement().attribute("Status");
					bool isAdd = false;
					if (!this->_scrapDeposits.contains(id) && status != "100")
						isAdd = true;
					QSharedPointer<ScrapDeposit> scrapDeposit = this->getOrAddScrapDeposit(id);
					scrapDeposit->setAccountId(accountId);
					scrapDeposit->update(scrapDepositChildlist.item(j));
					this->refreshPendingInventoryFromScrapDeposit(scrapDeposit);
					if (isAdd)
					{
						Common::SignalForwarder::getInstance()->sendAddPendingInventorySignal(id);
					}
				}
			}
			else if (name == "Funds")
			{
				QDomNodeList subFundChildlist = accountItemQDomNode.childNodes();
				for (int j = 0; j < subFundChildlist.count(); j++)
				{
					QUuid currencyId = QUuid(subFundChildlist.item(j).toElement().attribute("CurrencyID"));
					bool isAdd = false;
					if (this->_subFunds.contains(accountId))
					{
						if (!this->_subFunds[accountId].contains(currencyId))
							isAdd = true;
					}
					else
						isAdd = true;

					QSharedPointer<SubFund> subFund = this->getOrAddSubFund(accountId, currencyId);
					subFund->setAccountId(accountId);
					subFund->update(subFundChildlist.item(j));
					auto currency = this->_settingManager->getCurrency(currencyId);
					if (!currency.isNull())
					{
						subFund->setCurrencyCode(currency->getCode(), currency->getName());
						subFund->setDecimals(currency->getDecimals());
					}
					if (subFund->getBalance().isZero() && subFund->getTradePLFloat().isZero())
					{
                        //this->_subFunds[accountId].remove(currencyId);
                        qDebug() << " subFunds remove currencyId :" << currencyId.toString() << " by Balance is zero and tradePLFloat is Zero";
					}
					if (currencyId == settingAccount->getCurrencyId())
						subFund->setCreditAmount(settingAccount->getCreditAmount());
					if (isAdd)
						Common::SignalForwarder::getInstance()->sendAddSubFundSignal(accountId.toString(), currencyId.toString());
				}
				isCompute = true;
			}
			else if (name == "AccountBalance")
			{
				QUuid currencyId = QUuid(accountItemQDomNode.toElement().attribute("CurrencyId"));
				QDecNumber balance = QDecNumber::fromQString(accountItemQDomNode.toElement().attribute("Balance"));
				if (this->_funds.contains(accountId))
				{
					bool isAdd = false;
					if (this->_subFunds.contains(accountId))
					{
						if (!this->_subFunds[accountId].contains(currencyId))
							isAdd = true;
					}
					else
						isAdd = true;

					QSharedPointer<SubFund> subFund = this->getOrAddSubFund(accountId, currencyId);
					if (!subFund.isNull())
					{
						subFund->addBalance(balance);
					}
					QUuid targetCurrencyId = settingAccount->getCurrencyId();
					QSharedPointer<Model::Setting::CurrencyRate> currencyRate = this->_settingManager->getCurrencyRate(currencyId, targetCurrencyId);
					if (!currencyRate.isNull())
					{
						fund->addBalance(currencyRate->exchange(balance));
					}
					if (subFund->getBalance().isZero() && subFund->getTradePLFloat().isZero())
					{
                        //this->_subFunds[accountId].remove(currencyId);
                        qDebug() << " subFunds remove currencyId :" << currencyId.toString() << " by Balance is zero and tradePLFloat is Zero";
					}

					if (currencyId == settingAccount->getCurrencyId())
						subFund->setCreditAmount(settingAccount->getCreditAmount());

					if (isAdd)
						Common::SignalForwarder::getInstance()->sendAddSubFundSignal(accountId.toString(), currencyId.toString());
				}
			}
		}
		if (!settingAccount.isNull())
		{
			if (this->_funds.contains(accountId))
			{
				fund->update(accountNode);
				fund->setCreditAmount(settingAccount->getCreditAmount());
				fund->setRatioIncludeCredit(this->_settingManager->getSystemSetting()->getRatioIncludeCredit());
				auto currency = Business::BusinessManager::getInstance()->getSettingManager()->getCurrency(settingAccount->getCurrencyId());
				if (!currency.isNull())
				{
					fund->setCurrencyCode(currency->getId(), currency->getCode(), currency->getName());
					fund->setDecimals(currency->getDecimals());
				}
				if (oldAlertLevel != fund->getAlertLevel() && fund->needAlert(settingAccount->getDisplayAlert()))
				{
					accountCodes.append(settingAccount->getCode());

					if (this->_settingManager->isAllowTraderResetAlertLevel(settingAccount->getForbiddenAlert()))
					{
						if (resetAlertLevelAccountCodes != "")
							resetAlertLevelAccountCodes.append(";");
						resetAlertLevelAccountCodes.append(settingAccount->getCode());
						resetAlertLevelAccountIds.append(accountIdStr);
					}
				}
			}
			if (orderIds.count() > 0)
			{
				QList<QSharedPointer<TradingOrder>> needRefreshOpenOrderButtonIsEnable;
				foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
				{
					if (!orderIds.contains(order->getId())) continue;
					QList<QSharedPointer<Business::Model::Trading::OrderRelation>> orderRations = order->getOrderRelations();
					for (int i = 0; i < orderRations.length(); i++)
					{
						if (!this->_orders.contains(orderRations.at(i)->getOrderId1())) continue;
						QSharedPointer<TradingOrder> openOrder = this->_orders[orderRations.at(i)->getOrderId1()];
						if (!openOrder.isNull() && !openOrder->getTransactionId().isNull() && !openOrder->getAccountId().isNull())
						{
							if (order->getPhase() == Phase::Canceled
								|| order->getPhase() == Phase::Deleted)
							{
								openOrder->removeCloseOrder(order->getId());
							}
							else
							{
								if (order->getPhase() != Phase::Placing
									&& order->getPhase() != Phase::Placed)
									openOrder->removeCloseOrder(order->getId());
								else
									openOrder->addCloseOrder(order);
							}

							if (openOrder->getPhase() == Phase::Executed)
							{
								openOrder->caculateLimitSummary();

								QString openOrderInfo = "";
                                if(openOrder->getPhase() == Phase::Executed || openOrder->getPhase() == Phase::Completed)
                                {
                                    openOrderInfo.append(DateTimeHelper::toDateString(openOrder->getExecuteTradeDay()));
                                    openOrderInfo.append("x");
                                    openOrderInfo.append(Common::roundToString(orderRations.at(i)->getLot().toDouble(), openOrder->getDecimalPalcesForLot()));
                                    openOrderInfo.append("x");
                                    openOrderInfo.append(openOrder->getExecutePrice().toQString());
                                }
                                else
                                {
                                    QSharedPointer<Business::Model::Trading::Transaction> transaction = openOrder->getTransaction();
                                    openOrderInfo.append(DateTimeHelper::toDateString(transaction->getSubmitTime()));
                                    openOrderInfo.append("x");
                                    openOrderInfo.append(Common::roundToString(orderRations.at(i)->getLot().toDouble(), openOrder->getDecimalPalcesForLot()));
                                    openOrderInfo.append("x");
                                    openOrderInfo.append(openOrder->getSetPrice());
                                }
//								if (!openOrder->getLotBalance().isZero() && openOrder->getIsOpen())
//								{
//									openOrderInfo.append(DateTimeHelper::toDateString(openOrder->getExecuteTradeDay()));
//									openOrderInfo.append("x");
//									openOrderInfo.append(Common::roundToString(orderRations.at(i)->getLot().toDouble(), openOrder->getDecimalPalcesForLot()));
//									openOrderInfo.append("x");
//									openOrderInfo.append(openOrder->getExecutePrice().toQString());
//								}
//								else
//								{
//									openOrderInfo = DateTimeHelper::toDateString(openOrder->getTransaction()->getSubmitTime());
//								}
								orderRations.at(i)->setOpenOrderInfo(openOrderInfo);
							}
							if (openOrder->getTransaction()->getOrderType() == OrderType::BinaryOption)
							{
								openOrder->setTradePL(orderRations.at(i)->getTradePL());
							}
							needRefreshOpenOrderButtonIsEnable.append(openOrder);
						}
					}


					if (order->getIsOpen() && order->getPhase() == Phase::Executed
						&& !order->getLotBalance().isZero())
					{
						needRefreshOpenOrderButtonIsEnable.append(order);
					}
				}

				foreach(auto order, needRefreshOpenOrderButtonIsEnable)
				{
					this->setButtonIsEnable(order);
				}

				this->calculateInstrumentSummary();
				Common::SignalForwarder::getInstance()->sendCalculateInstrumentSummarySignal();
			}

			foreach(auto item, addComputingModel)
			{
				auto order = this->_orders[item];
				this->_computingManager->addComputingModel(order);
			}

			if (isCompute)
			{
				this->_computingManager->computePart(accountId);
                this->computeMarketingGroupFund(settingAccount->getSalesId().toString() + settingAccount->getCurrencyId().toString());
				Common::SignalForwarder::getInstance()->sendComputeAccountSignal();
			}

			if (isRefreshPhysical)
			{
				this->refreshPhysicalDataByInstruments(physicalInstrumentIds);
			}

			foreach(auto item, orderExecuted)
			{
                QLOG_INFO(Common::Constants::LogKeyConstants::Application) << " TradingManager orderExecuted Order Id:" << item;
                #if defined (RUNTIME_OS_IPHONE) || defined (RUNTIME_OS_ANDROID)
                QSharedPointer<Business::Model::Trading::TradingOrder> tradingOrder = this->getOrder(item);
                if(tradingOrder->getTransaction()->getInstrumentCategory() == InstrumentCategory::Physical
                        && !tradingOrder->getLotBalance().isZero())
                {
                    if(tradingOrder->getPhysicalTradeSide() == PhysicalTradeSide::ShortSell)
                        this->addShortSell(item);
                    else
                    {
                        if(tradingOrder->getIsOpen() && tradingOrder->getTransaction()->getOrderType() != OrderType::BinaryOption)
                            this->addInventory(item);
                    }
                }
                #endif
                Common::SignalForwarder::getInstance()->sendOrderExecutedSignal(item);
			}
			foreach(auto item, closeOrderForPending)
			{
                QLOG_INFO(Common::Constants::LogKeyConstants::Application) << " TradingManager closeOrderForPending Order Id:" << item;
				Common::SignalForwarder::getInstance()->sendCloseOrderForPendingSignal(item);
			}

			if (accountCodes != "")
			{				
				QLOG_WARN(Common::Constants::LogKeyConstants::Application) << " updateTradingDataByNotifyReceived  Account Margin Notify accountCodes:" << accountCodes;
				Common::SignalForwarder::getInstance()->sendAccountMarginNotifySignal(accountCodes, resetAlertLevelAccountIds, resetAlertLevelAccountCodes);
			}

			if (addOrderToChart.count() > 0)
			{
                QLOG_INFO(Common::Constants::LogKeyConstants::Application) << " TradingManager::sendAddOrdersToChartSignal start";
                foreach(auto item, addOrderToChart)
                    QLOG_INFO(Common::Constants::LogKeyConstants::Application) << " TradingManager AddOrdersToChart Order Id:" << item;
				Common::SignalForwarder::getInstance()->sendAddOrdersToChartSignal(addOrderToChart);
			}
			if (removeOrderToChart.count() > 0)
			{
				Common::SignalForwarder::getInstance()->sendRemoveOrdersToChartSignal(removeOrderToChart);
			}
			if (removeOrder.count() > 0)
			{
                QLOG_INFO(Common::Constants::LogKeyConstants::Application) << " TradingManager::sendRemoveOrderSignal start";
                foreach(auto item, removeOrder)
                    QLOG_INFO(Common::Constants::LogKeyConstants::Application) << " TradingManager Remove Order Id:" << item;
				Common::SignalForwarder::getInstance()->sendRemoveOrderSignal(removeOrder);
			}
			if (addRefreshInstrumentPageFiFoStatus.count() > 0)
			{
				Common::SignalForwarder::getInstance()->sendRefreshPageFiFoStatusSignal(addRefreshInstrumentPageFiFoStatus);
			}

			foreach(auto item, addOrderToWork)
			{
				auto order = this->_orders[item];
				if (!this->_workOrders.contains(item) && !order->isBOOrder())
					this->_workOrders.insert(item, order);
				Common::SignalForwarder::getInstance()->sendAddDataSignal(item, UpdateType::WorkOrder);
			}

            //for mobile
            foreach(auto item, todayExecuteOrder)
            {
                QLOG_INFO(Common::Constants::LogKeyConstants::Application) << " TradingManager todayExecuteOrder Order Id:" << item;
                Common::SignalForwarder::getInstance()->sendTodayExecuteOrderSignal(item);
            }
            //for mobile
            if (ordersChanged.count() > 0)
            {
                Common::SignalForwarder::getInstance()->sendOrdersChangedSignal(ordersChanged);
            }


		}
		else
		{
			QLOG_ERROR( Common::Constants::LogKeyConstants::Business) << " updateTradingDataByNotifyReceived  settingAccount isNull";
		}
	}

	void TradingManager::deleteOrderFromCommand(QUuid id)
	{
		if (this->_orders.contains(id))
		{
			auto order = this->_orders[id];
			order->setPhase(Phase::Deleted);
			if (!this->_workOrders.contains(order->getId()))
				this->_workOrders.insert(order->getId(), order);
			QUuid physicalRequestId = order->getPhysicalRequestId();
			if (this->_deliveryRequests.contains(physicalRequestId))
			{
				QSharedPointer<DeliveryRequest> deliveryRequest = this->_deliveryRequests[physicalRequestId];
				deliveryRequest->setStatus(DeliveryRequestStatus::Canceled);
				if (this->_pendingInventories.contains(physicalRequestId))
					this->_pendingInventories[physicalRequestId]->update(deliveryRequest);
			}
			if (this->_scrapDeposits.contains(physicalRequestId))
			{
				QSharedPointer<ScrapDeposit> scrapDeposit = this->_scrapDeposits[physicalRequestId];
				scrapDeposit->setStatus(DeliveryRequestStatus::Canceled);
				if (this->_pendingInventories.contains(physicalRequestId))
					this->_pendingInventories[physicalRequestId]->update(scrapDeposit);
			}
			this->_computingManager->deleteComputingModel(order);
		}
	}

	QSharedPointer<AccountCardResult> TradingManager::getAccountCardResult(const QUuid& accountId)
	{
		QSharedPointer<AccountCardResult> accountCardResult(new AccountCardResult);
		accountCardResult->accountId = accountId;
		QSharedPointer<Model::Setting::BusinessAccount> settingAccount = this->_settingManager->getAccount(accountId);
		if (settingAccount.isNull())
		{
			QSharedPointer<AccountCardResult> newAccountCardResult(Q_NULLPTR);
			return newAccountCardResult;
		}
		accountCardResult->accountName = settingAccount->getName();
		accountCardResult->isNoShowAccountStatus = this->_settingManager->getSystemSetting()->getIsNoShowAccountStatus();
		accountCardResult->salesCode = settingAccount->getSalesCode();
		//this->_computingManager->getAccountComputingResult(accountId);
		if (this->_funds.contains(accountId))
		{
			accountCardResult->fund = this->_funds.value(accountId);
			accountCardResult->fund->setDisplayAlert(settingAccount->getDisplayAlert());
			if (settingAccount->getIsMultiCurrency())
				accountCardResult->subFund = this->_subFunds.value(accountId);
		}
		return accountCardResult;
	}

	QSharedPointer<Fund> TradingManager::getOrAddFund(const QUuid& accountId)
	{
		if (this->_funds.contains(accountId))
		{
			//this->_computingManager->getAccountComputingResult(accountId);
			return this->_funds[accountId];
		}
		QSharedPointer<Fund> accountBalance(new Fund);
		this->_funds.insert(accountId, accountBalance);
		return accountBalance;
	}

	QSharedPointer<Fund> TradingManager::getFund(const QUuid& accountId)
	{
		if (this->_funds.contains(accountId))
		{
			return this->_funds[accountId];
		}
		QSharedPointer<Fund> accountBalance(NULL);
		return accountBalance;
	}

	QSharedPointer<SubFund> TradingManager::getOrAddSubFund(const QUuid& accountId, const QUuid& currencyId)
	{
		QMap<QUuid, QSharedPointer<SubFund>> accountCurrencies;
		if (this->_subFunds.contains(accountId))
		{
			accountCurrencies = this->_subFunds.value(accountId);
			if (accountCurrencies.contains(currencyId))
                return this->_subFunds[accountId][currencyId];
			else
			{
				QSharedPointer<SubFund> accountCurrency(new SubFund);
				this->_subFunds[accountId].insert(currencyId, accountCurrency);
				return accountCurrency;
			}
		}
		QSharedPointer<SubFund> accountCurrency(new SubFund);
		accountCurrencies.insert(currencyId, accountCurrency);
		this->_subFunds.insert(accountId, accountCurrencies);
		return accountCurrency;
	}

	QSharedPointer<SubFund> TradingManager::getSubFund(const QUuid& accountId, const QUuid& currencyId)
	{
		if (this->_subFunds.contains(accountId))
		{
			auto accountCurrencies = this->_subFunds.value(accountId);
			if (accountCurrencies.contains(currencyId))
                return this->_subFunds[accountId][currencyId];
		}
		QSharedPointer<SubFund> accountCurrency(NULL);
		return accountCurrency;
	}

	QMap<QUuid, QSharedPointer<SubFund>> TradingManager::getSubFunds(const QUuid& accountId)
	{
		if (this->_subFunds.contains(accountId))
		{
			return this->_subFunds.value(accountId);
		}
		return QMap<QUuid, QSharedPointer<SubFund>>();
	}

	QSharedPointer<TradingOrder> TradingManager::getOrAddOrder(const QUuid& id)
	{
		if (this->_orders.contains(id))
		{
			return this->_orders.value(id);
		}
		QSharedPointer<TradingOrder> order(new TradingOrder);
		order->setId(id);
		this->_orders.insert(id, order);
		return order;
	}

	QSharedPointer<TradingOrder> TradingManager::getOrAddOrderByUpdateCommand(const QUuid& id)
	{
		if (this->_orders.contains(id))
		{
			return this->_orders[id];
		}
		if (this->_removeOpenOrders.contains(id))
		{
			this->_orders.insert(id, this->_removeOpenOrders[id]);
			this->_orders[id]->clearData();
			return this->_orders[id];
		}
		QSharedPointer<TradingOrder> order(new TradingOrder);
		order->setId(id);
		this->_orders.insert(id, order);
		return order;
	}

	QMap<QUuid, QSharedPointer<TradingOrder>> TradingManager::getOpenOrders(QList<QUuid> accountIds)
	{
		QMap<QUuid, QSharedPointer<TradingOrder>> openOrders;
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (accountIds.contains(order->getAccountId())
				&& order->getIsOpen() && order->getPhase() == Phase::Executed
				&& !order->getLotBalance().isZero()
				&& order->getTransaction()->getInstrumentCategory() != InstrumentCategory::Physical
				&& order->getBOBetTypeId().isNull())
			{
				openOrders.insert(order->getId(), order);
			}
		}
		return openOrders;
	}

	QMap<QUuid, QSharedPointer<TradingOrder>> TradingManager::getOpenOrders(QUuid accountId, QUuid instrumentId)
	{
		QMap<QUuid, QSharedPointer<TradingOrder>> openOrders;
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (order->getAccountId() == accountId && order->getInstrumentId() == instrumentId
				&& order->getIsOpen() && order->getPhase() == Phase::Executed
				&& !order->getLotBalance().isZero()
				&& order->getBOBetTypeId().isNull())
			{
				openOrders.insert(order->getId(), order);
			}
		}
		return openOrders;
    }

	QMap<QUuid, QSharedPointer<TradingOrder>> TradingManager::getOpenOrdersByQuote(QUuid accountId, QUuid instrumentId)
	{
		QMap<QUuid, QSharedPointer<TradingOrder>> openOrders;
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (order->getAccountId() == accountId && order->getInstrumentId() == instrumentId
				&& order->getIsOpen() && order->getPhase() == Phase::Executed
				&& !order->getLotBalance().isZero()
				&& order->getBOBetTypeId().isNull())
			{
				openOrders.insert(order->getId(), order);
			}
		}
		return openOrders;
	}

	int TradingManager::getOpenOrdersCount(QUuid accountId, QUuid instrumentId)
	{
		/*QString s1 = accountId.toString();
		QString s2 = instrumentId.toString();*/
		int count = 0;
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			/*QString s3 = order->getAccountId().toString();
			QString s4 = order->getInstrumentId().toString();
			bool o = order->getIsOpen();
            int p = order->getPhase();
            double l1 = order->getLotBalance().toDouble();*/
			if (order->getAccountId() == accountId
				&& order->getInstrumentId() == instrumentId
				&& order->getIsOpen()
				&& order->getPhase() == Phase::Executed
				&& !order->getLotBalance().isZero())
			{
				count++;
			}
		}
		return count;
	}

	bool TradingManager::canPlaceFifoOrder(QUuid accountId, QUuid instrumentId)
	{
		auto account = this->_settingManager->getAccount(accountId);
		if (account.isNull()) return false;
		auto instrument = account->getInstrument(instrumentId);
		if (instrument.isNull()) return false;
		bool isAutoClose = instrument->getPlaceSetting()->getIsAutoClose();
		return !isAutoClose && this->getOpenOrdersCount(accountId, instrumentId) > 0;
	}

	QMap<QUuid, QSharedPointer<TradingOrder>> TradingManager::getOpenOrders(QUuid instrumentId)
	{
		QMap<QUuid, QSharedPointer<TradingOrder>> openOrders;
		QList<QUuid> accountIds = Common::TraderBaseInfo::getInstance()->getCheckedAccounts();
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (accountIds.contains(order->getAccountId())
				&& order->getInstrumentId() == instrumentId
				&& order->getIsOpen() && order->getPhase() == Phase::Executed
				&& order->getLotBalance().toDouble() > 0.0
				&& order->getTransaction()->getInstrumentCategory() != InstrumentCategory::Physical
				&& order->getBOBetTypeId().isNull())
			{
				openOrders.insert(order->getId(), order);
			}
		}
		return openOrders;
	}

	QMap<QUuid, QSharedPointer<TradingOrder>> TradingManager::getUnConfirmPendingOrders()
	{
		QMap<QUuid, QSharedPointer<TradingOrder>> unConfirmPendingOrders;
        foreach(QSharedPointer<TradingOrder> order, this->_workOrders.values())
		{
            if ((order->getPhase() == Phase::Placed ||
                 (order->getPhase() == Phase::Placing && order->getTransaction()->getSubType() == TransactionSubType::IfDone))
               && order->getBOBetTypeId().isNull())
			{
				unConfirmPendingOrders.insert(order->getId(), order);
			}
		}
		return unConfirmPendingOrders;
	}

	QList<QSharedPointer<TradingOrder>> TradingManager::getOpenOrders()
	{
		QList<QSharedPointer<TradingOrder>> openOrders;
		QMapIterator<QUuid, QSharedPointer<TradingOrder>> item(this->_orders);
		while (item.hasNext()) {
			item.next();
			QSharedPointer<TradingOrder> order = item.value();
			if (order.isNull())
			{
				continue;
			}
			if (order->getIsOpen() && order->getPhase() == Phase::Executed
				&& order->getLotBalance().toDouble() > 0.0
				&& order->getBOBetTypeId().isNull())
			{
				openOrders.append(order);
			}
		}
		return openOrders;
	}

	QList<QSharedPointer<TradingOrder>> TradingManager::getOpenOrdersNotContainPhysical()
    {
		QList<QSharedPointer<TradingOrder>> openOrders;
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
        {
			if (order->getIsOpen() && order->getPhase() == Phase::Executed
				&& order->getTransaction()->getInstrumentCategory() != InstrumentCategory::Physical
				&& order->getLotBalance().toDouble() > 0.0
				&& order->getBOBetTypeId().isNull())
            {
				openOrders.append(order);
			}
		}
		return openOrders;
	}

	QList<QSharedPointer<TradingOrder>> TradingManager::getBOOrders()
	{
		QList<QSharedPointer<TradingOrder>> openOrders;
		foreach(QSharedPointer<TradingOrder> order, this->_boOrders.values())
		{
			if (order->getIsOpen()
				&& !order->getLotBalance().isZero()
				&& !order->getBOBetTypeId().isNull())
			{
				openOrders.append(order);
			}
		}
		return openOrders;
	}

	QMap<QUuid, QSharedPointer<TradingOrder>> TradingManager::getWorkOrders(QList<QUuid> accountIds)
	{
		QMap<QUuid, QSharedPointer<TradingOrder>> workOrders;
		foreach(QSharedPointer<TradingOrder> order, this->_workOrders.values())
		{
			if (accountIds.contains(order->getAccountId()))
			{
				workOrders.insert(order->getId(), order);
			}
		}
		return workOrders;
	}

	QMap<QUuid, QSharedPointer<TradingOrder>> TradingManager::getWorkOrders()
	{
		return this->_workOrders;
	}

    bool variantLessThanWorkOrder(QSharedPointer<TradingOrder> v1, QSharedPointer<TradingOrder> v2)
    {
        return (v1->getPhase() == Phase::Executed ? v1->getExecuteTradeDay() : v1->getTransaction()->getSubmitTime())
                >
                (v2->getPhase() == Phase::Executed ? v2->getExecuteTradeDay() : v2->getTransaction()->getSubmitTime());
    }

	QMap<QUuid, QSharedPointer<TradingOrder>> TradingManager::getWorkOrders(QUuid accountId, QUuid instrumentId)
	{
		QMap<QUuid, QSharedPointer<TradingOrder>> workOrders;
		foreach(QSharedPointer<TradingOrder> order, this->_workOrders.values())
		{
			if (order->getAccountId() == accountId && order->getInstrumentId() == instrumentId)
			{
				workOrders.insert(order->getId(), order);
			}
        }
		return workOrders;
    }

    QList<QSharedPointer<TradingOrder>> TradingManager::getPlacedOrders(QUuid instrumentId)
    {
        QList<QSharedPointer<TradingOrder>> workOrders;
        foreach(QSharedPointer<TradingOrder> order, this->_workOrders.values())
        {
            auto transaction = order->getTransaction();
            if (order->getInstrumentId() == instrumentId &&
            ((order->getPhase() == Phase::Placed && (transaction->getOrderType() == OrderType::Limit || transaction->getOrderType() == OrderType::OneCancelOther))
            || (order->getPhase() == Phase::Placing && transaction->getSubType() == TransactionSubType::IfDone)
             ))
            {
                workOrders.append(order);
            }
        }
        qSort(workOrders.begin(), workOrders.end(), variantLessThanWorkOrder);
        return workOrders;
    }

    QList<QSharedPointer<TradingOrder>> TradingManager::getWorkOrdersBySortDateTime()
    {
        QList<QSharedPointer<TradingOrder>> workOrders = this->_workOrders.values();
        qSort(workOrders.begin(), workOrders.end(), variantLessThanWorkOrder);
        return workOrders;
    }

    bool variantLessThanWorkOrder2(QSharedPointer<TradingOrder> v1, QSharedPointer<TradingOrder> v2)
    {
        return v1->getTransaction()->getSubmitTime() >  v2->getTransaction()->getSubmitTime();
    }

    QList<QSharedPointer<TradingOrder>> TradingManager::getWorkOrdersBySubmitTime()
    {
        QList<QSharedPointer<TradingOrder>> workOrders = this->_workOrders.values();
        qSort(workOrders.begin(), workOrders.end(), variantLessThanWorkOrder2);
        return workOrders;
    }

    QList<QSharedPointer<TradingOrder>> TradingManager::getWorkOrdersBySubmitTime(QUuid accountId, QUuid instrumentId)
    {
        QList<QSharedPointer<TradingOrder>> workOrders;
        foreach(QSharedPointer<TradingOrder> order, this->_workOrders.values())
        {
            if (order->getAccountId() == accountId && order->getInstrumentId() == instrumentId)
            {
                workOrders.append(order);
            }
        }
        qSort(workOrders.begin(), workOrders.end(), variantLessThanWorkOrder2);
        return workOrders;
    }

    QList<QSharedPointer<TradingOrder>> TradingManager::getPlacedOrdersBySubmitTime()
    {
        QList<QSharedPointer<TradingOrder>> workOrders;
        foreach(QSharedPointer<TradingOrder> order, this->_workOrders.values())
        {
            auto transaction = order->getTransaction();
            if ((((order->getPhase() == Phase::Placed || order->getPhase() == Phase::Placing) &&
                  (transaction->getOrderType() == OrderType::Limit || transaction->getOrderType() == OrderType::OneCancelOther))
                    || (order->getPhase() == Phase::Placing && transaction->getSubType() == TransactionSubType::IfDone)
             ))
            {
                workOrders.append(order);
            }
        }
        qSort(workOrders.begin(), workOrders.end(), variantLessThanWorkOrder2);
        return workOrders;
    }

    QList<QSharedPointer<TradingOrder>> TradingManager::getUnConfirmPendingOrdersBySortDateTime()
    {
        QList<QSharedPointer<TradingOrder>> unConfirmPendingOrders;
        foreach(QSharedPointer<TradingOrder> order, this->_workOrders.values())
        {
            if ((order->getPhase() == Phase::Placed ||
                 (order->getPhase() == Phase::Placing && order->getTransaction()->getSubType() == TransactionSubType::IfDone))
               && order->getBOBetTypeId().isNull())
            {
                unConfirmPendingOrders.append(order);
            }
        }
        qSort(unConfirmPendingOrders.begin(), unConfirmPendingOrders.end(), variantLessThanWorkOrder2);
        return unConfirmPendingOrders;
    }

	QList<QSharedPointer<TradingOrder>> TradingManager::getOrders(QUuid accountId, QUuid instrumentId)
	{
		QList<QSharedPointer<TradingOrder>> orders;
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (order->getAccountId() == accountId && order->getInstrumentId() == instrumentId)
			{
				orders.append(order);
			}
		}
		return orders;
	}

	QList<QSharedPointer<TradingOrder>> TradingManager::getOrdersByInstrumentId(QUuid instrumentId)
	{
		QList<QUuid> accountIds = Common::TraderBaseInfo::getInstance()->getCheckedAccounts();
		QList<QSharedPointer<TradingOrder>> orders;
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (accountIds.contains(order->getAccountId()) && order->getInstrumentId() == instrumentId)
			{
				orders.append(order);
			}
		}
		return orders;
	}

	QList<QSharedPointer<TradingOrder>> TradingManager::getOrders(QList<QUuid> accountIds)
	{
		QList<QSharedPointer<TradingOrder>> orders;
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (accountIds.contains(order->getAccountId()))
			{
				orders.append(order);
			}
		}
		return orders;
	}

	QList<QSharedPointer<TradingOrder>> TradingManager::getBOOrders(QUuid accountId, QUuid instrumentId)
	{
		QList<QSharedPointer<TradingOrder>> orders;
		foreach(QSharedPointer<TradingOrder> order, this->_boOrders.values())
		{
			if (order->getAccountId() == accountId
				&& order->getInstrumentId() == instrumentId
				&& order->getIsOpen()
				&& order->getPhase() == Phase::Executed
				&& !order->getBOBetTypeId().isNull()
				&& order->getBOFrequency() != -1)
			{
				orders.append(order);
			}
		}
		return orders;
	}

	QSharedPointer<TradingOrder> TradingManager::getBOOrder(const QUuid& id) const
	{
		return this->_boOrders.value(id);
	}

	QList<QSharedPointer<TradingOrder>> TradingManager::getOrdersByAccountId(QUuid accountId)
	{
		QList<QSharedPointer<TradingOrder>> orders;
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (accountId == order->getAccountId())
			{
				orders.append(order);
			}
		}
		return orders;
	}

	QSharedPointer<Model::Trading::Transaction> TradingManager::getOrAddTransaction(const QUuid& id)
	{
		if (this->_transactions.contains(id))
		{
			return this->_transactions.value(id);
		}
		QSharedPointer<Model::Trading::Transaction> transaction(new Model::Trading::Transaction);
		this->_transactions.insert(id, transaction);
		return transaction;
	}

	QSharedPointer<DeliveryRequest> TradingManager::getOrAddDeliveryRequest(const QUuid& id)
	{
		if (this->_deliveryRequests.contains(id))
		{
			return this->_deliveryRequests.value(id);
		}
		QSharedPointer<DeliveryRequest> deliveryRequest(new DeliveryRequest);
		this->_deliveryRequests.insert(id, deliveryRequest);
		return deliveryRequest;
	}

	QSharedPointer<ScrapDeposit> TradingManager::getOrAddScrapDeposit(const QUuid& id)
	{
		if (this->_scrapDeposits.contains(id))
		{
			return this->_scrapDeposits.value(id);
		}
		QSharedPointer<ScrapDeposit> scrapDeposit(new ScrapDeposit);
		this->_scrapDeposits.insert(id, scrapDeposit);
		return scrapDeposit;
	}

	QSharedPointer<TradingOrder> TradingManager::getOrder(const QUuid& id) const
	{
		return this->_orders.value(id);
	}

	QSharedPointer<TradingOrder> TradingManager::getWordOrder(const QUuid& id) const
	{
		return this->_workOrders.value(id);
	}

	QList<QSharedPointer<TradingOrder>> TradingManager::getOrders()
	{
		return this->_orders.values();
	}

	QList<QUuid> TradingManager::getOrderIdByBuySell(QUuid accountId, QUuid instrumentId, BuySell::BuySell buySell)
	{
		QList<QUuid> orderIds;
		bool isBuy = buySell == BuySell::Buy ? true : false;
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (order->getAccountId() == accountId && order->getInstrumentId() == instrumentId
				&& order->getIsOpen() && order->getPhase() == Phase::Executed
				&& !order->getLotBalance().isZero()
				&& order->getTransaction()->getInstrumentCategory() != InstrumentCategory::Physical
				&& order->getIsBuy() == isBuy)
			{
				orderIds.append(order->getId());
			}
		}
		return orderIds;
	}

	QSharedPointer<InstrumentSummary> TradingManager::getInstrumentSummary(const QUuid& id) const
	{
		QMutexLocker locker(&TradingManager::m_Mutex);
		return this->_instrumentSummaries.value(id);
	}

	QMap<QUuid, QSharedPointer<InstrumentSummary>> TradingManager::getInstrumentSummaries()
	{
		QMutexLocker locker(&TradingManager::m_Mutex);
		return this->_instrumentSummaries;
	}


	QMap<QString, QSharedPointer<InventoryData>> TradingManager::getInventories()
	{
		return this->_inventories;
	}

	QSharedPointer<InventoryData> TradingManager::getInventory(QUuid accountId, QUuid instrumentId) const
	{
		QString key = accountId.toString() + instrumentId.toString();
		return this->_inventories.value(key);
	}

	QSharedPointer<PendingInventoryData> TradingManager::getPendingInventory(QUuid id) const
	{
		return this->_pendingInventories.value(id);
	}

	QMap<QUuid, QSharedPointer<PendingInventoryData>> TradingManager::getPendingInventories()
	{
		return this->_pendingInventories;
	}

    QMap<QUuid,QSharedPointer<PendingInventoryData>> TradingManager::getPendingInventories(QUuid accountId, QUuid instrumentId)
    {
        QMap<QUuid,QSharedPointer<PendingInventoryData>> dataMapList;
        QMapIterator<QUuid, QSharedPointer<PendingInventoryData>> pendingInventoryItem(this->_pendingInventories);
        while (pendingInventoryItem.hasNext())
        {
            pendingInventoryItem.next();
            auto dataItem = pendingInventoryItem.value();
            if(dataItem->getAccountId() == accountId && dataItem->getInstrumentId() == instrumentId)
                dataMapList.insert(pendingInventoryItem.key(), dataItem);
        }
        return dataMapList;
    }

	QMap<QUuid, QSharedPointer<ShortSellData>> TradingManager::getShortSells()
	{
		return this->_shortSells;
	}

	QSharedPointer<ShortSellData> TradingManager::getShortSell(QUuid orderId) const
	{
		return this->_shortSells.value(orderId);
	}

	bool TradingManager::isShowShortSelling(double lot, QString key)
	{
		if (this->_inventories.contains(key))
		{
			if (this->_inventories.value(key)->getTotalLotBalance() < lot)
				return true;
			else
				return false;
		}
		else
		{
			return true;
		}
	}

	void TradingManager::deleteInventory(QString key)
	{
		this->_inventories.remove(key);
	}

	void TradingManager::refreshPhysicalData()
	{
		foreach(auto inventory, this->_inventories)
		{
			foreach(auto item, inventory->getInventoryItemDatas())
			{
				auto orderComputingResult = this->_computingManager->getOrAddOrderComputingResult(item->_tradingOrder);
				if (orderComputingResult.isNull()) continue;
				item->_orderComputingResult = orderComputingResult;
			}
			inventory->calculate();
			inventory->setButtonIsEnabled(this->_computingManager->getOverridedQuotationComputingResult(inventory->getBusinessAccount()->getId(), inventory->getBusinessInstrument()->getId()));
		}
		this->refreshPhysicalButtonIsVisible();

		foreach(auto shortSell, this->_shortSells)
		{
			shortSell->setOrderComputingResult(this->_computingManager->getOrAddOrderComputingResult(shortSell->getTradingOrder()));
			auto account = shortSell->getTradingOrder()->getTransaction()->getBusinessAccount();
			auto instrument = shortSell->getTradingOrder()->getTransaction()->getBusinessInstrument();
			if (account->getIsMultiCurrency())
			{
				QUuid sourceCurrencyId = instrument->getCurrencyId();
				QSharedPointer<Model::Setting::Currency> currency = this->_settingManager->getCurrency(sourceCurrencyId);
				shortSell->setCurrency(currency->getCode());
			}
			else
			{
				QUuid sourceCurrencyId = instrument->getCurrencyId();
				QUuid targetCurrencyId = account->getCurrencyId();
				QSharedPointer<Model::Setting::CurrencyRate> currencyRate = this->_settingManager->getCurrencyRate(sourceCurrencyId, targetCurrencyId);
				if (!currencyRate.isNull())
				{
					shortSell->setCurrency(currencyRate->getTargetCurrency()->getCode());
				}
			}
			shortSell->setButtonIsEnabled();
		}
	}

	void TradingManager::refreshPhysicalDataByInstruments(QList<QUuid> instrumentIds)
	{
		foreach(auto inventory, this->_inventories)
		{
			if (instrumentIds.contains(inventory->getBusinessInstrument()->getId()))
			{
				foreach(auto item, inventory->getInventoryItemDatas())
				{
					if (item->_tradingOrder->getPhase() != Phase::Deleted)
					{
						auto orderComputingResult = this->_computingManager->getOrAddOrderComputingResult(item->_tradingOrder);
						if (orderComputingResult.isNull()) continue;
						item->_orderComputingResult = orderComputingResult;
					}
				}
				inventory->calculate();
				inventory->setButtonIsEnabled(this->_computingManager->getOverridedQuotationComputingResult(inventory->getBusinessAccount()->getId(), inventory->getBusinessInstrument()->getId()));
			}
		}
		this->refreshPhysicalButtonIsVisible();

		foreach(auto shortSell, this->_shortSells)
		{
			if (instrumentIds.contains(shortSell->getTradingOrder()->getTransaction()->getBusinessInstrument()->getId())
				&& shortSell->getTradingOrder()->getPhase() != Phase::Deleted)
			{
				shortSell->setOrderComputingResult(this->_computingManager->getOrAddOrderComputingResult(shortSell->getTradingOrder()));
				auto account = shortSell->getTradingOrder()->getTransaction()->getBusinessAccount();
				auto instrument = shortSell->getTradingOrder()->getTransaction()->getBusinessInstrument();
				if (account->getIsMultiCurrency())
				{
					QUuid sourceCurrencyId = instrument->getCurrencyId();
					QSharedPointer<Model::Setting::Currency> currency = this->_settingManager->getCurrency(sourceCurrencyId);
					shortSell->setCurrency(currency->getCode());
				}
				else
				{
					QUuid sourceCurrencyId = instrument->getCurrencyId();
					QUuid targetCurrencyId = account->getCurrencyId();
					QSharedPointer<Model::Setting::CurrencyRate> currencyRate = this->_settingManager->getCurrencyRate(sourceCurrencyId, targetCurrencyId);
					if (!currencyRate.isNull())
					{
						shortSell->setCurrency(currencyRate->getTargetCurrency()->getCode());
					}
				}
				shortSell->setButtonIsEnabled();
			}
		}
	}

	void TradingManager::refreshInventory()
	{
		this->_inventories.clear();
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			//&& order->getPhysicalTradeSide() != PhysicalTradeSide::None
			/*auto code = order->getCode();
			auto getInstrumentCategory = order->getTransaction()->getInstrumentCategory();
			auto getPhysicalTradeSide = order->getPhysicalTradeSide();
			auto getLotBalance = order->getLotBalance().toDouble();
			auto getDeliveryLockLot = order->getDeliveryLockLot().toDouble();
			auto getOrderType = order->getTransaction()->getOrderType();
			auto getPhase = order->getPhase();
			auto getIsOpen = order->getIsOpen();*/
			if (order->getTransaction()->getInstrumentCategory() == InstrumentCategory::Physical
				&& order->getPhysicalTradeSide() != PhysicalTradeSide::ShortSell
				&& order->getLotBalance() > 0.0
				&& order->getTransaction()->getOrderType() != OrderType::BinaryOption
				&& order->getPhase() == Common::Enum::Phase::Executed
				&& order->getIsOpen())
			{
				QString key = order->getAccountId().toString() + order->getInstrumentId().toString();
				auto orderResult = this->_computingManager->getOrAddOrderComputingResult(order);
				if (this->_inventories.contains(key))
				{
					this->_inventories[key]->addOrder(order, orderResult);
				}
				else
				{
					QSharedPointer<InventoryData> inventoryData(new InventoryData(
						this->_settingManager,
						this->_computingManager,
						order->getTransaction()->getBusinessAccount(),
						order->getTransaction()->getBusinessInstrument()));
					inventoryData->addOrder(order, orderResult);
					this->_inventories.insert(key, inventoryData);
				}
			}

		}

		foreach(auto inventory, this->_inventories)
		{
			inventory->calculate();
			inventory->setButtonIsEnabled(this->_computingManager->getOverridedQuotationComputingResult(inventory->getBusinessAccount()->getId(), inventory->getBusinessInstrument()->getId()));
		}
		this->refreshPhysicalButtonIsVisible();
	}

	void TradingManager::refreshPendingInventory()
	{
		foreach(auto deliveryRequest, this->_deliveryRequests)
		{
			this->refreshPendingInventoryFromDeliveryRequest(deliveryRequest);

		}

		foreach(auto scrapDeposit, this->_scrapDeposits)
		{
			this->refreshPendingInventoryFromScrapDeposit(scrapDeposit);
		}
	}

	void TradingManager::refreshPhysicalButtonIsVisible()
	{
		bool isHideInstalment = true;
		bool isHideRepayment = true;
		foreach(auto inventory, this->_inventories)
		{
			if (inventory->existInstalmentOrder())
				isHideInstalment = false;
			if (inventory->existRepaymentOrder())
				isHideRepayment = false;
		}
		Common::SignalForwarder::getInstance()->sendIsHideInstalmentOrRepaymentButtonSignal(isHideInstalment, true);
		Common::SignalForwarder::getInstance()->sendIsHideInstalmentOrRepaymentButtonSignal(isHideRepayment, false);
		Common::TraderBaseInfo::getInstance()->setIsHideInstalment(isHideInstalment);
		Common::TraderBaseInfo::getInstance()->setIsHideRepayment(isHideRepayment);
	}

	void TradingManager::refreshPhysicalButtonIsEnable()
	{
		this->refreshPhysicalButtonIsVisible();
		foreach(auto inventory, this->_inventories)
        {
			inventory->setButtonIsEnabled(this->_computingManager->getOverridedQuotationComputingResult(inventory->getBusinessAccount()->getId(), inventory->getBusinessInstrument()->getId()));
		}
	}

    void TradingManager::updateBussnessAccountInstrumentByUpdateCommand()
	{
		foreach(auto inventory, this->_inventories)
		{
			inventory->updateAccountInstrumentByUpdateCommand();
		}

		foreach(auto transaction, this->_transactions)
		{
			QSharedPointer<Business::Model::Setting::BusinessAccount> settingAccount = this->_settingManager->getAccount(transaction->getAccountId());
			QSharedPointer<Business::Model::Setting::BusinessInstrument> settingInstrument = settingAccount->getInstrument(transaction->getInstrumentId());
			if (!settingAccount.isNull())
				transaction->setBusinessAccount(settingAccount);
			if (!settingInstrument.isNull())
				transaction->setBusinessInstrument(settingInstrument);
		}

		QMapIterator<QUuid, QSharedPointer<Fund>> fundItem(this->_funds);
		while (fundItem.hasNext()) {
			fundItem.next();
			QUuid accountId = fundItem.key();
			QSharedPointer<Business::Model::Setting::BusinessAccount> settingAccount = this->_settingManager->getAccount(accountId);
			if (!settingAccount.isNull())
			{
				this->_funds[accountId]->setCreditAmount(settingAccount->getCreditAmount());
				if (this->_subFunds.contains(accountId))
				{
					QUuid accountCurrencyId = settingAccount->getCurrencyId();
					auto accountCurrencies = this->_subFunds[accountId];
					if (accountCurrencies.contains(accountCurrencyId))
					{
						accountCurrencies[accountCurrencyId]->setCreditAmount(settingAccount->getCreditAmount());
					}
				}
			}
		}

	}

	void TradingManager::deletePendingInventoryData(QUuid id)
	{
		if (this->_pendingInventories.contains(id))
		{
			this->_pendingInventories.remove(id);
			if (this->_deliveryRequests.contains(id))
				this->_deliveryRequests.remove(id);
			if (this->_scrapDeposits.contains(id))
				this->_scrapDeposits.remove(id);
		}
	}

	void TradingManager::refreshShortSell()
	{
		this->_shortSells.clear();
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (order->getTransaction()->getInstrumentCategory() == InstrumentCategory::Physical
				&& order->getPhysicalTradeSide() == PhysicalTradeSide::ShortSell
				&& !order->getLotBalance().isZero())
			{
                QSharedPointer<ShortSellData> shortSellData(new ShortSellData);
				shortSellData->setTradingOrder(order);
				shortSellData->setOrderComputingResult(this->_computingManager->getOrAddOrderComputingResult(order));
				auto account = order->getTransaction()->getBusinessAccount();
				auto instrument = order->getTransaction()->getBusinessInstrument();
				if (account->getIsMultiCurrency())
				{
					QUuid sourceCurrencyId = instrument->getCurrencyId();
					QSharedPointer<Model::Setting::Currency> currency = this->_settingManager->getCurrency(sourceCurrencyId);
					shortSellData->setCurrency(currency->getCode());
				}
				else
				{
					QUuid sourceCurrencyId = instrument->getCurrencyId();
					QUuid targetCurrencyId = account->getCurrencyId();
					QSharedPointer<Model::Setting::CurrencyRate> currencyRate = this->_settingManager->getCurrencyRate(sourceCurrencyId, targetCurrencyId);
					if (!currencyRate.isNull())
					{
						shortSellData->setCurrency(currencyRate->getTargetCurrency()->getCode());
					}
				}
				this->_shortSells.insert(order->getId(), shortSellData);
			}
		}
	}

	bool TradingManager::hasExistInventoryItemData(QString key, QUuid id)
	{
		if (this->_inventories.contains(key))
			return this->_inventories[key]->existInventoryItemData(id);
		return false;
	}

	QSharedPointer<InventoryData> TradingManager::addInventory(QUuid orderId)
	{
		auto order = this->_orders.value(orderId);
		QString key = order->getAccountId().toString() + order->getInstrumentId().toString();
		auto orderResult = this->_computingManager->getOrAddOrderComputingResult(order);
		if (this->_inventories.contains(key))
		{
			this->_inventories[key]->addOrder(order, orderResult);

		}
		else
		{
			QSharedPointer<InventoryData> inventoryData(new InventoryData(
				this->_settingManager,
				this->_computingManager,
				order->getTransaction()->getBusinessAccount(),
				order->getTransaction()->getBusinessInstrument()));
			inventoryData->addOrder(order, orderResult);
			this->_inventories.insert(key, inventoryData);
		}
		QSharedPointer<InventoryData> inventory = this->_inventories[key];
		inventory->calculate();
		inventory->setButtonIsEnabled(this->_computingManager->getOverridedQuotationComputingResult(inventory->getBusinessAccount()->getId(),
			inventory->getBusinessInstrument()->getId()));
		this->refreshPhysicalButtonIsVisible();
		return inventory;
	}

	QSharedPointer<ShortSellData> TradingManager::addShortSell(QUuid orderId)
	{
		if (this->_shortSells.contains(orderId))
		{
			return this->_shortSells.value(orderId);
		}
		else
		{
			QSharedPointer<ShortSellData> shortSellData(new ShortSellData);
			auto order = this->_orders.value(orderId);
			shortSellData->setTradingOrder(order);
			shortSellData->setOrderComputingResult(this->_computingManager->getOrAddOrderComputingResult(order));
			auto account = order->getTransaction()->getBusinessAccount();
			auto instrument = order->getTransaction()->getBusinessInstrument();
			if (account->getIsMultiCurrency())
			{
				QUuid sourceCurrencyId = instrument->getCurrencyId();
				QSharedPointer<Model::Setting::Currency> currency = this->_settingManager->getCurrency(sourceCurrencyId);
				shortSellData->setCurrency(currency->getCode());
			}
			else
			{
				QUuid sourceCurrencyId = instrument->getCurrencyId();
				QUuid targetCurrencyId = account->getCurrencyId();
				QSharedPointer<Model::Setting::CurrencyRate> currencyRate = this->_settingManager->getCurrencyRate(sourceCurrencyId, targetCurrencyId);
				if (!currencyRate.isNull())
				{
					shortSellData->setCurrency(currencyRate->getTargetCurrency()->getCode());
				}
			}
			this->_shortSells.insert(orderId, shortSellData);
			shortSellData->setButtonIsEnabled();
			return shortSellData;
		}
	}

	void TradingManager::addTransaction(QSharedPointer<Common::Struct::Transaction> placingTransaction)
	{
		QSharedPointer<Model::Trading::Transaction> transaction(new Model::Trading::Transaction);
		transaction->updatePlacingTransaction(placingTransaction);
		this->_transactions.insert(placingTransaction->_id, transaction);
	}

	void TradingManager::addOrder(QSharedPointer<Common::Struct::Order> placingOrder, QSharedPointer<Common::Struct::Transaction> placingTransaction)
	{
		if (!this->_transactions.contains(placingTransaction->_id))
		{
			QSharedPointer<Model::Trading::Transaction> transaction(new Model::Trading::Transaction);
			transaction->updatePlacingTransaction(placingTransaction);
			transaction->setSubmitTime(Common::UtcTimeManager::getInstance()->getUtcNow());
			QSharedPointer<Business::Model::Setting::BusinessInstrument> settingInstrument = this->_settingManager->getInstrument(placingTransaction->_accountId, placingTransaction->_instrumentId);
			QSharedPointer<Business::Model::Setting::BusinessAccount> settingAccount = this->_settingManager->getAccount(placingTransaction->_accountId);
			if (!settingAccount.isNull())
				transaction->setBusinessAccount(settingAccount);
			if (!settingInstrument.isNull())
				transaction->setBusinessInstrument(settingInstrument);
			this->_transactions.insert(placingTransaction->_id, transaction);
		}
		QSharedPointer<TradingOrder> order(new TradingOrder);
		order->updatePlacingOrder(placingOrder, placingTransaction);
		order->setTransaction(this->_transactions.value(placingTransaction->_id));
		this->_orders.insert(placingOrder->_id, order);
		if (order->isBOOrder())
		{
			if (!this->_boOrders.contains(placingOrder->_id))
				this->_boOrders.insert(placingOrder->_id, order);
		}
		else
			this->_workOrders.insert(placingOrder->_id, order);
		if (this->_pendingTransactionIds.contains(placingTransaction->_id))
			this->_pendingTransactionIds.append(placingTransaction->_id);
		Common::SignalForwarder::getInstance()->sendAddDataSignal(placingOrder->_id, UpdateType::WorkOrder);
	}

	QList<QUuid> TradingManager::clearAllForWork()
	{
		QList<QUuid> clearIds;
		foreach(QSharedPointer<TradingOrder> order, this->_workOrders.values())
		{
			if (order->getPhase() == Phase::Executed
				|| order->getPhase() == Phase::Completed
				|| order->getPhase() == Phase::Canceled
				|| order->getPhase() == Phase::Deleted)
			{
				clearIds.append(order->getId());
				this->_workOrders.remove(order->getId());
			}

		}
		return clearIds;
	}

	bool TradingManager::clearOrderForWork(QUuid id)
	{
		auto order = this->_workOrders.value(id);
		if (!order.isNull()
			&& (order->getPhase() == Phase::Executed
				|| order->getPhase() == Phase::Completed
				|| order->getPhase() == Phase::Canceled
				|| order->getPhase() == Phase::Deleted))
		{
			this->_workOrders.remove(id);
			return true;
		}
		else
			return false;
	}

	QSharedPointer<TradingOrder> TradingManager::updateOrderInfo(QUuid id, Phase::Phase phase, QString cancelReason)
	{
		auto order = this->_orders.value(id);
		if (!order.isNull())
		{
			order->setPhase(phase);
			auto transaction = order->getTransaction();
			transaction->setPhase(phase);
			order->setTransactionError(cancelReason);
		}
		return order;
	}

	bool TradingManager::hasOpentContract(QUuid accountId, QUuid currencyId)
	{
		bool hasOpentContract = false;
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (accountId == order->getAccountId()
				&& order->getIsOpen() && order->getPhase() == Phase::Executed
				&& !order->getLotBalance().isZero()
				&& order->getTransaction()->getInstrumentCategory() != InstrumentCategory::Physical)
			{
				if (this->_settingManager->getInstrument(accountId, order->getInstrumentId())->getCurrencyId() == currencyId)
				{
					hasOpentContract = true;
					break;
				}
			}
		}
		return hasOpentContract;
	}

	bool TradingManager::canBeDeselected(QUuid instrumentId)
	{
		foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
		{
			if (order->getInstrumentId() == instrumentId
                && ((order->getIsOpen() && order->getPhase() == Phase::Executed && (!order->getLotBalance().isZero() || !order->getDeliveryLockLot().isZero()))
                || (order->getPhase() != Phase::Canceled && order->getPhase() != Phase::Deleted && order->getPhase() != Phase::Executed && order->getPhase() != Phase::Completed)))
			{
				return false;
			}
		}
		return true;
	}

	void TradingManager::deleteTradingOrder(QUuid id)
	{
		if (this->_orders.contains(id))
		{
			auto order = this->_orders.value(id);
			if (order->getPhase() == Phase::Completed || order->getPhase() == Phase::Canceled)
			{
				this->_computingManager->deleteComputingModel(order);
				if (!order->getIsOpen())
				{
					foreach(auto orderRelation, order->getOrderRelations())
					{
						QUuid openOrderId = orderRelation->getOrderId1();
						if (this->_orders.contains(openOrderId))
						{
							this->_orders[openOrderId]->removeCloseOrder(id);
							this->_orders[openOrderId]->caculateLimitSummary();
						}
					}
				}
				else
				{
					this->_removeOpenOrders.insert(id, order);
				}
				this->_orders.remove(id);
			}
			else
			{
				if (order->getIsOpen()) this->_removeOpenOrders.insert(id, order);
			}
		}
	}

	bool TradingManager::canBeRemoved(QUuid instrumentId)
	{
		foreach(auto order, this->getOrdersByInstrumentId(instrumentId))
		{
			if (order->getLotBalance() > 0.0)
			{
				return false;
			}
			if (order->getPhase() != Phase::Canceled
				&& order->getPhase() != Phase::Deleted
				&& order->getPhase() != Phase::Executed)
			{
				return false;
			}
		}
		return true;
	}

	bool TradingManager::canPlaceOrder(QUuid quotePolicyId, QUuid instrumentId)
	{
		bool canPlacingOrder = false;
		QList<QUuid> instrumentIds;
		instrumentIds.append(instrumentId);
		auto quotePolicies = this->_settingManager->getQuotePolicies(instrumentIds);
		QList<QUuid> accountIds = Common::TraderBaseInfo::getInstance()->getCheckedAccounts();
		for (int i = 0; i < accountIds.count(); i++)
		{
			auto settingInstrument = this->_settingManager->getInstrument(accountIds[i], instrumentId);
			if (!settingInstrument.isNull() && !settingInstrument->isExistBOPolicySetting())
			{
				auto spotTradePlaceSettingDetail = settingInstrument->getPlaceSetting()->getPlaceSettingDetail(OrderType::SpotTrade);
				auto limitStopPlaceSettingDetail = settingInstrument->getPlaceSetting()->getPlaceSettingDetail(OrderType::Limit);
				if (spotTradePlaceSettingDetail->getAllowTrade()
					|| limitStopPlaceSettingDetail->getAllowTrade())
				{
					auto quotation = this->_computingManager->getOverridedQuotationComputingResultByQuotePolicyId(quotePolicyId.isNull() ? quotePolicies.first()->_id : quotePolicyId, instrumentId);
					if (!quotation->ask.isNullPrice() && !quotation->bid.isNullPrice())
					{
						canPlacingOrder = true;
						break;
					}
				}
			}
		}
		return canPlacingOrder;
	}

	bool TradingManager::canBackPlaceOrder(QUuid quotePolicyId, QUuid instrumentId)
	{
		bool canPlacingOrder = false;
		QList<QUuid> instrumentIds;
		instrumentIds.append(instrumentId);
		auto quotePolicies = this->_settingManager->getQuotePolicies(instrumentIds);
		QList<QUuid> accountIds = Common::TraderBaseInfo::getInstance()->getCheckedAccounts();
		for (int i = 0; i < accountIds.count(); i++)
		{
			auto settingInstrument = this->_settingManager->getInstrument(accountIds[i], instrumentId);
			if (!settingInstrument.isNull() && !settingInstrument->isExistBOPolicySetting())
			{
				auto spotTradePlaceSettingDetail = settingInstrument->getPlaceSetting()->getPlaceSettingDetail(OrderType::SpotTrade);
				if (spotTradePlaceSettingDetail->getAllowTrade())
				{
					auto quotation = this->_computingManager->getOverridedQuotationComputingResultByQuotePolicyId(quotePolicyId.isNull() ? quotePolicies.first()->_id : quotePolicyId, instrumentId);
					if (!quotation->ask.isNullPrice() && !quotation->bid.isNullPrice())
					{
						canPlacingOrder = true;
						break;
					}
				}
			}
		}
		return canPlacingOrder;
	}

	bool TradingManager::isTrading()
	{
		bool isTrading = false;
		QList<QUuid> accountIds = Common::TraderBaseInfo::getInstance()->getCheckedAccounts();
		foreach(QUuid accountId, accountIds)
		{
			auto settingAccount = this->_settingManager->getAccount(accountId);
			if (settingAccount->getAllowedTrade())
				isTrading = true;
		}
		return isTrading;
	}

	bool TradingManager::hasSufficientUsableMargin(QUuid accountId, QUuid currencyId, QString amount, QSharedPointer<Business::Model::Setting::CurrencyRate> currencyRate)
	{
		QDecNumber amount2 = Common::roundToDecimal(amount.toDouble(), Common::getValidDecimals(amount));
		auto sttingsAccount = this->_settingManager->getAccount(accountId);
		if (sttingsAccount.isNull()) return true;
		if (sttingsAccount->getIsMultiCurrency())
		{
			bool isOK = false;
			auto subfounds = this->_subFunds[accountId];
			foreach(auto item, subfounds)
			{
				if (item->getCurrencyId() == currencyId)
				{
					isOK = item->canPlacePayment(amount2, sttingsAccount->getBalanceDeficitAllowPay());
				}
			}
			if (isOK)
			{
				isOK = this->_funds.value(accountId)->hasSufficientUsableMargin(sttingsAccount->getBalanceDeficitAllowPay(), currencyRate, amount2);
			}
			return isOK;
		}
		else
		{
			if (this->_funds.contains(accountId))
			{
				QSharedPointer<Business::Model::Setting::CurrencyRate> currencyRate(Q_NULLPTR);
				return this->_funds[accountId]->hasSufficientUsableMargin(sttingsAccount->getBalanceDeficitAllowPay(), currencyRate, amount2);
			}
		}
		return false;
	}

	void TradingManager::updateBOCountdown()
	{
		if (this->_boOrders.count() == 0) return;
		QList<QUuid> needRemoveBoOrderList;
		QDateTime now = Common::UtcTimeManager::getInstance()->getUtcNow();
		QMapIterator<QUuid, QSharedPointer<TradingOrder>> boOrderItem(this->_boOrders);
		while (boOrderItem.hasNext())
		{
			boOrderItem.next();
			auto order = boOrderItem.value();
			if (order->getPhase() == Common::Enum::Phase::Placing
				|| order->getPhase() == Common::Enum::Phase::Placed
				|| order->getPhase() == Common::Enum::Phase::Canceled
				|| order->getPhase() == Common::Enum::Phase::Deleted
				|| order->getHitCount() == 1)
			{
				order->setBOCountdown(0);
				Common::SignalForwarder::getInstance()->sendBOInfoToBOChartSignal(order->getId(),
					0,
					"",
					order->getBOBeginTime(),
					order->getBOEndTime(),
					"",
					true);
				if (order->getPhase() == Common::Enum::Phase::Canceled)
					Common::SignalForwarder::getInstance()->sendHideBORealtimeChartSignal(order->getId());
				if (!(order->getPhase() == Common::Enum::Phase::Placing || order->getPhase() == Common::Enum::Phase::Placed))
					needRemoveBoOrderList.append(boOrderItem.key());
			}
			else
			{
				if (!order->updateBOCountdown(now))
				{
					needRemoveBoOrderList.append(boOrderItem.key());
					Common::SignalForwarder::getInstance()->sendBOInfoToBOChartSignal(order->getId(),
						0,
						"",
						order->getBOBeginTime(),
						order->getBOEndTime(),
						"",
						true);
				}
				else
				{
					QSharedPointer<OverridedQuotationComputingResult> overridedQuotationComputingResult = this->_computingManager->getOverridedQuotationComputingResult(order->getAccountId(), order->getInstrumentId());
					order->setMarketBOPrice(order->getTransaction()->getBusinessInstrument()->getIsNormal() ? overridedQuotationComputingResult->bid : overridedQuotationComputingResult->ask);
					Common::SignalForwarder::getInstance()->sendBOInfoToBOChartSignal(order->getId(),
						order->getBOCountdown(),
						order->getBOFloatPLInString(),
						order->getBOBeginTime(),
						DateTimeConstants::MinQDateTime,
						order->getExecutePrice().toQString(),
						order->getBOOption() == 1 ? true : false);
				}
			}
		}

		if (needRemoveBoOrderList.count() > 0)
		{
			foreach(QUuid id, needRemoveBoOrderList)
			{
				this->_boOrders.remove(id);
			}
		}
	}

	void TradingManager::cancelExpired(QList<QUuid> tranIds)
	{
		foreach(auto order, this->_orders)
		{
			if (tranIds.contains(order->getTransactionId()))
			{
				this->_orders[order->getId()]->setPhase(Phase::Canceled);
				this->_orders[order->getId()]->getTransaction()->setPhase(Phase::Canceled);
                //QString error = QString("TransactionExpired");
                this->_orders[order->getId()]->setTransactionError("TransactionExpired");
			}
		}
	}

	QList<QUuid> TradingManager::getTransactionsMayExpried()
	{
		QList<QUuid> transactionsMayExpried;
		QDateTime now = Common::UtcTimeManager::getInstance()->getUtcNow();
		foreach(QUuid transactionId, this->_pendingTransactionIds)
		{
			if (this->_transactions.contains(transactionId))
			{
				auto transaction = this->_transactions[transactionId];
				if ((transaction->getPhase() == Phase::Placing || transaction->getPhase() == Phase::Placed)
					&& transaction->getEndTime() <= now)
				{
					transactionsMayExpried.append(transactionId);
				}
			}
		}
		foreach(QUuid id, transactionsMayExpried)
		{
			this->_pendingTransactionIds.removeOne(id);
		}
		return transactionsMayExpried;
	}

	void TradingManager::computeAllMarketingGroupFund()
	{
		if (this->_settingManager->_marketingGroupFunds.count() > 0)
		{
            QMapIterator<QString, QSharedPointer<Trading::MarketingGroupFund>> marketingGroupFundItem(this->_settingManager->_marketingGroupFunds);
			while (marketingGroupFundItem.hasNext()) {
				marketingGroupFundItem.next();
				this->computeMarketingGroupFund(marketingGroupFundItem.key());
			}
		}
	}

    void TradingManager::computeMarketingGroupFund(QString saleIdAndAccountCurrencyId)
	{
        if (this->_settingManager->_marketingGroupFunds.contains(saleIdAndAccountCurrencyId))
		{
            auto marketingGroupFund = this->_settingManager->_marketingGroupFunds.value(saleIdAndAccountCurrencyId);
			marketingGroupFund->clear();

            if (marketingGroupFund->getCurrencyName() == "")
			{
				if (marketingGroupFund->getCurrencyId().isNull())
				{
					QUuid accountId = marketingGroupFund->getAccountIds().first();
					auto subFunds = this->_subFunds[accountId];
					if (subFunds.count() == 0)
					{
						auto fund = this->_funds[accountId];
                        marketingGroupFund->setCurrency(fund->getCurrencyId(), fund->getCurrencyName(), fund->getDecimals());
					}
					else
					{
						auto subFund = this->_subFunds[accountId].first();
                        marketingGroupFund->setCurrency(subFund->getCurrencyId(), subFund->getCurrencyName(), subFund->getDecimals());
					}
				}
				else
				{
					auto currency = this->_settingManager->getCurrency(marketingGroupFund->getCurrencyId());
                    marketingGroupFund->setCurrency(currency->getId(), currency->getName(), currency->getDecimals());
				}
			}
            //QUuid currentId = marketingGroupFund->getCurrencyId();
			foreach(QUuid accountId, marketingGroupFund->getAccountIds())
			{
                auto fund = this->_funds[accountId];
                marketingGroupFund->addFund(fund);

//				auto subFunds = this->_subFunds[accountId];
//				if (subFunds.count() == 0)
//				{
//					auto fund = this->_funds[accountId];
//					marketingGroupFund->addFund(fund);
//				}
//				else
//				{
//					QMapIterator<QUuid, QSharedPointer<SubFund>> subFundItem(subFunds);
//					while (subFundItem.hasNext()) {
//						subFundItem.next();
//						auto subFund = subFunds[subFundItem.key()];
//						if (currentId == subFundItem.key())
//						{
//							marketingGroupFund->addSubFund(subFund);
//						}
//						marketingGroupFund->addCurrency(subFund->getCurrencyId(), subFund->getCurrencyCode());
//					}
//				}
			}
		}

	}

	void TradingManager::changeMarketingGroupFund(QUuid saleId, QUuid currencyId)
	{
//		if (this->_settingManager->_marketingGroupFunds.contains(saleId))
//		{
//			auto marketingGroupFund = this->_settingManager->_marketingGroupFunds[saleId];
//			auto currency = this->_settingManager->getCurrency(currencyId);
//			marketingGroupFund->setCurrency(currencyId, currency->getCode(), currency->getDecimals());
//			this->computeMarketingGroupFund(saleId);
//		}
	}

    QStringList TradingManager::getOrderConfirmationParameter(const QList<QUuid>&  executedOrderIds)
    {
        QStringList orderIds;
        foreach(QUuid orderId, executedOrderIds)
        {
			if (!this->_orders.contains(orderId))
				continue;
            auto order =  this->_orders[orderId];
			if (order->getTransaction().isNull() || order->getTransaction()->getBusinessAccount().isNull())
				continue;
			if(order->getTransaction()->getBusinessAccount()->getPrintOrderConfirmation())
                orderIds.append(orderId.toString().mid(1, 36));
        }
        return orderIds;
    }

    QString TradingManager::getAdministrativeCharge(QUuid accountId, QUuid instrumentId, bool isOpen, QDecNumber lot, QDecNumber price)
    {
        auto businessInstrument = this->_settingManager->getInstrument(accountId, instrumentId);
        if(businessInstrument.isNull() || businessInstrument->getAdministrativeChargeId().isNull()) return "";
        auto administrativeChargeDetails = businessInstrument->getAdministrativeChargeDetails();
        if(administrativeChargeDetails.count() == 0) return "";
        QList<QUuid> commonAdministrativeChargeIdForInstrumentIds;
        auto settingAccount = this->_settingManager->getAccount(accountId);
        auto instruments = settingAccount->getInstruments();
        QUuid administrativeChargeId = businessInstrument->getAdministrativeChargeId();
        foreach(auto instrument, instruments)
        {
            if(instrument->getAdministrativeChargeId() == administrativeChargeId)
            {
                commonAdministrativeChargeIdForInstrumentIds.append(instrument->getId());
            }
        }
        QDecNumber totalValue = 0.0;
        foreach(auto needcalInstrumentId, commonAdministrativeChargeIdForInstrumentIds)
        {
            auto orders = this->getOpenOrders(accountId, needcalInstrumentId);
            foreach(auto order, orders)
            {
                totalValue +=  order->getLotBalance() * order->getTransaction()->getBusinessInstrument()->getAdministrativeChargeConvertRate();
                qDebug() << " TradingManager::getAdministrativeCharge totalValue: "<< totalValue.toDouble();
                QLOG_INFO(Common::Constants::LogKeyConstants::Business)
                        << " TradingManager::getAdministrativeCharge "
                        << " orderId: " << order->getId()
                        << " LotBalance: " << order->getLotBalance().toDouble()
                        << " AdministrativeChargeConvertRate: " << order->getTransaction()->getBusinessInstrument()->getAdministrativeChargeConvertRate().toDouble()
                        << " totalValue: "<< totalValue.toDouble();
            }
        }
        if(isOpen)
            totalValue += lot * businessInstrument->getAdministrativeChargeConvertRate();

        QLOG_INFO(Common::Constants::LogKeyConstants::Business)
                 << " TradingManager::getAdministrativeCharge current place order AdministrativeChargeConvertRate: "
                 << businessInstrument->getAdministrativeChargeConvertRate().toDouble()
                 << " totalValue: " << totalValue.toDouble();

        qDebug() << " TradingManager::getAdministrativeCharge current place order AdministrativeChargeConvertRate: "
                 << businessInstrument->getAdministrativeChargeConvertRate().toDouble()
                 << " totalValue: " << totalValue.toDouble();

        QDecNumber chargeRate = 0.0;
        if(!totalValue.isZero())
        {
            QMapIterator<QDecNumber, QDecNumber> item(administrativeChargeDetails);
            while (item.hasNext()) {
              item.next();
              QLOG_INFO(Common::Constants::LogKeyConstants::Business) << item.key().toDouble() << ": " << item.value().toDouble();
              qDebug() << " TradingManager::getAdministrativeCharge: "<< item.key().toDouble() << ": " << item.value().toDouble();
              if(item.key() >= totalValue)
              {
                  chargeRate = item.value();
                  break;
              }
            }
            if(chargeRate == 0.0)
                chargeRate = administrativeChargeDetails.last();
        }
        QString administrativeChargeStr = "";
        QDecNumber administrativeCharge = lot * price * chargeRate * businessInstrument->getPlaceSetting()->getContractSize();
        QLOG_INFO(Common::Constants::LogKeyConstants::Business)
                 << " TradingManager::getAdministrativeCharge current place order administrativeCharge:"<< administrativeCharge.toDouble()
                 << " lot: " << lot.toDouble()
                 << " price: " << price.toDouble()
                 << " chargeRate: " << chargeRate.toDouble()
                 << " ContractSize: " << businessInstrument->getPlaceSetting()->getContractSize().toDouble();
        qDebug() << " TradingManager::getAdministrativeCharge current place order administrativeCharge:"<< administrativeCharge.toDouble()
                 << " lot: " << lot.toDouble()
                 << " price: " << price.toDouble()
                 << " chargeRate: " << chargeRate.toDouble();
        bool isChangRate = false;
        if(!settingAccount->getIsMultiCurrency())
        {
            QUuid sourceCurrencyId = businessInstrument->getCurrencyId();
            QUuid targetCurrencyId = settingAccount->getCurrencyId();
            QSharedPointer<Model::Setting::CurrencyRate> currencyRate = this->_settingManager->getCurrencyRate(sourceCurrencyId, targetCurrencyId);
            if(!currencyRate.isNull())
            {
                int decimals = currencyRate->getTargetCurrency()->getDecimals();
                administrativeCharge = currencyRate->exchangeByRateIn(administrativeCharge);
                QLOG_INFO(Common::Constants::LogKeyConstants::Business)
                        << " exchangeByRateIn_ administrativeCharge:" << administrativeCharge.toDouble();
                qDebug() << " exchangeByRateIn_ administrativeCharge:" << administrativeCharge.toDouble();
                administrativeChargeStr = Common::roundToString(administrativeCharge.toDouble(), decimals, true);
            }
            else
            {
                QSharedPointer<Model::Setting::Currency> currency = this->_settingManager->getCurrency(targetCurrencyId);
                if(!currency.isNull())
                {
                    int decimals = currency->getDecimals();
                    administrativeChargeStr = Common::roundToString(administrativeCharge.toDouble(), decimals, true);
                }
                else
                    administrativeChargeStr = Common::roundToString(administrativeCharge.toDouble(), 2, true);
            }
            isChangRate = true;
        }
        if(!isChangRate)
        {
            QUuid sourceCurrencyId = businessInstrument->getCurrencyId();
            QSharedPointer<Model::Setting::Currency> currency = this->_settingManager->getCurrency(sourceCurrencyId);
            if(!currency.isNull())
            {
                int decimals = currency->getDecimals();
                administrativeChargeStr = Common::roundToString(administrativeCharge.toDouble(), decimals, true);
            }
            else
                administrativeChargeStr = Common::roundToString(administrativeCharge.toDouble(), 2, true);
        }
        QLOG_INFO(Common::Constants::LogKeyConstants::Business)
                << " administrativeChargeStr:" << administrativeChargeStr;
         qDebug() << " administrativeChargeStr:" << administrativeChargeStr;
        return administrativeChargeStr;
    }

    bool TradingManager::canPlaceCloseOrder(QUuid accountId, QUuid instrumentId, bool isPending, bool isBuy)
    {
        bool canPlace = false;
        foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
        {
            if (order->getAccountId() == accountId
                && order->getInstrumentId() == instrumentId
                && order->getIsOpen()
                && order->getPhase() == Phase::Executed
                && !order->getLotBalance().isZero()
                && order->getBOBetTypeId().isNull())
            {
                if(isPending)
                {
                    if(order->getIsBuy() == isBuy)
                    {
                        QDecNumber limitOrderLot = order->getLotBalanceForLimitOrderType();
                        QDecNumber stopOrderLot = order->getLotBalanceForStopOrderType();
                        if(limitOrderLot.toDouble() > 0 || stopOrderLot.toDouble() > 0)
                        {
                            canPlace = true;
                            break;
                        }
                    }
                }
                else
                {
                    canPlace = true;
                     break;
                }
            }
        }
        return canPlace;
    }


	//---------------------private method------------------------//
	void TradingManager::setOrderMidifyRelation()
	{
		foreach(auto order, this->_orders)
		{
			if (order.isNull()) return;
			if (order->getTransaction()->getSubType() == TransactionSubType::Amend)
			{
				auto replaceOrder = this->getOrder(order->getTransaction()->getAssigningOrderId());
				if (!replaceOrder.isNull())  // Cannot get canceled order from initial data, so use [Orders.FirstOrDefault].
				{
					order->setReplaceOrder(replaceOrder);
					replaceOrder->setReplacedByOrder(order);
				}
			}
		}
	}

    void TradingManager::refreshPendingInventoryFromDeliveryRequest(QSharedPointer<DeliveryRequest> deliveryRequest)
    {
        if (deliveryRequest->getStatus() == DeliveryRequestStatus::Accepted
            || deliveryRequest->getStatus() == DeliveryRequestStatus::Stocked
            || deliveryRequest->getStatus() == DeliveryRequestStatus::Approved)
        {
            QSharedPointer<PendingInventoryData> pendingInventoryData(new PendingInventoryData);
            auto account = this->_settingManager->getAccount(deliveryRequest->getAccountId());
            if (!account.isNull())
            {
                pendingInventoryData->setAccountId(account->getId());
                pendingInventoryData->setAccountCode(account->getCode());
            }

            auto instrument = this->_settingManager->getInstrument(deliveryRequest->getAccountId(), deliveryRequest->getInstrumentId());
            if (!instrument.isNull())
            {
                deliveryRequest->setInstrumentDesc(instrument->getDescription());
                deliveryRequest->setRequireQuantity(Common::roundToString(deliveryRequest->getRequireQuantity().toDouble(), instrument->getPhysicalLotDecimal()));
            }

            if (this->_pendingInventories.contains(deliveryRequest->getId()))
            {
                this->_pendingInventories[deliveryRequest->getId()]->update(deliveryRequest);
            }
            else
            {
                pendingInventoryData->update(deliveryRequest);
                this->_pendingInventories.insert(deliveryRequest->getId(), pendingInventoryData);
            }
        }
        else
        {
            if (this->_pendingInventories.contains(deliveryRequest->getId()))
            {
                this->_pendingInventories[deliveryRequest->getId()]->update(deliveryRequest);
            }
        }
    }

    void TradingManager::refreshPendingInventoryFromScrapDeposit(QSharedPointer<ScrapDeposit> scrapDeposit)
    {
        if (scrapDeposit->getStatus() == ScrapDepositStatus::ExaminationInProcess
            || scrapDeposit->getStatus() == ScrapDepositStatus::ExaminationCompleted)
        {
            QSharedPointer<PendingInventoryData> pendingInventoryData(new PendingInventoryData);
            auto account = this->_settingManager->getAccount(scrapDeposit->getAccountId());
            if (!account.isNull())
            {
                pendingInventoryData->setAccountId(account->getId());
                pendingInventoryData->setAccountCode(account->getCode());
            }

            auto instrument = this->_settingManager->getScrapInstrument(scrapDeposit->getScrapInstrumentId());
            if (!instrument.isNull())
            {
                scrapDeposit->setScrapInstrumentCode(instrument->getDescription());
                int decimals = instrument->getQuantityDecimalDigits();
                scrapDeposit->setQuantityQDecNumberDigits(decimals);
                scrapDeposit->setRawQuantity(Common::roundToString(scrapDeposit->getRawQuantity().toDouble(), decimals));
                scrapDeposit->setFinalQuantity(Common::roundToString(scrapDeposit->getFinalQuantity().toDouble(), decimals));
                scrapDeposit->setAdjustedQuantity(Common::roundToString(scrapDeposit->getAdjustedQuantity().toDouble(), decimals));
            }

            if (this->_pendingInventories.contains(scrapDeposit->getId()))
            {
                this->_pendingInventories[scrapDeposit->getId()]->update(scrapDeposit);
            }
            else
            {
                pendingInventoryData->update(scrapDeposit);
                this->_pendingInventories.insert(scrapDeposit->getId(), pendingInventoryData);
            }
        }
        else
        {
            if (this->_pendingInventories.contains(scrapDeposit->getId()))
            {
                this->_pendingInventories[scrapDeposit->getId()]->update(scrapDeposit);
            }
        }
    }

    void TradingManager::readAccountElement(QXmlStreamReader& xmlStreamReader, QString& accountCodes, QStringList& resetAlertLevelAccountIds, QString& resetAlertLevelAccountCodes)
    {
        QString name = xmlStreamReader.name().toString();
        if (xmlStreamReader.name() == "Account") {
            QString accountIdStr = xmlStreamReader.attributes().value("ID").toString();
            if (accountIdStr.length() == 0) return;
            qDebug() << "TradingManager::readAccountElement: accountIdStr:" << accountIdStr;
            QUuid accountId = QUuid(accountIdStr);
            QSharedPointer<Business::Model::Setting::BusinessAccount> settingAccount = this->_settingManager->getAccount(accountId);
            if (settingAccount.isNull()) return;
            ///----------------------------------------------------------------------
            QSharedPointer<Fund> fund = this->getOrAddFund(accountId);
            fund->update(xmlStreamReader);
            fund->setCreditAmount(settingAccount->getCreditAmount());
            fund->setRatioIncludeCredit(this->_settingManager->getSystemSetting()->getRatioIncludeCredit());
            if (!settingAccount.isNull())
            {
                auto currency = this->_settingManager->getCurrency(settingAccount->getCurrencyId());
                if (!currency.isNull())
                {
                    fund->setCurrencyCode(currency->getId(), currency->getCode(), currency->getName());
                    fund->setDecimals(currency->getDecimals());
                }
                if (fund->needAlert(settingAccount->getDisplayAlert()))
                {
                    if (accountCodes != "")
                        accountCodes.append(";");
                    accountCodes.append(settingAccount->getCode());

                    if (this->_settingManager->isAllowTraderResetAlertLevel(settingAccount->getForbiddenAlert()))
                    {
                        if (resetAlertLevelAccountCodes != "")
                            resetAlertLevelAccountCodes.append(";");
                        resetAlertLevelAccountCodes.append(settingAccount->getCode());
                        resetAlertLevelAccountIds.append(accountIdStr);
                    }
                }
            }
            ////---------------------------------------------------------------------
            xmlStreamReader.readNext();
            while (!xmlStreamReader.atEnd()) {
                if (xmlStreamReader.isEndElement()) {
                    xmlStreamReader.readNext();
                    break;
                }
                if (xmlStreamReader.isStartElement()) {
                    if (xmlStreamReader.name() == "Funds") {
                        xmlStreamReader.readNext();
                        QUuid currencyId = settingAccount->getCurrencyId();
                        QDecNumber amount = settingAccount->getCreditAmount();
                        readSubFundElement(xmlStreamReader, accountId, currencyId, amount);
                    }
                    else if (xmlStreamReader.name() == "Transactions")
                    {
                        xmlStreamReader.readNext();
                        readTransactionRequestElement(xmlStreamReader, accountId, settingAccount);
                    }
                    else if (xmlStreamReader.name() == "DeliveryRequests")
                    {
                        xmlStreamReader.readNext();
                        readDeliveryRequestElement(xmlStreamReader, accountId);
                    }
                    else if (xmlStreamReader.name() == "ScrapDeposits")
                    {
                        xmlStreamReader.readNext();
                        readScrapDepositElement(xmlStreamReader, accountId);
                    }
                    else if (xmlStreamReader.name() == "Instruments") {
                        xmlStreamReader.readNext();
                        while (!xmlStreamReader.atEnd()) {
                            if (xmlStreamReader.name() == "Funds"
                                || xmlStreamReader.name() == "Transactions"
                                || xmlStreamReader.name() == "DeliveryRequests"
                                || xmlStreamReader.name() == "ScrapDeposits") {
                                break;
                            }
                            xmlStreamReader.readNext();
                        }

                    }
                    else
                        xmlStreamReader.readNext();
                }
                else
                    xmlStreamReader.readNext();
            }
        }
    }

    void TradingManager::readSubFundElement(QXmlStreamReader& xmlStreamReader, QUuid& accountId, QUuid&  accountCurrencyId, QDecNumber& creditAmount)
    {
        while (!xmlStreamReader.atEnd()) {
            if (xmlStreamReader.isEndElement()) {
                xmlStreamReader.readNext();//Funds end
                break;
            }
            if (xmlStreamReader.isStartElement()) {
                while (!xmlStreamReader.atEnd()) {
                    if (xmlStreamReader.isEndElement()) {
                        xmlStreamReader.readNext();//Fund end
                        break;
                    }
                    if (xmlStreamReader.name() == "Fund") {
                        //if (xmlStreamReader.attributes().value("Balance").toString().toDouble() != 0.0)
                        {
                            QUuid currencyId = QUuid(xmlStreamReader.attributes().value("CurrencyID").toString());
                            QSharedPointer<SubFund> subFund = this->getOrAddSubFund(accountId, currencyId);
                            subFund->setAccountId(accountId);
                            subFund->update(xmlStreamReader);
                            auto currency = this->_settingManager->getCurrency(currencyId);
                            if (!currency.isNull())
                            {
                                subFund->setCurrencyCode(currency->getCode(), currency->getName());
                                subFund->setDecimals(currency->getDecimals());
                            }
                            if (currencyId == accountCurrencyId)
                                subFund->setCreditAmount(creditAmount);
                            qDebug() << " subFunds.count:" << this->_subFunds.count() << " currencyId:"<< currencyId.toString();
                        }
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

    void TradingManager::readDeliveryRequestElement(QXmlStreamReader& xmlStreamReader, QUuid& accountId)
    {
        while (!xmlStreamReader.atEnd()) {
            if (xmlStreamReader.isEndElement()) {
                xmlStreamReader.readNext();//DeliveryRequests end
                break;
            }
            if (xmlStreamReader.isStartElement()) {
                while (!xmlStreamReader.atEnd()) {
                    if (xmlStreamReader.isEndElement()) {
                        xmlStreamReader.readNext();//DeliveryRequest end
                        break;
                    }
                    if (xmlStreamReader.name() == "DeliveryRequest") {
                        Common::Enum::DeliveryRequestStatus::DeliveryRequestStatus status = static_cast<Common::Enum::DeliveryRequestStatus::DeliveryRequestStatus>(xmlStreamReader.attributes().value("Status").toInt());
                        QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
                        QSharedPointer<DeliveryRequest> deliveryRequest = this->getOrAddDeliveryRequest(id);
                        deliveryRequest->update(xmlStreamReader);
                        if (status == Common::Enum::DeliveryRequestStatus::Canceled)
                        {
                            this->_deliveryRequests.remove(id);
                        }
                        else
                        {
                            deliveryRequest->setAccountId(accountId);
                            auto settingInstrument = this->_settingManager->getInstrument(accountId, deliveryRequest->getInstrumentId());
                            if (!settingInstrument.isNull())
                            {
                                deliveryRequest->setUnit(settingInstrument->getUnit());
                            }
                        }
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

    void TradingManager::readScrapDepositElement(QXmlStreamReader& xmlStreamReader, QUuid& accountId)
    {
        while (!xmlStreamReader.atEnd()) {
            if (xmlStreamReader.isEndElement()) {
                xmlStreamReader.readNext();//ScrapDeposits end
                break;
            }
            if (xmlStreamReader.isStartElement()) {
                while (!xmlStreamReader.atEnd()) {
                    if (xmlStreamReader.isEndElement()) {
                        xmlStreamReader.readNext();//ScrapDeposit end
                        break;
                    }
                    if (xmlStreamReader.name() == "ScrapDeposit") {
                        QUuid id = QUuid(xmlStreamReader.attributes().value("Id").toString());
                        QSharedPointer<ScrapDeposit> scrapDeposit = this->getOrAddScrapDeposit(id);
                        scrapDeposit->setAccountId(accountId);
                        scrapDeposit->update(xmlStreamReader);
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

    void TradingManager::readTransactionRequestElement(QXmlStreamReader& xmlStreamReader, QUuid&  accountId, QSharedPointer<Business::Model::Setting::BusinessAccount>& settingAccount)
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "initTradingData Transactions start";
        while (!xmlStreamReader.atEnd()) {
            QString n1 = xmlStreamReader.name().toString();
            if (xmlStreamReader.isEndElement()) {
                xmlStreamReader.readNext();//Transactions end
                break;
            }
            if (xmlStreamReader.isStartElement()) {
                while (!xmlStreamReader.atEnd()) {
                    if (xmlStreamReader.isEndElement()) {
                        xmlStreamReader.readNext();//Transaction end
                        break;
                    }
                    QString n2 = xmlStreamReader.name().toString();
                    if (xmlStreamReader.name() == "Transaction") {
                        QUuid tranId = QUuid(xmlStreamReader.attributes().value("ID").toString());
                        QUuid instrumentId = QUuid(xmlStreamReader.attributes().value("InstrumentID").toString());
                        QSharedPointer<Business::Model::Setting::BusinessInstrument> settingInstrument = this->_settingManager->getInstrument(accountId, instrumentId);

                        if (settingInstrument.isNull() || !this->_initializeManager->isExistInstrument(instrumentId))
                        {
                            xmlStreamReader.readNext();
                            while (xmlStreamReader.name() != "Transaction")
                                xmlStreamReader.readNext();
                            continue;
                        }

                        QSharedPointer<Model::Trading::Transaction> transaction = this->getOrAddTransaction(tranId);
                        transaction->setBusinessAccount(settingAccount);
                        transaction->setBusinessInstrument(settingInstrument);
                        transaction->update(xmlStreamReader);

                        xmlStreamReader.readNext();
                        while (!xmlStreamReader.atEnd()) {
                            if (xmlStreamReader.isEndElement()) {
                                break;
                            }
                            QString n3 = xmlStreamReader.name().toString();
                            if (xmlStreamReader.isStartElement()) {
                                QString n4 = xmlStreamReader.name().toString();
                                if (xmlStreamReader.name() == "Orders") {
                                    xmlStreamReader.readNext();
                                    readOrderElement(xmlStreamReader, transaction, settingInstrument);
                                }
                                else
                                    xmlStreamReader.readNext();
                            }
                            else
                                xmlStreamReader.readNext();
                        }
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
        QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "initTradingData Transactions end";
    }

    void TradingManager::readOrderElement(QXmlStreamReader& xmlStreamReader, QSharedPointer<Model::Trading::Transaction>& transaction, QSharedPointer<Business::Model::Setting::BusinessInstrument>& settingInstrument)
    {
        while (!xmlStreamReader.atEnd()) {
            if (xmlStreamReader.isEndElement()) {
                xmlStreamReader.readNext();//orders end
                break;
            }
            if (xmlStreamReader.isStartElement()) {
                while (!xmlStreamReader.atEnd()) {
                    if (xmlStreamReader.isEndElement()) {
                        xmlStreamReader.readNext();// order end
                        break;
                    }
                    if (xmlStreamReader.name() == "Order") {
                        QString executePrice = xmlStreamReader.attributes().value("ExecutePrice").toString();
                        QString livePrice = xmlStreamReader.attributes().value("LivePrice").toString();
                        QUuid orderId = QUuid(xmlStreamReader.attributes().value("ID").toString());
                        QSharedPointer<TradingOrder> order = this->getOrAddOrder(orderId);
                        order->setAccountId(transaction->getAccountId());
                        order->setInstrumentId(transaction->getInstrumentId());
                        order->setTransactionId(transaction->getId());
                        order->setLivePrice(Common::PriceHelper::toPrice(livePrice, settingInstrument));
                        order->setExecutePrice(Common::PriceHelper::toPrice(executePrice, settingInstrument));
                        order->setExecutePriceOrigin(executePrice);
                        order->setTransaction(transaction);
                        order->update(xmlStreamReader);
                        order->setIsPayoff();
                        if (order->isBOOrder()
                            && order->getIsOpen()
                            && !order->getLotBalance().isZero()
                            && order->getPhase() != Phase::Canceled
                            && order->getPhase() != Phase::Deleted)
                        {
                            if (!this->_boOrders.contains(orderId))
                                this->_boOrders.insert(orderId, order);
                        }
                        if ((order->getPhase() == Phase::Placing || order->getPhase() == Phase::Placed) &&
                            this->_pendingTransactionIds.contains(transaction->getId()))
                            this->_pendingTransactionIds.append(transaction->getId());
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

    void TradingManager::setInitData()
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "initTradingData openOrderInfo start";
        foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
        {
            if ((order->getPhase() == Phase::Executed && !order->getIsOpen())
                || order->getPhase() == Phase::Canceled
                || order->getPhase() == Phase::Deleted) continue;
            QList<QSharedPointer<Business::Model::Trading::OrderRelation>> orderRations = order->getOrderRelations();
            for (int i = 0; i < orderRations.length(); i++)
            {
                if (!this->_orders.contains(orderRations.at(i)->getOrderId1())) continue;
                QSharedPointer<TradingOrder> openOrder = this->_orders[orderRations.at(i)->getOrderId1()];
                if (openOrder.isNull()) continue;
                openOrder->addCloseOrder(order);
                QString openOrderInfo = "";
                if(openOrder->getPhase() == Phase::Executed || openOrder->getPhase() == Phase::Completed)
                {
                    openOrderInfo.append(DateTimeHelper::toDateString(openOrder->getExecuteTradeDay()));
                    openOrderInfo.append("x");
                    openOrderInfo.append(Common::roundToString(orderRations.at(i)->getLot().toDouble(), openOrder->getDecimalPalcesForLot()));
                    openOrderInfo.append("x");
                    openOrderInfo.append(openOrder->getExecutePrice().toQString());
                }
                else
                {
                    QSharedPointer<Business::Model::Trading::Transaction> transaction = openOrder->getTransaction();
                    openOrderInfo.append(DateTimeHelper::toDateString(transaction->getSubmitTime()));
                    openOrderInfo.append("x");
                    openOrderInfo.append(Common::roundToString(orderRations.at(i)->getLot().toDouble(), openOrder->getDecimalPalcesForLot()));
                    openOrderInfo.append("x");
                    openOrderInfo.append(openOrder->getSetPrice());
                }
//				if (!openOrder->getLotBalance().isZero() && openOrder->getIsOpen())
//				{
//                    if(openOrder->getPhase() == Phase::Executed)
//                    {
//                        openOrderInfo.append(DateTimeHelper::toDateString(openOrder->getExecuteTradeDay()));
//                        openOrderInfo.append("x");
//                        openOrderInfo.append(Common::roundToString(orderRations.at(i)->getLot().toDouble(), openOrder->getDecimalPalcesForLot()));
//                        openOrderInfo.append("x");
//                        openOrderInfo.append(openOrder->getExecutePrice().toQString());
//                    }
//                    else
//                    {
//                        QSharedPointer<Business::Model::Trading::Transaction> transaction = openOrder->getTransaction();
//                        openOrderInfo.append(DateTimeHelper::toDateString(transaction->getSubmitTime()));
//                        openOrderInfo.append("x");
//                        openOrderInfo.append(Common::roundToString(orderRations.at(i)->getLot().toDouble(), openOrder->getDecimalPalcesForLot()));
//                        openOrderInfo.append("x");
//                        openOrderInfo.append(openOrder->getSetPrice());
//                    }
//				}
//				else
//				{
//					openOrderInfo = DateTimeHelper::toDateString(openOrder->getTransaction()->getSubmitTime());
//				}
                orderRations.at(i)->setOpenOrderInfo(openOrderInfo);
            }

            if (order->getPhase() != Phase::Executed
                && order->getPhase() != Phase::Completed
                && order->getPhase() != Phase::Canceled
                && order->getPhase() != Phase::Deleted)
            {
                this->_workOrders.insert(order->getId(), order);
            }
        }
        foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
        {
            if (order->getPhase() == Phase::Executed && order->getIsOpen())
                order->caculateLimitSummary();
        }
        QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "initTradingData openOrderInfo end";

        QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "initTradingData interestFreeDay start";
        foreach(QSharedPointer<TradingOrder> order, this->_orders.values())
        {
            if (order.isNull() || order->getLotBalance().isZero() || order->getExecuteTradeDay() == DateTimeConstants::MinQDateTime || this->_settingManager->getTradeDay().isNull()) continue;
            int midTime = this->_settingManager->getTradeDay()->getCurrentDay().daysTo(order->getExecuteTradeDay());
            int valueDay = order->getIsBuy() ?
                order->getTransaction()->getBusinessInstrument()->getBuyInterestValueDay() :
                order->getTransaction()->getBusinessInstrument()->getSellInterestValueDay();
            if (valueDay <= 0 || (valueDay - midTime) < 0)
                continue;
            if (valueDay - midTime <= this->_settingManager->getSystemSetting()->getInterestFreeAlertDay())
            {
                QSharedPointer<InterestFreeDay> interestFreeDay(new InterestFreeDay);
                interestFreeDay->setCode(order->getCode());
                interestFreeDay->setTradeDay(DateTimeHelper::toDateString(order->getExecuteTradeDay()));
                interestFreeDay->setLotBalance(order->getLotBalanceInFormat());
                interestFreeDay->setExecutePrice(order->getExecutePrice().toQString());
                interestFreeDay->setInterestFreeDays(QString::number(valueDay));
                double interestRate = order->getIsBuy() ?
                    order->getTransaction()->getBusinessInstrument()->getInterestRateSetting()->getBuy().toDouble() :
                    order->getTransaction()->getBusinessInstrument()->getInterestRateSetting()->getSell().toDouble();
                interestFreeDay->setInterestRate(Common::roundToString(interestRate, 4));

                if (order->getPhysicalTradeSide() != Common::Enum::PhysicalTradeSide::None)
                {
                    QDecNumber lotBalance = order->getLotBalance();
                    auto deliveryRequestList = this->_deliveryRequests.values();
                    foreach(auto deliveryRequestItem, deliveryRequestList)
                    {
                        if (deliveryRequestItem->getAccountId() == order->getAccountId() &&
                            deliveryRequestItem->getInstrumentId() == order->getInstrumentId())
                        {
                            foreach(auto deliveryRequestOrderRelationItem, deliveryRequestItem->getDeliveryRequestOrderRelations())
                            {
                                if (deliveryRequestOrderRelationItem->getOpenOrderId() == order->getId()
                                    && deliveryRequestItem->getStatus() == Common::Enum::DeliveryRequestStatus::Accepted)
                                {
                                    lotBalance = lotBalance - deliveryRequestOrderRelationItem->getDeliveryLot();
                                }
                            }
                        }
                    }
                    if (lotBalance > 0.0)
                        this->_interestFreeDays.append(interestFreeDay);
                }
                else
                {
                    this->_interestFreeDays.append(interestFreeDay);
                }
            }

        }
        if (this->_interestFreeDays.count() > 0)
        {
            Common::SignalForwarder::getInstance()->sendInterestFreeDaysNotifySignal();
        }
        QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "initTradingData interestFreeDay end";
        this->setOrderMidifyRelation();

        /*foreach(QSharedPointer<DeliveryRequest> deliveryRequest, this->_deliveryRequests.values())
        {
        QMapIterator<QUuid, QDecNumber> delivaryLockLot(deliveryRequest->getOrderDelivaryLockLots());
        while (delivaryLockLot.hasNext()) {
        delivaryLockLot.next();
        if (this->_orders.contains(delivaryLockLot.key()))
        {
        this->_orders[delivaryLockLot.key()]->addDeliveryLockLot(delivaryLockLot.value());
        }
        }
        }*/
        QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "initTradingData end";

    }




}
