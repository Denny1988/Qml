#include "ComputingInstrument.h"
#include <Math.h>
#include "Business/BusinessManager.h"
#include "Common/LogHelper.h"
#include "Common/UtcTimeManager.h"
#include <qDebug>

namespace Business
{
	namespace Model
	{
		namespace Computing
		{

			QSharedPointer<DemandComputingResult> ComputingInstrument::doCompute()
			{
				//QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "ComputingInstrument::doCompute";
                //qDebug() <<  "ComputingInstrument::doCompute start";
				QSharedPointer<InstrumentComputingResult> instrumentComputingResult(new InstrumentComputingResult());



				QDecNumber buyMarginSum = 0;
				QDecNumber sellMarginSum = 0;
				QDecNumber buySum = 0;
				QDecNumber sellSum = 0;
				QDecNumber physcialUnPaidBuyMarginSum = 0;
				QDecNumber physcialUnPaidBuySum = 0;
				QDecNumber physcialPaidPledgeSellMarginSum = 0;
				QDecNumber physcialPaidPledgeSellSum = 0;

				
				QMapIterator<QString, QSharedPointer<DemandComputing>> item(this->dependencies);
				while (item.hasNext())
				{
					item.next();
					QSharedPointer<OrderComputingResult> orderComputingResult = qSharedPointerCast<OrderComputingResult, DemandComputingResult>(this->dependencies[item.key()]->getResult());
					buyMarginSum += orderComputingResult->buyMarginSum;
					sellMarginSum += orderComputingResult->sellMarginSum;
					buySum += orderComputingResult->buySum;
					sellSum += orderComputingResult->sellSum;


					physcialUnPaidBuyMarginSum += orderComputingResult->physcialUnPaidBuyMarginSum;
					physcialUnPaidBuySum += orderComputingResult->physcialUnPaidBuySum;
					physcialPaidPledgeSellMarginSum += orderComputingResult->physcialPaidPledgeSellMarginSum;
					physcialPaidPledgeSellSum += orderComputingResult->physcialPaidPledgeSellSum;

					instrumentComputingResult->tradePLFloat += orderComputingResult->tradePLFloat;
					instrumentComputingResult->interestPLFloat += orderComputingResult->interestPLFloat;
					instrumentComputingResult->storagePLFloat += orderComputingResult->storagePLFloat;
					instrumentComputingResult->pledgeAmount += orderComputingResult->pledgeAmount;
					instrumentComputingResult->totalPaidAmount += orderComputingResult->totalPaidAmount;
					instrumentComputingResult->totalUnpaidAmount += orderComputingResult->totalUnpaidAmount;
					instrumentComputingResult->estimateCloseCommission += orderComputingResult->estimateCloseCommission;
					instrumentComputingResult->estimateCloseLevy += orderComputingResult->estimateCloseLevy;

                    //qDebug() <<"ComputingInstrument::doCompute itemKey:" << item.key();
                    //qDebug() << "ComputingInstrument::doCompute i_ estimateCloseCommission:" << instrumentComputingResult->estimateCloseCommission.toString();
                    //qDebug() << "ComputingInstrument::doCompute i_ estimateCloseLevy:" << instrumentComputingResult->estimateCloseLevy.toString();
					/*qDebug() << "iiicurrencyId:" << instrumentComputingResult->currencyId.toString();
					qDebug() << "iiiaaaInstrumentTraderPL:" << instrumentComputingResult->tradePLFloat.toString();
					qDebug() << "iiiaaaInstrumentinterestPLFloat:" << instrumentComputingResult->interestPLFloat.toString();*/
				}
				
				/*qDebug() << "buyMarginSum_Instrument:" << buyMarginSum.toString();
				qDebug() << "sellMarginSum_Instrument:" << sellMarginSum.toString();
				qDebug() << "buySum_Instrument:" << buySum.toString();
				qDebug() << "sellSum_Instrument:" << sellSum.toString();

				qDebug() << "physcialUnPaidBuyMarginSum_Instrument:" << physcialUnPaidBuyMarginSum.toString();
				qDebug() << "physcialUnPaidBuySum_Instrument:" << physcialUnPaidBuySum.toString();
				qDebug() << "physcialPaidPledgeSellMarginSum_Instrument:" << physcialPaidPledgeSellMarginSum.toString();
				qDebug() << "physcialPaidPledgeSellSum_Instrument:" << physcialPaidPledgeSellSum.toString();*/

				if(this->dependencies.size() > 0)
				{
					QSharedPointer<Model::Setting::BusinessInstrument> settingInstrument = Business::BusinessManager::getInstance()->getSettingManager()->getInstrument(this->_accountId, this->_instrumentId);				
					if(!settingInstrument.isNull())
					{
						instrumentComputingResult->necessary = this->calculateNecessary(settingInstrument, buyMarginSum, sellMarginSum, buySum, sellSum);
						instrumentComputingResult->partialPaymentPhysicalNecessary = this->calculatePartialPaymentPhysicalNecessary(settingInstrument, physcialUnPaidBuyMarginSum, physcialUnPaidBuySum, physcialPaidPledgeSellMarginSum, physcialPaidPledgeSellSum);
						instrumentComputingResult->currencyId = settingInstrument->getCurrencyId();
					}
				}
                //qDebug()<< "ComputingInstrument::doCompute _instrumentId: " << this->_instrumentId.toString() << "necessary_Instrument:" << instrumentComputingResult->necessary.toString();
				return instrumentComputingResult;
			}

