#include "Common/TraderBaseInfo.h"
#include "Common/UtcTimeManager.h"
#include "Common/CustomerSetting.h"
#include "Common/SignalForwarder.h"
#include <QCoreApplication>
#include <QDebug>
#include <QSysInfo>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QList>

namespace Common
{
	TraderBaseInfo* TraderBaseInfo::_instance=Q_NULLPTR;
	TraderBaseInfo* TraderBaseInfo::getInstance()
	{
		if (Q_NULLPTR==_instance)
		{
			_instance = new TraderBaseInfo;
		}
		return _instance;
	}

	TraderBaseInfo::TraderBaseInfo()
	{
		this->_languageKey = "ENG";
		_appDir = QCoreApplication::applicationDirPath();
		_hideSelectALLQCheckBox = false;
		_initFailCount = 0;
		_companyName="";
		this->_isConnect = true;
		this->_onlyExistsBOInstrument = false;
		_isShowOrderPage = true;
		this->_useLongPrice = false;
		this->_exceedCommonPriceNumber = 0;
		this->_isRefreshSystem = false;
		this->_directLogin = false;
		this->_regexPattern = "";
		this->_tipInfo = "";
        this->_logoutByNoOperation = 0;
        this->_isTraderIdleTimeout = false;
        this->_isPrivateMerchant = true;
        this->_isWXlogin = false;
        this->_isNewRegistId = false;
        this->_fastTraderForClose = -1;
        this->_loginGuest = false;
        this->_useBrowser = false;
	}

	TraderBaseInfo::~TraderBaseInfo()
	{

	}

	QString TraderBaseInfo::getLanguage() const
	{
		return this->_languageKey;
	}

	void TraderBaseInfo::setLanguage(QString languageKey)
	{
		this->_languageKey = languageKey;
	}
	QString TraderBaseInfo::getAppPath() const
	{
		return this->_appDir;
	}

	QString TraderBaseInfo::getUserName() const
	{
		return this->_userName;
	}

	QString TraderBaseInfo::getPassword() const
	{
		return this->_password;
	}

	QUuid TraderBaseInfo::getUserId() const
	{
		return this->_userId;
	}

	void TraderBaseInfo::setUserNameAndPassword(const QString& userName,const QString& password)
	{
		this->_userName = userName;
		this->_password = password;
	}

	void TraderBaseInfo::setUserInfor(const QString& userName,const QUuid& userId,const QString& password, const QString& serverDateTime)
	{
		this->_userId = userId;
		this->_userName = userName;
		this->_password = password;
		this->_serverDateTime = serverDateTime;
	}

        void TraderBaseInfo::clearUserId()
        {
            this->_userId = QUuid();
        }

	bool TraderBaseInfo::getIsShowOrderPage() const
	{
		return this->_isShowOrderPage;
	}

	void TraderBaseInfo::setIsShowOrderPage(bool isShowOrderPage)
	{
		this->_isShowOrderPage = isShowOrderPage;
	}

	int TraderBaseInfo::getCheckedAccountCount()
	{
		return this->_checkedAccounts.count();
	}

	QList<QUuid> TraderBaseInfo::getCheckedAccounts()
	{
		return this->_checkedAccounts;
	}

	QList<QUuid> TraderBaseInfo::getTotalAccounts()
	{
		return this->_totalAccounts;
	}

	QList<QUuid> TraderBaseInfo::getUnCheckedAccounts()
	{
		return this->_unCheckedAccounts;
	}

	void TraderBaseInfo::addOrRemoveCheckedAccount(bool isSelect, QUuid accountId)
	{
		//qDebug() << "addOrRemoveCheckedAccount(bool isSelect, QUuid accountId) : id:" << accountId << " isSelect:" << isSelect;
		if(isSelect)
		{
			if(!this->_checkedAccounts.contains(accountId))
			{
				this->_checkedAccounts.append(accountId);
				if(this->_unCheckedAccounts.contains(accountId))
					this->_unCheckedAccounts.removeOne(accountId);
			}
		}
		else
		{
			this->_checkedAccounts.removeOne(accountId);
			this->_unCheckedAccounts.append(accountId);
		}

		Common::CustomerSetting::getInstance()->setCheckAccountStatus(accountId, isSelect);
		if(this->_checkedAccounts.count() == 1 && this->_checkedAccounts[0]!=this->_currentAccountId)
		{
			this->_currentAccountId = this->_checkedAccounts[0];
		}
		if(this->_checkedAccounts.count() == 0 && this->_totalAccounts.count()!=0)
		{
			this->_currentAccountId = this->_totalAccounts[0];
			this->_checkedAccounts.append(this->_currentAccountId);
			Common::CustomerSetting::getInstance()->setCheckAccountStatus(this->_currentAccountId, isSelect);
		}
	}

