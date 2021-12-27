#include "Common/ViewHelper.h"
#include "Business/BusinessManager.h"
#include "Business/Model/Trading/Fund.h"
#include "stdafx.h"
#include "Common/viewconstants.h"
#include <QFontMetrics>

bool ViewHelper::canCancel(QSharedPointer<Business::Model::Trading::TradingOrder> order, QString& errorInfo)
{
    errorInfo = "";

    auto quotation = Business::BusinessManager::getInstance()->getComputingManager()->getOverridedQuotationComputingResult(order->getAccountId(), order->getInstrumentId());
    if (quotation->ask.isNullPrice() || quotation->bid.isNullPrice()) return false;

    auto transaction = order->getTransaction();
    if ((order->getPhase() != Phase::Placed && order->getPhase() != Phase::Placing)
        || transaction->getOrderType() == OrderType::SpotTrade)
    {
        return false;
    }

    if (transaction->getSubType() == TransactionSubType::IfDone)
    {
        foreach(auto orderRelation, order->getOrderRelations())
        {
            auto ifOrder = Business::BusinessManager::getInstance()->getTradingManager()->getOrder(orderRelation->getOrderId1());
            if (!ifOrder.isNull() && ifOrder->getPhase() != Phase::Executed) return true;
        }
    }

//    if (transaction->getOrderType() == OrderType::Limit && transaction->getSubType() != TransactionSubType::Match)
//    {
//        LimitOrStop::LimitOrStop limitStop = order->getTradeOption() == TradeOption::Better ? LimitOrStop::Limit : LimitOrStop::Stop;
//        QSharedPointer<Placing::PlacingValidData> placingValidData(new Placing::PlacingValidData(transaction->getBusinessAccount(), transaction->getBusinessInstrument()));
//        PriceError::PriceError error = placingValidData->checkPriceForCancel(order->getSetPrice(), quotation->bid, quotation->ask, order->getIsBuy() ? BuySell::Buy : BuySell::Sell, limitStop);
//        if (error == PriceError::TooCloseMarket)
//        {
//            errorInfo = ViewHelper::toMessage(error, limitStop, transaction->getBusinessInstrument(), true);
//            return false;
//        }
//    }

    return (order->getPhase() == Phase::Placing || order->getPhase() == Phase::Placed)
        && transaction->getOrderType() != OrderType::Market;
}

QString ViewHelper::toMessage(PriceError::PriceError pendingPriceError, LimitOrStop::LimitOrStop limitOrStop, QSharedPointer<Business::Model::Setting::BusinessInstrument> instrument, bool forCancel)
{
    QString priceName = limitOrStop == LimitOrStop::Limit ?
                g_Languages.Tr("PlacingOrder_LimitPrice", "DialogWindow") :
                g_Languages.Tr("PlacingOrder_StopPrice", "DialogWindow");
    switch (pendingPriceError)
    {
    case PriceError::BetweenAskAndBid:
        return g_Languages.Tr("PendingPriceError_BetweenAskAndBid", "DialogWindow").arg(priceName);
    case PriceError::InvalidPrice:
        return g_Languages.Tr("PendingPriceError_InvalidPrice", "DialogWindow").arg(priceName);
    case PriceError::TooFarAwayMarket:
        if (forCancel)
        {
            return g_Languages.Tr("PendingPriceErrorForClose_TooFarAwayMarket", "DialogWindow").arg(priceName);
        }
        else
        {
            return g_Languages.Tr("PendingPriceError_TooFarAwayMarket", "DialogWindow").arg(priceName);
        }
    case PriceError::TooCloseMarket:
    {
        if (forCancel)
        {
            return g_Languages.Tr("PendingPriceErrorForClose_TooCloseMarket", "DialogWindow").arg(priceName);
        }
        else
        {
            int acceptLmtVariation = instrument->getPlaceSetting()->getPlaceSettingDetail(OrderType::Limit)->getAcceptLmtVariation();
            return g_Languages.Tr("PendingPriceError_TooCloseMarket", "DialogWindow").arg(priceName, QString::number(acceptLmtVariation));
        }
    }
    default:
        return "";
    }
}


