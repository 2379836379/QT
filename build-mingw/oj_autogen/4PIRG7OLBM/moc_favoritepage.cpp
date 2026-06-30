/****************************************************************************
** Meta object code from reading C++ file 'favoritepage.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../oj-client/src/ui/pages/favoritepage.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'favoritepage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN12FavoritePageE_t {};
} // unnamed namespace

template <> constexpr inline auto FavoritePage::qt_create_metaobjectdata<qt_meta_tag_ZN12FavoritePageE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "FavoritePage",
        "backRequested",
        "",
        "homeRequested",
        "themeToggleRequested",
        "dark",
        "refreshRequested",
        "exportRequested",
        "importRequested",
        "createFolderRequested",
        "folderName",
        "folderRemoveRequested",
        "folderId",
        "folderSelected",
        "favoriteSelected",
        "title",
        "url",
        "favoriteRemoveRequested",
        "problemUrl"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'backRequested'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'homeRequested'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'themeToggleRequested'
        QtMocHelpers::SignalData<void(bool)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Signal 'refreshRequested'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'exportRequested'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'importRequested'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'createFolderRequested'
        QtMocHelpers::SignalData<void(const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 },
        }}),
        // Signal 'folderRemoveRequested'
        QtMocHelpers::SignalData<void(qint64, const QString &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 12 }, { QMetaType::QString, 10 },
        }}),
        // Signal 'folderSelected'
        QtMocHelpers::SignalData<void(qint64, const QString &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 12 }, { QMetaType::QString, 10 },
        }}),
        // Signal 'favoriteSelected'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 15 }, { QMetaType::QString, 16 },
        }}),
        // Signal 'favoriteRemoveRequested'
        QtMocHelpers::SignalData<void(qint64, const QString &, const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 12 }, { QMetaType::QString, 10 }, { QMetaType::QString, 18 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<FavoritePage, qt_meta_tag_ZN12FavoritePageE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject FavoritePage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12FavoritePageE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12FavoritePageE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12FavoritePageE_t>.metaTypes,
    nullptr
} };

void FavoritePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<FavoritePage *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->backRequested(); break;
        case 1: _t->homeRequested(); break;
        case 2: _t->themeToggleRequested((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->refreshRequested(); break;
        case 4: _t->exportRequested(); break;
        case 5: _t->importRequested(); break;
        case 6: _t->createFolderRequested((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->folderRemoveRequested((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 8: _t->folderSelected((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 9: _t->favoriteSelected((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 10: _t->favoriteRemoveRequested((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (FavoritePage::*)()>(_a, &FavoritePage::backRequested, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoritePage::*)()>(_a, &FavoritePage::homeRequested, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoritePage::*)(bool )>(_a, &FavoritePage::themeToggleRequested, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoritePage::*)()>(_a, &FavoritePage::refreshRequested, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoritePage::*)()>(_a, &FavoritePage::exportRequested, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoritePage::*)()>(_a, &FavoritePage::importRequested, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoritePage::*)(const QString & )>(_a, &FavoritePage::createFolderRequested, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoritePage::*)(qint64 , const QString & )>(_a, &FavoritePage::folderRemoveRequested, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoritePage::*)(qint64 , const QString & )>(_a, &FavoritePage::folderSelected, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoritePage::*)(const QString & , const QString & )>(_a, &FavoritePage::favoriteSelected, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoritePage::*)(qint64 , const QString & , const QString & )>(_a, &FavoritePage::favoriteRemoveRequested, 10))
            return;
    }
}

const QMetaObject *FavoritePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FavoritePage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12FavoritePageE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int FavoritePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void FavoritePage::backRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void FavoritePage::homeRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void FavoritePage::themeToggleRequested(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void FavoritePage::refreshRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void FavoritePage::exportRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void FavoritePage::importRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void FavoritePage::createFolderRequested(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void FavoritePage::folderRemoveRequested(qint64 _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2);
}

// SIGNAL 8
void FavoritePage::folderSelected(qint64 _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1, _t2);
}

// SIGNAL 9
void FavoritePage::favoriteSelected(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1, _t2);
}

// SIGNAL 10
void FavoritePage::favoriteRemoveRequested(qint64 _t1, const QString & _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1, _t2, _t3);
}
QT_WARNING_POP
