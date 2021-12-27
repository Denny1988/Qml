#ifndef VIEWSTRUCT_H
#define VIEWSTRUCT_H

#include <QString>

struct AccountItemStruct
{
    int TypeId;
    QString TypeName;
    QString Currency;
    QString Value;
    bool IsChildCurrency;
    bool IsRemark;
    bool IsMultiCurrency;

    AccountItemStruct(int typeId, QString typeName, QString currency, QString value, bool isChildCurrency, bool isRemark, bool isMultiCurrency)
    {
        TypeId = typeId;
        TypeName = typeName;
        Currency = currency;
        Value = value;
        IsChildCurrency = isChildCurrency;
        IsRemark = isRemark;
        IsMultiCurrency = isMultiCurrency;
    }
};


#endif // VIEWSTRUCT_H
