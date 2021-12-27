#include "browserWrapper.h"
#include <QVBoxLayout>

#ifdef RUNTIME_OS_WIN32

#include "windows.h"
#include <ActiveQt/QAxWidget>
#include "exdisp.h"
#include <qaxobject.h>
#include <QAxBase>

BraowserWrapper::BraowserWrapper(const QString& url,QWidget* parent):QWidget(parent)
{
	this->_startLoadingState = 0;
	this->_documentCompleteState = 0;
	this->_fileDownloadState = 0;
	this->_downloadCompleteState = 0;
	this->_webBrowser = new QAxWidget(this);
	this->_webBrowser->setControl(QStringLiteral("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
	this->_webBrowser->setObjectName(QStringLiteral("WebBrowser"));
    this->_webBrowser->setFocusPolicy(Qt::StrongFocus);
    this->_webBrowser->dynamicCall("Navigate(const QString&)", url);
	QVBoxLayout *vboxLayout = new QVBoxLayout(this);
	vboxLayout->addWidget(this->_webBrowser);
	vboxLayout->setSpacing(0);
	vboxLayout->setContentsMargins(0,0,0,0);
	connect(this->_webBrowser, SIGNAL(signal(const QString & , int , void * )), this, SLOT(receiverSlot(const QString &, int , void *)));
}

BraowserWrapper::~BraowserWrapper()
{
	disconnect(this->_webBrowser, SIGNAL(signal(const QString & , int , void * )), this, SLOT(receiverSlot(const QString &, int , void *)));
}
void BraowserWrapper::printHtml()
{
	this->_webBrowser->dynamicCall("Print()");
}
void BraowserWrapper::setURL(const QString& url)
{	
	disconnect(this->_webBrowser, SIGNAL(signal(const QString & , int , void * )), this, SLOT(receiverSlot(const QString &, int , void *)));
	this->_startLoadingState = 0;
	this->_documentCompleteState = 0;
	this->_fileDownloadState = 0;
	this->_downloadCompleteState = 0;
	this->_webBrowser->dynamicCall("Navigate(const QString&)", url);	
	connect(this->_webBrowser, SIGNAL(signal(const QString & , int , void * )), this, SLOT(receiverSlot(const QString &, int , void *)));
}

void BraowserWrapper::receiverSlot(const QString &name, int argc, void *argv)
{
	if(name.startsWith("StatusTextChange("))
	{
		if(this->_startLoadingState == 0) 
			this->_startLoadingState = 1;
		if(this->_startLoadingState == 1)
		{
			emit startLoadingSignal();
			this->_startLoadingState = 2;
		}		
	}
	else if (name.startsWith("DocumentComplete(")) 
	{	
		if(this->_documentCompleteState == 0) 
			this->_documentCompleteState = 1;
		if(this->_documentCompleteState == 1)
		{
			emit documentCompleteSignal();
			this->_documentCompleteState = 2;
		}
	}
	else if(name.startsWith("FileDownload("))
	{
		if(this->_fileDownloadState == 0) 
			this->_fileDownloadState = 1;
		if(this->_fileDownloadState == 1)
		{
			emit fileDownloadSignal();
			this->_fileDownloadState = 2;
		}
	}
	else if(name.startsWith("DownloadComplete("))
	{
		if(this->_downloadCompleteState == 0) 
			this->_downloadCompleteState = 1;
		if(this->_downloadCompleteState == 1)
		{
			emit downloadCompleteSignal();
			this->_downloadCompleteState = 2;
		}
	}
}
#endif //RUNTIME_OS_WIN32

#ifdef RUNTIME_OS_MACX
BraowserWrapper::BraowserWrapper(const QString& url,QWidget* parent, QString typeStr):QWidget(parent)
{
    qDebug() << "BraowserWrapper url = " << url << ", typeStr = " << typeStr;
    this->_LoadState = 0;
    this->_type = typeStr;

    this->_webView = new QWebEngineView(this);
    this->_webView->setObjectName(QStringLiteral("WebBrowser"));
    this->_webView->setFocusPolicy(Qt::StrongFocus);

    if(typeStr.compare(QString("PDF")) == 0)
    {
        this->_webView->page()->download(url, QString("Report%1.pdf").arg(QDateTime::currentDateTime().toString("yyMMddhhmmss")));
        connect(this->_webView->page()->profile(), SIGNAL(downloadRequested(QWebEngineDownloadItem*)),
                               this, SLOT(downloadRequestedSlot(QWebEngineDownloadItem*)));
    }
    else if(typeStr.compare(QString("FLASH")) == 0)
    {
        QUrl qFlashUrl = QUrl(url);
        QDesktopServices::openUrl(qFlashUrl);
    }
    else // for html;
    {
        if(url.toLower().contains("http"))
        {
           QUrl qurl = QUrl(url);
           this->_webView->setUrl(qurl);
        }
        else
        {
            QUrl qurl = QUrl::fromLocalFile(url);
            this->_webView->setUrl(qurl);
        }
    }

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
    vboxLayout->addWidget(this->_webView);
    vboxLayout->setSpacing(0);
    vboxLayout->setContentsMargins(0,0,0,0);


    //connect(this->_webView, SIGNAL(loadFinished(bool ok)), this, SLOT(onLoadFinished(bool isOK)));
}

BraowserWrapper::~BraowserWrapper()
{
    //disconnect(this->_webView, SIGNAL(loadFinished(bool ok)), this, SLOT(onLoadFinished(bool isOK)));
}



void BraowserWrapper::reload()
{
    this->_LoadState = 0;
    this->_webView->reload();
}
void BraowserWrapper::setURL(const QString& url)
{
    //disconnect(this->_webView, SIGNAL(loadFinished(bool ok)), this, SLOT(onLoadFinished(bool isOK)));
    this->_LoadState = 0;

    if(url.toLower().contains("http"))
    {
       QUrl qurl = QUrl(url);
       this->_webView->setUrl(qurl);
    }
    else
    {
        QUrl qurl = QUrl::fromLocalFile(url);
        this->_webView->setUrl(qurl);
    }
    //connect(this->_webView, SIGNAL(loadFinished(bool ok)), this, SLOT(onLoadFinished(bool isOK)));
}

void BraowserWrapper::loadFinishedSlot(bool isOK)
{
    if(isOK)
    {
        this->_LoadState = 2;
    }
    else
    {
        this->_LoadState = -1;
    }
    emit loadFinishedSignal(isOK);
}


void BraowserWrapper::downloadRequestedSlot(QWebEngineDownloadItem* download) {

    disconnect(this->_webView->page()->profile(), SIGNAL(downloadRequested(QWebEngineDownloadItem*)),
                           this, SLOT(downloadRequestedSlot(QWebEngineDownloadItem*)));
    if(this->_downloadSavePath == "")
    {
        qDebug() << "downloadRequested Format: " <<  download->savePageFormat();
        qDebug() << "downloadRequested Path: " << download->path();
        qDebug() << "downloadRequested state: " << download->state();
        this->_downloadSavePath = download->path();
        connect(download, SIGNAL(finished()), this, SLOT(downloadCompletedSlot()));
        download->accept();
    } else {
        qDebug() << "downloadRequested download processing, Path: " <<  this->_downloadSavePath;
    }

}


void BraowserWrapper::downloadCompletedSlot()
{
    connect(sender(), SIGNAL(finished()), this, SLOT(downloadCompletedSlot()));
    if(this->_downloadSavePath != "")
    {
        QUrl qsaveFileUrl = QUrl::fromLocalFile(this->_downloadSavePath);
        qDebug() << "onDownloadCompleted qsaveFileUrl: " << qsaveFileUrl;
        //this->_webView->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
        //this->_webView->load(qsaveFileUrl);
        //QDesktopServices::openUrl(qsaveFileUrl);
        emit downloadCompleteSignal(this->_downloadSavePath);
        qDebug() << "downloadCompletedSlot finish, Path: " <<  this->_downloadSavePath;
    }
}

#endif //RUNTIME_OS_MACX
