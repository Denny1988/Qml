#include "thememanager.h"

#include <QFile>
#include <QTextStream>
#include "qfile.h"  
#include "qdebug.h"  
#include <QDomDocument>  
#include "qtextcodec.h" 
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include "Common/Constants/ColorBrushConstants.h"
#include "Common/CustomerSetting.h"
#include "Merchant/merchantmanager.h"
#include "themesetting.h"
#include "Common/TraderBaseInfo.h"
#include "Utils/screenmanager.h"

using namespace Common;
using namespace Common::Constants;

ThemeItem::ThemeItem()
{

}
ThemeItem::ThemeItem(const QString& themeName,const QString& qssPath,
                     const QString& colorFileName,const QString& description)
{
    m_ThemeName = themeName;
    m_ColorFileName = colorFileName;
    m_QssPath = qssPath;
    m_Description = description;
}
QString ThemeItem::GetThemeName() const
{
    return m_ThemeName;
}
QString ThemeItem::GetThemeDir() const
{
    return m_QssPath;
}

QString ThemeItem::GetDescription() const
{
    return m_Description;
}

QString ThemeItem::GetColorFileName() const
{
    return m_ColorFileName;
}
//ThemeManager  樣式管理類
ThemeManager* ThemeManager::m_Instance=Q_NULLPTR;
ThemeManager* ThemeManager::GetInstance()
{
     if (Q_NULLPTR==m_Instance)
     {
         m_Instance = new ThemeManager;
     }
     return m_Instance;
}
ThemeManager::ThemeManager()
{
    this->initializeQPenCache();
}
ThemeManager::~ThemeManager()
{
    qDebug() << "destruct---------------- ThemeManager::~ThemeManager()";
}
void ThemeManager::SetCurrentTheme(const QString& themeName)
{
    for(int i=0;i<m_ThemeList.count();i++)
    {
        if(m_ThemeList[i].GetThemeName()==themeName)
        {
            //SetThemeSelected(theme);
        }
    }
}

ThemeItem ThemeManager::GetCurrentTheme() const
{
    return m_CurrentTheme;
}

ThemeItem ThemeManager::GetThemeByName(const QString& themeName)
{
    ThemeItem item = m_ThemeList[0];
    for(int i=0;i<m_ThemeList.count();i++)
    {
        if(m_ThemeList[i].GetThemeName()==themeName)
        {
            item =  m_ThemeList[i];
        }
    }
    return item;
}
QDomDocument ThemeManager::getThemeDocument(const QString& xmlPath)
{
    QDir dir;
    QDomDocument doc;
    //QString xmlFile = m_AppDir + "Qss/FontSetting.xml";
    QFile file(xmlPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug() << "open for do erro";
        file.close();
        return doc;
    }
    if(!doc.setContent(&file))
    {
        //qDebug() << "setcontent for do error";
        file.close();
    }
    return doc;
}

bool ThemeManager::LoadFontSetting()
{
    QString colorFileName = m_CurrentTheme.GetColorFileName();
    QString themeName = m_CurrentTheme.GetThemeName();
    if(this->_FontListMap.contains(themeName)) return true;
    QDomDocument doc = this->getThemeDocument(m_AppDir + "/Theme/" + colorFileName);
    if(!doc.hasChildNodes())
    {
        QMessageBox msgBox;
        msgBox.setText("Open font color setting failed!");
        msgBox.exec();
        return false;
    }
    QDomElement root = doc.documentElement();
    QDomNodeList itemNodes = root.childNodes();

    QMap<QString,QString> fontMap;
    for(int i=0;i< itemNodes.count();i++)
    {
        QDomElement themeElement = itemNodes.at(i).toElement();

        QString fontkey = themeElement.attribute("key");
        QString fontValue = themeElement.attribute("value");

        fontMap.insert(fontkey,fontValue);
    }
    this->_FontListMap.insert(themeName,fontMap);
    return true;
}

