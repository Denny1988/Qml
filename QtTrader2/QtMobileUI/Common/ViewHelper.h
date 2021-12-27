#ifndef VIEWHELPER_H
#define VIEWHELPER_H
#include <QSharedPointer>
#include "Common/Enum/PlaceEnum.h"
#include "Common/Enum/CommonEnum.h"
#include "Common/Enum/TransactionEnum.h"
#include "Common/Enum/PhysicalEnum.h"
#include "Common/Enum/InstalmentEnum.h"
#include "Common/Enum/PriceAlertEnum.h"
#include <QPoint>
#include <QFont>

namespace Business
{
	namespace Model
	{
		namespace Trading
		{
			class TradingOrder;
            class Fund;
		}

		namespace Setting
		{
			class BusinessInstrument;
		}
	}
}

using namespace Common::Enum;
using namespace Business::Model::Trading;
using namespace Business::Model::Setting;


class ViewHelper
{
public:
    static bool canCancel(QSharedPointer<TradingOrder> order, QString& errorInfo);
    static QString toMessage(PriceError::PriceError pendingPriceError, LimitOrStop::LimitOrStop limitOrStop, QSharedPointer<BusinessInstrument> instrument, bool forCancel);
    static QString getOrderTypeString(OrderType::OrderType orderType,
                TradeOption::TradeOption tradeOption,
                TransactionType::TransactionType transactionType,
                TransactionSubType::TransactionSubType transactionSubType, bool isFromConfirm  = false);
    static QString getOrderTypeForQueryString(OrderType::OrderType orderType, TradeOption::TradeOption tradeOption);
    static QString getCaption(OrderType::OrderType type);
    static QString getCaption(Frequence::Frequence frequence);
    static QString getCaption(RecalculateRateType::RecalculateRateType recalculateRateType);
    static QString getCaption(PhysicalInstalmentType::PhysicalInstalmentType physicalInstalmentType);
    static QString getCaption(InventoryInProcessStatus::InventoryInProcessStatus type);
    static QString getStstusQStr(Phase::Phase status);
    static PendingOrderExpireType::PendingOrderExpireType  getExpireType(ExpireType::ExpireType expireType);
    static QString getAlertLevelCaption(QSharedPointer<Fund> fund);
    static QString getPendingOrderExpireTypeCaption(PendingOrderExpireType::PendingOrderExpireType expireType);
    static QString getAccountCaption(int type);
    static bool isContainPoint(const QPoint& leftTop, const QPoint& bottomRight, const QPoint& clickPoint);
    static QString getLotFormat(QString lot);
    static QString geteElidedText(QFont font, QString str, int MaxWidth);
    static int getFontTextWidthLength(QFont font, QString str);
    static bool isEqualForDouble(double d1, double d2);
    static QString getConditionCaption(ConditionState::ConditionState condition);
};
#endif
