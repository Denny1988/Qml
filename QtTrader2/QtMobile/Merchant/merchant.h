#ifndef MERCHANT_H
#define MERCHANT_H
#include <QObject>
#include <QDomNodeList>

struct LanguageValues
{
  QString chs;
  QString cht;
  QString eng;
};

class Merchant
{

public:
    Merchant(QString code);
    void initialize(QDomNode merchantNode);
    QString getCode();
    QString getName();
    QString getSettingFileUrl();
    int getVersionCode();
    void setName(QString name);
    int getAppVersionCode();
    QString getAppversionName();
    bool getForceUpdate();
    QString getUpdateUrl1();
    QString getUpdateUrl2();
    QString getAndroidMinVersion();
    QString getIOSVersion();
    QString getIOSMinVersion();
    QString getIOSUpdateUrl();

private:
    QString _code;
    QString _settingFileUrl;
    QString _name;
    int _versionCode;
    LanguageValues _Names;
    LanguageValues _AppNames;
    int _appVersionCode;
    QString _appversionName;
    bool _forceUpdate;
    QString _updateUrl1;
    QString _updateUrl2;    
    QString _appName;
    QString _updateDirectoryAddress;
    QString _androidMinVersion;
    QString _IOSVersion;
    QString _IOSMinVersion;
    QString _IOSUpdateUrl;
};

#endif // MERCHANT_H
