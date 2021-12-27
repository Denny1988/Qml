#ifndef QTFRAMEWORK_BROWSERDLL_H
#define QTFRAMEWORK_BROWSERDLL_H

#ifdef RUNTIME_OS_WIN32
#include "globelmacro.h"
#include <QWidget>
#include <ActiveQt/QAxWidget>
#include "windows.h"
#include <QVBoxLayout>

class QAxWidget;
class QSLOG_SHARED_OBJECT BraowserWrapper : public QWidget
{
    Q_OBJECT
public:
    BraowserWrapper(const QString& url,QWidget* parent = NULL);
    ~BraowserWrapper();

    void printHtml();
    void setURL(const QString& url);
    QAxWidget* getAxWidget(){ return _webBrowser; }
private:
    QAxWidget*		_webBrowser;
    int _startLoadingState;
    int _documentCompleteState;
    int _fileDownloadState;
    int _downloadCompleteState;

signals:
    void startLoadingSignal();
    void documentCompleteSignal();
    void fileDownloadSignal();
    void downloadCompleteSignal();

public slots:
    void receiverSlot(const QString & name, int argc, void * argv);
};

#endif //RUNTIME_OS_WIN32

#ifdef RUNTIME_OS_MACX

#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <QWebEngineView>
#include <QUrl>

class BraowserWrapper : public QWidget
{
    Q_OBJECT
public:
    BraowserWrapper(const QString& url, QWidget* parent = NULL,QString type = "");
    ~BraowserWrapper();

    void reload();
    void setURL(const QString& url);
    QWebEngineView* getQWebEngineView(){ return _webView; }
private:
    QWebEngineView*		_webView;
    QString  _type;
    QString  _downloadSavePath;
    int _LoadState;
    int _fileDownloadState;

signals:
    void loadFinishedSignal(bool isOK);
    void fileDownloadSignal();
    void downloadCompleteSignal(QString savePath);

private slots:
    void loadFinishedSlot(bool isOK);
    void downloadRequestedSlot(QWebEngineDownloadItem* download);
    void downloadCompletedSlot();
};
#endif //RUNTIME_OS_MACX


#endif // BROWSERDLL_H
