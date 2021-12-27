#include "autoupdatemanager.h"
#include "autoupdatedialog.h"
#include "Common/TraderBaseInfo.h"
#include "Utils/CommonApiHelper.h"
#include <QFile>
#include <QDomNodeList>
#include <QTextStream>
#include <QStandardPaths>
#include <QMessageBox>
#include <QStringList>

AutoUpdateManager* AutoUpdateManager::_instance = Q_NULLPTR;
AutoUpdateManager* AutoUpdateManager::getInstance()
{
    if(!_instance)
    {
        _instance = new AutoUpdateManager();
    }
    return _instance;
}

AutoUpdateManager::AutoUpdateManager()
{
    this->_appversionName = CommonApiHelper::getAppversionName();
    this->_appVersionCode = CommonApiHelper::getAppVersionCode();
    Common::TraderBaseInfo::getInstance()->setAppVersionCode(QString::number(this->_appVersionCode));
    Common::TraderBaseInfo::getInstance()->setAppversionName(this->_appversionName);
    qDebug() << "MerchantManager() appversionName = " << this->_appversionName  << ",appVersionCode = "  << this->_appVersionCode;
}

AutoUpdateManager::~AutoUpdateManager()
{
    qDebug() << "destruct---------------- AutoUpdateManager::~AutoUpdateManager()";
}

bool AutoUpdateManager::checkUpdate(int appVersionCode, QString appversionName, QString minAppversionName, bool forceUpdate, QString updateUrl1, QString updateUrl2, QWidget *parent)
{
    bool isStartUpdate = false;
    if(appVersionCode > 0)
    {
        if(appVersionCode > 0 && appVersionCode > this->_appVersionCode)
        {
            isStartUpdate = true;
            if(forceUpdate)
                CommonApiHelper::openUrl(updateUrl1);
            else
            {
                bool isNeedDirectUpdate = this->isLessThanMinVersion(this->_appversionName, minAppversionName);
                AutoUpdateDialog* autoUpdateDialog = new AutoUpdateDialog(this->_appversionName, appversionName, isNeedDirectUpdate, parent);
                connect(autoUpdateDialog, &AutoUpdateDialog::startUpdateSignal, this, [=](){
                     CommonApiHelper::openUrl(updateUrl1);
                }, Qt::QueuedConnection);
                connect(autoUpdateDialog, &AutoUpdateDialog::closeSignal, this, [=](){
                     delete autoUpdateDialog;
                     emit startUpdateSignal();
                }, Qt::QueuedConnection);
                autoUpdateDialog->show();
            }
        }
    }
    else
    {
        //for ios
        bool isNeedUpdate = this->isLessThanMinVersion(this->_appversionName, appversionName);
        if(isNeedUpdate)
        {
            isStartUpdate = true;
            if(forceUpdate)
                CommonApiHelper::openUrl(updateUrl1);
            else
            {
                bool isNeedDirectUpdate = this->isLessThanMinVersion(this->_appversionName, minAppversionName);
                AutoUpdateDialog* autoUpdateDialog = new AutoUpdateDialog(this->_appversionName, appversionName, isNeedDirectUpdate, parent);
                connect(autoUpdateDialog, &AutoUpdateDialog::startUpdateSignal, this, [=](){
                     CommonApiHelper::openUrl(updateUrl1);
                }, Qt::QueuedConnection);
                connect(autoUpdateDialog, &AutoUpdateDialog::closeSignal, this, [=](){
                     delete autoUpdateDialog;
                     emit startUpdateSignal();
                }, Qt::QueuedConnection);
                autoUpdateDialog->show();
            }
        }
    }
    return isStartUpdate;
}

bool AutoUpdateManager::isLessThanMinVersion(QString version, QString minVersion)
{
    //version format 1.0.18
    QStringList versionArray = version.split('.');
    QStringList minVersionArray = minVersion.split('.');
    int minVersion1 = minVersionArray[0].toInt();
    int version1 = versionArray[0].toInt();
    if (minVersion1 > version1)
        return true;
    else if (minVersion1 < version1)
        return false;
    else
    {
        int minVersion2 = minVersionArray[1].toInt();
        int version2 = versionArray[1].toInt();
        if (minVersion2 > version2)
            return true;
        else if (minVersion2 < version2)
            return false;
        else
        {
            int minVersion2 = minVersionArray[2].toInt();
            int version2 = versionArray[2].toInt();
            if (minVersion2 > version2)
                return true;
            else
            {
                return false;
            }
        }
    }
}