	void TraderBaseInfo::addOrRemoveCheckedAccount(bool isSelect, QList<QUuid> accountIds)
	{
		foreach (QUuid accountId, accountIds)
		{
			if(isSelect)
			{
				if(!this->_checkedAccounts.contains(accountId))
				{
					this->_checkedAccounts.append(accountId);
					if(this->_unCheckedAccounts.contains(accountId))
						this->_unCheckedAccounts.removeOne(accountId);
				}
			}
			else
			{
				this->_checkedAccounts.removeOne(accountId);
				this->_unCheckedAccounts.append(accountId);
			}
		}

		Common::CustomerSetting::getInstance()->setCheckAccountsStatus(accountIds, isSelect);
		if(this->_checkedAccounts.count() == 1 && this->_checkedAccounts[0]!=this->_currentAccountId)
		{
			this->_currentAccountId = this->_checkedAccounts[0];
		}

		if(this->_checkedAccounts.count() == 0 && this->_totalAccounts.count()!=0)
		{
			this->_currentAccountId = this->_totalAccounts[0];
			this->_checkedAccounts.append(this->_currentAccountId);
			Common::CustomerSetting::getInstance()->setCheckAccountStatus(this->_currentAccountId, isSelect);
		}

		/*if(this->_totalAccounts.count() > 0)
		{
		QList<QUuid> notExistaccountIds;
		foreach (QUuid accountId, accountIds)
		{
		if(!this->_totalAccounts.contains(accountId))
		{
		notExistaccountIds.append(accountId);
		}
		}
		foreach (QUuid accountId, notExistaccountIds)
		{
		accountIds.removeOne(accountId);
		}
		}

		if(this->_totalAccounts.count() > 0 && isSelect && this->_unCheckedAccounts.count() == 0)
		{
		foreach (QUuid accountId, this->_totalAccounts)
		{
		if(!accountIds.contains(accountId))
		{
		this->_unCheckedAccounts.append(accountId);
		}
		}
		}
		if(this->_totalAccounts.count()!=this->_unCheckedAccounts.count())
		{
		foreach (QUuid accountId, accountIds)
		{
		//qDebug() << "addOrRemoveCheckedAccount(bool isSelect, QList<QUuid> accountIds) : id:" << accountId << " isSelect:" << isSelect;
		if(isSelect)
		{
		if(!this->_checkedAccounts.contains(accountId))
		{
		this->_checkedAccounts.append(accountId);
		if(this->_unCheckedAccounts.contains(accountId))
		this->_unCheckedAccounts.removeOne(accountId);
		}
		}
		else
		{
		this->_checkedAccounts.removeOne(accountId);
		this->_unCheckedAccounts.append(accountId);
		}
		}
		}
		else
		this->_checkedAccounts.clear();
		if(this->_checkedAccounts.count() == 1 && this->_checkedAccounts[0]!=this->_currentAccountId)
		{
		this->_currentAccountId = this->_checkedAccounts[0];
		}

		if(this->_checkedAccounts.count() == 0 && this->_totalAccounts.count()!=0)
		{
		this->_currentAccountId = this->_totalAccounts[0];
		this->_checkedAccounts.append(this->_currentAccountId);
		}*/
	}

