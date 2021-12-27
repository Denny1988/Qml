#include "traderapp.h"
#include "log.h"
TraderApp* TraderApp::m_traderApp = 0; //static

TraderApp::TraderApp()
{
    TraderApp::initApp();
}

void TraderApp::initApp(){

    QSettings settings;
    settings.beginGroup("ThemeStyle");
    QString style = QQuickStyle::name();
    if (!style.isEmpty()){
        settings.setValue("style", style);
        QByteArray logMegByteArray = ("TraderApp QSettings style = " + style).toLatin1();
        Log::i(logMegByteArray.data());
    }
    else {
        QQuickStyle::setStyle(settings.value("style").toString());
    }
    settings.endGroup();
}

QStringList TraderApp::getAvailableStyles(){

    return QQuickStyle::availableStyles();
}

TraderApp* TraderApp::getApp()
{
    if(!TraderApp::m_traderApp){
       TraderApp::m_traderApp = new TraderApp();
    }
    return TraderApp::m_traderApp;
}
void TraderApp::deleteApp(){
    if(TraderApp::m_traderApp){
       delete TraderApp::m_traderApp;
    }
    TraderApp::m_traderApp = Q_NULLPTR;
}
