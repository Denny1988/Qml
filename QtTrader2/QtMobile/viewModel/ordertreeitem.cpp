#include "orderTreeItem.h"


OrderTreeItem::OrderTreeItem(const QList<QVariant> &data)
{
    m_itemData = data;
}

OrderTreeItem::~OrderTreeItem()
{
    qDeleteAll(m_childItems);
}

void OrderTreeItem::appendChild(OrderTreeItem *item)
{
    item->setParent(this);
    m_childItems.append(item);
}

OrderTreeItem *OrderTreeItem::child(int row)
{
    return m_childItems.value(row);
}

int OrderTreeItem::childCount() const
{
    return m_childItems.count();
}

int OrderTreeItem::columnCount() const
{
    return m_itemData.count();
}

QVariant OrderTreeItem::data(int column) const
{
    return m_itemData.value(column);
}

OrderTreeItem *OrderTreeItem::parent()
{
    return m_parentItem;
}

void OrderTreeItem::setParent(OrderTreeItem *parent)
{
    m_parentItem = parent;
}

int OrderTreeItem::row() const
{
    if(!m_parentItem) { return 0; }

    return m_parentItem->m_childItems.indexOf(const_cast<OrderTreeItem*>(this));
}
