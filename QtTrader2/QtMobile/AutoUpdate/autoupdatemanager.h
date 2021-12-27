#ifndef AUTOUPDATEMANAGER_H
#define AUTOUPDATEMANAGER_H

#include <QWidget>

class AutoUpdateManager: public QObject
{
    Q_OBJECT
public:
    AutoUpdateManager();
    ~AutoUpdateManager();
    static AutoUpdateManager* getInstance();
    bool checkUpdate(int appVersionCode, QString appversionName, QString minAppversionName, bool forceUpdate, QString updateUrl1, QString updateUrl2, QWidget *parent);

private:
    bool isLessThanMinVersion(QString version, QString minVersion);

signals:
    void startUpdateSignal();

private:
    static AutoUpdateManager* _instance;
    int _appVersionCode;
    QString _appversionName;
};

#endif // AutoUpdateManager
