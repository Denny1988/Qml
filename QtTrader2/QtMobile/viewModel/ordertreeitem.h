#ifndef OrderTreeItem_H
#define OrderTreeItem_H


// Qt lib import
#include <QList>
#include <QVariant>
#include <QStringList>

class OrderTreeItem
{
private:
    OrderTreeItem *m_parentItem;
    QList<OrderTreeItem*> m_childItems;
    QList<QVariant> m_itemData;

public:
    OrderTreeItem(const QList<QVariant> &data);

    ~OrderTreeItem();

    void appendChild(OrderTreeItem *child);

    OrderTreeItem *child(int row);

    int childCount() const;

    int columnCount() const;

    QVariant data(int column) const;

    int row() const;

    OrderTreeItem *parent();

    void setParent(OrderTreeItem *parent);
};
#endif // OrderTreeItem_H
