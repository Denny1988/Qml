#include "splashscreen.h"
#include "ui_SplashScreen.h"
#include "stdafx.h"

#include <QDesktopServices>
#include <QLatin1String>
#include <QDebug>
#include <QDateTime>
#include <QUrl>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include "merchantmanager.h"
#include "merchantdialog.h"
#include "QsLog.h"
#include "Common/Constants/LogKeyConstants.h"
#include "Common/TraderBaseInfo.h"
#include "Common/LoginSetting.h"
#include "AutoUpdate/autoupdatemanager.h"
#include "Common/Util/Convert_Common.h"
//#include "CommonUI/alertboxwidget.h"
#include <QGroupBox>
//#include "CommonUI/processbarmanager.h"
#include "Utils/commonapihelper.h"

SplashScreen::SplashScreen(QWidget *parent) :
    QDialog(parent)
  //, ui(new Ui::SplashScreen)
{
    int width = g_ViewInfo->getScreenWidth();
    this->setFixedSize(width, g_ViewInfo->getScreenHeight());
    //ui->setupUi(this);
    this->setObjectName("SplashScreen");
    this->m_endProcessBar = false;

    QFont textFont = this->font();
    textFont.setPixelSize(g_ViewInfo->getDefaultPixelSize());
    g_ViewInfo->setSplashScreenFont(textFont);

#if defined (RUNTIME_OS_ANDROID)    
    this->setStyleSheet("#SplashScreen { border-image: url(:/Resources/Images/backgroud.png);} ");
    int moveX = (g_ViewInfo->getScreenWidth() - 500)/2;
    int moveY = g_ViewInfo->getScreenHeight()-g_ViewInfo->getButtonHeight()-10;
    //ProcessBarManager::getInstance()->startProcessBar2(this, moveX, moveY, false);

    this->m_movie = new QMovie(":/Resources/Images/Startup.gif");
    this->m_gif = new QLabel("gif", this);
    this->m_gif->setGeometry(QRect(moveX, moveY, g_ViewInfo->getScreenWidth() - moveX*2, g_ViewInfo->getButtonHeight()));
    this->m_gif->setMovie(this->m_movie);
    this->m_gif->setStyleSheet("QLabel {background-color: transparent;}");
    this->m_movie->start();
#else
    this->setStyleSheet("#SplashScreen{ background-color:rgb(64, 32, 64); }");
    QString message = "initialization in process";
    QString currentLanguage = Common::LoginSetting::getInstance()->getLanguage();
    if(currentLanguage == "CHS")
        message = "正在初始化";
    else if(currentLanguage == "CHT")
        message = "正在初始化";
    ProcessBarManager::getInstance()->startProcessBar(this, -1, -1, message);
#endif

    this->m_isUpdated = false;
    this->m_isNewEnterMerchant = false;

    connect(this, SIGNAL(startSignal()), this, SLOT(startSlot()), Qt::QueuedConnection);
#if defined (RUNTIME_OS_IPHONE)
    if(Common::TraderBaseInfo::getInstance()->getIsPrivateMerchant())
    {
        emit startSignal();
    }
    else
    {
        this->m_enableGroupUserDownfailedCount = 0;
        connect(this, SIGNAL(startEnableGroupUserDownSignal()), this, SLOT(startEnableGroupUserDownSlot()), Qt::QueuedConnection);
        this->m_enableGroupUserDownTimer = new QTimer(this);
        connect(this->m_enableGroupUserDownTimer, SIGNAL(timeout()), this, SLOT(startEnableGroupUserDownSlot()));
        emit startEnableGroupUserDownSignal();
    }
#else
    emit startSignal();
#endif

}

void SplashScreen::startEnableGroupUserDown(bool isUseFirstLine)
{
    this->m_isUseFirstLine = isUseFirstLine;
    QFile enableGroupUserFile(":/Resources/EnableGroupUserUrl.txt");
    enableGroupUserFile.open(QIODevice::ReadOnly);
    QTextStream in(&enableGroupUserFile);
    QString enableGroupUserURL = in.readLine();
    QString enableGroupUserURL2 = in.readLine();
    enableGroupUserFile.close();

    DownloadManager* downloadManager = new DownloadManager();
    qDebug() <<" isUseFirstLine:" << isUseFirstLine << " enableGroupUserURL:" << enableGroupUserURL << " enableGroupUserURL2:" << enableGroupUserURL2;
    QStringList downloadArgs(isUseFirstLine ? enableGroupUserURL : enableGroupUserURL2);
    downloadManager->execute(downloadArgs);
    connect(downloadManager,SIGNAL(downloadFinishSignal(bool,QString)), this, SLOT(enableGroupUserDownSlot(bool,QString)));
}