			//-------------------------------------------------private method------------------------------------------------------------
			bool ComputingInstrument::shouldUseDayNecessary(QDateTime baseTime, bool useNightNecessaryWhenBreak)
			{
				auto instrumentStatus = Business::BusinessManager::getInstance()->getSettingManager()->getInstrumentStatus(this->_instrumentId);				
				if (instrumentStatus.contains(InstrumentStatus::DayClose) || instrumentStatus.contains(InstrumentStatus::DayCloseQuotationReceived) || instrumentStatus.contains(InstrumentStatus::AfterDayCloseBeforeDayOpen))
				{
					return false;
				}
				else if (instrumentStatus.contains(InstrumentStatus::SessionOpen) || instrumentStatus.contains(InstrumentStatus::DayOpen))
				{
					return true;
				}
				else
				{
					return (instrumentStatus.contains(InstrumentStatus::SessionClose)) && (!useNightNecessaryWhenBreak);
				}
			}



			QDecNumber ComputingInstrument::calculateNecessary(QSharedPointer<Model::Setting::BusinessInstrument> settingInstrument, QDecNumber buyMarginSum, QDecNumber sellMarginSum, QDecNumber buySum, QDecNumber sellSum)
			{
				//QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "ComputingInstrument::calculateNecessary";
				QSharedPointer<Model::Setting::CalculateSetting> calculateSetting = settingInstrument->getCalculateSetting();
				bool useNightNecessaryWhenBreak = calculateSetting->getUseNightNecessaryWhenBreak();            
				QDecNumber netNecessary = 0;
				QDecNumber hedgeNecessary = 0;
                bool shouldUseDayNecessary = this->shouldUseDayNecessary(Common::UtcTimeManager::getInstance()->getUtcNow(), useNightNecessaryWhenBreak);
                QDecNumber rateMarginD = shouldUseDayNecessary ? calculateSetting->getRateMarginD() : calculateSetting->getRateMarginO();
                QDecNumber marginD = shouldUseDayNecessary ? calculateSetting->getMarginD() : calculateSetting->getMarginO();
                QDecNumber rateMarginLockD = shouldUseDayNecessary ? calculateSetting->getRateMarginLockD() : calculateSetting->getRateMarginLockO();
                QDecNumber marginLockedD = shouldUseDayNecessary ? calculateSetting->getMarginLockedD() : calculateSetting->getMarginLockedO();


                if (calculateSetting->getMarginFormula() == 2  || calculateSetting->getMarginFormula() == 3
                    || calculateSetting->getMarginFormula() == 6  || calculateSetting->getMarginFormula() == 7
                    || calculateSetting->getMarginFormula() == 8  || calculateSetting->getMarginFormula() == 10)
                    //|| calculateSetting->getMarginFormula() == 9  || calculateSetting->getMarginFormula() == 11)
                {
					QDecNumber buyAvarage = buySum.isZero() ? 0 : buyMarginSum / buySum;
					QDecNumber sellAvarage = sellSum.isZero() ? 0 : sellMarginSum / sellSum;
					QDecNumber hedge = qMin(buySum, sellSum);
					QDecNumber netAvarage = buySum > sellSum ? buyAvarage : sellAvarage;
					QDecNumber net = (buySum - sellSum).abs();

                    netNecessary = rateMarginD * marginD * net * netAvarage;

                    QDecNumber avarage = (buyAvarage + sellAvarage);
                    if(calculateSetting->getMarginFormula() == 8)
                    {
                        avarage = qMax(buyAvarage, sellAvarage);
                    }
                    else if(calculateSetting->getMarginFormula() == 10)
                    {
                        avarage = qMin(buyAvarage, sellAvarage);
                    }
//                    else if(calculateSetting->getMarginFormula() == 9 || calculateSetting->getMarginFormula() == 11)
//                    {
//                        QSharedPointer<OverridedQuotationComputingResult> overridedQuotationComputingResult = Business::BusinessManager::getInstance()->getComputingManager()->getOverridedQuotationComputingResult(this->_accountId, this->_instrumentId);
//                        QDecNumber ask = Common::roundToDecimal(overridedQuotationComputingResult->ask.toDouble(), overridedQuotationComputingResult->ask._decimals);
//                        QDecNumber bid = Common::roundToDecimal(overridedQuotationComputingResult->bid.toDouble(), overridedQuotationComputingResult->bid._decimals);
//                        avarage = calculateSetting->getMarginFormula() == 9 ? qMax(ask, bid) : qMin(ask, bid);
//                    }
                    hedgeNecessary = rateMarginLockD * marginLockedD * hedge * avarage;

//                    qDebug() << "  code:" << settingInstrument->getCode();
//                    qDebug() << "  MarginFormula:" << calculateSetting->getMarginFormula();
//                    qDebug() << "  buyMarginSum:" << buyMarginSum.toString();
//                    qDebug() << "  buySum:" << buySum.toString();
//                    qDebug() << "  sellMarginSum:" << sellMarginSum.toString();
//                    qDebug() << "  sellSum:" << sellSum.toString();
//                    qDebug() << "  net:" << net.toString();
//                    qDebug() << "  netAvarage:" << netAvarage.toString();
//                    qDebug() << "  hedge:" << hedge.toString();
//                    qDebug() << "  buyAvarage:" << buyAvarage.toString();
//                    qDebug() << "  sellAvarage:" << sellAvarage.toString();
//                    qDebug() << "  netNecessary:" << netNecessary.toString();
//                    qDebug() << "  getMarginLockedD:" << calculateSetting->getRateMarginD().toString();
//                    qDebug() << "  getMarginLockedD:" << calculateSetting->getMarginD().toString();
//                    qDebug() << "  hedgeNecessary:" << hedgeNecessary.toString();
//                    qDebug() << "  getMarginLockedD:" << calculateSetting->getRateMarginLockD().toString();
//                    qDebug() << "  getMarginLockedD:" << calculateSetting->getMarginLockedD().toString();
//                    qDebug() << "  ----------------------------------------------------------------------";

				}
				else if (calculateSetting->getMarginFormula() == 0 
					|| calculateSetting->getMarginFormula() == 1)
				{					

                    if (calculateSetting->getMarginFormula() == 0 && !settingInstrument->VolumeNecessaryIsNull())
                        netNecessary = this->calculateVolumeNecessary(settingInstrument, rateMarginD, marginD, (buyMarginSum - sellMarginSum).abs(), shouldUseDayNecessary);
                    else
                        netNecessary = rateMarginD * marginD * (buyMarginSum - sellMarginSum).abs();
                    hedgeNecessary = rateMarginLockD * marginLockedD * qMin(buyMarginSum, sellMarginSum);
				}
				else
				{

					//error
					//write log
				}
				/*qDebug() << "RateMarginO_Instrument:" << calculateSetting->getRateMarginO().toString();
				qDebug() << "MarginO_Instrument:" << calculateSetting->getMarginO() .toString();
				qDebug() << "RateMarginLockO_Instrument:" << calculateSetting->getRateMarginLockO().toString();
				qDebug() << "MarginLockedO_Instrument:" << calculateSetting->getMarginLockedO() .toString();
				qDebug() << "netNecessary_Instrument:" << netNecessary.toString();
				qDebug() << "hedgeNecessary_Instrument:" << hedgeNecessary.toString();*/
				return netNecessary + hedgeNecessary;
			}

