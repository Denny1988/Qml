#include "AppSetting.h"
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
	AppSetting* AppSetting::getInstance()
	{
		if(!_instance)
		{
			_instance = new AppSetting();
		}
		return _instance;
	}

	AppSetting* AppSetting::_instance = Q_NULLPTR;

	AppSetting::AppSetting()
	{
#if defined(RUNTIME_OS_WIN32) || defined( RUNTIME_OS_MACX)
        QString applicationDirPath = QtFramwork::CommonHelper::getAppDataDirPath();
		QString fileName = "%1/AppSetting.ini";
		this->_settings = new QSettings(fileName.arg(applicationDirPath), QSettings ::IniFormat);
#endif

	}

    void AppSetting::createSettingByPath(QString path)
    {
        QString fileName = QString("%1/AppSetting.ini").arg(path);
        this->_settings = new QSettings(fileName, QSettings ::IniFormat);
    }

	void AppSetting::setValue(const QString & type, const QString & key, const QVariant & value)
	{
		QString valueKey = "%1/%2";
		this->_settings->setValue(valueKey.arg(type,key), value);
	}

    QVariant AppSetting::getValue(QString key)
    {
        if(this->_settings->contains(key))
            return this->_settings->value(key);
        else
            return QVariant("");
    }

    QVariant AppSetting::getValue(const QString & type, const QString & key)
    {
        QString valueKey = QString("%1/%2").arg(type, key);
        if(this->_settings->contains(valueKey))
            return this->_settings->value(valueKey);
        else
            return QVariant("");
    }

	void AppSetting::removeValue(const QString & type, const QString & key)
	{
		QString valueKey = "%1/%2";
		this->_settings->remove(valueKey.arg(type,key));
	}

	int AppSetting::getBeatInterval()
	{
		return this->_settings->value("Communication/BeatInterval", 2).toInt();
	}

	int AppSetting::getBeatIntervalOnNotReceiveCommand()
	{
		return this->_settings->value("Communication/BeatIntervalOnNotReceiveCommand", 3).toInt();
	}	

	int AppSetting::getBeatIntervalOnReceiveCommand()
	{
		return this->_settings->value("Communication/BeatIntervalOnReceiveCommand", 5).toInt();
	}

	int AppSetting::getHeartbeaterTimeout()
	{
		return this->_settings->value("Communication/HeartbeaterTimeout", 4).toInt();
	}

	int AppSetting::getContinuityHeartbeaterTimeoutCount()
	{
		return this->_settings->value("Communication/ContinuityHeartbeaterTimeoutCount", 2).toInt();
	}

	int AppSetting::getConnectTimeout()
	{
		return this->_settings->value("Communication/ConnectTimeout", 10).toInt();
	}

	int AppSetting::getRecoverTimeout()
	{
		return this->_settings->value("Communication/RecoverTimeout", 5).toInt();
	}

	int AppSetting::getRecoverConnectionMaxCount()
	{
		return this->_settings->value("Communication/RecoverConnectionMaxCount", 3).toInt();
	}

	int AppSetting::getAsyncTimeoutCheckTimerInterval()
	{
		return this->_settings->value("Communication/AsyncTimeoutCheckTimerInterval", 3).toInt();
	}

	int AppSetting::getReGetTradingDataMaxCount()
	{
		return this->_settings->value("Business/ReGetTradingDataMaxCount", 3).toInt();
	}

	bool AppSetting::getContainsMilSInQuotationTime()
	{
		return this->_settings->value("QuotationSetting/ContainsMilSInQuotationTime", false).toBool();
	}

	bool AppSetting::getWriteLog()
	{
		return this->_settings->value("Log/WriteLog", false).toBool();
	}
	
	int AppSetting::getBORealtimeCount()
	{
		return this->_settings->value("Chart/BORealtimeCount", 200).toInt();
	}

	int AppSetting::getSynchronizeTime()
	{
		return this->_settings->value("Communication/SynchronizeTime", 5).toInt();
	}

	int AppSetting::getRefreshViewTimerInterval()
	{
		return this->_settings->value("PageView/RefreshViewTimerInterval", 300).toInt();
	}

	int AppSetting::getWatchWorlds()
	{
		return this->_settings->value("Communication/WatchwordsCount", 16).toInt();
	}

	int AppSetting::getWatchWorldsLessthanCount()
	{
		return this->_settings->value("Communication/WatchWorldsLessthanCount", 5).toInt();
	}

    int AppSetting::getTimeOutFromProvisional()
    {
        return this->_settings->value("Communication/TimeOutFromProvisional", 5).toInt();
    }

	int AppSetting::getProvisionalTimerInterval()
	{
		return this->_settings->value("Communication/ProvisionalTimerInterval", 60).toInt();
    }

	bool AppSetting::getEnableSystemPictureButton()
	{
		return this->_settings->value("PageView/EnableSystemPictureButton", true).toBool();
	}

	bool AppSetting::getIsNeedLoadMultiTradingData()
	{
		return this->_isNeedLoadMultiTradingData;
	}

    void AppSetting::setIsNeedLoadMultiTradingData(bool isNeedLoadMultiTradingData)
	{
		this->_isNeedLoadMultiTradingData = isNeedLoadMultiTradingData;
	}

    bool AppSetting::getIsChangeChartToolbarByChangeSystemTheme()
    {
        return this->_settings->value("PageView/IsChangeChartToolbarByChangeSystemTheme", false).toBool();
    }

    bool AppSetting::getAllUserSetChartColorSign()
    {
        return this->_settings->value("PageView/AllUserSetChartColorSign", false).toBool();
    }

    int AppSetting::getNoQuotationChangeSingleChannelTime()
    {
        return this->_settings->value("Communication/NoQuotationChangeSingleChannelTime", 6).toInt();
    }

}