void SplashScreen::startEnableGroupUserDownSlot()
{    
    if(this->m_enableGroupUserDownTimer->isActive())
        this->m_enableGroupUserDownTimer->stop();
    this->startEnableGroupUserDown();
}

void SplashScreen::enableGroupUserDownSlot(bool isSuccess, QString fileName)
{
    disconnect(sender(), SIGNAL(downloadFinishSignal(bool,QString)), this, SLOT(enableGroupUserDownSlot(bool,QString)));
    delete sender();
    if(isSuccess)
    {
        if(this->m_enableGroupUserDownTimer->isActive())
            this->m_enableGroupUserDownTimer->stop();

        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            bool oldIsPrivate = Common::toBool(in.readLine());
            qDebug() << oldIsPrivate;
            QString merchantCode = in.readLine();
            for(int i = 0; i<7; i++)
            {
                qDebug() << in.readLine();
            }
            bool newIsPrivate = true;
            QString appVersionCode = CommonApiHelper::getAppversionName();
            qDebug() <<"appVersionCode:"<< appVersionCode;
            while (!in.atEnd()) {
                QString newLine = in.readLine();
                qDebug() << newLine;
                if(newLine.contains("_"))
                {
                    int index = newLine.indexOf("_");
                    QString Config_AppVersionCode = newLine.mid(0, index);
                    if(Config_AppVersionCode == appVersionCode)
                        newIsPrivate = Common::toBool(newLine.mid(index + 1));
                }
                else
                {
                    newIsPrivate = Common::toBool(newLine);
                }
            }
            if(!newIsPrivate)
            {
                Common::TraderBaseInfo::getInstance()->setIsPrivateMerchant(!newIsPrivate);
                Common::TraderBaseInfo::getInstance()->setMerchantCode(merchantCode);
            }
            else
            {
                //ProcessBarManager::getInstance()->endProcessBar();
                //this->m_endProcessBar = true;
            }
            file.close();
        }
        emit startSignal();
    }
    else
    {
        if(this->m_isUseFirstLine)
        {
            this->startEnableGroupUserDown(false);
        }
        else
        {
            QString firstStartApp = Common::LoginSetting::getInstance()->getValue("LoginSetting", "FirstStart");
            if(firstStartApp != "false")
            {
                //第一次启动要用户设置网络访问权限，这个时候下载会失败 先自动重新试10次 如果还是不行就出一个提示框告知网络不能访问请设置后在点击后继续重试
                if(this->m_enableGroupUserDownfailedCount == 10)
                {
                    //ProcessBarManager::getInstance()->endProcessBar();
                    //this->m_endProcessBar = true;
                    this->quitSlot();
                }
                else
                {
                    this->m_enableGroupUserDownfailedCount++;
                    this->m_enableGroupUserDownTimer->start(1000);
                }
            }
            else
                emit startSignal();
        }
    }
}

SplashScreen::~SplashScreen()
{
    qDebug() << "destruct---------------- SplashScreen::~SplashScreen()";
    //delete ui;
#if defined (RUNTIME_OS_ANDROID)
    this->m_movie->stop();
    delete this->m_movie;
    delete this->m_gif;
#endif
#if defined (RUNTIME_OS_IPHONE)
    delete this->m_enableGroupUserDownTimer;
#endif
}

void SplashScreen::start()
{
    qDebug() << "SplashScreen start()";
    QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "SplashScreen Start";
    connect(MerchantManager::getInstance(), SIGNAL(checkMerchantStateSignals(bool, bool)), this, SLOT(checkMerchantStateSlot(bool, bool)));    
    connect(MerchantManager::getInstance(), SIGNAL(quitSignal()), this, SLOT(quitSlot()));
    QTimer::singleShot(0, this, SLOT(executeUpdateSlot()));
}

