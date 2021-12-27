/****************************************************************************
** Meta object code from reading C++ file 'uploadpicture.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Common/uploadpicture.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uploadpicture.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UploadPicture_t {
    QByteArrayData data[11];
    char stringdata0[151];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UploadPicture_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UploadPicture_t qt_meta_stringdata_UploadPicture = {
    {
QT_MOC_LITERAL(0, 0, 13), // "UploadPicture"
QT_MOC_LITERAL(1, 14, 20), // "uploadFinishedSignal"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 2), // "id"
QT_MOC_LITERAL(4, 39, 19), // "requestFinishedSlot"
QT_MOC_LITERAL(5, 59, 15), // "uploadErrorSlot"
QT_MOC_LITERAL(6, 75, 27), // "QNetworkReply::NetworkError"
QT_MOC_LITERAL(7, 103, 4), // "code"
QT_MOC_LITERAL(8, 108, 17), // "uploadProcessSlot"
QT_MOC_LITERAL(9, 126, 13), // "bytesReceived"
QT_MOC_LITERAL(10, 140, 10) // "bytesTotal"

    },
    "UploadPicture\0uploadFinishedSignal\0\0"
    "id\0requestFinishedSlot\0uploadErrorSlot\0"
    "QNetworkReply::NetworkError\0code\0"
    "uploadProcessSlot\0bytesReceived\0"
    "bytesTotal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UploadPicture[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   37,    2, 0x0a /* Public */,
       5,    1,   38,    2, 0x0a /* Public */,
       8,    2,   41,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    9,   10,

       0        // eod
};

void UploadPicture::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UploadPicture *_t = static_cast<UploadPicture *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->uploadFinishedSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->requestFinishedSlot(); break;
        case 2: _t->uploadErrorSlot((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 3: _t->uploadProcessSlot((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (UploadPicture::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UploadPicture::uploadFinishedSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UploadPicture::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UploadPicture.data,
      qt_meta_data_UploadPicture,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *UploadPicture::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UploadPicture::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UploadPicture.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UploadPicture::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void UploadPicture::uploadFinishedSignal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