	void TraderBaseInfo::addOrRemoveCheckedAccountForLayout(bool isSelect, QList<QUuid> accountIds)
	{
		if(this->_totalAccounts.count() > 0)
		{
			QList<QUuid> notExistaccountIds;
			foreach (QUuid accountId, accountIds)
			{
				if(!this->_totalAccounts.contains(accountId))
				{
					notExistaccountIds.append(accountId);
				}
			}
			foreach (QUuid accountId, notExistaccountIds)
			{
				accountIds.removeOne(accountId);
			}
		}

		if(accountIds.count() == 0) 
		{
			if(this->_checkedAccounts.count() == 0 && this->_totalAccounts.count()!=0)
			{
				this->_currentAccountId = this->_totalAccounts[0];
				this->_checkedAccounts.append(this->_currentAccountId);
			}
			return;
		}

		if(this->_totalAccounts.count() > 0 && isSelect && this->_unCheckedAccounts.count() == 0)
		{
			foreach (QUuid accountId, this->_totalAccounts)
			{
				if(!accountIds.contains(accountId))
				{
					this->_unCheckedAccounts.append(accountId);
				}
			}
		}
		if(this->_totalAccounts.count()!=this->_unCheckedAccounts.count())
		{
			foreach (QUuid accountId, accountIds)
			{
				//qDebug() << "addOrRemoveCheckedAccount(bool isSelect, QList<QUuid> accountIds) : id:" << accountId << " isSelect:" << isSelect;
				if(isSelect)
				{
					if(!this->_checkedAccounts.contains(accountId))
					{
						this->_checkedAccounts.append(accountId);
						if(this->_unCheckedAccounts.contains(accountId))
							this->_unCheckedAccounts.removeOne(accountId);
					}
				}
				else
				{
					this->_checkedAccounts.removeOne(accountId);
					this->_unCheckedAccounts.append(accountId);
				}
			}
		}
		else
			this->_checkedAccounts.clear();
		if(this->_checkedAccounts.count() == 1 && this->_checkedAccounts[0]!=this->_currentAccountId)
		{
			this->_currentAccountId = this->_checkedAccounts[0];
		}

		if(this->_checkedAccounts.count() == 0 && this->_totalAccounts.count()!=0)
		{
			this->_currentAccountId = this->_totalAccounts[0];
			this->_checkedAccounts.append(this->_currentAccountId);
		}
	}

	bool TraderBaseInfo::isCheckedAccount(QUuid accountId)
	{
		return this->_checkedAccounts.contains(accountId);
	}

	void TraderBaseInfo::setTotalAccounts(QList<QUuid> totalAccountIds, QList<QUuid> checkAccountIds)
	{
		this->_totalAccounts = totalAccountIds;
		this->_checkedAccounts = checkAccountIds;
		if(this->_checkedAccounts.count() == 0)		
		{
			this->_checkedAccounts.append(this->_totalAccounts[0]);
			this->_currentAccountId = this->_checkedAccounts[0];	
			Common::CustomerSetting::getInstance()->setCheckAccountStatus(this->_currentAccountId, true);
		}
		else
			this->_currentAccountId = this->_checkedAccounts[0];
		foreach (QUuid accountId, this->_totalAccounts)
		{
			if(!this->_checkedAccounts.contains(accountId))
			{
				this->_unCheckedAccounts.append(accountId);
			}
		}
		/*QList<QUuid> notExistaccountIds;
		foreach (QUuid accountId, this->_unCheckedAccounts)
		{
		if(!this->_totalAccounts.contains(accountId))
		{
		notExistaccountIds.append(accountId);
		}
		}
		foreach (QUuid accountId, notExistaccountIds)
		{
		this->_unCheckedAccounts.removeOne(accountId);
		}

		if(this->_checkedAccounts.count() > 0)
		{		
		notExistaccountIds.clear();
		foreach (QUuid accountId, this->_checkedAccounts)
		{
		if(!this->_totalAccounts.contains(accountId))
		{
		notExistaccountIds.append(accountId);
		}
		}
		foreach (QUuid accountId, notExistaccountIds)
		{
		this->_checkedAccounts.removeOne(accountId);
		}
		}
		if(this->_checkedAccounts.count() == 0)		
		{
		this->_checkedAccounts.append(this->_totalAccounts[0]);
		this->_currentAccountId = this->_checkedAccounts[0];			
		}*/
	}


	void TraderBaseInfo::clearCheckedAccount()
	{
		this->_checkedAccounts.clear();
		this->_unCheckedAccounts.clear();
	}

	QDateTime TraderBaseInfo::getUtcNow()
	{
		return Common::UtcTimeManager::getInstance()->getUtcNow();
	}

	void TraderBaseInfo::setCurrentServer(QString server)
	{
		_server = server;
	}

	QString TraderBaseInfo::getCurrentServer() const
	{
		return _server;
	}

	void TraderBaseInfo::setCurrentLayout(QString currentLayout)
	{
		_currentLayout = currentLayout;
	}

	void TraderBaseInfo::setCurrentTheme(QString currentTheme)
	{
		_currentTheme = currentTheme;
	}

	QString TraderBaseInfo::getCurrentLayout() const
	{
		return _currentLayout;
	}

