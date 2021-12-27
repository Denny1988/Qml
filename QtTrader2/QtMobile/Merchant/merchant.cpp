#include "merchant.h"
#include <QDomNodeList>
#include <qdebug.h>
Merchant::Merchant(QString code)
{
    this->_code = code;
}

void Merchant::initialize(QDomNode merchantNode)
{
    if(merchantNode.nodeName() != "Merchant")
    {
        qDebug() << "Merchant initialize return failed. nodeName = " << merchantNode.nodeName();
        return ;
    }

    QDomNamedNodeMap attrs = merchantNode.attributes();
    for (int i = 0; i < attrs.count(); i++)
    {
        QDomNode itemNode = attrs.item(i);
        QString nodeName = itemNode.nodeName();
        QString nodeValue = itemNode.nodeValue();
        qDebug() << "Merchant attributes nodeName = " << nodeName << " nodeValue = "  <<nodeValue ;
        if(nodeName == "code")
        {
           this->_code = nodeValue;
        }
        else if(nodeName == "settingFileUrl")
        {
            this->_settingFileUrl = nodeValue;
        }
        else if(nodeName == "name")
        {
            this->_name = nodeValue;
        }
        else if(nodeName == "versionCode")
        {
            this->_versionCode = nodeValue.toInt();
        }        
        else if(nodeName == "appVersionCode")
        {
            this->_appVersionCode = nodeValue.toInt();
        }
        else if(nodeName == "appversionName")
        {
            this->_appversionName = nodeValue;
        }
        else if(nodeName == "forceUpdate")
        {
            this->_forceUpdate = nodeValue.toLower() == "true";
        }
        else if(nodeName == "updateUrl1")
        {
            this->_updateUrl1 = nodeValue;
        }
        else if(nodeName == "updateUrl2")
        {
            this->_updateUrl2 = nodeValue;
        }
        else if(nodeName == "appName")
        {
            this->_appName = nodeValue;
        }
        else if(nodeName == "updateDirectoryAddress")
        {
            this->_updateDirectoryAddress = nodeValue;
        }
        else if(nodeName == "androidMinVersion")
        {
            this->_androidMinVersion = nodeValue;
        }
        else if(nodeName == "IOSVersion")
        {
            this->_IOSVersion = nodeValue;
        }
        else if(nodeName == "IOSMinVersion")
        {
            this->_IOSMinVersion = nodeValue;
        }
        else if(nodeName == "IOSUpdateUrl")
        {
            this->_IOSUpdateUrl = nodeValue;
        }
    }

    QDomNodeList merchantItemlist = merchantNode.toElement().childNodes();
    for (int i = 0; i < merchantItemlist.count(); i++)
    {
        QDomNode itemNode = merchantItemlist.item(i);
        QString nodeName = itemNode.nodeName();
        qDebug() << "Merchant initialize nodeName = " << nodeName;
        if(nodeName == "AppNames")
        {
            //this->_AppNames = nodeValue;
        }
        else if(nodeName == "Names")
        {
            //this->_Names = nodeValue;
        }
    }
}

QString Merchant::getCode()
{
    return this->_code;
}

QString Merchant::getName()
{
    return this->_name;
}

QString Merchant::getSettingFileUrl()
{
    return this->_settingFileUrl;
}

int Merchant::getVersionCode()
{
    return this->_versionCode;
}

void Merchant::setName(QString name)
{
    this->_name = name;
}

int Merchant::getAppVersionCode()
{
    return this->_appVersionCode;
}

QString Merchant::getAppversionName()
{
    return this->_appversionName;
}

bool Merchant::getForceUpdate()
{
    return this->_forceUpdate;
}

QString Merchant::getUpdateUrl1()
{
    if(this->_appName.length() > 0)
        return QString("%1%2_%3.apk").arg(this->_updateDirectoryAddress, this->_appName, this->_appversionName);
    else
        return this->_updateUrl1;
}

QString Merchant::getUpdateUrl2()
{
    return this->_updateUrl2;
}

QString Merchant::getAndroidMinVersion()
{
    return this->_androidMinVersion;
}

QString Merchant::getIOSVersion()
{
    return this->_IOSVersion;
}

QString Merchant::getIOSMinVersion()
{
   return this->_IOSMinVersion;
}

QString Merchant::getIOSUpdateUrl()
{
   return this->_IOSUpdateUrl;
}
