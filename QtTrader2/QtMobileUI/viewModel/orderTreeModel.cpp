#include "orderTreeModel.h"
#ifndef ORDEROrderTreeModel_H
#define ORDEROrderTreeModel_H

#include "orderTreeModel.h"

OrderTreeModel::OrderTreeModel(QObject *parent):
    QAbstractItemModel(parent)
{
    m_rootItem = new OrderTreeItem( { "Group", "Code" } );

    auto item1 = new OrderTreeItem( { "group001", "code001" } );
    auto item2 = new OrderTreeItem( { "group002", "code002" } );
    auto item3 = new OrderTreeItem( { "group003", "code003" } );
    auto item4 = new OrderTreeItem( { "group004", "code004" } );


    auto item11 = new OrderTreeItem( { "group001_1", "code001_1" } );
    auto item12 = new OrderTreeItem( { "group001_2", "code001_2" } );
    item1->appendChild(item11);
    item1->appendChild(item12);

    auto item21 = new OrderTreeItem( { "group002_1", "code002_1" } );
    auto item22 = new OrderTreeItem( { "group002_2", "code002_2" } );
    item2->appendChild(item21);
    item2->appendChild(item22);

    auto item221 = new OrderTreeItem( { "group002_2_1", "code002_2_1" } );
    item22->appendChild(item221);

    m_rootItem->appendChild(item1);
    m_rootItem->appendChild(item2);
    m_rootItem->appendChild(item3);
    m_rootItem->appendChild(item4);
}

OrderTreeModel::~OrderTreeModel()
{
    delete m_rootItem;
}

int OrderTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return static_cast<OrderTreeItem*>(parent.internalPointer())->columnCount();
    }
    else
    {
        return m_rootItem->columnCount();
    }
}

QHash<int, QByteArray> OrderTreeModel::roleNames() const
{
    QHash<int, QByteArray> names(QAbstractItemModel::roleNames());
    names.insert(GroupRole , "group");
    names.insert(CodeRole , "code");
    return names;
}

QVariant OrderTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    switch(role)
    {
        case GroupRole:
        {
            return static_cast<OrderTreeItem*>(index.internalPointer())->data(0);
        }
        case CodeRole:
        {
            return static_cast<OrderTreeItem*>(index.internalPointer())->data(1);
        }
        case Qt::DisplayRole:
        {
            return static_cast<OrderTreeItem*>(index.internalPointer())->data(index.column());
        }
        default:
        {

            return QVariant();
        }
    }
    
}

Qt::ItemFlags OrderTreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return 0;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant OrderTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return m_rootItem->data(section);
    }

    return QVariant();
}

QModelIndex OrderTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    OrderTreeItem *parentItem;
    if(!parent.isValid())
    {
        parentItem = m_rootItem;
    }
    else
    {
        parentItem = static_cast<OrderTreeItem*>(parent.internalPointer());
    }

    OrderTreeItem *childItem = parentItem->child(row);
    if(childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex OrderTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return QModelIndex();
    }

    OrderTreeItem *childItem = static_cast<OrderTreeItem*>(index.internalPointer());
    OrderTreeItem *parentItem = childItem->parent();

    if(parentItem == m_rootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int OrderTreeModel::rowCount(const QModelIndex &parent) const
{
    OrderTreeItem *parentItem;
    if(parent.column() > 0)
    {
        return 0;
    }

    if(!parent.isValid())
    {
        parentItem = m_rootItem;
    }
    else
    {
        parentItem = static_cast<OrderTreeItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

QAbstractItemModel *OrderTreeModel::model()
{
    return this;
}

#endif // ORDEROrderTreeModel_H