	QString TraderBaseInfo::getCurrentTheme() const
	{
		return _currentTheme;
	}

	QUuid TraderBaseInfo::getCurrentAccountId() const
	{
		return _currentAccountId;
	}

	void TraderBaseInfo::setCurrentAccountId(QUuid accountId)
	{
		_currentAccountId = accountId;
	}

	QUuid TraderBaseInfo::getCurrentQuotePolicyId() const
	{
		return _currentQuotePolicyId;
	}

	void TraderBaseInfo::setCurrentQuotePolicyId(QUuid quotePolicyId)
	{
		_currentQuotePolicyId = quotePolicyId;
	}

	void TraderBaseInfo::setHideSelectALLQCheckBox(bool value)
	{
		_hideSelectALLQCheckBox = value;
	}

	bool TraderBaseInfo::getHideSelectALLQCheckBox()
	{
		return _hideSelectALLQCheckBox;
	}

	void TraderBaseInfo::setInitFailCount(int value)
	{
		_initFailCount = value;
	}

	void TraderBaseInfo::addInitFailCount()
	{
		_initFailCount++;
	}

	int TraderBaseInfo::getInitFailCount()
	{
		return _initFailCount;
	}

	QString TraderBaseInfo::getCompanyName() const
	{
		return _companyName;
	}

	void TraderBaseInfo::setCompanyName(QString value)
	{
		_companyName = value;
    }

    bool TraderBaseInfo::hasFastTrade(QUuid instrumentId)
    {
        return this->_InstrumentStates.contains(instrumentId);
    }

	bool TraderBaseInfo::getFastTrade(QUuid instrumentId)
	{
		if(this->_InstrumentStates.contains(instrumentId))
		{
			return this->_InstrumentStates[instrumentId]._fastTrade;
		}
		return false;
	}

	void TraderBaseInfo::setFastTrade(QUuid instrumentId, bool fastTrade)
	{
		if(this->_InstrumentStates.contains(instrumentId))
		{
			this->_InstrumentStates[instrumentId]._fastTrade = fastTrade;
		}
		else
		{
			InstrumentStateStruct instrumentStateStruct;
			instrumentStateStruct._fastTrade = fastTrade;
			instrumentStateStruct._closeByFIFO = false;
			instrumentStateStruct._lot = "0";
			instrumentStateStruct._movepips = 0;
			this->_InstrumentStates.insert(instrumentId, instrumentStateStruct);
		}
	}
	
	void TraderBaseInfo::clearFastTrade()
	{
		QMapIterator<QUuid, InstrumentStateStruct> item(this->_InstrumentStates);
		while (item.hasNext())
		{
			item.next();
			this->_InstrumentStates[item.key()]._fastTrade = false;
			Common::CustomerSetting::getInstance()->setValue("FastTrade", item.key().toString(), false);
		}
	}

	bool TraderBaseInfo::getCloseByFIFO(QUuid instrumentId)
	{
		if(this->_InstrumentStates.contains(instrumentId))
		{
			return this->_InstrumentStates[instrumentId]._closeByFIFO;
		}
		return false;
	}

	void TraderBaseInfo::setCloseByFIFO(QUuid instrumentId, bool closeByFIFO)
	{
		if(this->_InstrumentStates.contains(instrumentId))
		{
			this->_InstrumentStates[instrumentId]._closeByFIFO = closeByFIFO;
		}
		else
		{
			InstrumentStateStruct instrumentStateStruct;
			instrumentStateStruct._fastTrade = false;
			instrumentStateStruct._closeByFIFO = closeByFIFO;
			instrumentStateStruct._lot = "0";
			instrumentStateStruct._movepips = 0;
			this->_InstrumentStates.insert(instrumentId, instrumentStateStruct);
		}
	}

	double TraderBaseInfo::getLot(QUuid instrumentId)
	{
		if(this->_InstrumentStates.contains(instrumentId))
		{
			return this->_InstrumentStates[instrumentId]._lot.toDouble();
		}
		return 0.0;
	}

	void TraderBaseInfo::setLot(QUuid instrumentId, QString lot)
	{
		if(this->_InstrumentStates.contains(instrumentId))
		{
			this->_InstrumentStates[instrumentId]._lot = lot;
		}
		else
		{
			InstrumentStateStruct instrumentStateStruct;
			instrumentStateStruct._fastTrade = false;
			instrumentStateStruct._closeByFIFO = false;
			instrumentStateStruct._lot = lot;
			instrumentStateStruct._movepips = 0;
			this->_InstrumentStates.insert(instrumentId, instrumentStateStruct);
		}
	}

