#ifndef BUSINESS_MODEL_SETTING_CALCULATESETTING_H
#define BUSINESS_MODEL_SETTING_CALCULATESETTING_H
#include "Common/Enum/CommonEnum.h"
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"
#include "SettingDataBase.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class CalculateSetting: public SettingDataBase
			{
			public:
				CalculateSetting()
				{
                    _pairRelationFactor = -1.0;
                }

			private:
				bool _isCalculateFloat;
				QDecNumber  _rateMarginD;
				QDecNumber  _rateMarginO;
				QDecNumber  _rateMarginLockD;
				QDecNumber  _rateMarginLockO;
				bool _caculateChangeWithDenominator;
				bool _useNightNecessaryWhenBreak;
				int  _tradePLFormula;
				int  _marginFormula;				
				int  _numeratorUnit;
				int  _denominator;	
				QDecNumber  _marginD;
				QDecNumber  _marginO;
				QDecNumber  _marginLockedD;
				QDecNumber  _marginLockedO;
				QDecNumber  _pairRelationFactor;
				QDecNumber  _discountOfOdd;
				QDecNumber  _valueDiscountAsMargin;
				QDecNumber  _physicalMinDeliveryQuantity;
				QDecNumber  _physicalDeliveryIncremental;
				QDecNumber  _partPaidPhysicalNecessary;
				int  _necessaryRound;	
				QDecNumber  _rateCommission;
				QDecNumber  _commissionCloseD;
				QDecNumber  _commissionCloseO;
				QDecNumber  _minCommissionClose;
				QDecNumber  _minCommissionOpen;
                QDecNumber  _commissionOpen;
				int  _commissionFormula;
				QDecNumber  _rateLevy;
				QDecNumber  _levyClose;
				int _levyFormula;

			public:
				void update(const QDomNode& xmlCalculateSetting)
				{
					QDomNamedNodeMap map = xmlCalculateSetting.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "IsCalculateFloat")
							this->_isCalculateFloat= Common::toBool(value);
						else if(name == "RateMarginD")
							this->_rateMarginD= QDecNumber::fromQString(value);
						else if(name == "RateMarginO")
							this->_rateMarginO= QDecNumber::fromQString(value);
						else if(name == "RateMarginLockD")
							this->_rateMarginLockD= QDecNumber::fromQString(value);
						else if(name == "RateMarginLockO")
							this->_rateMarginLockO= QDecNumber::fromQString(value);
						else if(name == "CaculateChangeWithDenominator")
							this->_caculateChangeWithDenominator= Common::toBool(value);
						else if(name == "UseNightNecessaryWhenBreak")
							this->_useNightNecessaryWhenBreak= Common::toBool(value);
						else if(name == "TradePLFormula")
							this->_tradePLFormula= value.toInt();
						else if(name == "MarginFormula")
							this->_marginFormula= value.toInt();						
						else if(name == "NumeratorUnit")
							this->_numeratorUnit= value.toInt();
						else if(name == "Denominator")
							this->_denominator= value.toInt();						
						else if(name == "MarginD")
							this->_marginD= QDecNumber::fromQString(value);
						else if(name == "MarginO")
							this->_marginO= QDecNumber::fromQString(value);
						else if(name == "MarginLockedD")
							this->_marginLockedD= QDecNumber::fromQString(value);
						else if(name == "MarginLockedO")
							this->_marginLockedO= QDecNumber::fromQString(value);
						else if(name == "PairRelationFactor")
							this->_pairRelationFactor= QDecNumber::fromQString(value);
						else if(name == "DiscountOfOdd")
							this->_discountOfOdd= QDecNumber::fromQString(value);
						else if(name == "ValueDiscountAsMargin")
							this->_valueDiscountAsMargin= QDecNumber::fromQString(value);
						else if(name == "PhysicalMinDeliveryQuantity")
							this->_physicalMinDeliveryQuantity= QDecNumber::fromQString(value);
						else if(name == "PhysicalDeliveryIncremental")
							this->_physicalDeliveryIncremental= QDecNumber::fromQString(value);
						else if(name == "PartPaidPhysicalNecessary")
							this->_partPaidPhysicalNecessary= QDecNumber::fromQString(value);
						else if(name == "NecessaryRound")
							this->_necessaryRound= value.toInt();
						else if(name == "RateCommission")
							this->_rateCommission= QDecNumber::fromQString(value);
						else if(name == "CommissionCloseD")
							this->_commissionCloseD= QDecNumber::fromQString(value);
						else if(name == "CommissionCloseO")
							this->_commissionCloseO = QDecNumber::fromQString(value);
						else if(name == "MinCommissionClose")
							this->_minCommissionClose= QDecNumber::fromQString(value);
						else if(name == "MinCommissionOpen")
							this->_minCommissionOpen= QDecNumber::fromQString(value);
                        else if(name == "CommissionOpen")
                            this->_commissionOpen= QDecNumber::fromQString(value);
						else if(name == "CommissionFormula")
							this->_commissionFormula= value.toInt();	
						else if(name == "RateLevy")
							this->_rateLevy= QDecNumber::fromQString(value);
						else if(name == "LevyClose")
							this->_levyClose= QDecNumber::fromQString(value);
						else if(name == "LevyFormula")
							this->_levyFormula= value.toInt();	
					}
				}

                QSharedPointer<CalculateSetting> copy()
                {
                    QSharedPointer<CalculateSetting> newObject(new CalculateSetting);
                    newObject->_isCalculateFloat = _isCalculateFloat;
                    newObject->_rateMarginD = _rateMarginD;
                    newObject->_rateMarginO = _rateMarginO;
                    newObject->_rateMarginLockD = _rateMarginLockD;
                    newObject->_rateMarginLockO = _rateMarginLockO;
                    newObject->_caculateChangeWithDenominator = _caculateChangeWithDenominator;
                    newObject->_useNightNecessaryWhenBreak = _useNightNecessaryWhenBreak;
                    newObject->_tradePLFormula = _tradePLFormula;
                    newObject->_marginFormula = _marginFormula;
                    newObject->_numeratorUnit = _numeratorUnit;
                    newObject->_denominator = _denominator;
                    newObject->_marginD = _marginD;
                    newObject->_marginO = _marginO;
                    newObject->_marginLockedD = _marginLockedD;
                    newObject->_marginLockedO = _marginLockedO;
                    newObject->_pairRelationFactor = _pairRelationFactor;
                    newObject->_discountOfOdd = _discountOfOdd;
                    newObject->_valueDiscountAsMargin = _valueDiscountAsMargin;
                    newObject->_physicalMinDeliveryQuantity = _physicalMinDeliveryQuantity;
                    newObject->_physicalDeliveryIncremental = _physicalDeliveryIncremental;
                    newObject->_partPaidPhysicalNecessary = _partPaidPhysicalNecessary;
                    newObject->_necessaryRound = _necessaryRound;
                    newObject->_rateCommission = _rateCommission;
                    newObject->_commissionCloseD = _commissionCloseD;
                    newObject->_commissionCloseO = _commissionCloseO;
                    newObject->_minCommissionClose = _minCommissionClose;
                    newObject->_minCommissionOpen = _minCommissionOpen;
                    newObject->_commissionOpen = _commissionOpen;
                    newObject->_commissionFormula = _commissionFormula;
                    newObject->_rateLevy = _rateLevy;
                    newObject->_levyClose = _levyClose;
                    newObject->_levyFormula = _levyFormula;
                    return newObject;
                }

			public:	
				bool getIsCalculateFloat() const
				{
					return _isCalculateFloat;
				}

				QDecNumber getRateMarginD() const
				{
					return _rateMarginD;
				}

				QDecNumber getRateMarginO() const
				{
					return _rateMarginO;
				}

				QDecNumber getRateMarginLockD() const
				{
					return _rateMarginLockD;
				}

				QDecNumber getRateMarginLockO() const
				{
					return _rateMarginLockO;
				}

				bool getCaculateChangeWithDenominator() const
				{
					return _caculateChangeWithDenominator;
				}

				bool getUseNightNecessaryWhenBreak() const
				{
					return _useNightNecessaryWhenBreak;
				}

				int getTradePLFormula() const
				{
					return _tradePLFormula;
				}

				int getMarginFormula() const
				{
					return _marginFormula;
				}
				
				int getNumeratorUnit() const
				{
					return _numeratorUnit;
				}

				int getDenominator() const
				{
					return _denominator;
				}

				QDecNumber getMarginD() const
				{
					return _marginD;
				}

				QDecNumber getMarginO() const
				{
					return _marginO;
				}

				QDecNumber getMarginLockedD() const
				{
					return _marginLockedD;
				}

				QDecNumber getMarginLockedO() const
				{
					return _marginLockedO;
				}

				QDecNumber getPairRelationFactor() const
				{
					return _pairRelationFactor;
				}

				QDecNumber getDiscountOfOdd() const
				{
					return _discountOfOdd;
				}

				QDecNumber getValueDiscountAsMargin() const
				{
					return _valueDiscountAsMargin;
				}

				QDecNumber getPhysicalMinDeliveryQuantity() const
				{
					return _physicalMinDeliveryQuantity;
				}

				QDecNumber getPhysicalDeliveryIncremental() const
				{
					return _physicalDeliveryIncremental;
				}

				QDecNumber getPartPaidPhysicalNecessary() const
				{
					return _partPaidPhysicalNecessary;
				}

				int getNecessaryRound() const
				{
					return _necessaryRound;
				}

				int getDecimal()
				{
					return (int)log10((double)_denominator);
				}

				QDecNumber getRateCommission() const
				{
					return _rateCommission;
				}

				QDecNumber getCommissionCloseD() const
				{
					return _commissionCloseD;
				}

				QDecNumber getCommissionCloseO() const
				{
					return _commissionCloseO;
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

				int getCommissionFormula() const
				{
					return _commissionFormula;
				}


				QDecNumber getRateLevy() const
				{
					return _rateLevy;
				}

				QDecNumber getLevyClose() const
				{
					return _levyClose;
				}

				int getLevyFormula() const
				{
					return _levyFormula;
				}




				void setCaculateChangeWithDenominator(const bool& value)
				{
					_caculateChangeWithDenominator = value;
				}

				void setCommissionCloseD(const QDecNumber& value)
				{
					_commissionCloseD = value;
				}

				void setCommissionCloseO(const QDecNumber& value)
				{
					_commissionCloseO = value;
				}

				void setCommissionFormula(const int& value)
				{
					_commissionFormula = value;
				}

				void setDenominator(const int& value)
				{
					_denominator = value;
				}

				void setDiscountOfOdd(const QDecNumber& value)
				{
					_discountOfOdd = value;
				}

				void setIsCalculateFloat(const bool& value)
				{
					_isCalculateFloat = value;
				}

				void setLevyClose(const QDecNumber& value)
				{
					_levyClose = value;
				}

				void setLevyFormula(const int& value)
				{
					_levyFormula = value;
				}

				void setMarginD(const QDecNumber& value)
				{
					_marginD = value;
				}

				void setMarginFormula(const int& value)
				{
					_marginFormula = value;
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

				void setNecessaryRound(const int& value)
				{
					_necessaryRound = value;
				}

				void setNumeratorUnit(const int& value)
				{
					_numeratorUnit = value;
				}

				void setPairRelationFactor(const QDecNumber& value)
				{
					_pairRelationFactor = value;
				}

				void setPartPaidPhysicalNecessary(const QDecNumber& value)
				{
					_partPaidPhysicalNecessary = value;
				}

				void setPhysicalDeliveryIncremental(const QDecNumber& value)
				{
					_physicalDeliveryIncremental = value;
				}

				void setPhysicalMinDeliveryQuantity(const QDecNumber& value)
				{
					_physicalMinDeliveryQuantity = value;
				}

				void setRateCommission(const QDecNumber& value)
				{
					_rateCommission = value;
				}

				void setRateLevy(const QDecNumber& value)
				{
					_rateLevy = value;
				}

				void setRateMarginD(const QDecNumber& value)
				{
					_rateMarginD = value;
				}

				void setRateMarginLockD(const QDecNumber& value)
				{
					_rateMarginLockD = value;
				}

				void setRateMarginLockO(const QDecNumber& value)
				{
					_rateMarginLockO = value;
				}

				void setRateMarginO(const QDecNumber& value)
				{
					_rateMarginO = value;
				}

				void setTradePLFormula(const int& value)
				{
					_tradePLFormula = value;
				}

				void setUseNightNecessaryWhenBreak(const bool& value)
				{
					_useNightNecessaryWhenBreak = value;
				}

				void setValueDiscountAsMargin(const QDecNumber& value)
				{
					_valueDiscountAsMargin = value;
				}






			};
		}
	}
}
#endif

