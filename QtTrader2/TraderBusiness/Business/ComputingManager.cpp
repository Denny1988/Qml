#include "ComputingManager.h"
#include "QuotationManager.h"
#include <qdebug.h>
#include "SettingManager.h"
#include "Business/Model/Trading/Transaction.h"

namespace Business
{
	QMutex ComputingManager::m_Mutex(QMutex::Recursive);
	ComputingManager::ComputingManager(QSharedPointer<SettingManager>& settingManager, QSharedPointer<QuotationManager>& quotationManager)
	{
		this->_settingManager = settingManager;
		this->_quotationManager = quotationManager;
	}

	//--------------create Computing model--------------
	void ComputingManager::updateOrAddComputingModel(QList<QSharedPointer<Model::Trading::TradingOrder>> orderList)
	{
		QMutexLocker locker(&ComputingManager::m_Mutex);
		QList<QUuid> accountIds;
		foreach (QSharedPointer<Model::Trading::TradingOrder> item, orderList)
		{
			if(item->getTransaction()->getOrderType() == OrderType::BinaryOption) continue;
			if(!accountIds.contains(item->getAccountId()))
				accountIds.append(item->getAccountId());
			this->addComputingModel(item);
		}
		if (accountIds.count() > 0)
			this->computePart(accountIds, true);
		else
			this->computeAll();
	}

	void ComputingManager::addComputingModel(QSharedPointer<Model::Trading::TradingOrder> tradingOrder)
	{
		QMutexLocker locker(&ComputingManager::m_Mutex);
		if(tradingOrder->getTransaction()->getOrderType() == OrderType::BinaryOption) return;
		QUuid accountId = tradingOrder->getAccountId();
		QUuid instrumentId = tradingOrder->getInstrumentId();
		QUuid orderId = tradingOrder->getId();

		if(!this->_orderMapping.contains(orderId))
		{
			AccountMapping accountMapping;
			accountMapping.accountId = accountId;
			accountMapping.instrumentId = instrumentId;
			this->_orderMapping.insert(orderId, accountMapping);
		}

		if(this->_computingAccounts.contains(accountId))
		{
			QSharedPointer<ComputingAccount> account = this->_computingAccounts[accountId];
			if(account->getDependency().contains(instrumentId.toString()))
			{
				auto instrument = account->getDependency()[instrumentId.toString()];
				if(!instrument->getDependency().contains(orderId.toString()))
				{
					QSharedPointer<ComputingOrder> order(new ComputingOrder);
					order->_accountId = accountId;
					order->_instrumentId = instrumentId;
					order->_orderId = orderId;
					order->calculate();

					instrument->addDependency(orderId.toString(), order);
				}
			}
			else
			{
				QSharedPointer<ComputingInstrument> instrument(new ComputingInstrument);
				instrument->_accountId = accountId;
				instrument->_instrumentId = instrumentId;

				QSharedPointer<ComputingOrder> order(new ComputingOrder);
				order->_accountId = accountId;
				order->_instrumentId = instrumentId;
				order->_orderId = orderId;
				order->calculate();

				instrument->addDependency(orderId.toString(), order);
				account->addDependency(instrumentId.toString(), instrument);
				this->_computingAccounts.insert(accountId.toString(), account);
			}
		}
		else
		{
			QSharedPointer<ComputingAccount> account(new ComputingAccount);
			account->_accountId = accountId;

			QSharedPointer<ComputingInstrument> instrument(new ComputingInstrument);
			instrument->_accountId = accountId;
			instrument->_instrumentId = instrumentId;

			QSharedPointer<ComputingOrder> order(new ComputingOrder);
			order->_accountId = accountId;
			order->_instrumentId = instrumentId;
			order->_orderId = orderId;
			order->calculate();

			instrument->addDependency(orderId.toString(), order);
			account->addDependency(instrumentId.toString(), instrument);
			this->_computingAccounts.insert(accountId.toString(), account);
		}
	}

