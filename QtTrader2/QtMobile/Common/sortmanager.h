#ifndef PLACINGMANAGER_H
#define PLACINGMANAGER_H

#include <QSharedPointer>
#include <QUuid>
#include <QMap>
#include <QPair>
#include "Common/Enum/PlaceEnum.h"
#include "Common/Struct/PlacingStruct.h"


namespace Business
{
    namespace Model
    {
        namespace Setting
        {
            class DeliverySpecificationDetail;
        }
        namespace Trading {
            class InventoryItemData;
            class PendingInventoryData;
        }
        class Chat;
        class News;
        class Log;
    }
}
namespace Common
{
    namespace Struct
    {
        struct SettingInstrument;
    }
}

using namespace Common::Enum;
using namespace Common::Struct;
using namespace Business::Model::Setting;
using namespace Business::Model::Trading;

class SortManager
{
public:
    static SortManager* getInstance();

protected:
    SortManager();

private:
    static SortManager* m_Instance;

public:
    static bool variantLessThanDeliverySpecification(QSharedPointer<DeliverySpecificationDetail> v1, QSharedPointer<DeliverySpecificationDetail> v2);
    static bool variantLessThanInventoryItem(QSharedPointer<InventoryItemData> v1, QSharedPointer<InventoryItemData> v2);
    static bool variantLessThanPendingInventory(QSharedPointer<PendingInventoryData> v1, QSharedPointer<PendingInventoryData> v2);    
    static bool variantLessThanInstrument(QSharedPointer<Common::Struct::SettingInstrument> v1, QSharedPointer<Common::Struct::SettingInstrument> v2);
    static bool variantLessThanMessage(QSharedPointer<Business::Model::Chat> v1, QSharedPointer<Business::Model::Chat> v2);
    static bool variantLessThanLog(QSharedPointer<Business::Model::Log> v1, QSharedPointer<Business::Model::Log> v2);
    static bool variantLessThanNews(QSharedPointer<Business::Model::News> v1, QSharedPointer<Business::Model::News> v2);
    static bool variantLessThanPair(QPair<QUuid, QString> v1, QPair<QUuid, QString> v2);

};


#endif
