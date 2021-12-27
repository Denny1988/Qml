#ifndef BUSINESS_MODEL_TRADING_TRANSACTION_H
#define BUSINESS_MODEL_TRADING_TRANSACTION_H
#include <qglobal.h>
#include <quuid.h>
#include <qdatetime.h>
#include <qstring.h>
#include "Common/Enum/PlaceEnum.h"
#include "Common/Enum/TransactionEnum.h"
#include "Common/Enum/InstrumentEnum.h"
#include "Common/Enum/TransactionErrorEnum.h"
#include "QDecNumber.h"
#include <QDomNode>
#include "Common/Struct/PlacingStruct.h"
#include "traderbusiness_global.h"
#include <QXmlStreamReader>

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class BusinessAccount;
			class BusinessInstrument;
		}
	}
}

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			class TRADERBUSINESS_EXPORT Transaction
			{
			public:
				Transaction();
				~Transaction();
			private:
				QUuid  _id;
				QUuid  _accountId;
				QUuid  _instrumentId;
				QString  _code;
				Common::Enum::TransactionType::TransactionType   _type;
				Common::Enum::TransactionSubType::TransactionSubType  _subType;
				Common::Enum::Phase::Phase   _phase;
				QDateTime  _beginTime;
				QDateTime  _endTime;
				Common::Enum::ExpireType::ExpireType  _expireType;
				QDateTime  _submitTime;
				QDateTime  _executeTime;
				Common::Enum::OrderType::OrderType  _orderType;
				QDecNumber  _contractSize;
				QUuid  _submitorId;
				QUuid  _assigningOrderId;
				Common::Enum::InstrumentCategory::InstrumentCategory  _instrumentCategory;
				QString _placeDetail;
				QSharedPointer<Business::Model::Setting::BusinessAccount> _businessAccount;
				QSharedPointer<Business::Model::Setting::BusinessInstrument> _businessInstrument;

			public:
				void update(const QXmlStreamReader& xmlStreamReader);
				void update(const QDomNode& xmlTransaction);
				void updatePlacingTransaction(QSharedPointer<Common::Struct::Transaction> transaction);
				ExpireType::ExpireType getExpireType(PendingOrderExpireType::PendingOrderExpireType pendingOrderExpireType);
				void setBusinessAccount(QSharedPointer<Business::Model::Setting::BusinessAccount> businessAccount);
				void setBusinessInstrument(QSharedPointer<Business::Model::Setting::BusinessInstrument> businessInstrument);
				QString getAccountCode();
				QString getInstrumentCode();
				QString getInstrumentDescription();
				QUuid getInstrumentSummaryPLFloatCurrnecyId();

			public:

				QUuid getId() const
				{
					return _id;
				}
				

				QString getCode() const
				{
					return _code;
				}


				Common::Enum::TransactionType::TransactionType getType() const
				{
					return _type;
				}


				Common::Enum::TransactionSubType::TransactionSubType getSubType() const
				{
					return _subType;
				}

				Common::Enum::Phase::Phase getPhase() const
				{
					return _phase;
				}

				void setPhase(Common::Enum::Phase::Phase phase)
				{
					this->_phase = phase;
				}
				
				QUuid getAccountId() const
				{
					return _accountId;
				}

				void setAccountId(const QUuid& value)
				{
					_accountId = value;
				}



				QUuid getInstrumentId() const
				{
					return _instrumentId;
				}
				

				QDateTime getBeginTime() const
				{
					return _beginTime;
				}

				
				QDateTime getEndTime() const
				{
					return _endTime;
				}


				Common::Enum::ExpireType::ExpireType getExpireType() const
				{
					return _expireType;
				}


				QDateTime getSubmitTime() const
				{
					return _submitTime;
				}


				QDateTime getExecuteTime() const
				{
					return _executeTime;
				}

				


				Common::Enum::OrderType::OrderType getOrderType() const
				{
					return _orderType;
				}
				

				QDecNumber getContractSize() const
				{
					return _contractSize;
				}
				

				QUuid getSubmitorId() const
				{
					return _submitorId;
				}


				QUuid getAssigningOrderId() const
				{
					return _assigningOrderId;
				}



				/*Common::Enum::TransactionError::TransactionError getError() const
				{
					return _error;
				}

				void setError(int value)
				{
					_error = static_cast<Common::Enum::TransactionError::TransactionError>(value);
				}
*/

				Common::Enum::InstrumentCategory::InstrumentCategory getInstrumentCategory() const
				{
					return _instrumentCategory;
				}
				
				QString getPlaceDetail() const
				{
					return _placeDetail;
				}				

				QSharedPointer<Business::Model::Setting::BusinessInstrument> getBusinessInstrument() const
				{
					return _businessInstrument;
				}

				QSharedPointer<Business::Model::Setting::BusinessAccount> getBusinessAccount() const
				{
					return _businessAccount;
				}
				
				void setSubmitTime(QDateTime value)
				{
					_submitTime = value;
				}
			};
		}
	}
}
#endif
