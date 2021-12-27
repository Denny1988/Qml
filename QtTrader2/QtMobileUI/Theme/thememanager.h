#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QString> 
#include <QList>
#include <QStringList>
#include <QMap>
#include <QPen>
#include <QSharedPointer>
#include <QDomDocument>
#include <QMessageBox>
#include "Common/Enum/InstrumentEnum.h"
#include "Common/Enum/PlaceEnum.h"

using namespace Common::Enum;

class ThemeItem
{
public:
    ThemeItem();
    ThemeItem(const QString& themeName,const QString& qssPath,
              const QString& colorFileName,const QString& description);
public:
    QString GetThemeName() const;
    QString GetThemeDir() const;
    QString GetDescription() const;
    QString GetColorFileName() const;
private:
    QString	m_ThemeName;
    QString m_QssPath;
    QString m_Description;
    QString m_ColorFileName;
};


class ThemeManager
{

private:
    ThemeManager();
    ~ThemeManager();
public:
    static ThemeManager*	GetInstance();

private:
    void initializeQPenCache();
public:
    void			SetCurrentTheme(const QString& themeName);

    ThemeItem	    GetCurrentTheme() const;
    ThemeItem       GetThemeByName(const QString& themeName);
    bool            ApplyTheme();
    bool            ApplyWidgetTheme(QWidget* widget);
    QList<ThemeItem>   GetThemeList();
    bool            LoadFontSetting();
    bool			LoadThemeSetting(QString language, bool isStart = false);
    bool			ChangeTheme(const QString& themeName);
    QDomDocument    getThemeDocument(const QString& xmlPath);
    QString         getLanguage(const QString& language,QDomElement languageElement);
    QColor          getFontColor(const QString& fontColorKey);
    QString          getFontColorValue(const QString& fontColorKey);
    QString    getNumberColorValue(QString value);
    QString getNumberColorValueForPlacingPage(QString value);
    QPen getQPen(const QString& key);
    void addStyleDialog(QWidget* widget);
    void removeStyleDialog(QWidget* widget);
    void addNoneStyleDialog(QDialog* widget);
    void addNoneStyleWidget(QWidget* widget);
    void setFontgroundColor(QWidget* widget,const QString& colorString);

    QString    getPriceUpDownColorValue(bool isUp, bool isFromMainPage);
    QString    getBuySellFontColorValue(bool isBuy, bool isFromMainPage);
    QString    getQuickPlacing_Price_Color(PriceTrend::PriceTrend priceTrend, bool isFont);
    QString    getPhaseFontColorValue(Phase::Phase phase);
    QString    getQuickPlacingDisabledFontColorValue();

private:
    static	ThemeManager*		m_Instance;
    ThemeItem			        m_CurrentTheme;
    QList<ThemeItem>	        m_ThemeList;
    QString                     m_AppDir;
    QString                     m_DialogQssDir;
    QString						m_Language;

    QMap<QString,QMap<QString,QString>> _FontListMap;
    QHash<QString,QPen>  _QPenCache;
    QList<QWidget*> _styleWidgets;
    QString						m_QssContent;

};

#endif //MAINTHEME_H

