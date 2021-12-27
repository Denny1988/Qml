#include "themesetting.h"
#include "Common/TraderBaseInfo.h"
#include "Utils/commonapihelper.h"
#include <QStandardPaths>
#include <QDir>
/*
ini里面内容格式
[type1]
key1 = value1
key2 = value2
[type2]
key3 = value3
key4 = value4
*/


ThemeSetting* ThemeSetting::getInstance()
{
    if(!_instance)
    {
        _instance = new ThemeSetting;
    }
    return _instance;
}

ThemeSetting* ThemeSetting::_instance = Q_NULLPTR;

ThemeSetting::ThemeSetting()
{
    QDir dir;
    QString strDir = CommonApiHelper::getAppDirectoryPath();
    QString path = QString("%1/Setting").arg(strDir);
    if(!dir.exists(path))
    {
        dir.mkpath(path);
        QString fileName = QString("%1/ThemeSetting.ini").arg(path);
        this->_userSettings = new QSettings(fileName, QSettings ::IniFormat);
        this->_userSettings->setValue("ThemeSetting/defaultColorTheme", "WhiteTheme");
        this->_userSettings->setValue("ThemeSetting/colorTheme", "");
        this->_userSettings->setValue("ThemeSetting/colorPriceUp", "Green");
        this->_userSettings->setValue("ThemeSetting/colorPriceDown", "Red");
    }
    else
    {
        QString fileName = QString("%1/ThemeSetting.ini").arg(path);
        this->_userSettings = new QSettings(fileName, QSettings ::IniFormat);
    }
}

void ThemeSetting::createSetting(QString userPath)
{
    QString fileName = QString("%1/ThemeSetting.ini").arg(userPath);
    this->_userSettings = new QSettings(fileName, QSettings ::IniFormat);
}

void ThemeSetting::setValue(const QString & type, const QString & key, const QVariant & value)
{
    QString valueKey = "%1/%2";
    this->_userSettings->setValue(valueKey.arg(type).arg(key), value);
}

void ThemeSetting::removeValue(const QString & type, const QString & key)
{
    QString valueKey = "%1/%2";
    this->_userSettings->remove(valueKey.arg(type).arg(key));
}

QString ThemeSetting::getValue(QString key)
{
    QString value="";
    if(this->_userSettings->contains(key))
    {
        value = this->_userSettings->value(key).toString();
    }
    return value;
}

QString ThemeSetting::getValue(const QString & type, const QString & key)
{
    QString valueKey = QString("%1/%2").arg(type, key);
    return getValue(valueKey);
}

void ThemeSetting::saveThemeSettingStruct(ThemeSettingStruct themeSettingStruct)
{
    this->_userSettings->setValue("ThemeSetting/colorTheme", themeSettingStruct.colorTheme);
    this->_userSettings->setValue("ThemeSetting/colorPriceUp", themeSettingStruct.colorPriceUp);
    this->_userSettings->setValue("ThemeSetting/colorPriceDown", themeSettingStruct.colorPriceDown);
    this->_userSettings->sync();
}

ThemeSettingStruct ThemeSetting::getThemeSettingStruct()
{
    ThemeSettingStruct themeSettingStruct;
    themeSettingStruct.colorTheme = this->getValue("ThemeSetting/colorTheme");
    if(themeSettingStruct.colorTheme.length() == 0)
        themeSettingStruct.colorTheme =  this->getValue("ThemeSetting/defaultColorTheme");
    themeSettingStruct.colorPriceUp = this->getValue("ThemeSetting/colorPriceUp");
    if(themeSettingStruct.colorPriceUp.length() == 0)
        themeSettingStruct.colorPriceUp =  "Green";
    themeSettingStruct.colorPriceDown = this->getValue("ThemeSetting/colorPriceDown");
    if(themeSettingStruct.colorPriceDown.length() == 0)
        themeSettingStruct.colorPriceDown =  "Red";
    return themeSettingStruct;
}
