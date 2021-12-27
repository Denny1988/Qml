#include "colormodelitem.h"

ColorModelItem::ColorModelItem()
{

}

QVariant ColorModelItem::data(int column) const
{
    if(column >= _itemData.count()) return "";
    return _itemData[column];
}

void ColorModelItem::setData(int column, QVariant value)
{
    _itemData.insert(column, value);
}
