#include "LoginSetting.h"
#include "EncryptHelper.h"
#include <QCoreApplication>
#include <commonhelper.h>
/*
ini里面内容格式
[type1]
key1 = value1
key2 = value2
[type2]
key3 = value3
key4 = value4
*/

namespace Common
{
	LoginSetting* LoginSetting::getInstance()
	{
		if(!_instance)
		{
			_instance = new LoginSetting();
		}
		return _instance;
	}

	LoginSetting* LoginSetting::_instance = Q_NULLPTR;

	LoginSetting::LoginSetting()
	{
        #if defined(RUNTIME_OS_WIN32) || defined( RUNTIME_OS_MACX)
        QString applicationDirPath = QtFramwork::CommonHelper::getAppDataDirPath();;
        QString fileName = "%1/LoginSetting.ini";
        this->_settings = new QSettings(fileName.arg(applicationDirPath), QSettings ::IniFormat);
        #endif

	}

    void LoginSetting::createSettingByPath(QString path)
    {
        QString fileName = QString("%1/LoginSetting.ini").arg(path);
        this->_settings = new QSettings(fileName, QSettings ::IniFormat);
    }

	bool LoginSetting::saveLoginSetting(QSharedPointer<Struct::LoginSettingStruct> loginSetting)
	{
        #if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
                this->_settings->setValue("LoginSetting/UserName", loginSetting->_loginName);
                QString pwd = EncryptHelper::encrypt(loginSetting->_loginName, loginSetting->_loginPassword);//加密
                this->_settings->setValue("LoginSetting/Pwd", pwd);
                this->_settings->setValue("LoginSetting/Number", loginSetting->_loginPassword.length());
        #else
                this->_settings->setValue("LoginSetting/UserName", loginSetting->_isUserNameRemembered ? loginSetting->_loginName : "");
                if(loginSetting->_isPasswordRemembered)
                {
                    QString pwd = EncryptHelper::encrypt(loginSetting->_loginName, loginSetting->_loginPassword);//加密
                    this->_settings->setValue("LoginSetting/Pwd", pwd);
                    this->_settings->setValue("LoginSetting/Number", loginSetting->_loginPassword.length());
                }
        #endif
		this->_settings->setValue("LoginSetting/IsUserNameRemembered", loginSetting->_isUserNameRemembered);
		this->_settings->setValue("LoginSetting/IsPasswordRemembered", loginSetting->_isPasswordRemembered);
		this->_settings->setValue("LoginSetting/Language", loginSetting->_currentLanguage);
		this->_settings->setValue("LoginSetting/Server", loginSetting->_currentServer);
		this->_settings->setValue("LoginSetting/Connection", loginSetting->_currentConnection);
        this->_settings->setValue("LoginSetting/ConnectionValue", loginSetting->_currentConnectionValue);
		this->_settings->setValue("LoginSetting/IsUseDefined", loginSetting->_isUseDefined);
		this->_settings->setValue("LoginSetting/UseDefinedConnection", loginSetting->_useDefinedConnection);		
		return true;
	}

	void LoginSetting::savePasswordSetting(QString pwd, int length)
	{
		auto loginSettingStruct = this->getLoginSetting();
        #if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
            this->_settings->setValue("LoginSetting/Pwd", pwd);
            this->_settings->setValue("LoginSetting/Number", length);
        #else
		if(loginSettingStruct->_isPasswordRemembered)
		{
			this->_settings->setValue("LoginSetting/Pwd", pwd);
			this->_settings->setValue("LoginSetting/Number", length);
		}
        #endif
	}

	void LoginSetting::clearUserAndPasswordSetting()
	{
		this->_settings->setValue("LoginSetting/UserName", "");
		this->_settings->setValue("LoginSetting/Pwd", "");
	}

