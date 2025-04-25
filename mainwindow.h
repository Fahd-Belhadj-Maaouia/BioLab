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
#include <QMap>
#include <QString>
#include <QWidget>
#include <QMainWindow>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QButtonGroup>
#include <QVariantMap>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>





QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ToolsManager;
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




// Forward declaration

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showVaccinationHistory(int patientId);
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
    void exportStatsToPDF();



    void onAddPatientClicked();
    void onEditPatientClicked();
    void onDeletePatientClicked();
    void loadPatientData(int patientID, QLineEdit *nomInput, QLineEdit *prenomInput, QComboBox *sexeInput, QLineEdit *cinInput, QComboBox *adresseInput, QLineEdit *numtelInput, QLineEdit *maladiechroniqueInput);
    // Handle submission of modification
    void onModifyPatientSubmit(int patientID, QLineEdit *nomInput, QLineEdit *prenomInput, QComboBox *sexeInput, QLineEdit *cinInput, QComboBox *adresseInput, QLineEdit *numtelInput, QLineEdit *maladiechroniqueInput);
    bool patientExists( int patientID);
private:

    void clearLayout(QLayout *layout);
    void animateStatsUpdate();

    // Statistics widgets
    QWidget *totalCard;
    QWidget *genderCard;
    QWidget *chronicCard;
    QWidget *cityCard;
    QWidget *statsContainer;
    QHBoxLayout *statsLayout;

    // Dark mode toggle (optional)
    bool darkModeEnabled = false;

    QPushButton *exportGenderChartBtn;
    QPushButton *exportChronicChartBtn;
    QPushButton *exportRegionChartBtn;
    QPushButton *toggleDarkModeBtn;

    // Statistics values
    QString totalPatientsValue;
    QString genderDistributionValue;
    QString chronicPercentageValue;
    QString topCityValue;







    QLineEdit *nomInput;
    QLineEdit *prenomInput;
    QComboBox *sexeInput;
    QLineEdit *cinInput;
    QComboBox *adresseInput;
    QLineEdit *numtelInput;
    QLineEdit *maladiechroniqueInput;




    // Function to create stat cards
    QWidget* createModernStatCard(const QString &title,
                                  const QMap<QString, int> &data,
                                  const QString &bgColor);

    // Update statistics function
    void updateStatistics();


    QWidget* createStatCard(const QString &iconPath, const QString &title, const QString &value);
    void updateStatistics(QWidget *totalCard, QWidget *genderCard, QWidget *chronicCard, QWidget *cityCard);



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
