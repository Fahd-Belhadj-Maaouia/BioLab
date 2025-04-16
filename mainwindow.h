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
#include "buttondelegate.h"
#include <QMainWindow>
#include <QListWidget>
#include<QSqlQuery>
#include"todolist.h"
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool initDatabase();
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
    void setupResearchesFormUpdatePage(int id);
    void setupResearchesTablePage();
    void handleDeleteRow(const QModelIndex &index);
    void searchProjects();
    void refreshCostChart();
    void exportToExcel();


private:


    QTableView *tableView;
    ProjetDeRecherche Etmp;
    QWidget *researchesFormUpdatePage;
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
    ToDoList *todoManager;
    QChartView* createCostRangeChart(); // New function for chart creation
    QWidget* chartCard = nullptr;       // Pointer to the card containing the chart
    QChartView* costChartView = nullptr;

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

    QListWidget *todoList;
    QListWidget *completedList;

    //searchTableVariables
    QSqlQueryModel *sqlModel;
    QSortFilterProxyModel *proxyModel;
    ButtonDelegate *buttonDelegate;

    void setupUI();
    void setupSidebar();
    void setupPages();
    void setupTable();

    // Ajoute cette ligne pour déclarer searchLineEdit
    QLineEdit *searchLineEdit;

signals:
    void taskMovedToCompleted(const QString &task);
    void taskMovedToTodo(const QString &task);
     void projectDataChanged();
};

#endif // MAINWINDOW_H
