#include <qmetaobject.h>
#include <QByteArray>
#include "SocketWrapper.h"
#include "Common/LogHelper.h"

/*
通信有自己的包头，长度为6
前两个字节为保留字节，除了第一个字节的第一位必须为1外，其他的都为0
后四个字节是包的长度，不包括包头本身
*/
namespace Communication
{
	SocketWrapper::SocketWrapper(bool useSsl, bool isWriteLogOnTraderServerSsl) : _useSsl(useSsl), _isWriteLogOnTraderServerSsl(isWriteLogOnTraderServerSsl)
	{
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << "SocketWrapper::SocketWrapper() ThreadId:" << QThread::currentThreadId();
		//qRegisterMetaType<QSharedPointer<QByteArray>>("QSharedPointer<QByteArray>");
		this->readingHeaderData = true;
        this->_isClosed = false;
        qDebug() << "SocketWrapper::innerClose isClosed false";
		if (this->_useSsl)
			this->_tradeServerSocket.setParent(this);
		else
			this->_quotationServerSocket.setParent(this);
		this->moveToThread(&this->_workThread);
		this->_workThread.start();
		//QObject::connect(this, SIGNAL(innerCloseSignal()), this, SLOT(innerClose()), Qt::BlockingQueuedConnection);	
	}

	SocketWrapper::~SocketWrapper()
	{
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << "SocketWrapper::~SocketWrapper()" << QThread::currentThreadId();
        qDebug() << "SocketWrapper::~SocketWrapper()";
        //this->close();
        if(this->_workThread.isRunning())
            this->_workThread.terminate();
	}

	///------------------public methods---------------------
	void SocketWrapper::connect(QString host, int port, int timeout)
	{
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << "SocketWrapper::connect()" << QThread::currentThreadId();

		QMetaObject::invokeMethod(this, "innerConnect", Qt::QueuedConnection,
                          Q_ARG(QString, host), Q_ARG(int, port), Q_ARG(int, timeout));
	}

	void SocketWrapper::send(QSharedPointer<QByteArray> data)
	{
		QMetaObject::invokeMethod(this, "innerSend", Qt::QueuedConnection,
                          Q_ARG(QSharedPointer<QByteArray>, data));
	}

	void SocketWrapper::close()
	{
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << "SocketWrapper::close()";		
		QMetaObject::invokeMethod(this, "innerClose", Qt::BlockingQueuedConnection);
		bool needWait = this->_workThread.isRunning();		
		if(needWait) 
		{
            this->_workThread.quit();
			bool isok = this->_workThread.wait(3000);
			if (!isok)
			{
				this->_workThread.terminate();
			}
		}
		QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << " SocketWrapper::close end";
		//emit this->innerCloseSignal();
	}

	bool SocketWrapper::isConnected()
	{
		return this->_useSsl ? this->_tradeServerSocket.state() == QSslSocket::ConnectedState : this->_quotationServerSocket.state() == QSslSocket::ConnectedState;
	}

	///------------------public methods---------------------

    bool SocketWrapper::innerConnectSetParameter(QString host, int port, int timeout)
    {
        this->_tradeServerSocket.connectToHostEncrypted(host, port);
        this->_tradeServerSocket.ignoreSslErrors();
        return !this->_tradeServerSocket.waitForEncrypted(timeout);
    }

