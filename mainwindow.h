#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Standard Qt Includes
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QTableWidget>
#include <QTableView>
#include <QStackedWidget>
#include <QLabel>
#include <QButtonGroup>
#include <QLineEdit>
#include <QSpinBox>
#include <QFormLayout>
#include <QDateEdit>
#include <QInputDialog>
#include <QProgressBar>
#include <QTextEdit>
#include <QListWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QComboBox>
#include <QMessageBox>
#include <QSplitter>
#include <QFileDialog>
#include <QDate>
#include <QPixmap>
#include <QImage>
#include <QString>
#include <QByteArray>
#include <QUrl>
#include <QMap>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QVariantMap>
#include <QSortFilterProxyModel>
#include <QTimer>

// Charts (QtCharts)
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

// Serial Communication
#include <QSerialPort>
#include <QSerialPortInfo>

// SQL & Data Handling
#include <QSqlQuery>

// JSON Handling
#include <QJsonObject>
#include <QJsonDocument>

// QR Code Generator
#include "qrcodegen.hpp"
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

// Custom Project Headers
#include "arduinomanager.h"
#include "researchermanager.h"
#include "serialmanager.h"
#include "materialinputdialog.h"
#include "projetderecherche.h"
#include "buttondelegate.h"
#include "todolist.h"


// Forward declarations
class QChart;
class QPieSeries;
class QBarSeries;

// Forward declarations
class ToolsManager;
class VaccinManager;
class MessagesManager;
class PatientsManager;
class ResearcherManager;







QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class ClickableWidget : public QWidget {
    Q_OBJECT
public:
    explicit ClickableWidget(QWidget *parent = nullptr) : QWidget(parent) {}
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent *event) override {
        QWidget::mousePressEvent(event);
        emit clicked();
    }
};



class MainWindow : public QMainWindow {
    Q_OBJECT
protected:
    void showEvent(QShowEvent *event) override;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showVaccinationHistory(int patientId);

private slots:
    // Navigation slots
    void showPatientsPage();
    void showPersonelPage();
    void showResearchPage();
    void showresearchersPage();
    void showToolsPage();
    void showVaccinsPage();
    void showSettingsPage();
    void showResearchTablePage();
    void showToolsTablePage();
    void showAddToolPage();
    void showMessagerieePage();
    void showPatientsTablePage();
    void showResearchersTablePage();
    void updateSidebarIcons(QPushButton *selectedButton);
    void setupResearchCards();
    void setupResearchesFormAddPage();
    void showResearchFormAdd();
    void setupResearchesFormUpdatePage(int id);
    void setupResearchesTablePage();
    void handleDeleteRow(const QModelIndex &index);
    void searchProjects();
    void refreshCostChart();
    void exportToExcel();
    void sendMessageToChatbot(const QString &userMessage);
    void onKeypadPressed(char key);

    // Arduino-related slots
    void readSerialData();

    // Tools-related slots
    void onDeleteToolClicked(int);
    void updatePaginationControls(int currentPage, int totalPages);
    void exportToolsToPDF();

    void exportStatsToPDF();

    // Vaccin-related slots
    void showVaccinsTablePage();
    void onAddVaccinClicked();
    void onEditVaccinClicked();
    void onDeleteVaccinClicked();
    void showVaccinTypeStats();
    void displayVaccinTypeStats(QWidget *parentWidget);
    void exportVaccinStatsToPDF(const QMap<QString, int>& typeStats, int totalVaccins);
    void showExpirationPopup(const QString &message);
    void loadNotifications();
    void styleNotificationsWidget(QWidget *notificationsWidget);
    void loadVaccinDataToEditForm(int vaccinId);
    void exportVaccinsTableToPDF();

    // Patient-related slots
    void onAddPatientClicked();
    void onEditPatientClicked();
    void onDeletePatientClicked();
    void loadPatientData(int patientID, QLineEdit *nomInput, QLineEdit *prenomInput, QComboBox *sexeInput, QLineEdit *cinInput, QComboBox *adresseInput, QLineEdit *numtelInput, QLineEdit *maladiechroniqueInput);
    void onModifyPatientSubmit(int patientID, QLineEdit *nomInput, QLineEdit *prenomInput, QComboBox *sexeInput, QLineEdit *cinInput, QComboBox *adresseInput, QLineEdit *numtelInput, QLineEdit *maladiechroniqueInput);
    bool patientExists(int patientID);

