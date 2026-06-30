/****************************************************************************
** Meta object code from reading C++ file 'openjudgeclient.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../oj-client/src/network/openjudgeclient.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'openjudgeclient.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN15OpenJudgeClientE_t {};
} // unnamed namespace

template <> constexpr inline auto OpenJudgeClient::qt_create_metaobjectdata<qt_meta_tag_ZN15OpenJudgeClientE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "OpenJudgeClient",
        "loginFinished",
        "",
        "NetworkResult",
        "result",
        "homePageFinished",
        "userHomePageFinished",
        "classPageFinished",
        "groupPageFinished",
        "contestPageFinished",
        "problemPageFinished",
        "resultPageFinished",
        "submitPageFinished",
        "solutionSubmitted",
        "judgeFinished",
        "problemListFinished",
        "problemDetailFinished",
        "submissionListFinished"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'loginFinished'
        QtMocHelpers::SignalData<void(const NetworkResult &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'homePageFinished'
        QtMocHelpers::SignalData<void(const NetworkResult &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'userHomePageFinished'
        QtMocHelpers::SignalData<void(const NetworkResult &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'classPageFinished'
        QtMocHelpers::SignalData<void(const NetworkResult &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'groupPageFinished'
        QtMocHelpers::SignalData<void(const NetworkResult &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'contestPageFinished'
        QtMocHelpers::SignalData<void(const NetworkResult &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'problemPageFinished'
        QtMocHelpers::SignalData<void(const NetworkResult &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'resultPageFinished'
        QtMocHelpers::SignalData<void(const NetworkResult &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'submitPageFinished'
        QtMocHelpers::SignalData<void(const NetworkResult &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'solutionSubmitted'
        QtMocHelpers::SignalData<void(const NetworkResult &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'judgeFinished'
        QtMocHelpers::SignalData<void(const NetworkResult &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'problemListFinished'
        QtMocHelpers::SignalData<void(const NetworkResult &)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'problemDetailFinished'
        QtMocHelpers::SignalData<void(const NetworkResult &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'submissionListFinished'
        QtMocHelpers::SignalData<void(const NetworkResult &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<OpenJudgeClient, qt_meta_tag_ZN15OpenJudgeClientE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject OpenJudgeClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15OpenJudgeClientE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15OpenJudgeClientE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15OpenJudgeClientE_t>.metaTypes,
    nullptr
} };

void OpenJudgeClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<OpenJudgeClient *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->loginFinished((*reinterpret_cast<std::add_pointer_t<NetworkResult>>(_a[1]))); break;
        case 1: _t->homePageFinished((*reinterpret_cast<std::add_pointer_t<NetworkResult>>(_a[1]))); break;
        case 2: _t->userHomePageFinished((*reinterpret_cast<std::add_pointer_t<NetworkResult>>(_a[1]))); break;
        case 3: _t->classPageFinished((*reinterpret_cast<std::add_pointer_t<NetworkResult>>(_a[1]))); break;
        case 4: _t->groupPageFinished((*reinterpret_cast<std::add_pointer_t<NetworkResult>>(_a[1]))); break;
        case 5: _t->contestPageFinished((*reinterpret_cast<std::add_pointer_t<NetworkResult>>(_a[1]))); break;
        case 6: _t->problemPageFinished((*reinterpret_cast<std::add_pointer_t<NetworkResult>>(_a[1]))); break;
        case 7: _t->resultPageFinished((*reinterpret_cast<std::add_pointer_t<NetworkResult>>(_a[1]))); break;
        case 8: _t->submitPageFinished((*reinterpret_cast<std::add_pointer_t<NetworkResult>>(_a[1]))); break;
        case 9: _t->solutionSubmitted((*reinterpret_cast<std::add_pointer_t<NetworkResult>>(_a[1]))); break;
        case 10: _t->judgeFinished((*reinterpret_cast<std::add_pointer_t<NetworkResult>>(_a[1]))); break;
        case 11: _t->problemListFinished((*reinterpret_cast<std::add_pointer_t<NetworkResult>>(_a[1]))); break;
        case 12: _t->problemDetailFinished((*reinterpret_cast<std::add_pointer_t<NetworkResult>>(_a[1]))); break;
        case 13: _t->submissionListFinished((*reinterpret_cast<std::add_pointer_t<NetworkResult>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (OpenJudgeClient::*)(const NetworkResult & )>(_a, &OpenJudgeClient::loginFinished, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (OpenJudgeClient::*)(const NetworkResult & )>(_a, &OpenJudgeClient::homePageFinished, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (OpenJudgeClient::*)(const NetworkResult & )>(_a, &OpenJudgeClient::userHomePageFinished, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (OpenJudgeClient::*)(const NetworkResult & )>(_a, &OpenJudgeClient::classPageFinished, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (OpenJudgeClient::*)(const NetworkResult & )>(_a, &OpenJudgeClient::groupPageFinished, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (OpenJudgeClient::*)(const NetworkResult & )>(_a, &OpenJudgeClient::contestPageFinished, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (OpenJudgeClient::*)(const NetworkResult & )>(_a, &OpenJudgeClient::problemPageFinished, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (OpenJudgeClient::*)(const NetworkResult & )>(_a, &OpenJudgeClient::resultPageFinished, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (OpenJudgeClient::*)(const NetworkResult & )>(_a, &OpenJudgeClient::submitPageFinished, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (OpenJudgeClient::*)(const NetworkResult & )>(_a, &OpenJudgeClient::solutionSubmitted, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (OpenJudgeClient::*)(const NetworkResult & )>(_a, &OpenJudgeClient::judgeFinished, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (OpenJudgeClient::*)(const NetworkResult & )>(_a, &OpenJudgeClient::problemListFinished, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (OpenJudgeClient::*)(const NetworkResult & )>(_a, &OpenJudgeClient::problemDetailFinished, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (OpenJudgeClient::*)(const NetworkResult & )>(_a, &OpenJudgeClient::submissionListFinished, 13))
            return;
    }
}

const QMetaObject *OpenJudgeClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OpenJudgeClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15OpenJudgeClientE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int OpenJudgeClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void OpenJudgeClient::loginFinished(const NetworkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void OpenJudgeClient::homePageFinished(const NetworkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void OpenJudgeClient::userHomePageFinished(const NetworkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void OpenJudgeClient::classPageFinished(const NetworkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void OpenJudgeClient::groupPageFinished(const NetworkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void OpenJudgeClient::contestPageFinished(const NetworkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void OpenJudgeClient::problemPageFinished(const NetworkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void OpenJudgeClient::resultPageFinished(const NetworkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void OpenJudgeClient::submitPageFinished(const NetworkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void OpenJudgeClient::solutionSubmitted(const NetworkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1);
}

// SIGNAL 10
void OpenJudgeClient::judgeFinished(const NetworkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1);
}

// SIGNAL 11
void OpenJudgeClient::problemListFinished(const NetworkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1);
}

// SIGNAL 12
void OpenJudgeClient::problemDetailFinished(const NetworkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1);
}

// SIGNAL 13
void OpenJudgeClient::submissionListFinished(const NetworkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1);
}
QT_WARNING_POP
