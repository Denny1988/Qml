#include "CustomerSetting.h"
#include <QCoreApplication>
#include "Common/TraderBaseInfo.h"
#include "Common/TraderBaseInfo.h"
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
	CustomerSetting* CustomerSetting::getInstance()
	{
		if(!_instance)
		{
			_instance = new CustomerSetting;
		}
		return _instance;
	}

	CustomerSetting* CustomerSetting::_instance = Q_NULLPTR;

	CustomerSetting::CustomerSetting()
	{		
	}

    void CustomerSetting::createSettingByPath(QString path)
    {
        QString fileName = QString("%1/CustomerSetting.ini").arg(path);
        this->_settings = new QSettings(fileName, QSettings ::IniFormat);
        this->_settingMap.clear();
    }

	void CustomerSetting::createSetting(QString userName)
	{		
        QString appDataDirPath = QtFramwork::CommonHelper::getAppDataDirPath();
        QString fileName = QString("%1/User/%2/CustomerSetting.ini").arg(appDataDirPath, userName);
		this->_settings = new QSettings(fileName, QSettings ::IniFormat);
		this->_settingMap.clear();
	}

	void CustomerSetting::setValue(const QString & type, const QString & key, const QVariant & value)
	{
		QString valueKey = "%1/%2";
		this->_settings->setValue(valueKey.arg(type).arg(key), value);
		this->_settingMap.insert(valueKey.arg(type).arg(key),value.toString());
	}

	void CustomerSetting::removeValue(const QString & type, const QString & key)
	{
		QString valueKey = "%1/%2";
		this->_settings->remove(valueKey.arg(type).arg(key));
	}

	QString CustomerSetting::getValue(QString key)
	{
		if(this->_settingMap.contains(key))
		{
			return _settingMap.value(key);
		}
		else
		{
            if(this->_settings->contains(key))
                return this->_settings->value(key).toString();
            else
                return "";
		}
	}

	QString CustomerSetting::getValue(const QString & type, const QString & key)
	{
		QString valueKey = QString("%1/%2").arg(type, key);
		return getValue(valueKey);
	}

	bool CustomerSetting::getIsSaveLot(QUuid accountId, QUuid instrumentId)
	{
		QString valueKey = "%1/%2/IsSaveLot";
		return this->getValue(valueKey.arg(accountId.toString(), instrumentId.toString())) == "true" ? true : false;
	}

	bool CustomerSetting::getIsSaveMovePips(QUuid accountId, QUuid instrumentId)
	{
		QString valueKey = "%1/%2/IsSaveMovePips";
		return this->getValue(valueKey.arg(accountId.toString(), instrumentId.toString())) == "true" ? true : false;
	}

	double CustomerSetting::getLot(QUuid accountId, QUuid instrumentId)
	{
		QString valueKey = "%1/%2/Lot";
		return this->getValue(valueKey.arg(accountId.toString(), instrumentId.toString())).toDouble();
	}

	int CustomerSetting::getMovePips(QUuid accountId, QUuid instrumentId)
	{
		QString valueKey = "%1/%2/MovePips";
		return this->getValue(valueKey.arg(accountId.toString(), instrumentId.toString())).toInt();
	}

	void CustomerSetting::saveLot(QUuid accountId, QUuid instrumentId, bool isSave, double lot)
	{
		QString valueKey = "%1/%2";
		this->setValue(valueKey.arg(accountId.toString(), instrumentId.toString()), "IsSaveLot", isSave);
		this->setValue(valueKey.arg(accountId.toString(), instrumentId.toString()), "Lot",  lot);
	}

	void CustomerSetting::saveMovePips(QUuid accountId, QUuid instrumentId, bool isSave, int movePips)
	{
		QString valueKey = "%1/%2";
		this->setValue(valueKey.arg(accountId.toString(), instrumentId.toString()), "IsSaveMovePips", isSave);
		this->setValue(valueKey.arg(accountId.toString(), instrumentId.toString()), "MovePips",  movePips);
	}

	bool CustomerSetting::isShowDisclaimerDialog()
	{
		int option = this->_settings->value("Disclaimer/Option", 3).toInt();
		if(option == 1)
			return false;
		else if(option == 2)
		{
			return Common::TraderBaseInfo::getInstance()->getLoginSession();
		}
		else
			return true;
	}

	int CustomerSetting::getDisclaimerOption()
	{
		return this->_settings->value("Disclaimer/Option", 3).toInt();
	}

	void CustomerSetting::setDisclaimerOption(int option)
	{
		this->setValue("Disclaimer", "Option", option);
	}

	bool CustomerSetting::getCheckAccountStatus(QUuid accountId)
	{
		QString valueKey = "Account/%1";
		QString checkStr = this->getValue(valueKey.arg(accountId.toString()));
		return (checkStr == "true" || checkStr.length() ==0) ? true : false;
	}

	void CustomerSetting::setCheckAccountStatus(QUuid accountId, bool checkValue)
	{
		this->setValue("Account", accountId.toString(), checkValue);
	}

	void CustomerSetting::setCheckAccountsStatus(QList<QUuid> accountIds, bool checkValue)
	{
		for (int i = 0; i < accountIds.count(); i++)
		{
			this->setValue("Account", accountIds[i].toString(), checkValue);
		}
	}

	void CustomerSetting::saveBORealTimePriceSettings(BORealTimePriceSettingStruct BORealTimePriceSettings)
	{
		this->_settings->setValue("BORealTimeSetting/bDrawGrid", BORealTimePriceSettings.bDrawGrid);
		this->_settings->setValue("BORealTimeSetting/bDrawBarStroke", BORealTimePriceSettings.bDrawBarStroke);
		this->_settings->setValue("BORealTimeSetting/bDrawCrosshair", BORealTimePriceSettings.bDrawCrosshair);
		this->_settings->setValue("BORealTimeSetting/isShowXaxis", BORealTimePriceSettings.isShowXaxis);
		this->_settings->setValue("BORealTimeSetting/isShowBOText", BORealTimePriceSettings.isShowBOText);

		this->_settings->setValue("BORealTimeSetting/colorBarStroke", BORealTimePriceSettings.colorBarStroke);
		this->_settings->setValue("BORealTimeSetting/colorBackground", BORealTimePriceSettings.colorBackground);
		this->_settings->setValue("BORealTimeSetting/coordinateBackground", BORealTimePriceSettings.coordinateBackground);
		this->_settings->setValue("BORealTimeSetting/colorBOText", BORealTimePriceSettings.colorBOText);
		this->_settings->setValue("BORealTimeSetting/colorForeground", BORealTimePriceSettings.colorForeground);
		this->_settings->setValue("BORealTimeSetting/colorGrid", BORealTimePriceSettings.colorGrid);
		this->_settings->setValue("BORealTimeSetting/colorCrosshair", BORealTimePriceSettings.colorCrosshair);
		this->_settings->setValue("BORealTimeSetting/colorBullish", BORealTimePriceSettings.colorBullish);
		this->_settings->setValue("BORealTimeSetting/colorBearish", BORealTimePriceSettings.colorBearish);
	}
		
	BORealTimePriceSettingStruct CustomerSetting::getBORealTimePriceSettings()
	{
		BORealTimePriceSettingStruct BORealTimePriceSettings;
		BORealTimePriceSettings.bDrawGrid = this->_settings->value("BORealTimeSetting/bDrawGrid", true).toBool();
		BORealTimePriceSettings.bDrawBarStroke = this->_settings->value("BORealTimeSetting/bDrawBarStroke", true).toBool();
		BORealTimePriceSettings.bDrawCrosshair = this->_settings->value("BORealTimeSetting/bDrawCrosshair", true).toBool();
		BORealTimePriceSettings.isShowXaxis = this->_settings->value("BORealTimeSetting/isShowXaxis", true).toBool();
		BORealTimePriceSettings.isShowBOText = this->_settings->value("BORealTimeSetting/isShowBOText", true).toBool();
		if(this->_settings->contains("BORealTimeSetting/colorBarStroke"))
			BORealTimePriceSettings.colorBarStroke = this->_settings->value("BORealTimeSetting/colorBarStroke").toString();
		if(this->_settings->contains("BORealTimeSetting/colorBackground"))
			BORealTimePriceSettings.colorBackground = this->_settings->value("BORealTimeSetting/colorBackground").toString();
		if(this->_settings->contains("BORealTimeSetting/coordinateBackground"))
			BORealTimePriceSettings.coordinateBackground = this->_settings->value("BORealTimeSetting/coordinateBackground").toString();		
		if(this->_settings->contains("BORealTimeSetting/colorBOText"))
			BORealTimePriceSettings.colorBOText = this->_settings->value("BORealTimeSetting/colorBOText").toString();
		if(this->_settings->contains("BORealTimeSetting/colorForeground"))
			BORealTimePriceSettings.colorForeground = this->_settings->value("BORealTimeSetting/colorForeground").toString();
		if(this->_settings->contains("BORealTimeSetting/colorCrosshair"))
			BORealTimePriceSettings.colorCrosshair = this->_settings->value("BORealTimeSetting/colorCrosshair").toString();
		if(this->_settings->contains("BORealTimeSetting/colorGrid"))
			BORealTimePriceSettings.colorGrid = this->_settings->value("BORealTimeSetting/colorGrid").toString();
		if(this->_settings->contains("BORealTimeSetting/colorBullish"))
			BORealTimePriceSettings.colorBullish = this->_settings->value("BORealTimeSetting/colorBullish").toString();
		if(this->_settings->contains("BORealTimeSetting/colorBearish"))
			BORealTimePriceSettings.colorBearish = this->_settings->value("BORealTimeSetting/colorBearish").toString();

		return BORealTimePriceSettings;
	}

    bool CustomerSetting::getIsUseDefaultLayout()
    {
        bool isUseDefaultLayout;
        QString valueKey = QString("Layout/IsUseDefaultLayout");
        if(this->_settings->contains(valueKey))
            isUseDefaultLayout = this->_settings->value(valueKey).toBool();
        else
            isUseDefaultLayout = true;
        if(isUseDefaultLayout)
            this->setValue("Layout", "IsUseDefaultLayout", false);
        return isUseDefaultLayout;
    }

    void CustomerSetting::saveUserSetChartColorSign(bool value)
    {
        this->setValue("Layout", "UserSetChartColorSign", value);
    }

    bool CustomerSetting::getUserSetChartColorSign()
    {
        QString valueKey = QString("Layout/UserSetChartColorSign");
        if(this->_settings->contains(valueKey))
            return this->_settings->value(valueKey).toBool();
        else
            return false;
    }

    void CustomerSetting::saveUserSetRealTimeChartColorSign(bool value)
    {
        this->setValue("Layout", "UserSetRealTimeChartColorSign", value);
    }

    bool CustomerSetting::getUserSetRealTimeChartColorSign()
    {
        QString valueKey = QString("Layout/UserSetRealTimeChartColorSign");
        if(this->_settings->contains(valueKey))
            return this->_settings->value(valueKey).toBool();
        else
            return false;
    }
}