	void ComputingManager::deleteComputingModel(QSharedPointer<Model::Trading::TradingOrder> tradingOrder)
	{
		QMutexLocker locker(&ComputingManager::m_Mutex);
		QUuid accountId = tradingOrder->getAccountId();
		QUuid instrumentId = tradingOrder->getInstrumentId();
		QUuid orderId = tradingOrder->getId();

		if(!this->_orderMapping.contains(orderId))
		{
			this->_orderMapping.remove(orderId);
		}

		if(this->_computingAccounts.contains(accountId))
		{
			QSharedPointer<ComputingAccount> account = this->_computingAccounts[accountId];
			if(account->getDependency().contains(instrumentId.toString()))
			{
				auto instrument = account->getDependency()[instrumentId.toString()];
				if(instrument->getDependency().contains(orderId.toString()))
				{
					this->_computingAccounts[accountId]->getDependency()[instrumentId.toString()]->removeDependency(orderId.toString());
				}
			}
		}
	}

	void ComputingManager::InitializeQuotation(QList<QSharedPointer<Model::InitialQuotation>> initialQuotationList)
	{
		foreach (QSharedPointer<Model::InitialQuotation> initialQuotation, initialQuotationList)
		{
			QString key = initialQuotation->getQuotePolicyId().toString() + initialQuotation->getInstrumentId().toString();
			this->addComputingQuotetation(key, initialQuotation);
		}
	}


	//---------------------compute method------------------------------
	void ComputingManager::computeAll()
	{
		QMutexLocker locker(&ComputingManager::m_Mutex);
		QList<QUuid> removeIds;
		QMapIterator<QUuid, QSharedPointer<ComputingAccount>> item(this->_computingAccounts);
		while (item.hasNext()) {
			item.next();
			if(this->_settingManager->containAccount(item.value()->_accountId))
				this->_computingAccounts[item.key()]->compute();
			else
				removeIds.append(item.key());
		}
		foreach (QUuid accountId, removeIds)
		{
			this->_computingAccounts.remove(accountId);
		}
	}

	void ComputingManager::computeAll(QList<QUuid> instrumentIds)
	{
		QMutexLocker locker(&ComputingManager::m_Mutex);
		QList<QUuid> removeIds;
		QMapIterator<QUuid, QSharedPointer<ComputingAccount>> item(this->_computingAccounts);
		while (item.hasNext()) {
			item.next();
			if(this->_settingManager->containAccount(item.value()->_accountId))
				this->_computingAccounts[item.key()]->compute(instrumentIds);
			else
				removeIds.append(item.key());
		}
		foreach (QUuid accountId, removeIds)
		{
			this->_computingAccounts.remove(accountId);
		}
	}

	void ComputingManager::computePart(QList<QUuid> accountIds, bool isLoad)
	{
		QMutexLocker locker(&ComputingManager::m_Mutex);
		foreach (QSharedPointer<ComputingAccount> account, this->_computingAccounts)
		{
			if(accountIds.contains(account->_accountId)
				&& (!isLoad || this->isComputForQuotation(account->_accountId)))
			{
				account->compute();
			}
		}
	}

	void ComputingManager::computePart(QUuid accountId)
	{
		QMutexLocker locker(&ComputingManager::m_Mutex);
		foreach (QSharedPointer<ComputingAccount> account, this->_computingAccounts)
		{
			if(accountId == account->_accountId)
			{
				account->compute();
			}
		}
	}

	bool ComputingManager::isComputForQuotation(QUuid accountId)
	{
		bool isNeedComput = true;
		auto settingAccount = this->_settingManager->getAccount(accountId);
		if(settingAccount.isNull())
		{
			return false;
		}
		foreach (QUuid instrumentId , settingAccount->getInstrumentIds())
		{
			auto quotation = this->getOverridedQuotationComputingResult(accountId, instrumentId);
			if(quotation.isNull() || (quotation->ask.isNullPrice() && quotation->bid.isNullPrice()))
				isNeedComput = false;
		}
		return isNeedComput;
	}

	
	//--------------------UI GetResult-------------------------------
	QMap<QUuid, QSharedPointer<AccountComputingResult>> ComputingManager::getAccountComputingResults(QList<QUuid> accountIds)
	{
		QMap<QUuid, QSharedPointer<AccountComputingResult>> accountComputingResultMap;
		foreach (QUuid accountId, accountIds)
		{
			if(this->_computingAccounts.contains(accountId))
			{
				accountComputingResultMap.insert(accountId, qSharedPointerCast<AccountComputingResult, DemandComputingResult>(this->_computingAccounts.value(accountId)->getResult()));
			}
		}
		return accountComputingResultMap;
	}

