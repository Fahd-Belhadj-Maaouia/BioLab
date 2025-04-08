#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include "projetderecherche.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
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
    void showResearchTablePage();
    void updateSidebarIcons(QPushButton *selectedButton);
    void setupResearchCards();
    void setupResearchesFormAddPage();
    void showResearchFormAdd();
    void setupResearchesTablePage();


private:
    QTableView *tableView;
    ProjetDeRecherche Etmp;
    QWidget *centralWidget;
    QWidget *contentWrapper;
    QVBoxLayout *mainLayout;

    // Sidebar
    QVBoxLayout *sidebarLayout;
    QWidget *sidebarWidget;
    QPushButton *btnPatients;
    QPushButton *btnResearchers;
    QPushButton *btnSettings;
    QPushButton *btnVaccins;
    QPushButton *btnResearches;
    QPushButton *btnTools;
    QPushButton *btnPersonel;
    QButtonGroup *sidebarButtonGroup;  // Ensure only one selection at a time


    // Main Content
    QVBoxLayout *dashboardLayout;
    QWidget *dashboardWidget;
    QStackedWidget *stackedWidget;

    // Pages
    QWidget *patientsPage;
    QWidget *researchersPage;
    QWidget *settingsPage;
    QWidget *vaccinsPage;
    QWidget *researchesPage;
    QWidget *personelPage;
    QWidget *toolsPage;
    QWidget *researchesTablePage;
    QWidget *researchesFormAddPage;


    // Table
    QTableWidget *dataTable;

    void setupUI();
    void setupSidebar();
    void setupPages();
    void setupTable();
};

#endif // MAINWINDOW_H
