#ifndef BUSINESS_MODEL_SETTING_CALCULATEQUOTATIONDEFAULTSETTING_H
#define BUSINESS_MODEL_SETTING_CALCULATEQUOTATIONDEFAULTSETTING_H
#include <quuid.h>
#include "Common/Enum/InstrumentEnum.h"
#include "QDecNumber.h"
#include "Common/Util/Convert_Common.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class CalculateQuotationDefaultSetting
			{
			private:
				QUuid  _instrumentId;
				int _spreadPoints;
				int _autoAdjustPoints;
				Common::Enum::PriceType::PriceType _priceType;
				int _numeratorUnit;
				int _denominator;
				bool _caculateChangeWithDenominator;
			public:
				void update(const QDomNode& xmlInstalmentPolicyDetail)
				{
					QDomNamedNodeMap map = xmlInstalmentPolicyDetail.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "InstrumentId")
							this->_instrumentId = QUuid(value);
						else if(name == "SpreadPoints")
							this->_spreadPoints= value.toInt();
						else if(name == "AutoAdjustPoints")
							this->_autoAdjustPoints = value.toInt();
						else if(name == "PriceType")
							this->_priceType = static_cast<Common::Enum::PriceType::PriceType>(value.toInt());
						else if(name == "NumeratorUnit")
							this->_numeratorUnit = value.toInt();
						else if(name == "Denominator")
							this->_denominator = value.toInt();
						else if(name == "CaculateChangeWithDenominator")
							this->_caculateChangeWithDenominator = Common::toBool(value);
					}
				}

			public:

				QUuid getInstrumentId() const
				{
					return _instrumentId;
				}
				
				int getSpreadPoints() const
				{
					return _spreadPoints;
				}

				int getAutoAdjustPoints() const
				{
					return _autoAdjustPoints;
				}
								
				Common::Enum::PriceType::PriceType getPriceType() const
				{
					return _priceType;
				}

				int getNumeratorUnit() const
				{
					return _numeratorUnit;
				}

				int getDenominator() const
				{
					return _denominator;
				}
				
				bool getCaculateChangeWithDenominator() const
				{
					return _caculateChangeWithDenominator;
				}
				

				void setAutoAdjustPoints(const int& value)
				{
					_autoAdjustPoints = value;
				}

				void setCaculateChangeWithDenominator(const bool& value)
				{
					_caculateChangeWithDenominator = value;
				}

				void setDenominator(const int& value)
				{
					_denominator = value;
				}

				void setInstrumentId(const QUuid& value)
				{
					_instrumentId = value;
				}

				void setNumeratorUnit(const int& value)
				{
					_numeratorUnit = value;
				}

				void setPriceType(const Common::Enum::PriceType::PriceType& value)
				{
					_priceType = value;
				}

				void setSpreadPoints(const int& value)
				{
					_spreadPoints = value;
				}
			};
		}
	}
}
#endif