	QMap<QUuid, QSharedPointer<OrderComputingResult>> ComputingManager::getOrderComputingResults(QList<QUuid> orderIds)
	{
		QMap<QUuid, QSharedPointer<OrderComputingResult>> orderComputingResultMap;
		foreach (QUuid orderId, orderIds)
		{
			AccountMapping accountMapping = this->_orderMapping.value(orderId);
			ComputingInstrument* instrument = (ComputingInstrument*)this->_computingAccounts.value(accountMapping.accountId)->getDependency().value(accountMapping.instrumentId.toString()).data();
			ComputingOrder* order = (ComputingOrder*)instrument->getDependency().value(orderId.toString()).data();
			orderComputingResultMap.insert(orderId, qSharedPointerCast<OrderComputingResult, DemandComputingResult>(order->getResult()));
		}
		return orderComputingResultMap;
	}

	QSharedPointer<OrderComputingResult> ComputingManager::getOrAddOrderComputingResult(QSharedPointer<Model::Trading::TradingOrder> order)
	{
		QUuid id = order->getId();
		if(!this->_orderMapping.contains(id))
		{
			this->addComputingModel(order);
		}
		AccountMapping accountMapping = this->_orderMapping.value(id);
		ComputingInstrument* computingInstrument = (ComputingInstrument*)this->_computingAccounts.value(accountMapping.accountId)->getDependency().value(accountMapping.instrumentId.toString()).data();
		if(!computingInstrument->getDependency().contains(id.toString()))
		{
			this->addComputingModel(order);
			if(!computingInstrument->getDependency().contains(id.toString())) return QSharedPointer<OrderComputingResult>(Q_NULLPTR);
		}
		ComputingOrder* computingOrder = (ComputingOrder*)computingInstrument->getDependency().value(id.toString()).data();
		return qSharedPointerCast<OrderComputingResult, DemandComputingResult>(computingOrder->getResult());
	}

	QSharedPointer<AccountComputingResult> ComputingManager::getAccountComputingResult(QUuid accountId)
	{
		if(this->_computingAccounts.contains(accountId))
		{
			return qSharedPointerCast<AccountComputingResult, DemandComputingResult>(this->_computingAccounts.value(accountId)->getResult());

		}
		return QSharedPointer<AccountComputingResult>(Q_NULLPTR);
	}

