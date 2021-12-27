#ifndef BUSINESS_MODEL_SERVER_BOPOLICYDETAIL_H
#define BUSINESS_MODEL_SERVER_BOPOLICYDETAIL_H
#include "QDecNumber.h"
#include <QUuid>
#include <QXmlStreamReader>
#include "Common/Util/Convert_Common.h"

namespace Business
{
	namespace Model
	{
		namespace Server
		{
			class BOPolicyDetail
			{
			private:
				QUuid _bOBetTypeId;
				QUuid _bOPolicyId;
				int _frequency;
				QDecNumber _maxBet;
				int _maxOrderCount;
				QDecNumber _minBet;
				QDecNumber _odds;
				QDecNumber _stepBet;
				QDecNumber _totalBetLimit;
				int _oddsDecimals;

			public:
				QUuid getBOBetTypeId() const
				{
					return _bOBetTypeId;
				}

				QUuid getBOPolicyId() const
				{
					return _bOPolicyId;
				}

				int getFrequency() const
				{
					return _frequency;
				}

				QDecNumber getMaxBet() const
				{
					return _maxBet;
				}

				int getMaxOrderCount() const
				{
					return _maxOrderCount;
				}

				QDecNumber getMinBet() const
				{
					return _minBet;
				}

				QDecNumber getOdds() const
				{
					return _odds;
				}

				int getOddsDecimals() const
				{
					return _oddsDecimals;
				}

				QDecNumber getStepBet() const
				{
					return _stepBet;
				}

				QDecNumber getTotalBetLimit() const
				{
					return _totalBetLimit;
				}

				void setBOBetTypeId(const QUuid& value)
				{
					_bOBetTypeId = value;
				}

				void setBOPolicyId(const QUuid& value)
				{
					_bOPolicyId = value;
				}

				void setFrequency(const int& value)
				{
					_frequency = value;
				}

				void setMaxBet(const QDecNumber& value)
				{
					_maxBet = value;
				}

				void setMaxOrderCount(const int& value)
				{
					_maxOrderCount = value;
				}

				void setMinBet(const QDecNumber& value)
				{
					_minBet = value;
				}

				void setOdds(const QDecNumber& value)
				{
					_odds = value;
				}

				void setStepBet(const QDecNumber& value)
				{
					_stepBet = value;
				}

				void setTotalBetLimit(const QDecNumber& value)
				{
					_totalBetLimit = value;
				}


				public:
					void update(QXmlStreamReader& xmlStreamReader)
					{
						QXmlStreamAttributes attributes = xmlStreamReader.attributes();
						this->_bOBetTypeId = QUuid(attributes.value("BOBetTypeId").toString());
						this->_bOPolicyId = QUuid(attributes.value("BOPolicyId").toString());
						this->_frequency = attributes.value("Frequency").toInt();
						this->_maxBet = QDecNumber::fromQString(attributes.value("MaxBet").toString());
						this->_maxOrderCount = attributes.value("MaxOrderCount").toInt();
						this->_minBet = QDecNumber::fromQString(attributes.value("MinBet").toString());
						this->_oddsDecimals = Common::getValidDecimals(attributes.value("Odds").toString());
						this->_odds = QDecNumber::fromQString(attributes.value("Odds").toString());
						this->_stepBet = QDecNumber::fromQString(attributes.value("StepBet").toString());
						this->_totalBetLimit = QDecNumber::fromQString(attributes.value("TotalBetLimit").toString());
					}

					void update(const QDomNode& xmlNode)
					{
						QDomNamedNodeMap map = xmlNode.attributes();
						for (int i = 0; i < map.count(); i++)
						{
							QDomNode qDomNode = map.item(i);
							QString name = qDomNode.nodeName();
							QString value = qDomNode.nodeValue();
							if (name == "BOBetTypeId")
								this->_bOBetTypeId = QUuid(value);
							else if (name == "BOPolicyId")
								this->_bOPolicyId = QUuid(value);
							else if (name == "Frequency")
								this->_frequency = value.toInt();
							else if (name == "MaxBet")
								this->_maxBet = QDecNumber::fromQString(value);
							else if (name == "MaxOrderCount")
								this->_maxOrderCount = value.toInt();
							else if (name == "MinBet")
								this->_minBet = QDecNumber::fromQString(value);
							else if (name == "Odds")
							{
								this->_oddsDecimals = Common::getValidDecimals(value);
								this->_odds = QDecNumber::fromQString(value);
							}
							else if (name == "StepBet")
								this->_stepBet = QDecNumber::fromQString(value);
							else if (name == "TotalBetLimit")
								this->_totalBetLimit = QDecNumber::fromQString(value);
						}
					}


			};
		}
	}
}
#endif

