/****************************************************************************
** Meta object code from reading C++ file 'Plateau.h'
**
** Created: Wed Aug 14 00:36:00 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Plateau.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Plateau.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Plateau[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,    9,    8,    8, 0x05,
      36,    9,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      56,    8,    8,    8, 0x0a,
      64,    8,    8,    8, 0x08,
      77,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Plateau[] = {
    "Plateau\0\0newScore\0scoreChanged(int)\0"
    "hiScoreChanged(int)\0shoot()\0updateMove()\0"
    "updateSeulement()\0"
};

void Plateau::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Plateau *_t = static_cast<Plateau *>(_o);
        switch (_id) {
        case 0: _t->scoreChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->hiScoreChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->shoot(); break;
        case 3: _t->updateMove(); break;
        case 4: _t->updateSeulement(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Plateau::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Plateau::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Plateau,
      qt_meta_data_Plateau, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Plateau::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Plateau::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Plateau::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Plateau))
        return static_cast<void*>(const_cast< Plateau*>(this));
    return QWidget::qt_metacast(_clname);
}

int Plateau::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Plateau::scoreChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Plateau::hiScoreChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