			bool ComputingInstrument::caseInsensitiveLessThan(QSharedPointer<Model::Setting::VolumeNecessaryDetail> v1, QSharedPointer<Model::Setting::VolumeNecessaryDetail> v2)
			{
				return v1->getFrom() < v2->getFrom();
			}

			QDecNumber ComputingInstrument::calculateVolumeNecessary(QSharedPointer<Model::Setting::BusinessInstrument> settingInstrument, QDecNumber marginRate, QDecNumber defaultMargin, QDecNumber netLot, bool useDayMargin)
			{
				/*qDebug() << "-------------------------------Code:" << settingInstrument->getCode();
				qDebug() << "marginRate:" << marginRate.toDouble();
				qDebug() << "defaultMargin:" << defaultMargin.toDouble();
				qDebug() << "netLot:" << netLot.toDouble();*/
				//QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "ComputingInstrument::calculateVolumeNecessary";
				if (settingInstrument->VolumeNecessaryIsNull()) return marginRate * defaultMargin * netLot;
				QList<QSharedPointer<Model::Setting::VolumeNecessaryDetail>> volumeNecessaryDetails = settingInstrument->getVolumeNecessary()->getVolumeNecessaryDetails();				
				if (volumeNecessaryDetails.length() == 0)
				{
					//qDebug() << "------------------length is 0";
					return marginRate * defaultMargin * netLot;
				}
                qSort(volumeNecessaryDetails.begin(), volumeNecessaryDetails.end(), caseInsensitiveLessThan);
				if (settingInstrument->getVolumeNecessary()->getOption() == Common::Enum::VolumeNecessaryOption::Flat)
				{
					//qDebug() << "------------------Flat";
					QDecNumber margin = defaultMargin;
					foreach (auto volumeNecessaryDetailItem, volumeNecessaryDetails)
					{
						if(netLot > volumeNecessaryDetailItem->getFrom())
							margin = (useDayMargin ? volumeNecessaryDetailItem->getMarginD() : volumeNecessaryDetailItem->getMarginO());
					}
					//qDebug() << "margin:" << margin.toDouble();
					return marginRate * margin * netLot;
				}
				else if (settingInstrument->getVolumeNecessary()->getOption() == Common::Enum::VolumeNecessaryOption::Progessive)
				{
					//qDebug() << "------------------Progessive";
					QDecNumber necessary = marginRate * defaultMargin * qMin(netLot, volumeNecessaryDetails[0]->getFrom());
					//qDebug() << "net_necessary:" << necessary.toDouble();
					int index = 0;
					while (index < volumeNecessaryDetails.length() && netLot > volumeNecessaryDetails[index]->getFrom())
					{
						QDecNumber margin = useDayMargin ? volumeNecessaryDetails[index]->getMarginD() : volumeNecessaryDetails[index]->getMarginO();
						QDecNumber lot = netLot - volumeNecessaryDetails[index]->getFrom();
						if (index < volumeNecessaryDetails.length() - 1)
						{
							lot = qMin(lot, (volumeNecessaryDetails[index + 1]->getFrom() - volumeNecessaryDetails[index]->getFrom()));
						}
						necessary += marginRate * margin * lot;
						//qDebug() << "necessary:" << necessary.toDouble() << " margin: " << margin.toDouble() << " Lot:" << lot.toDouble();
						index++;
					}
					return necessary;
				}
				else
				{
					//error
					//write log
					//throw new NotSupportedException(string.Format("Option = {0} is not supported", volumeNecessary.Option));
					return 0;
				}
				//qDebug() << "--------------------------------------------------------------";

			}