	QSharedPointer<Struct::LoginSettingStruct> LoginSetting::getLoginSetting()
	{
		QSharedPointer<Struct::LoginSettingStruct> loginSettingStruct(new Struct::LoginSettingStruct);
		loginSettingStruct->_isUserNameRemembered = this->_settings->value("LoginSetting/IsUserNameRemembered", false).toBool();
		loginSettingStruct->_isPasswordRemembered = this->_settings->value("LoginSetting/IsPasswordRemembered", false).toBool();
        #if defined(RUNTIME_OS_IPHONE) || defined(RUNTIME_OS_ANDROID)
            loginSettingStruct->_loginName = this->_settings->value("LoginSetting/UserName","").toString();
            QString userName = loginSettingStruct->_loginName;
            int pwdLength = this->_settings->value("LoginSetting/Number","0").toString().toInt();
            QString encryptPwd = this->_settings->value("LoginSetting/Pwd","").toString();
            QString pwd = (userName.isEmpty() || encryptPwd.isEmpty()) ? "" : EncryptHelper::decrypt(userName, encryptPwd);//解密
            if(pwdLength!=0 && pwd.length()!=pwdLength)
                pwd = pwd.mid(0, pwdLength);
            loginSettingStruct->_loginPassword = pwd;
        #else
        loginSettingStruct->_loginName = loginSettingStruct->_isUserNameRemembered ? this->_settings->value("LoginSetting/UserName","").toString() : "";
		if(loginSettingStruct->_isPasswordRemembered)
		{
			QString userName = loginSettingStruct->_loginName;
			int pwdLength = this->_settings->value("LoginSetting/Number","0").toString().toInt();
			QString encryptPwd = this->_settings->value("LoginSetting/Pwd","").toString();
			QString pwd = (userName.isEmpty() || encryptPwd.isEmpty()) ? "" : EncryptHelper::decrypt(userName, encryptPwd);//解密
			if(pwdLength!=0 && pwd.length()!=pwdLength)
				pwd = pwd.mid(0, pwdLength);
			loginSettingStruct->_loginPassword = pwd;
		}
		else
			loginSettingStruct->_loginPassword = "";
        #endif
		loginSettingStruct->_currentLanguage = this->_settings->value("LoginSetting/Language","").toString();
		loginSettingStruct->_currentServer = this->_settings->value("LoginSetting/Server","").toString();
		loginSettingStruct->_currentConnection = this->_settings->value("LoginSetting/Connection", "").toString();
        loginSettingStruct->_currentConnectionValue = this->_settings->value("LoginSetting/ConnectionValue", "").toString();
		loginSettingStruct->_isUseDefined = this->_settings->value("LoginSetting/IsUseDefined", false).toBool();
		loginSettingStruct->_useDefinedConnection = this->_settings->value("LoginSetting/UseDefinedConnection", "").toString();
		return loginSettingStruct;
	}

	QString LoginSetting::getLanguage()
	{
		return this->_settings->value("LoginSetting/Language","").toString();
	}

	void LoginSetting::setCurrentLanguage(QString language)
	{
		this->_settings->setValue("LoginSetting/Language", language);
	}
	
	void LoginSetting::setValue(const QString & type, const QString & key, const QVariant & value)
	{
		QString valueKey = "%1/%2";
		this->_settings->setValue(valueKey.arg(type,key), value);
	}

    QString LoginSetting::getValue(const QString & type, const QString & key)
    {
        QString valueKey = QString("%1/%2").arg(type, key);
        if(this->_settings->contains(valueKey))
            return this->_settings->value(valueKey).toString();
        else
            return "";
    }

	void LoginSetting::removeValue(const QString & type, const QString & key)
	{
		QString valueKey = "%1/%2";
		this->_settings->remove(valueKey.arg(type,key));
	}

	bool LoginSetting::getUseTraderQuotationServer()
	{
		return this->_settings->value("LoginSetting/UseTraderQuotationServer", false).toBool();
	}

	void LoginSetting::saveAutoUpdateSetting(QSharedPointer<Struct::AutoUpdateSettingStruct> loginSetting)
	{
		this->_settings->setValue("AutoUpdateSetting/AutoUpdateServer", loginSetting->_autoUpdateServer);
		this->_settings->setValue("AutoUpdateSetting/AutoUpdateServerPort", loginSetting->_autoUpdateServerPort);
		this->_settings->setValue("AutoUpdateSetting/ReTranslateTimes", loginSetting->_reTranslateTimes);
		this->_settings->setValue("AutoUpdateSetting/Organization", loginSetting->_organization);
		this->_settings->setValue("AutoUpdateSetting/OrganizationNewVersion", loginSetting->_organization_NewVersion);
		this->_settings->setValue("AutoUpdateSetting/AppName", loginSetting->_appName);
	}

	QSharedPointer<Struct::AutoUpdateSettingStruct> LoginSetting::getAutoUpdateSetting()
	{
		QSharedPointer<Struct::AutoUpdateSettingStruct> autoUpdateSettingStruct(new Struct::AutoUpdateSettingStruct);
		autoUpdateSettingStruct->_autoUpdateServer = this->_settings->value("AutoUpdateSetting/AutoUpdateServer", "").toString();
		autoUpdateSettingStruct->_autoUpdateServerPort = this->_settings->value("AutoUpdateSetting/AutoUpdateServerPort", "").toString();
		autoUpdateSettingStruct->_reTranslateTimes = this->_settings->value("AutoUpdateSetting/ReTranslateTimes", "").toString();
		autoUpdateSettingStruct->_organization = this->_settings->value("AutoUpdateSetting/Organization", "").toString();
		autoUpdateSettingStruct->_organization_NewVersion = this->_settings->value("AutoUpdateSetting/OrganizationNewVersion", "").toString();
		autoUpdateSettingStruct->_appName = this->_settings->value("AutoUpdateSetting/AppName", "").toString();
		return autoUpdateSettingStruct;
	}
	bool LoginSetting::getFromTraderStartUp()
	{
		return this->_settings->value("LoginSetting/FromTraderStartUp", false).toBool();;
	}

	void LoginSetting::setFromTraderStartUp()
	{
		this->_settings->setValue("LoginSetting/FromTraderStartUp", false);
		this->_settings->sync();
	}
}
