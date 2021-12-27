/****************************************************************************
** Meta object code from reading C++ file 'orderTreeModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../viewModel/orderTreeModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'orderTreeModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OrderTreeModel_t {
    QByteArrayData data[7];
    char stringdata0[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OrderTreeModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OrderTreeModel_t qt_meta_stringdata_OrderTreeModel = {
    {
QT_MOC_LITERAL(0, 0, 14), // "OrderTreeModel"
QT_MOC_LITERAL(1, 15, 5), // "model"
QT_MOC_LITERAL(2, 21, 19), // "QAbstractItemModel*"
QT_MOC_LITERAL(3, 41, 0), // ""
QT_MOC_LITERAL(4, 42, 5), // "Roles"
QT_MOC_LITERAL(5, 48, 9), // "GroupRole"
QT_MOC_LITERAL(6, 58, 8) // "CodeRole"

    },
    "OrderTreeModel\0model\0QAbstractItemModel*\0"
    "\0Roles\0GroupRole\0CodeRole"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OrderTreeModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       1,   20, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    3, 0x0a /* Public */,

 // slots: parameters
    0x80000000 | 2,

 // enums: name, flags, count, data
       4, 0x0,    2,   24,

 // enum data: key, value
       5, uint(OrderTreeModel::GroupRole),
       6, uint(OrderTreeModel::CodeRole),

       0        // eod
};

void OrderTreeModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OrderTreeModel *_t = static_cast<OrderTreeModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { QAbstractItemModel* _r = _t->model();
            if (_a[0]) *reinterpret_cast< QAbstractItemModel**>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject OrderTreeModel::staticMetaObject = {
    { &QAbstractItemModel::staticMetaObject, qt_meta_stringdata_OrderTreeModel.data,
      qt_meta_data_OrderTreeModel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *OrderTreeModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrderTreeModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OrderTreeModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractItemModel::qt_metacast(_clname);
}

int OrderTreeModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
