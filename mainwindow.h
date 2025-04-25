#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt includes
#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QTableWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QButtonGroup>
#include <QLineEdit>
#include <QSpinBox>
#include <QFormLayout>
#include <QFileDialog>
#include <QDateEdit>
#include <QInputDialog>
#include <QProgressBar>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QTimer>
#include <QListWidget>
#include <QTextEdit>
#include <QSplitter>
#include <QScrollArea>
#include <QScrollBar>
#include <QComboBox>
#include <QMessageBox>
#include <QMap>
#include <QString>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QVariantMap>
#include "arduinomanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Forward declarations
class ToolsManager;
class VaccinManager;
class MessagesManager;
class PatientsManager;

// Add before MainWindow class declaration
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
    void showResearchersPage();
    void showToolsPage();
    void showVaccinsPage();
    void showSettingsPage();
    void showToolsTablePage();
    void showMessagerieePage();
    void showPatientsTablePage();
    void updateSidebarIcons(QPushButton *selectedButton);

    // Arduino-related slots
    void readSerialData();

    // Tools-related slots
    void onAddToolClicked();
    void onEditToolClicked();
    void onDeleteToolClicked();
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

    // UI Components
    QWidget *centralWidget;
    QWidget *contentWrapper;
    QVBoxLayout *mainLayout;

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
    QButtonGroup *sidebarButtonGroup;

    // Arduino components
    ArduinoManager *arduinoManager;
    QComboBox *portSelector;
    QPushButton *connectButton;
    QLabel *statusLabel;
    QLabel *detectedColorLabel;
    QLabel *vaccinInfoLabel;

    // Main Content
    QStackedWidget *stackedWidget;

    // Pages
    QWidget *patientsPage;
    QWidget *researchersPage;
    QWidget *settingsPage;
    QWidget *vaccinsPage;
    QWidget *messagerieePage;
    QWidget *personelPage;
    QWidget *toolsPage;
    QWidget *researchesPage;

    // Tools Pages
    QWidget *toolsTablePage;
    QWidget *addToolFormPage;

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

    // Image upload data
    QByteArray uploadImageData;

    // Initialization Methods
    void setupUI();
    void setupSidebar();
    void setupPages();
    void setupToolsPage();
    void setupToolsTablePage();
    void setupAddToolFormPage();
    void setupVaccinsPage();
    void setupVaccinsTablePage();
    void setupAddVaccinFormPage();
    void setupEditVaccinFormPage();
    void setupMessagerieePage();
    void setuppatientsPage();
    void setupPatientsTablePage();
    void setupaddPatientsFormPage();
    void setupModifyPatientFormPage(const int patientID);
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
};

#endif // MAINWINDOW_H
