#include "manager.h"
#include "Service/ServiceManager.h"
#include "Business/BusinessManager.h"
#include "Common/AppSetting.h"
#include "Common/LoginSetting.h"
#include "Common/CustomerSetting.h"
#include "Common/UtcTimeManager.h"
#include <QPushButton>
#include <QCoreApplication>
#include "Utils/commonapihelper.h"
#include <QDir>

Manager* Manager::m_manager = Q_NULLPTR;


Manager::Manager()
{
    QString host =  QString("10.3.192.1");
    Service::ServiceManager::getInstance()->setServer(host, 4523, host, 4529, false);
}


Manager* Manager::getInstance(){
    if (!m_manager)
    {
        m_manager = new Manager;
    }
    return m_manager;
}

void Manager::deleteInstance(){
    if(Manager::m_manager != Q_NULLPTR){
       delete(Manager::m_manager);
    }
    Manager::m_manager = Q_NULLPTR;
}


void Manager::init()
{
    this->m_account = new Account();
    this->m_instrumentList = QList<Instrument*>();
    for(int i=0; i < 10; i++)
    {
        Instrument* instrument = new Instrument();
        instrument->m_code.append(i);
        this->m_instrumentList << instrument;
    }

    //login
    QString loginID = "GST001";
    QString password = "12345678";
    QString language = "CHS";
    QDir dir;
    QString strDir = CommonApiHelper::getAppDirectoryPath();
    QString path = QString("%1/Setting").arg(strDir);
    if(!dir.exists(path)) dir.mkpath(path);
    QString userPath = QString("%1/User/%2").arg(path, loginID);
    if(!dir.exists(userPath)) dir.mkpath(userPath);
    Common::CustomerSetting::getInstance()->createSettingByPath(userPath);
    m_LoginResult = Service::ServiceManager::getInstance()->getTradeService()->login(loginID, password, language, QString("DEM"), "", true);
    connect(m_LoginResult.data(), SIGNAL(completed()), this, SLOT(loginResultCompletedSlot()));

}

void Manager::clear()
{
    if(this->m_account){
        delete this->m_account;
        this->m_account = Q_NULLPTR;
    }
    if(!this->m_instrumentList.isEmpty()){
        for(int i = 0; i < this->m_instrumentList.size(); i ++ ){
            delete this->m_instrumentList[i];
        }
        this->m_instrumentList.clear();
    }
}

Account* Manager::getAccount(){
       return this->m_account;
}


void Manager::loginResultCompletedSlot()
{
    disconnect(sender(), SIGNAL(completed()), this, SLOT(loginResultCompletedSlot()));
    if (this->m_LoginResult->hasError())
    {
        Service::ServiceManager::getInstance()->stop();
        return;
    }

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QDateTime serverDate = QDateTime::fromString(this->m_LoginResult->getServerDateTime(), Common::Constants::DateTimeConstants::DateTimeFormat);
    qint64 serverToClientMilliseconds = currentDateTime.msecsTo(serverDate);
    Common::UtcTimeManager::getInstance()->startServerDateTime(serverToClientMilliseconds);

    qDebug() << "connect startGetInitData ";
    connect(this, SIGNAL(startGetInitData()), Business::BusinessManager::getInstance(), SLOT(getInitData()));

    Service::ServiceManager::getInstance()->setQuotationServerParameter(this->m_LoginResult->getWatchwords(), this->m_LoginResult->getClientId());

    qDebug() << "emit startGetInitData ";
    emit this->startGetInitData();

    qDebug() << "emit startGetInitData end ";

    Business::BusinessManager::getInstance()->isGetSettingDataOrTraderData();
    qDebug() << "isGetSettingDataOrTraderData end ";
}
