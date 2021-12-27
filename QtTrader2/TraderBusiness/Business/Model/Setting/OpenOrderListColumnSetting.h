#ifndef BUSINESS_MODEL_SETTING_OPENORDERLISTCOLUMNSETTING_H
#define BUSINESS_MODEL_SETTING_OPENORDERLISTCOLUMNSETTING_H
#include <QDomNode>
#include "Common/Util/Convert_Common.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class OpenOrderListColumnSetting
			{
			private:
				bool _showLimitCloseOrderSummary;
				bool _showStopCloseOrderSummary;
				bool _showAutoLimitPrice;
				bool _showAutoStopPrice;
				bool _showTradePLFloat;
				bool _showUnrealisedSwap;
				bool _showInterestPLFloat;
				bool _showStoragePLFloat;
				bool _showCommissionSum;
				bool _showInterestRate;
				bool _showCurrencyRate;

			public:
				void update(const QDomNode& xmlNode)
				{
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "ShowLimitCloseOrderSummary")
							this->_showLimitCloseOrderSummary= Common::toBool(value);
						else if(name == "ShowStopCloseOrderSummary")
							this->_showStopCloseOrderSummary= Common::toBool(value);
						else if(name == "ShowAutoLimitPrice")
							this->_showAutoLimitPrice= Common::toBool(value);
						else if(name == "ShowAutoStopPrice")
							this->_showAutoStopPrice= Common::toBool(value);
						else if(name == "ShowTradePLFloat")
							this->_showTradePLFloat= Common::toBool(value);
						else if(name == "ShowUnrealisedSwap")
							this->_showUnrealisedSwap= Common::toBool(value);
						else if(name == "ShowInterestPLFloat")
							this->_showInterestPLFloat= Common::toBool(value);
						else if(name == "ShowStoragePLFloat")
							this->_showStoragePLFloat= Common::toBool(value);
						else if(name == "ShowCommissionSum")
							this->_showCommissionSum= Common::toBool(value);
						else if(name == "ShowInterestRate")
							this->_showInterestRate= Common::toBool(value);
						else if(name == "ShowCurrencyRate")
							this->_showCurrencyRate= Common::toBool(value);
					}
				}

			public:	
				bool getShowLimitCloseOrderSummary() const
				{
					return _showLimitCloseOrderSummary;
				}

				bool getShowStopCloseOrderSummary() const
				{
					return _showStopCloseOrderSummary;
				}

				bool getAutoLimitPrice() const
				{
					return _showAutoLimitPrice;
				}

				bool getShowAutoStopPrice() const
				{
					return _showAutoStopPrice;
				}

				bool getShowTradePLFloat() const
				{
					return _showTradePLFloat;
				}

				bool getShowUnrealisedSwap() const
				{
					return _showUnrealisedSwap;
				}

				bool getShowInterestPLFloat() const
				{
					return _showInterestPLFloat;
				}

				bool getShowStoragePLFloat() const
				{
					return _showStoragePLFloat;
				}

				bool getShowCommissionSum() const
				{
					return _showCommissionSum;
				}

				bool getShowInterestRate() const
				{
					return _showInterestRate;
				}

				bool getShowCurrencyRate() const
				{
					return _showCurrencyRate;
				}

			};
		}
	}
}
#endif

