#ifndef COMMUNICATION_SOCKETWRAPPER_H
#define COMMUNICATION_SOCKETWRAPPER_H

#include <QObject>
#include <QSslSocket>
#include <QByteArray>
#include <QStringList>
#include <QJsonArray>
#include <QJsonDocument>
#include <qthread.h>
#include <QWaitCondition>
#include <qmutex.h>
#include "Common/Enum/CommunicationEnum.h"
#include <QSharedPointer>

#define HEADERLENGTH 6
namespace Communication
{	
	class SocketWrapper : public QObject
	{
		Q_OBJECT

	public:
		SocketWrapper(bool useSsl = true, bool isWriteLogOnTraderServerSsl = true);
		~SocketWrapper();

		void connect(QString host, int port, int timeout);
		void send(QSharedPointer<QByteArray> data);
		void close();
		bool isConnected();

	private slots:
		void innerConnect(QString host, int port, int timeout);
		void innerSend(QSharedPointer<QByteArray> data);
		void innerClose();
        void readData();

	signals:
		void statusChanged(Common::Enum::ConnectionStatus::ConnectionStatus status);
		void packetReceived(QSharedPointer<QByteArray>);
		void innerCloseSignal();

	private:
		static int getDataLength(const unsigned char* headBytes);
		bool fullRead(char * buffer, int readLength);
		bool fullWrite(char * buffer, int readLength);
		void fillHeader(char * buffer, int dataLength);
        bool innerConnectSetParameter(QString host, int port, int timeout);

	private:
		QSslSocket	_tradeServerSocket;
		QTcpSocket	_quotationServerSocket;
		bool _useSsl;
		bool _isWriteLogOnTraderServerSsl;
		QThread _workThread;
		
		bool readingHeaderData;
		QSharedPointer<QByteArray> packet;
        bool _isClosed;
	};
}
#endif
