/****************************************************************************
** Meta object code from reading C++ file 'droparea.h'
**
** Created: Sun Jun 28 20:53:39 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../droparea.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'droparea.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DropArea[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      14,   10,    9,    9, 0x05,
      40,   31,    9,    9, 0x05,
      66,    9,    9,    9, 0x25,

 // slots: signature, parameters, type, tag, flags
      76,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DropArea[] = {
    "DropArea\0\0img\0getArea(QImage*)\0mimeData\0"
    "changed(const QMimeData*)\0changed()\0"
    "clear()\0"
};

const QMetaObject DropArea::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_DropArea,
      qt_meta_data_DropArea, 0 }
};

const QMetaObject *DropArea::metaObject() const
{
    return &staticMetaObject;
}

void *DropArea::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DropArea))
        return static_cast<void*>(const_cast< DropArea*>(this));
    return QLabel::qt_metacast(_clname);
}

int DropArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: getArea((*reinterpret_cast< QImage*(*)>(_a[1]))); break;
        case 1: changed((*reinterpret_cast< const QMimeData*(*)>(_a[1]))); break;
        case 2: changed(); break;
        case 3: clear(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void DropArea::getArea(QImage * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DropArea::changed(const QMimeData * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, 2, _a);
}
QT_END_MOC_NAMESPACE
