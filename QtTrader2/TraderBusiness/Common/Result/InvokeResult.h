#ifndef INVOKERESULT_H
#define INVOKERESULT_H

#include <QObject>
#include "Communication/SSLChannel.h"
#include "traderbusiness_global.h"

namespace Common
{
	namespace Result
	{
		class TRADERBUSINESS_EXPORT InvokeResult: public QObject
		{
			Q_OBJECT

		protected:
			QSharedPointer<Communication::AsyncResult> asyncResult;
		signals:
			void completed();
		public:
			InvokeResult(QSharedPointer<Communication::AsyncResult> asyncResult);
			~InvokeResult();

			bool hasError()
			{
				return this->asyncResult->hasError();
			}

			const QString& error()
			{
				return this->asyncResult->error();
			}

			const QJsonObject& getJsonResult()
			{
				return this->asyncResult->jsonResult();
			}

			const QDomElement& getXmlResult()
			{
				return this->asyncResult->xmlResult();
			}
			const QByteArray& getByteArrayResult()
			{
				return this->asyncResult->byteArrayResult();
			}
			const bool isTimeOut()
			{
				return this->asyncResult->isTimeOut();
			}

			void clearByteArray()
			{
				this->asyncResult->clearByteArray();
			}
		};
	}
}

#endif	
