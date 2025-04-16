/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSClickableWidgetENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSClickableWidgetENDCLASS = QtMocHelpers::stringData(
    "ClickableWidget",
    "clicked",
    ""
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSClickableWidgetENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ClickableWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSClickableWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSClickableWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSClickableWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ClickableWidget, std::true_type>,
        // method 'clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ClickableWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ClickableWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ClickableWidget::*)();
            if (_t _q_method = &ClickableWidget::clicked; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject *ClickableWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClickableWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSClickableWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ClickableWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ClickableWidget::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSAnimatedCounterENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSAnimatedCounterENDCLASS = QtMocHelpers::stringData(
    "AnimatedCounter",
    "value"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSAnimatedCounterENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       1,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::Int, 0x00015103, uint(-1), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject AnimatedCounter::staticMetaObject = { {
    QMetaObject::SuperData::link<QLabel::staticMetaObject>(),
    qt_meta_stringdata_CLASSAnimatedCounterENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSAnimatedCounterENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSAnimatedCounterENDCLASS_t,
        // property 'value'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AnimatedCounter, std::true_type>
    >,
    nullptr
} };

void AnimatedCounter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<AnimatedCounter *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->value(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<AnimatedCounter *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setValue(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *AnimatedCounter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AnimatedCounter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSAnimatedCounterENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QLabel::qt_metacast(_clname);
}

int AnimatedCounter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "showPatientsPage",
    "",
    "showPersonelPage",
    "showResearchPage",
    "showResearchersPage",
    "showToolsPage",
    "showVaccinsPage",
    "showSettingsPage",
    "showToolsTablePage",
    "showPatientsTablePage",
    "updateSidebarIcons",
    "QPushButton*",
    "selectedButton",
    "onAddToolClicked",
    "onEditToolClicked",
    "onDeleteToolClicked",
    "exportStatsToPDF",
    "onAddPatientClicked",
    "onEditPatientClicked",
    "onDeletePatientClicked",
    "loadPatientData",
    "patientID",
    "QLineEdit*",
    "nomInput",
    "prenomInput",
    "QComboBox*",
    "sexeInput",
    "cinInput",
    "adresseInput",
    "numtelInput",
    "maladiechroniqueInput",
    "onModifyPatientSubmit",
    "patientExists"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  134,    2, 0x08,    1 /* Private */,
       3,    0,  135,    2, 0x08,    2 /* Private */,
       4,    0,  136,    2, 0x08,    3 /* Private */,
       5,    0,  137,    2, 0x08,    4 /* Private */,
       6,    0,  138,    2, 0x08,    5 /* Private */,
       7,    0,  139,    2, 0x08,    6 /* Private */,
       8,    0,  140,    2, 0x08,    7 /* Private */,
       9,    0,  141,    2, 0x08,    8 /* Private */,
      10,    0,  142,    2, 0x08,    9 /* Private */,
      11,    1,  143,    2, 0x08,   10 /* Private */,
      14,    0,  146,    2, 0x08,   12 /* Private */,
      15,    0,  147,    2, 0x08,   13 /* Private */,
      16,    0,  148,    2, 0x08,   14 /* Private */,
      17,    0,  149,    2, 0x08,   15 /* Private */,
      18,    0,  150,    2, 0x08,   16 /* Private */,
      19,    0,  151,    2, 0x08,   17 /* Private */,
      20,    0,  152,    2, 0x08,   18 /* Private */,
      21,    8,  153,    2, 0x08,   19 /* Private */,
      32,    8,  170,    2, 0x08,   28 /* Private */,
      33,    1,  187,    2, 0x08,   37 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 23, 0x80000000 | 23, 0x80000000 | 26, 0x80000000 | 23, 0x80000000 | 26, 0x80000000 | 23, 0x80000000 | 23,   22,   24,   25,   27,   28,   29,   30,   31,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 23, 0x80000000 | 23, 0x80000000 | 26, 0x80000000 | 23, 0x80000000 | 26, 0x80000000 | 23, 0x80000000 | 23,   22,   24,   25,   27,   28,   29,   30,   31,
    QMetaType::Bool, QMetaType::Int,   22,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'showPatientsPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showPersonelPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showResearchPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showResearchersPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showToolsPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showVaccinsPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showSettingsPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showToolsTablePage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showPatientsTablePage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateSidebarIcons'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QPushButton *, std::false_type>,
        // method 'onAddToolClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEditToolClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDeleteToolClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'exportStatsToPDF'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAddPatientClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEditPatientClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDeletePatientClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loadPatientData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QComboBox *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QComboBox *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        // method 'onModifyPatientSubmit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QComboBox *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QComboBox *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        // method 'patientExists'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->showPatientsPage(); break;
        case 1: _t->showPersonelPage(); break;
        case 2: _t->showResearchPage(); break;
        case 3: _t->showResearchersPage(); break;
        case 4: _t->showToolsPage(); break;
        case 5: _t->showVaccinsPage(); break;
        case 6: _t->showSettingsPage(); break;
        case 7: _t->showToolsTablePage(); break;
        case 8: _t->showPatientsTablePage(); break;
        case 9: _t->updateSidebarIcons((*reinterpret_cast< std::add_pointer_t<QPushButton*>>(_a[1]))); break;
        case 10: _t->onAddToolClicked(); break;
        case 11: _t->onEditToolClicked(); break;
        case 12: _t->onDeleteToolClicked(); break;
        case 13: _t->exportStatsToPDF(); break;
        case 14: _t->onAddPatientClicked(); break;
        case 15: _t->onEditPatientClicked(); break;
        case 16: _t->onDeletePatientClicked(); break;
        case 17: _t->loadPatientData((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[8]))); break;
        case 18: _t->onModifyPatientSubmit((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[8]))); break;
        case 19: { bool _r = _t->patientExists((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QPushButton* >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 5:
            case 3:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QComboBox* >(); break;
            }
            break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 5:
            case 3:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QComboBox* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}
QT_WARNING_POP
