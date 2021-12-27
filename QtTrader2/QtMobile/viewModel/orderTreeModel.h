#ifndef OrderTreeModel_H
#define OrderTreeModel_H

// Qt lib import
#include <QObject>
#include <QAbstractItemModel>
#include <QDebug>

#include "orderTreeItem.h"

class OrderTreeModel: public QAbstractItemModel
{
    Q_OBJECT

private:
    OrderTreeItem *m_rootItem;

public:
    OrderTreeModel(QObject *parent = NULL);

    ~OrderTreeModel();

    QVariant data(const QModelIndex &index, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QHash<int,QByteArray> roleNames() const;

    enum Roles  {
        GroupRole = Qt::UserRole + 5,
        CodeRole =  Qt::UserRole + 6,
    };
    Q_ENUM(Roles)

public slots:
    QAbstractItemModel *model();
};

#endif // OrderTreeModel_H
