#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// Qt Core
#include <QMainWindow>

// File handling & JSON
#include <QFile>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDate>

// QR Code library
#include "qrcodegen.hpp"

using qrcodegen::QrCode;
using qrcodegen::QrSegment;

// PDF and printing
#include <QPrinter>
#include <QPdfWriter>
#include <QTextDocument>
#include <QPainter>

// UI elements
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QDateEdit>
#include <QProgressBar>
#include <QInputDialog>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>

// Layouts and containers
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSplitter>
#include <QStackedWidget>
#include <QFrame>
#include <QGroupBox>
#include <QHeaderView>
#include <QStyle>
#include <QButtonGroup>

// Charts (QtCharts)
#include <QChartView>
#include <QPieSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QValueAxis>

// Miscellaneous
#include <QPixmap>
#include <QImage>
#include <QString>
#include <QByteArray>
#include <QUrl>

// Forward declarations
class QChart;
class QPieSeries;
class QBarSeries;

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
protected:
    void showEvent(QShowEvent *event) override;

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

    void onDeleteToolClicked(int);


    // Vaccin-related slots
    void showVaccinsTablePage();
    void onAddVaccinClicked();
    void onEditVaccinClicked();
    void onDeleteVaccinClicked();
    // New slots for pagination
    void updatePaginationControls(int currentPage, int totalPages);
    void exportToolsToPDF();

private:
    //qrcode
    QString generateRandomString(int length);
    void showToolQRCode(int toolId);
    QImage generateQRCode(const QString &text, int scale = 10, int border = 4);
    QWidget *qrCodePage = nullptr;
    QLabel *qrCodeLabel = nullptr;

    // Statistics widgets
    QWidget *statsContainer;
    QChartView *pieChartView;
    QChartView *barChartView;
    QChart *pieChart;
    QChart *barChart;

    // For data tracking
    QMap<QString, int> categoryCounts;
    QMap<QString, int> categoryStockTotals;
    QMap<QString, int> categoryMaxQuantities;

    // Methods
    void setupStatisticsSection();
    void updateStatistics();
    void calculateStatistics();

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
    QPushButton *uploadImageButton;
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
    QPushButton *firstPageBtn;
    QPushButton *prevPageBtn;
    QLabel *pageInfoLabel;
    QPushButton *nextPageBtn;
    QPushButton *lastPageBtn;
    QComboBox *itemsPerPageCombo;

    // PDF Export button
    QPushButton *exportPdfBtn;

    // Vaccin Pages
    QWidget *vaccinsTablePage;
    QWidget *addVaccinFormPage;
    QWidget *editVaccinFormPage;

    // Managers
    ToolsManager *toolsManager;
    VaccinManager *vaccinManager;

    // Initialization Methods
    void setupUI();
    void setupSidebar();
    void setupPages();

    // Tools-related methods
    void clearToolsForm();
    void setupToolsPage();
    void setupToolsTablePage();

    // Vaccin-related methods
    void setupVaccinsPage();
    void setupVaccinsTablePage();
    void setupAddVaccinFormPage();
    void setupEditVaccinFormPage();

    // Utility method
    QPushButton* createSidebarButton(const QString &text, const QString &iconPath);
};

#endif // MAINWINDOW_H
