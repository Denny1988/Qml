#include "sortmanager.h"
#include "stdafx.h"
#include "Business/Model/Setting/DeliverySpecificationDetail.h"
#include "Business/Model/Trading/InventoryData.h"
#include "Business/Model/Trading/TradingOrder.h"
#include "Business/Model/Trading/PendingInventoryData.h"
#include "Business/Model/Chat.h"
#include "Business/Model/News.h"
#include "Business/Model/Log.h"
#include "Common/Struct/SettingStruct.h"

using namespace Common;

SortManager* SortManager:: m_Instance = Q_NULLPTR;

SortManager* SortManager::getInstance()
{
    if(!m_Instance)
    {
        m_Instance = new SortManager;
    }
    return m_Instance;
}

SortManager::SortManager()
{
}

bool SortManager::variantLessThanDeliverySpecification(QSharedPointer<DeliverySpecificationDetail> v1, QSharedPointer<DeliverySpecificationDetail> v2)
{
    return v1->getSize().toDouble() > v2->getSize().toDouble();
}

bool SortManager::variantLessThanInventoryItem(QSharedPointer<InventoryItemData> v1, QSharedPointer<InventoryItemData> v2)
{
    return v1->_tradingOrder->getExecuteTradeDay() > v2->_tradingOrder->getExecuteTradeDay();
}

bool SortManager::variantLessThanPendingInventory(QSharedPointer<PendingInventoryData> v1, QSharedPointer<PendingInventoryData> v2)
{
    return v1->getSubmitTime() > v2->getSubmitTime();//Descending
}

bool SortManager::variantLessThanInstrument(QSharedPointer<Common::Struct::SettingInstrument> v1, QSharedPointer<Common::Struct::SettingInstrument> v2)
{
    if(v1->_sequence == v2->_sequence)
        return v1->_description.toLower() < v2->_description.toLower();
    else
        return v1->_sequence < v2->_sequence;
}

bool SortManager::variantLessThanMessage(QSharedPointer<Business::Model::Chat> v1, QSharedPointer<Business::Model::Chat> v2)
{
    return v1->getPublishQDateTime() > v2->getPublishQDateTime();//Descending
}

bool SortManager::variantLessThanLog(QSharedPointer<Business::Model::Log> v1, QSharedPointer<Business::Model::Log> v2)
{
    return v1->getTime() > v2->getTime();//Descending
}

bool SortManager::variantLessThanNews(QSharedPointer<Business::Model::News> v1, QSharedPointer<Business::Model::News> v2)
{
    return v1->getPublishQDateTime() > v2->getPublishQDateTime();//Descending
}

bool SortManager::variantLessThanPair(QPair<QUuid, QString> v1, QPair<QUuid, QString> v2)
{
    return v1.second < v2.second;//Ascending
}
