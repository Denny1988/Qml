#ifndef BUSINESS_MODEL_TRADING_SCRAPDEPOSIT_H
#define BUSINESS_MODEL_TRADING_SCRAPDEPOSIT_H
#include <quuid.h>
#include <qdatetime.h>
#include "Common/Enum/PhysicalEnum.h"
#include "QDecNumber.h"
#include <QDomNode>
#include "Common/Constants/DateTimeConstants.h"
#include <QXmlStreamReader>
using namespace Common::Constants;
namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			class ScrapDeposit
			{
			private:
				QUuid  _accountId;
				QUuid  _id;				
				QUuid  _tradeInstrumentId;
				QUuid  _scrapInstrumentId;
				QString _scrapInstrumentCode;
				int _quantityQDecNumberDigits;
				QString _rawQuantity;
				QString _adjustedQuantity;
				QString _finalQuantity;
				QString _code;
				QString _unit;
				QDateTime _acceptTime;
				Common::Enum::ScrapDepositStatus::ScrapDepositStatus _status;
				QDateTime _submitTime;
			public:
				void update(const QXmlStreamReader& xmlStreamReader)
				{	
					QXmlStreamAttributes attributes = xmlStreamReader.attributes();  
					this->_id = QUuid(attributes.value("Id").toString());
					this->_tradeInstrumentId = QUuid(attributes.value("TradeInstrumentId").toString());
					this->_scrapInstrumentCode = attributes.value("ScrapInstrumentCode").toString();
					this->_quantityQDecNumberDigits = attributes.value("QuantityQDecNumberDigits").toInt();
					this->_rawQuantity = attributes.value("RawQuantity").toString();
					this->_adjustedQuantity= attributes.value("AdjustedQuantity").toString();
					this->_finalQuantity= attributes.value("FinalQuantity").toString();
					this->_code = attributes.value("Code").toString();
					this->_unit = attributes.value("Unit").toString();
					this->_acceptTime = QDateTime::fromString(attributes.value("AcceptTime").toString(), DateTimeConstants::DateTimeFormat);
					this->_status = static_cast<Common::Enum::ScrapDepositStatus::ScrapDepositStatus>(attributes.value("Status").toInt());
					this->_submitTime = QDateTime::fromString(attributes.value("SubmitTime").toString(), DateTimeConstants::DateTimeFormat);
				}

				void update(const QDomNode& xmlScrapDeposit)
				{
					QDomNamedNodeMap map = xmlScrapDeposit.attributes();
					for (int i = 0; i < map.count(); i++)
					{
						QDomNode qDomNode = map.item(i);
						QString name = qDomNode.nodeName();
						QString value = qDomNode.nodeValue();
						if(name == "Id")
							this->_id = QUuid(value);
						else if(name == "TradeInstrumentId")
							this->_tradeInstrumentId = QUuid(value);
						else if(name == "ScrapInstrumentCode")
							this->_scrapInstrumentCode = value;
						else if(name == "QuantityQDecNumberDigits")
							this->_quantityQDecNumberDigits= value.toInt();
						else if(name == "RawQuantity")
							this->_rawQuantity= value;
						else if(name == "AdjustedQuantity")
							this->_adjustedQuantity= value;
						else if(name == "FinalQuantity")
							this->_finalQuantity= value;
						else if(name == "Code")
							this->_code = value;
						else if(name == "Unit")
							this->_unit = value;
						else if(name == "AcceptTime")
							this->_acceptTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
						else if(name == "Status")
							this->_status = static_cast<Common::Enum::ScrapDepositStatus::ScrapDepositStatus>(value.toInt());
						else if(name == "SubmitTime")
							this->_submitTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
					}
				}

			public:

				QUuid getId() const
				{
					return _id;
				}

				void setId(const QUuid& value)
				{
					_id = value;
				}


				QUuid getAccountId() const
				{
					return _accountId;
				}

				void setAccountId(const QUuid& value)
				{
					_accountId = value;
				}


				QUuid getTradeInstrumentId() const
				{
					return _tradeInstrumentId;
				}

				void setTradeInstrumentId(const QUuid& value)
				{
					_tradeInstrumentId = value;
				}


				QUuid getScrapInstrumentId() const
				{
					return _scrapInstrumentId;
				}

				void setScrapInstrumentId(const QUuid& value)
				{
					_scrapInstrumentId = value;
				}


				QString getScrapInstrumentCode() const
				{
					return _scrapInstrumentCode;
				}

				void setScrapInstrumentCode(const QString& value)
				{
					_scrapInstrumentCode = value;
				}


				int getQuantityQDecNumberDigits() const
				{
					return _quantityQDecNumberDigits;
				}

				void setQuantityQDecNumberDigits(int value)
				{
					_quantityQDecNumberDigits = value;
				}

				QString getRawQuantity() const
				{
					return _rawQuantity;
				}

				void setRawQuantity(QString value)
				{
					_rawQuantity = value;
				}

				QString getAdjustedQuantity() const
				{
					return _adjustedQuantity;
				}

				void setAdjustedQuantity(QString value)
				{
					_adjustedQuantity = value;
				}

				QString getFinalQuantity() const
				{
					return _finalQuantity;
				}

				void setFinalQuantity(QString value)
				{
					_finalQuantity = value;
				}


				QString getCode() const
				{
					return _code;
				}

				void setCode(const QString& value)
				{
					_code = value;
				}


				QString getUnit() const
				{
					return _unit;
				}

				void setUnit(const QString& value)
				{
					_unit = value;
				}


				QDateTime getAcceptTime() const
				{
					return _acceptTime;
				}

				void setAcceptTime(const QDateTime& value)
				{
					_acceptTime = value;
				}

				QDateTime getSubmitTime() const
				{
					return _submitTime;
				}

				void setSubmitTime(const QDateTime& value)
				{
					_submitTime = value;
				}


				Common::Enum::ScrapDepositStatus::ScrapDepositStatus getStatus() const
				{
					return _status;
				}

				void setStatus(int value)
				{
					_status = static_cast<Common::Enum::ScrapDepositStatus::ScrapDepositStatus>(value);
				}
			};
		}
	}
}
#endif
