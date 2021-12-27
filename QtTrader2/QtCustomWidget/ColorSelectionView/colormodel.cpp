#include "colormodel.h"

ColorModel::ColorModel(QObject *parent) : QAbstractTableModel(parent)
{
}

int ColorModel::rowCount(const QModelIndex & parent) const
{
    return _items.count();
}

int ColorModel::columnCount(const QModelIndex & parent) const
{
    return 3;
}

QVariant ColorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
            return QVariant();
    if (role == Qt::TextAlignmentRole) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
    } else if (role == Qt::DisplayRole) {
        return _items[index.row()]->data(index.column());
    }
    return QVariant();
}

QVariant ColorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
//        if (role != Qt::DisplayRole)
//                return QVariant();
    return QVariant();
}

void ColorModel::setMap(const QMap<QVariant, QString> &map, QVariant defaultKey)
{
    this->_colNumberWithCheckBox = 0;
    QMapIterator<QVariant, QString> iter(map);
    while (iter.hasNext()) {
        iter.next();
        QSharedPointer<ColorModelItem> colorModelItem(new ColorModelItem());
        colorModelItem->setData(0, defaultKey == iter.key());
        colorModelItem->setData(1, iter.value());
        colorModelItem->setData(2, iter.value());
        this->_items.append(colorModelItem);
    }
    emit dataChanged(QModelIndex(), QModelIndex());
}

QString ColorModel::getCurrentColorName(int row)
{
    return this->_items[row]->data(2).toString();
}

void ColorModel::refreshDataSlot()
{
    emit dataChanged(QModelIndex(), QModelIndex());
}
