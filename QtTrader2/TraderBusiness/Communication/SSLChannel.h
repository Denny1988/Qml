#ifndef COMMUNICATION_SSLCHANNEL_H
#define COMMUNICATION_SSLCHANNEL_H

#include <QObject>
#include <QSharedPointer>
#include <QMutex>
#include <QTimer>
#include <QUuid>
#include <QJsonObject>
#include <QQueue>
#include "SocketWrapper.h"
#include "Common/Parameter.h"
#include "Package.h"
#include "Heartbeater.h"
#include "Common/Constants/CommunicationConstants.h"
#include "Common/Enum/CommunicationEnum.h"
#include "Common/Enum/SettingOptionEnum.h"

using namespace Common::Enum;
namespace Communication
{	
	class AsyncResult : public QObject
	{
		friend class SslChannel;

		Q_OBJECT

	signals:
		void completed();
	private:
		AsyncResult(const QString& invokeId, int timeout, bool isReadByteArray = false) : _isTimeOut(false)
		{
			this->_invokeId = invokeId;
			this->_timeout = timeout*1000;
			this->_isReadByteArray = isReadByteArray;
			this->_createTime = QDateTime::currentDateTime();
		}
		
		void complete(QString& error, QJsonObject& result)
		{
			this->_error = error;
			this->_jsonResult = result;

			emit this->completed();
		}

		void complete(QString& error, QDomElement& result)
		{
			this->_error = error;
			this->_xmlResult = result;

			emit this->completed();
		}

		void complete(QString& error, QByteArray& result)
		{
			this->_error = error;
			this->_arrayResult = result;

			emit this->completed();
		}

		bool checkIsTimeout()
		{
			QDateTime now = QDateTime::currentDateTime();
			qint64 n = now.toMSecsSinceEpoch();
			qint64 c  = this->_createTime.toMSecsSinceEpoch();
			return (n - c) > this->_timeout;
		}
		void TimeOut()
		{
			this->_isTimeOut = true;
		}
	public:
		bool isTimeOut()
		{
			return this->_isTimeOut;
		}
		bool hasError()
		{
			return !this->_error.isNull() && !this->_error.isEmpty();
		}

		const QString& error()
		{
			return this->_error;
		}

		const QJsonObject& jsonResult()
		{
			return this->_jsonResult;
		}
		const QDomElement& xmlResult()
		{
			return this->_xmlResult;
		}
		const QByteArray& byteArrayResult()
		{
			return this->_arrayResult;
		}
		bool isReadByteArray()
		{
			return this->_isReadByteArray;
		}

		void clearByteArray()
		{
			this->_arrayResult.clear();
		}

		QString getInvokeId()
		{
			return this->_invokeId;
		}

		void complete(QString error)
		{
			this->_error = error;
			emit this->completed();
		}

	private:
		QString _invokeId;
		int _timeout;
		QDateTime _createTime;

		bool _isTimeOut;
		QString _error;
		QJsonObject _jsonResult;
		QDomElement _xmlResult;
		QByteArray _arrayResult;
		bool _isReadByteArray;
	};

	class SslChannel : public QObject
	{
		friend class Heartbeater;

		Q_OBJECT

	public:
		SslChannel(bool useSsl = true, bool isWriteLogOnTraderServerSsl = true, bool useTraderQuotationServer = false, bool isProvisionalConnect = false);
		~SslChannel(); 

	public:
		void setServer(QString serverName, quint16 serverPort);
		void close();
		QSharedPointer<AsyncResult> asyncInvoke(QString methodName, QVector<Common::Parameter> parameters, int timeout = 10, bool isReadByteArray = false);
		void setWatchwords(QStringList watchwords);
		QString getWatchword();
        void setClientId(QString clientId) { _clientId = clientId; }
        QString getClientId() { return _clientId; }
        bool isChangeServer(QString serverName, quint16 serverPort);
		bool isTimeOutFromProvisional();
		void asyncInvoke(QString methodName, QVector<Common::Parameter> parameters, QSharedPointer<AsyncResult> asyncResult);
		QSharedPointer<AsyncResult> getInvokeAsyncResult(QString methodName, int timeout = 10, bool isReadByteArray = false);
        bool isConnected();

	signals:
		void statusChanged(Common::Enum::ConnectionStatus::ConnectionStatus status);
		void notifyReceived(const QDomElement& xmlNotify);
		void quotationReceived(const QString quotations);
		void heartbeatReceived(QSharedPointer<Package>& package);
		void startAsyncTimeoutCheckTimer();
        void stopAsyncTimeoutCheckTimer();
		void startHeartbeaterTimer(int beatInterval);
		void startHeartbeaterCheckTimeout(int beatInterval);
		void getWatchwordsSignal();

	private slots:
		void processSocketStatusChanged(Common::Enum::ConnectionStatus::ConnectionStatus status);
		void processPacket(QSharedPointer<QByteArray> packet);
		void processHeartbeaterStatusChanged(Common::Enum::BeatStatus::BeatStatus beatStatus);
		void checkTimeout();

	private:
		QSharedPointer<SocketWrapper> _socketWrapper;
		QSharedPointer<Heartbeater> heartbeater;
		QThread workThread;		

		QString serverName;
		quint16 serverPort;
		QString sessionId;

		QQueue<QSharedPointer<QByteArray>> _queuedSendData;
		QHash<QString, QSharedPointer<AsyncResult>> _asyncResults;
		QSharedPointer<AsyncResult> _getWatchWorldsResult;
		QSharedPointer<AsyncResult> _recoverResult;

		bool needRecover;
		int recoverFailedCount;
        int reConnectionFailedCount;

		QTimer _asyncTimeoutCheckTimer;
		QDateTime _lastActiveDateTime;
		bool _useSsl;
		bool _isWriteLogOnTraderServerSsl;
		bool _useTraderQuotationServer;
		QStringList _watchwords;
		QStringList _usedWatchwords;
		QString _clientId;
		bool _isCloseWorkThread;
		bool _isProvisionalConnect;

	private slots:
		void innerSetServer(QString serverName, quint16 serverPort);
		void innerAsyncInvoke(QSharedPointer<QByteArray> request, QSharedPointer<AsyncResult> asyncResult);
		void innerAddAsyncResult(QSharedPointer<AsyncResult> asyncResult);
		void innerSendData(QSharedPointer<QByteArray> request);
        void innerRecoverConnection();

	public slots:
		void recoverResultCompleted();
		void startHeartBeatSlot();
		

	private:
		void connect();
        void stop(bool isRecover = false);
		void startRecover();
        void recoverConnection();

		void sendData(QSharedPointer<QByteArray>& data);
		void addAsyncResult(QSharedPointer<AsyncResult>& asyncResult);
		void processResponse(QSharedPointer<Package>& package);
        void processAsyncResult(QSharedPointer<AsyncResult> asyncResult,  QString error, QJsonObject jsonResult, QDomElement xmlResult);
	};	
}
#endif	   //CommunicationChannel_H