    // Message-related slots
    void onReadMessageButtonClicked();
    void onNewMessageButtonClicked();
    void onDeleteMessageButtonClicked();
    void onContactSelected(int row);
    void onSendMessageClicked();
    void onRefreshMessagesButtonClicked();
    void displayConversation(int contactId);

    //chercheur-related slots
    void onAddResearcherClicked();
    void onEditResearcherClicked();
    void onDeleteResearcherClicked();
    void loadResearcherData(int researcherID, QLineEdit *nomInput, QLineEdit *prenomInput, QComboBox *sexeInput, QLineEdit *cinInput, QComboBox *adresseInput, QLineEdit *numtelInput, QLineEdit *specialiteInput, QLineEdit *datedenaissanceInput, QLineEdit *emailInput);
    // Handle submission of modification
    void onModifyResearcherSubmit(int researcherID, QLineEdit *nomInput, QLineEdit *prenomInput, QComboBox *sexeInput, QLineEdit *cinInput, QComboBox *adresseInput, QLineEdit *numtelInput, QLineEdit *specialiteInput, QLineEdit *datedenaissanceInput, QLineEdit *emailInput);
    void showResearchersStatsPage();

    // Arduino related slots
    // Kept as comments to preserve Arduino-related functionality
    /*void setupArduinoConnection();
    void handleColorDetection(const QString &color);
    void handleVaccinExpiration(bool isExpired, const QString &vaccinName, const QString &reference, const QString &expDate);
    void handleArduinoError(const QString &errorMessage);*/

private:
    // Utility methods
    void clearLayout(QLayout *layout);
    void animateStatsUpdate();
    QPushButton* createSidebarButton(const QString &text, const QString &iconPath);
    QString buttonStyle(const QString &color);
    QString tableStyle();
    QString groupBoxStyle(const QString &color);
    QString getIncomingMessageStyle();
    QString getOutgoingMessageStyle();
    QTableView *tableView;
    ProjetDeRecherche Etmp;
    QWidget *researchesFormUpdatePage;

    // UI Components
    QWidget *centralWidget;
    QWidget *contentWrapper;
    QVBoxLayout *mainLayout;
    class ResearchForm : public QWidget {
    public:
        explicit ResearchForm(int projectId, QWidget *parent = nullptr);
    private:
        int m_projectId;
    };
    void refreshResearchTable();
    QWidget* createTaskCard(const QString &title, QListWidget *taskList);
    void loadTasksFromDB(QListWidget *todoList, QListWidget *completedList);
    void addTaskToDB(const QString &task, bool completed);
    void moveTaskInDB(const QString &task, bool fromTodoToCompleted);
    void deleteTaskFromDB(const QString &task, bool fromCompleted);

    // Sidebar
    QWidget *sidebarWidget;
    QVBoxLayout *sidebarLayout;
    QPushButton *btnPatients;
    QPushButton *btnPersonel;
    QPushButton *btnResearches;
    QPushButton *btnResearchers;
    QPushButton *btnTools;
    QPushButton *btnVaccins;
    QPushButton *btnSettings;
    QPushButton *btnMessagerie;
    QPushButton *uploadImageButton;
    QButtonGroup *sidebarButtonGroup;  // Ensure only one selection at a time

    QTextEdit *chatTextEdit = nullptr;
    QLineEdit *inputLineEdit = nullptr;
    QPushButton *sendButton = nullptr;

    ToDoList *todoManager;
    SerialManager *serialManager;
    MaterialInputDialog *inputDialog = nullptr;

    QChartView* createCostRangeChart(); // New function for chart creation
    QWidget* chartCard = nullptr;       // Pointer to the card containing the chart
    QChartView* costChartView = nullptr;


