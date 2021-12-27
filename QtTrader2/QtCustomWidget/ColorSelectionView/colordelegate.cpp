#include "colordelegate.h"
#include <QLabel>
#include <QDebug>

ColorDelegate::ColorDelegate(QMap<int, QString> itemColors, QObject *parent) : QItemDelegate(parent)
{
    this->_itemColors = itemColors;
}

void ColorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect rectHeader = option.rect;
    rectHeader.setHeight(1);
    painter->fillRect(rectHeader, QColor("#c2c2c2"));
    QRect rectContent = option.rect;
    rectContent.setY(rectContent.y() + 1);
    rectContent.setHeight(rectContent.height() - 2);
    painter->fillRect(rectContent, QColor(this->_itemColors[index.row()]));
    QRect rectBottom = option.rect;
    rectBottom.setY(rectBottom.y() + rectBottom.height() - 1);
    rectBottom.setHeight(1);
    painter->fillRect(rectBottom, QColor("#c2c2c2"));
}

bool ColorDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    switch (event->type())  {
        case QEvent::MouseButtonRelease:
        {
            emit mouseClickSignal(index);
            emit model->dataChanged(QModelIndex(), QModelIndex());
            return true;
        }
    }
    return false;
}
