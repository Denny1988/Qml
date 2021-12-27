#ifndef COLORMODELITEM_H
#define COLORMODELITEM_H
#include <QVector>
#include <QVariant>

class ColorModelItem
{
public:
    ColorModelItem();
    QVariant data(int column) const;
    void setData(int column, QVariant value);

private:
    QVector<QVariant> _itemData;

};

#endif // COLORMODELITEM_H