	QSharedPointer<OverridedQuotationComputingResult> ComputingManager::getOverridedQuotationComputingResult(QUuid accountId, QUuid instrumentId)
	{		
		QSharedPointer<Model::Setting::BusinessAccount> settingAccount = this->_settingManager->getAccount(accountId);		
		if(settingAccount.isNull())
			return QSharedPointer<OverridedQuotationComputingResult>(NULL);
		QString privatekey = settingAccount->getQuotePolicyId().toString() + instrumentId.toString();
		if(!this->_computingOverridedQuotations.contains(privatekey)){
			QSharedPointer<Model::InitialQuotation> quotation = this->_quotationManager->getOverridedQuotation(settingAccount->getQuotePolicyId(), instrumentId);
			if(quotation.isNull())
			{
				QString publickey = settingAccount->getPublicQuotePolicyId().toString() + instrumentId.toString();
				if(this->_computingOverridedQuotations.contains(publickey)){
					QSharedPointer<OverridedQuotation> overridedQuotation = this->_computingOverridedQuotations[publickey];
					return qSharedPointerCast<OverridedQuotationComputingResult, DemandComputingResult>(overridedQuotation->getResult());
				}
				else
				{
					QSharedPointer<Model::Setting::CalculateQuotationDefaultSetting> calculateQuotationDefaultSetting = this->_settingManager->getOrAddCalculateQuotationDefaultSetting(instrumentId);
					QSharedPointer<OverridedQuotation> overridedQuotation(new OverridedQuotation);
					overridedQuotation->_quotePolicyId = settingAccount->getPublicQuotePolicyId();
					overridedQuotation->_instrumentId = instrumentId;
					overridedQuotation->_denominator = calculateQuotationDefaultSetting->getDenominator();
					overridedQuotation->_numeratorUnit = calculateQuotationDefaultSetting->getNumeratorUnit();
                    overridedQuotation->_priceType = calculateQuotationDefaultSetting->getPriceType();
					overridedQuotation->_caculateChangeWithDenominator = calculateQuotationDefaultSetting->getCaculateChangeWithDenominator();
					this->_computingOverridedQuotations.insert(publickey, overridedQuotation);
					return qSharedPointerCast<OverridedQuotationComputingResult, DemandComputingResult>(overridedQuotation->compute());
				}	
			}
			else
			{
				this->addComputingQuotetation(privatekey, quotation);
			}
		}
		QSharedPointer<OverridedQuotation> overridedQuotation = this->_computingOverridedQuotations[privatekey];
		if(overridedQuotation.isNull())
		{
			this->_computingOverridedQuotations.remove(privatekey);
			return QSharedPointer<OverridedQuotationComputingResult>(NULL);
		}
		else
			return qSharedPointerCast<OverridedQuotationComputingResult, DemandComputingResult>(overridedQuotation->getResult());
	}

	bool ComputingManager::hasOverridedQuotationComputingResultByQuotePolicyId(QUuid quotePolicyId, QUuid instrumentId)
	{
		QString key = quotePolicyId.toString() + instrumentId.toString();
		return this->_computingOverridedQuotations.contains(key);
	}

	QSharedPointer<OverridedQuotationComputingResult> ComputingManager::getOverridedQuotationComputingResultByQuotePolicyId(QUuid quotePolicyId, QUuid instrumentId)
	{		
		QString key = quotePolicyId.toString() + instrumentId.toString();
		if(!this->_computingOverridedQuotations.contains(key))
		{
			QSharedPointer<Model::InitialQuotation> quotation = this->_quotationManager->getOverridedQuotation(quotePolicyId, instrumentId);
			if(quotation.isNull())
			{
				QSharedPointer<Model::Setting::BusinessAccount> settingAccount = this->_settingManager->getFirstAccount();	
				QString publickey = settingAccount->getPublicQuotePolicyId().toString() + instrumentId.toString();
				if(this->_computingOverridedQuotations.contains(publickey)){
					QSharedPointer<OverridedQuotation> overridedQuotation = this->_computingOverridedQuotations[publickey];
					return qSharedPointerCast<OverridedQuotationComputingResult, DemandComputingResult>(overridedQuotation->getResult());
				}
				else
				{
					QSharedPointer<Model::Setting::CalculateQuotationDefaultSetting> calculateQuotationDefaultSetting = this->_settingManager->getOrAddCalculateQuotationDefaultSetting(instrumentId);
					QSharedPointer<OverridedQuotation> overridedQuotation(new OverridedQuotation);
					overridedQuotation->_quotePolicyId = settingAccount->getPublicQuotePolicyId();
					overridedQuotation->_instrumentId = instrumentId;
					overridedQuotation->_denominator = calculateQuotationDefaultSetting->getDenominator();
					overridedQuotation->_numeratorUnit = calculateQuotationDefaultSetting->getNumeratorUnit();
                    overridedQuotation->_priceType = calculateQuotationDefaultSetting->getPriceType();
					overridedQuotation->_caculateChangeWithDenominator = calculateQuotationDefaultSetting->getCaculateChangeWithDenominator();
					this->_computingOverridedQuotations.insert(publickey, overridedQuotation);
					return qSharedPointerCast<OverridedQuotationComputingResult, DemandComputingResult>(overridedQuotation->compute());
				}		
			}
			else
			{
				this->addComputingQuotetation(key, quotation);
			}
		}
		QSharedPointer<OverridedQuotation> overridedQuotation = this->_computingOverridedQuotations[key];
		if(overridedQuotation.isNull())
		{
			this->_computingOverridedQuotations.remove(key);
			return QSharedPointer<OverridedQuotationComputingResult>(NULL);
		}
		else
			return qSharedPointerCast<OverridedQuotationComputingResult, DemandComputingResult>(overridedQuotation->getResult());
	}

