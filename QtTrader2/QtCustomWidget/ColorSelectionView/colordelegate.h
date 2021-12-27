#ifndef COLORDELEGATE_H
#define COLORDELEGATE_H
#include <QItemDelegate>
#include <QMap>
#include <QPainter>
#include <QEvent>

class ColorDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ColorDelegate(QMap<int, QString> itemColors, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);

signals:
    void mouseClickSignal(const QModelIndex& index);

private:
    QMap<int, QString> _itemColors;

};

#endif // COLORDELEGATE_H
