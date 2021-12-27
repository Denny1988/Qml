#ifndef BUSINESS_MODEL_INITIALQUOTATION_H
#define BUSINESS_MODEL_INITIALQUOTATION_H
#include <quuid.h>
#include <qdatetime.h>
#include <string>
#include "Common/Util/Convert_Common.h"
#include "Common/Constants/DateTimeConstants.h"
#include "Common/Constants/QuotationConstants.h"
using namespace Common::Constants;
namespace Business
{
	namespace Model
	{
		class InitialQuotation
		{
		public:
			InitialQuotation()
			{
				_ask = "";
				_bid = "";
				_high = "";
				_low = "";
				_askHigh = "";
				_askLow = "";
				_open = "";
				_prevClose = "";
				_volume = "";
				_totalVolume = "";
				_priceTrend = 0;
				this->_timestamp = QuotationConstants::_OrginTime;
				this->_utcTimestamp = QuotationConstants::_OrginTime;
			}
		private:
			QUuid  _accountId;
			QUuid  _QuotePolicyId;
			QUuid  _instrumentId;
			QString  _ask;
			QString  _bid;
			QString  _high;
			QString  _low;
			QString  _askHigh;
			QString  _askLow;
			QDateTime  _timestamp;
			QDateTime  _utcTimestamp;
			QString  _open;
			QString  _askOpen;
			QString  _prevClose;
			QString  _prevAsk;
			QString  _prevBid;
			QString  _volume;
			QString  _totalVolume;
			int _priceTrend;//NoChange:0, Up: 1, Down: 2

		public:
			void update(const QDomNode& xmlQuotation)
			{
				QDomNamedNodeMap map = xmlQuotation.attributes();
				for (int i = 0; i < map.count(); i++)
				{
					QDomNode qDomNode = map.item(i);
					QString name = qDomNode.nodeName();
					QString value = qDomNode.nodeValue();
					if (name == "AccountId")
						this->_accountId = QUuid(value);
					else if (name == "InstrumentId")
						this->_instrumentId = QUuid(value);
					else if (name == "QuotePolicyId")
						this->_QuotePolicyId = QUuid(value);
					else if (name == "Ask")
						this->_ask = value;
					else if (name == "Bid")
						this->_bid = value;
					else if (name == "High")
						this->_high = value;
					else if (name == "Low")
						this->_low = value;
					else if (name == "AskHigh")
						this->_askHigh = value;
					else if (name == "AskLow")
						this->_askLow = value;
					else if (name == "Timestamp")
						this->_timestamp = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
					else if (name == "Open")
						this->_open = value;
					else if (name == "AskOpen")
						this->_askOpen = value;
					else if (name == "PrevClose")
						this->_prevClose = value;
					else if (name == "PrevAsk")
						this->_prevAsk = value;
					else if (name == "PrevBid")
						this->_prevBid = value;
					else if (name == "Volume")
						this->_volume = value;
					else if (name == "TotalVolume")
						this->_totalVolume = value;
				}
			}

			void updateQuotation(QString ask, QString bid, QString high, QString low, QString askHigh, QString askLow, QDateTime timestamp)
			{
				if (ask != "") this->_ask = ask;
				if (bid != "") this->_bid = bid;
				if (high != "") this->_high = high;
				if (low != "") this->_low = low;
				if (askHigh != "") this->_askHigh = askHigh;
				if (askLow != "") this->_askLow = askLow;
				if (timestamp != this->_timestamp) this->_timestamp = timestamp;
			}

