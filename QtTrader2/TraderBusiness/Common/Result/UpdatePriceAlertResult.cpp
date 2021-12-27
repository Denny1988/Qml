#include "UpdatePriceAlertResult.h"
#include "Business/Model/PriceAlert.h"

namespace Common
{
	namespace Result
	{
		UpdatePriceAlertResult::UpdatePriceAlertResult(QSharedPointer<Communication::AsyncResult> asyncResult, const QUuid& id)
		{
			this->asyncResult = asyncResult;
			this->_id = id;
			QObject::connect(this->asyncResult.data(), SIGNAL(completed()), this, SIGNAL(completed()));
		}

		UpdatePriceAlertResult::~UpdatePriceAlertResult()
		{
			QObject::disconnect(this->asyncResult.data(), SIGNAL(completed()), this, SIGNAL(completed()));
		}

		const QUuid& UpdatePriceAlertResult::getId() const
		{
			return _id;
		}
		const QString& UpdatePriceAlertResult::getPrice() const
		{
			return _price;
		}
		void UpdatePriceAlertResult::setPrice(const QString& price)
		{
			_price = price;
		}
		QSharedPointer<Business::Model::PriceAlert> UpdatePriceAlertResult::getPriceAlert()
		{
			return _priceAlert;
		}
		void UpdatePriceAlertResult::setPriceAlert(QSharedPointer<Business::Model::PriceAlert> priceAlert)
		{
			_priceAlert = priceAlert;
		}
	}
}
#include "moc_UpdatePriceAlertResult.cpp"