#ifndef MERCHANTMANAGER_H
#define MERCHANTMANAGER_H

#include <QDebug>
#include <QSettings>
#include "merchant.h"
#include "Common/downloadmanager.h"

class MerchantManager: public QObject
{
    Q_OBJECT
public:
    MerchantManager();
    ~MerchantManager();
    static MerchantManager* getInstance();    
    bool isSetMerchant();
    void excute();
    void setCurrentMerchant(QString code);
    void updateCurrentMerchantResouce();
    Merchant* getMerchant(QString code);
    Merchant* getCurrentMerchant();
    bool isExist(QString code);
    Q_INVOKABLE QString getCurrentMerchantPath();
    Q_INVOKABLE QString getImagePath(QString imageName);
    void setChangingStatus(bool value){ _changing = value; }
    void unzip(QString resourceZipFile, QString destDirPath);

private slots:
    void merchantListDownSlot(bool isSuccess, QString fileName);
    void merchantResourceSlot(bool isSuccess, QString fileName);

signals:
    void updateMerchantListSignals(bool success);
    void checkMerchantStateSignals(bool needResetMerchant, bool needUpdate);
    void updateCurrentResourceSignals();
    void quitSignal();

private:
    static MerchantManager* _instance;
    QSettings * _settings;
    DownloadManager *m_downloadManager;
    QString _merchantListURL;
    QString _spareMerchantListURL;
    QString _merchantCode;
    int _merchantVersionCode;
    QList<Merchant*> _merchantList;
    bool _changing;
    bool _useSpareMerchantListURL;


    bool updateMerchantList(QString xmlFile);
    void updateCurrentResource(QString resourceZipFile);
};

#endif // MERCHANTMANAGER_H
