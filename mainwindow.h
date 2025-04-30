#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qcombobox.h"
#include "researchermanager.h"
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QTableWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QButtonGroup>
#include <QMessageBox>

#include <QChartView>
#include <QChart>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>

#include "serialmanager.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ToolsManager; // Forward declaration
class ResearcherManager;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showPatientsPage();
    void showPersonelPage();
    void showResearchPage();
    void showresearchersPage();
    void showToolsPage();
    void showVaccinsPage();
    void showSettingsPage();
    void showToolsTablePage();
    void showResearchersTablePage();
    void updateSidebarIcons(QPushButton *selectedButton);

    // Tools-related slots
    void onAddToolClicked();
    void onEditToolClicked();
    void onDeleteToolClicked();


    void onAddResearcherClicked();
    void onEditResearcherClicked();
    void onDeleteResearcherClicked();
    void loadResearcherData(int researcherID, QLineEdit *nomInput, QLineEdit *prenomInput, QComboBox *sexeInput, QLineEdit *cinInput, QComboBox *adresseInput, QLineEdit *numtelInput, QLineEdit *specialiteInput, QLineEdit *datedenaissanceInput, QLineEdit *emailInput);
    // Handle submission of modification
    void onModifyResearcherSubmit(int researcherID, QLineEdit *nomInput, QLineEdit *prenomInput, QComboBox *sexeInput, QLineEdit *cinInput, QComboBox *adresseInput, QLineEdit *numtelInput, QLineEdit *specialiteInput, QLineEdit *datedenaissanceInput, QLineEdit *emailInput);
    void showResearchersStatsPage();


    void handleCardScanned(const QString &cardUID);


private:

    QLineEdit *nomInput;
    QLineEdit *prenomInput;
    QComboBox *sexeInput;
    QLineEdit *cinInput;
    QComboBox *adresseInput;
    QLineEdit *numtelInput;
    QLineEdit *specialiteInput;
    QLineEdit *datedenaissanceInput;
    QLineEdit *emailInput;


    // UI Components
    QWidget *centralWidget;
    QWidget *contentWrapper;
    QVBoxLayout *mainLayout;
    QWidget* searchWidget;


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
    QWidget *toolsTablePage;
    QWidget *addToolFormPage;
    QWidget *ResearchersTablePage;
    QWidget *addResearcherFormPage;
    QWidget *ModifyResearcherFormPage;
    QByteArray uploadImageData;


    // Tools Manager
    ToolsManager *toolsManager;
    ResearcherManager *researchersManager;


    // Initialization Functions
    void setupUI();

    void setupSerialConnection();  // New dedicated method for serial setup

    void setupSidebar();
    void setupPages();
    void setupToolsPage();
    void setupToolsTablePage();
    void setupAddToolFormPage();
    void setupresearchersPage();
    void setupResearchersTablePage();
    void setupaddResearcherFormPage();
    void setupModifyResearcherFormPage(const int researcherID);
    void setupSearchWidget();
    void onSearchButtonClicked();
    QPushButton* createSidebarButton(const QString &text, const QString &iconPath);


    QWidget *researchersStatsPage;
    void setupResearchersStatsPage();

    void updateResearchersStatsChart();  // 👈 Add this declaration
    void exportResearchersTableToPDF();


    SerialManager *serialManager = nullptr;




};

#endif // MAINWINDOW_H
