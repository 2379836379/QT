/****************************************************************************
** Meta object code from reading C++ file 'favoriteproblemservice.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../oj-client/src/service/favorite/favoriteproblemservice.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'favoriteproblemservice.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN22FavoriteProblemServiceE_t {};
} // unnamed namespace

template <> constexpr inline auto FavoriteProblemService::qt_create_metaobjectdata<qt_meta_tag_ZN22FavoriteProblemServiceE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "FavoriteProblemService",
        "foldersLoaded",
        "",
        "QList<FavoriteFolderInfo>",
        "folders",
        "folderCreated",
        "FavoriteFolderInfo",
        "folderInfo",
        "folderRemoved",
        "folderId",
        "folderFavoritesLoaded",
        "QList<ProblemPageInfo>",
        "favorites",
        "favoriteLoaded",
        "ProblemPageInfo",
        "problemPageInfo",
        "favoriteSavedToFolder",
        "favoriteRemovedFromFolder",
        "problemUrl",
        "failed",
        "message"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'foldersLoaded'
        QtMocHelpers::SignalData<void(const QList<FavoriteFolderInfo> &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'folderCreated'
        QtMocHelpers::SignalData<void(const FavoriteFolderInfo &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Signal 'folderRemoved'
        QtMocHelpers::SignalData<void(qint64)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 9 },
        }}),
        // Signal 'folderFavoritesLoaded'
        QtMocHelpers::SignalData<void(qint64, const QList<ProblemPageInfo> &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 9 }, { 0x80000000 | 11, 12 },
        }}),
        // Signal 'favoriteLoaded'
        QtMocHelpers::SignalData<void(const ProblemPageInfo &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 14, 15 },
        }}),
        // Signal 'favoriteSavedToFolder'
        QtMocHelpers::SignalData<void(qint64, const ProblemPageInfo &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 9 }, { 0x80000000 | 14, 15 },
        }}),
        // Signal 'favoriteRemovedFromFolder'
        QtMocHelpers::SignalData<void(qint64, const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 9 }, { QMetaType::QString, 18 },
        }}),
        // Signal 'failed'
        QtMocHelpers::SignalData<void(const QString &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 20 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<FavoriteProblemService, qt_meta_tag_ZN22FavoriteProblemServiceE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject FavoriteProblemService::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22FavoriteProblemServiceE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22FavoriteProblemServiceE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN22FavoriteProblemServiceE_t>.metaTypes,
    nullptr
} };

void FavoriteProblemService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<FavoriteProblemService *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->foldersLoaded((*reinterpret_cast<std::add_pointer_t<QList<FavoriteFolderInfo>>>(_a[1]))); break;
        case 1: _t->folderCreated((*reinterpret_cast<std::add_pointer_t<FavoriteFolderInfo>>(_a[1]))); break;
        case 2: _t->folderRemoved((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 3: _t->folderFavoritesLoaded((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QList<ProblemPageInfo>>>(_a[2]))); break;
        case 4: _t->favoriteLoaded((*reinterpret_cast<std::add_pointer_t<ProblemPageInfo>>(_a[1]))); break;
        case 5: _t->favoriteSavedToFolder((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<ProblemPageInfo>>(_a[2]))); break;
        case 6: _t->favoriteRemovedFromFolder((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 7: _t->failed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (FavoriteProblemService::*)(const QList<FavoriteFolderInfo> & )>(_a, &FavoriteProblemService::foldersLoaded, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoriteProblemService::*)(const FavoriteFolderInfo & )>(_a, &FavoriteProblemService::folderCreated, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoriteProblemService::*)(qint64 )>(_a, &FavoriteProblemService::folderRemoved, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoriteProblemService::*)(qint64 , const QList<ProblemPageInfo> & )>(_a, &FavoriteProblemService::folderFavoritesLoaded, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoriteProblemService::*)(const ProblemPageInfo & )>(_a, &FavoriteProblemService::favoriteLoaded, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoriteProblemService::*)(qint64 , const ProblemPageInfo & )>(_a, &FavoriteProblemService::favoriteSavedToFolder, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoriteProblemService::*)(qint64 , const QString & )>(_a, &FavoriteProblemService::favoriteRemovedFromFolder, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (FavoriteProblemService::*)(const QString & )>(_a, &FavoriteProblemService::failed, 7))
            return;
    }
}

const QMetaObject *FavoriteProblemService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FavoriteProblemService::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22FavoriteProblemServiceE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FavoriteProblemService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void FavoriteProblemService::foldersLoaded(const QList<FavoriteFolderInfo> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void FavoriteProblemService::folderCreated(const FavoriteFolderInfo & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void FavoriteProblemService::folderRemoved(qint64 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void FavoriteProblemService::folderFavoritesLoaded(qint64 _t1, const QList<ProblemPageInfo> & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}

// SIGNAL 4
void FavoriteProblemService::favoriteLoaded(const ProblemPageInfo & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void FavoriteProblemService::favoriteSavedToFolder(qint64 _t1, const ProblemPageInfo & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void FavoriteProblemService::favoriteRemovedFromFolder(qint64 _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2);
}

// SIGNAL 7
void FavoriteProblemService::failed(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}
QT_WARNING_POP
