#ifndef UPLOADPICTURE_H
#define UPLOADPICTURE_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QFile>
#include <QDataStream>
#include <QTextCodec>
#include <QImage>

class UploadPicture: public QObject
{
    Q_OBJECT
public:
    UploadPicture();
    ~UploadPicture();
    void uploadPictureByPath(QString path, QString userId, QString fileId);
    void uploadPictureByImage(QImage image, QString userId, QString fileId);

public slots:
    void  requestFinishedSlot();
    void  uploadErrorSlot(QNetworkReply::NetworkError code);
    void  uploadProcessSlot(qint64 bytesReceived, qint64 bytesTotal);

signals:
    void uploadFinishedSignal(QString id);

private :
    QNetworkAccessManager * m_pNetWorkManager = NULL;
    QNetworkReply *reply;

};

#endif // DOWNLOADMANAGER_H
