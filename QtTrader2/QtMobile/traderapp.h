#ifndef TRADERAPP_H
#define TRADERAPP_H
#include<manager.h>

#include <QSettings>
#include <QQuickStyle>

class TraderApp
{
public:
    static TraderApp* getApp();
    static void deleteApp();
    void initApp();
    QStringList getAvailableStyles();
private :
    TraderApp();
    static TraderApp* m_traderApp;


};

#endif // TRADERAPP_H
