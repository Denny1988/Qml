#include "merchantmanager.h"
#include "Common/TraderBaseInfo.h"
#include "Common/LoginSetting.h"
#include <QFile>
#include <QDomNodeList>
#include <QTextStream>
#include <QStandardPaths>
#include <QMessageBox>
#include <QApplication>

#include "Utils/CommonApiHelper.h"
#include "QtGui/private/qzipreader_p.h"

MerchantManager* MerchantManager::_instance = Q_NULLPTR;
MerchantManager* MerchantManager::getInstance()
{
    if(!_instance)
    {
        _instance = new MerchantManager();
    }
    return _instance;
}

MerchantManager::MerchantManager()
{
    this->_changing = false;
    this->_useSpareMerchantListURL = false;
    QFile merchantFile(":/Resources/Merchant.txt");
    merchantFile.open(QIODevice::ReadOnly);
    QTextStream in(&merchantFile);
    this->_merchantListURL = in.readLine();
    this->_spareMerchantListURL = in.readLine();
    merchantFile.close();

    this->_settings =new QSettings("MobileTrader", "MerchantManager");
    this->m_downloadManager = new DownloadManager();

    QString fileName = "%1/Merchant.ini";
    this->_settings = new QSettings(fileName.arg(this->getCurrentMerchantPath()), QSettings ::IniFormat);

    if(this->_settings->contains("UserSettings/MerchantCode"))
    {
        this->_merchantCode = this->_settings->value("UserSettings/MerchantCode").toString();
        this->_merchantVersionCode = this->_settings->value("UserSettings/MerchantVersionCode").toInt();
    }
    else
    {
        this->_merchantCode = "";
        this->_merchantVersionCode = 0;
    }
    qDebug() << "MerchantManager() code = " << this->_merchantCode  << ",versionCode = "  << this->_merchantVersionCode ;
}

MerchantManager::~MerchantManager()
{
    qDebug() << "destruct---------------- MerchantManager::~MerchantManager()";
   delete this->m_downloadManager;
   delete this->_settings;
}

bool MerchantManager::isSetMerchant()
{
    return this->_merchantCode.length() > 0;
}

void MerchantManager::excute()
{
    //update merchant list 更新商户列表
    QStringList downloadArgs(this->_merchantListURL);
    qDebug() << this->_merchantListURL;
    m_downloadManager->execute(downloadArgs);
    connect( m_downloadManager,SIGNAL(downloadFinishSignal(bool,QString)), this, SLOT(merchantListDownSlot(bool,QString)));
}

void MerchantManager::setCurrentMerchant(QString code)
{
    if(!code.isEmpty() && isExist(code))
    {
        Merchant *pMerchant = this->getMerchant(code);
        QString settingFileUrl = pMerchant->getSettingFileUrl();
        QString resourceFileUrl =  QString("%1/%2.zip").arg(settingFileUrl).arg(pMerchant->getName());
        //QString versionFileUrl =  QString("%1/version.xml").arg(settingFileUrl);
        QStringList downloadArgs(resourceFileUrl);
        //downloadArgs.push_back(resourceFileUrl);
        connect( m_downloadManager,SIGNAL(downloadFinishSignal(bool,QString)), this, SLOT(merchantResourceSlot(bool,QString)));
        m_downloadManager->execute(downloadArgs);
        this->_merchantCode = code;
        this->_settings->setValue("UserSettings/MerchantCode", code);

    }
    else
    {
        this->_merchantCode = "";
        this->_settings->setValue("UserSettings/MerchantCode", code);
    }
    qDebug() << "MerchantManager setCurrentMerchant code = " << code ;
}

void MerchantManager::updateCurrentMerchantResouce()
{
    Merchant *pMerchant = this->getCurrentMerchant();
    QString settingFileUrl = pMerchant->getSettingFileUrl();
    QString resourceFileUrl =  QString("%1/%2.zip").arg(settingFileUrl).arg(pMerchant->getName());
    //QString versionFileUrl =  QString("%1/version.xml").arg(settingFileUrl);
    QStringList downloadArgs(resourceFileUrl);
    //downloadArgs.push_back(resourceFileUrl);
    connect( m_downloadManager,SIGNAL(downloadFinishSignal(bool,QString)), this, SLOT(merchantResourceSlot(bool,QString)));
    m_downloadManager->execute(downloadArgs);
    qDebug() << "MerchantManager updateCurrentMerchantResouce finish ";
}


Merchant* MerchantManager::getCurrentMerchant()
{
    return getMerchant(this->_merchantCode);
}

QString MerchantManager::getCurrentMerchantPath()
{
    QString dirPath = CommonApiHelper::getAppDirectoryPath() + QDir::separator() + "Merchant";
    return dirPath;
}

QString MerchantManager::getImagePath(QString imageName)
{
    QString imagePath = "file:///" + this->getCurrentMerchantPath() + QDir::separator() + "Images"+ QDir::separator() + imageName;
    return imagePath;
}

