#ifndef BUSINESS_MODEL_PRICEALERT_H
#define BUSINESS_MODEL_PRICEALERT_H
#include <quuid.h>
#include <qdatetime.h>
#include <QDomNode>
#include "Common/Enum/PriceAlertEnum.h"
#include <QSharedPointer>
#include "Common/Constants/DateTimeConstants.h"
#include "DateTimeHelper.h"

using namespace Common::Constants;
using namespace Common::Enum;
namespace Business
{
	namespace Model
	{
		class PriceAlert
		{
		private:
			QUuid  _id;
			QUuid  _userId;
			QUuid  _quotePolicyId;
			QUuid  _instrumentId;
			QString  _instrumentDescription;
			QString _price;
			ConditionState::ConditionState _condition;
			QDateTime _expirationTime;
			int _action;
			QString _color;
			Common::Enum::PriceAlertState::PriceAlertState _state;
			PriceAlertModifyFlag::PriceAlertModifyFlag _modifyFlag;
			QString _hitPrice;
			QDateTime _hitTime;

		public:
			void update(const QDomNode& xmlChat)
			{
				QDomNamedNodeMap map = xmlChat.attributes();
				for (int i = 0; i < map.count(); i++)
				{
					QDomNode qDomNode = map.item(i);
					QString name = qDomNode.nodeName();
					QString value = qDomNode.nodeValue();
					if(name == "Id")
						this->_id = QUuid(value);
					else if(name == "UserId")
						this->_userId = QUuid(value);
					else if(name == "InstrumentId")
						this->_instrumentId = QUuid(value);
					else if(name == "QuotePolicyId"  && value != "")
						this->_quotePolicyId = QUuid(value);
					else if(name == "Price")
						this->_price = value;
					else if(name == "Condition")
						this->_condition = static_cast<ConditionState::ConditionState>(value.toInt());
					else if(name == "ExpirationTime")
						this->_expirationTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
					else if(name == "Action")
						this->_action = value.toInt();
					else if(name == "Color")
						this->_color = value;
					else if(name == "State")
						this->_state = static_cast<Common::Enum::PriceAlertState::PriceAlertState>(value.toInt());
					else if(name == "HitPrice")
						this->_hitPrice = value;
					else if(name == "HitTime" && value != "")
						this->_hitTime = QDateTime::fromString(value, DateTimeConstants::DateTimeFormat);
				}
			}

			QSharedPointer<PriceAlert> clone()
			{
				QSharedPointer<PriceAlert> priceAlert(new PriceAlert);
				priceAlert->_id = this->_id;
				priceAlert->_userId = this->_userId;
				priceAlert->_quotePolicyId = this->_quotePolicyId;
				priceAlert->_action = this->_action;
				priceAlert->_state = this->_state;
				priceAlert->_price = this->_price;
				priceAlert->_expirationTime = this->_expirationTime;
				priceAlert->_condition = this->_condition;
				priceAlert->_hitPrice = this->_hitPrice;
				priceAlert->_hitTime = this->_hitTime;
				priceAlert->_instrumentId = this->_instrumentId;
				priceAlert->_instrumentDescription = this->_instrumentDescription;
				priceAlert->_color = this->_color;
				priceAlert->_modifyFlag = PriceAlertModifyFlag::Update;
				return priceAlert;
			}

			void updatePriceAlert(QSharedPointer<PriceAlert> priceAlert)
			{
				this->_id = priceAlert->_id;
				this->_userId = priceAlert->_userId;
				this->_quotePolicyId = priceAlert->_quotePolicyId;
				this->_action = priceAlert->_action;
				this->_state = priceAlert->_state;
				this->_instrumentId = priceAlert->_instrumentId;
				this->_price = priceAlert->_price;
				this->_condition = priceAlert->_condition;
				this->_expirationTime = priceAlert->_expirationTime;
				this->_color = priceAlert->_color;

				this->_modifyFlag = PriceAlertModifyFlag::Add;
				this->_instrumentDescription= priceAlert->_instrumentDescription;
				this->_hitPrice = priceAlert->_hitPrice;
				this->_hitTime = priceAlert->_hitTime;
			}

			/*QString getConditionString()
			{
				QString conditionStr;
				QString bidText = g_Languages.Tr("Bid", "MainListView");
				QString askText = g_Languages.Tr("Ask", "MainListView");
				switch (_condition)
				{
				case BidLessThan:
					conditionStr = QString("%1 <").arg(bidText);
					break;
				case BidGreaterThan:
					conditionStr = QString("%1 >").arg(bidText);
					break;
				case AskLessthan:
					conditionStr = QString("%1 <").arg(askText);
					break;
				case AskGreaterThan:
					conditionStr = QString("%1 >").arg(askText);
					break;
				}
				return conditionStr;
			}*/

		public:
			QUuid getId() const
			{
				return _id;
			}

			void setId(const QUuid& value)
			{
				_id = value;
			}

			QUuid getUserId() const
			{
				return _userId;
			}

			void setUserId(const QUuid& value)
			{
				_userId = value;
			}

			QUuid getQuotePolicyId() const
			{
				return _quotePolicyId;
			}

			void setQuotePolicyId(const QUuid& value)
			{
				_quotePolicyId = value;
			}

			QUuid getInstrumentId() const
			{
				return _instrumentId;
			}

			void setInstrumentId(const QUuid& value)
			{
				_instrumentId = value;
			}

			QString getInstrumentDescription() const
			{
				return _instrumentDescription;
			}

			void setInstrumentDescription(const QString& value)
			{
				_instrumentDescription = value;
			}


			QString getPrice() const
			{
				return _price;
			}

			void setPrice(const QString& value)
			{
				_price = value;
			}

			int getCondition() const
			{
				return _condition;
			}

			ConditionState::ConditionState getConditionEnum() const
			{
				return _condition;
			}

			void setCondition(int value)
			{
				_condition = static_cast<ConditionState::ConditionState>(value);
			}

			void setCondition(ConditionState::ConditionState value)
			{
				_condition = value;
			}

			QDateTime getExpirationTime() const
			{
				return _expirationTime;
			}

            QString getExpirationTimeStr()
			{
				return DateTimeHelper::toDateTimeString(_expirationTime);
			}

			void setExpirationTime(const QDateTime& value)
			{
				_expirationTime = value;
			}		

			int getAction() const
			{
				return _action;
			}

			void setAction(int value)
			{
				_action = value;
			}

			QString getColor() const
			{
				return _color;
			}

			void setColor(const QString& value)
			{
				_color = value;
			}

			Common::Enum::PriceAlertState::PriceAlertState getState() const
			{
				return _state;
			}

			void setState(int value)
			{
				_state = static_cast<Common::Enum::PriceAlertState::PriceAlertState>(value);
			}

			Common::Enum::PriceAlertModifyFlag::PriceAlertModifyFlag getModifyFlag() const
			{
				return _modifyFlag;
			}

			void setModifyFlag(int value)
			{
				_modifyFlag = static_cast<Common::Enum::PriceAlertModifyFlag::PriceAlertModifyFlag>(value);
			}

			QString getHitPrice() const
			{
				return _hitPrice;
			}

			void setHitPrice(const QString& value)
			{
				_hitPrice = value;
			}

			QDateTime getHitTime() const
			{
				return _hitTime;
			}

            QString getHitTimeStr()
			{
				return DateTimeHelper::toDateTimeString(_hitTime);
			}

			void setHitTime(const QDateTime& value)
			{
				_hitTime = value;
			}
		};
	}
}
#endif
