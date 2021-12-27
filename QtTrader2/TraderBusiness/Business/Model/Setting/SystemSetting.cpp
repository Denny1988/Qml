#include "SystemSetting.h"
#include <qDebug>
#include "Common/Constants/DateTimeConstants.h"
#include "Common/SignalForwarder.h"
#include "Common/Util/Convert_Common.h"

using namespace Common::Constants;

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			SystemSetting::SystemSetting()
			{
				this->_isEmployee = false;
				this->_needSelectAccount = false;
				this->_allowFreeAgent = false;
				this->_DQOrderOutTime = 0;
				this->_DQMaxLotApplyAccount = false;
				this->_showPriceChangedBeforevCloseConfirmWindow = false;
				this->_highBid = false;
				this->_lowBid = false;
				this->_enableTrendSheetChart = false;
				this->_orderBeginTimeDiff = 0;
				this->_isCheckAcceptDQVariation = false;
				this->_nonQuoteVerificationUiDelay = 0;
				this->_isAllowLimitInSpread = false;
				this->_enableResetTelephonePin = false;
				this->_isNoShowAccountStatus = false;
				this->_isShowOpenOrderAndSummary = true;
				this->_isShowPhysical = true;
				this->_showLog = true;
				this->_interestFreeAlertDay = 0;
				this->_showHundredChart = false;
				this->_sptReconfirmDuration = 0;
				this->_priceRow = 0;
				this->_ratioIncludeCredit = false;
				this->_includeFeeOnRiskAction = false;
				this->_boIncludePrincipal = false;
				this->_organizationName = "";
				this->_isSptReconfirm = false;
				this->_openPositionShowTime = false;
				this->_disallowTrade = false;
				this->_allowEditBankAccountInTrader = false;
				this->_enableNetAsset = false;
				this->_traderIdleTimeout = 0;
				this->_maxPasswordReserveCount = 0;
                this->_enableClickTrade = false;                
                this->_tradePLFloatColumnNumber = -1;
                this->_orderCodeColumnNumber = -1;
                this->_accountCodeColumnNumber = -1;
                this->_isUploadClientLog = false;
			}

			SystemSetting::~SystemSetting()
			{
			}					

			void SystemSetting::update(const QDomNode& xmlSystemSetting)
			{
				QDomNamedNodeMap map = xmlSystemSetting.attributes();
				for (int i = 0; i < map.count(); i++)
				{
					QDomNode qDomNode = map.item(i);
					QString name = qDomNode.nodeName();
					QString value = qDomNode.nodeValue();
					if (name == "DQOrderOutTime")
						this->_DQOrderOutTime = value.toInt();
					else if (name == "NeedSelectAccount")
						this->_needSelectAccount = Common::toBool(value);
					else if (name == "OrganizationName")
						this->_organizationName = value;
					else if (name == "TraderIdleTimeout")
						this->_traderIdleTimeout = value.toInt();
					else if (name == "MaxPasswordReserveCount")
						this->_maxPasswordReserveCount = value.toInt();
				}
			}
			

			

		}
	}
}
