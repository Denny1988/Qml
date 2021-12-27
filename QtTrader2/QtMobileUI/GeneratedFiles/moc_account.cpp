/****************************************************************************
** Meta object code from reading C++ file 'account.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../viewModel/account.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'account.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Account_t {
    QByteArrayData data[18];
    char stringdata0[186];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Account_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Account_t qt_meta_stringdata_Account = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Account"
QT_MOC_LITERAL(1, 8, 17), // "loginStateChanged"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 9), // "isSuccess"
QT_MOC_LITERAL(4, 37, 17), // "alertLevelChanged"
QT_MOC_LITERAL(5, 55, 10), // "AlertLevel"
QT_MOC_LITERAL(6, 66, 10), // "alertLevel"
QT_MOC_LITERAL(7, 77, 15), // "banlanceChanged"
QT_MOC_LITERAL(8, 93, 10), // "startLogin"
QT_MOC_LITERAL(9, 104, 9), // "loginName"
QT_MOC_LITERAL(10, 114, 8), // "password"
QT_MOC_LITERAL(11, 123, 11), // "getBanlance"
QT_MOC_LITERAL(12, 135, 9), // "isLogined"
QT_MOC_LITERAL(13, 145, 8), // "banlance"
QT_MOC_LITERAL(14, 154, 7), // "Level_0"
QT_MOC_LITERAL(15, 162, 7), // "Level_1"
QT_MOC_LITERAL(16, 170, 7), // "Level_2"
QT_MOC_LITERAL(17, 178, 7) // "Level_3"

    },
    "Account\0loginStateChanged\0\0isSuccess\0"
    "alertLevelChanged\0AlertLevel\0alertLevel\0"
    "banlanceChanged\0startLogin\0loginName\0"
    "password\0getBanlance\0isLogined\0banlance\0"
    "Level_0\0Level_1\0Level_2\0Level_3"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Account[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       3,   52, // properties
       1,   64, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    1,   42,    2, 0x06 /* Public */,
       7,    0,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    2,   46,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      11,    0,   51,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    9,   10,

 // methods: parameters
    QMetaType::Double,

 // properties: name, type, flags
      12, QMetaType::Bool, 0x00095001,
       6, 0x80000000 | 5, 0x00495009,
      13, QMetaType::Double, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,

 // enums: name, flags, count, data
       5, 0x0,    4,   68,

 // enum data: key, value
      14, uint(Account::Level_0),
      15, uint(Account::Level_1),
      16, uint(Account::Level_2),
      17, uint(Account::Level_3),

       0        // eod
};

void Account::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Account *_t = static_cast<Account *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->loginStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->alertLevelChanged((*reinterpret_cast< AlertLevel(*)>(_a[1]))); break;
        case 2: _t->banlanceChanged(); break;
        case 3: _t->startLogin((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: { double _r = _t->getBanlance();
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Account::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Account::loginStateChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Account::*_t)(AlertLevel );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Account::alertLevelChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Account::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Account::banlanceChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        Account *_t = static_cast<Account *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isLogined(); break;
        case 1: *reinterpret_cast< AlertLevel*>(_v) = _t->alertLevel(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->getBanlance(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject Account::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Account.data,
      qt_meta_data_Account,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Account::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Account::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Account.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Account::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Account::loginStateChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Account::alertLevelChanged(AlertLevel _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Account::banlanceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