	double TraderBaseInfo::getMovePips(QUuid instrumentId)
	{
		if(this->_InstrumentStates.contains(instrumentId))
		{
			return this->_InstrumentStates[instrumentId]._movepips;
		}
		return 0.0;
	}

	void TraderBaseInfo::setMovePips(QUuid instrumentId, double movePips)
	{
		if(this->_InstrumentStates.contains(instrumentId))
		{
			this->_InstrumentStates[instrumentId]._movepips = movePips;
		}
		else
		{
			InstrumentStateStruct instrumentStateStruct;
			instrumentStateStruct._fastTrade = false;
			instrumentStateStruct._closeByFIFO = false;
			instrumentStateStruct._lot = "0";
			instrumentStateStruct._movepips = movePips;
			this->_InstrumentStates.insert(instrumentId, instrumentStateStruct);
		}
	}

	void TraderBaseInfo::clearInstrumentStates()
	{
		this->_InstrumentStates.clear();
	}

	void TraderBaseInfo::setIsConnect(bool isConect)
	{
		this->_isConnect = isConect;
	}

	void TraderBaseInfo::setIsHideInstalment(bool value)
	{
		this->_isHideInstalment = value;
	}

	void TraderBaseInfo::setIsHideRepayment(bool value)
	{
		this->_isHideRepayment = value;
	}

	void TraderBaseInfo::setExceedCommonPriceNumber(int value)
	{
		int lastExceedCommonPriceNumber = this->_exceedCommonPriceNumber;
		if(value > 7) //大于7位的就显示不正常， 价格就显示不到了
		{
			this->_useLongPrice = true;
			this->_exceedCommonPriceNumber = value - 7;
		}
		else
		{
			this->_useLongPrice = false;
			this->_exceedCommonPriceNumber = 0;
		}
		if(lastExceedCommonPriceNumber != this->_exceedCommonPriceNumber)
			Common::SignalForwarder::getInstance()->sendChangeQuickPlacingWidgetSizeSignal(this->_exceedCommonPriceNumber);
	}

	void Common::TraderBaseInfo::setIsRefreshSystem(bool value)
	{
		this->_isRefreshSystem = value;
	}

	void Common::TraderBaseInfo::setDirectLogin(bool directLogin)
	{
		this->_directLogin = directLogin;
	}

	void Common::TraderBaseInfo::setVerificationCodeAndPasswordValidatePolicy(QString verificationCode, int verificationValidTime, QString mobilePhone, bool isPasswordExpired, QString regexPattern, QString tipInfo)
	{
		this->_verificationCode = verificationCode;
		this->_verificationValidTime = verificationValidTime;
		this->_mobilePhone = mobilePhone;
		this->_isPasswordExpired = isPasswordExpired;
		this->_regexPattern = regexPattern;
		this->_tipInfo = tipInfo;
	}

	void Common::TraderBaseInfo::setVerificationCode(QString verificationCode, int verificationValidTime)
	{
		this->_verificationCode = verificationCode;
		this->_verificationValidTime = verificationValidTime;
	}


