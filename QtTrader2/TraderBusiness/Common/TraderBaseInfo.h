#ifndef COMMON_TRADERBASEINFO_H
#define COMMON_TRADERBASEINFO_H

#include <QString>
#include <QList>
#include <QUuid>
#include <QDateTime>
#include <QMap>
#include "Common/Struct/InstrumentStateStruct.h"
#include "traderbusiness_global.h"

using namespace Common::Struct;
namespace Common
{
	class TRADERBUSINESS_EXPORT TraderBaseInfo
	{
		public:
			TraderBaseInfo();
			~TraderBaseInfo();
		//signals:
		public:
			static TraderBaseInfo*	getInstance();

			void setUserInfor(const QString& userName, const QUuid& userId, const QString& password, const QString& serverDateTime);
			void setUserNameAndPassword(const QString& userName,const QString& password);
                        bool isResetLayout() const{return true;}
			QString getUserName() const;
			QString getPassword() const;
			QUuid   getUserId() const;
			QString getAppPath() const;
			QString getLanguage() const;
			bool getIsShowOrderPage() const;
			QDateTime getUtcNow();
			QString getCurrentServer() const;
			QString getCurrentTheme() const;
			QString getCurrentLayout() const;
			QUuid getCurrentAccountId() const;
			QUuid getCurrentQuotePolicyId() const;
			QString getCompanyName() const;
			QString getPathName() { return _pathName;}
			
			void setCompanyName(QString value);
			void setLanguage(QString languageKey);
			void setIsShowOrderPage(bool isShowOrderPage);
			int getCheckedAccountCount();
			void addOrRemoveCheckedAccount(bool isSelect, QUuid accountId);
			void addOrRemoveCheckedAccount(bool isSelect, QList<QUuid> accountIds);
			void addOrRemoveCheckedAccountForLayout(bool isSelect, QList<QUuid> accountIds);			
			bool isCheckedAccount(QUuid accountId);
			QList<QUuid> getCheckedAccounts();
			QList<QUuid> getTotalAccounts();
			QList<QUuid> getUnCheckedAccounts();
			void setTotalAccounts(QList<QUuid> totalAccountIds, QList<QUuid> checkAccountIds);
			void setCurrentServer(QString server);
			void setCurrentTheme(QString theme);
			void setCurrentLayout(QString currentLayout);
			void setCurrentAccountId(QUuid accountId);
			void setCurrentQuotePolicyId(QUuid quotePolicyId);
			void clearCheckedAccount();
			void setHideSelectALLQCheckBox(bool value);
			bool getHideSelectALLQCheckBox();
			void setInitFailCount(int value);
			void addInitFailCount();
			int getInitFailCount();
            bool hasFastTrade(QUuid instrumentId);
            bool getFastTrade(QUuid instrumentId);
			void setFastTrade(QUuid instrumentId, bool fastTrade);
			bool getCloseByFIFO(QUuid instrumentId);
			void setCloseByFIFO(QUuid instrumentId, bool closeByFIFO);
			double getLot(QUuid instrumentId);
			void setLot(QUuid instrumentId, QString lot);
			double getMovePips(QUuid instrumentId);
			void setMovePips(QUuid instrumentId, double movePips);
			void setPathName(QString pathName) { _pathName = pathName;}
			QString getServerDateTime(){ return _serverDateTime; }
			void clearInstrumentStates();
			void setIsConnect(bool isConect);
			bool isConnect() { return _isConnect; }
			void setIsHideInstalment(bool value);
			void setIsHideRepayment(bool value);
			bool getIsHideInstalment() { return _isHideInstalment; }
			bool getIsHideRepayment() { return _isHideRepayment; }
			void startLoginSessionForDisclaimer() { _loginSession = true; }
			void endLoginSessionForDisclaimer() { _loginSession = false; }
			bool getLoginSession() { return _loginSession; }
		    void setIsEmployee(bool isEmployee) { _isEmployee = isEmployee;}
			bool getIsEmployee() { return _isEmployee; }
			void setOnlyExistsBOInstrument(bool onlyExistsBOInstrument) { _onlyExistsBOInstrument = onlyExistsBOInstrument;}
			bool getOnlyExistsBOInstrument() { return _onlyExistsBOInstrument; }
			void clearFastTrade();
			void setExceedCommonPriceNumber(int value);
			bool getUseLongPrice() { return _useLongPrice; }
			int getExceedCommonPriceNumber() { return _exceedCommonPriceNumber; }
			void setIsRefreshSystem(bool value);
			bool getIsRefreshSystem() { return _isRefreshSystem; }
			bool getDirectLogin() { return _directLogin;  }
			void setDirectLogin(bool directLogin);
			void setVerificationCodeAndPasswordValidatePolicy(QString verificationCode, int verificationValidTime, QString mobilePhone, bool isPasswordExpired, QString regexPattern, QString tipInfo);
			QString getVerificationCode() { return _verificationCode; }
			int getVerificationValidTime() { return _verificationValidTime; }
			QString getMobilePhone() { return _mobilePhone; }
			bool getIsPasswordExpired() { return _isPasswordExpired; }
			QString getRegexPattern() { return _regexPattern; }
			QString getTipInfo() { return _tipInfo; }
			void setVerificationCode(QString verificationCode, int verificationValidTime);

