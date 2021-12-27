#include "downloadmanager.h"
#include "Utils/commonapihelper.h"

DownloadManager::DownloadManager()
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));
}

DownloadManager::~DownloadManager()
{
    qDebug() <<"destruct---------------- DownloadManager destory!";
    this->m_downloadDir = "";
}

void DownloadManager::setDownloadDir(QString dirPath)
{
    qDebug() <<"DownloadManager setDownloadDir: " << dirPath;
    this->m_downloadDir = dirPath;
    if( !dirPath.isNull() && !dirPath.isEmpty())
    {
        QDir dir(this->m_downloadDir);
        if(!dir.exists())
        {
           dir.mkdir(this->m_downloadDir);
        }
    }
}

void DownloadManager::doDownload(const QUrl &url)
{
    qDebug() <<"DownloadManager doDownload begin:" << url;
    QNetworkRequest request(url);
    //request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    if(manager.networkAccessible() == QNetworkAccessManager::NotAccessible){
        manager.setNetworkAccessible(QNetworkAccessManager::Accessible);
    }
    QNetworkReply *reply = manager.get(request);
    currentDownloads.append(reply);
    qDebug() <<"DownloadManager doDownload end";
}

QString DownloadManager::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download";

    if (QFile::exists(basename)) {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
            ++i;

        basename += QString::number(i);
    }

    return basename;
}

bool DownloadManager::saveToDisk(const QString &filename, QIODevice *data)
{
    QString fullFilePath;
    if(this->m_downloadDir == "")
    {
       QString dirPath = CommonApiHelper::getAppDirectoryPath() + QDir::separator() + "Download";
       QDir dir(dirPath);
       if(!dir.exists())
       {
          dir.mkdir(dirPath);
       }
       this->m_downloadDir = dirPath;
    }
    fullFilePath = this->m_downloadDir + QDir::separator() + filename;
    QFile file(fullFilePath);
    qDebug() << "DownloadManager: saveToDisk() fullFilePath = " << fullFilePath;

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "DownloadManager: Could not open file for writing: " << filename << " Error:" << file.errorString();
        return false;
    }

    file.write(data->readAll());
    file.close();
    return true;
}

bool DownloadManager::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
           || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void DownloadManager::execute(QStringList args)
{
    qDebug() <<"DownloadManager execute() start... ";
    if (args.isEmpty()) {
        qDebug() << "DownloadManager: download list is Empty!";
        return;
    }

    for (const QString &arg : qAsConst(args)) {
        QUrl url = QUrl::fromEncoded(arg.toLocal8Bit());
        doDownload(url);
    }
}

void DownloadManager::sslErrors(const QList<QSslError> &sslErrors)
{
    Q_UNUSED(sslErrors);
}

void DownloadManager::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    QNetworkReply::NetworkError error = reply->error();
    qDebug() << " NetworkError:" << error;
    if (error != QNetworkReply::NoError) {
        QString fileUrl = url.toEncoded().constData();
        emit downloadFinishSignal(false, fileUrl);
        qDebug() <<"DownloadManager Download of " << fileUrl <<" Failed!";
    } else {
        if (isHttpRedirect(reply)) {
            fputs("Request was redirected.\n", stderr);
        } else {
            QString filename = saveFileName(url);
            if (saveToDisk(filename, reply)) {
                QString fileSavePath = this->m_downloadDir + QDir::separator() + filename;
                emit downloadFinishSignal(true, fileSavePath);
                qDebug() <<"Download of " << url.toEncoded().constData() <<" Success fileSavePath = " << fileSavePath;
            }
        }
    }

    currentDownloads.removeAll(reply);
    reply->deleteLater();

    if (currentDownloads.isEmpty()) {
        qDebug() << "DownloadManager: all downloads finished";
    }
}