bool MerchantManager::isExist(QString code)
{
    foreach (Merchant *pMerchant, this->_merchantList)
    {
        if (pMerchant->getCode() == code)
        {
            return true;
        }
    }
    return false;
}

Merchant* MerchantManager::getMerchant(QString code)
{
    if(!code.isEmpty())
    {
        foreach (Merchant *pMerchant, this->_merchantList)
        {
            if (pMerchant->getCode() == code)
            {
                return pMerchant;
            }
        }
    }
    return NULL;
}


void MerchantManager::merchantListDownSlot(bool isSuccess, QString fileName)
{
    disconnect( sender(), SIGNAL(downloadFinishSignal(bool,QString)), this, SLOT(merchantListDownSlot(bool,QString)));
    qDebug() << "MerchantManager merchantListDownSlot isSuccess = " << isSuccess << ", filename = " << fileName;
    if(isSuccess)
    {
        this->updateMerchantList(fileName);
        Merchant* pMerchant = this->getCurrentMerchant();
        if( !pMerchant || pMerchant == NULL || _changing)
        {
            _changing = false;
            bool needRestMerchant = true;
            emit checkMerchantStateSignals(needRestMerchant, true);
        }
        else
        {
            qDebug() << "MerchantManager merchantListDownSlot merchantCode = " << this->_merchantCode << ", merchantVersionCode = " << this->_merchantVersionCode;
            bool needUpdateResource = pMerchant->getVersionCode() > this->_merchantVersionCode;
            emit checkMerchantStateSignals(false, needUpdateResource);
        }
    }
    else
    {
        if(!this->_useSpareMerchantListURL)
        {
            this->_useSpareMerchantListURL = true;
            //使用备用链接
            QStringList downloadArgs(this->_spareMerchantListURL);
            m_downloadManager->execute(downloadArgs);
            connect( m_downloadManager,SIGNAL(downloadFinishSignal(bool,QString)), this, SLOT(merchantListDownSlot(bool,QString)));
        }
        else
        {
            emit quitSignal();
        }
    }

}

void MerchantManager::merchantResourceSlot(bool success,QString resourceZipFile)
{
   disconnect(sender(), SIGNAL(downloadFinishSignal(bool,QString)), this, SLOT(merchantResourceSlot(bool,QString)));
   if(success)
   {
       qDebug() <<" MerchantManager merchantResourceSlot updateCurrentResource!";
       updateCurrentResource(resourceZipFile);
   }
   else
   {
     qDebug() <<" MerchantManager merchantResourceSlot failed!";
   }
}

void MerchantManager::updateCurrentResource(QString resourceZipFile)
{
    qDebug() <<" MerchantManager updateCurrentResource " << resourceZipFile;
    QString destDirPath = getCurrentMerchantPath();
    QDir dir(destDirPath);
    if(!dir.exists())
    {
       dir.mkdir(destDirPath);
    }
    else
    {
       dir.cleanPath(destDirPath);
    }
    //JlCompress::extractDir(resourceZipFile, destDirPath); //解压缩
    this->unzip(resourceZipFile, destDirPath);
    this->_merchantVersionCode = this->getCurrentMerchant()->getVersionCode();
    this->_settings->setValue("UserSettings/MerchantVersionCode", this->_merchantVersionCode);
    emit updateCurrentResourceSignals();
}

void MerchantManager::unzip(QString resourceZipFile, QString destDirPath)
{
    QZipReader reader(resourceZipFile);//压缩路径
    reader.extractAll(destDirPath);//解压文件夹到当前目录

    QFile file1(destDirPath);
    file1.open(QIODevice::WriteOnly);
    file1.write(reader.fileData(QString::fromLocal8Bit("%1").arg(destDirPath)));//解压文件
    file1.close();
    reader.close();
}

bool MerchantManager::updateMerchantList(QString xmlFile)
{
    this->_merchantList = QList<Merchant*>();
    QFile file(xmlFile);
    if (!(file.open(QIODevice::ReadOnly | QIODevice::Text)))
    {
        return false;
    }
    QDomDocument domDoc;
    if (!(domDoc.setContent(&file)))
    {
        file.close();
        return false;
    }
    file.close();

    QDomNodeList merchantList = domDoc.elementsByTagName("MerchantList");
    for (int index = 0; index < merchantList.count(); index++)
    {
        QDomNode merchantNode = merchantList.item(index);
        qDebug() <<" MerchantManager update merchantListNode index = " << index  << " nodeName = "<< merchantNode.nodeName();
        QDomNodeList childNodeList = merchantNode.childNodes();
        for (int index_j = 0; index_j < childNodeList.count(); index_j ++ )
        {
            QDomNode childNode = childNodeList.item(index_j);
            if(childNode.nodeName() == "Merchant")
            {
                Merchant* pMerchant = new Merchant("");
                pMerchant->initialize(childNode);
                this->_merchantList.append(pMerchant);
            }
        }

    }
    qDebug() <<" MerchantManager update merchantList success";
    emit updateMerchantListSignals(true);
}
