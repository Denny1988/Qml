#include "uploadpicture.h"
#include <QDebug>
#include "Common/TraderBaseInfo.h"
#include "Utils/commonapihelper.h"
#include <QUrlQuery>
#include <QDateTime>

UploadPicture::UploadPicture()
{
    m_pNetWorkManager =  new  QNetworkAccessManager();
}

UploadPicture::~UploadPicture()
{
    qDebug() <<"destruct---------------- UploadPicture destory!";
}

void  UploadPicture::uploadPictureByPath(QString path, QString userId, QString fileId)
{
    QDateTime now = QDateTime::currentDateTime();
    QString timeStr;//yyyyMMddhhmmsszzz
    QDate date = now.date();
    QTime time = now.time();
    timeStr.sprintf("%d%02d%02d%02d%02d%02d%03d", date.year(), date.month(), date.day(), time.hour(), time.minute(), time.second(),time.msec());

     QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
     QHttpPart imagePart;
     imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/*"));
     imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"%1\"; filename=\"%2\"").arg("file1", path));
     QFile *file = new QFile(path);
     file->open(QIODevice::ReadOnly);
     imagePart.setBodyDevice(file);
     file->setParent(multiPart);
     multiPart->append(imagePart);

     QString urlStr = QString("%1?method=form&userid=%2&fileid=%3&time=%4&sign=").arg(
                 Common::TraderBaseInfo::getInstance()->getUploadUrl(), userId, fileId, timeStr);
     //for test
//     QString urlStr = QString("http://10.3.191.1/iExchange/AuxService/FileUpLoad/receiver.ashx?method=form&userid=%1&fileid=%2&time=%3&sign=111").arg(
//                 userId, fileId, timeStr);
     QUrl url(urlStr);
     QNetworkRequest request(url);

     reply = m_pNetWorkManager->post(request, multiPart);
     multiPart->setParent(reply);

     connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),  this , SLOT(uploadErrorSlot(QNetworkReply::NetworkError)));
     connect(reply, SIGNAL(downloadProgress(qint64, qint64)),  this , SLOT(uploadProcessSlot(qint64, qint64)));
     connect(reply, SIGNAL(finished()), this, SLOT(requestFinishedSlot()));
}

void UploadPicture::uploadPictureByImage(QImage image, QString userId, QString fileId)
{
    QDateTime now = QDateTime::currentDateTime();
    QString timeStr;//yyyyMMddhhmmss
    QDate date = now.date();
    QTime time = now.time();
    timeStr.sprintf("%d%02d%02d%02d%02d%02d%03d", date.year(), date.month(), date.day(), time.hour(), time.minute(), time.second(),time.msec());
    QString path = QString("%1/%2.jpg").arg(CommonApiHelper::getAppDirectoryPath(), timeStr);
    qDebug() << "uploadPictureByImage path:" << path;
    image.save(path);
    this->uploadPictureByPath(path, userId, fileId);
}

void UploadPicture::requestFinishedSlot()
{
     QByteArray resultContent = reply->readAll();
     QTextCodec* pCodec = QTextCodec::codecForName("UTF-8");
     QString strResult = pCodec->toUnicode(resultContent);
     int nHttpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(); //http返回码
     qDebug() << "requestFinishedSlot strResult:" << strResult << " nHttpCode:" << nHttpCode;
     emit uploadFinishedSignal(strResult);
}

void  UploadPicture::uploadErrorSlot(QNetworkReply::NetworkError code)
{
     qDebug() << code;
}

void  UploadPicture::uploadProcessSlot(qint64 bytesReceived, qint64 bytesTotal)
{
     qDebug() << bytesReceived << bytesTotal;
}