	///------------------private slots---------------------
	void SocketWrapper::innerConnect(QString host, int port, int timeout)
	{
		bool isConnected = this->isConnected();
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<"SocketWrapper::innerConnect()" << (isConnected ? " already connected" : "not connected") << QThread::currentThreadId();        
        if (isConnected) return;
		bool isConnectFailed = false;
		if (this->_useSsl)
        {
            QLOG_INFO(Common::Constants::LogKeyConstants::Communication) << "SocketWrapper::innerConnect Origin SslProtocol:" << this->_tradeServerSocket.protocol();
            #if defined(RUNTIME_OS_ANDROID)
                this->_tradeServerSocket.setProtocol(QSsl::SslProtocol::SslV3);
                QLOG_INFO(Common::Constants::LogKeyConstants::Communication) << "SocketWrapper::innerConnect New SslProtocol:" << this->_tradeServerSocket.protocol();
            #endif
//            #if defined(RUNTIME_OS_IPHONE)
//                this->_tradeServerSocket.setProtocol(QSsl::SslProtocol::SslV3);
//                QLOG_INFO(Common::Constants::LogKeyConstants::Communication) << "New SslProtocol:" << this->_tradeServerSocket.protocol();
//            #endif
            isConnectFailed = this->innerConnectSetParameter(host, port, timeout);
            QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << "SocketWrapper::innerConnect isConnectFailed:" << isConnectFailed << " protocol:" << this->_tradeServerSocket.protocol();
            QLOG_INFO(Common::Constants::LogKeyConstants::Communication) <<  "SocketWrapper::innerConnect isConnectFailed:" << isConnectFailed;
		}
		else
		{
			this->_quotationServerSocket.connectToHost(host, port);
			isConnectFailed = !this->_quotationServerSocket.waitForConnected(timeout);
		}
		if (isConnectFailed)
		{
            QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<"SocketWrapper::innerConnect() Connect Failed";
			emit this->statusChanged(Common::Enum::ConnectionStatus::ConnectFailed);
		}
		else
		{
			this->readingHeaderData = true;
            this->_isClosed = false;
            qDebug() << "SocketWrapper::innerClose isClosed false";
			if (this->_useSsl)
			{
				QAbstractSocket::SocketState socketState = this->_tradeServerSocket.state();
				QObject::connect(&this->_tradeServerSocket, SIGNAL(readyRead()), this, SLOT(readData()));
				QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SocketWrapper::innerConnect sslSocket.ConnectedState:" << socketState;
			}
			else
			{
				QAbstractSocket::SocketState socketState = this->_quotationServerSocket.state();
				QObject::connect(&this->_quotationServerSocket, SIGNAL(readyRead()), this, SLOT(readData()));
				QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SocketWrapper::innerConnect sslSocket.ConnectedState:" << socketState;
			}			
			emit this->statusChanged(Common::Enum::ConnectionStatus::Connected);
		}
	}

	void SocketWrapper::innerSend(QSharedPointer<QByteArray> data)
	{
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SocketWrapper:innerSend start";
		char header[HEADERLENGTH];
		this->fillHeader(header, data->size());

		if(!this->fullWrite(header, HEADERLENGTH) || !this->fullWrite(data->data(), data->size()))
		{
			QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SocketWrapper:innerSend emit statusChanged(ConnectionBroken) singal";
			emit this->statusChanged(Common::Enum::ConnectionStatus::ConnectionBroken);
		}
	}