    // Arduino components
    ArduinoManager *arduinoManager;
    QComboBox *portSelector;
    QPushButton *connectButton;
    QLabel *statusLabel;
    QLabel *detectedColorLabel;
    QLabel *vaccinInfoLabel;

    // Main Content
    QVBoxLayout *dashboardLayout;
    QWidget *dashboardWidget;
    QStackedWidget *stackedWidget;

    // Pages
    QWidget *patientsPage;
    QWidget *researchersPage;
    QWidget *settingsPage;
    QWidget *vaccinsPage;
    QWidget *messagerieePage;
    QWidget *personelPage;
    QWidget *toolsPage;
    QWidget *addToolPage;
    QWidget *researchesPage;
    QWidget *researchesTablePage;
    QWidget *researchesFormAddPage;


    //Table
    QListWidget *todoList;
    QListWidget *completedList;


    //searchTableVariables
    QSqlQueryModel *sqlModel;
    QSortFilterProxyModel *proxyModel;
    ButtonDelegate *buttonDelegate;

    // Tools Pages

    QWidget *toolsTablePage;
    QPushButton *firstPageBtn;
    QPushButton *prevPageBtn;
    QLabel *pageInfoLabel;
    QPushButton *nextPageBtn;
    QPushButton *lastPageBtn;
    QComboBox *itemsPerPageCombo;

    // PDF Export button
    QPushButton *exportPdfBtn;
    //qrcode
    QString generateRandomString(int length);
    void showToolQRCode(int toolId);
    QImage generateQRCode(const QString &text, int scale = 10, int border = 4);
    QWidget *qrCodePage = nullptr;
    QLabel *qrCodeLabel = nullptr;

    // Statistics widgets
    QWidget *ToolstatsContainer;
    QChartView *pieChartView;
    QChartView *barChartView;
    QChart *pieChart;
    QChart *barChart;

    // For data tracking
    QMap<QString, int> categoryCounts;
    QMap<QString, int> categoryStockTotals;
    QMap<QString, int> categoryMaxQuantities;

    // Methods
    void setupStatisticsSection();
    void ToolsUpdateStatistics();
    void calculateStatistics();

    // Vaccin Pages
    QWidget *vaccinsTablePage;
    QWidget *addVaccinFormPage;
    QWidget *editVaccinFormPage;

    // Patient Pages
    QWidget *PatientsTablePage;
    QWidget *addPatientsFormPage;
    QWidget *ModifyPatientFormPage;

    // Messagerie components
    QListWidget *contactsList;
    QWidget *conversationContainer;
    QScrollArea *messagesScrollArea;
    QWidget *messagesWidget;
    QVBoxLayout *messagesLayout;
    QTextEdit *messageInputField;
    QPushButton *sendMessageButton;
    int currentContactId;
    QTableWidget *messageTable;

    //researcher components
    QWidget *ResearchersTablePage;
    QWidget *addResearcherFormPage;
    QWidget *ModifyResearcherFormPage;

    // Chart components
    QChartView *chartView;
    QTimer *updateTimer;

    // Vaccin form inputs
    QLineEdit *m_idInput = nullptr;
    QLineEdit *m_nomVaccinInput = nullptr;
    QLineEdit *m_referenceInput = nullptr;
    QLineEdit *m_typeInput = nullptr;
    QLineEdit *m_maladieChronicInput = nullptr;
    QSpinBox *m_nbDoseInput = nullptr;
    QSpinBox *m_quantiteInput = nullptr;
    QDateEdit *m_dateExpInput = nullptr;

    // Vaccin Summary Components
    QTableWidget *vaccinSummaryTable;
    QWidget *ageGroupStatsWidget;
    QWidget *notificationsWidget;

    // Patient form inputs
    QLineEdit *nomInput;
    QLineEdit *prenomInput;
    QComboBox *sexeInput;
    QLineEdit *cinInput;
    QComboBox *adresseInput;
    QLineEdit *numtelInput;
    QLineEdit *maladiechroniqueInput;


