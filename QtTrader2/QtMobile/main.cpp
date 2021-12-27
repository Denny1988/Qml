#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>

#include <qqmlengine.h>
#include <qqmlcontext.h>
#include <qqml.h>

#include <QtQuick/qquickitem.h>
#include <QtQuick/qquickview.h>

#include <log.h>
#include <manager.h>
#include<viewmodel/account.h>
#include<viewmodel/orderTreeModel.h>
#include<viewmodel/makeorder.h>
#include "stdafx.h"
#include "Utils/commonapihelper.h"
#include "Theme/themesetting.h"
#include "Theme/thememanager.h"
#include "Common/AppSetting.h"
#include "Common/LoginSetting.h"
#include "Common/TraderBaseInfo.h"
#include "Merchant/merchantmanager.h"


#include <QFile>
#include <QCoreApplication>
#include <QDir>
#include <QList>
#include <QFileInfoList>
#include <QDebug>
#include <QTextCodec>

#include <QDir>
#include "Merchant/splashscreen.h"
#include <QStandardPaths>


QMutiLanguages	g_Languages;
ViewInfo*	g_ViewInfo;


bool readConfigXml(const QDomDocument& domDoc)
{
    QDomNodeList merchantNodeList = domDoc.elementsByTagName("Merchant");
    for (int i = 0; i < merchantNodeList.count(); i++)
    {
        QDomNode merchantNode = merchantNodeList.item(i);
        Common::TraderBaseInfo::getInstance()->setIsPrivateMerchant(Common::toBool(merchantNode.toElement().attribute("IsPrivate")));
        Common::TraderBaseInfo::getInstance()->setMerchantCode(merchantNode.toElement().attribute("MerchantCode"));
    }

    QDomNodeList weChatNodeList = domDoc.elementsByTagName("WeChat");
    for (int i = 0; i < weChatNodeList.count(); i++)
    {
        QDomNode weChatNode = weChatNodeList.item(i);
        Common::TraderBaseInfo::getInstance()->setWXAPPID(weChatNode.toElement().attribute("WXAPPID"));
        Common::TraderBaseInfo::getInstance()->setTemplateCode(weChatNode.toElement().attribute("TemplateCode"));
    }
    return true;
}

bool readConfigXml()
{
    QFile file(":/Resources/Config.xml");
    if (!(file.open(QIODevice::ReadOnly)))
    {
        return false;
    }
    QDomDocument domDoc;
    if (!(domDoc.setContent(&file)))
    {
        file.close();
        return false;
    }
    file.close();
    return readConfigXml(domDoc);
}


#define Android_App

#ifdef Android_App

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //init loger
    QLoggingCategory::setFilterRules(QStringLiteral("qt.log.debug=false"));
    qSetMessagePattern("%{category} %{message}");

    g_ViewInfo = ViewInfo::getInstance();
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    //get config xml
    readConfigXml();

    QString strAppDir = CommonApiHelper::getAppDirectoryPath();
    Common::AppSetting::getInstance()->createSettingByPath(strAppDir);
    Common::LoginSetting::getInstance()->createSettingByPath(strAppDir);
    QString currentLanguage = Common::LoginSetting::getInstance()->getLanguage();
    if(currentLanguage.length() == 0)
    {
#if defined (RUNTIME_OS_IPHONE)
        QString systemLanguage = CommonApiHelper::getOSLanguage();
        qDebug()<< " OSLanguage:" << systemLanguage;
        if(systemLanguage.contains("zh-Hans"))
            currentLanguage = "CHS";
        else if(systemLanguage.contains("zh-Hant"))
            currentLanguage = "CHT";
        else
            currentLanguage = "ENG";
#else
        QString systemLanguage = CommonApiHelper::getOSLanguage();
        qDebug()<< " OSLanguage:" << systemLanguage;
        if(systemLanguage == "zh-Hans")
            currentLanguage = "CHS";
        else if(systemLanguage == "zh-Hant")
            currentLanguage = "CHT";
        else
            currentLanguage = "ENG";
#endif
        Common::LoginSetting::getInstance()->setCurrentLanguage(currentLanguage);
    }

    Manager* manager = Manager::getInstance();

    qDebug() << "Mobile SplashScreen Start " << QDateTime::currentDateTimeUtc();
    //start
    QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "Mobile SplashScreen Start";
    //qputenv("QT_USE_ANDROID_NATIVE_DIALOGS", "0");
    //SplashScreen splash;
    //splash.exec();
    //splash.deleteLater();

    if(MerchantManager::getInstance()->isSetMerchant())
    {
        QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "Mobile SplashScreen end";
        qDebug() << "Mobile SplashScreen end " << QDateTime::currentDateTimeUtc();

        ThemeSetting::getInstance();
        ThemeManager::GetInstance()->LoadThemeSetting(currentLanguage, true);
        Common::TraderBaseInfo::getInstance()->setLanguage(currentLanguage);
        QString langId;
        if(currentLanguage == "CHS")
            langId = QString("0x0804");
        else if(currentLanguage == "CHT")
            langId = QString("0x0404");
        else
            langId = QString("0x0409");
        QString languagePath = "%1/Language/MutiLanguagesConfig.xml";
        QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "Mobile Languages Start";
        bool ok;
        g_Languages = QMutiLanguages(languagePath.arg(MerchantManager::getInstance()->getCurrentMerchantPath()), langId.toInt(&ok, 16));

        //init data
        manager->init();
        qmlRegisterType<Account>("ViewModel", 1, 0, "Account");

        TraderApp *p_TraderApp = TraderApp::getApp();

        //setContextProperty
        QQmlApplicationEngine engine;
        engine.rootContext()->setContextProperty("accountViewModel", manager->getAccount());
        //QQmlContext *context = new QQmlContext(engine.rootContext());
        //context->setContextProperty("accountViewModel", manager.getAccount());

        OrderTreeModel * P_OrderTreeModel = new OrderTreeModel();
        MakeOrder *p_makeOrderModel = new MakeOrder();

        engine.rootContext()->setContextProperty("g_Languages", &g_Languages);
        engine.rootContext()->setContextProperty("g_ViewInfo", g_ViewInfo);
        engine.rootContext()->setContextProperty("merchantManager", MerchantManager::getInstance());
        engine.rootContext()->setContextProperty("orderTreeModel", P_OrderTreeModel);
        engine.rootContext()->setContextProperty("availableStyles", p_TraderApp->getAvailableStyles());
        engine.rootContext()->setContextProperty("cppMakeOrderModel",  p_makeOrderModel);


    //    ChartViewManager* chartViewManager = ChartViewManager::getInstance();
    //    engine.rootContext()->setContextProperty("chartViewManager", chartViewManager);
        //qmlRegisterType<ChartViewQmlAdapter>("ChartViewQmlAdapter", 1, 0, "ChartViewQmlAdapter");

        engine.addImportPath("qrc:///qml");
        //load main qml
        engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
        if (engine.rootObjects().isEmpty()){
                return -1;
        }

    }
    return app.exec();

}

#else
//this is for test now:
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    Manager* manager = Manager::getInstance();
    manager->init();
    //qmlRegisterType<Account>("ViewModel", 1, 0, "Account");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("accountViewModel", manager->getAccount());

    engine.load(QUrl(QLatin1String("qrc:/qml/MyItem.qml")));

    if (engine.rootObjects().isEmpty()){
            return -1;
    }
    else {

    }
    return app.exec();
}

#endif
