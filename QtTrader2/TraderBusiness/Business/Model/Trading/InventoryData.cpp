#include "InventoryData.h"
#include "Transaction.h"
#include "Business/SettingManager.h"
#include "Business/ComputingManager.h"
#include "Common/Price.h"
#include "Common/Util/Convert_Common.h"
#include "Business/Model/Setting/BusinessAccount.h"
#include "Business/Model/Setting/BusinessInstrument.h"
#include "Business/BusinessHelper.h"
#include "Business/Model/Computing/ComputingResult.h"
#include "Common/Constants/DateTimeConstants.h"
using namespace Common::Constants;

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			InventoryData::InventoryData(QSharedPointer<SettingManager> settingManager,
				QSharedPointer<ComputingManager> computingManager,
				QSharedPointer<Business::Model::Setting::BusinessAccount> businessAccount,
				QSharedPointer<Business::Model::Setting::BusinessInstrument> businessInstrument)
			{
				this->_settingManager = settingManager;
				this->_computingManager = computingManager;
				this->_businessAccount = businessAccount;
				this->_businessInstrument = businessInstrument;
				this->_decimal = 1;
				this->initDeliveryTime();
			}

			//////---------------------private method--------------------------///////
			void InventoryData::setCanPrepayment(QSharedPointer<InventoryItemData> inventoryItemData)
			{
				auto InstalmentSetting =  inventoryItemData->_tradingOrder->getTransaction()->getBusinessInstrument()->getInstalmentSetting();
				if (!InstalmentSetting.isNull())
				{
					AdvancePaymentOption::AdvancePaymentOption advancePaymentOption = InstalmentSetting->getAdvancePayment();
					if (!inventoryItemData->_tradingOrder->getIsPayoff())
					{
						inventoryItemData->_canPrepayment = advancePaymentOption == AdvancePaymentOption::AllowAll 
							||(inventoryItemData->_tradingOrder->getInstalmentFrequence() == Frequence::Occasional && advancePaymentOption == AdvancePaymentOption::AllowPrepayment)
							|| (inventoryItemData->_tradingOrder->getInstalmentFrequence() != Frequence::Occasional && advancePaymentOption == AdvancePaymentOption::AllowInstalment);

						inventoryItemData->_canInstalment = (inventoryItemData->_tradingOrder->getInstalmentFrequence() != Frequence::Occasional);
					}
					else
					{
						inventoryItemData->_canPrepayment = false;
					}
				}
				else
				{
					inventoryItemData->_canPrepayment = false;
				}
			}

			void InventoryData::setCanClose(QSharedPointer<InventoryItemData> inventoryItemData)
			{            
				auto InstalmentSetting =  inventoryItemData->_tradingOrder->getTransaction()->getBusinessInstrument()->getInstalmentSetting();
				if (!InstalmentSetting.isNull())
				{
					if (InstalmentSetting->getAllowClose() == AllowCloseInstalment::DisAllow
						|| (InstalmentSetting->getAllowClose() == AllowCloseInstalment::AllowWhenNoOutstandingInstalment 
						&& inventoryItemData->_tradingOrder->getIsInstalmentOverdue()))
						inventoryItemData->_tradingOrder->setCanClose(false);
					else
					{
						if (InstalmentSetting->getAllowClose() == AllowCloseInstalment::AllowPrepayment 
							&& inventoryItemData->_tradingOrder->getInstalmentFrequence() != Frequence::Occasional)
							inventoryItemData->_tradingOrder->setCanClose(false);
						else
							inventoryItemData->_tradingOrder->setCanClose(true);
					}
				}
			}

			void InventoryData::initDeliveryTime()
			{
				this->_deliveryTimes.clear();
				int deliveryTimeBeginDay = this->_businessInstrument->getDeliveryTimeBeginDay();
				int deliveryTimeEndDay = this->_businessInstrument->getDeliveryTimeEndDay();
                this->_endDeliveryTime = QDate::fromString("9999-01-01",DateTimeConstants::DateFormat);
                QDate now = QDate::currentDate();
				if (deliveryTimeEndDay == 0)
				{
					this->_deliveryTime = this->getBeginDateTime(deliveryTimeBeginDay, now);
					this->_beginDeliveryTime = this->_deliveryTime;					
				}
				else
				{
					this->_deliveryTime = this->getBeginDateTime(deliveryTimeBeginDay, now); 
					int i = 2;
					QDate recordDay = this->_deliveryTime;
					while (true)
					{
						if (i > deliveryTimeEndDay)
							break;
						recordDay = this->getVaildDeliveryDays(recordDay.addDays(1));
						this->_deliveryTimes.append(recordDay);
						i++;                    
					}
					this->_deliveryTimes.append(this->_deliveryTime);
					this->_beginDeliveryTime = this->_deliveryTime;
					foreach (QDate date, this->_deliveryTimes)
					{
						if(this->_endDeliveryTime<date)
							this->_endDeliveryTime = date;
					}
				}
				
			}

			QDate InventoryData::isDeliveryHolidaysDay(QDate date)
			{
				auto deliveryHoliday = this->_settingManager->getDeliveryHoliday(date);
				if (deliveryHoliday.isNull())
				{
					return date;
				}
				else
				{
					return deliveryHoliday->getEndDate().addDays(1);
				}
			}
				
			QDate InventoryData::isDeliveryWeekDay(QDate date)
			{
				if (date.dayOfWeek() == Qt::Saturday)
				{
					return date.addDays(2);
				}
				else if (date.dayOfWeek() == Qt::Sunday)
				{
					return date.addDays(1);
				}
				else
					return date;
			}
				
			QDate InventoryData::getVaildDeliveryDays(QDate date)
			{
				while (true)
				{
					QDate newNoHolidaysDay = this->isDeliveryHolidaysDay(date);
					QDate newNoWeekDay = this->isDeliveryWeekDay(date);
					if (newNoHolidaysDay == date && newNoWeekDay == date)
					{
						break;
					}
					else if (newNoHolidaysDay != date && newNoWeekDay == date)
					{
						date = newNoHolidaysDay;
					}
					else
						date = newNoWeekDay;
				}
				return date;
			}
				
			QDate InventoryData::getBeginDateTime(int deliveryTimeBeginDay, QDate now)
			{
				QDate newDate = now;
				if (deliveryTimeBeginDay == 0)
					newDate = this->getVaildDeliveryDays(now);
				else
				{
					for (int i = 1; i <= deliveryTimeBeginDay; i++)
					{
						newDate = this->getVaildDeliveryDays(newDate.addDays(1));
					}
				}
				return newDate;
			}


			//////---------------------public method--------------------------///////
			void InventoryData::addOrder(QSharedPointer<TradingOrder>& tradingOrder, QSharedPointer<OrderComputingResult> orderComputingResult)
			{
				if(!this->_inventoryItemDatas.contains(tradingOrder->getId()))
				{
					QSharedPointer<InventoryItemData> inventoryItemData(new InventoryItemData);
					inventoryItemData->_tradingOrder = tradingOrder;
					inventoryItemData->_orderComputingResult = orderComputingResult;
                    this->_inventoryItemDatas.insert(tradingOrder->getId(), inventoryItemData);
				}
				else
				{
					this->_inventoryItemDatas[tradingOrder->getId()]->_tradingOrder = tradingOrder;
                    this->_inventoryItemDatas[tradingOrder->getId()]->_orderComputingResult = orderComputingResult;
				}
				this->_unit = tradingOrder->getTransaction()->getBusinessInstrument()->getUnit();
				this->_referencePrice = tradingOrder->getLivePrice().toQString();
			}

			void InventoryData::setButtonIsEnabled(QSharedPointer<Business::Model::Computing::OverridedQuotationComputingResult> overridedQuotationComputingResult)
			{
				if (this->_inventoryItemDatas.count() == 0)
					return;

				auto instrument = this->_inventoryItemDatas.first()->_tradingOrder->getTransaction()->getBusinessInstrument();
				auto account = this->_inventoryItemDatas.first()->_tradingOrder->getTransaction()->getBusinessAccount();


				bool priceIsNull = false;
				auto deliveryCharge = this->_settingManager->getDeliveryCharge(account->getId(), instrument->getId());
				if (!deliveryCharge.isNull())
				{
					if (deliveryCharge->getChargeBasis() == PhysicalChargeBasis::DayOpenPrice)
						priceIsNull = overridedQuotationComputingResult->open == "" ? true : false;
					else if (deliveryCharge->getChargeBasis() == PhysicalChargeBasis::MkTPrice)
						priceIsNull = (overridedQuotationComputingResult->ask.isNullPrice() || overridedQuotationComputingResult->bid.isNullPrice()) ? true : false;
					else
						priceIsNull = false;
				}

				bool canClose = true;  
				bool canSumClose = false; 
				QDecNumber unDeliveryLot = 0;
				foreach (auto item, this->_inventoryItemDatas)
				{
					item->_tradingOrder->setCanClose(false);
					item->_canDelivery = false;
					item->_canInstalment = false;
					item->_canPrepayment = false;

					canClose = true;

					if (!item->_tradingOrder->getTransaction()->getBusinessAccount()->getAllowedTrade()
						|| item->_tradingOrder->getTransaction()->getBusinessAccount()->getAccountType() == AccountType::Transit
						|| item->_tradingOrder->getTransaction()->getBusinessInstrument()->getExpired())
					{
						continue;
					}

					this->setCanPrepayment(item);

					if (overridedQuotationComputingResult->ask.isNullPrice() 
						|| overridedQuotationComputingResult->bid.isNullPrice())
					{
						item->_tradingOrder->setCanClose(false);
						item->_canDelivery = false;
						continue;
                    }
					unDeliveryLot += item->_tradingOrder->getLotBalance();
					if ((item->_tradingOrder->getInstalmentFrequence() == Frequence::Occasional || item->_canInstalment) && !item->_tradingOrder->getIsPayoff())
					{
						if (canClose)
							this->setCanClose(item);
                        item->_canDelivery = false;
					}
					else
					{
						item->_tradingOrder->setCanClose(canClose);						
						item->_canDelivery = (instrument->getPlaceSetting()->getAllowDelivery() && (item->_tradingOrder->getLotBalance() > 0.0)) && !priceIsNull;
					}
					if (item->_tradingOrder->getCanClose())
						canSumClose = true;
				}

				this->_canClose = canSumClose; 
				if (instrument->getPlaceSetting()->getAllowDelivery() && unDeliveryLot > 0.0 && !priceIsNull)
					this->_canDelivery = true;
				else
					this->_canDelivery = false;
			}

			void InventoryData::calculate()
			{
				if (this->_inventoryItemDatas.count() == 0)
					return;
                QDecNumber totalPrice = 0.0;
                QDecNumber totalLot = 0.0;
                QDecNumber totalQuantity = 0.0;
                QDecNumber quantityLock = 0.0;
                QDecNumber totalMarketValue = 0.0;
                QDecNumber noDeliveryQuantity = 0.0;
                this->_originalDeliveryTotalLot = 0.0;

				auto instrument = this->_inventoryItemDatas.first()->_tradingOrder->getTransaction()->getBusinessInstrument();
				auto account = this->_inventoryItemDatas.first()->_tradingOrder->getTransaction()->getBusinessAccount();

				int decimalValue = instrument->getPhysicalLotDecimal();
				foreach (auto item, this->_inventoryItemDatas)
				{
					if (item->_tradingOrder->getPhase() == Phase::Deleted) continue;
					totalPrice += item->_tradingOrder->getExecutePrice().toDecimal() * item->_tradingOrder->getLotBalance();
                    totalLot += item->_tradingOrder->getLotBalance();
                    QDecNumber quantity = item->_tradingOrder->getTransaction()->getContractSize() * item->_tradingOrder->getLotBalance() * instrument->getWeight();
					totalQuantity += quantity;
					quantityLock += item->_tradingOrder->getDeliveryQuantityLock();
					item->_tradingOrder->setQuantityFormat(Common::roundToString(quantity.toDouble(), decimalValue));
					if(!item->_orderComputingResult.isNull()) totalMarketValue += item->_orderComputingResult->marketValue;
					this->_referencePrice = item->_tradingOrder->getLivePrice().toQString();
                    if(!item->_canDelivery)
                    {
                        noDeliveryQuantity += quantity;
                    }
                    else
                    {
                        this->_originalDeliveryTotalLot += item->_tradingOrder->getLotBalance();
                    }
				}

				this->_averagePrice = Common::roundToString((totalPrice / totalLot).toDouble(), instrument->getCalculateSetting()->getDecimal());
                this->_totalQuantity = totalQuantity;
                this->_originalDeliveryTotalQuantity = totalQuantity - noDeliveryQuantity;
                this->_availableQuantity = totalQuantity - noDeliveryQuantity;
                this->_totalQuantityStr = Common::roundToString(totalQuantity.toDouble(), decimalValue);
                this->_quantityLock = quantityLock;

				if (account->getIsMultiCurrency())
				{
					QUuid sourceCurrencyId = instrument->getCurrencyId();
					QSharedPointer<Model::Setting::Currency> currency = this->_settingManager->getCurrency(sourceCurrencyId);
					this->_marketValue = Common::roundToString(totalMarketValue.toDouble(), currency->getDecimals(), true);
					this->_currencyId = instrument->getCurrencyId();
					this->_chargeCurrency = currency->getCode();
				}
				else
				{
					QUuid sourceCurrencyId = instrument->getCurrencyId();
					QUuid targetCurrencyId = account->getCurrencyId();
					QSharedPointer<Model::Setting::CurrencyRate> currencyRate = this->_settingManager->getCurrencyRate(sourceCurrencyId, targetCurrencyId);
					if(!currencyRate.isNull())
					{
						this->_marketValue = Common::roundToString(currencyRate->exchangeByRateOut(totalMarketValue).toDouble(), currencyRate->getSourceCurrency()->getDecimals(), true);
						this->_currencyId = targetCurrencyId;						
						this->_chargeCurrency = currencyRate->getTargetCurrency()->getCode();
					}
					else
					{
						/*QSharedPointer<Model::Setting::Currency> currency = this->_settingManager->getCurrency(sourceCurrencyId);
						this->_marketValue = Common::roundToString(totalMarketValue.toDouble(), currency->getDecimals(), true);
						this->_currencyId = sourceCurrencyId;
						this->_chargeCurrency = currency->getCode();*/
					}
				}
			}
			
			bool InventoryData::isShowQuoteDescription()
			{
				return this->_businessInstrument->getQuoteDescription()!="";
			}

            QString InventoryData::getRequireLotString()
			{
                return Common::roundToString3(this->_requireLot.toDouble(),
					this->_businessInstrument->getPhysicalLotDecimal());
			}

			QString InventoryData::getRequireQuantityString()
			{
                return Common::roundToString3(this->_requireQuantity.toDouble(),
					this->_businessInstrument->getPhysicalLotDecimal());
			}

			QString InventoryData::getRequireQuantityUIString()
			{
				QString format = "%1 %2";
                return format.arg(Common::roundToString3(this->_requireQuantity.toDouble(),
					this->_businessInstrument->getPhysicalLotDecimal()),
					this->_businessInstrument->getUnit());
			}

            QString InventoryData::getAvailableQuantityString()
            {
                return Common::roundToString(this->_availableQuantity.toDouble(),
					this->_businessInstrument->getPhysicalLotDecimal());
            }

            QString InventoryData::getOriginalTotalQuantityString()
            {
                return Common::roundToString(this->_originalDeliveryTotalQuantity.toDouble(),
                    this->_businessInstrument->getPhysicalLotDecimal());
            }

			QString InventoryData::getDeliveryChargeString()
			{
                return Common::roundToString3(this->_deliveryCharge.toDouble(),
					this->_decimal);
			}

			bool InventoryData::reCalculateQuantity()
			{
				bool isCheckSuccess = true;
				QDecNumber requireLot = 0;
				QDecNumber requireQuantity = 0;
				int decimals = 0;
				foreach (auto item, this->_inventoryItemDatas)
				{
					if (item->_isSelect)
					{
						requireLot += item->_tradingOrder->getDeliveryLot();
						requireQuantity += item->_tradingOrder->getDeliveryLot() * 
							item->_tradingOrder->getTransaction()->getContractSize() * 
							this->_businessInstrument->getWeight();
						decimals = this->_businessInstrument->getPhysicalLotDecimal();
					}
				}
				if (!requireLot.isZero() && !requireQuantity.isZero())
				{
					this->_requireLot = Common::round(requireLot.toDouble(),decimals);
                    this->_requireQuantity = Common::round(requireQuantity.toDouble(), decimals);
					if (this->checkRequireLot(requireLot))
					{                    
						this->_isSelectDeliveryQuantity = true;
					}
					else
					{
						this->_isSelectDeliveryQuantity = false;
						isCheckSuccess = false;
					}
				}
				else
				{
					this->_requireLot = 0;
					this->_requireQuantity = 0;
					this->_isSelectDeliveryQuantity = false;
				}
				return isCheckSuccess;
			}

			void InventoryData::reCalculateCharge()
			{	
				auto deliveryCharge = this->_businessInstrument->getDeliveryCharge();
				if (deliveryCharge.isNull())
				{
					this->_deliveryCharge = 0;
					this->_deliveryChargeFormat = "";
					this->_isShowDeliveryCharge = false;
					return;
				}

				QDecNumber deliveryLot = 0;
				foreach (auto item, this->_inventoryItemDatas)
				{
					if (item->_isSelect)
					{
						deliveryLot += item->_tradingOrder->getDeliveryLot();
					}
				}

				if (!deliveryLot.isZero())
				{
                    qDebug() << " InventoryData::reCalculateCharge: deliveryLot" << deliveryLot.toDouble();
					auto quotation = this->_computingManager->getOverridedQuotationComputingResult(this->_businessAccount->getId(), this->_businessInstrument->getId());
					
					QDecNumber marketValueInDelivery;
					QString price;
					if (deliveryCharge->getChargeBasis() == PhysicalChargeBasis::MkTPrice)
					{
						if (this->_inventoryItemDatas.first()->_tradingOrder->getIsBuy())
							price = this->_businessInstrument->getIsNormal() ? quotation->bid.toQString() : quotation->ask.toQString();
						else
							price = this->_businessInstrument->getIsNormal() ? quotation->ask.toQString() : quotation->bid.toQString();
					}
					else if (deliveryCharge->getChargeBasis() == PhysicalChargeBasis::DayOpenPrice)
                    {
                        if (this->_inventoryItemDatas.first()->_tradingOrder->getIsBuy())
                            price = this->_businessInstrument->getIsNormal() ? quotation->bidOpen : quotation->askOpen;
                        else
                            price = this->_businessInstrument->getIsNormal() ? quotation->askOpen : quotation->bidOpen;
                    }
                    if (price == "" || price.toDouble() == 0.0)
					{
						marketValueInDelivery = 0;
					}
					else
					{
						int lotInteger = deliveryLot.toString().toInt();
						QDecNumber lotIntegerDecimal = QDecNumber(lotInteger);
						QDecNumber lotRemainder = deliveryLot - lotIntegerDecimal;
                        qDebug() << " InventoryData::reCalculateCharge: lotIntegerDecimal" << lotIntegerDecimal.toDouble()
                                 << " lotRemainder:" << lotRemainder.toDouble()
                                 << " lotInteger:" << lotInteger
                                 << " price:" << price
                                 << " ContractSize:" <<   this->_inventoryItemDatas.first()->_tradingOrder->getContractSize().toDouble()
                                 << " TradePLFormula:" <<  this->_businessInstrument->getCalculateSetting()->getTradePLFormula()
                                 << " DiscountOfOdd:" << this->_businessInstrument->getCalculateSetting()->getDiscountOfOdd()
                                  << " ChargeRate:" <<deliveryCharge->getChargeRate()
                                          ;
						marketValueInDelivery = BusinessHelper::getInstance()->calculateValue(
							this->_businessInstrument->getCalculateSetting()->getTradePLFormula(), 
							this->_inventoryItemDatas.first()->_tradingOrder->getContractSize(), lotIntegerDecimal, lotRemainder,
							QDecNumber::fromQString(price), 
                            this->_businessInstrument->getCalculateSetting()->getDiscountOfOdd());
					}

					if (deliveryCharge->getChargeBasis() != PhysicalChargeBasis::UnitFixAmount)
					{
						if (this->_businessAccount->getIsMultiCurrency())
						{
							QUuid sourceCurrencyId = this->_businessInstrument->getCurrencyId();
							QSharedPointer<Model::Setting::Currency> currency = this->_settingManager->getCurrency(sourceCurrencyId);
							this->_decimal = currency->getDecimals();
							this->_deliveryCharge = 
								Common::roundToDecimal(qMax((marketValueInDelivery * deliveryCharge->getChargeRate()), deliveryCharge->getMinCharge()),
                                this->_decimal);
						}
						else
						{
							QUuid sourceCurrencyId = this->_businessInstrument->getCurrencyId();
							QUuid targetCurrencyId = this->_businessAccount->getCurrencyId();
							QSharedPointer<Model::Setting::CurrencyRate> currencyRate = this->_settingManager->getCurrencyRate(sourceCurrencyId, targetCurrencyId);
							if(!currencyRate.isNull())
							{
								this->_deliveryCharge = currencyRate->exchange(qMax((marketValueInDelivery * deliveryCharge->getChargeRate()), deliveryCharge->getMinCharge()));
                                this->_decimal = currencyRate->getTargetCurrency()->getDecimals();
							}
							else
							{
								QSharedPointer<Model::Setting::Currency> currency = this->_settingManager->getCurrency(sourceCurrencyId);
								this->_decimal = currency->getDecimals();
								this->_deliveryCharge = 
									Common::roundToDecimal(qMax((marketValueInDelivery * deliveryCharge->getChargeRate()), deliveryCharge->getMinCharge()),
                                    this->_decimal);
							}
						}
					}
					else 
					{
						QSharedPointer<Model::Setting::Currency> currency = this->_settingManager->getCurrency(this->_businessAccount->getIsMultiCurrency() ? 
							this->_businessInstrument->getCurrencyId() :
						    this->_businessAccount->getCurrencyId());
						this->_decimal = currency->getDecimals();
						this->_deliveryCharge = Common::roundToDecimal(qMax(this->_requireLot * deliveryCharge->getChargeRate(), deliveryCharge->getMinCharge()), this->_decimal);
					}
					this->_deliveryChargeFormat="";
					if(this->_chargeCurrency != "")
					{
						this->_deliveryChargeFormat.append(this->_chargeCurrency);
						this->_deliveryChargeFormat.append(" ");
					}
					this->_deliveryChargeFormat.append(this->getDeliveryChargeString());
				}
				else
				{
					this->_deliveryCharge = 0;
					this->_deliveryChargeFormat = "";                 
				}
				if (this->_deliveryCharge.isZero())
					this->_isShowDeliveryCharge = false;
				else
					this->_isShowDeliveryCharge = true;
			}

			void InventoryData::setDeliveryCharge(QDecNumber value)
            {
				if(value.isZero())
				{
					this->_deliveryCharge = 0.0;
					this->_deliveryChargeFormat = "";
					this->_isShowDeliveryCharge = false;
				}
				else
				{
					this->_isShowDeliveryCharge = true;
					if (this->_businessAccount->getIsMultiCurrency())
					{
						QUuid sourceCurrencyId = this->_businessInstrument->getCurrencyId();
						QSharedPointer<Model::Setting::Currency> currency = this->_settingManager->getCurrency(sourceCurrencyId);
						this->_decimal = currency->getDecimals();
                        this->_deliveryCharge = Common::roundToDecimal(value, this->_decimal);
					}
					else
					{
						QUuid sourceCurrencyId = this->_businessInstrument->getCurrencyId();
						QUuid targetCurrencyId = this->_businessAccount->getCurrencyId();
						QSharedPointer<Model::Setting::CurrencyRate> currencyRate = this->_settingManager->getCurrencyRate(sourceCurrencyId, targetCurrencyId);
						if (!currencyRate.isNull())
						{
							this->_deliveryCharge = currencyRate->exchange(value);
							this->_decimal = currencyRate->getTargetCurrency()->getDecimals();
						}
						else
						{
							QSharedPointer<Model::Setting::Currency> currency = this->_settingManager->getCurrency(sourceCurrencyId);
							this->_decimal = currency->getDecimals();
							this->_deliveryCharge = Common::roundToDecimal(value, this->_decimal);
                        }
					}

					this->_deliveryChargeFormat = "";
					if (this->_chargeCurrency != "")
					{
						this->_deliveryChargeFormat.append(this->_chargeCurrency);
						this->_deliveryChargeFormat.append(" ");
					}
					this->_deliveryChargeFormat.append(this->getDeliveryChargeString());
				}
			}


			bool InventoryData::checkRequireLot(QDecNumber requireLot)
			{
				auto calculateSetting = this->getBusinessInstrument()->getCalculateSetting();
				qDebug() << " InventoryData::checkRequireLot requireLot:" << requireLot.toDouble();
				qDebug() << " InventoryData::checkRequireLot PhysicalMinDeliveryQuantity:" << calculateSetting->getPhysicalMinDeliveryQuantity().toDouble();
                qDebug() << " InventoryData::checkRequireLot PhysicalDeliveryIncremental:" << calculateSetting->getPhysicalDeliveryIncremental().toDouble();
				if (requireLot.isZero() || requireLot < calculateSetting->getPhysicalMinDeliveryQuantity())
				{
					qDebug() << " InventoryData::checkRequireLot requireLot Is Zero:" << requireLot.isZero();
					return false;
                }
                int decimal = Common::getDecimal(requireLot.toString());
                decimal = qMax(decimal, Common::getDecimal(calculateSetting->getPhysicalDeliveryIncremental().toString()));
                if(decimal == 0)
                    decimal = 1;
				double requireLot_d = requireLot.toDouble();
                bool verifyingValid1 = !calculateSetting->getPhysicalDeliveryIncremental().isZero();
                double deliveryIncremental = calculateSetting->getPhysicalDeliveryIncremental().toDouble();
                qDebug() << " InventoryData::checkRequireLot1 requireLot_d: " << requireLot_d;
                qDebug() << " InventoryData::checkRequireLot deliveryIncremental: " << deliveryIncremental;
                bool verifyingValid2 = deliveryIncremental!=0.0 ?
                            !Common::isMultiple(requireLot_d, deliveryIncremental) :
                            false;
                if (verifyingValid2 && verifyingValid2)
				{						
					qDebug() << " InventoryData::checkRequireLot verifyingValid1: " << verifyingValid1;
					qDebug() << " InventoryData::checkRequireLot verifyingValid2: " << verifyingValid2;
					return false;
				}
				return true;
			}

            void InventoryData::reAddOpenOrderStructs()
            {
                this->_openOrderStructs.clear();
                this->_originalDeliveryTotalLot = 0.0;
                foreach (auto item, this->_inventoryItemDatas)
                {
                    if (item->_tradingOrder->getPhase() == Phase::Deleted) continue;
                    if(item->_canDelivery)
                    {
                        item->_isSelect = false;
                        this->_originalDeliveryTotalLot += item->_tradingOrder->getLotBalance();
                        QSharedPointer<OpenOrderStruct> openOrderStructData(new OpenOrderStruct);
                        openOrderStructData->_OrderId = item->_tradingOrder->getId();
                        openOrderStructData->_ContractSize = item->_tradingOrder->getTransaction()->getContractSize();
                        openOrderStructData->_ExecuteTime = item->_tradingOrder->getExecuteTradeDay();
                        openOrderStructData->_Orderlot = item->_tradingOrder->getLotBalance();
                        openOrderStructData->_OrderQuantity = openOrderStructData->_Orderlot * openOrderStructData->_ContractSize * this->_businessInstrument->getWeight();;
                        openOrderStructData->_RequireLot = 0.0;
                        openOrderStructData->_RequireQuantity =  0.0;
                        openOrderStructData->_IsChecked = false;
                        this->_openOrderStructs.append(openOrderStructData);
                    }
                }
                this->setOpenOrderSort();
            }

            bool InventoryData::setAvailableQuantity(QDecNumber requireQuantity)
            {
				this->_availableQuantity = this->_originalDeliveryTotalQuantity - requireQuantity;
				this->_requireQuantity = requireQuantity;
				this->_requireLot = 0.0;
				qDebug() << " InventoryData::setAvailableQuantity requireQuantity:" << requireQuantity.toDouble();

				this->reAddOpenOrderStructs();
				for (int i = 0; i < this->_openOrderStructs.count(); i++)
				{
					if (requireQuantity == 0.0) break;
					if (requireQuantity >= this->_openOrderStructs[i]->_OrderQuantity)
					{
						this->_openOrderStructs[i]->_RequireQuantity = this->_openOrderStructs[i]->_OrderQuantity;
						this->_openOrderStructs[i]->_RequireLot = this->_openOrderStructs[i]->_Orderlot;
						requireQuantity = requireQuantity - this->_openOrderStructs[i]->_RequireQuantity;
						this->_requireLot += this->_openOrderStructs[i]->_RequireLot;
					}
					else
					{
						this->_openOrderStructs[i]->_RequireQuantity = requireQuantity;
						this->_openOrderStructs[i]->_RequireLot = requireQuantity / (this->_openOrderStructs[i]->_ContractSize * this->_businessInstrument->getWeight());
						requireQuantity = 0.0;
						this->_requireLot += this->_openOrderStructs[i]->_RequireLot;
					}
					this->_inventoryItemDatas[this->_openOrderStructs[i]->_OrderId]->_isSelect = true;
					this->_inventoryItemDatas[this->_openOrderStructs[i]->_OrderId]->_tradingOrder->setDeliveryLot(this->_openOrderStructs[i]->_RequireLot);
					qDebug() << " InventoryData::setAvailableQuantity setDeliveryLot:" << this->_openOrderStructs[i]->_RequireLot.toDouble()
						<< " this->_requireLot:" << this->_requireLot.toDouble();
					this->_openOrderStructs[i]->_IsChecked = true;
				}
                return true;
            }

            void InventoryData::setAvailableQuantityByDeliveryLot(QDecNumber deliveryLot)
            {                
//                auto lotSettingByBusiness = this->_businessInstrument->getLotSetting(true, OrderType::SpotTrade);
//                if(deliveryLot.toDouble() != 0.0)
//                {
//                    if(deliveryLot < lotSettingByBusiness->getMin())
//                        deliveryLot = lotSettingByBusiness->getMin();
//                    else
//                    {
//                        if(!(deliveryLot % lotSettingByBusiness->getStep()).isZero())
//                        {
//                            int multiple = (int)(deliveryLot.toDouble() / lotSettingByBusiness->getStep().toDouble());
//                            deliveryLot = (double)multiple * lotSettingByBusiness->getStep().toDouble();
//                        }
//                    }
//                }
                if(deliveryLot > this->_originalDeliveryTotalLot)
                    deliveryLot = this->_originalDeliveryTotalLot;

                this->reAddOpenOrderStructs();

                this->_requireLot = deliveryLot;
                this->_availableQuantity = 0.0;
                this->_requireQuantity = 0.0;
                for(int i=0; i < this->_openOrderStructs.count(); i++)
                {
                   if(deliveryLot == 0.0) break;
                   qDebug()<< " i:" << i << " deliveryLot:" << deliveryLot.toDouble();
                   if(deliveryLot >= this->_openOrderStructs[i]->_Orderlot)
                   {
                       this->_openOrderStructs[i]->_RequireQuantity = this->_openOrderStructs[i]->_OrderQuantity;
                       this->_openOrderStructs[i]->_RequireLot = this->_openOrderStructs[i]->_Orderlot;
                       deliveryLot = deliveryLot - this->_openOrderStructs[i]->_RequireLot;
                       this->_requireQuantity += this->_openOrderStructs[i]->_RequireQuantity;
                   }
                   else
                   {
                       this->_openOrderStructs[i]->_RequireLot = deliveryLot;
                       this->_openOrderStructs[i]->_RequireQuantity = deliveryLot * this->_openOrderStructs[i]->_ContractSize * this->_businessInstrument->getWeight();
                       deliveryLot = 0.0;
                       this->_requireQuantity += this->_openOrderStructs[i]->_RequireQuantity;
                   }
                   this->_inventoryItemDatas[this->_openOrderStructs[i]->_OrderId]->_isSelect = true;
				   this->_inventoryItemDatas[this->_openOrderStructs[i]->_OrderId]->_tradingOrder->setDeliveryLot(this->_openOrderStructs[i]->_RequireLot);
                   this->_openOrderStructs[i]->_IsChecked = true;
                }
                this->_availableQuantity = this->_originalDeliveryTotalQuantity - this->_requireQuantity;
            }

            QDecNumber InventoryData::getMaxAvailableQuantity(QDecNumber requireQuantityLock)
            {
                return this->_originalDeliveryTotalQuantity - requireQuantityLock;
            }

			QDate InventoryData::checkDeliveryTime(QDate date)
			{
				if (date <= this->_beginDeliveryTime)
				{
					return this->_beginDeliveryTime;
				}
				else
				{
					QDate newDay = this->getVaildDeliveryDays(date);
					int deliveryTimeEndDay = this->_businessInstrument->getDeliveryTimeEndDay();
					if (deliveryTimeEndDay == 0 || newDay < this->_endDeliveryTime)
					{
						return newDay;
					}
					else 
					{
						return this->_endDeliveryTime;
					}
				}
			}

			double InventoryData::getTotalLotBalance()
			{
				double totalLot = 0;
				foreach (auto item, this->_inventoryItemDatas.values())
				{
					totalLot += item->_tradingOrder->getLotBalance().toDouble();
				}
				return totalLot;
			}

			void InventoryData::updateItems()
			{
				QList<QUuid> orderIds;
				foreach (auto item, this->_inventoryItemDatas.values())
				{
					if(item->_tradingOrder->getLotBalance()<=0.0)
					{
						orderIds.append(item->_tradingOrder->getId());
					}
				}
				foreach (QUuid id, orderIds)
				{
                    this->_inventoryItemDatas.remove(id);
				}
				this->calculate();
			}

			bool InventoryData::isRemove()
			{
				bool isRemove = true;
				foreach (auto item, this->_inventoryItemDatas.values())
				{
					if(item->_tradingOrder->getLotBalance()>0.0)
					{
						isRemove = false;
					}
				}
				return isRemove;
			}

			bool InventoryData::existInstalmentOrder()
			{
				bool isExistInstalmentOrder = false;
				foreach (auto item, this->_inventoryItemDatas.values())
				{
					if(item->_tradingOrder->getPhysicalType() == Common::Enum::PhysicalType::Instalment)
					{
						isExistInstalmentOrder = true;
					}
				}
				return isExistInstalmentOrder;
			}
				
			bool InventoryData::existRepaymentOrder()
			{
				bool isExistRepaymentOrder = false;
				foreach (auto item, this->_inventoryItemDatas.values())
				{
					//if(item->_tradingOrder->getPhysicalType() == Common::Enum::PhysicalType::PrePayment)
					if(!item->_tradingOrder->getIsPayoff())
					{
						isExistRepaymentOrder = true;
					}
				}
				return isExistRepaymentOrder;
			}

			bool InventoryData::existInventoryItemData(QUuid id)
			{
				return this->_inventoryItemDatas.keys().contains(id);
			}

			void InventoryData::updateAccountInstrumentByUpdateCommand()
			{
				this->_businessAccount = this->_settingManager->getAccount(this->_businessAccount->getId());
				this->_businessInstrument = this->_businessAccount->getInstrument(this->_businessInstrument->getId());
			}


            void InventoryData::setOpenOrderSort()
            {
                fastSortForOpenOrder(this->_openOrderStructs, 0 , this->_openOrderStructs.count()-1 , Qt::AscendingOrder);
            }

            void InventoryData::fastSortForOpenOrder(QList<QSharedPointer<OpenOrderStruct>>& ptr, int begin, int end, Qt::SortOrder order)
            {
                QSharedPointer<OpenOrderStruct> temp = ptr[begin];//如按照Sequence排序，重复的就按字母排序，从0开始排序
                int i = begin + 1, j = end, curPosition = begin;
                bool direction = false;
                while(i <= j)
                {
                    if(direction)
                    {
                        bool compareResult = ptr.at(i)->_ExecuteTime < temp->_ExecuteTime;
                        if(order == Qt::AscendingOrder ? !compareResult : compareResult)  //如果当前数据小于基准数据 那么换位置 改当前位置
                        {
                            ptr.swap(curPosition,i);
                            curPosition = i;
                            direction = false;
                        }
                        i++;
                    }
                    else//先从后到前比较数据
                    {
                        bool compareResult = ptr.at(j)->_ExecuteTime < temp->_ExecuteTime;
                        if(order == Qt::AscendingOrder ? compareResult : !compareResult)//如果最后一个大于基准 那么最后一个数据赋值给当前基准数据的那个位置 调整基准数据的位置
                        {
                            ptr.swap(curPosition,j);
                            curPosition = j;
                            direction = true;
                        }
                        j--;
                    }
                }
                ptr.replace(curPosition, temp);

                if(curPosition - begin > 1)//前面小的比较
                    fastSortForOpenOrder(ptr, begin, curPosition - 1, order);
                if(end - curPosition > 1)//后面大的比较
                    fastSortForOpenOrder(ptr, curPosition + 1, end, order);
            }

		}
	}
}
