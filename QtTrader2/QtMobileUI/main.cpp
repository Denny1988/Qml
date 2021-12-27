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

#define Android_App

#ifdef Android_App

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //init loger
    QLoggingCategory::setFilterRules(QStringLiteral("qt.log.debug=false"));
    qSetMessagePattern("%{category} %{message}");

    //init data
    Manager* manager = Manager::getInstance();
    manager->init();
    qmlRegisterType<Account>("ViewModel", 1, 0, "Account");

    //set Organization
    QCoreApplication::setOrganizationName("Omnicare");
    QCoreApplication::setOrganizationDomain("mobile.omnicare.com");

    //set Application
    QCoreApplication::setApplicationName("Trader");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


    TraderApp *p_TraderApp = TraderApp::getApp();

    //setContextProperty
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("accountViewModel", manager->getAccount());
    //QQmlContext *context = new QQmlContext(engine.rootContext());
    //context->setContextProperty("accountViewModel", manager.getAccount());

    OrderTreeModel * P_OrderTreeModel = new OrderTreeModel();
    MakeOrder *p_makeOrderModel = new MakeOrder();

    engine.rootContext()->setContextProperty("orderTreeModel", P_OrderTreeModel );
    engine.rootContext()->setContextProperty("availableStyles", p_TraderApp->getAvailableStyles() );
    engine.rootContext()->setContextProperty("cppMakeOrderModel",  p_makeOrderModel);


//    ChartViewManager* chartViewManager = ChartViewManager::getInstance();
//    engine.rootContext()->setContextProperty("chartViewManager", chartViewManager);

    //qmlRegisterType<ChartViewQmlAdapter>("ChartViewQmlAdapter", 1, 0, "ChartViewQmlAdapter");

    //
    engine.addImportPath("qrc:///qml");
    //load main qml
    engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty()){
            return -1;
    }

    return app.exec();

}

#else
//this is for test now:
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    Manager manager = Manager::getInstance();
    manager.init();
    //qmlRegisterType<Account>("ViewModel", 1, 0, "Account");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("accountViewModel", manager.getAccount());

    engine.load(QUrl(QLatin1String("qrc:/qml/MyItem.qml")));

    if (engine.rootObjects().isEmpty()){
            return -1;
    }
    else {

    }
    return app.exec();
}

#endif