bool ThemeManager::LoadThemeSetting(QString language, bool isStart)
{
    m_Language = language.toUpper();
    m_ThemeList.clear();
    this->m_AppDir = MerchantManager::getInstance()->getCurrentMerchantPath();
    QString xmlFile = m_AppDir + "/Theme/ThemeSettings.xml";
    QDomDocument doc = this->getThemeDocument(xmlFile);
    if(!doc.hasChildNodes())
    {
        QMessageBox msgBox;
        msgBox.setText("Open theme setting failed!");
        msgBox.exec();
        return false;
    }
    QDomElement root = doc.documentElement();
    QDomNodeList themeNodes = root.childNodes();
    QString userSettingThemeName = ThemeSetting::getInstance()->getValue("ThemeSetting/colorTheme");
    for(int i = 0; i <themeNodes.count();i++)
    {
        QDomElement themeElement= themeNodes.item(i).toElement();
        qDebug() <<themeElement.tagName();
        QString themeName = themeElement.attribute("Name");
        QString fileName = themeElement.attribute("FileName");
        QString colorFileName = themeElement.attribute("ColorFileName");
        QString isSelected = themeElement.attribute("IsSelected");

        QDomElement languageElement = themeElement.firstChild().toElement();

        QString discription = this->getLanguage(language,languageElement);
        QString qssPath = m_AppDir + "/Theme/" + fileName;
        m_DialogQssDir = m_AppDir + "/Theme/White.qss";
        ThemeItem themeItem = ThemeItem(themeName,qssPath,colorFileName,discription);        
        if(userSettingThemeName == themeName && userSettingThemeName!="")
            this->m_CurrentTheme = themeItem;
        else
        {
            if(isSelected == "true" && isStart)
                this->m_CurrentTheme = themeItem;
        }
        m_ThemeList.append(themeItem);
    }
    if(!this->ChangeTheme(m_CurrentTheme.GetThemeName()))
    {
        QMessageBox msgBox;
        msgBox.setText("Load theme failed!");
        msgBox.exec();
    }
    ThemeSetting::getInstance()->setValue("ThemeSetting", "colorTheme", m_CurrentTheme.GetThemeName());
    return true;
}

QString ThemeManager::getLanguage(const QString& language, QDomElement languageElement)
{
    if(languageElement.hasAttribute(language))
    {
        return languageElement.attribute(language);
    }
    return "";
}

QList<ThemeItem> ThemeManager::GetThemeList()
{
    return m_ThemeList;
}

bool ThemeManager::ApplyTheme()
{
    //解码字节流QString::fromLatin1
    ThemeItem currentThem = m_CurrentTheme;
    QString qssPath = m_CurrentTheme.GetThemeDir();
    QFile file(qssPath);
    if(!file.open(QFile::ReadOnly))return false;
    this->m_QssContent = QString::fromLatin1(file.readAll());
    //qApp->setStyleSheet(styleSheet);
    foreach (QWidget* item,this->_styleWidgets)
    {
        item->setStyleSheet(this->m_QssContent);
    }
    return true;
}
bool ThemeManager::ApplyWidgetTheme(QWidget* widget)
{
    //解码字节流QString::fromLatin1
    QFile file(m_DialogQssDir);
    if(!file.open(QFile::ReadOnly))return false;
    this->m_QssContent = QString::fromLatin1(file.readAll());

    widget->setStyle(Q_NULLPTR);
    widget->setStyleSheet(this->m_QssContent);

    foreach (QObject* child,widget->children())
    {
        QWidget* childItem = dynamic_cast<QWidget*>(child);
        if(Q_NULLPTR!=childItem)
        {
            childItem->setStyleSheet(this->m_QssContent);
        }
    }
    return true;
}

bool ThemeManager::ChangeTheme(const QString& themeName)
{
    //if(themeName == "NoneTheme")
    //{
    //	qApp->setStyleSheet(Q_NULLPTR);
    //	return true;
    //}
    if(Common::TraderBaseInfo::getInstance()->getCurrentTheme() == themeName)
        return true;
    Common::TraderBaseInfo::getInstance()->setCurrentTheme(themeName);
    ThemeItem themeItem = this->GetThemeByName(themeName);
    m_CurrentTheme = themeItem;
    this->LoadFontSetting();

    this->initializeQPenCache();
    //g_App->setCurrentTheme(themeName);
    return this->ApplyTheme();
}

QPen ThemeManager::getQPen(const QString& key)
{
    if(this->_QPenCache.contains(key))
    {
        return this->_QPenCache.value(key);
    }
    return QPen(key);
}

void ThemeManager::initializeQPenCache()
{
    this->_QPenCache.clear();
    QPen normalPen(getFontColor("NoChange"));
    QPen whitePen(Qt::white);
    QPen blackPen(Qt::black);
    QPen redPen(Qt::red);
    QPen greenPen(Qt::darkGreen);
    QPen goldPen(Qt::darkYellow);
    this->_QPenCache.insert(ColorBrushConstants::NormalFontBrush,normalPen);
    this->_QPenCache.insert(ColorBrushConstants::WhiteBrush,whitePen);
    this->_QPenCache.insert(ColorBrushConstants::BlackBrush,blackPen);
    this->_QPenCache.insert(ColorBrushConstants::RedBrush,redPen);
    this->_QPenCache.insert(ColorBrushConstants::GreenBrush,greenPen);
    this->_QPenCache.insert(ColorBrushConstants::GoldBrush,goldPen);
}

QColor ThemeManager::getFontColor(const QString& fontColorKey)
{
    QString themeKey = m_CurrentTheme.GetThemeName();
    if(_FontListMap.contains(themeKey))
    {
        QMap<QString,QString> fontMap = _FontListMap.value(themeKey);
        if(fontMap.contains(fontColorKey))
        {
            QString fontColor = fontMap.value(fontColorKey);
            return QColor(fontColor);
        }
    }
    return QColor(Qt::white);
}

