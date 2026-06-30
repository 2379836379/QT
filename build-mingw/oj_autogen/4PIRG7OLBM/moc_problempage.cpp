/****************************************************************************
** Meta object code from reading C++ file 'problempage.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../oj-client/src/ui/pages/problempage.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'problempage.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN11ProblemPageE_t {};
} // unnamed namespace

template <> constexpr inline auto ProblemPage::qt_create_metaobjectdata<qt_meta_tag_ZN11ProblemPageE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ProblemPage",
        "backRequested",
        "",
        "homeRequested",
        "themeToggleRequested",
        "dark",
        "translateProblemRequested",
        "favoriteRequested",
        "saveProblemMetaRequested",
        "ProblemMeta",
        "meta",
        "aiAskRequested",
        "question",
        "testRequested",
        "languageLabel",
        "sourceText",
        "stdinText",
        "submitRequested",
        "language"
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
        // Signal 'translateProblemRequested'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'favoriteRequested'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'saveProblemMetaRequested'
        QtMocHelpers::SignalData<void(const ProblemMeta &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 10 },
        }}),
        // Signal 'aiAskRequested'
        QtMocHelpers::SignalData<void(const QString &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 12 },
        }}),
        // Signal 'testRequested'
        QtMocHelpers::SignalData<void(const QString &, const QString &, const QString &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 14 }, { QMetaType::QString, 15 }, { QMetaType::QString, 16 },
        }}),
        // Signal 'submitRequested'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 18 }, { QMetaType::QString, 15 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ProblemPage, qt_meta_tag_ZN11ProblemPageE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ProblemPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ProblemPageE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ProblemPageE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11ProblemPageE_t>.metaTypes,
    nullptr
} };

void ProblemPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProblemPage *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->backRequested(); break;
        case 1: _t->homeRequested(); break;
        case 2: _t->themeToggleRequested((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->translateProblemRequested(); break;
        case 4: _t->favoriteRequested(); break;
        case 5: _t->saveProblemMetaRequested((*reinterpret_cast<std::add_pointer_t<ProblemMeta>>(_a[1]))); break;
        case 6: _t->aiAskRequested((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->testRequested((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 8: _t->submitRequested((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ProblemPage::*)()>(_a, &ProblemPage::backRequested, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProblemPage::*)()>(_a, &ProblemPage::homeRequested, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProblemPage::*)(bool )>(_a, &ProblemPage::themeToggleRequested, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProblemPage::*)()>(_a, &ProblemPage::translateProblemRequested, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProblemPage::*)()>(_a, &ProblemPage::favoriteRequested, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProblemPage::*)(const ProblemMeta & )>(_a, &ProblemPage::saveProblemMetaRequested, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProblemPage::*)(const QString & )>(_a, &ProblemPage::aiAskRequested, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProblemPage::*)(const QString & , const QString & , const QString & )>(_a, &ProblemPage::testRequested, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProblemPage::*)(const QString & , const QString & )>(_a, &ProblemPage::submitRequested, 8))
            return;
    }
}

const QMetaObject *ProblemPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProblemPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ProblemPageE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ProblemPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ProblemPage::backRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ProblemPage::homeRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ProblemPage::themeToggleRequested(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void ProblemPage::translateProblemRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ProblemPage::favoriteRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ProblemPage::saveProblemMetaRequested(const ProblemMeta & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void ProblemPage::aiAskRequested(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void ProblemPage::testRequested(const QString & _t1, const QString & _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2, _t3);
}

// SIGNAL 8
void ProblemPage::submitRequested(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1, _t2);
}
QT_WARNING_POP