    QString Common::TraderBaseInfo::getSystemInfo()
    {
        QString systemInfo = "";
#ifdef Q_OS_WIN
         //Windows
        QSysInfo::WinVersion version = QSysInfo::windowsVersion();
        switch (version)
        {
            case QSysInfo::WV_4_0:
                systemInfo = "Operating system version 4.0, corresponds to Windows NT";
                 break;
            case QSysInfo::WV_5_0:
                systemInfo = "Operating system version 5.0, corresponds to Windows 2000";
                break;
            case QSysInfo::WV_5_1:
                systemInfo = "Operating system version 5.1, corresponds to Windows XP";
                break;
            case QSysInfo::WV_5_2:
                systemInfo = "Operating system version 5.2, corresponds to Windows Server 2003, Windows Server 2003 R2, Windows Home Server, and Windows XP Professional x64 Edition";
                break;
            case QSysInfo::WV_6_0:
                systemInfo = "Operating system version 6.0, corresponds to Windows Vista and Windows Server 2008";
                break;
            case QSysInfo::WV_6_1:
                systemInfo = "Operating system version 6.1, corresponds to Windows 7 and Windows Server 2008 R2";
                break;
            case QSysInfo::WV_6_2:
                systemInfo = "Operating system version 6.2, corresponds to Windows 8";
                break;
            case QSysInfo::WV_6_3:
                systemInfo = "Operating system version 6.3, corresponds to Windows 8.1, introduced in Qt 5.2";
                break;
            case QSysInfo::WV_10_0:
                systemInfo = "Operating system version 10.0, corresponds to Windows 10, introduced in Qt 5.5";
                break;
            default:
                systemInfo = "An Windows unknown and currently unsupported platform";
                break;
        }

        //MachineName
        QString machineName = QString(" MachineName:%1").arg(QHostInfo::localHostName());
        systemInfo.append(machineName);

        //IP
        QString ipAddress = "";
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        // use the first non-localhost IPv4 address
        for (int i = 0; i < ipAddressesList.size(); ++i) {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
                ipAddress = ipAddressesList.at(i).toString();
                break;
            }
        }
        // if we did not find one, use IPv4 localhost
        if (ipAddress.length() == 0)
            ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
        QString ipAddressStr = QString(" IPv4 Address:%1").arg(ipAddress);
        systemInfo.append(ipAddressStr);
#endif
#ifdef Q_OS_LINUX
    //Linux

#endif
#ifdef Q_OS_MAC
     //Mac
        QSysInfo::MacVersion  version  = QSysInfo::macVersion();
        switch (version)
        {
            case QSysInfo::MV_9:
                systemInfo = "Mac OS 9";
                 break;
            case QSysInfo::MV_10_0:
                systemInfo = "Mac OS X 10.0";
                break;
            case QSysInfo::MV_10_1:
                systemInfo = "Mac OS X 10.1";
                break;
            case QSysInfo::MV_10_2:
                systemInfo = "Mac OS X 10.2";
                break;
            case QSysInfo::MV_10_3:
                systemInfo = "Mac OS X 10.3";
                break;
            case QSysInfo::MV_10_4:
                systemInfo = "Mac OS X 10.4";
                break;
            case QSysInfo::MV_10_5:
                systemInfo = "Mac OS X 10.5";
                break;
            case QSysInfo::MV_10_6:
                systemInfo = "Mac OS X 10.6";
                break;
            case QSysInfo::MV_10_7:
                systemInfo = "Mac OS X 10.7";
                break;
            case QSysInfo::MV_10_8:
                systemInfo = "OS X 10.8";
                break;
            case QSysInfo::MV_10_9:
                systemInfo = "OS X 10.9";
                break;
            case QSysInfo::MV_10_10:
                systemInfo = "OS X 10.10";
                break;
            case QSysInfo::MV_10_11:
                systemInfo = "OS X 10.11";
                break;
            case QSysInfo::MV_IOS:
                systemInfo = "iOS (any)";
                break;
            case QSysInfo::MV_IOS_4_3:
                systemInfo = "iOS 4.3";
                break;
            case QSysInfo::MV_IOS_5_0:
                systemInfo = "iOS 5.0";
                break;
            case QSysInfo::MV_IOS_5_1:
                systemInfo = "iOS 5.1";
                break;
            case QSysInfo::MV_IOS_6_0:
                systemInfo = "iOS 6.0";
                break;
            case QSysInfo::MV_IOS_6_1:
                systemInfo = "iOS 6.1";
                break;
            case QSysInfo::MV_IOS_7_0:
                systemInfo = "iOS 7.0";
                break;
            case QSysInfo::MV_IOS_7_1:
                systemInfo = "iOS 7.1";
                break;
            case QSysInfo::MV_IOS_8_0:
                systemInfo = "iOS 8.0";
                break;
            case QSysInfo::MV_IOS_8_1:
                systemInfo = "iOS 8.1";
                break;
            case QSysInfo::MV_IOS_8_2:
                systemInfo = "iOS 8.2";
                break;
            case QSysInfo::MV_IOS_8_3:
                systemInfo = "iOS 8.3";
                break;
            case QSysInfo::MV_IOS_8_4:
                systemInfo = "iOS 8.4";
                break;
            case QSysInfo::MV_IOS_9_0:
                systemInfo = "iOS 9.0";
                break;
            default:
                systemInfo = "An Mac or IOS unknown and currently unsupported platform";
                break;
        }
#endif
        QString cpuInfo = QString(" CPU:%1").arg(QSysInfo::currentCpuArchitecture());
        systemInfo.append(cpuInfo);
        return systemInfo;
    }
}
