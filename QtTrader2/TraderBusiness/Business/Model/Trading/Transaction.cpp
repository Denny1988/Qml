#include "Transaction.h"
#include "Business/Model/Setting/BusinessAccount.h"
#include "Business/Model/Setting/BusinessInstrument.h"
#include "Common/Constants/DateTimeConstants.h"
using namespace Common::Constants;
namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			Transaction::Transaction()
			{
				this->_executeTime = DateTimeConstants::MinQDateTime;
			}

			Transaction::~Transaction()
			{
			}

			void Transaction::update(const QXmlStreamReader& xmlStreamReader)
			{
				QXmlStreamAttributes attributes = xmlStreamReader.attributes();  
				this->_id = QUuid(attributes.value("ID").toString());
				this->_accountId = QUuid(attributes.value("AccountID").toString());
				this->_instrumentId = QUuid(attributes.value("InstrumentID").toString());
				this->_code = attributes.value("Code").toString();
				this->_type = static_cast<Common::Enum::TransactionType::TransactionType>(attributes.value("Type").toInt());
				this->_subType = static_cast<Common::Enum::TransactionSubType::TransactionSubType>(attributes.value("SubType").toInt());
				this->_phase = static_cast<Common::Enum::Phase::Phase>(attributes.value("Phase").toInt());
				this->_beginTime = QDateTime::fromString(attributes.value("BeginTime").toString(), DateTimeConstants::DateTimeFormat);
				this->_endTime = QDateTime::fromString(attributes.value("EndTime").toString(), DateTimeConstants::DateTimeFormat);
				this->_expireType = static_cast<Common::Enum::ExpireType::ExpireType>(attributes.value("ExpireType").toInt());
				this->_submitTime = QDateTime::fromString(attributes.value("SubmitTime").toString(), DateTimeConstants::DateTimeFormat);
				this->_submitorId = QUuid(attributes.value("SubmitorID").toString());
				this->_executeTime = QDateTime::fromString(attributes.value("ExecuteTime").toString(), DateTimeConstants::DateTimeFormat);
				this->_assigningOrderId = QUuid(attributes.value("AssigningOrderID").toString());
				this->_contractSize= QDecNumber::fromQString(attributes.value("ContractSize").toString());
				this->_orderType = static_cast<Common::Enum::OrderType::OrderType>(attributes.value("OrderType").toInt());
				this->_instrumentCategory = static_cast<Common::Enum::InstrumentCategory::InstrumentCategory>(attributes.value("InstrumentCategory").toInt());
				this->_placeDetail = attributes.value("PlaceDetail").toString();	
			}

			void Transaction::update(const QDomNode& xmlTransaction)
			{
				QDomNamedNodeMap map = xmlTransaction.attributes();
				for (int i = 0; i < map.count(); i++)
				{
					QDomNode qDomNode = map.item(i);
					QString name = qDomNode.nodeName();
					QString value = qDomNode.nodeValue();
					if(name == "ID")
						this->_id = QUuid(value);
					else if(name == "AccountId")
						this->_accountId = QUuid(value);
					else if(name == "InstrumentId")
						this->_instrumentId = QUuid(value);
					else if(name == "Code")
						this->_code = value;
					else if(name == "Type")
						this->_type = static_cast<Common::Enum::TransactionType::TransactionType>(value.toInt());
					else if(name == "SubType")
						this->_subType = static_cast<Common::Enum::TransactionSubType::TransactionSubType>(value.toInt());
					else if(name == "Phase")
						this->_phase = static_cast<Common::Enum::Phase::Phase>(value.toInt());
					else if(name == "BeginTime")
						this->_beginTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
					else if(name == "EndTime")
						this->_endTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
					else if(name == "ExpireType")
						this->_expireType = static_cast<Common::Enum::ExpireType::ExpireType>(value.toInt());
					else if(name == "SubmitTime")
						this->_submitTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
					else if(name == "SubmitorId")
						this->_submitorId = QUuid(value);
					else if(name == "ExecuteTime")
						this->_executeTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
					else if(name == "AssigningOrderID")
						this->_assigningOrderId = QUuid(value);
					else if(name == "ContractSize")
						this->_contractSize= QDecNumber::fromQString(value);
					else if(name == "OrderType")
						this->_orderType = static_cast<Common::Enum::OrderType::OrderType>(value.toInt());
					else if(name == "InstrumentCategory")
						this->_instrumentCategory = static_cast<Common::Enum::InstrumentCategory::InstrumentCategory>(value.toInt());
					else if(name == "PlaceDetail")
						this->_placeDetail = value;					
				}
			}

			void Transaction::updatePlacingTransaction(QSharedPointer<Common::Struct::Transaction> transaction)
			{
				this->_id = transaction->_id;
				this->_accountId = transaction->_accountId;
				this->_instrumentId = transaction->_instrumentId;
				this->_type = transaction->_type;
				this->_subType = transaction->_subType;
				this->_phase = transaction->_phase;
				this->_expireType = this->getExpireType(transaction->_expireType);
				this->_orderType = transaction->_orderType;
				this->_contractSize = transaction->_contractSize;
				this->_instrumentCategory = transaction->_instrumentCategory;
				this->_submitorId = transaction->_submitorId;
				this->_assigningOrderId = transaction->_assigningOrderId;
			}

			ExpireType::ExpireType Transaction::getExpireType(PendingOrderExpireType::PendingOrderExpireType pendingOrderExpireType)
			{
				switch (pendingOrderExpireType)
				{
				case PendingOrderExpireType::GoodTillDay:
					return ExpireType::Day;
				case PendingOrderExpireType::GoodTillSession:
					return ExpireType::Session;
				default:
					return ExpireType::GTD;
				}
			}

			void Transaction::setBusinessAccount(QSharedPointer<Business::Model::Setting::BusinessAccount> businessAccount)
			{
				this->_businessAccount = businessAccount;
			}

			void Transaction::setBusinessInstrument(QSharedPointer<Business::Model::Setting::BusinessInstrument> businessInstrument)
			{
				this->_businessInstrument = businessInstrument;
				this->_instrumentId = businessInstrument->getId();
			}

			QString Transaction::getAccountCode()
			{
				if(!this->_businessAccount.isNull())
					return this->_businessAccount->getCode();
				else
					return "";
			}

			QString Transaction::getInstrumentCode()
			{
				if(!this->_businessInstrument.isNull())
					return this->_businessInstrument->getCode();
				else
					return "";
			}

			QString Transaction::getInstrumentDescription()
			{
				if(!this->_businessInstrument.isNull())
					return this->_businessInstrument->getDescription();
				else
					return "";
			}

			QUuid Transaction::getInstrumentSummaryPLFloatCurrnecyId()
			{
				return this->_businessAccount->getIsMultiCurrency() ? this->_businessInstrument->getCurrencyId() : this->_businessAccount->getCurrencyId();
			}

		}
	}
}
