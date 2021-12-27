#include "TradingOrder.h"
#include "Transaction.h"
#include "Business/BusinessHelper.h"
#include "Business/Model/Setting/BusinessInstrument.h"
#include "Common/Constants/DateTimeConstants.h"
#include "float.h"
#include "DateTimeHelper.h"
#include <QDebug>

using namespace Common::Constants;

namespace BillType
{
	enum BillType
	{
		None = 0,
		Commission = 1,
		Levy = 2,
		PaidPledge = 6,
		PayBackPledge = 7,
		OverdueCutPenalty = 8,
		ClosePenalty = 9,
		FrozenFund = 10,
		InstalmentAdministrationFee = 11,
		InterestPL = 21,
		StoragePL = 22,
		TradePL = 23,
		PhysicalPaidAmount = 28,
		PhysicalPaymentDiscount = 37,
		OtherFee = 40
	};
}

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			TradingOrder::TradingOrder()
			{
				this->clearData();

				this->_interestPerLot = 0.0;
				this->_storagePerLot = 0.0;
				this->_interestPLFloat = 0.0;
				this->_storagePLFloat = 0.0;
				this->_tradePLFloat = 0.0;
				this->_storagePLValued = 0.0;
				this->_physicalOriginValueBalance = 0.0;
				this->_physicalOriginValue = 0.0;
				this->_paidPledgeBalance = 0.0;
				this->_necessary = 0.0;
				this->_lotBalance = 0.0;
				this->_deliveryLockLot = 0.0;
				this->_interestPLNotValued = 0.0;
				this->_storagePLNotValued = 0.0;

				this->_executeTradeDay = DateTimeConstants::MinQDateTime;
				this->_canClose = true;
				this->_transactionError = "";
				this->_cancelReason = CancelReason::None;
				this->_lotBalanceDecimals = 0;
				this->_isNewAddOrder = true;
				this->_isPayoff = true;
				this->_BOBetTypeId = QUuid();
				this->_BOFrequency = -1;
				this->_hitCount = 0;
				this->_BOCountdown = 0;
                this->_BOHitResult = BOHitResult::Waiting;
                this->_officialFixingRequestId = QUuid();

                this->_downPayment = 0;
			}

			TradingOrder::~TradingOrder()
			{
			}

			void TradingOrder::clearData()
			{
				this->_commissionSum = DBL_MIN;
				this->_levySum = DBL_MIN;
				this->_paidPledge = DBL_MIN;
				this->_payBackPledge = DBL_MIN;
				this->_overdueCutPenalty = DBL_MIN;
				this->_closePenalty = DBL_MIN;
				this->_frozenFund = DBL_MIN;
				this->_instalmentAdministrationFee = DBL_MIN;
				this->_interestPL = DBL_MIN;
				this->_storagePL = DBL_MIN;
				this->_tradePL = DBL_MIN;
				this->_physicalPaidAmount = DBL_MIN;
				this->_physicalPaymentDiscount = DBL_MIN;
				this->_otherFee = DBL_MIN;
				this->_rebate = DBL_MIN;
			}

            bool TradingOrder::applyOfficialFixing()
            {
                return !this->_officialFixingRequestId.isNull();
            }

            bool TradingOrder::officialFixingEnableStatus()
            {
                bool isPlacePendingOrder = false;
                foreach(auto closeOrder, this->_closeOrders.values())
                {
                    if (closeOrder.isNull()) continue;
                    if (closeOrder->getPhase() == Phase::Placed || closeOrder->getPhase() == Phase::Placing)
                    {
                        if (this->getCategory(closeOrder->getTransaction()->getOrderType()) == OrderTypeCategory::Pending
                                || closeOrder->getTransaction()->getOrderType() == OrderType::MarketOnOpen
                                || closeOrder->getTransaction()->getOrderType() == OrderType::MarketOnClose)
                        {
                            isPlacePendingOrder = true;
                        }
                        else if(this->getCategory(closeOrder->getTransaction()->getOrderType()) == OrderTypeCategory::DQ)
                        {
                            isPlacePendingOrder = true;
                        }
                    }
                }
                return !applyOfficialFixing() && !isPlacePendingOrder;
            }

			void TradingOrder::setIsPayoff()
			{
				if (this->_transaction->getInstrumentCategory() == Common::Enum::InstrumentCategory::Forex || this->_physicalType == PhysicalType::FullPayment)
					this->_isPayoff = true;
				else
				{
					this->_isPayoff = (qAbs(this->_paidPledgeBalance.toDouble()) == this->_physicalOriginValueBalance.toDouble());
				}
			}

			void TradingOrder::update(QXmlStreamReader& xmlStreamReader)
			{
				this->_isNewAddOrder = false;
				QXmlStreamAttributes attributes = xmlStreamReader.attributes();
				this->_id = QUuid(attributes.value("ID").toString());
				this->_code = attributes.value("Code").toString();
				this->_lot = QDecNumber::fromQString(attributes.value("Lot").toString());
				this->_lotBalance = QDecNumber::fromQString(attributes.value("LotBalance").toString());
				this->_lotBalanceDecimals = Common::getValidDecimals(attributes.value("LotBalance").toString());
				if (attributes.hasAttribute("DeliveryLockLot"))
				{
					QString deliveryLockLot = attributes.value("DeliveryLockLot").toString();
					this->_deliveryLockLot = deliveryLockLot.contains("-") || deliveryLockLot == "0" ? 0.0 : QDecNumber::fromQString(deliveryLockLot);
				}
				this->_autoLimitPrice = attributes.value("AutoLimitPrice").toString();
				this->_autoStopPrice = attributes.value("AutoStopPrice").toString();
				if (attributes.hasAttribute("IsPayoff"))
					this->_isPayoff = Common::toBool(attributes.value("IsPayoff").toString());
				this->_isInstalmentOverdue = Common::toBool(attributes.value("IsInstalmentOverdue").toString());
				this->_minLot = QDecNumber::fromQString(attributes.value("MinLot").toString());
				this->_maxShow = QDecNumber::fromQString(attributes.value("MaxShow").toString());
				this->_isOpen = Common::toBool(attributes.value("IsOpen").toString());
				this->_isBuy = Common::toBool(attributes.value("IsBuy").toString());
				this->_setPrice = attributes.value("SetPrice").toString();
				this->_priceTimestamp = QDateTime::fromString(attributes.value("PriceTimestamp").toString(), DateTimeConstants::DateTimeFormat);
				this->_isQuotedPrice = Common::toBool(attributes.value("IsQuotedPrice").toString());
				this->_setPrice2 = attributes.value("SetPrice2").toString();
				this->_limitPriceForIfDone = attributes.value("LimitPriceForIfDone").toString();
				this->_stopPriceForIfDone = attributes.value("StopPriceForIfDone").toString();
				this->_tradeOption = static_cast<TradeOption::TradeOption>(attributes.value("TradeOption").toInt());
				this->_dQMaxMove = attributes.value("DQMaxMove").toInt();
				this->_executeTradeDay = QDateTime::fromString(attributes.value("ExecuteTradeDay").toString(), DateTimeConstants::DateTimeFormat);
				this->_physicalType = static_cast<PhysicalType::PhysicalType>(attributes.value("PhysicalType").toInt());
				this->_physicalTradeSide = static_cast<PhysicalTradeSide::PhysicalTradeSide>(attributes.value("PhysicalTradeSide").toInt());
                this->_physicalRequestId = QUuid(attributes.value("PhysicalRequestId").toString());
                this->_instalmentPolicyId = attributes.hasAttribute("InstalmentPolicyId") ? QUuid(attributes.value("InstalmentPolicyId").toString()) : QUuid();
                this->_physicalInstalmentType = attributes.hasAttribute("PhysicalInstalmentType") ?
                        static_cast<PhysicalInstalmentType::PhysicalInstalmentType>(attributes.value("PhysicalInstalmentType").toInt()) : PhysicalInstalmentType::None;
                this->_recalculateRateType = attributes.hasAttribute("RecalculateRateType") ?
                        static_cast<RecalculateRateType::RecalculateRateType>(attributes.value("RecalculateRateType").toInt()) : RecalculateRateType::None;
                this->_period = attributes.hasAttribute("Period") ? attributes.value("Period").toInt() : 0;
                this->_downPayment = attributes.hasAttribute("DownPayment") ? QDecNumber::fromQString(attributes.value("DownPayment").toString()) : 0.0;
                this->_instalmentFrequence = attributes.hasAttribute("Frequence") ? static_cast<Frequence::Frequence>(attributes.value("Frequence").toInt()): Frequence::Month;
                this->_downPaymentBasis = attributes.hasAttribute("DownPaymentBasis") ? attributes.value("DownPaymentBasis").toInt() : 0;
				this->_phase = static_cast<Phase::Phase>(attributes.value("Phase").toInt());
				if (attributes.hasAttribute("PhysicalOriginValue"))
					this->_physicalOriginValue = QDecNumber::fromQString(attributes.value("PhysicalOriginValue").toString());                
				if (attributes.hasAttribute("PhysicalOriginValueBalance"))
					this->_physicalOriginValueBalance = QDecNumber::fromQString(attributes.value("PhysicalOriginValueBalance").toString());
				if (attributes.hasAttribute("PaidPledgeBalance"))
					this->_paidPledgeBalance = QDecNumber::fromQString(attributes.value("PaidPledgeBalance").toString());

				if (attributes.hasAttribute("BOBetTypeID"))
					this->_BOBetTypeId = QUuid(attributes.value("BOBetTypeID").toString());
				if (attributes.hasAttribute("BOFrequency"))
					this->_BOFrequency = attributes.value("BOFrequency").toInt();
				if (attributes.hasAttribute("BOSettleTime"))
					this->_BOSettleTime = QDateTime::fromString(attributes.value("BOSettleTime").toString(), DateTimeConstants::DateTimeFormat);
				if (attributes.hasAttribute("BOBetOption"))
					this->_BOOption = attributes.value("BOBetOption").toInt();
				if (attributes.hasAttribute("BOOdds"))
					this->_BOOdds = attributes.value("BOOdds").toString();
				if (attributes.hasAttribute("BestPrice"))
					this->_hitPrice = attributes.value("BestPrice").toString();
				if (attributes.hasAttribute("BetResult"))
				{
					if (attributes.value("BetResult").toString() != "")
					{
						this->_BOHitResult = static_cast<BOHitResult::BOHitResult>(attributes.value("BetResult").toInt());
						this->_hitCount = 1;
					}
				}
				if (attributes.hasAttribute("HitCount"))
				{
					this->_hitCount = attributes.value("HitCount").toInt();
				}


				QString interestPerLot = attributes.value("InterestPerLot").toString();
				QString storagePerLot = attributes.value("StoragePerLot").toString();
				//qDebug() << "interestPerLot:" << interestPerLot;
				//qDebug() << "storagePerLot:" << storagePerLot;

				this->_interestPerLot = QDecNumber::fromQString(attributes.value("InterestPerLot").toString());
				this->_storagePerLot = QDecNumber::fromQString(attributes.value("StoragePerLot").toString());
				this->_interestPLFloat = QDecNumber::fromQString(attributes.value("InterestPLFloat").toString());
				this->_storagePLFloat = QDecNumber::fromQString(attributes.value("StoragePLFloat").toString());
				this->_tradePLFloat = QDecNumber::fromQString(attributes.value("TradePLFloat").toString());
				this->_necessary = QDecNumber::fromQString(attributes.value("Necessary").toString());
				if (attributes.value("CancelReason").toString() != "")
					this->_cancelReason = static_cast<CancelReason::CancelReason>(attributes.value("CancelReason").toInt());
				if (attributes.value("InterestPLNotValued").toString() != "")
				{
					this->_interestPLNotValued = QDecNumber::fromQString(attributes.value("InterestPLNotValued").toString());
				}
				if (attributes.value("StoragePLNotValued").toString() != "")
				{
					this->_storagePLNotValued = QDecNumber::fromQString(attributes.value("StoragePLNotValued").toString());
				}
				this->_unrealizedswap = this->_interestPLNotValued + this->_storagePLNotValued;

                if (attributes.value("OfficialFixingRequestID").toString() != "")
                {
                    this->_officialFixingRequestId = QUuid(attributes.value("OfficialFixingRequestID").toString());
                }


				while (!xmlStreamReader.atEnd()) {
					if (xmlStreamReader.isEndElement()) {//Order end
						break;
					}
					xmlStreamReader.readNext();
					if (xmlStreamReader.isStartElement()) {
						if (xmlStreamReader.name() == "Bills") {
							xmlStreamReader.readNext();
							readBillElement(xmlStreamReader);
						}
						else if (xmlStreamReader.name() == "OrderRelations") {
							xmlStreamReader.readNext();
							readOrderRelationElement(xmlStreamReader);
						}
                        else if (xmlStreamReader.name() == "OfficialFixingRequests")
                        {
                            xmlStreamReader.readNext();
                            readOfficialFixingRequestElement(xmlStreamReader);
                        }
						else if (xmlStreamReader.name() == "InstalmentDetails")
						{
							xmlStreamReader.readNext();
							while (!xmlStreamReader.atEnd()) {
								if (xmlStreamReader.isEndElement()) {
									xmlStreamReader.readNext();//InstalmentDetails end
									break;
								}
								if (xmlStreamReader.isStartElement()) {
									while (!xmlStreamReader.atEnd()) {
										if (xmlStreamReader.isEndElement()) {
											xmlStreamReader.readNext();//InstalmentDetail end
											break;
										}
										if (xmlStreamReader.name() == "InstalmentDetail") {
											QSharedPointer<OrderInstalmentData> orderInstalmentData(new OrderInstalmentData);
											QString isDeletedStr = xmlStreamReader.attributes().value("IsDeleted").toString();
											QString periodStr = xmlStreamReader.attributes().value("Period").toString();
											if (isDeletedStr == "True")
											{
												if (this->_orderInstalmentDatas.contains(periodStr))
													this->_orderInstalmentDatas.remove(periodStr);
											}
											else
											{
												orderInstalmentData->update(xmlStreamReader);
												this->_orderInstalmentDatas.insert(orderInstalmentData->getPeriod(), orderInstalmentData);
											}
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
					}
				}
			}

			void TradingOrder::readBillElement(QXmlStreamReader& xmlStreamReader)
			{
				QDecNumber  commissionSum = 0.0;
				QDecNumber  levySum = 0.0;
				QDecNumber  interestPL = 0.0;
				QDecNumber  storagePL = 0.0;
				QDecNumber  tradePL = 0.0;
				QDecNumber  paidPledge = 0.0;
				QDecNumber  payBackPledge = 0.0;
				QDecNumber  overdueCutPenalty = 0.0;
				QDecNumber  closePenalty = 0.0;
				QDecNumber  frozenFund = 0.0;
				QDecNumber  instalmentAdministrationFee = 0.0;
				QDecNumber  physicalPaidAmount = 0.0;
				QDecNumber  physicalPaymentDiscount = 0.0;
				QDecNumber  otherFee = 0.0;

				while (!xmlStreamReader.atEnd()) {
					if (xmlStreamReader.isEndElement()) {
						xmlStreamReader.readNext();//Bills end
						break;
					}
					if (xmlStreamReader.isStartElement()) {
						while (!xmlStreamReader.atEnd()) {
							if (xmlStreamReader.isEndElement()) {
								xmlStreamReader.readNext();//Bill end
								break;
							}
							if (xmlStreamReader.name() == "Bill") {
								BillType::BillType billType;
								QString typeStr = xmlStreamReader.attributes().value("Type").toString();
								QString billValue = xmlStreamReader.attributes().value("Value").toString();
								billType = static_cast<BillType::BillType>(typeStr.toInt());
								if (billValue.toDouble() == 0.0)
								{
									xmlStreamReader.readNext();
									continue;
								}
								switch (billType)
								{
								case BillType::Commission:
									commissionSum += QDecNumber::fromQString(billValue);
									break;
								case BillType::Levy:
									levySum += QDecNumber::fromQString(billValue);
									break;
								case BillType::PaidPledge:
									paidPledge += QDecNumber::fromQString(billValue);
									break;
								case BillType::PayBackPledge:
									payBackPledge += QDecNumber::fromQString(billValue);
									break;
								case BillType::OverdueCutPenalty:
									overdueCutPenalty += QDecNumber::fromQString(billValue);
									break;
								case BillType::ClosePenalty:
									closePenalty += QDecNumber::fromQString(billValue);
									break;
								case BillType::FrozenFund:
									frozenFund += QDecNumber::fromQString(billValue);
									break;
								case BillType::InstalmentAdministrationFee:
									instalmentAdministrationFee += QDecNumber::fromQString(billValue);
									break;
								case BillType::InterestPL:
									interestPL += QDecNumber::fromQString(billValue);
									break;
								case BillType::StoragePL:
									storagePL += QDecNumber::fromQString(billValue);
									break;
								case BillType::TradePL:
									tradePL += QDecNumber::fromQString(billValue);
									break;
								case BillType::PhysicalPaidAmount:
									physicalPaidAmount += QDecNumber::fromQString(billValue);
									break;
								case BillType::PhysicalPaymentDiscount:
									physicalPaymentDiscount += QDecNumber::fromQString(billValue);
									break;
								case BillType::OtherFee:
									otherFee += QDecNumber::fromQString(billValue);
									break;
								case BillType::None:
								default:
									break;
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

				if (commissionSum.toDouble() < 0)
                    this->_commissionSum = this->_commissionSum == DBL_MIN ? qAbs(commissionSum.toDouble()) : this->_commissionSum + QDecNumber(qAbs(commissionSum.toDouble()));
				else
                    this->_rebate = this->_rebate == DBL_MIN ? qAbs(commissionSum.toDouble()) : this->_rebate + QDecNumber(qAbs(commissionSum.toDouble()));
				levySum = -(levySum.toDouble());
                this->_levySum = this->_levySum == DBL_MIN ? levySum.toDouble() : this->_levySum + QDecNumber(levySum.toDouble());
				this->_interestPL = this->_interestPL == DBL_MIN ? interestPL : this->_interestPL + interestPL;
				this->_storagePL = this->_storagePL == DBL_MIN ? storagePL : this->_storagePL + storagePL;
				this->_tradePL = tradePL;
				this->_paidPledge = this->_paidPledge == DBL_MIN ? paidPledge : this->_paidPledge + paidPledge;
				this->_payBackPledge = this->_payBackPledge == DBL_MIN ? payBackPledge : this->_payBackPledge + payBackPledge;
				this->_overdueCutPenalty = this->_overdueCutPenalty == DBL_MIN ? overdueCutPenalty : this->_overdueCutPenalty + overdueCutPenalty;
				this->_closePenalty = this->_closePenalty == DBL_MIN ? closePenalty : this->_closePenalty + closePenalty;
				this->_frozenFund = this->_frozenFund == DBL_MIN ? frozenFund : this->_frozenFund + frozenFund;
				this->_instalmentAdministrationFee = this->_instalmentAdministrationFee == DBL_MIN ? instalmentAdministrationFee : this->_instalmentAdministrationFee + instalmentAdministrationFee;
				this->_physicalPaidAmount = this->_physicalPaidAmount == DBL_MIN ? physicalPaidAmount : this->_physicalPaidAmount + physicalPaidAmount;
				this->_physicalPaymentDiscount = this->_physicalPaymentDiscount == DBL_MIN ? physicalPaymentDiscount : this->_physicalPaymentDiscount + physicalPaymentDiscount;
				this->_otherFee = this->_otherFee == DBL_MIN ? otherFee : this->_otherFee + otherFee;
				this->_openOrderCommission = this->_commissionSum;
			}

			void TradingOrder::readOrderRelationElement(QXmlStreamReader& xmlStreamReader)
			{
				while (!xmlStreamReader.atEnd()) {
					if (xmlStreamReader.isEndElement()) {
						xmlStreamReader.readNext();//OrderRelations end
						break;
					}
					if (xmlStreamReader.isStartElement()) {
						while (!xmlStreamReader.atEnd()) {
							if (xmlStreamReader.isEndElement()) {
								xmlStreamReader.readNext();//OrderRelation end
								break;
							}
							if (xmlStreamReader.name() == "OrderRelation") {
                                QSharedPointer<Trading::OrderRelation> orderRelation(new Trading::OrderRelation);
                                orderRelation->update(xmlStreamReader);
                                QString key = QString("%1_%2").arg(orderRelation->getOrderId1().toString(), orderRelation->getOrderId2().toString());
                                if(this->_orderRelations.contains(key))
                                    this->_orderRelations[key]->update(orderRelation);
                                else
                                    this->_orderRelations.insert(key, orderRelation);
								xmlStreamReader.readNext();

								while (!xmlStreamReader.atEnd()) {
									if (xmlStreamReader.isEndElement()) {
										xmlStreamReader.readNext();//Bills end
										break;
									}
									if (xmlStreamReader.isStartElement()) {
										while (!xmlStreamReader.atEnd()) {
											if (xmlStreamReader.isEndElement()) {
												xmlStreamReader.readNext();//Bill end
												break;
											}
											xmlStreamReader.readNext();
											if (xmlStreamReader.isEndElement()) {
												break;//Bill end
											}
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
					}
					else {
						xmlStreamReader.readNext();
					}
				}
			}

            void TradingOrder::readOfficialFixingRequestElement(QXmlStreamReader& xmlStreamReader)
            {
                while (!xmlStreamReader.atEnd()) {
                    if (xmlStreamReader.isEndElement()) {
                        xmlStreamReader.readNext();//OfficialFixingRequests end
                        break;
                    }
                    if (xmlStreamReader.isStartElement()) {
                        while (!xmlStreamReader.atEnd()) {
                            if (xmlStreamReader.isEndElement()) {
                                xmlStreamReader.readNext();//OfficialFixingRequest end
                                break;
                            }
                            if (xmlStreamReader.name() == "OfficialFixingRequest") {
                               //<OfficialFixingRequests>
                               //<OfficialFixingRequest IsDeleted="False" ID="b151011b-c5aa-4c02-b778-7a73ba84b690" SettleDate="2018-07-25 00:00:00" Status="0" SubmitTime="2018-07-25 11:35:16" SubmitorID="c6aa29e3-3bfb-4d3e-88e0-19e79e59cada" UpdateTime="2018-07-25 11:35:16" UpdatePersonID="c6aa29e3-3bfb-4d3e-88e0-19e79e59cada" />
                               //</OfficialFixingRequests>
//								QSharedPointer<OrderRelation> orderRelation(new OrderRelation);
//								orderRelation->update(xmlStreamReader);
//								this->_orderRelations.append(orderRelation);
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

			void TradingOrder::update(const QDomNode& xmlTradingOrder)
			{
				bool isCalcuBOFloatPL = false;
				QDomNamedNodeMap map = xmlTradingOrder.attributes();
				for (int i = 0; i < map.count(); i++)
				{
					QDomNode qDomNode = map.item(i);
					QString name = qDomNode.nodeName();
					QString value = qDomNode.nodeValue();
					if (name == "ID")
						this->_id = QUuid(value);
					else if (name == "Code")
						this->_code = value;
					else if (name == "Lot")
					{
						this->_lot = QDecNumber::fromQString(value);
					}
					else if (name == "LotBalance")
					{
						this->_lotBalanceDecimals = Common::getValidDecimals(value);
						this->_lotBalance = QDecNumber::fromQString(value);
					}
					else if (name == "DeliveryLockLot")
					{
						this->_deliveryLockLot = value.contains("-") || value == "0" ? 0.0 : QDecNumber::fromQString(value);
					}
					else if (name == "AutoLimitPrice")
						this->_autoLimitPrice = value;
					else if (name == "AutoStopPrice")
						this->_autoStopPrice = value;
					else if (name == "IsPayoff")
						this->_isPayoff = Common::toBool(value);
					else if (name == "IsInstalmentOverdue")
						this->_isInstalmentOverdue = Common::toBool(value);
					else if (name == "MinLot")
						this->_minLot = QDecNumber::fromQString(value);
					else if (name == "MaxShow")
						this->_maxShow = QDecNumber::fromQString(value);
					else if (name == "IsOpen")
						this->_isOpen = Common::toBool(value);
					else if (name == "IsBuy")
						this->_isBuy = Common::toBool(value);
					else if (name == "SetPrice")
						this->_setPrice = value;
					else if (name == "PriceTimestamp")
						this->_priceTimestamp = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
					else if (name == "IsQuotedPrice")
						this->_isQuotedPrice = Common::toBool(value);
					else if (name == "SetPrice2")
						this->_setPrice2 = value;
					else if (name == "LimitPriceForIfDone")
						this->_limitPriceForIfDone = value;
					else if (name == "StopPriceForIfDone")
						this->_stopPriceForIfDone = value;
					else if (name == "TradeOption")
						this->_tradeOption = static_cast<TradeOption::TradeOption>(value.toInt());
					else if (name == "DQMaxMove")
						this->_dQMaxMove = value.toInt();
					else if (name == "ExecuteTradeDay")
						this->_executeTradeDay = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
					else if (name == "PhysicalType")
						this->_physicalType = static_cast<PhysicalType::PhysicalType>(value.toInt());
					else if (name == "PhysicalTradeSide")
						this->_physicalTradeSide = static_cast<PhysicalTradeSide::PhysicalTradeSide>(value.toInt());
					else if (name == "PhysicalRequestId")
						this->_physicalRequestId = QUuid(value);
					else if (name == "InstalmentPolicyId")
						this->_instalmentPolicyId = QUuid(value);
					else if (name == "PhysicalInstalmentType")
						this->_physicalInstalmentType = static_cast<PhysicalInstalmentType::PhysicalInstalmentType>(value.toInt());
					else if (name == "RecalculateRateType")
						this->_recalculateRateType = static_cast<RecalculateRateType::RecalculateRateType>(value.toInt());
					else if (name == "Period")
						this->_period = value.toInt();
					else if (name == "DownPayment")
						this->_downPayment = QDecNumber::fromQString(value);
					else if (name == "Frequence")
						this->_instalmentFrequence = static_cast<Frequence::Frequence>(value.toInt());
					else if (name == "DownPaymentBasis")
						this->_downPaymentBasis = value.toInt();
					else if (name == "Phase")
					{
						this->_phase = static_cast<Phase::Phase>(value.toInt());
						isCalcuBOFloatPL = true;
					}
					else if (name == "PhysicalOriginValue")
						this->_physicalOriginValue = QDecNumber::fromQString(value);
					else if (name == "PhysicalOriginValueBalance")
						this->_physicalOriginValueBalance = QDecNumber::fromQString(value);
					else if (name == "PaidPledgeBalance")
						this->_paidPledgeBalance = QDecNumber::fromQString(value);
					else if (name == "InterestPerLot")
						this->_interestPerLot = QDecNumber::fromQString(value);
					else if (name == "StoragePerLot")
						this->_storagePerLot = QDecNumber::fromQString(value);
					else if (name == "InterestPLFloat")
						this->_interestPLFloat = QDecNumber::fromQString(value);
					else if (name == "StoragePLFloat")
						this->_storagePLFloat = QDecNumber::fromQString(value);
					else if (name == "TradePLFloat")
						this->_tradePLFloat = QDecNumber::fromQString(value);
					else if (name == "Necessary")
						this->_necessary = QDecNumber::fromQString(value);
					else if (name == "CancelReason" && value != "")
						this->_cancelReason = static_cast<CancelReason::CancelReason>(value.toInt());
					else if (name == "BOBetTypeID")
						this->_BOBetTypeId = QUuid(value);
					else if (name == "BOFrequency")
						this->_BOFrequency = value.toInt();
					else if (name == "BOSettleTime")
						this->_BOSettleTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
					else if (name == "BOBetOption")
						this->_BOOption = value.toInt();
					else if (name == "BOOdds")
						this->_BOOdds = value;
					else if (name == "BestPrice")
						this->_hitPrice = value;
					else if (name == "HitCount")
					{
						this->_hitCount = value.toInt();
						isCalcuBOFloatPL = true;
					}
					else if (name == "BetResult" && value != "")
					{
						this->_BOHitResult = static_cast<BOHitResult::BOHitResult>(value.toInt());
						this->_hitCount = 1;
						isCalcuBOFloatPL = true;
					}
					else if (name == "InterestPLNotValued")
						this->_interestPLNotValued = QDecNumber::fromQString(value);
					else if (name == "StoragePLNotValued")
						this->_storagePLNotValued = QDecNumber::fromQString(value);
                    else if (name == "OfficialFixingRequestID")
                        this->_officialFixingRequestId = QUuid(value);
				}
				this->_unrealizedswap = this->_interestPLNotValued + this->_storagePLNotValued;

				if (isCalcuBOFloatPL)
					this->setBOFloatPLInString();
				QDomNodeList orderChildlist = xmlTradingOrder.childNodes();
				for (int j = 0; j < orderChildlist.count(); j++)
				{
					QDomNode orderItemQDomNode = orderChildlist.item(j);
					QString name = orderItemQDomNode.nodeName();
					if (name == "Bills")
					{
						QDecNumber  commissionSum = 0.0;
						QDecNumber  levySum = 0.0;
						QDecNumber  interestPL = 0.0;
						QDecNumber  storagePL = 0.0;
						QDecNumber  tradePL = 0.0;
						QDecNumber  paidPledge = 0.0;
						QDecNumber  payBackPledge = 0.0;
						QDecNumber  overdueCutPenalty = 0.0;
						QDecNumber  closePenalty = 0.0;
						QDecNumber  frozenFund = 0.0;
						QDecNumber  instalmentAdministrationFee = 0.0;
						QDecNumber  physicalPaidAmount = 0.0;
						QDecNumber  physicalPaymentDiscount = 0.0;
						QDecNumber  otherFee = 0.0;

						QDomNodeList billChildlist = orderItemQDomNode.childNodes();
						for (int k = 0; k < billChildlist.count(); k++)
						{
							QDomNode billItemQDomNode = billChildlist.item(k);
							BillType::BillType billType;
							QString typeStr = billItemQDomNode.toElement().attribute("Type");
							QString billValue = billItemQDomNode.toElement().attribute("Value");
							billType = static_cast<BillType::BillType>(typeStr.toInt());
							if (billValue.toDouble() == 0.0) continue;
							switch (billType)
							{
							case BillType::Commission:
								commissionSum += QDecNumber::fromQString(billValue);
								break;
							case BillType::Levy:
								levySum += QDecNumber::fromQString(billValue);
								break;
							case BillType::PaidPledge:
								paidPledge += QDecNumber::fromQString(billValue);
								break;
							case BillType::PayBackPledge:
								payBackPledge += QDecNumber::fromQString(billValue);
								break;
							case BillType::OverdueCutPenalty:
								overdueCutPenalty += QDecNumber::fromQString(billValue);
								break;
							case BillType::ClosePenalty:
								closePenalty += QDecNumber::fromQString(billValue);
								break;
							case BillType::FrozenFund:
								frozenFund += QDecNumber::fromQString(billValue);
								break;
							case BillType::InstalmentAdministrationFee:
								instalmentAdministrationFee += QDecNumber::fromQString(billValue);
								break;
							case BillType::InterestPL:
								interestPL += QDecNumber::fromQString(billValue);
								break;
							case BillType::StoragePL:
								storagePL += QDecNumber::fromQString(billValue);
								break;
							case BillType::TradePL:
								tradePL += QDecNumber::fromQString(billValue);
								break;
							case BillType::PhysicalPaidAmount:
								physicalPaidAmount += QDecNumber::fromQString(billValue);
								break;
							case BillType::PhysicalPaymentDiscount:
								physicalPaymentDiscount += QDecNumber::fromQString(billValue);
								break;
							case BillType::OtherFee:
								otherFee += QDecNumber::fromQString(billValue);
								break;
							case BillType::None:
							default:
								break;
							}
						}
						if (commissionSum.toDouble() < 0)
                            this->_commissionSum = this->_commissionSum == DBL_MIN ? qAbs(commissionSum.toDouble()) : this->_commissionSum + QDecNumber(qAbs(commissionSum.toDouble()));
						else
                            this->_rebate = this->_rebate == DBL_MIN ? qAbs(commissionSum.toDouble()) : this->_rebate + QDecNumber(qAbs(commissionSum.toDouble()));
						levySum = -(levySum.toDouble());
                        this->_levySum = this->_levySum == DBL_MIN ? levySum : this->_levySum + levySum;
						this->_interestPL = this->_interestPL == DBL_MIN ? interestPL : this->_interestPL + interestPL;
						this->_storagePL = this->_storagePL == DBL_MIN ? storagePL : this->_storagePL + storagePL;
						this->_tradePL = tradePL;
						this->_paidPledge = this->_paidPledge == DBL_MIN ? paidPledge : this->_paidPledge + paidPledge;
						this->_payBackPledge = this->_payBackPledge == DBL_MIN ? payBackPledge : this->_payBackPledge + payBackPledge;
						this->_overdueCutPenalty = this->_overdueCutPenalty == DBL_MIN ? overdueCutPenalty : this->_overdueCutPenalty + overdueCutPenalty;
						this->_closePenalty = this->_closePenalty == DBL_MIN ? closePenalty : this->_closePenalty + closePenalty;
						this->_frozenFund = this->_frozenFund == DBL_MIN ? frozenFund : this->_frozenFund + frozenFund;
						this->_instalmentAdministrationFee = this->_instalmentAdministrationFee == DBL_MIN ? instalmentAdministrationFee : this->_instalmentAdministrationFee + instalmentAdministrationFee;
						this->_physicalPaidAmount = this->_physicalPaidAmount == DBL_MIN ? physicalPaidAmount : this->_physicalPaidAmount + physicalPaidAmount;
						this->_physicalPaymentDiscount = this->_physicalPaymentDiscount == DBL_MIN ? physicalPaymentDiscount : this->_physicalPaymentDiscount + physicalPaymentDiscount;
						this->_otherFee = this->_otherFee == DBL_MIN ? otherFee : this->_otherFee + otherFee;
						this->_openOrderCommission = this->_commissionSum;
					}
					else if (name == "OrderRelations")
					{
						QDomNodeList orderRelationChildlist = orderItemQDomNode.childNodes();
						for (int k = 0; k < orderRelationChildlist.count(); k++)
						{
							QDomNode orderRelationItemQDomNode = orderRelationChildlist.item(k);
                            QSharedPointer<Trading::OrderRelation> orderRelation(new Trading::OrderRelation);
							orderRelation->update(orderRelationItemQDomNode);
                            QString key = QString("%1_%2").arg(orderRelation->getOrderId1().toString(), orderRelation->getOrderId2().toString());
                            if(this->_orderRelations.contains(key))
                                this->_orderRelations[key]->update(orderRelation);
                            else
                                this->_orderRelations.insert(key, orderRelation);
                            //this->_tradePL = this->_tradePL + orderRelation->getTradePL();
						}
					}
					else if (name == "InstalmentDetails")
					{
						QDomNodeList orderInstalmentDataChildlist = orderItemQDomNode.childNodes();
						for (int k = 0; k < orderInstalmentDataChildlist.count(); k++)
						{
							QDomNode itemQDomNode = orderInstalmentDataChildlist.item(k);
							QString periodStr = itemQDomNode.toElement().attribute("Period");
							QString isDeletedStr = itemQDomNode.toElement().attribute("IsDeleted");
							if (isDeletedStr == "True")
							{
								if (this->_orderInstalmentDatas.contains(periodStr))
									this->_orderInstalmentDatas.remove(periodStr);
							}
							else
							{
								if (this->_orderInstalmentDatas.contains(periodStr))
									this->_orderInstalmentDatas[periodStr]->update(itemQDomNode);
								else
								{
									QSharedPointer<OrderInstalmentData> orderInstalmentData(new OrderInstalmentData);
									orderInstalmentData->update(itemQDomNode);
									this->_orderInstalmentDatas.insert(periodStr, orderInstalmentData);
								}
							}
						}
					}

					if (this->_phase == Phase::Canceled)
					{
						this->_rebate = 0.0;
						this->_commissionSum = 0.0;
						this->_levySum = 0.0;
					}
				}
			}

			void TradingOrder::updatePlacingOrder(QSharedPointer<Common::Struct::Order> order, QSharedPointer<Common::Struct::Transaction> transaction)
			{
				this->_accountId = transaction->_accountId;
				this->_instrumentId = transaction->_instrumentId;
				this->_id = order->_id;
				this->_transactionId = transaction->_id;
				this->_lot = QDecNumber::fromQString(order->_lot);
				this->_lotBalance = this->_lot;
				this->_minLot = QDecNumber::fromQString(order->_minLot);
				this->_maxShow = QDecNumber::fromQString(order->_maxShow);
				this->_isOpen = order->_isOpen;
				this->_isBuy = order->_isBuy;
				this->_setPrice = order->_setPrice;
				this->_tradeOption = order->_tradeOption;
				this->_dQMaxMove = order->_DQMaxMove;
				this->_setPrice2 = order->_setPrice2;
				this->_limitPriceForIfDone = order->_limitPriceForIfDone;
				this->_stopPriceForIfDone = order->_stopPriceForIfDone;
				this->_physicalTradeSide = order->_physicalTradeSide;
				this->_instalmentPolicyId = order->_instalmentPolicyId;
				this->_physicalInstalmentType = order->_physicalInstalmentType;
				this->_recalculateRateType = order->_recalculateRateType;
				this->_period = order->_period;
				this->_downPayment = order->_downPayment;
				this->_instalmentFrequence = order->_instalmentFrequence;
				this->_downPaymentBasis = order->_downPaymentBasis;
				this->_phase = transaction->_phase;
				this->_instrumentCategory = transaction->_instrumentCategory;
				this->_BOBetTypeId = order->_BOBetTypeId;
				this->_BOFrequency = order->_BOFrequency;
				this->_BOSettleTime = QDateTime::fromString(order->_BOSettleTime, DateTimeConstants::DateTimeFormat);
				this->_BOOption = order->_BOOption;
				this->_BOOdds = order->_BOOdds;
			}

            void TradingOrder::setTransaction(QSharedPointer<Business::Model::Trading::Transaction> transaction)
			{
				_transaction = transaction;
                QString yyMMdd = transaction->getCode().mid(3, 6);
                _tradeDay = QDateTime(QDate((QString("20%1").arg(yyMMdd.left(2))).toInt(),
                                           yyMMdd.mid(2, 2).toInt(),
                                           yyMMdd.right(2).toInt()),
                                     QTime(0,0,0));
			}

            QSharedPointer<Business::Model::Trading::Transaction> TradingOrder::getTransaction()
			{
				return _transaction;
			}

			QDateTime TradingOrder::getExecuteTradeDay()
			{
				if (this->_executeTradeDay.isNull() || this->_executeTradeDay == DateTimeConstants::MinQDateTime)
				{
					return this->_transaction->getExecuteTime();
				}
				else
				{
					return this->_executeTradeDay;
				}
			}

			QDecNumber TradingOrder::getContractSize() const
			{
				return this->_transaction->getContractSize();
			}

			OrderTypeCategory::OrderTypeCategory TradingOrder::getCategory(OrderType::OrderType orderType)
			{
				if (orderType == OrderType::Market || orderType == OrderType::SpotTrade)
				{
					return OrderTypeCategory::DQ;
				}
				else
				{
					return OrderTypeCategory::Pending;
				}
			}

			QList<QSharedPointer<TradingOrder>> TradingOrder::getPendingCloseOrders()
			{
				QList<QSharedPointer<TradingOrder>> tradingOrders;
				foreach(auto closeOrder, _closeOrders)
				{
					if (closeOrder.isNull() || closeOrder->getPhase() == Phase::Canceled || closeOrder->getPhase() == Phase::Deleted) continue;
					if (getCategory(closeOrder->getTransaction()->getOrderType()) == OrderTypeCategory::Pending
						&& (closeOrder->getPhase() == Phase::Placing || closeOrder->getPhase() == Phase::Placed))
					{
						tradingOrders.append(closeOrder);
					}
				}
				return tradingOrders;
			}

			QDecNumber TradingOrder::getLotBalanceForDqOrderType()
			{
				return this->getLotBalance();
			}

			QDecNumber TradingOrder::getLotBalanceForMarketOnOpenType()
			{
				QDecNumber pendingClosedLot = 0;
				foreach(auto closeOrder, this->_closeOrders.values())
				{
					if (closeOrder.isNull()) continue;
					if (closeOrder->getPhase() == Phase::Placed || closeOrder->getPhase() == Phase::Placing)
					{
						if (closeOrder->getTransaction()->getOrderType() == OrderType::MarketOnOpen)
						{
                            pendingClosedLot += closeOrder->getCloseLot(this->_id);
						}
					}
				}
				return this->getLotBalance() - pendingClosedLot;
			}

			QDecNumber TradingOrder::getLotBalanceForMarketOnCloseType()
			{
				QDecNumber pendingClosedLot = 0;
				foreach(auto closeOrder, this->_closeOrders.values())
				{
					if (closeOrder.isNull()) continue;
					if (closeOrder->getPhase() == Phase::Placed || closeOrder->getPhase() == Phase::Placing)
					{
						if (closeOrder->getTransaction()->getOrderType() == OrderType::MarketOnClose)
						{
                            pendingClosedLot += closeOrder->getCloseLot(this->_id);
						}
					}
				}
				return this->getLotBalance() - pendingClosedLot;
			}

			QDecNumber TradingOrder::getLotBalanceForPendingOrderType()
			{
				QDecNumber pendingClosedLot = 0;
				QDecNumber ocopendingClosedLot = 0;
				foreach(auto closeOrder, this->_closeOrders.values())
				{
					if (closeOrder.isNull()) continue;
					if (closeOrder->getPhase() == Phase::Placed || closeOrder->getPhase() == Phase::Placing)
					{
						if (this->getCategory(closeOrder->getTransaction()->getOrderType()) == OrderTypeCategory::Pending)
						{
							if (closeOrder->getTransaction()->getType() == TransactionType::OneCancelOther)
							{
                                ocopendingClosedLot += closeOrder->getCloseLot(this->_id);
							}
							else
							{
                                pendingClosedLot += closeOrder->getCloseLot(this->_id);
							}
						}
					}
				}
				return this->getLotBalance() - pendingClosedLot - ocopendingClosedLot / 2;
			}

			QDecNumber TradingOrder::getLotBalanceForLimitOrderType()
			{
				QDecNumber pendingClosedLot = 0.0;
				QDecNumber ocopendingClosedLot = 0.0;
				foreach(auto closeOrder, this->_closeOrders.values())
				{
					if (closeOrder.isNull()) continue;
					if (closeOrder->getPhase() == Phase::Placed || closeOrder->getPhase() == Phase::Placing)
					{
						if (this->getCategory(closeOrder->getTransaction()->getOrderType()) == OrderTypeCategory::Pending)
						{
							if (closeOrder->getTransaction()->getType() == TransactionType::OneCancelOther)
							{
                                ocopendingClosedLot += closeOrder->getCloseLot(this->_id);
							}
							else
							{
								if (closeOrder->getTradeOption() == TradeOption::Stop) continue;
                                pendingClosedLot += closeOrder->getCloseLot(this->_id);
							}
						}
					}
				}
				return this->getLotBalance() - pendingClosedLot - ocopendingClosedLot / 2;
			}

			QDecNumber TradingOrder::getLotBalanceForStopOrderType()
			{
				QDecNumber pendingClosedLot = 0.0;
				QDecNumber ocopendingClosedLot = 0.0;
				foreach(auto closeOrder, this->_closeOrders.values())
				{
					if (closeOrder.isNull()) continue;
					if (closeOrder->getPhase() == Phase::Placed || closeOrder->getPhase() == Phase::Placing)
					{
						if (this->getCategory(closeOrder->getTransaction()->getOrderType()) == OrderTypeCategory::Pending)
						{
							if (closeOrder->getTransaction()->getType() == TransactionType::OneCancelOther)
							{
                                ocopendingClosedLot += closeOrder->getCloseLot(this->_id);
							}
							else
							{
								if (closeOrder->getTradeOption() == TradeOption::Better) continue;
                                pendingClosedLot += closeOrder->getCloseLot(this->_id);
							}
						}
					}
				}
				return this->getLotBalance() - pendingClosedLot - ocopendingClosedLot / 2;
			}

			QDecNumber TradingOrder::getLotBalance()
			{
				return this->_deliveryLockLot.isZero() ? this->_lotBalance : (this->_lotBalance - this->_deliveryLockLot);
			}

			void TradingOrder::addDeliveryLockLot(const QDecNumber& value)
			{
				this->_deliveryLockLot = this->_deliveryLockLot + value;
			}

			QString TradingOrder::getPaidPledgeBalanceInFormat()
			{
				if (this->_paidPledgeBalance.isZero() || (this->_physicalOriginValueBalance.toDouble() == qAbs(this->_paidPledgeBalance.toDouble()) && _isBuy))
					return "";
				else
					return Common::roundToString(qAbs(this->_paidPledgeBalance.toDouble()),
						Business::BusinessHelper::getInstance()->getDecimals(
							this->_transaction->getBusinessAccount(), this->_transaction->getBusinessInstrument()), true);
			}

			QString TradingOrder::getPhysicalPaymentDiscountInFormat()
			{
				return Common::roundToString(this->_physicalPaymentDiscount.toDouble(),
					Business::BusinessHelper::getInstance()->getDecimals(
						this->_transaction->getBusinessAccount(), this->_transaction->getBusinessInstrument()), true);
			}

			QString TradingOrder::getLotInFormat()
			{
				if (this->_lot.isZero())
					return "";
				else
					return this->_lot.toString();
					/*return Common::roundToString(this->_lot.toDouble(),
						Business::BusinessHelper::getInstance()->getDecimals(
							this->_transaction->getBusinessAccount(), this->_transaction->getBusinessInstrument()));*/
			}

			QString TradingOrder::getLotBalanceInFormat()
			{
				if (this->getLotBalance().isZero())
					return "";
				else
					return Common::roundToString(this->getLotBalance().toDouble(),
						Business::BusinessHelper::getInstance()->getDecimals(
							this->_transaction->getBusinessAccount(), this->_transaction->getBusinessInstrument()));
			}

			void TradingOrder::setDeliveryLot(QDecNumber value)
			{
				this->_deliveryLot = value;
				this->_deliveryQuantity = value *
					this->_transaction->getContractSize() *
					this->_transaction->getBusinessInstrument()->getWeight();
			}

			void TradingOrder::calculateDeliveryLot()
			{
				this->_deliveryLot = this->getLotBalance();
				this->_deliveryQuantity = this->_deliveryLot *
					this->_transaction->getContractSize() *
					this->_transaction->getBusinessInstrument()->getWeight();
			}

			QString TradingOrder::getDeliveryLotString()
			{
				return Common::roundToString(this->_deliveryLot.toDouble(),
					this->_transaction->getBusinessInstrument()->getPhysicalLotDecimal());
			}

			QString TradingOrder::getDeliveryQuantityString()
			{
				return Common::roundToString(this->_deliveryQuantity.toDouble(),
					this->_transaction->getBusinessInstrument()->getPhysicalLotDecimal());
			}

			bool TradingOrder::getAllowTradeLimitStopOrder()
			{
				return this->_transaction->getBusinessInstrument()->getPlaceSetting()->getPlaceSettingDetail(OrderType::Limit);
			}

			bool TradingOrder::getAllowTradeSpotTradeOrder()
			{
				return this->_transaction->getBusinessInstrument()->getPlaceSetting()->getPlaceSettingDetail(OrderType::Limit);
			}

			QString TradingOrder::getCommissionSumInFormat()
			{
				return Common::roundToString(this->_commissionSum.toDouble(),
					this->getDecimalPalcesForLot(), true);
			}

			QString TradingOrder::getRebateInFormat()
			{
				return Common::roundToString(this->_rebate.toDouble(),
					this->getDecimalPalcesForLot(), true);
			}

			QString TradingOrder::getLevySumInFormat()
			{
				return Common::roundToString(this->_levySum.toDouble(),
					this->getDecimalPalcesForLot(), true);
			}

			QString TradingOrder::getOpenOrderCommissionInFormat()
			{
				return Common::roundToString(this->_openOrderCommission.toDouble(),
					this->getDecimalPalcesForLot(), true);
			}

			QString TradingOrder::getOpenOrderLevyInFormat()
			{
				return Common::roundToString(this->_openOrderLevy.toDouble(),
					this->getDecimalPalcesForLot(), true);
			}

			QString TradingOrder::getTradePLInFormat()
			{
				if (!this->_BOBetTypeId.isNull())
				{
					bool isCompleted = (this->_hitCount == 1 || this->_BOHitResult == BOHitResult::Draw || this->_BOHitResult == BOHitResult::Lose);
					if (isCompleted)
					{
						QDecNumber pl = 0;
						if (this->_BOHitResult == BOHitResult::Lose)
						{
							pl = 0;
						}
						else if (this->_BOHitResult == BOHitResult::Draw)
						{
							pl = this->_lot;
						}
						else
						{
							pl = this->_tradePL + this->_lot;
						}

						if (!Business::BusinessHelper::getInstance()->getBOIncludePrincipal())
						{
							pl -= this->_lot;
						}
						return Common::roundToString(pl.toDouble(), this->getDecimalPalcesForLot(), true);
					}
					else
					{
						return "";
					}
				}
				else
					return Common::roundToString(this->_tradePL.toDouble(), this->getDecimalPalcesForLot(), true);
			}

			QString TradingOrder::getTradePLFloatInFormat()
			{
				/*if(this->_tradePLFloat.isZero())
				{
				return "";
				}
				else
				{
				return Common::roundToString(this->_tradePLFloat.toDouble(),
				this->getDecimalPalcesForLot(), true);
				}*/
				return this->_tradePLFloatInFormat;
			}

            QString TradingOrder::getTradePLFloatForPendingInFormat()
            {
                /*if(this->_tradePLFloat.isZero())
                {
                return "";
                }
                else
                {
                return Common::roundToString(this->_tradePLFloat.toDouble(),
                this->getDecimalPalcesForLot(), true);
                }*/
                return this->_tradePLFloatForPendingInFormat;
            }

			void TradingOrder::setTradePLFloat(QDecNumber value)
			{
				this->_tradePLFloat = value;
				/*if (this->_tradePLFloat.isZero())
				{
					this->_tradePLFloatInFormat = "";
				}
				else
				{*/
					this->_tradePLFloatInFormat = Common::roundToString(this->_tradePLFloat.toDouble(),
						this->getDecimalPalcesForLot(), true);
				//}
			}

            void TradingOrder::setTradePLFloatForPending(QDecNumber value)
            {
                this->_tradePLFloatForPending = value;
                /*if (this->_tradePLFloat.isZero())
                {
                    this->_tradePLFloatInFormat = "";
                }
                else
                {*/
                    this->_tradePLFloatForPendingInFormat = Common::roundToString(this->_tradePLFloatForPending.toDouble(),
                        this->getDecimalPalcesForLot(), true);
                //}
            }

			QString TradingOrder::getInterestPLFloatInFormat()
			{
				return Common::roundToString(this->_interestPLFloat.toDouble(),
					this->getDecimalPalcesForLot(), true);
			}

			QString TradingOrder::getStoragePLFloatInFormat()
			{
				return Common::roundToString(this->_storagePLFloat.toDouble(),
					this->getDecimalPalcesForLot(), true);
			}

			QString TradingOrder::getUnrealizedswapInFormat()
			{
				return Common::roundToString(this->_unrealizedswap.toDouble(),
					this->getDecimalPalcesForLot(), true);
			}

			QString TradingOrder::getCurrencyRateInFormat()
			{
				return Business::BusinessHelper::getInstance()->getCurrencyRate(
					this->_transaction->getBusinessAccount(),
					this->_transaction->getBusinessInstrument(),
					this->_tradePL);
			}

			QString TradingOrder::getInterestRateInFormat()
			{
				auto interestRateSetting = this->_transaction->getBusinessInstrument()->getInterestRateSetting();
				if (interestRateSetting.isNull())
				{
					return "";
				}
				else
				{
					QDecNumber interestRate = this->_isBuy ? interestRateSetting->getBuy() : interestRateSetting->getSell();
					return Common::roundToString(interestRate.toDouble(), 4);
				}
			}

			QString TradingOrder::getCloseLotInFormat(QUuid openOrderId)
			{
				double lot = 0.0;
                foreach(auto orderRelation, this->_orderRelations.values())
				{
					if (orderRelation->getOrderId1() == openOrderId)
					{
						lot = orderRelation->getLot().toDouble();
					}
				}
				if (lot == 0.0)
					return "";
				else
					return Common::roundToString(lot,
						Business::BusinessHelper::getInstance()->getDecimals(
							this->_transaction->getBusinessAccount(), this->_transaction->getBusinessInstrument()));
			}

			QString TradingOrder::getOpenOrderInfo()
			{
				QString openOrderInfo;
				if (!this->_isOpen)
				{
                    foreach(auto orderRelation, this->_orderRelations.values())
					{
						if (orderRelation->getOrderId2() == this->_id)
						{
							if (openOrderInfo.length() > 0) openOrderInfo.append(";");
							openOrderInfo.append(orderRelation->getOpenOrderInfo());
						}
					}
				}
				return openOrderInfo;
			}

			QDecNumber TradingOrder::getCloseLot(QUuid openOrderId)
			{
				QDecNumber closeLot;
                foreach(auto orderRelation, this->_orderRelations.values())
				{
					if (orderRelation->getOrderId1() == openOrderId)
					{
						closeLot = closeLot + orderRelation->getLot();
					}
				}
				return closeLot;
			}

			int TradingOrder::getDecimalPalcesForLot()
			{
				return Business::BusinessHelper::getInstance()->getDecimals(
					this->_transaction->getBusinessAccount(), this->_transaction->getBusinessInstrument());
			}
						
			bool TradingOrder::canClear()
			{
				return (this->_phase == Phase::Canceled || this->_phase == Phase::Deleted ||
					this->_phase == Phase::Executed || this->_phase == Phase::Completed)
					&& (this->_replaceOrder.isNull() || this->_replaceOrder->canClear());
			}

			void TradingOrder::calculate()
			{
				Business::BusinessHelper::getInstance()->calculatePLNotValued(
					this->_transaction->getBusinessAccount(),
					this->_transaction->getBusinessInstrument(),
					this->_interestPLNotValueds,
					this->_storagePLNotValueds);
			}

			void TradingOrder::addCloseOrder(QSharedPointer<TradingOrder> closeOrder)
			{
				if (this->_closeOrders.contains(closeOrder->getId()))
					this->_closeOrders.remove(closeOrder->getId());
				this->_closeOrders.insert(closeOrder->getId(), closeOrder);
			}

			void TradingOrder::removeCloseOrder(QUuid orderId)
			{
				this->_closeOrders.remove(orderId);
			}

			QList<QSharedPointer<TradingOrder>> TradingOrder::getCloseOrders()
			{
				QList<QSharedPointer<TradingOrder>> closeOrders;
				QList<QUuid> removeAccountIds;
				QMapIterator<QUuid, QSharedPointer<TradingOrder>> closeOrderItem(this->_closeOrders);
				while (closeOrderItem.hasNext())
				{
					closeOrderItem.next();
					auto closeOrder = closeOrderItem.value();
					if (closeOrder.isNull())
					{
						removeAccountIds.append(closeOrderItem.key());
						continue;
					}
					closeOrders.append(closeOrderItem.value());
				}
				foreach(QUuid id, removeAccountIds)
				{
					this->_closeOrders.remove(id);
				}
				return closeOrders;
			}


			void TradingOrder::caculateLimitSummary()
			{
				QDecNumber totalLimitLot = 0;
				QDecNumber totalStopLot = 0;
				QDecNumber totalLimitPrice = 0;
				QDecNumber totalStopPrice = 0;

				auto settingsInstrument = this->_transaction->getBusinessInstrument();

				QList<QUuid> removeAccountIds;
				QMapIterator<QUuid, QSharedPointer<TradingOrder>> closeOrderItem(this->_closeOrders);
				while (closeOrderItem.hasNext())
				{
					closeOrderItem.next();
					auto closeOrder = closeOrderItem.value();
					if (closeOrder.isNull())
					{
						removeAccountIds.append(closeOrderItem.key());
						continue;
					}
					if (closeOrder->getTransaction()->getOrderType() == OrderType::Limit)
					{
						if (closeOrder->getTradeOption() == TradeOption::Better)
						{
							QDecNumber lot = closeOrder->getCloseLot(this->_id);
                            //double aa = lot.toDouble();
							totalLimitLot = totalLimitLot + lot;
                            totalLimitPrice = totalLimitPrice + QDecNumber(closeOrder->getSetPrice().toDouble() * lot.toDouble());
						}
						else if (closeOrder->getTradeOption() == TradeOption::Stop)
						{
							QDecNumber lot = closeOrder->getCloseLot(this->_id);
                            //double aa = lot.toDouble();
							totalStopLot = totalStopLot + lot;
                            totalStopPrice = totalStopPrice + QDecNumber(closeOrder->getSetPrice().toDouble() * lot.toDouble());
						}
					}
				}
				foreach(QUuid id, removeAccountIds)
				{
					this->_closeOrders.remove(id);
				}

				int decimals = this->_executePrice._decimals;

				this->_limitCloseOrderSumarry = "";
				this->_avarageLimitClosePrice = "";
				if (!totalLimitLot.isZero())
				{
					Common::Price avarageLimitClosePrice = Common::Price::createInstance((totalLimitPrice / totalLimitLot).toDouble(),
						settingsInstrument->getCalculateSetting()->getNumeratorUnit(),
						settingsInstrument->getCalculateSetting()->getDenominator(), decimals);
					this->_limitCloseOrderSumarry.append(Common::roundToString(totalLimitLot.toDouble(), decimals));
					this->_limitCloseOrderSumarry.append(" X ");
					this->_limitCloseOrderSumarry.append(avarageLimitClosePrice._normalizedPrice);
					this->_avarageLimitClosePrice = avarageLimitClosePrice._normalizedPrice;
				}

				this->_stopCloseOrderSumarry = "";
				this->_avarageStopClosePrice = "";
				if (!totalStopLot.isZero())
				{
					Common::Price avarageStopClosePrice = Common::Price::createInstance((totalStopPrice / totalStopLot).toDouble(),
						settingsInstrument->getCalculateSetting()->getNumeratorUnit(),
						settingsInstrument->getCalculateSetting()->getDenominator(), decimals);
					this->_stopCloseOrderSumarry.append(Common::roundToString(totalStopLot.toDouble(), decimals));
					this->_stopCloseOrderSumarry.append(" X ");
					this->_stopCloseOrderSumarry.append(avarageStopClosePrice._normalizedPrice);
					this->_avarageStopClosePrice = avarageStopClosePrice._normalizedPrice;
				}
			}

			QString TradingOrder::getBOOddsInFormat()
			{
				int oddsDecimals = Common::getValidDecimals(this->_BOOdds) - 2;
				if (oddsDecimals < 0) oddsDecimals = 0;
				QString oddsStr =
					Common::roundToString((Business::BusinessHelper::getInstance()->getBOIncludePrincipal() ? this->_BOOdds.toDouble() : this->_BOOdds.toDouble() - 1.0) * 100, oddsDecimals);
				return QString("%1% ").arg(oddsStr);
			}

			int TradingOrder::getBOHitcount()
			{
				return this->_hitCount;
			}

			QString TradingOrder::getBOCountdown()
			{
				if (this->_BOCountdown == 0)
					return "";
				else
					return QString::number(this->_BOCountdown);
			}

			bool TradingOrder::isBOOrder()
			{
				return this->_transaction->getOrderType() == OrderType::BinaryOption;
			}

			QString TradingOrder::getBOBeginTimeStr()
			{
				return !this->_transaction->getExecuteTime().isNull() && this->_transaction->getExecuteTime() != DateTimeConstants::MinQDateTime ? DateTimeHelper::toDateTimeString(this->_transaction->getExecuteTime()) : "";
			}

			QString TradingOrder::getBOEndTimeStr()
			{
				if (this->_transaction->getExecuteTime().isNull()
					|| this->_transaction->getExecuteTime() == DateTimeConstants::MinQDateTime
					|| this->_BOBetTypeId.isNull()
					|| !this->_transaction->getBusinessInstrument()->isExistBOPolicySetting()) return "";
				auto detail = this->_transaction->getBusinessInstrument()->getBOPolicySetting()->getBOPolicyDetailSetting(this->_BOFrequency, this->_BOBetTypeId);
				if (detail.isNull()) return "";
				QDateTime settleTime = BusinessHelper::getInstance()->getSettleTime(this->_transaction->getExecuteTime(), this->_BOFrequency, detail->getOption(), detail->getLastOrderTimeSpan());
				return DateTimeHelper::toDateTimeString(settleTime);
			}

			QDateTime TradingOrder::getBOBeginTime()
			{
				return this->_transaction->getExecuteTime();
			}

			QDateTime TradingOrder::getBOEndTime()
			{
				if (this->_transaction->getExecuteTime().isNull()
					|| this->_transaction->getExecuteTime() == DateTimeConstants::MinQDateTime
					|| this->_BOBetTypeId.isNull()
					|| !this->_transaction->getBusinessInstrument()->isExistBOPolicySetting())
					return DateTimeConstants::MinQDateTime;
				auto detail = this->_transaction->getBusinessInstrument()->getBOPolicySetting()->getBOPolicyDetailSetting(this->_BOFrequency, this->_BOBetTypeId);
				if (detail.isNull())
					return DateTimeConstants::MinQDateTime;
				return BusinessHelper::getInstance()->getSettleTime(this->_transaction->getExecuteTime(), this->_BOFrequency, detail->getOption(), detail->getLastOrderTimeSpan());
			}

			bool TradingOrder::updateBOCountdown(QDateTime now)
			{
				if (this->_transaction->getExecuteTime().isNull() || this->_BOBetTypeId.isNull()) return true;
				auto detail = this->_transaction->getBusinessInstrument()->getBOPolicySetting()->getBOPolicyDetailSetting(this->_BOFrequency, this->_BOBetTypeId);
				if (detail.isNull()) return true;
				int boCountdown = 0;
				if (detail->getOption() == BOBetOption::Instance)
				{
					boCountdown = this->_BOFrequency - (this->_transaction->getExecuteTime().secsTo(now));
				}
				else
				{
					if (this->_BOSettleTime.isNull()) return true;
					boCountdown = now.secsTo(this->_BOSettleTime);
				}
				this->setBOFloatPLInString();
				if (boCountdown >= 0)
				{
					this->setBOCountdown(boCountdown);
					return true;
				}
				else
				{
					this->setBOCountdown(0);
					return false;
				}
			}

			void TradingOrder::setBOFloatPLInString()
			{
				if (this->_phase != Phase::Executed || this->_BOBetTypeId.isNull())
				{
					this->_BOFloatPLInString = "";
					return;
				}
				if (this->_hitCount == 0 && !this->_executePrice.isNullPrice())
				{
					double marketPrice = this->_marketBOPrice.isNullPrice() ? this->_setPrice.toDouble() : this->_marketBOPrice.toDouble();
					double orderPrice = this->_executePrice.toDouble();
					QDecNumber pl = 0;
					if (marketPrice == orderPrice)
					{
						pl = this->_lot;
					}
					else if ((marketPrice > orderPrice && this->_BOOption == 1)
						|| (marketPrice < orderPrice && this->_BOOption == 0))
					{
						pl = (this->_lot * this->_BOOdds.toDouble());
					}
					else
					{
						pl = 0.0;
					}

					if (!Business::BusinessHelper::getInstance()->getBOIncludePrincipal())
					{
						pl -= this->_lot;
					}
					this->_BOFloatPLInString = Common::roundToString(pl.toDouble(), this->getDecimalPalcesForLot());
				}
				else
				{
					this->_BOFloatPLInString = "";
				}
			}


		}
	}
}