    // Statistics widgets
    QWidget *totalCard;
    QWidget *genderCard;
    QWidget *chronicCard;
    QWidget *cityCard;
    QWidget *statsContainer;
    QHBoxLayout *statsLayout;

    // --- Statistiques chercheurs ---
    //class ResearcherManager *researcherManager = nullptr;





    // Dark mode toggle
    bool darkModeEnabled = false;

    // Export buttons
    QPushButton *exportGenderChartBtn;
    QPushButton *exportChronicChartBtn;
    QPushButton *exportRegionChartBtn;
    QPushButton *toggleDarkModeBtn;

    // Statistics values
    QString totalPatientsValue;
    QString genderDistributionValue;
    QString chronicPercentageValue;
    QString topCityValue;

    // Managers
    ToolsManager *toolsManager;
    VaccinManager *vaccinManager;
    MessagesManager *messagesManager;
    PatientsManager *patientsManager;
    ResearcherManager *researchersManager;



    // Image upload data
    QByteArray uploadImageData;

    // Initialization Methods
    void setupUI();
    void setupSidebar();
    void setupPages();
    void setupToolsPage();
    void setupToolsTablePage();
    void setupAddToolPage();
    void setupVaccinsPage();
    void setupVaccinsTablePage();
    void setupAddVaccinFormPage();
    void setupEditVaccinFormPage();
    void setupMessagerieePage();
    void setuppatientsPage();
    void setupPatientsTablePage();
    void setupaddPatientsFormPage();
    void setupModifyPatientFormPage(const int patientID);

    void setupresearchersPage();
    void setupResearchersTablePage();
    void setupaddResearcherFormPage();
    void setupModifyResearcherFormPage(const int researcherID);
    void setupSearchWidget();
    void onSearchButtonClicked();

    QWidget *researchersStatsPage;
    QLineEdit *searchLineEdit;

    void setupResearchersStatsPage();



    void updateResearchersStatsChart();  // 👈 Add this declaration
    void exportResearchersTableToPDF();

   // void setupArduinoPanel();

    // Statistics methods
    QWidget* createModernStatCard(const QString &title, const QMap<QString, int> &data, const QString &bgColor);
    void updateStatistics();
    QWidget* createStatCard(const QString &iconPath, const QString &title, const QString &value);
    void updateStatistics(QWidget *totalCard, QWidget *genderCard, QWidget *chronicCard, QWidget *cityCard);

    // Vaccin page helper methods
    void addPageTitle(QVBoxLayout *vaccinsLayout);
    void addVaccinSummaryTable(QVBoxLayout *vaccinsLayout);
    QTableWidget* createVaccinSummaryTable();
    void addStatisticsSection(QVBoxLayout *vaccinsLayout);
    QWidget* createAgeGroupVaccinationWidget();
    QHBoxLayout* createVaccinationRateRow(const QString &ageGroup, double rate);
    QProgressBar* createVaccinationRateBar(double percentage);
    QWidget* createNotificationsWidget();
    QWidget* createVaccinTypeStatsWidget();
    QWidget* createExpiringVaccinsWidget();
    void addMoreButton(QVBoxLayout *vaccinsLayout);
    void updateVaccinTypeStats();
    QChartView* createPieChart(const QMap<QString, int> &typeStats, int totalVaccins);
    void applyAnimatedContainerStyle(QWidget *container);

    // Messagerie methods
    void addMessageBubble(const QString &message, const QString &sender, const QString &timestamp, bool isCurrentUser);
    void loadContacts();
    void clearConversation();

    // Resize handler class
    class ResizeEventFilter : public QObject {
    public:
        ResizeEventFilter(QWidget *target, QObject *parent = nullptr) : QObject(parent), m_target(target) {}

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override {
            if (event->type() == QEvent::Resize) {
                m_target->resize(static_cast<QWidget*>(obj)->size());
                return true;
            }
            return QObject::eventFilter(obj, event);
        }

    private:
        QWidget *m_target;
    };
signals:
    void taskMovedToCompleted(const QString &task);
    void taskMovedToTodo(const QString &task);
    void projectDataChanged();
};

#endif // MAINWINDOW_H
