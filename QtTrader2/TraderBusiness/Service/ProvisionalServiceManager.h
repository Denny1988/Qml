#ifndef SERVICE_PROVISIONALSERVICEMANAGER_H
#define SERVICE_PROVISIONALSERVICEMANAGER_H
#include "traderbusiness_global.h"
#include <QObject>
#include "Common/Result/Result.h"
#include "Common/Result/PlaceResult.h"
#include <QMutex>
#include <QMap>

namespace Service
{
	class ProvisionalLoginResult : public QObject
	{
		friend class ProvisionalServiceManager;

		Q_OBJECT
	signals :
		void completed();

	private:
		QMap<QString, QSharedPointer<Communication::AsyncResult>> _placeAsyncResults;
		QSharedPointer<Communication::AsyncResult> _asyncResult;
		QString _connectkey;
		QMap<QString, QString> _tranXmlStrings;
		
	public:
		ProvisionalLoginResult(QSharedPointer<Communication::AsyncResult> asyncResult)
		{
			this->_asyncResult = asyncResult;
			QObject::connect(asyncResult.data(), SIGNAL(completed()), this, SIGNAL(completed()));
		}

		~ProvisionalLoginResult()
		{
			QObject::disconnect(this->_asyncResult.data(), SIGNAL(completed()), this, SIGNAL(completed()));
		}

		void addItem(QSharedPointer<Communication::AsyncResult> placeAsyncResult, QString invokeId, QString connectkey, QString tranXmlString)
		{
			this->_connectkey = connectkey;
			this->_placeAsyncResults.insert(invokeId, placeAsyncResult);			
			this->_tranXmlStrings.insert(invokeId, tranXmlString);
		}

		bool hasError()
		{
			return this->_asyncResult->hasError();
		}

		QString error()
		{
			return this->_asyncResult->error();
		}
	};

	class GetTransactionXmlResult : public QObject
	{
		friend class ProvisionalServiceManager;

		Q_OBJECT
			signals :
		void completed();

	private:
		QSharedPointer<Communication::AsyncResult> _placeAsyncResult;
		QSharedPointer<Common::Result::XmlDataResult> _asyncResult;
		QString _invokeId;
		QString _connectkey;
		QString _tranXmlString;

	public:
		GetTransactionXmlResult(QSharedPointer<Common::Result::XmlDataResult> asyncResult, QSharedPointer<Communication::AsyncResult> placeAsyncResult, QString invokeId, QString connectkey)
		{
			this->_asyncResult = asyncResult;
			QObject::connect(asyncResult.data(), SIGNAL(completed()), this, SIGNAL(completed()));
			this->_placeAsyncResult = placeAsyncResult;
			this->_invokeId = invokeId;
			this->_connectkey = connectkey;
		}

		~GetTransactionXmlResult()
		{
			QObject::disconnect(this->_asyncResult.data(), SIGNAL(completed()), this, SIGNAL(completed()));
		}

		bool hasError()
		{
			return this->_asyncResult->hasError();
		}
	};

	class TRADERBUSINESS_EXPORT ProvisionalServiceManager : public QObject
	{		
		Q_OBJECT		
		public:
			static ProvisionalServiceManager* getInstance();
			static QMutex  m_Mutex;

		private:
			ProvisionalServiceManager();

		public:
			QSharedPointer<ProvisionalLoginResult> provisionalLogin(QString serverName, quint16 serverPort, QString connectkey);
			QSharedPointer<Common::Result::PlaceResult> beginProvisionalPlace(QString serverName, quint16 serverPort, QString tranXmlString, QUuid id);
			void close();			

		private:
			static ProvisionalServiceManager* m_Instance;
			QMap<QString, QSharedPointer<Communication::SslChannel>> _provisionalChannels;
			QMap<QString, QSharedPointer<Communication::SslChannel>> _authenticatedProvisionalChannels;		
			QMap<QString, QSharedPointer<ProvisionalLoginResult>> _provisionalLoginResults;
			QMap<QString, QSharedPointer<GetTransactionXmlResult>> _getTransactionXmlResultResults;
			QTimer* _provisionalTimer;

		signals:
			void startProvisionalTimer();
			void connectionStatusChangedSignal(Common::Enum::ConnectionStatus::ConnectionStatus status);
			void getWatchWorldsSignal();

		public slots:
			void  checkProvisionalConnectTimeOutSlot();
			void  authenticatedCompletedSlot();
			void  getTransactionXmlCompletedSlot();
			
	};	
}
#endif
