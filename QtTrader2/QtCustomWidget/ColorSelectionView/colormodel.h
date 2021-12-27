#ifndef COLORMODEL_H
#define COLORMODEL_H
#include <QAbstractTableModel>
#include <QVector>
#include <QSharedPointer>
#include "colormodelitem.h"
#include <QMap>

class ColorModel : public QAbstractTableModel
{    
    Q_OBJECT
public:
    ColorModel(QObject *parent = 0);
    void setMap(const QMap<QVariant, QString> &map, QVariant defaultKey);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QString getCurrentColorName(int row);

public slots:
    void refreshDataSlot();

private:

protected:
    QVector<QSharedPointer<ColorModelItem>> _items;
    QMap<int, Qt::CheckState> _rowCheckStateMap;
    int _colNumberWithCheckBox;
};

#endif // ColorModel_H