			void updateQuotation(QSharedPointer<Model::InitialQuotation> quotation)
			{
				this->_QuotePolicyId = quotation->getQuotePolicyId();
				this->_instrumentId = quotation->getInstrumentId();
				if (quotation->getAsk() != "") this->_ask = quotation->getAsk();
				if (quotation->getBid() != "") this->_bid = quotation->getBid();
				if (quotation->getHigh() != "") this->_high = quotation->getHigh();
				if (quotation->getLow() != "") this->_low = quotation->getLow();
				if (quotation->getAskHigh() != "") this->_askHigh = quotation->getAskHigh();
				if (quotation->getAskLow() != "") this->_askLow = quotation->getAskLow();
				if (quotation->getOpen() != "") this->_open = quotation->getOpen();
				if (quotation->getAskOpen() != "") this->_askOpen = quotation->getAskOpen();
				if (quotation->getPrevClose() != "") this->_prevClose = quotation->getPrevClose();
				if (quotation->getPrevAsk() != "") this->_prevAsk = quotation->getPrevAsk();
				if (quotation->getPrevBid() != "") this->_prevBid = quotation->getPrevBid();
				if (quotation->getVolume() != "") this->_volume = quotation->getVolume();
				if (quotation->getTotalVolume() != "") this->_totalVolume = quotation->getTotalVolume();
				if (quotation->getTimestamp() != this->_timestamp) this->_timestamp = quotation->getTimestamp();
			}

			int getMaxPriceLength()
			{
				int maxLength = 0;
				maxLength = qMax(maxLength, this->_ask.length());
				maxLength = qMax(maxLength, this->_bid.length());
				/*maxLength = qMax(maxLength, this->_high.length());
				maxLength = qMax(maxLength, this->_low.length());
				maxLength = qMax(maxLength, this->_open.length());
				maxLength = qMax(maxLength, this->_prevClose.length());*/
				return maxLength;
			}

		public:
			/*const QUuid getAccountId()
			{
				return _accountId;
			}*/

			const QUuid getQuotePolicyId()
			{
				return _QuotePolicyId;
			}

			const QUuid getInstrumentId()
			{
				return _instrumentId;
			}

			QString getAsk()
			{
				return _ask;
			}

			QString getBid()
			{
				return _bid;
			}

			QString getHigh()
			{
				return _high;
			}

			QString getLow()
			{
				return _low;
			}

			QString getAskHigh()
			{
				return _askHigh;
			}

			QString getAskLow()
			{
				return _askLow;
			}

			QDateTime getTimestamp()
			{
				return _timestamp;
			}

			QDateTime getUtcTimestamp()
			{
				return _utcTimestamp;
			}		

			QString getOpen()
			{
				return _open;
			}

			QString getAskOpen()
			{
				return _askOpen;
			}

			QString getPrevClose()
			{
				return _prevClose;
			}

			QString getPrevAsk()
			{
				return _prevAsk;
			}

			QString getPrevBid()
			{
				return _prevBid;
			}

			QString getVolume()
			{
				return _volume;
			}

			QString getTotalVolume()
			{
				return _totalVolume;
			}

			int getPriceTrend()
			{
				return _priceTrend;
			}


			void setInstrumentID(const QUuid& value)
			{
				_instrumentId = value;
			}

			void setQuotePolicyID(const QUuid& value)
			{
				_QuotePolicyId = value;
			}

			/*void setAccountID(const QUuid& value)
			{
				_accountId = value;
			}*/

			void setAsk(const QString& value)
			{
				_ask = value;
			}

			void setBid(const QString& value)
			{
				_bid = value;
			}

			void setHigh(const QString& value)
			{
				_high = value;
			}

			void setLow(const QString& value)
			{
				_low = value;
			}

			void setAskHigh(const QString& value)
			{
				_askHigh = value;
			}

			void setAskLow(const QString& value)
			{
				_askLow = value;
			}

			void setTimestamp(QDateTime value)
			{
				_timestamp = value;
			}

			void setUtcTimestamp(QDateTime value)
			{
				_utcTimestamp = value;
			}

            void setOpen(const QString& value)
            {
                _open = value;
            }
			
			void setAskOpen(const QString& value)
			{
				_askOpen = value;
			}

			void setPrevClose(const QString& value)
			{
				_prevClose = value;
			}

			void setPrevAsk(const QString& value)
			{
				_prevAsk = value;
			}

			void setPrevBid(const QString& value)
			{
				_prevBid = value;
			}

			void setVolume(const QString& value)
			{
				_volume = value;
			}

			void setTotalVolume(const QString& value)
			{
				_totalVolume = value;
			}

			void setPriceTrend(int value)
			{
				_priceTrend = value;
			}
		};
	}
}
#endif