	QSharedPointer<OverridedQuotationComputingResult> ComputingManager::getOverridedQuotationComputingResultByDefaultQuotePolicy(QUuid instrumentId)
	{
		QSharedPointer<Model::Setting::BusinessAccount> settingAccount = this->_settingManager->getFirstAccount();	
		QString publickey = settingAccount->getPublicQuotePolicyId().toString() + instrumentId.toString();
		if(this->_computingOverridedQuotations.contains(publickey)){
			QSharedPointer<OverridedQuotation> overridedQuotation = this->_computingOverridedQuotations[publickey];
			return qSharedPointerCast<OverridedQuotationComputingResult, DemandComputingResult>(overridedQuotation->getResult());
		}
		else
		{
			QSharedPointer<Model::Setting::CalculateQuotationDefaultSetting> calculateQuotationDefaultSetting = this->_settingManager->getOrAddCalculateQuotationDefaultSetting(instrumentId);
			QSharedPointer<OverridedQuotation> overridedQuotation(new OverridedQuotation);
			overridedQuotation->_quotePolicyId = settingAccount->getPublicQuotePolicyId();
			overridedQuotation->_instrumentId = instrumentId;
			overridedQuotation->_denominator = calculateQuotationDefaultSetting->getDenominator();
			overridedQuotation->_numeratorUnit = calculateQuotationDefaultSetting->getNumeratorUnit();
            overridedQuotation->_priceType = calculateQuotationDefaultSetting->getPriceType();
			overridedQuotation->_caculateChangeWithDenominator = calculateQuotationDefaultSetting->getCaculateChangeWithDenominator();
			this->_computingOverridedQuotations.insert(publickey, overridedQuotation);
			return qSharedPointerCast<OverridedQuotationComputingResult, DemandComputingResult>(overridedQuotation->compute());
		}		
	}

	void ComputingManager::computeQuotation(QList<QUuid> instrumentIds)
	{
		auto item = this->_computingOverridedQuotations.constBegin();
		while (item != this->_computingOverridedQuotations.constEnd()) {
			if(instrumentIds.contains(item.value()->_instrumentId))
			{
				QUuid instrumentId = item.value()->_instrumentId;
				QUuid accountId = this->_settingManager->getAccountIdByQuotePolicyId(item.value()->_quotePolicyId, instrumentId);
				QSharedPointer<Model::Setting::BusinessInstrument> settingInstrument =  this->_settingManager->getInstrument(accountId, instrumentId);
				if(settingInstrument.isNull())
				{
					QSharedPointer<Model::Setting::CalculateQuotationDefaultSetting> calculateQuotationDefaultSetting = this->_settingManager->getOrAddCalculateQuotationDefaultSetting(instrumentId);
					this->_computingOverridedQuotations[item.key()]->_denominator = calculateQuotationDefaultSetting->getDenominator();
					this->_computingOverridedQuotations[item.key()]->_numeratorUnit = calculateQuotationDefaultSetting->getNumeratorUnit();
                    this->_computingOverridedQuotations[item.key()]->_priceType = calculateQuotationDefaultSetting->getPriceType();
					this->_computingOverridedQuotations[item.key()]->_caculateChangeWithDenominator = calculateQuotationDefaultSetting->getCaculateChangeWithDenominator();	
				}
				else
				{
					this->_computingOverridedQuotations[item.key()]->_denominator = settingInstrument->getCalculateSetting()->getDenominator();
					this->_computingOverridedQuotations[item.key()]->_numeratorUnit = settingInstrument->getCalculateSetting()->getNumeratorUnit();
                    this->_computingOverridedQuotations[item.key()]->_priceType = settingInstrument->getPriceType();
					this->_computingOverridedQuotations[item.key()]->_caculateChangeWithDenominator = settingInstrument->getCalculateSetting()->getCaculateChangeWithDenominator();
				}

				this->_computingOverridedQuotations[item.key()]->compute();
			}
			++item;
		}
	}

