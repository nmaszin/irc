/****************************************************************************
** Meta object code from reading C++ file 'Client.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Client.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Client.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Client_t {
    QByteArrayData data[16];
    char stringdata0[185];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Client_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Client_t qt_meta_stringdata_Client = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Client"
QT_MOC_LITERAL(1, 7, 7), // "Connect"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 12), // "Disconnected"
QT_MOC_LITERAL(4, 29, 16), // "DisconnectServer"
QT_MOC_LITERAL(5, 46, 23), // "DisconnectCurrentServer"
QT_MOC_LITERAL(6, 70, 4), // "Exit"
QT_MOC_LITERAL(7, 75, 8), // "ShowUser"
QT_MOC_LITERAL(8, 84, 14), // "ShowConnection"
QT_MOC_LITERAL(9, 99, 5), // "About"
QT_MOC_LITERAL(10, 105, 4), // "Help"
QT_MOC_LITERAL(11, 110, 20), // "ChangeConnectionItem"
QT_MOC_LITERAL(12, 131, 5), // "index"
QT_MOC_LITERAL(13, 137, 23), // "HandleCommandFromServer"
QT_MOC_LITERAL(14, 161, 7), // "command"
QT_MOC_LITERAL(15, 169, 15) // "HandleUserInput"

    },
    "Client\0Connect\0\0Disconnected\0"
    "DisconnectServer\0DisconnectCurrentServer\0"
    "Exit\0ShowUser\0ShowConnection\0About\0"
    "Help\0ChangeConnectionItem\0index\0"
    "HandleCommandFromServer\0command\0"
    "HandleUserInput"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Client[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    1,   75,    2, 0x08 /* Private */,
       4,    1,   78,    2, 0x08 /* Private */,
       5,    0,   81,    2, 0x08 /* Private */,
       6,    0,   82,    2, 0x08 /* Private */,
       7,    0,   83,    2, 0x08 /* Private */,
       8,    0,   84,    2, 0x08 /* Private */,
       9,    0,   85,    2, 0x08 /* Private */,
      10,    0,   86,    2, 0x08 /* Private */,
      11,    1,   87,    2, 0x08 /* Private */,
      13,    2,   90,    2, 0x08 /* Private */,
      15,    0,   95,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   12,   14,
    QMetaType::Void,

       0        // eod
};

void Client::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Client *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Connect(); break;
        case 1: _t->Disconnected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->DisconnectServer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->DisconnectCurrentServer(); break;
        case 4: _t->Exit(); break;
        case 5: _t->ShowUser(); break;
        case 6: _t->ShowConnection(); break;
        case 7: _t->About(); break;
        case 8: _t->Help(); break;
        case 9: _t->ChangeConnectionItem((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 10: _t->HandleCommandFromServer((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 11: _t->HandleUserInput(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Client::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_Client.data,
    qt_meta_data_Client,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Client::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Client::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Client.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui_Client"))
        return static_cast< Ui_Client*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Client::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
