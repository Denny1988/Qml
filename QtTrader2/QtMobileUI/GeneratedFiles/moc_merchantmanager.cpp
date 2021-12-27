/****************************************************************************
** Meta object code from reading C++ file 'merchantmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Merchant/merchantmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'merchantmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MerchantManager_t {
    QByteArrayData data[13];
    char stringdata0[207];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MerchantManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MerchantManager_t qt_meta_stringdata_MerchantManager = {
    {
QT_MOC_LITERAL(0, 0, 15), // "MerchantManager"
QT_MOC_LITERAL(1, 16, 25), // "updateMerchantListSignals"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 7), // "success"
QT_MOC_LITERAL(4, 51, 25), // "checkMerchantStateSignals"
QT_MOC_LITERAL(5, 77, 17), // "needResetMerchant"
QT_MOC_LITERAL(6, 95, 10), // "needUpdate"
QT_MOC_LITERAL(7, 106, 28), // "updateCurrentResourceSignals"
QT_MOC_LITERAL(8, 135, 10), // "quitSignal"
QT_MOC_LITERAL(9, 146, 20), // "merchantListDownSlot"
QT_MOC_LITERAL(10, 167, 9), // "isSuccess"
QT_MOC_LITERAL(11, 177, 8), // "fileName"
QT_MOC_LITERAL(12, 186, 20) // "merchantResourceSlot"

    },
    "MerchantManager\0updateMerchantListSignals\0"
    "\0success\0checkMerchantStateSignals\0"
    "needResetMerchant\0needUpdate\0"
    "updateCurrentResourceSignals\0quitSignal\0"
    "merchantListDownSlot\0isSuccess\0fileName\0"
    "merchantResourceSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MerchantManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    2,   47,    2, 0x06 /* Public */,
       7,    0,   52,    2, 0x06 /* Public */,
       8,    0,   53,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    2,   54,    2, 0x08 /* Private */,
      12,    2,   59,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,    5,    6,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   10,   11,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   10,   11,

       0        // eod
};

void MerchantManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MerchantManager *_t = static_cast<MerchantManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateMerchantListSignals((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->checkMerchantStateSignals((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->updateCurrentResourceSignals(); break;
        case 3: _t->quitSignal(); break;
        case 4: _t->merchantListDownSlot((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->merchantResourceSlot((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (MerchantManager::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MerchantManager::updateMerchantListSignals)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MerchantManager::*_t)(bool , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MerchantManager::checkMerchantStateSignals)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MerchantManager::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MerchantManager::updateCurrentResourceSignals)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MerchantManager::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MerchantManager::quitSignal)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MerchantManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MerchantManager.data,
      qt_meta_data_MerchantManager,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MerchantManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MerchantManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MerchantManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MerchantManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void MerchantManager::updateMerchantListSignals(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MerchantManager::checkMerchantStateSignals(bool _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MerchantManager::updateCurrentResourceSignals()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MerchantManager::quitSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
