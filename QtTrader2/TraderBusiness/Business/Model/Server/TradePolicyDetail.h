#ifndef BUSINESS_MODEL_SERVER_TRADEPOLICYDETAIL_H
#define BUSINESS_MODEL_SERVER_TRADEPOLICYDETAIL_H
#include "QDecNumber.h"
#include <QUuid>
#include <QXmlStreamReader>

namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class TradePolicyDetail
			{
			private:
				bool _allowIfDone;
                bool _allowIfDoneForDQ;
				bool _allowNewLimitStopSameTime;
				bool _allowNewOCO;
				int _allowedPhysicalTradeSides;
				QUuid _bOPolicyId;
				int _buyInterestValueDay;
				bool _changePlacedOrderAllowed;
				QDecNumber _closeMultiplier;
				QDecNumber _commissionCloseD;
				QDecNumber _commissionCloseO;
				QDecNumber _contractSize;
				int _dQMaxMove;
				QDecNumber _defaultLot;
				QUuid _deliveryChargeId;
				QUuid _deliveryPointGroupId;
				QDecNumber _discountOfOdd;
				bool _goodTillDate;
				bool _goodTillMonthDayOrder;
				bool _goodTillMonthGTF;
				bool _goodTillMonthGTM;
				bool _goodTillMonthSession;
                int _goodTillDateIndex;
                int _goodTillDayIndex;
                int _goodTillSessionIndex;
                int _goodTillMonthIndex;
                int _goodTillFridayIndex;
				QUuid _instalmentPolicyId;
				QUuid _instrumentId;
				QUuid _interestRateId;
				int _isAcceptNewLimit;
				bool _isAcceptNewMOOMOC;
				int _isAcceptNewStop;
				bool _isTradeActive;
				QDecNumber _levyClose;
				QDecNumber _marginD;
				QDecNumber _marginLockedD;
				QDecNumber _marginLockedO;
				QDecNumber _marginO;
				QDecNumber _minClose;
				QDecNumber _minCommissionClose;
				QDecNumber _minCommissionOpen;
                QDecNumber _commissionOpen;
				QDecNumber _minOpen;
				bool _multipleCloseAllowed;
				int _necessaryRound;
				QDecNumber _openMultiplier;
				QDecNumber _pairRelationFactor;
				QDecNumber _partPaidPhysicalNecessary;
				int _paymentForm;
				QDecNumber _physicalDeliveryIncremental;
				QDecNumber _physicalMinDeliveryQuantity;
				QUuid _physicalPaymentDiscountId;
				QUuid _prepaymentInterestRateId;
				int _sellInterestValueDay;
				QUuid _tradePolicyId;
				QDecNumber _valueDiscountAsMargin;
				QUuid _volumeNecessaryId;
                int _deliveryMode;
                QUuid _administrativeChargeId;
                QDecNumber _administrativeChargeConvertRate;


			public:
				bool getAllowIfDone() const
				{
					return _allowIfDone;
				}

                bool getAllowIfDoneForDQ() const
                {
                    return _allowIfDoneForDQ;
                }

				bool getAllowNewLimitStopSameTime() const
				{
					return _allowNewLimitStopSameTime;
				}

				bool getAllowNewOCO() const
				{
					return _allowNewOCO;
				}

				int getAllowedPhysicalTradeSides() const
				{
					return _allowedPhysicalTradeSides;
				}

				QUuid getBOPolicyId() const
				{
					return _bOPolicyId;
				}

				int getBuyInterestValueDay() const
				{
					return _buyInterestValueDay;
				}

				bool getChangePlacedOrderAllowed() const
				{
					return _changePlacedOrderAllowed;
				}

				QDecNumber getCloseMultiplier() const
				{
					return _closeMultiplier;
				}

				QDecNumber getCommissionCloseD() const
				{
					return _commissionCloseD;
				}

				QDecNumber getCommissionCloseO() const
				{
					return _commissionCloseO;
				}

				QDecNumber getContractSize() const
				{
					return _contractSize;
				}

				int getDQMaxMove() const
				{
					return _dQMaxMove;
				}

				QDecNumber getDefaultLot() const
				{
					return _defaultLot;
				}

				QUuid getDeliveryChargeId() const
				{
					return _deliveryChargeId;
				}

				QUuid getDeliveryPointGroupId() const
				{
					return _deliveryPointGroupId;
				}

				QDecNumber getDiscountOfOdd() const
				{
					return _discountOfOdd;
				}

				bool getGoodTillDate() const
				{
					return _goodTillDate;
				}

				bool getGoodTillMonthDayOrder() const
				{
					return _goodTillMonthDayOrder;
				}

				bool getGoodTillMonthGTF() const
				{
					return _goodTillMonthGTF;
				}

				bool getGoodTillMonthGTM() const
				{
					return _goodTillMonthGTM;
				}

				bool getGoodTillMonthSession() const
				{
					return _goodTillMonthSession;
				}

                int getGoodTillDateIndex() const
                {
                    return _goodTillDateIndex;
                }

                int getGoodTillDayIndex() const
                {
                    return _goodTillDayIndex;
                }

                int getGoodTillSessionIndex() const
                {
                    return _goodTillSessionIndex;
                }

                int getGoodTillMonthIndex() const
                {
                    return _goodTillMonthIndex;
                }

                int getGoodTillFridayIndex() const
                {
                    return _goodTillFridayIndex;
                }

				QUuid getInstalmentPolicyId() const
				{
					return _instalmentPolicyId;
				}

				QUuid getInstrumentId() const
				{
					return _instrumentId;
				}

				QUuid getInterestRateId() const
				{
					return _interestRateId;
				}

				int getIsAcceptNewLimit() const
				{
					return _isAcceptNewLimit;
				}

				bool getIsAcceptNewMOOMOC() const
				{
					return _isAcceptNewMOOMOC;
				}

				int getIsAcceptNewStop() const
				{
					return _isAcceptNewStop;
				}

				bool getIsTradeActive() const
				{
					return _isTradeActive;
				}

				QDecNumber getLevyClose() const
				{
					return _levyClose;
				}

				QDecNumber getMarginD() const
				{
					return _marginD;
				}

				QDecNumber getMarginLockedD() const
				{
					return _marginLockedD;
				}

				QDecNumber getMarginLockedO() const
				{
					return _marginLockedO;
				}

				QDecNumber getMarginO() const
				{
					return _marginO;
				}

				QDecNumber getMinClose() const
				{
					return _minClose;
				}

				QDecNumber getMinCommissionClose() const
				{
					return _minCommissionClose;
				}

				QDecNumber getMinCommissionOpen() const
				{
					return _minCommissionOpen;
				}

                QDecNumber getCommissionOpen() const
                {
                    return _commissionOpen;
                }

				QDecNumber getMinOpen() const
				{
					return _minOpen;
				}

				bool getMultipleCloseAllowed() const
				{
					return _multipleCloseAllowed;
				}

				int getNecessaryRound() const
				{
					return _necessaryRound;
				}

				QDecNumber getOpenMultiplier() const
				{
					return _openMultiplier;
				}

				QDecNumber getPairRelationFactor() const
				{
					return _pairRelationFactor;
				}

				QDecNumber getPartPaidPhysicalNecessary() const
				{
					return _partPaidPhysicalNecessary;
				}

				int getPaymentForm() const
				{
					return _paymentForm;
				}

				QDecNumber getPhysicalDeliveryIncremental() const
				{
					return _physicalDeliveryIncremental;
				}

				QDecNumber getPhysicalMinDeliveryQuantity() const
				{
					return _physicalMinDeliveryQuantity;
				}

				QUuid getPhysicalPaymentDiscountId() const
				{
					return _physicalPaymentDiscountId;
				}

				QUuid getPrepaymentInterestRateId() const
				{
					return _prepaymentInterestRateId;
				}

				int getSellInterestValueDay() const
				{
					return _sellInterestValueDay;
				}

				QUuid getTradePolicyId() const
				{
					return _tradePolicyId;
				}

				QDecNumber getValueDiscountAsMargin() const
				{
					return _valueDiscountAsMargin;
				}

				QUuid getVolumeNecessaryId() const
				{
					return _volumeNecessaryId;
				}

                int getDeliveryMode() const
                {
                    return _deliveryMode;
                }

                QUuid getAdministrativeChargeId() const
                {
                    return _administrativeChargeId;
                }

                QDecNumber getAdministrativeChargeConvertRate() const
                {
                    return _administrativeChargeConvertRate;
                }


			public:
				void setAllowIfDone(const bool& value)
				{
					_allowIfDone = value;
				}

                void setAllowIfDoneForDQ(const bool& value)
                {
                    _allowIfDoneForDQ = value;
                }

				void setAllowNewLimitStopSameTime(const bool& value)
				{
					_allowNewLimitStopSameTime = value;
				}

				void setAllowNewOCO(const bool& value)
				{
					_allowNewOCO = value;
				}

				void setAllowedPhysicalTradeSides(const int& value)
				{
					_allowedPhysicalTradeSides = value;
				}

				void setBOPolicyId(const QUuid& value)
				{
					_bOPolicyId = value;
				}

				void setBuyInterestValueDay(const int& value)
				{
					_buyInterestValueDay = value;
				}

				void setChangePlacedOrderAllowed(const bool& value)
				{
					_changePlacedOrderAllowed = value;
				}

				void setCloseMultiplier(const QDecNumber& value)
				{
					_closeMultiplier = value;
				}

				void setCommissionCloseD(const QDecNumber& value)
				{
					_commissionCloseD = value;
				}

				void setCommissionCloseO(const QDecNumber& value)
				{
					_commissionCloseO = value;
				}

				void setContractSize(const QDecNumber& value)
				{
					_contractSize = value;
				}

				void setDQMaxMove(const int& value)
				{
					_dQMaxMove = value;
				}

				void setDefaultLot(const QDecNumber& value)
				{
					_defaultLot = value;
				}

				void setDeliveryChargeId(const QUuid& value)
				{
					_deliveryChargeId = value;
				}

				void setDeliveryPointGroupId(const QUuid& value)
				{
					_deliveryPointGroupId = value;
				}

				void setDiscountOfOdd(const QDecNumber& value)
				{
					_discountOfOdd = value;
				}

				void setGoodTillDate(const bool& value)
				{
					_goodTillDate = value;
				}

				void setGoodTillMonthDayOrder(const bool& value)
				{
					_goodTillMonthDayOrder = value;
				}

				void setGoodTillMonthGTF(const bool& value)
				{
					_goodTillMonthGTF = value;
				}

				void setGoodTillMonthGTM(const bool& value)
				{
					_goodTillMonthGTM = value;
				}

				void setGoodTillMonthSession(const bool& value)
				{
					_goodTillMonthSession = value;
				}

                void setGoodTillDateIndex(const int& value)
                {
                    _goodTillDateIndex = value;
                }

                void setGoodTillDayIndex(const int& value)
                {
                    _goodTillDayIndex = value;
                }

                void setGoodTillSessionIndex(const int& value)
                {
                    _goodTillSessionIndex = value;
                }

                void setGoodTillMonthIndex(const int& value)
                {
                    _goodTillMonthIndex = value;
                }

                void setGoodTillFridayIndex(const int& value)
                {
                    _goodTillFridayIndex = value;
                }

				void setInstalmentPolicyId(const QUuid& value)
				{
					_instalmentPolicyId = value;
				}

				void setInstrumentId(const QUuid& value)
				{
					_instrumentId = value;
				}

				void setInterestRateId(const QUuid& value)
				{
					_interestRateId = value;
				}

				void setIsAcceptNewLimit(const int& value)
				{
					_isAcceptNewLimit = value;
				}

				void setIsAcceptNewMOOMOC(const bool& value)
				{
					_isAcceptNewMOOMOC = value;
				}

				void setIsAcceptNewStop(const int& value)
				{
					_isAcceptNewStop = value;
				}

				void setIsTradeActive(const bool& value)
				{
					_isTradeActive = value;
				}

				void setLevyClose(const QDecNumber& value)
				{
					_levyClose = value;
				}

				void setMarginD(const QDecNumber& value)
				{
					_marginD = value;
				}

				void setMarginLockedD(const QDecNumber& value)
				{
					_marginLockedD = value;
				}

				void setMarginLockedO(const QDecNumber& value)
				{
					_marginLockedO = value;
				}

				void setMarginO(const QDecNumber& value)
				{
					_marginO = value;
				}

				void setMinClose(const QDecNumber& value)
				{
					_minClose = value;
				}

				void setMinCommissionClose(const QDecNumber& value)
				{
					_minCommissionClose = value;
                }

				void setMinCommissionOpen(const QDecNumber& value)
				{
					_minCommissionOpen = value;
				}

                void setCommissionOpen(const QDecNumber& value)
                {
                    _commissionOpen = value;
                }

				void setMinOpen(const QDecNumber& value)
				{
					_minOpen = value;
				}

				void setMultipleCloseAllowed(const bool& value)
				{
					_multipleCloseAllowed = value;
				}

				void setNecessaryRound(const int& value)
				{
					_necessaryRound = value;
				}

				void setOpenMultiplier(const QDecNumber& value)
				{
					_openMultiplier = value;
				}

				void setPairRelationFactor(const QDecNumber& value)
				{
					_pairRelationFactor = value;
				}

				void setPartPaidPhysicalNecessary(const QDecNumber& value)
				{
					_partPaidPhysicalNecessary = value;
				}

				void setPaymentForm(const int& value)
				{
					_paymentForm = value;
				}

				void setPhysicalDeliveryIncremental(const QDecNumber& value)
				{
					_physicalDeliveryIncremental = value;
				}

				void setPhysicalMinDeliveryQuantity(const QDecNumber& value)
				{
					_physicalMinDeliveryQuantity = value;
				}

				void setPhysicalPaymentDiscountId(const QUuid& value)
				{
					_physicalPaymentDiscountId = value;
				}

				void setPrepaymentInterestRateId(const QUuid& value)
				{
					_prepaymentInterestRateId = value;
				}

				void setSellInterestValueDay(const int& value)
				{
					_sellInterestValueDay = value;
				}

				void setTradePolicyId(const QUuid& value)
				{
					_tradePolicyId = value;
				}

				void setValueDiscountAsMargin(const QDecNumber& value)
				{
					_valueDiscountAsMargin = value;
				}

				void setVolumeNecessaryId(const QUuid& value)
				{
					_volumeNecessaryId = value;
				}

                void setDeliveryMode(const int& value)
                {
                    _deliveryMode = value;
                }

                void setAdministrativeChargeId(const QUuid& value)
                {
                    _administrativeChargeId = value;
                }

                void setAdministrativeChargeConvertRate(const QDecNumber& value)
                {
                    _administrativeChargeConvertRate = value;
                }




				public:
					void update(QXmlStreamReader& xmlStreamReader)
					{
						QXmlStreamAttributes attributes = xmlStreamReader.attributes();
						this->_allowIfDone = Common::toBool(attributes.value("AllowIfDone").toString());
                        if(attributes.hasAttribute("AllowIfDone2"))
                            this->_allowIfDoneForDQ = attributes.value("AllowIfDone2").toInt() == 2;
                        else
                            this->_allowIfDoneForDQ = false;
						this->_allowNewLimitStopSameTime = Common::toBool(attributes.value("AllowNewLimitStopSameTime").toString());
						this->_allowNewOCO = Common::toBool(attributes.value("AllowNewOCO").toString());
						this->_allowedPhysicalTradeSides = attributes.value("AllowedPhysicalTradeSides").toInt();						
						this->_bOPolicyId = attributes.value("BOPolicyId").toString() != "" ? QUuid(attributes.value("BOPolicyId").toString()) : QUuid();
						this->_buyInterestValueDay = attributes.value("BuyInterestValueDay").toInt();
						this->_changePlacedOrderAllowed = Common::toBool(attributes.value("ChangePlacedOrderAllowed").toString());
						this->_closeMultiplier = QDecNumber::fromQString(attributes.value("CloseMultiplier").toString());
						this->_commissionCloseD = QDecNumber::fromQString(attributes.value("CommissionCloseD").toString());
						this->_commissionCloseO = QDecNumber::fromQString(attributes.value("CommissionCloseO").toString());
						this->_contractSize = QDecNumber::fromQString(attributes.value("ContractSize").toString());
						this->_dQMaxMove = attributes.value("DQMaxMove").toInt();
						this->_defaultLot = QDecNumber::fromQString(attributes.value("DefaultLot").toString());						
						this->_deliveryChargeId = attributes.value("DeliveryChargeId").toString() != "" ? QUuid(attributes.value("DeliveryChargeId").toString()) : QUuid();						
						this->_deliveryPointGroupId = QUuid(attributes.value("DeliveryPointGroupId").toString());
						this->_discountOfOdd = QDecNumber::fromQString(attributes.value("DiscountOfOdd").toString());
						this->_goodTillDate = Common::toBool(attributes.value("GoodTillDate").toString());
						this->_goodTillMonthDayOrder = Common::toBool(attributes.value("GoodTillMonthDayOrder").toString());
						this->_goodTillMonthGTF = Common::toBool(attributes.value("GoodTillMonthGTF").toString());
						this->_goodTillMonthGTM = Common::toBool(attributes.value("GoodTillMonthGTM").toString());
						this->_goodTillMonthSession = Common::toBool(attributes.value("GoodTillMonthSession").toString());                        
                        this->_goodTillDateIndex = attributes.value("GoodTillDateIndex").toString().toInt();
                        this->_goodTillDayIndex = attributes.value("GoodTillDayIndex").toString().toInt();
                        this->_goodTillSessionIndex = attributes.value("GoodTillSessionIndex").toString().toInt();
                        this->_goodTillMonthIndex = attributes.value("GoodTillMonthIndex").toString().toInt();
                        this->_goodTillFridayIndex = attributes.value("GoodTillFridayIndex").toString().toInt();
						this->_instalmentPolicyId = QUuid(attributes.value("InstalmentPolicyId").toString());
						this->_instrumentId = QUuid(attributes.value("InstrumentId").toString());
						this->_interestRateId = QUuid(attributes.value("InterestRateId").toString());
						this->_isAcceptNewLimit = attributes.value("IsAcceptNewLimit").toString().toInt();
						this->_isAcceptNewMOOMOC = Common::toBool(attributes.value("IsAcceptNewMOOMOC").toString());
						this->_isAcceptNewStop = attributes.value("IsAcceptNewStop").toString().toInt();
						this->_isTradeActive = Common::toBool(attributes.value("IsTradeActive").toString());
						this->_levyClose = QDecNumber::fromQString(attributes.value("LevyClose").toString());
						this->_marginD = QDecNumber::fromQString(attributes.value("MarginD").toString());
						this->_marginLockedD = QDecNumber::fromQString(attributes.value("MarginLockedD").toString());
						this->_marginLockedO = QDecNumber::fromQString(attributes.value("MarginLockedO").toString());
						this->_marginO = QDecNumber::fromQString(attributes.value("MarginO").toString());
						this->_minClose = QDecNumber::fromQString(attributes.value("MinClose").toString());
						this->_minCommissionClose = QDecNumber::fromQString(attributes.value("MinCommissionClose").toString());                        
						this->_minCommissionOpen = QDecNumber::fromQString(attributes.value("MinCommissionOpen").toString());
                        if(attributes.hasAttribute("CommissionOpen"))
                            this->_commissionOpen = QDecNumber::fromQString(attributes.value("CommissionOpen").toString());
                        else
                            this->_commissionOpen = 0.0;
						this->_minOpen = QDecNumber::fromQString(attributes.value("MinOpen").toString());
						this->_multipleCloseAllowed = Common::toBool(attributes.value("MultipleCloseAllowed").toString());
						this->_necessaryRound = attributes.value("NecessaryRound").toInt();
						this->_openMultiplier = QDecNumber::fromQString(attributes.value("OpenMultiplier").toString());
						this->_pairRelationFactor = QDecNumber::fromQString(attributes.value("PairRelationFactor").toString());
						this->_partPaidPhysicalNecessary = QDecNumber::fromQString(attributes.value("PartPaidPhysicalNecessary").toString());
						this->_paymentForm = attributes.value("PaymentForm").toInt();
						this->_physicalDeliveryIncremental = QDecNumber::fromQString(attributes.value("PhysicalDeliveryIncremental").toString());
						this->_physicalMinDeliveryQuantity = QDecNumber::fromQString(attributes.value("PhysicalMinDeliveryQuantity").toString());						
						this->_physicalPaymentDiscountId = attributes.value("PhysicalPaymentDiscountId").toString()!="" ? QUuid(attributes.value("PhysicalPaymentDiscountId").toString()) : QUuid();						
						this->_prepaymentInterestRateId = attributes.value("PrepaymentInterestRateId").toString() != "" ? QUuid(attributes.value("PrepaymentInterestRateId").toString()) : QUuid();
						this->_sellInterestValueDay = attributes.value("SellInterestValueDay").toInt();
						this->_tradePolicyId = QUuid(attributes.value("TradePolicyId").toString());
						this->_valueDiscountAsMargin = QDecNumber::fromQString(attributes.value("ValueDiscountAsMargin").toString());						
						this->_volumeNecessaryId = attributes.value("VolumeNecessaryId").toString() != "" ? QUuid(attributes.value("VolumeNecessaryId").toString()) : QUuid();
                        this->_deliveryMode = attributes.value("DeliveryMode").toInt();
                        this->_administrativeChargeId = attributes.value("AdministrativeChargeId").toString() != "" ? QUuid(attributes.value("AdministrativeChargeId").toString()) : QUuid();
                        this->_administrativeChargeConvertRate = QDecNumber::fromQString(attributes.value("AdministrativeChargeConvertRate").toString());
                    }

					public:
						void update(const QDomNode& xmlNode)
						{
							QDomNamedNodeMap map = xmlNode.attributes();
							for (int i = 0; i < map.count(); i++)
							{
								QDomNode qDomNode = map.item(i);
								QString name = qDomNode.nodeName();
								QString value = qDomNode.nodeValue();
								if (name == "AllowIfDone")
									this->_allowIfDone = Common::toBool(value);
                                else if (name == "AllowIfDone2")
                                    this->_allowIfDoneForDQ = (value.toInt() == 2);
								else if (name == "AllowNewLimitStopSameTime")
									this->_allowNewLimitStopSameTime = Common::toBool(value);
								else if (name == "AllowNewOCO")
									this->_allowNewOCO = Common::toBool(value);
								else if (name == "AllowedPhysicalTradeSides")
									this->_allowedPhysicalTradeSides = value.toInt();
								else if (name == "BOPolicyId")
									this->_bOPolicyId = value != "" ? QUuid(value) : QUuid();
								else if (name == "BuyInterestValueDay")
									this->_buyInterestValueDay = value.toInt();
								else if (name == "ChangePlacedOrderAllowed")
									this->_changePlacedOrderAllowed = Common::toBool(value);
								else if (name == "CloseMultiplier")
									this->_closeMultiplier = QDecNumber::fromQString(value);
								else if (name == "CommissionCloseD")
									this->_commissionCloseD = QDecNumber::fromQString(value);
								else if (name == "CommissionCloseO")
									this->_commissionCloseO = QDecNumber::fromQString(value);
								else if (name == "ContractSize")
									this->_contractSize = QDecNumber::fromQString(value);
								else if (name == "DQMaxMove")
									this->_dQMaxMove = value.toInt();
								else if (name == "DefaultLot")
									this->_defaultLot = QDecNumber::fromQString(value);
								else if (name == "DeliveryChargeId")
									this->_deliveryChargeId = value != "" ? QUuid(value) : QUuid();
								else if (name == "DeliveryPointGroupId")
									this->_deliveryPointGroupId = value != "" ? QUuid(value) : QUuid();
								else if (name == "DiscountOfOdd")
									this->_discountOfOdd = QDecNumber::fromQString(value);
								else if (name == "GoodTillDate")
									this->_goodTillDate = Common::toBool(value);
								else if (name == "GoodTillMonthDayOrder")
									this->_goodTillMonthDayOrder = Common::toBool(value);
								else if (name == "GoodTillMonthGTF")
									this->_goodTillMonthGTF = Common::toBool(value);
								else if (name == "GoodTillMonthGTM")
									this->_goodTillMonthGTM = Common::toBool(value);
								else if (name == "GoodTillMonthSession")
									this->_goodTillMonthSession = Common::toBool(value);                                
                                else if (name == "GoodTillDateIndex")
                                    this->_goodTillDateIndex = value.toInt();
                                else if (name == "GoodTillDayIndex")
                                    this->_goodTillDayIndex = value.toInt();
                                else if (name == "GoodTillSessionIndex")
                                    this->_goodTillSessionIndex = value.toInt();
                                else if (name == "GoodTillMonthIndex")
                                    this->_goodTillMonthIndex = value.toInt();
                                else if (name == "GoodTillFridayIndex")
                                    this->_goodTillFridayIndex = value.toInt();
								else if (name == "InstalmentPolicyId")
									this->_instalmentPolicyId = value != "" ? QUuid(value) : QUuid();
								else if (name == "InstrumentId")
									this->_instrumentId = QUuid(value);
								else if (name == "InterestRateId")
									this->_interestRateId = QUuid(value);
								else if (name == "IsAcceptNewLimit")
									this->_isAcceptNewLimit = value.toInt();
								else if (name == "IsAcceptNewMOOMOC")
									this->_isAcceptNewMOOMOC = Common::toBool(value);
								else if (name == "IsAcceptNewStop")
									this->_isAcceptNewStop = value.toInt();
								else if (name == "IsTradeActive")
									this->_isTradeActive = Common::toBool(value);
								else if (name == "LevyClose")
									this->_levyClose = QDecNumber::fromQString(value);
								else if (name == "MarginD")
									this->_marginD = QDecNumber::fromQString(value);
								else if (name == "MarginLockedD")
									this->_marginLockedD = QDecNumber::fromQString(value);
								else if (name == "MarginLockedO")
									this->_marginLockedO = QDecNumber::fromQString(value);
								else if (name == "MarginO")
									this->_marginO = QDecNumber::fromQString(value);
								else if (name == "MinClose")
									this->_minClose = QDecNumber::fromQString(value);
								else if (name == "MinCommissionClose")
									this->_minCommissionClose = QDecNumber::fromQString(value);
								else if (name == "MinCommissionOpen")
									this->_minCommissionOpen = QDecNumber::fromQString(value);
                                else if (name == "CommissionOpen")
                                    this->_commissionOpen = QDecNumber::fromQString(value);
								else if (name == "MinOpen")
									this->_minOpen = QDecNumber::fromQString(value);
								else if (name == "MultipleCloseAllowed")
									this->_multipleCloseAllowed = Common::toBool(value);
								else if (name == "NecessaryRound")
									this->_necessaryRound = value.toInt();
								else if (name == "OpenMultiplier")
									this->_openMultiplier = QDecNumber::fromQString(value);
								else if (name == "PairRelationFactor")
									this->_pairRelationFactor = QDecNumber::fromQString(value);
								else if (name == "PartPaidPhysicalNecessary")
									this->_partPaidPhysicalNecessary = QDecNumber::fromQString(value);
								else if (name == "PaymentForm")
									this->_paymentForm = value.toInt();
								else if (name == "PhysicalDeliveryIncremental")
									this->_physicalDeliveryIncremental = QDecNumber::fromQString(value);
								else if (name == "PhysicalMinDeliveryQuantity")
									this->_physicalMinDeliveryQuantity = QDecNumber::fromQString(value);
								else if (name == "PhysicalPaymentDiscountId")
									this->_physicalPaymentDiscountId = value != "" ? QUuid(value) : QUuid();
								else if (name == "PrepaymentInterestRateId")
									this->_prepaymentInterestRateId = value != "" ? QUuid(value) : QUuid();
								else if (name == "SellInterestValueDay")
									this->_sellInterestValueDay = value.toInt();
								else if (name == "TradePolicyId")
									this->_tradePolicyId = QUuid(value);
								else if (name == "ValueDiscountAsMargin")
									this->_valueDiscountAsMargin = QDecNumber::fromQString(value);
								else if (name == "VolumeNecessaryId")
									this->_volumeNecessaryId = value!="" ? QUuid(value) : QUuid();
                                else if (name == "DeliveryMode")
                                    this->_deliveryMode = value.toInt();
                                else if (name == "AdministrativeChargeId")
                                    this->_administrativeChargeId = value!="" ? QUuid(value) : QUuid();
                                else if (name == "AdministrativeChargeConvertRate")
                                    this->_administrativeChargeConvertRate = QDecNumber::fromQString(value);
							}
						}

			};

		}
	}
}
#endif

