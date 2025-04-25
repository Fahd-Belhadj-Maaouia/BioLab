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
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "showVaccinationHistory",
    "",
    "patientId",
    "showPatientsPage",
    "showPersonelPage",
    "showResearchPage",
    "showResearchersPage",
    "showToolsPage",
    "showVaccinsPage",
    "showSettingsPage",
    "showToolsTablePage",
    "showMessagerieePage",
    "showPatientsTablePage",
    "updateSidebarIcons",
    "QPushButton*",
    "selectedButton",
    "readSerialData",
    "onAddToolClicked",
    "onEditToolClicked",
    "onDeleteToolClicked",
    "exportStatsToPDF",
    "showVaccinsTablePage",
    "onAddVaccinClicked",
    "onEditVaccinClicked",
    "onDeleteVaccinClicked",
    "showVaccinTypeStats",
    "displayVaccinTypeStats",
    "QWidget*",
    "parentWidget",
    "exportVaccinStatsToPDF",
    "QMap<QString,int>",
    "typeStats",
    "totalVaccins",
    "showExpirationPopup",
    "message",
    "loadNotifications",
    "styleNotificationsWidget",
    "notificationsWidget",
    "loadVaccinDataToEditForm",
    "vaccinId",
    "exportVaccinsTableToPDF",
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
    "patientExists",
    "onReadMessageButtonClicked",
    "onNewMessageButtonClicked",
    "onDeleteMessageButtonClicked",
    "onContactSelected",
    "row",
    "onSendMessageClicked",
    "onRefreshMessagesButtonClicked",
    "displayConversation",
    "contactId"
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
      42,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  266,    2, 0x0a,    1 /* Public */,
       4,    0,  269,    2, 0x08,    3 /* Private */,
       5,    0,  270,    2, 0x08,    4 /* Private */,
       6,    0,  271,    2, 0x08,    5 /* Private */,
       7,    0,  272,    2, 0x08,    6 /* Private */,
       8,    0,  273,    2, 0x08,    7 /* Private */,
       9,    0,  274,    2, 0x08,    8 /* Private */,
      10,    0,  275,    2, 0x08,    9 /* Private */,
      11,    0,  276,    2, 0x08,   10 /* Private */,
      12,    0,  277,    2, 0x08,   11 /* Private */,
      13,    0,  278,    2, 0x08,   12 /* Private */,
      14,    1,  279,    2, 0x08,   13 /* Private */,
      17,    0,  282,    2, 0x08,   15 /* Private */,
      18,    0,  283,    2, 0x08,   16 /* Private */,
      19,    0,  284,    2, 0x08,   17 /* Private */,
      20,    0,  285,    2, 0x08,   18 /* Private */,
      21,    0,  286,    2, 0x08,   19 /* Private */,
      22,    0,  287,    2, 0x08,   20 /* Private */,
      23,    0,  288,    2, 0x08,   21 /* Private */,
      24,    0,  289,    2, 0x08,   22 /* Private */,
      25,    0,  290,    2, 0x08,   23 /* Private */,
      26,    0,  291,    2, 0x08,   24 /* Private */,
      27,    1,  292,    2, 0x08,   25 /* Private */,
      30,    2,  295,    2, 0x08,   27 /* Private */,
      34,    1,  300,    2, 0x08,   30 /* Private */,
      36,    0,  303,    2, 0x08,   32 /* Private */,
      37,    1,  304,    2, 0x08,   33 /* Private */,
      39,    1,  307,    2, 0x08,   35 /* Private */,
      41,    0,  310,    2, 0x08,   37 /* Private */,
      42,    0,  311,    2, 0x08,   38 /* Private */,
      43,    0,  312,    2, 0x08,   39 /* Private */,
      44,    0,  313,    2, 0x08,   40 /* Private */,
      45,    8,  314,    2, 0x08,   41 /* Private */,
      56,    8,  331,    2, 0x08,   50 /* Private */,
      57,    1,  348,    2, 0x08,   59 /* Private */,
      58,    0,  351,    2, 0x08,   61 /* Private */,
      59,    0,  352,    2, 0x08,   62 /* Private */,
      60,    0,  353,    2, 0x08,   63 /* Private */,
      61,    1,  354,    2, 0x08,   64 /* Private */,
      63,    0,  357,    2, 0x08,   66 /* Private */,
      64,    0,  358,    2, 0x08,   67 /* Private */,
      65,    1,  359,    2, 0x08,   68 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
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
    QMetaType::Void, 0x80000000 | 15,   16,
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
    QMetaType::Void, 0x80000000 | 28,   29,
    QMetaType::Void, 0x80000000 | 31, QMetaType::Int,   32,   33,
    QMetaType::Void, QMetaType::QString,   35,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 28,   38,
    QMetaType::Void, QMetaType::Int,   40,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 47, 0x80000000 | 47, 0x80000000 | 50, 0x80000000 | 47, 0x80000000 | 50, 0x80000000 | 47, 0x80000000 | 47,   46,   48,   49,   51,   52,   53,   54,   55,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 47, 0x80000000 | 47, 0x80000000 | 50, 0x80000000 | 47, 0x80000000 | 50, 0x80000000 | 47, 0x80000000 | 47,   46,   48,   49,   51,   52,   53,   54,   55,
    QMetaType::Bool, QMetaType::Int,   46,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   62,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   66,

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
        // method 'showVaccinationHistory'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
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
        // method 'showMessagerieePage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showPatientsTablePage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateSidebarIcons'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QPushButton *, std::false_type>,
        // method 'readSerialData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAddToolClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEditToolClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDeleteToolClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'exportStatsToPDF'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showVaccinsTablePage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAddVaccinClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEditVaccinClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDeleteVaccinClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showVaccinTypeStats'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'displayVaccinTypeStats'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QWidget *, std::false_type>,
        // method 'exportVaccinStatsToPDF'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QMap<QString,int> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'showExpirationPopup'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'loadNotifications'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'styleNotificationsWidget'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QWidget *, std::false_type>,
        // method 'loadVaccinDataToEditForm'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'exportVaccinsTableToPDF'
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
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onReadMessageButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNewMessageButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDeleteMessageButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onContactSelected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onSendMessageClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRefreshMessagesButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'displayConversation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
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
        case 0: _t->showVaccinationHistory((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->showPatientsPage(); break;
        case 2: _t->showPersonelPage(); break;
        case 3: _t->showResearchPage(); break;
        case 4: _t->showResearchersPage(); break;
        case 5: _t->showToolsPage(); break;
        case 6: _t->showVaccinsPage(); break;
        case 7: _t->showSettingsPage(); break;
        case 8: _t->showToolsTablePage(); break;
        case 9: _t->showMessagerieePage(); break;
        case 10: _t->showPatientsTablePage(); break;
        case 11: _t->updateSidebarIcons((*reinterpret_cast< std::add_pointer_t<QPushButton*>>(_a[1]))); break;
        case 13: _t->onAddToolClicked(); break;
        case 14: _t->onEditToolClicked(); break;
        case 15: _t->onDeleteToolClicked(); break;
        case 16: _t->exportStatsToPDF(); break;
        case 17: _t->showVaccinsTablePage(); break;
        case 18: _t->onAddVaccinClicked(); break;
        case 19: _t->onEditVaccinClicked(); break;
        case 20: _t->onDeleteVaccinClicked(); break;
        case 22: _t->displayVaccinTypeStats((*reinterpret_cast< std::add_pointer_t<QWidget*>>(_a[1]))); break;
        case 23: _t->exportVaccinStatsToPDF((*reinterpret_cast< std::add_pointer_t<QMap<QString,int>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 24: _t->showExpirationPopup((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 25: _t->loadNotifications(); break;
        case 26: _t->styleNotificationsWidget((*reinterpret_cast< std::add_pointer_t<QWidget*>>(_a[1]))); break;
        case 27: _t->loadVaccinDataToEditForm((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 28: _t->exportVaccinsTableToPDF(); break;
        case 29: _t->onAddPatientClicked(); break;
        case 30: _t->onEditPatientClicked(); break;
        case 31: _t->onDeletePatientClicked(); break;
        case 32: _t->loadPatientData((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[8]))); break;
        case 33: _t->onModifyPatientSubmit((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[8]))); break;
        case 34: { bool _r = _t->patientExists((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 35: _t->onReadMessageButtonClicked(); break;
        case 36: _t->onNewMessageButtonClicked(); break;
        case 37: _t->onDeleteMessageButtonClicked(); break;
        case 38: _t->onContactSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 39: _t->onSendMessageClicked(); break;
        case 40: _t->onRefreshMessagesButtonClicked(); break;
        case 41: _t->displayConversation((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QPushButton* >(); break;
            }
            break;
        case 22:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        case 26:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        case 32:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 5:
            case 3:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QComboBox* >(); break;
            case 7:
            case 6:
            case 4:
            case 2:
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QLineEdit* >(); break;
            }
            break;
        case 33:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 5:
            case 3:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QComboBox* >(); break;
            case 7:
            case 6:
            case 4:
            case 2:
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QLineEdit* >(); break;
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
        if (_id < 42)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 42;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 42)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 42;
    }
    return _id;
}
QT_WARNING_POP
