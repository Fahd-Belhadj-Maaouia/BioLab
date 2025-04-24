/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtGui/qscreen.h>
#include <QtCharts/qlineseries.h>
#include <QtCharts/qabstractbarseries.h>
#include <QtCharts/qvbarmodelmapper.h>
#include <QtCharts/qboxplotseries.h>
#include <QtCharts/qcandlestickseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qpieseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qboxplotseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qpieseries.h>
#include <QtCharts/qpieseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qxyseries.h>
#include <QtCharts/qxyseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qboxplotseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qpieseries.h>
#include <QtCore/qabstractitemmodel.h>
#include <QtCharts/qxyseries.h>
#include <QtCore/qabstractitemmodel.h>
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
    "showPersonnelTablePage",
    "onAddToolClicked",
    "onEditToolClicked",
    "onDeleteToolClicked",
    "onAddPersonelClicked",
    "onEditPersonelClicked",
    "onDeletePersonelClicked",
    "onSearchTextChanged",
    "text",
    "onFilterByVille",
    "index",
    "onSortByRecent",
    "onBackToSummaryClicked",
    "updateStats"
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
      11,    0,  143,    2, 0x08,   10 /* Private */,
      12,    0,  144,    2, 0x08,   11 /* Private */,
      13,    0,  145,    2, 0x08,   12 /* Private */,
      14,    0,  146,    2, 0x08,   13 /* Private */,
      15,    0,  147,    2, 0x08,   14 /* Private */,
      16,    0,  148,    2, 0x08,   15 /* Private */,
      17,    1,  149,    2, 0x08,   16 /* Private */,
      19,    1,  152,    2, 0x08,   18 /* Private */,
      21,    0,  155,    2, 0x08,   20 /* Private */,
      22,    0,  156,    2, 0x08,   21 /* Private */,
      23,    0,  157,    2, 0x08,   22 /* Private */,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

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
        // method 'showPersonnelTablePage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAddToolClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEditToolClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDeleteToolClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAddPersonelClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEditPersonelClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDeletePersonelClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onFilterByVille'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onSortByRecent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onBackToSummaryClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateStats'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
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
        case 8: _t->showPersonnelTablePage(); break;
        case 9: _t->onAddToolClicked(); break;
        case 10: _t->onEditToolClicked(); break;
        case 11: _t->onDeleteToolClicked(); break;
        case 12: _t->onAddPersonelClicked(); break;
        case 13: _t->onEditPersonelClicked(); break;
        case 14: _t->onDeletePersonelClicked(); break;
        case 15: _t->onSearchTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->onFilterByVille((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 17: _t->onSortByRecent(); break;
        case 18: _t->onBackToSummaryClicked(); break;
        case 19: _t->updateStats(); break;
        default: ;
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 20;
    }
    return _id;
}
QT_WARNING_POP
