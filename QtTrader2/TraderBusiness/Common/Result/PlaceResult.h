#ifndef PLACERESULT_H
#define PLACERESULT_H

#include "InvokeResult.h"
#include <QObject>
#include <QUuid>
#include "traderbusiness_global.h"

namespace Common
{
	namespace Result
	{
		class TRADERBUSINESS_EXPORT PlaceResult : public InvokeResult
		{
			Q_OBJECT
		public:
			PlaceResult(QSharedPointer<Communication::AsyncResult> asyncResult, QUuid id)
				: InvokeResult(asyncResult)
			{
				this->_id = id;
				connect(asyncResult.data(), SIGNAL(completed()), this, SLOT(placeResultCompleted()));
			}

			PlaceResult(QSharedPointer<Communication::AsyncResult> asyncResult)
				: InvokeResult(asyncResult)
			{
				connect(asyncResult.data(), SIGNAL(completed()), this, SLOT(placeResultCompleted()));
			}
			
			QString getTransactionError();
			QUuid getId() { return _id; }

		private:
			QUuid _id;

		signals:
			void placeCompleted(QUuid id);

		private slots:
			void placeResultCompleted();
		};
	}
}

#endif	
