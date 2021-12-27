#include "ComputingAccount.h"
#include "Business/BusinessManager.h"
#include "Common/LogHelper.h"
#include "Common/Enum/CommonEnum.h"

namespace Business
{
	namespace Model
	{
		namespace Computing
		{
			QSharedPointer<DemandComputingResult> ComputingAccount::doCompute()
			{
				//QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "ComputingAccount::doCompute";
                //qDebug() <<  "ComputingAccount::doCompute start";
				QSharedPointer<AccountComputingResult> accountComputingResult(new AccountComputingResult());
				QSharedPointer<Model::Setting::BusinessAccount> settingAccount = Business::BusinessManager::getInstance()->getSettingManager()->getAccount(this->_accountId);
				if (settingAccount.isNull()) return accountComputingResult;
				accountComputingResult->accountId = this->_accountId;
				bool includeFeeOnRiskAction = Business::BusinessManager::getInstance()->getSettingManager()->getSystemSetting()->getIncludeFeeOnRiskAction();
                QMapIterator<QString, QSharedPointer<DemandComputing>> item(this->dependencies);
				while (item.hasNext())
				{
					item.next();
					QSharedPointer<InstrumentComputingResult> instrumentComputingResult = qSharedPointerCast<InstrumentComputingResult, DemandComputingResult>(this->dependencies[item.key()]->getResult());

					if (accountComputingResult->fund.isNull())
					{
						QSharedPointer<ChangeFund> fund(new ChangeFund);
						accountComputingResult->fund = fund;
					}

					if (!settingAccount->getIsMultiCurrency())
					{
						accountComputingResult->fund->necessary += instrumentComputingResult->necessary;
						accountComputingResult->fund->partialPaymentPhysicalNecessary += instrumentComputingResult->partialPaymentPhysicalNecessary;
						accountComputingResult->fund->tradePLFloat += instrumentComputingResult->tradePLFloat;
						accountComputingResult->fund->interestPLFloat += instrumentComputingResult->interestPLFloat;
						accountComputingResult->fund->storagePLFloat += instrumentComputingResult->storagePLFloat;
						accountComputingResult->fund->pledgeAmount += instrumentComputingResult->pledgeAmount;
						accountComputingResult->fund->totalPaidAmount += instrumentComputingResult->totalPaidAmount;
						accountComputingResult->fund->totalUnpaidAmount += instrumentComputingResult->totalUnpaidAmount;
						if (includeFeeOnRiskAction)
						{
							accountComputingResult->fund->estimateCloseCommission -= instrumentComputingResult->estimateCloseCommission;
							accountComputingResult->fund->estimateCloseLevy -= instrumentComputingResult->estimateCloseLevy;
						}
						else
						{
							accountComputingResult->fund->estimateCloseCommission = 0.0;
							accountComputingResult->fund->estimateCloseLevy = 0.0;
						}
						/*qDebug() << "TraderPL:" << accountComputingResult->fund->tradePLFloat.toString();
						qDebug() << "TraderPL:" << accountComputingResult->fund->tradePLFloat.toDouble();*/
						//qDebug() << "PhysicalNecessary:" << accountComputingResult->fund->partialPaymentPhysicalNecessary.toDouble();
						//qDebug() << "Line56 necessary:" << accountComputingResult->fund->necessary.toDouble();
					}
					else
					{
                         //qDebug() << "ComputingAccount::doCompute currencyId:" << instrumentComputingResult->currencyId.toString();
						//currencyId
						if (accountComputingResult->subFund.contains(instrumentComputingResult->currencyId))
						{
							/*qDebug() << "currencyId:" << instrumentComputingResult->currencyId.toString();
							qDebug() << "InstrumentTraderPL:" << instrumentComputingResult->tradePLFloat.toString();
							qDebug() << "AccountTraderPL:" << instrumentComputingResult->tradePLFloat.toString();*/
							QSharedPointer<ChangeFund> fund = accountComputingResult->subFund[instrumentComputingResult->currencyId];
							fund->necessary += instrumentComputingResult->necessary;
							fund->partialPaymentPhysicalNecessary += instrumentComputingResult->partialPaymentPhysicalNecessary;
							fund->tradePLFloat += instrumentComputingResult->tradePLFloat;
							fund->interestPLFloat += instrumentComputingResult->interestPLFloat;
							fund->storagePLFloat += instrumentComputingResult->storagePLFloat;
							fund->pledgeAmount += instrumentComputingResult->pledgeAmount;
							fund->totalPaidAmount += instrumentComputingResult->totalPaidAmount;
							fund->totalUnpaidAmount += instrumentComputingResult->totalUnpaidAmount;
							if (includeFeeOnRiskAction)
							{
								fund->estimateCloseCommission -= instrumentComputingResult->estimateCloseCommission;
								fund->estimateCloseLevy -= instrumentComputingResult->estimateCloseLevy;
							}
							else
							{
								fund->estimateCloseCommission = 0.0;
								fund->estimateCloseLevy = 0.0;
							}
							//qDebug() << "Line85 necessary:" << instrumentComputingResult->necessary.toDouble();
                            //qDebug() << "ComputingAccount::doCompute currencyId111:" << instrumentComputingResult->currencyId.toString();
						}
						else
						{
							QSharedPointer<ChangeFund> fund(new ChangeFund);
							fund->currencyId = instrumentComputingResult->currencyId;
							fund->necessary = instrumentComputingResult->necessary;
							fund->partialPaymentPhysicalNecessary = instrumentComputingResult->partialPaymentPhysicalNecessary;
							fund->tradePLFloat = instrumentComputingResult->tradePLFloat;
							fund->interestPLFloat = instrumentComputingResult->interestPLFloat;
							fund->storagePLFloat = instrumentComputingResult->storagePLFloat;
							fund->pledgeAmount = instrumentComputingResult->pledgeAmount;
							fund->totalPaidAmount = instrumentComputingResult->totalPaidAmount;
							fund->totalUnpaidAmount = instrumentComputingResult->totalUnpaidAmount;
							fund->estimateCloseCommission -= instrumentComputingResult->estimateCloseCommission;
							fund->estimateCloseLevy -= instrumentComputingResult->estimateCloseLevy;
							accountComputingResult->subFund.insert(instrumentComputingResult->currencyId, fund);
                            //qDebug() << "ComputingAccount::doCompute currencyId222:" << instrumentComputingResult->currencyId.toString();
						}
					}
				}

				foreach(QSharedPointer<ChangeFund> accountCurrency, accountComputingResult->subFund)
				{
					QUuid sourceCurrencyId = accountCurrency->currencyId;
					QUuid targetCurrencyId = settingAccount->getCurrencyId();
					QSharedPointer<Model::Setting::CurrencyRate> currencyRate = Business::BusinessManager::getInstance()->getSettingManager()->getCurrencyRate(sourceCurrencyId, targetCurrencyId);
					if (!currencyRate.isNull())
					{
						accountComputingResult->fund->tradePLFloat += currencyRate->exchange(accountCurrency->tradePLFloat);
						accountComputingResult->fund->interestPLFloat += currencyRate->exchange(accountCurrency->interestPLFloat);
						accountComputingResult->fund->storagePLFloat += currencyRate->exchange(accountCurrency->storagePLFloat);
						accountComputingResult->fund->pledgeAmount += currencyRate->exchange(accountCurrency->pledgeAmount);
						accountComputingResult->fund->totalPaidAmount += currencyRate->exchange(accountCurrency->totalPaidAmount);
						accountComputingResult->fund->totalUnpaidAmount += currencyRate->exchange(accountCurrency->totalUnpaidAmount);
						accountComputingResult->fund->necessary += currencyRate->exchange(accountCurrency->necessary);
						accountComputingResult->fund->partialPaymentPhysicalNecessary += currencyRate->exchange(accountCurrency->partialPaymentPhysicalNecessary);
						accountComputingResult->fund->estimateCloseCommission += currencyRate->exchange(accountCurrency->estimateCloseCommission);
						accountComputingResult->fund->estimateCloseLevy += currencyRate->exchange(accountCurrency->estimateCloseLevy);
					}
					else
					{
						accountComputingResult->fund->tradePLFloat += accountCurrency->tradePLFloat;
						accountComputingResult->fund->interestPLFloat += accountCurrency->interestPLFloat;
						accountComputingResult->fund->storagePLFloat += accountCurrency->storagePLFloat;
						accountComputingResult->fund->pledgeAmount += accountCurrency->pledgeAmount;
						accountComputingResult->fund->totalPaidAmount += accountCurrency->totalPaidAmount;
						accountComputingResult->fund->totalUnpaidAmount += accountCurrency->totalUnpaidAmount;
						accountComputingResult->fund->necessary += accountCurrency->necessary;
						accountComputingResult->fund->partialPaymentPhysicalNecessary += accountCurrency->partialPaymentPhysicalNecessary;
						accountComputingResult->fund->estimateCloseCommission += accountCurrency->estimateCloseCommission;
						accountComputingResult->fund->estimateCloseLevy += accountCurrency->estimateCloseLevy;
					}
				}


				auto accountBalance = Business::BusinessManager::getInstance()->getTradingManager()->getOrAddFund(this->_accountId);
				if (!accountBalance.isNull())
				{
					auto currency = Business::BusinessManager::getInstance()->getSettingManager()->getCurrency(settingAccount->getCurrencyId());
					if (!currency.isNull())
					{
						accountBalance->setCurrencyCode(currency->getId(), currency->getCode(), currency->getName());
						accountBalance->setDecimals(currency->getDecimals());
					}
					accountBalance->setCreditAmount(settingAccount->getCreditAmount());
                    auto systemSetting = Business::BusinessManager::getInstance()->getSettingManager()->getSystemSetting();
                    if(!systemSetting.isNull())
                        accountBalance->setRatioIncludeCredit(systemSetting->getRatioIncludeCredit());
					accountBalance->updateChangeFund(accountComputingResult->fund);
				}

				if (accountComputingResult->subFund.count() == 0)
				{
					auto accountCurrency = Business::BusinessManager::getInstance()->getTradingManager()->getOrAddSubFund(this->_accountId, settingAccount->getCurrencyId());
					if (!accountCurrency.isNull())
					{
						auto currency = Business::BusinessManager::getInstance()->getSettingManager()->getCurrency(accountCurrency->getCurrencyId());
						if (!currency.isNull())
						{
							accountCurrency->setCurrencyCode(currency->getCode(), currency->getName());
							accountCurrency->setDecimals(currency->getDecimals());
						}
						accountCurrency->setCreditAmount(accountBalance->getCreditAmount());
						accountCurrency->updateChangeFund(accountComputingResult->fund);
					}
				}
				else
				{
					foreach(auto item, accountComputingResult->subFund)
					{
						auto accountCurrency = Business::BusinessManager::getInstance()->getTradingManager()->getOrAddSubFund(this->_accountId, item->currencyId);
						if (!accountCurrency.isNull())
						{
							auto currency = Business::BusinessManager::getInstance()->getSettingManager()->getCurrency(accountCurrency->getCurrencyId());
							if (!currency.isNull())
							{
								accountCurrency->setCurrencyCode(currency->getCode(), currency->getName());
								accountCurrency->setDecimals(currency->getDecimals());
							}
							accountCurrency->updateChangeFund(item);
							if (settingAccount->getCurrencyId() == item->currencyId)
								accountCurrency->setCreditAmount(accountBalance->getCreditAmount());
						}
					}
				}
				return accountComputingResult;
			}

		}
	}
}
