#ifndef THEMESETTINGSTRUCT_H
#define THEMESETTINGSTRUCT_H
#include <QSettings>
#include <QMap>
#include <QUuid>
#include <QSettings> 
#include "traderbusiness_global.h"


struct ThemeSettingStruct
{
    QString colorTheme;
    QString colorPriceUp;
    QString colorPriceDown;
};

class ThemeSetting
{
public:
    static ThemeSetting* getInstance();
    void setValue(const QString & type, const QString & key, const QVariant & value);
    void removeValue(const QString & type, const QString & key);
    void createSetting(QString userPath);
    QString getValue(QString key);
    QString getValue(const QString & type, const QString & key);
    void saveThemeSettingStruct(ThemeSettingStruct themeSettingStruct);
    ThemeSettingStruct getThemeSettingStruct();

private:
    ThemeSetting();
    static ThemeSetting* _instance;
    QSettings * _userSettings;
    QSettings * _defaultSettings;
    ThemeSettingStruct _themeSettingStruct;
};
#endif
