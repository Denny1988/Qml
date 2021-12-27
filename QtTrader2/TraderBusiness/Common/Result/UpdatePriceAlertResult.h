#ifndef UPDATEPRICEALERTRESULT_H
#define UPDATEPRICEALERTRESULT_H

#include <QObject>
#include "Communication/SSLChannel.h"
#include "traderbusiness_global.h"

namespace Business
{
	namespace Model
	{
		class PriceAlert;
	}
}

namespace Common
{
	namespace Result
	{
		class TRADERBUSINESS_EXPORT UpdatePriceAlertResult: public QObject
		{
			Q_OBJECT

		protected:
			QSharedPointer<Communication::AsyncResult> asyncResult;
		signals:
			void completed();
		public:
			UpdatePriceAlertResult(QSharedPointer<Communication::AsyncResult> asyncResult, const QUuid& id = Q_NULLPTR);
			~UpdatePriceAlertResult();
			const QUuid& getId() const;
			const QString& getPrice() const;
			void setPrice(const QString& price);
			QSharedPointer<Business::Model::PriceAlert> getPriceAlert();
			void setPriceAlert(QSharedPointer<Business::Model::PriceAlert> priceAlert);

			bool hasError()
			{
				return this->asyncResult->hasError();
			}

			const QString& error()
			{
				return this->asyncResult->error();
			}

			

		private:
			QUuid _id;
			QString _price;
			QSharedPointer<Business::Model::PriceAlert> _priceAlert;
		};
	}
}

#endif	
