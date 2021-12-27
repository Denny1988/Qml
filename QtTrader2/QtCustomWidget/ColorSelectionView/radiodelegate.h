#ifndef RADIODELEGATE_H
#define RADIODELEGATE_H
#include <QItemDelegate>
#include <QMap>
#include <QPainter>
#include <QEvent>

class RadioDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    RadioDelegate(int outerRingMargin, int innerRingMargin, QMap<int, bool> itemChecks, QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);

signals:
    void mouseClickSignal(const QModelIndex& index);

public slots:
    void mouseClickSlot(const QModelIndex& index);

private:
    QMap<int, bool> _itemChecks;
    int _outerRingMargin;
    int _innerRingMargin;
};

#endif // RADIODELEGATE_H