QString ViewHelper::getOrderTypeString(OrderType::OrderType orderType,
            TradeOption::TradeOption tradeOption,
            TransactionType::TransactionType transactionType,
            TransactionSubType::TransactionSubType transactionSubType, bool isFromConfirm)
{
    if (orderType == OrderType::Limit)
    {
        if (transactionType == TransactionType::OneCancelOther)
        {
            if (tradeOption == TradeOption::Better)
            {
                if(isFromConfirm)
                    return QString("<p>%1").arg(g_Languages.Tr("LimitOfOCO", "MainView"));
                else
                    return g_Languages.Tr("LimitOfOCO", "MainView");
            }
            else if (tradeOption == TradeOption::Stop)
            {
                if(isFromConfirm)
                    return QString("<p>%1").arg(g_Languages.Tr("StopOfOCO", "MainView"));
                else
                    return g_Languages.Tr("StopOfOCO", "MainView");
            }
            else
            {
                return g_Languages.Tr("OCO", "DialogWindow");
            }
        }
        else
        {
            if (tradeOption == TradeOption::Better)
            {
                return g_Languages.Tr("OrderType_Limit", "MainView");
            }
            else
            {
                return g_Languages.Tr("Stop", "MainView");
            }
        }
    }
    else if (orderType == OrderType::OneCancelOther)
    {
        if (tradeOption == TradeOption::Better)
        {
            if(isFromConfirm)
                return QString("<p>%1").arg(g_Languages.Tr("LimitOfOCO", "MainView"));
            else
                return g_Languages.Tr("LimitOfOCO", "MainView");
        }
        else if (tradeOption == TradeOption::Stop)
        {
            if(isFromConfirm)
                return QString("<p>%1").arg(g_Languages.Tr("StopOfOCO", "MainView"));
            else
                return g_Languages.Tr("StopOfOCO", "MainView");
        }
        else
        {
            return g_Languages.Tr("OCO", "PlacingOrder");
        }
    }
    else
    {
        return getCaption(orderType);
    }
}

QString ViewHelper::getOrderTypeForQueryString(OrderType::OrderType orderType, TradeOption::TradeOption tradeOption)
{
    if (orderType == OrderType::Limit || orderType == OrderType::OneCancelOther)
    {
        if (tradeOption == TradeOption::Better)
            return g_Languages.Tr("OrderType_Limit", "MainView");
        else
            return g_Languages.Tr("OrderType_Stop", "MainView");
    }
    else
    {
        return getCaption(orderType);
    }
}

QString ViewHelper::getCaption(OrderType::OrderType type)
{
    switch (type)
    {
    case Common::Enum::OrderType::SpotTrade:
        return g_Languages.Tr("OrderType_Spot", "MainView");
    case Common::Enum::OrderType::Limit:
        return g_Languages.Tr("OrderType_Limit", "MainView");
    case Common::Enum::OrderType::Market:
        return g_Languages.Tr("OrderType_Market", "MainView");
    case Common::Enum::OrderType::MarketOnOpen:
        return g_Languages.Tr("OrderType_MarketOnOpen", "MainView");
    case Common::Enum::OrderType::MarketOnClose:
        return g_Languages.Tr("OrderType_MarketOnClose", "MainView");
    case Common::Enum::OrderType::OneCancelOther:
        return g_Languages.Tr("OrderType_OneCancelOther", "MainView");
    case Common::Enum::OrderType::Risk:
        return g_Languages.Tr("OrderType_Risk", "MainView");
    case Common::Enum::OrderType::Stop:
        return g_Languages.Tr("OrderType_Stop", "MainView");
    case Common::Enum::OrderType::MultipleClose:
        return g_Languages.Tr("OrderType_MultipleClose", "MainView");
    case Common::Enum::OrderType::MarketToLimit:
        return g_Languages.Tr("OrderType_MarketToLimit", "MainView");
    case Common::Enum::OrderType::StopLimit:
        return g_Languages.Tr("OrderType_StopLimit", "MainView");
    case Common::Enum::OrderType::FAK_Market:
        return g_Languages.Tr("OrderType_FAK_Market", "MainView");
    case Common::Enum::OrderType::BinaryOption:
        return g_Languages.Tr("OrderType_BinaryOption", "MainView");
    default:
        return "";
    }
}

QString ViewHelper::getCaption(Frequence::Frequence frequence)
{
    switch (frequence)
    {
    case Frequence::Year:
        return g_Languages.Tr("Frequence_Year", "DialogWindow");
    case Frequence::Month:
        return g_Languages.Tr("Frequence_Month", "DialogWindow");
    case Frequence::Quarterly:
        return g_Languages.Tr("Frequence_Quarterly", "DialogWindow");
    case Frequence::Fortnight:
        return g_Languages.Tr("Frequence_Fortnight", "DialogWindow");
    case Frequence::Occasional:
        return g_Languages.Tr("Frequence_Occasional", "DialogWindow");
    default:
        return "";
    }
}

QString ViewHelper::getCaption(RecalculateRateType::RecalculateRateType recalculateRateType)
{
    switch (recalculateRateType)
    {
    case RecalculateRateType::RecalucateNextMonth:
        return g_Languages.Tr("Physical_RecalucateNextMonth", "DialogWindow");
    case RecalculateRateType::RecalucateNextYear:
        return g_Languages.Tr("Physical_RecalucateNextYear", "DialogWindow");
    default:
        return "";
    }
}