			QDecNumber ComputingInstrument::calculatePartialPaymentPhysicalNecessary(QSharedPointer<Model::Setting::BusinessInstrument> settingInstrument, QDecNumber physcialUnPaidBuyMarginSum, QDecNumber physcialUnPaidBuySum, QDecNumber physcialPaidPledgeSellMarginSum, QDecNumber physcialPaidPledgeSellSum)
			{
				//QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "ComputingInstrument::calculatePartialPaymentPhysicalNecessary";
				QDecNumber physcialNecessary = 0;
				if (settingInstrument->getInstrumentCategory() == Common::Enum::InstrumentCategory::Physical)
				{
					QDecNumber physcialMarginSum = 0;
					if (!physcialUnPaidBuySum.isNegative() && !physcialUnPaidBuySum.isZero())
					{
						physcialMarginSum = physcialUnPaidBuyMarginSum;
					}
					else if (!physcialPaidPledgeSellSum.isNegative() && !physcialPaidPledgeSellSum.isZero())
					{
						physcialMarginSum = physcialPaidPledgeSellMarginSum;
					}
					if (this->shouldUseDayNecessary(Common::UtcTimeManager::getInstance()->getUtcNow(), settingInstrument->getCalculateSetting()->getUseNightNecessaryWhenBreak()))
					{
						physcialNecessary = settingInstrument->getCalculateSetting()->getRateMarginD() * physcialMarginSum * settingInstrument->getCalculateSetting()->getPartPaidPhysicalNecessary();
					}
					else
					{
						physcialNecessary = settingInstrument->getCalculateSetting()->getRateMarginO() * physcialMarginSum * settingInstrument->getCalculateSetting()->getPartPaidPhysicalNecessary();
					}
				}
				return physcialNecessary;
			}


		}
	}
}