void SplashScreen::finish()
{
    disconnect(MerchantManager::getInstance(), SIGNAL(checkMerchantStateSignals(bool, bool)), this, SLOT(checkMerchantStateSlot(bool, bool)));
    disconnect(MerchantManager::getInstance(),SIGNAL(updateCurrentResourceSignals()), this, SLOT(settingMerchantFinishSlot()));
#if defined (RUNTIME_OS_ANDROID)
    QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "SplashScreen finish m_isUpdated:" << this->m_isUpdated;
    auto merchant = MerchantManager::getInstance()->getCurrentMerchant();
    bool isUpdate = AutoUpdateManager::getInstance()->checkUpdate(
                merchant->getAppVersionCode(),
                merchant->getAppversionName(),
                merchant->getAndroidMinVersion(),
                merchant->getForceUpdate(),
                merchant->getUpdateUrl1(),
                merchant->getUpdateUrl2(),
                this);
    connect(AutoUpdateManager::getInstance(), &AutoUpdateManager::startUpdateSignal, this, [=] () {
                if(!this->m_isUpdated)
                {
                    this->m_isUpdated = true;
                    emit finishSignal();
                }
                this->accept();
            }, Qt::QueuedConnection);
    if(!isUpdate)
    {
        if(!this->m_isUpdated)
        {
            this->m_isUpdated = true;
            emit finishSignal();
        }
        this->accept();
    }
#else
    if(!this->m_endProcessBar)
        ProcessBarManager::getInstance()->endProcessBar();
    QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "SplashScreen finish m_isUpdated:" << this->m_isUpdated ;
    auto merchant = MerchantManager::getInstance()->getCurrentMerchant();
    bool isUpdate = AutoUpdateManager::getInstance()->checkUpdate(
                -1,
                merchant->getIOSVersion(),
                merchant->getIOSMinVersion(),
                merchant->getForceUpdate(),
                merchant->getIOSUpdateUrl(),
                merchant->getIOSUpdateUrl(),
                this);
    connect(AutoUpdateManager::getInstance(), &AutoUpdateManager::startUpdateSignal, this, [=] () {
                if(!this->m_isUpdated)
                {
                    this->m_isUpdated = true;
                    emit finishSignal();
                }
                this->accept();
            }, Qt::QueuedConnection);
    if(!isUpdate)
    {
        if(!this->m_isUpdated)
        {
            this->m_isUpdated = true;
            emit finishSignal();
        }
        Common::LoginSetting::getInstance()->setValue("LoginSetting", "FirstStart", "false");
        this->accept();
    }
#endif
}

void SplashScreen::startSlot()
{
    this->start();
}

void SplashScreen::quitSlot()
{
    if(!MerchantManager::getInstance()->isSetMerchant())
    {
#if defined (RUNTIME_OS_IPHONE)
            this->m_enableGroupUserDownfailedCount = 0;
            this->startEnableGroupUserDownSlot();
#else
            disconnect(MerchantManager::getInstance(), SIGNAL(checkMerchantStateSignals(bool, bool)), this, SLOT(checkMerchantStateSlot(bool, bool)));
            disconnect(MerchantManager::getInstance(), SIGNAL(quitSignal()), this, SLOT(quitSlot()));
            this->start();
#endif
//        AlertBoxWidget* alertBoxWidget = new AlertBoxWidget(AlertBoxWidget::Communication, this);
//        connect(alertBoxWidget, &AlertBoxWidget::leftClickButtonSignal, this, [=](){
//#if defined (RUNTIME_OS_IPHONE)
//            this->m_enableGroupUserDownfailedCount = 0;
//            this->startEnableGroupUserDownSlot();
//#else
//            disconnect(MerchantManager::getInstance(), SIGNAL(checkMerchantStateSignals(bool, bool)), this, SLOT(checkMerchantStateSlot(bool, bool)));
//            disconnect(MerchantManager::getInstance(), SIGNAL(quitSignal()), this, SLOT(quitSlot()));
//            this->start();
//#endif
//        }, Qt::QueuedConnection);
//        connect(alertBoxWidget, &AlertBoxWidget::rightClickButtonSignal, this, [=](){
//            emit finishSignal();
//            this->accept();
//        }, Qt::QueuedConnection);
//        alertBoxWidget->showDialog();
    }
    else
    {
        emit finishSignal();
        this->accept();
    }
}

void SplashScreen::executeUpdateSlot()
{
    QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "  SplashScreen::executeUpdateSlot start";
    MerchantManager::getInstance()->excute();
}

void SplashScreen::executeAnimationSlot()
{
    QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "  SplashScreen::executeAnimationSlot start";
    QDateTime n = QDateTime::currentDateTime();
    QDateTime now;
    do{
        if(this->m_isUpdated){
            break;
        }
        now=QDateTime::currentDateTime();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 500);
    } while (n.secsTo(now)<30);//delay 30s
    this->finish();
}

