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

// Forward declarations
class ToolsManager;
class VaccinManager;

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
    QButtonGroup *sidebarButtonGroup;

    // Main Content
    QStackedWidget *stackedWidget;

    // Pages
    QWidget *patientsPage;
    QWidget *researchersPage;
    QWidget *settingsPage;
    QWidget *vaccinsPage;
    QWidget *personelPage;
    QWidget *toolsPage;
    QWidget *researchesPage;

    // Tools Pages
    QWidget *toolsTablePage;
    QWidget *addToolFormPage;

    // Vaccin Pages
    QWidget *vaccinsTablePage;
    QWidget *addVaccinFormPage;

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

    // Vaccin page helper methods
    void addPageTitle(QVBoxLayout *vaccinsLayout);
    void addVaccinSummaryTable(QVBoxLayout *vaccinsLayout);
    QTableWidget* createVaccinSummaryTable();
    void addStatisticsSection(QVBoxLayout *vaccinsLayout);
    QWidget* createAgeGroupVaccinationWidget();
    QHBoxLayout* createVaccinationRateRow(const QString &ageGroup, double rate);
    QProgressBar* createVaccinationRateBar(double rate);
    QWidget* createNotificationsWidget();
    void addMoreButton(QVBoxLayout *vaccinsLayout);

    // Utility method
    QPushButton* createSidebarButton(const QString &text, const QString &iconPath);
};

#endif // MAINWINDOW_H
