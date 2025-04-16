#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
#include <QScrollBar>  // Ajout de l'inclusion pour QScrollBar

// Forward declarations
class ToolsManager;
class VaccinManager;
class MessagesManager;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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
    void updateSidebarIcons(QPushButton *selectedButton);

    // Tools-related slots
    void onAddToolClicked();
    void onEditToolClicked();
    void onDeleteToolClicked();

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
    void exportVaccinsTableToPDF();

    // Message-related slots
    void onReadMessageButtonClicked();
    void onNewMessageButtonClicked();
    void onDeleteMessageButtonClicked();
    void onContactSelected(int row);
    void onSendMessageClicked();
    void onRefreshMessagesButtonClicked();
    void displayConversation(int contactId);

private:
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
    QWidget* createVaccinTypeStatsWidget();
    QWidget* createExpiringVaccinsWidget();

    // Messagerie components - Nouveaux composants pour une interface conversationnelle
    QListWidget *contactsList;           // Liste des contacts/conversations
    QWidget *conversationContainer;      // Conteneur pour la conversation actuelle
    QScrollArea *messagesScrollArea;     // Zone de défilement pour les messages
    QWidget *messagesWidget;             // Widget contenant les messages
    QVBoxLayout *messagesLayout;         // Layout pour organiser les messages
    QTextEdit *messageInputField;        // Champ pour saisir un nouveau message
    QPushButton *sendMessageButton;      // Bouton pour envoyer un message
    int currentContactId;                // ID du contact/conversation actuellement sélectionné

    // Ancien composant de tableau (conservé pour compatibilité)
    QTableWidget *messageTable;
    MessagesManager *messagesManager;

    QChartView *chartView;
    QTimer *updateTimer;

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

    // Managers
    ToolsManager *toolsManager;
    VaccinManager *vaccinManager;

    // Image upload data
    QByteArray uploadImageData;

    // Initialization Methods
    void setupUI();
    void setupSidebar();
    void setupPages();

    // Tools-related methods
    void setupToolsPage();
    void setupToolsTablePage();
    void setupAddToolFormPage();

    // Vaccin-related methods
    void setupVaccinsPage();
    void setupVaccinsTablePage();
    void setupAddVaccinFormPage();
    void setupEditVaccinFormPage();

    // Messagerie
    void setupMessagerieePage();
    void addMessageBubble(const QString &message, const QString &sender, const QString &timestamp, bool isCurrentUser);
    void loadContacts();
    void clearConversation();



    // Vaccin page helper methods
    void addPageTitle(QVBoxLayout *vaccinsLayout);
    void addVaccinSummaryTable(QVBoxLayout *vaccinsLayout);
    QTableWidget* createVaccinSummaryTable();
    void addStatisticsSection(QVBoxLayout *vaccinsLayout);
    QWidget* createAgeGroupVaccinationWidget();
    QHBoxLayout* createVaccinationRateRow(const QString &ageGroup, double rate);
    QProgressBar* createVaccinationRateBar(double percentage);
    QWidget* createNotificationsWidget();
    void addMoreButton(QVBoxLayout *vaccinsLayout);
    void updateVaccinTypeStats();
    QChartView* createPieChart(const QMap<QString, int> &typeStats, int totalVaccins);
    void applyAnimatedContainerStyle(QWidget *container);

    // Message bubble styles
    QString getIncomingMessageStyle();
    QString getOutgoingMessageStyle();

    // Classe utilitaire pour gérer le redimensionnement
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

    // Utility method
    QPushButton* createSidebarButton(const QString &text, const QString &iconPath);
};

#endif // MAINWINDOW_H
