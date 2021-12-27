#ifndef BUSINESS_MODEL_SERVER_DEALINGPOLICYDETAIL_H
#define BUSINESS_MODEL_SERVER_DEALINGPOLICYDETAIL_H
#include "QDecNumber.h"
#include <QUuid>
#include <QXmlStreamReader>

namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class DealingPolicyDetail
			{
			private:
				int _acceptDQVariation;
				int _acceptLmtVariation;
				int _allowedNewTradeSides;
				int _cancelLmtVariation;
				int _acceptCloseLmtVariation;
				QDecNumber _dQQuoteMinLot;
				QUuid _dealingPolicyId;
				QUuid _instrumentId;
				QDecNumber _maxDQLot;
				QDecNumber _maxOtherLot;
				int _priceValidTime;
				int _acceptIfDoneVariation;


			public:
				int getAcceptDQVariation() const
				{
					return _acceptDQVariation;
				}

				int getAcceptLmtVariation() const
				{
					return _acceptLmtVariation;
				}

				int getAllowedNewTradeSides() const
				{
					return _allowedNewTradeSides;
				}

				int getCancelLmtVariation() const
				{
					return _cancelLmtVariation;
				}

				int getAcceptCloseLmtVariation() const
				{
					return _acceptCloseLmtVariation;
				}

				QDecNumber getDQQuoteMinLot() const
				{
					return _dQQuoteMinLot;
				}

				QUuid getDealingPolicyId() const
				{
					return _dealingPolicyId;
				}

				QUuid getInstrumentId() const
				{
					return _instrumentId;
				}

				QDecNumber getMaxDQLot() const
				{
					return _maxDQLot;
				}

				QDecNumber getMaxOtherLot() const
				{
					return _maxOtherLot;
				}

				int getPriceValidTime() const
				{
					return _priceValidTime;
				}

				int getAcceptIfDoneVariation() const
				{
					return _acceptIfDoneVariation;
				}

				void setAcceptDQVariation(const int& value)
				{
					_acceptDQVariation = value;
				}

				void setAcceptLmtVariation(const int& value)
				{
					_acceptLmtVariation = value;
				}

				void setAllowedNewTradeSides(const int& value)
				{
					_allowedNewTradeSides = value;
				}

				void setCancelLmtVariation(const int& value)
				{
					_cancelLmtVariation = value;
				}

				void setAcceptCloseLmtVariation(const int& value)
				{
					_acceptCloseLmtVariation = value;
				}				

				void setDQQuoteMinLot(const QDecNumber& value)
				{
					_dQQuoteMinLot = value;
				}

				void setDealingPolicyId(const QUuid& value)
				{
					_dealingPolicyId = value;
				}

				void setInstrumentId(const QUuid& value)
				{
					_instrumentId = value;
				}

				void setMaxDQLot(const QDecNumber& value)
				{
					_maxDQLot = value;
				}

				void setMaxOtherLot(const QDecNumber& value)
				{
					_maxOtherLot = value;
				}

				void setPriceValidTime(const int& value)
				{
					_priceValidTime = value;
				}

				void setAcceptIfDoneVariation(const int& value)
				{
					_acceptIfDoneVariation = value;
				}

				public:
					void update(QXmlStreamReader& xmlStreamReader)
					{
						QXmlStreamAttributes attributes = xmlStreamReader.attributes();
						this->_acceptDQVariation = attributes.value("AcceptDQVariation").toInt();
						this->_acceptLmtVariation = attributes.value("AcceptLmtVariation").toInt();
						this->_allowedNewTradeSides = attributes.value("AllowedNewTradeSides").toInt();
						this->_cancelLmtVariation = attributes.value("CancelLmtVariation").toInt();
						this->_acceptCloseLmtVariation = attributes.value("AcceptCloseLmtVariation").toInt();
						this->_dQQuoteMinLot = QDecNumber::fromQString(attributes.value("DQQuoteMinLot").toString());
						this->_dealingPolicyId = QUuid(attributes.value("DealingPolicyId").toString());
						this->_instrumentId = QUuid(attributes.value("InstrumentId").toString());
						this->_maxDQLot = QDecNumber::fromQString(attributes.value("MaxDQLot").toString());
						this->_maxOtherLot = QDecNumber::fromQString(attributes.value("MaxOtherLot").toString());
						this->_priceValidTime = attributes.value("PriceValidTime").toInt();
						this->_acceptIfDoneVariation = attributes.value("AcceptIfDoneVariation").toInt();
					}

					void update(const QDomNode& xmlNode)
					{
						QDomNamedNodeMap map = xmlNode.attributes();
						for (int i = 0; i < map.count(); i++)
						{
							QDomNode qDomNode = map.item(i);
							QString name = qDomNode.nodeName();
							QString value = qDomNode.nodeValue();
							if (name == "AcceptDQVariation")
								this->_acceptDQVariation = value.toInt();
							else if (name == "AcceptLmtVariation")
								this->_acceptLmtVariation = value.toInt();
							else if (name == "AllowedNewTradeSides")
								this->_allowedNewTradeSides = value.toInt();
							else if (name == "CancelLmtVariation")
								this->_cancelLmtVariation = value.toInt();
							else if (name == "AcceptCloseLmtVariation")
								this->_acceptCloseLmtVariation = value.toInt();
							else if (name == "DQQuoteMinLot")
								this->_dQQuoteMinLot = QDecNumber::fromQString(value);
							else if (name == "DealingPolicyId")
								this->_dealingPolicyId = QUuid(value);
							else if (name == "InstrumentId")
								this->_instrumentId = QUuid(value);
							else if (name == "MaxDQLot")
								this->_maxDQLot = QDecNumber::fromQString(value);
							else if (name == "MaxOtherLot")
								this->_maxOtherLot = QDecNumber::fromQString(value);
							else if (name == "PriceValidTime")
								this->_priceValidTime = value.toInt();
							else if (name == "AcceptIfDoneVariation")
								this->_acceptIfDoneVariation = value.toInt();
						}
					}

			};
		}
	}
}
#endif