QString ThemeManager::getFontColorValue(const QString& fontColorKey)
{
    QString themeKey = m_CurrentTheme.GetThemeName();
    if(_FontListMap.contains(themeKey))
    {
        QMap<QString,QString> fontMap = _FontListMap.value(themeKey);
        if(fontMap.contains(fontColorKey))
        {
            return fontMap.value(fontColorKey);
        }
    }
    return "";
}

QString ThemeManager::getPriceUpDownColorValue(bool isUp, bool isFromMainPage)
{
    if(isFromMainPage)
    {
        if(isUp)
            return getFontColorValue(m_Language == "CHS" ? "MainPage_PriceDown" : "MainPage_PriceUp");
        else
            return getFontColorValue(m_Language == "CHS" ? "MainPage_PriceUp" : "MainPage_PriceDown");
    }
    else
    {
        if(isUp)
            return getFontColorValue(m_Language == "CHS" ? "PriceDown" : "PriceUp");
        else
            return getFontColorValue(m_Language == "CHS" ? "PriceUp" : "PriceDown");
    }
}

QString ThemeManager::getBuySellFontColorValue(bool isBuy, bool isFromMainPage)
{
    if(isFromMainPage)
    {
        if(isBuy)
            return getFontColorValue("MainPage_BuyFontColor");
        else
            return getFontColorValue("MainPage_SellFontColor");
    }
    else
    {
        if(isBuy)
            return getFontColorValue("BuyFontColor");
        else
            return getFontColorValue("SellFontColor");
    }
}

QString ThemeManager::getNumberColorValue(QString value)
{
    return getFontColorValue(value.startsWith('-') ? "NegativeNumber" : "PositiveNumber");
}

QString ThemeManager::getNumberColorValueForPlacingPage(QString value)
{
    return getFontColorValue(value.startsWith('-') ? "NegativeNumber" : "PositiveNumberForPlacingPage");
}


QString ThemeManager::getQuickPlacing_Price_Color(PriceTrend::PriceTrend priceTrend, bool isFont)
{
    if (priceTrend == PriceTrend::Up)
    {
        if(isFont)
            return this->getFontColorValue(m_Language == "CHS" ? "QuickPlacing_Price_Dow_FontColor" : "QuickPlacing_Price_Up_FontColor");
        else
            return this->getFontColorValue(m_Language == "CHS" ? "QuickPlacing_Price_Dow_BgColor" : "QuickPlacing_Price_Up_BgColor");
    }
    else if (priceTrend == PriceTrend::Down)
    {
        if(isFont)
            return this->getFontColorValue(m_Language == "CHS" ? "QuickPlacing_Price_Up_FontColor" : "QuickPlacing_Price_Dow_FontColor");
        else
            return this->getFontColorValue(m_Language == "CHS" ? "QuickPlacing_Price_Up_BgColor" : "QuickPlacing_Price_Dow_BgColor");
    }
    else
    {
        if(isFont)
            return this->getFontColorValue("QuickPlacing_FontColor");
        else
            return this->getFontColorValue("QuickPlacing_BgColor");
    }
}

QString  ThemeManager::getQuickPlacingDisabledFontColorValue()
{
    return this->getFontColorValue("QuickPlacing_Disabled_FontColor");
}

QString ThemeManager::getPhaseFontColorValue(Phase::Phase phase)
{
    switch (phase)
    {
    case Common::Enum::Phase::Placing:
        return getFontColorValue("PlacingOrder");
        break;
    case Common::Enum::Phase::Placed:
        return getFontColorValue("PlacedOrder");
        break;
    case Common::Enum::Phase::Canceled:
        return getFontColorValue("CancelOrder");
        break;
    case Common::Enum::Phase::Executed:
        return getFontColorValue("ExecutedOrder");
        break;
    case Common::Enum::Phase::Completed:
        return getFontColorValue("ExecutedOrder");
        break;
    case Common::Enum::Phase::Deleted:
        return getFontColorValue("DeleteOrder");
        break;
    default:
        return ColorBrushConstants::NormalFontBrush;
        break;
    }
}

void ThemeManager::addStyleDialog(QWidget* widget)
{
    if(!this->_styleWidgets.contains(widget))
    {
        this->_styleWidgets.append(widget);
        widget->setStyleSheet(this->m_QssContent);
    }
}

void ThemeManager::removeStyleDialog(QWidget* widget)
{
    if(this->_styleWidgets.contains(widget))
    {
        this->_styleWidgets.removeOne(widget);
    }
}

void ThemeManager::addNoneStyleDialog(QDialog* widget)
{
    //this->ApplyWidgetTheme(widget);
}

void ThemeManager::addNoneStyleWidget(QWidget* widget)
{
    //this->ApplyWidgetTheme(widget);
}

void ThemeManager::setFontgroundColor(QWidget* widget,const QString& colorString)
{
    QString style = "color:%1;margin:1px;";
    style = style.arg(colorString);
    widget->setStyleSheet(style);
}
