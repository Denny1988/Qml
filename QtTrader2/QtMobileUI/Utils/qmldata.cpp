#include "qdebug.h"
#include "qmldata.h"
#include "stdafx.h"
#include "Merchant/merchantmanager.h"
#include "Theme/thememanager.h"

QmlData::QmlData(QObject *parent) : QObject(parent)
{
    this->m_title = "";
    this->m_view_x = 0;//for padding
    this->m_view_y = g_ViewInfo->getHeaderTitleMenuHeight();
    this->m_url = "http://10.3.191.1/iExchange/TradingConsoleSLReport/ReportViewerOnMobile.aspx?accountId=343C9F0D-2BE3-4B9C-AA65-B675C04A8016&tradeDayBegin=2019-01-01&tradeDayEnd=2019-01-05&language=CHS&reportType=Statement&userId=286D5936-F1C3-4057-8712-00CBE26DE157&companyPath=DEM";
}

QmlData::~QmlData()
{
    qDebug() << "destruct---------------- QmlData::~QmlData()";
}

void QmlData::loadFinsh()
{
    qDebug() << " loadFinsh()";
}

void QmlData::jscallme(QString jsonStr)
{
    emit getDeliveryTimeSignal(jsonStr);
}

int QmlData::getViewX()
{
    return this->m_view_x;
}

int QmlData::getViewY()
{
    return this->m_view_y;
}

int QmlData::getQmlWidth()
{
    return g_ViewInfo->getScreenWidth();
}

int QmlData::getQmlHeight()
{
    return g_ViewInfo->getScreenHeight() - g_ViewInfo->getHeaderTitleMenuHeight();
}

void QmlData::getCloseQmlPage()
{
    emit closeQmlPage();
}

QString QmlData::getTitle()
{
    return m_title;
}

int QmlData::getTitleBarHeight()
{
    return g_ViewInfo->getHeaderTitleMenuHeight();
}

QString QmlData::getTitleBarBgColor()
{
    return ThemeManager::GetInstance()->getFontColorValue("QML_TitleBarBgColor");
}

QString QmlData::getTitleBarColor()
{
    return ThemeManager::GetInstance()->getFontColorValue("QML_TitleBarColor");
}

int QmlData::getTitleBackHeight()
{
    return g_ViewInfo->getMenuIconHeight();
}

int QmlData::getTitleBackTop()
{
    return (g_ViewInfo->getHeaderTitleMenuHeight() - g_ViewInfo->getMenuIconHeight())/2;
}

QString QmlData::getTitleBackPicPath()
{
    QString path = QString("file:///%1/Images/back.png").arg(MerchantManager::getInstance()->getCurrentMerchantPath());
    qDebug() << path;
    return path;
}

QString QmlData::getTitleBackText()
{
    return g_Languages.Tr("Back", "DialogWindow");
}

int QmlData::getTitleBarPixelSize()
{
    return g_ViewInfo->getOriginalDefaultPixelSize();
}

QString QmlData::url() const
{
    return this->m_url;
}

void QmlData::setUrl(QString url)
{
    this->m_url = url;
}

void QmlData::setPosition(int x, int y)
{
    this->m_view_x = x;
    this->m_view_y = y;
    emit this->dataChanged();
}

void QmlData::setTitle(QString value)
{
    m_title = value;
}
