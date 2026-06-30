/****************************************************************************
** Meta object code from reading C++ file 'submitservice.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../oj-client/src/service/submit/submitservice.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'submitservice.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13SubmitServiceE_t {};
} // unnamed namespace

template <> constexpr inline auto SubmitService::qt_create_metaobjectdata<qt_meta_tag_ZN13SubmitServiceE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SubmitService",
        "loadingChanged",
        "",
        "loading",
        "submittingChanged",
        "submitting",
        "submitPageLoaded",
        "SubmitPageInfo",
        "submitPageInfo",
        "submitPayloadBuilt",
        "languageValue",
        "sourceText",
        "payload",
        "solutionSubmitted",
        "NetworkResult",
        "result",
        "failed",
        "message"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'loadingChanged'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'submittingChanged'
        QtMocHelpers::SignalData<void(bool)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Signal 'submitPageLoaded'
        QtMocHelpers::SignalData<void(const SubmitPageInfo &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Signal 'submitPayloadBuilt'
        QtMocHelpers::SignalData<void(const QString &, const QString &, const QByteArray &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 }, { QMetaType::QString, 11 }, { QMetaType::QByteArray, 12 },
        }}),
        // Signal 'solutionSubmitted'
        QtMocHelpers::SignalData<void(const NetworkResult &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 14, 15 },
        }}),
        // Signal 'failed'
        QtMocHelpers::SignalData<void(const QString &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 17 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SubmitService, qt_meta_tag_ZN13SubmitServiceE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SubmitService::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SubmitServiceE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SubmitServiceE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13SubmitServiceE_t>.metaTypes,
    nullptr
} };

void SubmitService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SubmitService *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->loadingChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->submittingChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->submitPageLoaded((*reinterpret_cast<std::add_pointer_t<SubmitPageInfo>>(_a[1]))); break;
        case 3: _t->submitPayloadBuilt((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QByteArray>>(_a[3]))); break;
        case 4: _t->solutionSubmitted((*reinterpret_cast<std::add_pointer_t<NetworkResult>>(_a[1]))); break;
        case 5: _t->failed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SubmitService::*)(bool )>(_a, &SubmitService::loadingChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (SubmitService::*)(bool )>(_a, &SubmitService::submittingChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (SubmitService::*)(const SubmitPageInfo & )>(_a, &SubmitService::submitPageLoaded, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (SubmitService::*)(const QString & , const QString & , const QByteArray & )>(_a, &SubmitService::submitPayloadBuilt, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (SubmitService::*)(const NetworkResult & )>(_a, &SubmitService::solutionSubmitted, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (SubmitService::*)(const QString & )>(_a, &SubmitService::failed, 5))
            return;
    }
}

const QMetaObject *SubmitService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SubmitService::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SubmitServiceE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SubmitService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void SubmitService::loadingChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void SubmitService::submittingChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void SubmitService::submitPageLoaded(const SubmitPageInfo & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void SubmitService::submitPayloadBuilt(const QString & _t1, const QString & _t2, const QByteArray & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2, _t3);
}

// SIGNAL 4
void SubmitService::solutionSubmitted(const NetworkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void SubmitService::failed(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}
QT_WARNING_POP