	void ComputingManager::computeQuotationOrAddComputingQuotetation(QSharedPointer<Model::InitialQuotation> quotation)
	{
		QString key = quotation->getQuotePolicyId().toString() + quotation->getInstrumentId().toString();
		if(this->_computingOverridedQuotations.contains(key))
		{
			auto item = this->_computingOverridedQuotations.constBegin();
			while (item != this->_computingOverridedQuotations.constEnd()) {
				if(item.key() == key)
				{
					this->_computingOverridedQuotations[key]->compute();
				}
				++item;
			}
		}
		else
		{
			this->addComputingQuotetation(key, quotation);
		}
	}

	//////-------------------------------Private method---------------------------------/////
	void ComputingManager::addComputingQuotetation(QString key, QSharedPointer<Model::InitialQuotation> initialQuotation)
	{
		QSharedPointer<OverridedQuotation> overridedQuotation(new OverridedQuotation);
		overridedQuotation->_instrumentId = initialQuotation->getInstrumentId();
		overridedQuotation->_quotePolicyId = initialQuotation->getQuotePolicyId();
		QUuid accountId = this->_settingManager->getAccountIdByQuotePolicyId(overridedQuotation->_quotePolicyId, overridedQuotation->_instrumentId);
		if (accountId.isNull()) return;
		QSharedPointer<Model::Setting::BusinessAccount> settingAccount =  this->_settingManager->getAccount(accountId);
		if(settingAccount.isNull()) return;
		if(!settingAccount.isNull() && settingAccount->containsInstrument(overridedQuotation->_instrumentId))
		{
			QSharedPointer<Model::Setting::BusinessInstrument> settingInstrument =  this->_settingManager->getInstrument(accountId, overridedQuotation->_instrumentId);
			if(overridedQuotation->_quotePolicyId != settingAccount->getPublicQuotePolicyId())
			{
				overridedQuotation->_denominator = settingInstrument->getCalculateSetting()->getDenominator();
				overridedQuotation->_numeratorUnit = settingInstrument->getCalculateSetting()->getNumeratorUnit();
                overridedQuotation->_priceType = settingInstrument->getPriceType();
				overridedQuotation->_caculateChangeWithDenominator = settingInstrument->getCalculateSetting()->getCaculateChangeWithDenominator();
			}
			else
			{
				QSharedPointer<Model::Setting::CalculateQuotationDefaultSetting> calculateQuotationDefaultSetting = this->_settingManager->getOrAddCalculateQuotationDefaultSetting(overridedQuotation->_instrumentId);
				overridedQuotation->_denominator = calculateQuotationDefaultSetting->getDenominator();
				overridedQuotation->_numeratorUnit = calculateQuotationDefaultSetting->getNumeratorUnit();
                overridedQuotation->_priceType = calculateQuotationDefaultSetting->getPriceType();
				overridedQuotation->_caculateChangeWithDenominator = calculateQuotationDefaultSetting->getCaculateChangeWithDenominator();	
			}
		}
		else
		{
			QSharedPointer<Model::Setting::CalculateQuotationDefaultSetting> calculateQuotationDefaultSetting = this->_settingManager->getOrAddCalculateQuotationDefaultSetting(overridedQuotation->_instrumentId);
			overridedQuotation->_denominator = calculateQuotationDefaultSetting->getDenominator();
			overridedQuotation->_numeratorUnit = calculateQuotationDefaultSetting->getNumeratorUnit();
            overridedQuotation->_priceType = calculateQuotationDefaultSetting->getPriceType();
			overridedQuotation->_caculateChangeWithDenominator = calculateQuotationDefaultSetting->getCaculateChangeWithDenominator();	
		}
		this->_computingOverridedQuotations.insert(key, overridedQuotation);	
	}

}