QString ViewHelper::getCaption(PhysicalInstalmentType::PhysicalInstalmentType physicalInstalmentType)
{
    switch (physicalInstalmentType)
    {
    case PhysicalInstalmentType::EqualPrincipal:
        return g_Languages.Tr("Physical_EqualPrincipal", "DialogWindow");
    case PhysicalInstalmentType::EqualInstalment:
        return g_Languages.Tr("Physical_EqualInstalment", "DialogWindow");
    default:
        return "";
    }
}

QString ViewHelper::getCaption(InventoryInProcessStatus::InventoryInProcessStatus type)
{
    switch (type)
    {
    case InventoryInProcessStatus::ExaminationInProcess:
        return g_Languages.Tr("Physical_ExaminationInProcess", "DialogWindow");
    case InventoryInProcessStatus::ExaminationCompleted:
        return g_Languages.Tr("Physical_ExaminationCompleted", "DialogWindow");
    case InventoryInProcessStatus::ContractConverted:
        return g_Languages.Tr("Physical_ContractConverted", "DialogWindow");
    case InventoryInProcessStatus::Submitted:
        return g_Languages.Tr("Physical_Submitted", "DialogWindow");
    case InventoryInProcessStatus::Approved:
        return g_Languages.Tr("Physical_Approved", "DialogWindow");
    case InventoryInProcessStatus::StockIsAvailable:
        return g_Languages.Tr("Physical_StockIsAvailable", "DialogWindow");
    case InventoryInProcessStatus::DeliveryCompletede:
        return g_Languages.Tr("Physical_DeliveryCompletede", "DialogWindow");
    case InventoryInProcessStatus::Canceled:
        return g_Languages.Tr("Physical_Canceled", "DialogWindow");
    default:
        return "";
    }
}

QString ViewHelper::getStstusQStr(Phase::Phase status)
{
    QString statusStr = "";
    switch (status) {
        case Phase::Placing:
            statusStr = g_Languages.Tr("PlacingPhase", "MainView");
            break;
        case Phase::Placed:
            statusStr = g_Languages.Tr("PlacedPhase", "MainView");
            break;
        case Phase::Canceled:
            statusStr = g_Languages.Tr("CanceledPhase", "MainView");
            break;
        case Phase::Executed:
            statusStr = g_Languages.Tr("ExecutedPhase", "MainView");
            break;
        case Phase::Completed:
            statusStr = g_Languages.Tr("CompletedPhase", "MainView");
            break;
        case Phase::Deleted:
            statusStr = g_Languages.Tr("DeletedPhase", "MainView");
            break;
        default:
            break;
    }
    return statusStr;
}

PendingOrderExpireType::PendingOrderExpireType  ViewHelper::getExpireType(ExpireType::ExpireType expireType)
{
    switch (expireType)
    {
    case ExpireType::Day:
        return PendingOrderExpireType::GoodTillDay;
    case ExpireType::Session:
        return PendingOrderExpireType::GoodTillSession;
    default:
        return PendingOrderExpireType::GoodTillDate;
    }
}

QString ViewHelper::getAlertLevelCaption(QSharedPointer<Fund> fund)
{
    if (fund->needAlert())
    {
        AlertLevel::AlertLevel alertLevel = fund->getAlertLevel();
        switch (alertLevel)
        {
        case AlertLevel::Level0:
            return g_Languages.Tr("AlertLevel0Prompt", "MainView");
        case AlertLevel::Level1:
            return g_Languages.Tr("AlertLevel1Prompt", "MainView");
        case AlertLevel::Level2:
            return g_Languages.Tr("AlertLevel2Prompt", "MainView");
        case AlertLevel::Level3:
            return g_Languages.Tr("AlertLevel3Prompt", "MainView");
        default:
            return QString::number(alertLevel);
        }
    }
    return "";
}

QString ViewHelper::getPendingOrderExpireTypeCaption(PendingOrderExpireType::PendingOrderExpireType expireType)
{
    switch (expireType)
    {
    case PendingOrderExpireType::GoodTillDate:
        return g_Languages.Tr("ExpireType_GoodTillDate", "DialogWindow");
    case PendingOrderExpireType::GoodTillDay:
        return g_Languages.Tr("ExpireType_Day", "DialogWindow");
    case PendingOrderExpireType::GoodTillFriday:
        return g_Languages.Tr("ExpireType_GTF", "DialogWindow");
    case PendingOrderExpireType::GoodTillMonth:
        return g_Languages.Tr("ExpireType_GTM", "DialogWindow");
    case PendingOrderExpireType::GoodTillSession:
        return g_Languages.Tr("ExpireType_Session", "DialogWindow");
    default:
        return "";
    }
}

