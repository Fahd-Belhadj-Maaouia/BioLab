#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QTableWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QButtonGroup>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QHeaderView>
#include <QChartView>
#include <QChart>
#include <QtCharts>
#include "toolsmanager.h"
#include "personnelmanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setCurrentUser(int userId, const QString &nom, const QString &prenom);
    ~MainWindow();

signals:  // Ajoutez cette section
    void logoutRequested();

public slots:  // Ajoutez cette section
    void logout();

private slots:
    void showPersonelPage();
    void showPatientsPage();
    void showResearchesPage();
    void showResearchersPage();
    void showToolsPage();
    void showVaccinsPage();
    void showSettingsPage();
    void showPersonnelTablePage();
    void onAddPersonelClicked();
    void onEditPersonelClicked();
    void onDeletePersonelClicked();
    void onSearchTextChanged(const QString &text);
    void onFilterByVille(int index);
    void onSortByRecent();
    void onBackToSummaryClicked();
    void updateStats();

private:
    void setupUI();
    void setupSidebar();
    void setupPages();
    void setupPersonelPage();
    void setupPersonelTablePage();
    void setupAddPersonelFormPage();
    void setupEditPersonelFormPage();
    void setupToolsPage();
    void styleTableView(QTableWidget *tableView);
    QPushButton* createSidebarButton(const QString &text, const QString &iconPath);
    void updateSidebarIcons(QPushButton *selectedButton);
    QLabel *photoLabel;
    QString currentPhotoBase64;
    void loadPersonnelToEdit();
    QString m_currentPhotoBase64;
    QComboBox *securityQuestionCombo;
    QLineEdit *securityAnswerEdit;
    QComboBox *editSecurityQuestionCombo;
    QLineEdit *editSecurityAnswerEdit;
    int currentUserId;
    QString currentUserName;
    QString currentUserRole;
    QString convertImageToBase64(const QString& imagePath);
    QPushButton *btnLogout;

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QWidget *contentWrapper;
    QWidget *sidebarWidget;
    QVBoxLayout *sidebarLayout;
    QButtonGroup *sidebarButtonGroup;
    QStackedWidget *stackedWidget;

    QPushButton *btnPatients;
    QPushButton *btnPersonel;
    QPushButton *btnResearches;
    QPushButton *btnResearchers;
    QPushButton *btnTools;
    QPushButton *btnVaccins;
    QPushButton *btnSettings;

    QWidget *patientsPage;
    QWidget *researchersPage;
    QWidget *settingsPage;
    QWidget *vaccinsPage;
    QWidget *personelPage;
    QWidget *PersonelTablePage;
    QWidget *addPersonelFormPage;
    QWidget *editPersonelFormPage;
    QWidget *toolsPage;
    QWidget *researchesPage;
    QWidget *toolsTablePage;
    QWidget *addToolFormPage;

    QTableWidget *miniPersonnelTable;
    QPushButton *voirPlusButton;
    QPushButton *backButton;
    QPushButton *sortRecentButton;
    QComboBox *villeFilterCombo;
    QLineEdit *searchInput;

    QChartView *villeChartView;
    QChartView *genderChartView;
    QLabel *totalLabel;
    QLabel *infirmiersLabel;
    QLabel *secretairesLabel;

    QLineEdit *nomEdit;
    QLineEdit *prenomEdit;
    QLineEdit *cinEdit;
    QLineEdit *numTelEdit;
    QLineEdit *emailEdit;
    QComboBox *villeEdit;
    QComboBox *roleEdit;
    QComboBox *sexeEdit;
    QDateEdit *dateAjoutEdit;

    PersonnelManager *personnelManager;
};

#endif // MAINWINDOW_H
