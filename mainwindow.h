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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class PersonnelManager;
class ToolsManager;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showPatientsPage();
    void showPersonelPage();
    void showPersonelTablePage();
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

    // Personnel-related slots
    void onAddPersonelClicked();
    void onEditPersonelClicked();
    void onDeletePersonelClicked();

private:
    QWidget *centralWidget;
    QWidget *contentWrapper;
    QVBoxLayout *mainLayout;

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

    QStackedWidget *stackedWidget;

    QWidget *patientsPage;
    QWidget *researchersPage;
    QWidget *settingsPage;
    QWidget *vaccinsPage;
    QWidget *personelPage;
    QWidget *PersonelTablePage;
    QWidget *addPersonelFormPage;
    QWidget *editPersonnelFormPage;
    QWidget *toolsPage;
    QWidget *researchesPage;
    QWidget *toolsTablePage;
    QWidget *addToolFormPage;
    QByteArray uploadImageData;

    ToolsManager *toolsManager;
    PersonnelManager *personnelManager;

    void setupUI();
    void setupSidebar();
    void setupPages();
    void setupPersonelPage();
    void setupPersonelTablePage();
    void setupToolsPage();
    void setupToolsTablePage();
    void setupAddToolFormPage();
    void setupAddPersonelFormPage();
    void setupeditPersonnelFormPage();

    // Déclaration unique
    QPushButton* createSidebarButton(const QString &text, const QString &iconPath);
};

#endif // MAINWINDOW_H