void SplashScreen::skipClickedSlot()
{
    this->finish();
    qDebug() << "skipbutton onClicked";
    QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "skipbutton onClicked";
}

void SplashScreen::showMerchantSlot()
{
    qDebug() << "showLinkbutton onClicked";
    QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "showLinkbutton onClicked";
}

void SplashScreen::setMerchantSlot(QString merchantCode)
{
    if(!merchantCode.isEmpty() && MerchantManager::getInstance()->isExist(merchantCode))
    {
        MerchantManager::getInstance()->setCurrentMerchant(merchantCode);
        connect(MerchantManager::getInstance(),SIGNAL(updateCurrentResourceSignals()), this, SLOT(settingMerchantFinishSlot()));
        if(!Common::TraderBaseInfo::getInstance()->getIsPrivateMerchant())
        {
            disconnect(m_merchantPage, SIGNAL(setMerchantSignal(QString)), this, SLOT(setMerchantSlot(QString)));
            //m_merchantPage->accept();
            delete m_merchantPage;
        }
        return;
    }
    else
    {
        MerchantManager::getInstance()->setCurrentMerchant("");
        QString currentLanguage = Common::LoginSetting::getInstance()->getLanguage();
        if(Common::TraderBaseInfo::getInstance()->getIsPrivateMerchant())
        {
            if(currentLanguage == "CHS")
               QMessageBox::critical(nullptr,"通知", "连接服务器失败, 请过一会儿再试！");
            else if(currentLanguage == "CHT")
               QMessageBox::critical(nullptr,"通知", "連接服務器失敗，請過一會兒再試！");
            else
               QMessageBox::critical(nullptr,"notice", "Connection server failed, please try again later!");
            QApplication::quit();
        }
        else
        {
            if(currentLanguage == "CHS")
                QMessageBox::information(nullptr,"通知", "商户号不存在!");
            else if(currentLanguage == "CHT")
                QMessageBox::information(nullptr,"通知", "商戶號不存在!");
            else
                QMessageBox::information(nullptr,"notice", "Merchant number does not exist!");
            this->showMerchantSettingDialog();
        }
    }
}


void SplashScreen::showMerchantSettingDialog()
{
    m_merchantPage = new MerchantDialog(false, this);
    connect(m_merchantPage, SIGNAL(setMerchantSignal(QString)), this, SLOT(setMerchantSlot(QString)));
    m_merchantPage->show();
}

void SplashScreen::checkMerchantStateSlot(bool needSetMerchant, bool needUpdateResource)
{    
    qDebug() << "SplashScreen checkMerchantStateSlot needSetMerchant = " <<needSetMerchant << ",needUpdateResource = " <<needUpdateResource;    
    QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "SplashScreen checkMerchantStateSlot needSetMerchant = " <<needSetMerchant << ",needUpdateResource = " <<needUpdateResource;
    if(needSetMerchant)
    {
        this->m_isNewEnterMerchant = true;
        if(Common::TraderBaseInfo::getInstance()->getIsPrivateMerchant())
            this->setMerchantSlot(Common::TraderBaseInfo::getInstance()->getMerchantCode());
        else
            showMerchantSettingDialog();
    }
    else if(needUpdateResource)
    {
        MerchantManager::getInstance()->updateCurrentMerchantResouce();
        connect( MerchantManager::getInstance(),SIGNAL(updateCurrentResourceSignals()), this, SLOT(settingMerchantFinishSlot()));
    }
    else
    {
        this->finish();
    }
}

void SplashScreen::settingMerchantFinishSlot()
{
    qDebug() << "SplashScreen settingMerchantFinishSlot";
    QLOG_INFO(Common::Constants::LogKeyConstants::Application) << "SplashScreen settingMerchantFinishSlot";
    if(!Common::TraderBaseInfo::getInstance()->getIsPrivateMerchant() && this->m_isNewEnterMerchant)
    {
        QString currentLanguage = Common::LoginSetting::getInstance()->getLanguage();
        if(currentLanguage == "CHS")
            QMessageBox::information(nullptr,"通知", QString("商户号设置为%1").arg(MerchantManager::getInstance()->getCurrentMerchant()->getName()));
        else if(currentLanguage == "CHT")
            QMessageBox::information(nullptr,"通知", QString("商戶號設置為%1").arg(MerchantManager::getInstance()->getCurrentMerchant()->getName()));
        else
            QMessageBox::information(nullptr,"notice", QString("Merchant number is set to %1").arg(MerchantManager::getInstance()->getCurrentMerchant()->getName()));
    }
    this->finish();
}
