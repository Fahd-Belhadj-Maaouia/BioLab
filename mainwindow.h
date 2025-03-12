#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qcombobox.h"
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


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ToolsManager;
class PatientsManager;    // Forward declaration

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showPatientsPage();
    void showPersonelPage();
    void showResearchPage();
    void showResearchersPage();
    void showToolsPage();
    void showVaccinsPage();
    void showSettingsPage();
    void showToolsTablePage();
    void showPatientsTablePage();
    void updateSidebarIcons(QPushButton *selectedButton);

    // Tools-related slots
    void onAddToolClicked();
    void onEditToolClicked();
    void onDeleteToolClicked();



    void onAddPatientClicked();
    void onEditPatientClicked();
    void onDeletePatientClicked();
    void loadPatientData(int patientID, QLineEdit *nomInput, QLineEdit *prenomInput, QComboBox *sexeInput, QLineEdit *cinInput, QComboBox *adresseInput, QLineEdit *numtelInput, QLineEdit *maladiechroniqueInput);
    // Handle submission of modification
    void onModifyPatientSubmit(int patientID, QLineEdit *nomInput, QLineEdit *prenomInput, QComboBox *sexeInput, QLineEdit *cinInput, QComboBox *adresseInput, QLineEdit *numtelInput, QLineEdit *maladiechroniqueInput);

private:

    QLineEdit *nomInput;
    QLineEdit *prenomInput;
    QComboBox *sexeInput;
    QLineEdit *cinInput;
    QComboBox *adresseInput;
    QLineEdit *numtelInput;
    QLineEdit *maladiechroniqueInput;



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
    QWidget *toolsTablePage;
    QWidget *addToolFormPage;
    QWidget *PatientsTablePage;
    QWidget *addPatientsFormPage;
    QWidget *ModifyPatientFormPage;
    QByteArray uploadImageData;


    // Tools Manager
    PatientsManager *patientsManager;
    ToolsManager *toolsManager;



    // Initialization Functions
    void setupUI();
    void setupSidebar();
    void setupPages();
    void setupToolsPage();
    void setupToolsTablePage();
    void setupAddToolFormPage();
    void setuppatientsPage();
    void setupPatientsTablePage();
    void setupaddPatientsFormPage();
    void setupModifyPatientFormPage(const int patientID);
    QPushButton* createSidebarButton(const QString &text, const QString &iconPath);
};

#endif // MAINWINDOW_H
