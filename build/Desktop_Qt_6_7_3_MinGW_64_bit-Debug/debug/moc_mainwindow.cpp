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
    "taskMovedToCompleted",
    "",
    "task",
    "taskMovedToTodo",
    "projectDataChanged",
    "showVaccinationHistory",
    "patientId",
    "showPatientsPage",
    "showPersonelPage",
    "showResearchPage",
    "showresearchersPage",
    "showToolsPage",
    "showVaccinsPage",
    "showSettingsPage",
    "showResearchTablePage",
    "showToolsTablePage",
    "showAddToolPage",
    "showMessagerieePage",
    "showPatientsTablePage",
    "showResearchersTablePage",
    "updateSidebarIcons",
    "QPushButton*",
    "selectedButton",
    "setupResearchCards",
    "setupResearchesFormAddPage",
    "showResearchFormAdd",
    "setupResearchesFormUpdatePage",
    "id",
    "setupResearchesTablePage",
    "handleDeleteRow",
    "QModelIndex",
    "index",
    "searchProjects",
    "refreshCostChart",
    "exportToExcel",
    "sendMessageToChatbot",
    "userMessage",
    "onKeypadPressed",
    "key",
    "readSerialData",
    "onDeleteToolClicked",
    "updatePaginationControls",
    "currentPage",
    "totalPages",
    "exportToolsToPDF",
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
    "contactId",
    "onAddResearcherClicked",
    "onEditResearcherClicked",
    "onDeleteResearcherClicked",
    "loadResearcherData",
    "researcherID",
    "specialiteInput",
    "datedenaissanceInput",
    "emailInput",
    "onModifyResearcherSubmit",
    "showResearchersStatsPage"
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
      65,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  404,    2, 0x06,    1 /* Public */,
       4,    1,  407,    2, 0x06,    3 /* Public */,
       5,    0,  410,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    1,  411,    2, 0x0a,    6 /* Public */,
       8,    0,  414,    2, 0x08,    8 /* Private */,
       9,    0,  415,    2, 0x08,    9 /* Private */,
      10,    0,  416,    2, 0x08,   10 /* Private */,
      11,    0,  417,    2, 0x08,   11 /* Private */,
      12,    0,  418,    2, 0x08,   12 /* Private */,
      13,    0,  419,    2, 0x08,   13 /* Private */,
      14,    0,  420,    2, 0x08,   14 /* Private */,
      15,    0,  421,    2, 0x08,   15 /* Private */,
      16,    0,  422,    2, 0x08,   16 /* Private */,
      17,    0,  423,    2, 0x08,   17 /* Private */,
      18,    0,  424,    2, 0x08,   18 /* Private */,
      19,    0,  425,    2, 0x08,   19 /* Private */,
      20,    0,  426,    2, 0x08,   20 /* Private */,
      21,    1,  427,    2, 0x08,   21 /* Private */,
      24,    0,  430,    2, 0x08,   23 /* Private */,
      25,    0,  431,    2, 0x08,   24 /* Private */,
      26,    0,  432,    2, 0x08,   25 /* Private */,
      27,    1,  433,    2, 0x08,   26 /* Private */,
      29,    0,  436,    2, 0x08,   28 /* Private */,
      30,    1,  437,    2, 0x08,   29 /* Private */,
      33,    0,  440,    2, 0x08,   31 /* Private */,
      34,    0,  441,    2, 0x08,   32 /* Private */,
      35,    0,  442,    2, 0x08,   33 /* Private */,
      36,    1,  443,    2, 0x08,   34 /* Private */,
      38,    1,  446,    2, 0x08,   36 /* Private */,
      40,    0,  449,    2, 0x08,   38 /* Private */,
      41,    1,  450,    2, 0x08,   39 /* Private */,
      42,    2,  453,    2, 0x08,   41 /* Private */,
      45,    0,  458,    2, 0x08,   44 /* Private */,
      46,    0,  459,    2, 0x08,   45 /* Private */,
      47,    0,  460,    2, 0x08,   46 /* Private */,
      48,    0,  461,    2, 0x08,   47 /* Private */,
      49,    0,  462,    2, 0x08,   48 /* Private */,
      50,    0,  463,    2, 0x08,   49 /* Private */,
      51,    0,  464,    2, 0x08,   50 /* Private */,
      52,    1,  465,    2, 0x08,   51 /* Private */,
      55,    2,  468,    2, 0x08,   53 /* Private */,
      59,    1,  473,    2, 0x08,   56 /* Private */,
      61,    0,  476,    2, 0x08,   58 /* Private */,
      62,    1,  477,    2, 0x08,   59 /* Private */,
      64,    1,  480,    2, 0x08,   61 /* Private */,
      66,    0,  483,    2, 0x08,   63 /* Private */,
      67,    0,  484,    2, 0x08,   64 /* Private */,
      68,    0,  485,    2, 0x08,   65 /* Private */,
      69,    0,  486,    2, 0x08,   66 /* Private */,
      70,    8,  487,    2, 0x08,   67 /* Private */,
      81,    8,  504,    2, 0x08,   76 /* Private */,
      82,    1,  521,    2, 0x08,   85 /* Private */,
      83,    0,  524,    2, 0x08,   87 /* Private */,
      84,    0,  525,    2, 0x08,   88 /* Private */,
      85,    0,  526,    2, 0x08,   89 /* Private */,
      86,    1,  527,    2, 0x08,   90 /* Private */,
      88,    0,  530,    2, 0x08,   92 /* Private */,
      89,    0,  531,    2, 0x08,   93 /* Private */,
      90,    1,  532,    2, 0x08,   94 /* Private */,
      92,    0,  535,    2, 0x08,   96 /* Private */,
      93,    0,  536,    2, 0x08,   97 /* Private */,
      94,    0,  537,    2, 0x08,   98 /* Private */,
      95,   10,  538,    2, 0x08,   99 /* Private */,
     100,   10,  559,    2, 0x08,  110 /* Private */,
     101,    0,  580,    2, 0x08,  121 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    7,
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
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   28,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 31,   32,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   37,
    QMetaType::Void, QMetaType::Char,   39,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   43,   44,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 53,   54,
    QMetaType::Void, 0x80000000 | 56, QMetaType::Int,   57,   58,
    QMetaType::Void, QMetaType::QString,   60,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 53,   63,
    QMetaType::Void, QMetaType::Int,   65,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 72, 0x80000000 | 72, 0x80000000 | 75, 0x80000000 | 72, 0x80000000 | 75, 0x80000000 | 72, 0x80000000 | 72,   71,   73,   74,   76,   77,   78,   79,   80,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 72, 0x80000000 | 72, 0x80000000 | 75, 0x80000000 | 72, 0x80000000 | 75, 0x80000000 | 72, 0x80000000 | 72,   71,   73,   74,   76,   77,   78,   79,   80,
    QMetaType::Bool, QMetaType::Int,   71,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   87,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   91,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 72, 0x80000000 | 72, 0x80000000 | 75, 0x80000000 | 72, 0x80000000 | 75, 0x80000000 | 72, 0x80000000 | 72, 0x80000000 | 72, 0x80000000 | 72,   96,   73,   74,   76,   77,   78,   79,   97,   98,   99,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 72, 0x80000000 | 72, 0x80000000 | 75, 0x80000000 | 72, 0x80000000 | 75, 0x80000000 | 72, 0x80000000 | 72, 0x80000000 | 72, 0x80000000 | 72,   96,   73,   74,   76,   77,   78,   79,   97,   98,   99,
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
        // method 'taskMovedToCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'taskMovedToTodo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'projectDataChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showVaccinationHistory'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'showPatientsPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showPersonelPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showResearchPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showresearchersPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showToolsPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showVaccinsPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showSettingsPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showResearchTablePage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showToolsTablePage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showAddToolPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showMessagerieePage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showPatientsTablePage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showResearchersTablePage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateSidebarIcons'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QPushButton *, std::false_type>,
        // method 'setupResearchCards'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setupResearchesFormAddPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showResearchFormAdd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setupResearchesFormUpdatePage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setupResearchesTablePage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleDeleteRow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'searchProjects'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'refreshCostChart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'exportToExcel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendMessageToChatbot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onKeypadPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<char, std::false_type>,
        // method 'readSerialData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDeleteToolClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updatePaginationControls'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'exportToolsToPDF'
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
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onAddResearcherClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEditResearcherClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDeleteResearcherClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loadResearcherData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QComboBox *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QComboBox *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        // method 'onModifyResearcherSubmit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QComboBox *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QComboBox *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLineEdit *, std::false_type>,
        // method 'showResearchersStatsPage'
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
        case 0: _t->taskMovedToCompleted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->taskMovedToTodo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->projectDataChanged(); break;
        case 3: _t->showVaccinationHistory((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->showPatientsPage(); break;
        case 5: _t->showPersonelPage(); break;
        case 6: _t->showResearchPage(); break;
        case 7: _t->showresearchersPage(); break;
        case 8: _t->showToolsPage(); break;
        case 9: _t->showVaccinsPage(); break;
        case 10: _t->showSettingsPage(); break;
        case 11: _t->showResearchTablePage(); break;
        case 12: _t->showToolsTablePage(); break;
        case 13: _t->showAddToolPage(); break;
        case 14: _t->showMessagerieePage(); break;
        case 15: _t->showPatientsTablePage(); break;
        case 17: _t->updateSidebarIcons((*reinterpret_cast< std::add_pointer_t<QPushButton*>>(_a[1]))); break;
        case 30: _t->onDeleteToolClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 31: _t->updatePaginationControls((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 32: _t->exportToolsToPDF(); break;
        case 33: _t->exportStatsToPDF(); break;
        case 34: _t->showVaccinsTablePage(); break;
        case 35: _t->onAddVaccinClicked(); break;
        case 36: _t->onEditVaccinClicked(); break;
        case 37: _t->onDeleteVaccinClicked(); break;
        case 39: _t->displayVaccinTypeStats((*reinterpret_cast< std::add_pointer_t<QWidget*>>(_a[1]))); break;
        case 40: _t->exportVaccinStatsToPDF((*reinterpret_cast< std::add_pointer_t<QMap<QString,int>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 41: _t->showExpirationPopup((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 42: _t->loadNotifications(); break;
        case 43: _t->styleNotificationsWidget((*reinterpret_cast< std::add_pointer_t<QWidget*>>(_a[1]))); break;
        case 44: _t->loadVaccinDataToEditForm((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 45: _t->exportVaccinsTableToPDF(); break;
        case 46: _t->onAddPatientClicked(); break;
        case 47: _t->onEditPatientClicked(); break;
        case 48: _t->onDeletePatientClicked(); break;
        case 49: _t->loadPatientData((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[8]))); break;
        case 50: _t->onModifyPatientSubmit((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[8]))); break;
        case 51: { bool _r = _t->patientExists((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 52: _t->onReadMessageButtonClicked(); break;
        case 53: _t->onNewMessageButtonClicked(); break;
        case 54: _t->onDeleteMessageButtonClicked(); break;
        case 55: _t->onContactSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 56: _t->onSendMessageClicked(); break;
        case 57: _t->onRefreshMessagesButtonClicked(); break;
        case 58: _t->displayConversation((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 59: _t->onAddResearcherClicked(); break;
        case 60: _t->onEditResearcherClicked(); break;
        case 61: _t->onDeleteResearcherClicked(); break;
        case 62: _t->loadResearcherData((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[8])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[9])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[10]))); break;
        case 63: _t->onModifyResearcherSubmit((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QComboBox*>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[8])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[9])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[10]))); break;
        case 64: _t->showResearchersStatsPage(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QPushButton* >(); break;
            }
            break;
        case 39:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        case 43:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        case 49:
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
        case 50:
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
        case 62:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 5:
            case 3:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QComboBox* >(); break;
            case 9:
            case 8:
            case 7:
            case 6:
            case 4:
            case 2:
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QLineEdit* >(); break;
            }
            break;
        case 63:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 5:
            case 3:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QComboBox* >(); break;
            case 9:
            case 8:
            case 7:
            case 6:
            case 4:
            case 2:
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QLineEdit* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(const QString & );
            if (_t _q_method = &MainWindow::taskMovedToCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(const QString & );
            if (_t _q_method = &MainWindow::taskMovedToTodo; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::projectDataChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
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
        if (_id < 65)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 65;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 65)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 65;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::taskMovedToCompleted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::taskMovedToTodo(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::projectDataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
