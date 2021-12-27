#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QDebug>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
class QSslError;
QT_END_NAMESPACE

class DownloadManager: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QVector<QNetworkReply *> currentDownloads;

public:
    DownloadManager();
    ~DownloadManager();
    void doDownload(const QUrl &url);
    static QString saveFileName(const QUrl &url);
    bool saveToDisk(const QString &filename, QIODevice *data);
    static bool isHttpRedirect(QNetworkReply *reply);
    void setDownloadDir(QString dirPath);
public slots:
    void execute(QStringList args);
    void downloadFinished(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);

signals:
    void downloadFinishSignal(bool isSuccess, QString fileName);

private :
    QString m_downloadDir;
};

#endif // DOWNLOADMANAGER_H
