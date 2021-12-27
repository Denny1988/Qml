#include "OverridedQuotation.h"
#include <Math.h>
#include "Business/BusinessManager.h"
#include "Business/Model/Server/PricingPolicyDetail.h"
#include "Common/Util/Convert_Common.h"
#include "Common/LogHelper.h"
#include "Common/SignalForwarder.h"

namespace Business
{
	namespace Model
	{
		namespace Computing
		{
			QSharedPointer<DemandComputingResult> OverridedQuotation::doCompute()
			{
				return this->innerCompute();
			}
						
			QSharedPointer<OverridedQuotationComputingResult> OverridedQuotation::innerCompute()
			{
				//QLOG_INFO(Common::Constants::LogKeyConstants::Business) << "OverridedQuotation::innerCompute";
				QSharedPointer<OverridedQuotationComputingResult> overridedQuotationComputingResult(new OverridedQuotationComputingResult());
				
				QSharedPointer<Model::InitialQuotation> overridedQuotation =  BusinessManager::getInstance()->getQuotationManager()->getOverridedQuotation(this->_quotePolicyId, this->_instrumentId);
				if(overridedQuotation.isNull()) return overridedQuotationComputingResult;

				QString bidBase = overridedQuotation->getBid();
				QString askBase = overridedQuotation->getAsk();
				QString last = "%1/%2";
				double decimalDigits = log10((double)this->_denominator);

				overridedQuotationComputingResult->last = (bidBase == "" || askBase == "") ? "" : last.arg(bidBase, askBase);
				overridedQuotationComputingResult->timestamp = overridedQuotation->getTimestamp();
				overridedQuotationComputingResult->volume = overridedQuotation->getVolume();
				overridedQuotationComputingResult->totalVolume = overridedQuotation->getTotalVolume();
				int rearPrice = BusinessManager::getInstance()->getServerSourceManager()->getRearPrice(this->_instrumentId);
				overridedQuotationComputingResult->open = Common::getHighOrLowFormatString(overridedQuotation->getOpen(), overridedQuotation->getAskOpen(), rearPrice);
                overridedQuotationComputingResult->askOpen = overridedQuotation->getAskOpen();
                overridedQuotationComputingResult->bidOpen = overridedQuotation->getOpen();
				overridedQuotationComputingResult->high = Common::getHighOrLowFormatString(overridedQuotation->getHigh(), overridedQuotation->getAskHigh(), rearPrice);
				overridedQuotationComputingResult->low = Common::getHighOrLowFormatString(overridedQuotation->getLow(), overridedQuotation->getAskLow(), rearPrice);
				overridedQuotationComputingResult->priceTrend = overridedQuotation->getPriceTrend();
				overridedQuotation->setAsk(askBase);
				overridedQuotation->setBid(bidBase);
                int askNumeratorUnit = this->_numeratorUnit;
                int bidNumeratorUnit = this->_numeratorUnit;
                //qDebug() << " OverridedQuotation::innerCompute instrumentId:"<< this->_instrumentId << " numeratorUnit:" << _numeratorUnit;
                auto bidPriceDetail =  BusinessManager::getInstance()->getServerSourceManager()->getPricingPolicyDetail(this->_instrumentId, Common::roundToDecimal(bidBase.toDouble(), Common::getDecimal(bidBase)));
                if(!bidPriceDetail.isNull())
                    bidNumeratorUnit = bidPriceDetail->getNumerator();
                auto askPriceDetail =  BusinessManager::getInstance()->getServerSourceManager()->getPricingPolicyDetail(this->_instrumentId, Common::roundToDecimal(askBase.toDouble(), Common::getDecimal(askBase)));
                if(!askPriceDetail.isNull())
                    askNumeratorUnit = askPriceDetail->getNumerator();
                //qDebug() << " OverridedQuotation::innerCompute instrumentId:"<< this->_instrumentId <<" bidNumeratorUnit:"<< bidNumeratorUnit << " askNumeratorUnit:" << askNumeratorUnit;
                overridedQuotationComputingResult->bid = Common::Price::createInstance(bidBase, bidNumeratorUnit, this->_denominator);
                overridedQuotationComputingResult->ask = Common::Price::createInstance(askBase, askNumeratorUnit, this->_denominator);
//                qDebug() << " instrumentId" << this->_instrumentId << " bidBase:" << bidBase << " askBase:" << askBase << " decimalDigits:" << decimalDigits
//                         << " numeratorUnit:"<< this->_numeratorUnit << " denominator:" << this->_denominator
//                         << " ask:"<< overridedQuotationComputingResult->ask.toDouble()
//                         << " bid:"<< overridedQuotationComputingResult->bid.toDouble();

				if (overridedQuotation->getPrevAsk().isEmpty() && overridedQuotation->getPrevBid().isEmpty())
				{
					overridedQuotationComputingResult->prevClose = Common::roundToString(overridedQuotation->getPrevClose().toDouble(), (int)decimalDigits);
					if (!overridedQuotationComputingResult->bid.isNullPrice() && overridedQuotationComputingResult->prevClose != "")
					{
						double change = overridedQuotationComputingResult->bid.toQString().toDouble() - overridedQuotationComputingResult->prevClose.toDouble();
						if (this->_caculateChangeWithDenominator)
						{
							if ((int)change == change)
							{
								overridedQuotationComputingResult->netChange = QString::number((int)change);
							}
							else
							{
								overridedQuotationComputingResult->netChange = Common::roundToString(change, (int)decimalDigits);
							}
						}
						else if (overridedQuotationComputingResult->prevClose.toDouble() != 0)
						{
							double changePercentage = change * 100 / overridedQuotationComputingResult->prevClose.toDouble();
							overridedQuotationComputingResult->netChange.append(change >= 0 ? "+" : "");
							overridedQuotationComputingResult->netChange.append(Common::roundToString(change, (int)decimalDigits));
							overridedQuotationComputingResult->netChange.append("(");
							overridedQuotationComputingResult->netChange.append(Common::roundToString(changePercentage, 2));
							overridedQuotationComputingResult->netChange.append("%)");
						}
					}
					else
						overridedQuotationComputingResult->netChange = "-";
				}
				else
				{
					overridedQuotationComputingResult->prevClose = Common::getHighOrLowFormatString(overridedQuotation->getPrevBid(), overridedQuotation->getPrevAsk(), rearPrice);
					QString prevClose = Common::roundToString(overridedQuotation->getPrevBid().toDouble(), (int)decimalDigits);
					if (!overridedQuotationComputingResult->bid.isNullPrice() && prevClose != "")
					{
						double change = overridedQuotationComputingResult->bid.toQString().toDouble() - prevClose.toDouble();
						if (this->_caculateChangeWithDenominator)
						{
							if ((int)change == change)
							{
								overridedQuotationComputingResult->netChange = QString::number((int)change);
							}
							else
							{
								overridedQuotationComputingResult->netChange = Common::roundToString(change, (int)decimalDigits);
							}
						}
						else if (prevClose.toDouble() != 0.0)
						{
                            if(change!=0.0)
                            {
                                double changePercentage = change * 100 / prevClose.toDouble();
                                overridedQuotationComputingResult->netChange.append(change >= 0 ? "+" : "");
                                overridedQuotationComputingResult->netChange.append(Common::roundToString(change, (int)decimalDigits));
                                overridedQuotationComputingResult->netChange.append("(");
                                overridedQuotationComputingResult->netChange.append(Common::roundToString(changePercentage, 2));
                                overridedQuotationComputingResult->netChange.append("%)");
                            }
                            else
                            {
                                overridedQuotationComputingResult->netChange = "-";
                            }

						}
					}
					else
						overridedQuotationComputingResult->netChange = "-";
				}


				
				if(!overridedQuotationComputingResult->ask.isNullPrice() 
					&& !overridedQuotationComputingResult->bid.isNullPrice())
				{
					Common::SignalForwarder::getInstance()->sendQuotaionSignal(
						this->_quotePolicyId,
						this->_instrumentId, 
						overridedQuotationComputingResult->ask.toQString(), 
						overridedQuotationComputingResult->bid.toQString(), 
						overridedQuotationComputingResult->timestamp);
				}

				return overridedQuotationComputingResult;
			}
		}
	}
}