			void setEnableNetAsset(bool value) { _enableNetAsset = value; }
			bool getEnableNetAsset() { return _enableNetAsset; }
			void setLastOperationTime(QDateTime value) { _lastOperationTime = value; }
			QDateTime getLastOperationTime() { return _lastOperationTime; }
			void setLogoutByNoOperation(int value) { _logoutByNoOperation = value; }
			int getLogoutByNoOperation() { return _logoutByNoOperation; }
			void setIsTraderIdleTimeout(bool value) { _isTraderIdleTimeout = value; }
			bool getIsTraderIdleTimeout() { return _isTraderIdleTimeout; }
			void setMaxPasswordReserveCount(int value) { _maxPasswordReserveCount = value; }
			int getMaxPasswordReserveCount() { return _maxPasswordReserveCount; }
            void setLoginGuest(bool value) { _loginGuest = value; }
            bool getLoginGuest(){ return _loginGuest; }
            void setLoginGuestName(QString value) { _loginGuestName = value; }
            QString getLoginGuestName(){ return _loginGuestName; }
            QString getSystemInfo();
            double getFontSizeScale(){ return _fontSizeScale; }
            void setFontSizeScale(double value) { _fontSizeScale = value; }
            void setIsPrivateMerchant(bool value) { _isPrivateMerchant = value; }
            bool getIsPrivateMerchant() { return _isPrivateMerchant; }
            void setMerchantCode(QString value) { _merchantCode = value; }
            QString getMerchantCode() { return _merchantCode; }
            void setWXAPPID(QString value) { _WXAPPID = value; }
            QString getWXAPPID() { return _WXAPPID; }
            void setTemplateCode(QString value) { _templateCode = value; }
            QString getTemplateCode() { return _templateCode; }
            void setIsWXlogin(bool value) { _isWXlogin = value; }
            bool getIsWXlogin() { return _isWXlogin; }
            void setWXOpenId(QString value) { _WXOpenId = value; }
            QString getWXOpenId() { return _WXOpenId; }
            void setUserOpenId(QString value) { _userOpenId = value; }
            QString getUserOpenId() { return _userOpenId; }
            void setIsNewRegistId(bool value) { _isNewRegistId = value; }
            bool getIsNewRegistId() { return _isNewRegistId; }
            void setFastTraderForClose(int value) { _fastTraderForClose = value; }
            int getFastTraderForClose() { return _fastTraderForClose; }
            void setOpenAccountUrl(QString value) { _openAccountUrl = value; }
            QString getOpenAccountUrl() { return _openAccountUrl; }
            void setUseBrowser(bool value) { _useBrowser = value; }
            bool getUseBrowser() { return _useBrowser; }
            void setNewsUrl(QString value) { _newsUrl = value; }
            QString getNewsUrl() { return _newsUrl; }
            void setAppVersionCode(QString value) { _appVersionCode = value; }
            QString getAppVersionCode() { return _appVersionCode; }
            void setAppversionName(QString value) { _appversionName = value; }
            QString getAppversionName() { return _appversionName; }
            void setUploadUrl(QString value) { _uploadUrl = value; }
            QString getUploadUrl() { return _uploadUrl; }
            void setTemporarySpace(QString value) { _temporarySpace = value; }
            QString getTemporarySpace() { return _temporarySpace; }
            void setLanguageFromUrl(QString value) { _languageFromUrl = value; }
            QString getLanguageFromUrl() { return _languageFromUrl; }
            void setServerFromUrl(QString value) { _serverFromUrl = value; }
            QString getServerFromUrl() { return _serverFromUrl; }
            void clearUserId();

		private:
			static	TraderBaseInfo*	_instance;
			QString _userName;		
			QUuid   _userId;
			QString _password;	
			QString _appDir;
			QString _languageKey;
			bool _isShowOrderPage;
			QList<QUuid> _checkedAccounts;
			QList<QUuid> _totalAccounts;
			QList<QUuid> _unCheckedAccounts;
			QDateTime _utcNow;
			QString _server;	
			QString _currentLayout;
			QString _currentTheme;
			bool _hideSelectALLQCheckBox;
			int _initFailCount;
			QString _companyName;
			QUuid _currentAccountId;
			QUuid _currentQuotePolicyId;
			QMap<QUuid, InstrumentStateStruct> _InstrumentStates;
			QString _pathName;		
			QString _serverDateTime;
			bool _isConnect;
			bool _isHideInstalment;
			bool _isHideRepayment;
			bool _loginSession;//true=start , false=End
			bool _isEmployee;
			bool _onlyExistsBOInstrument;
			bool _useLongPrice;
			int _exceedCommonPriceNumber;
			bool _isRefreshSystem;
			bool _directLogin;	
			QString _verificationCode;
			int _verificationValidTime;
			QString _mobilePhone;
			bool _isPasswordExpired;
			QString _regexPattern;
			QString _tipInfo;
			bool _enableNetAsset;
			QDateTime _lastOperationTime;
			int _logoutByNoOperation;
			bool _isTraderIdleTimeout;
			int _maxPasswordReserveCount;
            bool _loginGuest;
            QString _loginGuestName;
            double _fontSizeScale;

            bool _isPrivateMerchant;
            QString _merchantCode;
            QString _WXAPPID;
            QString _templateCode;
            bool _isWXlogin;
            QString _WXOpenId;
            QString _userOpenId;
            bool _isNewRegistId;
            int _fastTraderForClose;// -1=none, 0=false, 1= true
            QString _openAccountUrl;
            bool _useBrowser;
            QString _newsUrl;
            QString _appVersionCode;
            QString _appversionName;

            QString _uploadUrl;
            QString _temporarySpace;

            QString _languageFromUrl;
            QString _serverFromUrl;
	};
}
#endif	  
