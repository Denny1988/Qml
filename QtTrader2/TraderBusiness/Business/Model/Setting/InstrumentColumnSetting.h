#ifndef BUSINESS_MODEL_SETTING_INSTRUMENTCOLUMNSETTING_H
#define BUSINESS_MODEL_SETTING_INSTRUMENTCOLUMNSETTING_H
#include <QDomNode>
#include "Common/Util/Convert_Common.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class InstrumentColumnSetting
			{
			private:
				bool _showHigh;
				bool _showLow;
				bool _showOpen;
				bool _showPrevClose;
				bool _showChange;
				bool _showInterestRateBuy;
				bool _showInterestRateSell;
				bool _showFilledVolume;
				bool _showTotalFilledVolume;
			public:
				void update(const QDomNode& xmlNode)
				{
					QDomNamedNodeMap map = xmlNode.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "ShowHigh")
							this->_showHigh= Common::toBool(value);
						else if(name == "ShowLow")
							this->_showLow= Common::toBool(value);
						else if(name == "ShowOpen")
							this->_showOpen= Common::toBool(value);
						else if(name == "ShowPrevClose")
							this->_showPrevClose= Common::toBool(value);
						else if(name == "ShowChange")
							this->_showChange= Common::toBool(value);
						else if(name == "ShowInterestRateBuy")
							this->_showInterestRateBuy= Common::toBool(value);
						else if(name == "ShowInterestRateSell")
							this->_showInterestRateSell= Common::toBool(value);
						else if(name == "ShowFilledVolume")
							this->_showFilledVolume= Common::toBool(value);
						else if(name == "ShowTotalFilledVolume")
							this->_showTotalFilledVolume= Common::toBool(value);
					}
				}

			public:	
				bool getShowHigh() const
				{
					return _showHigh;
				}

				bool getShowLow() const
				{
					return _showLow;
				}

				bool getShowOpen() const
				{
					return _showOpen;
				}

				bool getShowPrevClose() const
				{
					return _showPrevClose;
				}

				bool getShowChange() const
				{
					return _showChange;
				}

				bool getShowInterestRateBuy() const
				{
					return _showInterestRateBuy;
				}

				bool getShowInterestRateSell() const
				{
					return _showInterestRateSell;
				}

				bool getShowFilledVolume() const
				{
					return _showFilledVolume;
				}

				bool getShowTotalFilledVolume() const
				{
					return _showTotalFilledVolume;
				}

			};
		}
	}
}
#endif

