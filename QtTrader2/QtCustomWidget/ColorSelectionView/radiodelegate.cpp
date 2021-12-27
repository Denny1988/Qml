#include "radiodelegate.h"
#include <QRadioButton>
#include <QDebug>

RadioDelegate::RadioDelegate(int outerRingMargin, int innerRingMargin, QMap<int, bool> itemChecks, QObject *parent) : QItemDelegate(parent)
{
    this->_itemChecks = itemChecks;
    this->_outerRingMargin = outerRingMargin;
    this->_innerRingMargin = innerRingMargin;
}

void RadioDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect rectHeader = option.rect;
    rectHeader.setHeight(1);
    painter->fillRect(rectHeader, QColor("#c2c2c2"));
    QRect rectBottom = option.rect;
    rectBottom.setY(rectBottom.y() + rectBottom.height() - 1);
    rectBottom.setHeight(1);
    painter->fillRect(rectBottom, QColor("#c2c2c2"));

    bool isCheck = this->_itemChecks[index.row()];
    int marginX = this->_outerRingMargin; //8;
    int marginX2 = this->_innerRingMargin; //15;
    QRect rect = option.rect;
    rect.setX(rect.x() + marginX);
    rect.setY(rect.y() + marginX);
    rect.setWidth(rect.width() - marginX);
    rect.setHeight(rect.height() - marginX);
    painter->setRenderHints(QPainter::Antialiasing, true);
    QBrush oldBrush = QBrush(painter->brush());
    QPen oldPen = QPen(painter->pen());

    QPen pen1(isCheck ? Qt::black : Qt::gray, 1);
    painter->setPen(pen1);
    painter->drawEllipse(rect);

    if(isCheck)
    {
        QRectF boundingRectangle(rect.x() + marginX2, rect.y() + marginX2, rect.width() - marginX2*2, rect.height() - marginX2*2);
        QPainterPath myPath;
        myPath.addEllipse(boundingRectangle);
        QPen pen2(Qt::black, 1);
        painter->setBrush(Qt::black);
        painter->setPen(pen2);
        painter->drawPath(myPath);

        painter->setBrush(oldBrush);
        painter->setPen(oldPen);
    }
}

bool RadioDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    switch (event->type())  {
        case QEvent::MouseButtonRelease:
        {
            emit mouseClickSignal(index);
            return true;
        }
    }
    return false;
}

void RadioDelegate::mouseClickSlot(const QModelIndex& index)
{
    bool isCheck = this->_itemChecks[index.row()];
    if(!isCheck)
    {
        QMapIterator<int, bool> iter(this->_itemChecks);
        while (iter.hasNext()) {
            iter.next();
            this->_itemChecks[iter.key()] = false;
        }
        this->_itemChecks[index.row()] = true;
    }
}