	void SocketWrapper::innerClose()
	{ 
        this->_isClosed = true;
        qDebug() << "SocketWrapper::innerClose isClosed true";
        QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << "SocketWrapper::innerClose() ";
		this->disconnect();//disconnect connection between slot to socket's signal
		if (this->_useSsl)
			if (this->_tradeServerSocket.state() == QSslSocket::ConnectedState) this->_tradeServerSocket.close();
		else
			if (this->_quotationServerSocket.state() == QSslSocket::ConnectedState)	this->_quotationServerSocket.close();
		if(this->_workThread.isRunning())
			this->_workThread.quit();
		QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) << "SocketWrapper::innerClose() end ";
	}

	void SocketWrapper::readData()
	{
        if(this->_isClosed) return;
		while ((this->_useSsl ? this->_tradeServerSocket.bytesAvailable() > 0 : this->_quotationServerSocket.bytesAvailable() > 0))
		{
			if(this->readingHeaderData)
			{
				int readLength = HEADERLENGTH;
				if ((this->_useSsl ? this->_tradeServerSocket.bytesAvailable() < readLength : this->_quotationServerSocket.bytesAvailable() < readLength))
				{
					return;
				}
				else
				{
					unsigned char headerBuffer[HEADERLENGTH];
					if(!this->fullRead((char *)headerBuffer, readLength))
					{
						//QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SocketWrapper:readData emit statusChanged(ConnectionBroken) singal";
                        emit this->statusChanged(Common::Enum::ConnectionStatus::ConnectionBroken);
						return;
					}

					qint64 contentLength = SocketWrapper::getDataLength(headerBuffer);
					if(contentLength>0)
					{
						this->readingHeaderData = false;
						this->packet = QSharedPointer<QByteArray>(new QByteArray());
						this->packet->reserve(contentLength);
                        //QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SocketWrapper:readData: content length=" << contentLength;
						/*int header3 = headerBuffer[3] << 8;
						int header4 = headerBuffer[4] << 16;
						int header5 = headerBuffer[5] << 24;
						QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SocketWrapper:OriginPackHeaderContent:" <<  headerBuffer[0] << " " <<  headerBuffer[1]<< " " <<  headerBuffer[2] << " "<<  header3 << " "<<  header4 << " "<<  header5;*/
                    }
				}
			}
			else
			{
				const int bufferLegth = 4096;
				char buffer[bufferLegth];
				
				int readLength = this->_useSsl ? qMin<int>(this->_tradeServerSocket.bytesAvailable(), bufferLegth) : qMin<int>(this->_quotationServerSocket.bytesAvailable(), bufferLegth);
				int packetNeedReadLen = this->packet->capacity() - this->packet->length();
				readLength = qMin<int>(readLength, packetNeedReadLen);
				if(!this->fullRead(buffer, readLength))
				{
                    //QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SocketWrapper:readData emit statusChanged(ConnectionBroken) singal";
					emit this->statusChanged(Common::Enum::ConnectionStatus::ConnectionBroken);
					return;
				}

				this->packet->append(buffer, readLength);
				if(this->packet->capacity() == this->packet->length())
				{
					this->readingHeaderData = true;
					//QLOG_INFO(this->_isWriteLogOnTraderServerSsl ? Common::Constants::LogKeyConstants::Communication : Common::Constants::LogKeyConstants::QuotationServerCommunication) <<  "SocketWrapper:readData emit packetReceived singal";
                    //qDebug() << "SocketWrapper:readData emit packetReceived singal packet：" << this->packet->constData();
                    emit this->packetReceived(this->packet);
				}
			}
		}	  
	}
	///------------------private slots---------------------

	void SocketWrapper::fillHeader(char * header, int dataLength)
	{
		header[0] = 0x80;
		header[1] = 0x00;
		header[2] = dataLength;
		header[3] = dataLength >> 8;
		header[4] = dataLength >> 16;
		header[5] = dataLength >> 24;
	}

	int SocketWrapper::getDataLength(const unsigned char* headBytes)
	{
		int result = 0;
		result += headBytes[2];
		result += headBytes[3] << 8;
		result += headBytes[4] << 16;
		result += headBytes[5] << 24;
		return result;
	}

	bool SocketWrapper::fullWrite(char * buffer, int writeLength)
	{
		int begin = 0;
		int remainLegth = writeLength;

		while(remainLegth > 0)
		{
			qint64 writeLength = this->_useSsl ? this->_tradeServerSocket.write(buffer + begin, remainLegth) : this->_quotationServerSocket.write(buffer + begin, remainLegth);
			if(writeLength == -1) 
			{
				return false;
			}
			else
			{
				begin += writeLength;
				remainLegth -= writeLength;
			}
		}
		return true;
	}

	bool SocketWrapper::fullRead(char * buffer, int readLength)
	{
		int begin = 0;
		int remainLegth = readLength;

		while(remainLegth > 0)
		{
			qint64 readLength = this->_useSsl ? this->_tradeServerSocket.read(buffer + begin, remainLegth) : this->_quotationServerSocket.read(buffer + begin, remainLegth);
			if(readLength == -1) 
			{
				return false;
			}
			else
			{
				begin += readLength;
				remainLegth -= readLength;
			}
		}

		return true;
	}
}
#include "moc_SocketWrapper.cpp"
