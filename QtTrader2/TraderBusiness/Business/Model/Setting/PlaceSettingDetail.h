#ifndef BUSINESS_MODEL_SETTING_PLACESETTINGDETAIL_H
#define BUSINESS_MODEL_SETTING_PLACESETTINGDETAIL_H
#include "Common/Enum/PlaceEnum.h"
#include "Common/Util/Convert_Common.h"
#include "traderbusiness_global.h"
#include <QString>
#include <QMap>
#include "QDecNumber.h"
#include <QStringList>

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class TRADERBUSINESS_EXPORT PlaceSettingDetail
			{
			private:
				Common::Enum::OrderType::OrderType _orderType;
				bool _allowTrade;
				bool _allowMultipleClose;
				QDecNumber _dQQuoteMinLot;
				int _acceptDQVariation;
				bool _allowLimitStop;
				bool _allowMarket; 
				bool _allowMarketOnOpen;
				bool _allowMarketOnClose;
				bool _isAcceptNewMOOMOC;
				bool _allowOneCancelOther;
				bool _allowIfDone;
                bool _allowIfDoneForDQ;
				bool _allowNewLimitStopSameTime; 
				bool _allowNewOCO;
				bool _allowChangePlacedOrder; 
				bool _allowLimitAcceptHedge; 
				bool _allowLimitAcceptNew;
				bool _allowLimitAcceptUnlock; 
				bool _allowStopAcceptHedge;
				bool _allowStopAcceptNew; 
				bool _allowStopAcceptUnlock;
				int _acceptIfDoneVariation;
				int _acceptLmtVariation;
				int _cancelLmtVariation;
				int _acceptCloseLmtVariation;

                int _goodTillDateIndex;
                int _goodTillDayIndex;
                int _goodTillSessionIndex;
                int _goodTillMonthIndex;
                int _goodTillFridayIndex;

                QMap<int, Common::Enum::PendingOrderExpireType::PendingOrderExpireType> _allExpireType;
			public:	
				void update(const QDomNode& xmlPlaceSettingDetail)
				{
					QDomNamedNodeMap map = xmlPlaceSettingDetail.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "OrderType")
							this->_orderType = static_cast<Common::Enum::OrderType::OrderType>(value.toInt());
						else if(name == "AllowTrade")
							this->_allowTrade= Common::toBool(value);
						else if(name == "AllowMultipleClose")
							this->_allowMultipleClose= Common::toBool(value);
						else if(name == "DQQuoteMinLot")
							this->_dQQuoteMinLot = QDecNumber::fromQString(value);
						else if(name == "AcceptDQVariation")
							this->_acceptDQVariation= value.toInt();
						else if(name == "AllowLimitStop")
							this->_allowLimitStop= Common::toBool(value);	
						else if(name == "AllowMarket")
							this->_allowMarket= Common::toBool(value);		
						else if(name == "AllowMarketOnClose")
							this->_allowMarketOnClose= Common::toBool(value);	
						else if(name == "AllowMarketOnOpen")
							this->_allowMarketOnOpen= Common::toBool(value);	
						else if(name == "IsAcceptNewMOOMOC")
							this->_isAcceptNewMOOMOC = Common::toBool(value);
						else if(name == "AllowOneCancelOther")
							this->_allowOneCancelOther= Common::toBool(value);
                        else if(name == "AllowIfDone")
                            this->_allowIfDone = Common::toBool(value);
                        else if(name == "AllowIfDone2")
                            this->_allowIfDoneForDQ = value.toInt() == 2;
						else if(name == "AllowNewLimitStopSameTime")
							this->_allowNewLimitStopSameTime= Common::toBool(value);
						else if(name == "AllowNewOCO")
							this->_allowNewOCO= Common::toBool(value);
						else if(name == "AllowChangePlacedOrder")
							this->_allowChangePlacedOrder= Common::toBool(value);
						else if(name == "AllowLimitAcceptHedge")
							this->_allowLimitAcceptHedge= Common::toBool(value);
						else if(name == "AllowLimitAcceptNew")
							this->_allowLimitAcceptNew= Common::toBool(value);
						else if(name == "AllowLimitAcceptUnlock")
							this->_allowLimitAcceptUnlock= Common::toBool(value);
						else if(name == "AllowStopAcceptHedge")
							this->_allowStopAcceptHedge= Common::toBool(value);
						else if(name == "AllowStopAcceptNew")
							this->_allowStopAcceptNew= Common::toBool(value);
						else if(name == "AllowStopAcceptUnlock")
							this->_allowStopAcceptUnlock= Common::toBool(value);
						else if(name == "AcceptIfDoneVariation")
							this->_acceptIfDoneVariation= value.toInt();
						else if(name == "AcceptLmtVariation")
							this->_acceptLmtVariation= value.toInt();
						else if(name == "CancelLmtVariation")
							this->_cancelLmtVariation= value.toInt();
						else if(name == "AcceptCloseLmtVariation")
							this->_acceptCloseLmtVariation= value.toInt();
						else if(name == "AllExpireType")
						{
							QStringList list = value.split(",");
                            this->_allExpireType = QMap<int, Common::Enum::PendingOrderExpireType::PendingOrderExpireType>();
							for (int i = 0; i < list.length(); i++)
							{
								QString expireType = list.at(i);
								if(expireType == "GoodTillDate")
                                    this->_allExpireType.insert(_goodTillDateIndex, Common::Enum::PendingOrderExpireType::GoodTillDate);
								else if(expireType == "GoodTillDay")
                                    this->_allExpireType.insert(_goodTillDayIndex, Common::Enum::PendingOrderExpireType::GoodTillDay);
								else if(expireType == "GoodTillFriday")
                                    this->_allExpireType.insert(_goodTillFridayIndex, Common::Enum::PendingOrderExpireType::GoodTillFriday);
								else if(expireType == "GoodTillMonth")
                                    this->_allExpireType.insert(_goodTillMonthIndex, Common::Enum::PendingOrderExpireType::GoodTillMonth);
								else if(expireType == "GoodTillSession")
                                    this->_allExpireType.insert(_goodTillSessionIndex, Common::Enum::PendingOrderExpireType::GoodTillSession);
							}
						}
					}
				}
				void clearExpireType()
				{
					_allExpireType.clear();
				}
                void addExpireType(int index, Common::Enum::PendingOrderExpireType::PendingOrderExpireType value)
				{
                    _allExpireType.insert(index, value);
				}
					
				void init()
				{
					_allowTrade = false;
					_allowMultipleClose = false;
					_allowLimitStop = false;
					_allowMarket = false; 
					_allowMarketOnOpen = false;
					_allowMarketOnClose = false;
					_isAcceptNewMOOMOC = false;
					_allowOneCancelOther = false;
					_allowIfDone = false;
                    _allowIfDoneForDQ = false;
					_allowNewLimitStopSameTime = false; 
					_allowNewOCO = false;
					_allowChangePlacedOrder = false; 
					_allowLimitAcceptHedge = false; 
					_allowLimitAcceptNew = false;
					_allowLimitAcceptUnlock = false; 
					_allowStopAcceptHedge = false;
					_allowStopAcceptNew = false; 
					_allowStopAcceptUnlock = false;
					this->_allExpireType.clear();
				}

                QSharedPointer<PlaceSettingDetail> copy()
                {
                    QSharedPointer<PlaceSettingDetail> newObject(new PlaceSettingDetail);
                    newObject->_orderType = _orderType;
                    newObject->_allowTrade = _allowTrade;
                    newObject->_allowMultipleClose = _allowMultipleClose;
                    newObject->_dQQuoteMinLot = _dQQuoteMinLot;
                    newObject->_acceptDQVariation = _acceptDQVariation;
                    newObject->_allowLimitStop = _allowLimitStop;
                    newObject->_allowMarket = _allowMarket;
                    newObject->_allowMarketOnOpen = _allowMarketOnOpen;
                    newObject->_allowMarketOnClose = _allowMarketOnClose;
                    newObject->_isAcceptNewMOOMOC = _isAcceptNewMOOMOC;
                    newObject->_allowOneCancelOther = _allowOneCancelOther;
                    newObject->_allowIfDone = _allowIfDone;
                    newObject->_allowIfDoneForDQ = _allowIfDoneForDQ;
                    newObject->_allowNewLimitStopSameTime = _allowNewLimitStopSameTime;
                    newObject->_allowNewOCO = _allowNewOCO;
                    newObject->_allowChangePlacedOrder = _allowChangePlacedOrder;
                    newObject->_allowLimitAcceptHedge = _allowLimitAcceptHedge;
                    newObject->_allowLimitAcceptNew = _allowLimitAcceptNew;
                    newObject->_allowLimitAcceptUnlock = _allowLimitAcceptUnlock;
                    newObject->_allowStopAcceptHedge = _allowStopAcceptHedge;
                    newObject->_allowStopAcceptNew = _allowStopAcceptNew;
                    newObject->_allowStopAcceptUnlock = _allowStopAcceptUnlock;
                    newObject->_acceptIfDoneVariation = _acceptIfDoneVariation;
                    newObject->_acceptLmtVariation = _acceptLmtVariation;
                    newObject->_cancelLmtVariation = _cancelLmtVariation;
                    newObject->_acceptCloseLmtVariation = _acceptCloseLmtVariation;
                    newObject->_goodTillDateIndex = _orderType;
                    newObject->_goodTillDayIndex = _orderType;
                    newObject->_goodTillSessionIndex = _orderType;
                    newObject->_goodTillMonthIndex = _orderType;
                    newObject->_goodTillFridayIndex = _orderType;
                    newObject->_allExpireType = _allExpireType;
                    return newObject;
                }


				public:	
					Common::Enum::OrderType::OrderType getOrderType() const
					{
						return _orderType;
					}

					bool getAllowTrade() const
					{
						return _allowTrade;
					}

					bool getAllowMultipleClose() const
					{
						return _allowMultipleClose;
					}

					QDecNumber getDQQuoteMinLot() const
					{
						return _dQQuoteMinLot;
					}

					int getAcceptDQVariation() const
					{
						return _acceptDQVariation;
					}

					bool getAllowLimitStop() const
					{
						return _allowLimitStop;
					}
					
					bool getAllowMarket() const
					{
						return _allowMarket;
					}

					bool getAllowMarketOnOpen() const
					{
						return _allowMarketOnOpen;
					}

					bool getAllowMarketOnClose() const
					{
						return _allowMarketOnClose;
					}

					bool getIsAcceptNewMOOMOC() const
					{
						return _isAcceptNewMOOMOC;
					}
										
					bool getAllowOneCancelOther() const
					{
						return _allowOneCancelOther;
					}

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

					bool getAllowChangePlacedOrder() const
					{
						return _allowChangePlacedOrder;
					}

					bool getAllowLimitAcceptHedge() const
					{
						return _allowLimitAcceptHedge;
					}

					bool getAllowLimitAcceptNew() const
					{
						return _allowLimitAcceptNew;
					}

					bool getAllowLimitAcceptUnlock() const
					{
						return _allowLimitAcceptUnlock;
					}

					bool getAllowStopAcceptHedge() const
					{
						return _allowStopAcceptHedge;
					}

					bool getAllowStopAcceptNew() const
					{
						return _allowStopAcceptNew;
					}

					bool getAllowStopAcceptUnlock() const
					{
						return _allowStopAcceptUnlock;
					}

					int getAcceptIfDoneVariation() const
					{
						return _acceptIfDoneVariation;
					}

					int getAcceptLmtVariation() const
					{
						return _acceptLmtVariation;
					}

					int getCancelLmtVariation() const
					{
						return _cancelLmtVariation;
					}

					int getAcceptCloseLmtVariation() const
					{
						return _acceptCloseLmtVariation;
					}

                    QMap<int, Common::Enum::PendingOrderExpireType::PendingOrderExpireType> getPendingOrderExpireType()
					{
						return _allExpireType;
					}



					void setAcceptCloseLmtVariation(const int& value)
					{
						_acceptCloseLmtVariation = value;
					}

					void setAcceptDQVariation(const int& value)
					{
						_acceptDQVariation = value;
					}

					void setAcceptIfDoneVariation(const int& value)
					{
						_acceptIfDoneVariation = value;
					}

					void setAcceptLmtVariation(const int& value)
					{
						_acceptLmtVariation = value;
					}

					void setAllowChangePlacedOrder(const bool& value)
					{
						_allowChangePlacedOrder = value;
					}

					void setAllowIfDone(const bool& value)
					{
						_allowIfDone = value;
					}

                    void setAllowIfDoneForDQ(const bool& value)
                    {
                        _allowIfDoneForDQ = value;
                    }

					void setAllowLimitAcceptHedge(const bool& value)
					{
						_allowLimitAcceptHedge = value;
					}

					void setAllowLimitAcceptNew(const bool& value)
					{
						_allowLimitAcceptNew = value;
					}

					void setAllowLimitAcceptUnlock(const bool& value)
					{
						_allowLimitAcceptUnlock = value;
					}

					void setAllowLimitStop(const bool& value)
					{
						_allowLimitStop = value;
					}

					void setAllowMarket(const bool& value)
					{
						_allowMarket = value;
					}

					void setAllowMarketOnClose(const bool& value)
					{
						_allowMarketOnClose = value;
					}

					void setAllowMarketOnOpen(const bool& value)
					{
						_allowMarketOnOpen = value;
					}

					void setAllowMultipleClose(const bool& value)
					{
						_allowMultipleClose = value;
					}

					void setAllowNewLimitStopSameTime(const bool& value)
					{
						_allowNewLimitStopSameTime = value;
					}

					void setAllowNewOCO(const bool& value)
					{
						_allowNewOCO = value;
					}

					void setAllowOneCancelOther(const bool& value)
					{
						_allowOneCancelOther = value;
					}

					void setAllowStopAcceptHedge(const bool& value)
					{
						_allowStopAcceptHedge = value;
					}

					void setAllowStopAcceptNew(const bool& value)
					{
						_allowStopAcceptNew = value;
					}

					void setAllowStopAcceptUnlock(const bool& value)
					{
						_allowStopAcceptUnlock = value;
					}

					void setAllowTrade(const bool& value)
					{
						_allowTrade = value;
					}

					void setCancelLmtVariation(const int& value)
					{
						_cancelLmtVariation = value;
					}

					void setDQQuoteMinLot(const QDecNumber& value)
					{
						_dQQuoteMinLot = value;
					}

					void setIsAcceptNewMOOMOC(const bool& value)
					{
						_isAcceptNewMOOMOC = value;
					}

					void setOrderType(const Common::Enum::OrderType::OrderType& value)
					{
						_orderType = value;
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


				};
			}
		}
	}
#endif