QString ViewHelper::getAccountCaption(int type)
{
    if(type == ViewConstants::BALANCE)
        return g_Languages.Tr("Balance", "MainView");
    else if(type == ViewConstants::NECESSARY)
        return g_Languages.Tr("Necessary", "MainView");
    else if(type == ViewConstants::FLOATING)
        return g_Languages.Tr("TradePLFloat", "MainView");
    else if(type == ViewConstants::EQUITY)
        return g_Languages.Tr("Equity", "MainView");
    else if(type == ViewConstants::NETASSET)
        return g_Languages.Tr("NetAsset", "MainView");
    else if(type == ViewConstants::USABLE)
        return g_Languages.Tr("Usable", "MainView");
    else if(type == ViewConstants::RATIO)
        return g_Languages.Tr("Ratio", "MainView");
    else if(type == ViewConstants::TOTALUNREALISEDSWAP)
        return g_Languages.Tr("TotalUnrealisedSwap", "MainView");
    else if(type == ViewConstants::UNREALISEDPL)
        return g_Languages.Tr("UnrealisedPL", "MainView");
    else if(type == ViewConstants::REMARKS)
        return g_Languages.Tr("AlertLevel", "MainView");
    else if(type == ViewConstants::UNCLEAR)
        return g_Languages.Tr("Unclear", "MainView");
    else if(type == ViewConstants::CREDITAMOUNT)
        return g_Languages.Tr("CreditAmount", "MainView");
    else if(type == ViewConstants::PLEDGEAMOUNT)
        return g_Languages.Tr("PledgeAmount", "MainView");
    else if(type == ViewConstants::FROZENFUND)
        return g_Languages.Tr("FrozenFund", "MainView");
    else if(type == ViewConstants::TOTALPAIDAMOUNT)
        return g_Languages.Tr("TotalPaidAmount", "MainView");
    else if(type == ViewConstants::PARTIALPAYMENTPHYSICALNECESSARY)
        return g_Languages.Tr("PartialPaymentPhysicalNecessary", "MainView");
    else if(type == ViewConstants::TOTALUNPAIDAMOUNT)
        return g_Languages.Tr("TotalUnpaidAmount", "MainView");
    else if(type == ViewConstants::ESTIMATECLOSECOMMISSION)
        return g_Languages.Tr("EstimateCloseCommission", "MainView");
    else if(type == ViewConstants::ESTIMATECLOSELEVY)
        return g_Languages.Tr("EstimateCloseLevy", "MainView");
    else if(type == ViewConstants::REMARKS)
        return g_Languages.Tr("AlertLevel", "MainView");
    else
        return QString::number(type);
}

bool ViewHelper::isContainPoint(const QPoint& leftTop, const QPoint& bottomRight, const QPoint& clickPoint)
{
    if(clickPoint.x() >= leftTop.x() && clickPoint.x() <= bottomRight.x()
            && clickPoint.y() >= leftTop.y() && clickPoint.y() <= bottomRight.y())
        return true;
    else
        return false;
}

QString ViewHelper::getLotFormat(QString lot)
{
    if(!lot.contains(".")) return lot;
    QString newLotStr = "";
    for(int i = lot.count() -1; i >0; i--)
    {
        if(lot[i]==".")
        {
            newLotStr = lot.left(i);
            break;
        }
        else if(lot[i]!="0")
        {
            newLotStr = lot.left(i+1);
            break;
        }
    }
    return newLotStr;
}

QString ViewHelper::geteElidedText(QFont font, QString str, int MaxWidth)
{
    QFontMetrics fontWidth(font);
    int width = fontWidth.width(str);
    if(width>=MaxWidth)
    {
        str = fontWidth.elidedText(str,Qt::ElideRight,MaxWidth);
    }
    return str;
}

int ViewHelper::getFontTextWidthLength(QFont font, QString str)
{
    QFontMetrics fm(font);
    return fm.boundingRect(str).width();
}

bool ViewHelper::isEqualForDouble(double d1, double d2)
{
    return  qAbs(d1-d2)<=0.000001;
}


QString ViewHelper::getConditionCaption(ConditionState::ConditionState condition)
{
    QString conditionStr;
    QString bidText = g_Languages.Tr("Bid", "DialogWindow");
    QString askText = g_Languages.Tr("Ask", "DialogWindow");
    switch (condition)
    {
    case ConditionState::BidLessThan:
        conditionStr = QString("%1 <").arg(bidText);
        break;
    case ConditionState::BidGreaterThan:
        conditionStr = QString("%1 >").arg(bidText);
        break;
    case ConditionState::AskLessthan:
        conditionStr = QString("%1 <").arg(askText);
        break;
    case ConditionState::AskGreaterThan:
        conditionStr = QString("%1 >").arg(askText);
        break;
    }
    return conditionStr;
}
