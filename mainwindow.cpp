#include "mainwindow.h"
#include "toolsmanager.h"
#include "vaccinmanager.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlError>
#include <QComboBox>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QUrl>
#include <QDate>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QUrl>
#include <QDate>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QPageSize>
#include <QMarginsF>
#include <QPageLayout>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    qDebug() << "MainWindow constructor start";
    pieChartView = new QChartView(this);
    barChartView = new QChartView(this);
    qDebug() << "Chart views created";
    setupUI();
     qDebug() << "MainWindow constructor complete";
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    // Central Widget and Main Layout
     qDebug() << "1. Starting setupUI";
    centralWidget = new QWidget(this);
     qDebug() << "2. Created central widget";
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);

    // Content Wrapper
    contentWrapper = new QWidget(this);
    contentWrapper->setStyleSheet(
        "background-color: #f8f8ff;"
        "border-radius: 20px;"
        "margin-top: 0px;"
        "padding: 20px;"
        "box-shadow: 0px 4px 10px rgba(0, 0, 0, 0.1);"
        );

    QVBoxLayout *contentWrapperLayout = new QVBoxLayout(contentWrapper);
    contentWrapperLayout->setContentsMargins(0, 0, 0, 0);

    // Setup Sidebar and Pages
    setupSidebar();
    setupPages();

    // Add Pages to Content Wrapper
    contentWrapperLayout->addWidget(stackedWidget);

    // Main Layout
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->addWidget(sidebarWidget);
    contentLayout->addWidget(contentWrapper, 1);
    mainLayout->addLayout(contentLayout);
}

void MainWindow::setupSidebar() {
    sidebarWidget = new QWidget(this);
    sidebarWidget->setFixedWidth(180);
    sidebarWidget->setStyleSheet(
        "background-color: #1a1a1a; border-radius: 15px;"
        );

    sidebarLayout = new QVBoxLayout(sidebarWidget);

    // Logo
    QLabel *logoLabel = new QLabel(this);
    QPixmap logoPixmap(":/icons/svg/BIOLAB.svg");
    logoLabel->setPixmap(logoPixmap);
    logoLabel->setAlignment(Qt::AlignLeft);
    logoLabel->setStyleSheet("QLabel { margin-left: 20px; margin-top: 10px; }");
    sidebarLayout->addWidget(logoLabel);

    // Sidebar Buttons
    btnPatients = createSidebarButton("  Patients", ":/icons/svg/patient.svg");
    btnPersonel = createSidebarButton("  Personel", ":/icons/svg/personel.svg");
    btnResearches = createSidebarButton("  Recherche", ":/icons/svg/flask.svg");
    btnResearchers = createSidebarButton("  Chercheurs", ":/icons/svg/scientist.svg");
    btnTools = createSidebarButton("  Matériels", ":/icons/svg/microscope.svg");
    btnVaccins = createSidebarButton("  Vaccin", ":/icons/svg/syringe.svg");
    btnSettings = createSidebarButton("  Paramètres", ":/icons/svg/settings.svg");

    // Button Group
    sidebarButtonGroup = new QButtonGroup(this);
    sidebarButtonGroup->addButton(btnPatients);
    sidebarButtonGroup->addButton(btnPersonel);
    sidebarButtonGroup->addButton(btnResearches);
    sidebarButtonGroup->addButton(btnResearchers);
    sidebarButtonGroup->addButton(btnTools);
    sidebarButtonGroup->addButton(btnVaccins);
    sidebarButtonGroup->addButton(btnSettings);
    sidebarButtonGroup->setExclusive(true);

    // Add Buttons to Sidebar
    sidebarLayout->addWidget(btnPatients);
    sidebarLayout->addWidget(btnPersonel);
    sidebarLayout->addWidget(btnResearches);
    sidebarLayout->addWidget(btnResearchers);
    sidebarLayout->addWidget(btnTools);
    sidebarLayout->addWidget(btnVaccins);
    sidebarLayout->addWidget(btnSettings);

    // Bottom Image
    QLabel *PicLabel = new QLabel(this);
    QPixmap PicPixmap(":/icons/svg/Asset 2.svg");
    PicLabel->setPixmap(PicPixmap.scaled(22, 22, Qt::KeepAspectRatio));
    PicLabel->setStyleSheet("QLabel { margin-left: 8px; margin-bottom: 50px; }");
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(PicLabel);

    // Connect Buttons to Slots
    connect(btnPatients, &QPushButton::clicked, this, &MainWindow::showPatientsPage);
    connect(btnPersonel, &QPushButton::clicked, this, &MainWindow::showPersonelPage);
    connect(btnResearches, &QPushButton::clicked, this, &MainWindow::showResearchPage);
    connect(btnResearchers, &QPushButton::clicked, this, &MainWindow::showResearchersPage);
    connect(btnTools, &QPushButton::clicked, this, &MainWindow::showToolsPage);
    connect(btnVaccins, &QPushButton::clicked, this, &MainWindow::showVaccinsPage);
    connect(btnSettings, &QPushButton::clicked, this, &MainWindow::showSettingsPage);
}

QPushButton* MainWindow::createSidebarButton(const QString &text, const QString &iconPath) {
    QPushButton *button = new QPushButton(text, this);
    button->setIcon(QIcon(iconPath));
    button->setIconSize(QSize(28, 28));
    button->setStyleSheet(
        "QPushButton { background: transparent; color: white; text-align: left; padding: 15px; font-size: 16px; border-radius: 8px; }"
        "QPushButton:hover { background: rgba(255, 255, 255, 0.1); }"
        "QPushButton:checked { background: #198754; color: white; font-weight: bold; }"
        "QPushButton::icon { margin-right: 10px; padding: 5px; icon-size: 20px; }"
        );
    return button;
}

void MainWindow::setupPages() {
    stackedWidget = new QStackedWidget(this);

    // Create Pages
    patientsPage = new QWidget();
    researchersPage = new QWidget();
    settingsPage = new QWidget();
    vaccinsPage = new QWidget();
    personelPage = new QWidget();
    toolsPage = new QWidget();
    researchesPage = new QWidget();

    // Tools Pages
    toolsTablePage = new QWidget();


    // Vaccin Pages
    vaccinsTablePage = new QWidget();
    addVaccinFormPage = new QWidget();
    editVaccinFormPage = new QWidget();

    // Setup Pages
    setupToolsPage();
    setupToolsTablePage();

    setupVaccinsPage();
    setupVaccinsTablePage();
    setupAddVaccinFormPage();
    setupEditVaccinFormPage();

    // Add Pages to Stacked Widget
    stackedWidget->addWidget(patientsPage);
    stackedWidget->addWidget(researchersPage);
    stackedWidget->addWidget(vaccinsPage);
    stackedWidget->addWidget(personelPage);
    stackedWidget->addWidget(researchesPage);
    stackedWidget->addWidget(toolsPage);
    stackedWidget->addWidget(settingsPage);
    stackedWidget->addWidget(toolsTablePage);

    stackedWidget->addWidget(vaccinsTablePage);
    stackedWidget->addWidget(addVaccinFormPage);
    stackedWidget->addWidget(editVaccinFormPage);

    // Set Default Page
    stackedWidget->setCurrentWidget(patientsPage);
}

void MainWindow::setupToolsPage() {
    QVBoxLayout *toolsLayout = new QVBoxLayout(toolsPage);
    toolsLayout->setSpacing(20);

    // Add statistics section at the top
    statsContainer = new QWidget();
    statsContainer->setStyleSheet(R"(
    QWidget {
        background: white;
        border-radius: 10px;
        padding: 15px;
        border: 1px solid #e0e0e0;
    }
    QChartView {
        background: transparent;
        border: none;
    }
    QLabel {
        font-family: 'Segoe UI', Arial, sans-serif;
        color: #333;
    }
    )");

    QHBoxLayout *statsLayout = new QHBoxLayout(statsContainer);
    statsLayout->setContentsMargins(0, 0, 0, 0);
    statsLayout->setSpacing(10);

    // Setup the statistics charts
    setupStatisticsSection();
    statsLayout->addWidget(pieChartView, 1);  // Now using statsLayout
    statsLayout->addWidget(barChartView, 1);  // Now using statsLayout


    // Add "Voir Matériels" Button below statistics
    QPushButton *goToToolsTableButton = new QPushButton("Voir Matériels", this);
    goToToolsTableButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #198754;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #157347;"
        "}"
        );

    // Add widgets to layout
    toolsLayout->addWidget(statsContainer);
    toolsLayout->addWidget(goToToolsTableButton, 0, Qt::AlignCenter);

    // Connect Button to Slot
    connect(goToToolsTableButton, &QPushButton::clicked, this, &MainWindow::showToolsTablePage);


}
void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);

    // Refresh statistics when tools page is shown
    if (stackedWidget->currentWidget() == toolsPage) {
        calculateStatistics();
        updateStatistics();
    }
}
void MainWindow::setupStatisticsSection() {
    // Create pie chart (left side)
    pieChart = new QChart();
    pieChart->setTitle("Distribution du stock par Category");
    pieChart->setTitleFont(QFont("Arial", 10, QFont::Bold));
    pieChart->setAnimationOptions(QChart::AllAnimations);
    pieChart->legend()->setVisible(true);
    pieChart->legend()->setAlignment(Qt::AlignRight);

    pieChartView->setChart(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    pieChartView->setMinimumHeight(300);

    // Create bar chart (right side)
    barChart = new QChart();
    barChart->setTitle("Stock vs Quantitée Maximale");
    barChart->setTitleFont(QFont("Arial", 10, QFont::Bold));
    barChart->setAnimationOptions(QChart::AllAnimations);

    barChartView->setChart(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);
    barChartView->setMinimumHeight(300);
}

void MainWindow::calculateStatistics() {
    categoryCounts.clear();
    categoryStockTotals.clear();
    categoryMaxQuantities.clear();

    // Get data from ToolsManager
    QSqlQuery query = toolsManager->getAllToolsQuery();

    while (query.next()) {
        QString category = query.value("categorie").toString();
        int stock = query.value("stock").toInt();
        int maxQuantity = query.value("quantiteMaximale").toInt();

        // Update counts
        categoryCounts[category]++;

        // Update stock totals
        if (categoryStockTotals.contains(category)) {
            categoryStockTotals[category] += stock;
        } else {
            categoryStockTotals[category] = stock;
        }

        // Update max quantities
        if (categoryMaxQuantities.contains(category)) {
            categoryMaxQuantities[category] += maxQuantity;
        } else {
            categoryMaxQuantities[category] = maxQuantity;
        }
    }
}
void MainWindow::updateStatistics() {
    // Clear previous data
    if (pieChart->series().count() > 0) {
        pieChart->removeAllSeries();
    }
    if (barChart->series().count() > 0) {
        barChart->removeAllSeries();
    }
    // Clear bar chart data and axes
    if (barChart->series().count() > 0) {
        barChart->removeAllSeries();
    }

    // Remove all axes from bar chart
    QList<QAbstractAxis*> oldAxes = barChart->axes();
    for (QAbstractAxis* axis : oldAxes) {
        barChart->removeAxis(axis);
        delete axis;  // Important to prevent memory leaks
    }
    // Update pie chart
    QPieSeries *pieSeries = new QPieSeries();
    int totalTools = 0;
    for (int count : categoryCounts) {
        totalTools += count;
    }

    if (totalTools > 0) {  // Only update if we have data
        for (const QString &category : categoryCounts.keys()) {
            double percentage = (categoryCounts[category] * 100.0) / totalTools;
            QPieSlice *slice = pieSeries->append(category, percentage);
            slice->setLabel(QString("%1 (%2%)").arg(category).arg(QString::number(percentage, 'f', 1)));
        }
        pieChart->addSeries(pieSeries);
    }

    // Update bar chart
    if (!categoryStockTotals.isEmpty()) {  // Only update if we have data
        QBarSeries *barSeries = new QBarSeries();
        QBarSet *stockSet = new QBarSet("Current Stock");
        QBarSet *maxSet = new QBarSet("Max Capacity");

        for (const QString &category : categoryStockTotals.keys()) {
            *stockSet << categoryStockTotals[category];
            *maxSet << categoryMaxQuantities[category];
        }

        barSeries->append(stockSet);
        barSeries->append(maxSet);
        barChart->addSeries(barSeries);

        // Set up axes
        QStringList categories = categoryStockTotals.keys();
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        barChart->addAxis(axisX, Qt::AlignBottom);
        barSeries->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        axisY->setLabelFormat("%d");
        axisY->setTitleText("Quantity");
        barChart->addAxis(axisY, Qt::AlignLeft);
        barSeries->attachAxis(axisY);
    }

    // Style the charts
    pieChart->setTheme(QChart::ChartThemeLight);
    barChart->setTheme(QChart::ChartThemeLight);
}

void MainWindow::setupToolsTablePage() {
    // Create a horizontal layout for the entire page
    QHBoxLayout *mainLayout = new QHBoxLayout(toolsTablePage);

    // Left side - Tools Table
    QWidget *tableWidget = new QWidget();
    QVBoxLayout *tableLayout = new QVBoxLayout(tableWidget);
    tableLayout->setContentsMargins(15, 15, 15, 15);
    tableLayout->setSpacing(12);

    // Back button at the top of the table section
    QPushButton *backButton = new QPushButton(this);
    backButton->setIcon(QIcon(":/icons/svg/back.svg"));
    backButton->setIconSize(QSize(24, 24));
    backButton->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    border: none;"
        "    padding: 8px;"
        "    border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(0, 0, 0, 0.1);"
        "}"
        );
    tableLayout->addWidget(backButton, 0, Qt::AlignLeft);

    // Modern filter bar - streamlined and professional
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->setSpacing(12);

    // Search input with dropdown
    QComboBox *searchFieldCombo = new QComboBox();
    searchFieldCombo->addItem("Nom Matériel");
    searchFieldCombo->addItem("Catégorie");
    searchFieldCombo->addItem("Fournisseur");
    searchFieldCombo->setFixedWidth(140);
    searchFieldCombo->setStyleSheet(
        "QComboBox {"
        "    padding: 8px 12px;"
        "    border: 1px solid #d4d4d4;"
        "    border-radius: 4px;"
        "    background-color: #ffffff;"
        "}"
        "QComboBox::drop-down {"
        "    border: 0px;"
        "    width: 20px;"
        "}"
        "QComboBox::down-arrow {"
        "    image: url(:/icons/svg/down-arrow.svg);"
        "    width: 12px;"
        "    height: 12px;"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 1px solid #d4d4d4;"
        "    selection-background-color: #f5f5f5;"
        "}"
        );

    QLineEdit *searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Rechercher...");
    searchInput->setStyleSheet(
        "QLineEdit {"
        "    padding: 8px 12px;"
        "    border: 1px solid #d4d4d4;"
        "    border-radius: 4px;"
        "    background-color: #ffffff;"
        "}"
        );

    QPushButton *searchButton = new QPushButton("Rechercher");
    searchButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #198754;"
        "    color: white;"
        "    padding: 8px 16px;"
        "    border-radius: 4px;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: #157347;"
        "}"
        );

    // Sort controls - clean and horizontal
    QLabel *sortLabel = new QLabel("Trier:");
    sortLabel->setStyleSheet("color: #333;");

    QComboBox *sortFieldCombo = new QComboBox();
    sortFieldCombo->addItem("Catégorie");
    sortFieldCombo->addItem("Nom Matériel");
    sortFieldCombo->addItem("Stock");
    sortFieldCombo->setFixedWidth(120);
    sortFieldCombo->setStyleSheet(searchFieldCombo->styleSheet());

    QPushButton *sortAscButton = new QPushButton("↑");
    QPushButton *sortDescButton = new QPushButton("↓");

    QString sortButtonStyle =
        "QPushButton {"
        "    background-color: #f8f9fa;"
        "    color: #333;"
        "    padding: 8px 12px;"
        "    border: 1px solid #d4d4d4;"
        "    min-width: 40px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e9ecef;"
        "}";

    sortAscButton->setStyleSheet(sortButtonStyle + "border-radius: 4px 0 0 4px; border-right: none;");
    sortDescButton->setStyleSheet(sortButtonStyle + "border-radius: 0 4px 4px 0;");

    // Add all to filter layout
    filterLayout->addWidget(searchFieldCombo);
    filterLayout->addWidget(searchInput, 1);  // Give search box more space
    filterLayout->addWidget(searchButton);
    filterLayout->addSpacing(20);  // Space between search and sort
    filterLayout->addWidget(sortLabel);
    filterLayout->addWidget(sortFieldCombo);
    filterLayout->addWidget(sortAscButton);
    filterLayout->addWidget(sortDescButton);

    tableLayout->addLayout(filterLayout);

    // Table with header
    QVBoxLayout *tableContainerLayout = new QVBoxLayout();
    tableContainerLayout->setSpacing(0);
    tableContainerLayout->setContentsMargins(0, 0, 0, 0);

    // Custom header for the table - clean and professional
    QWidget *tableHeader = new QWidget();
    tableHeader->setStyleSheet(

        "border: 1px solid #d4d4d4;"
        "border-bottom: none;"
        );
    QHBoxLayout *headerLayout = new QHBoxLayout(tableHeader);
    headerLayout->setContentsMargins(8, 6, 8, 6);
    headerLayout->setSpacing(4);

    // Define column headers and their widths - adjust these to match your actual data columns
    QStringList headers = {"Nom Matériel", "Catégorie","Description", "Stock", "Stock Max","Image","Fournisseur", "Nom Projet", "Actions"};
    int columnWidths[] = {12, 13, 12,8, 8,10,12 , 15, 25}; // percentages

    for (int i = 0; i < headers.size(); i++) {
        QLabel *headerLabel = new QLabel(headers[i]);
        headerLabel->setStyleSheet("font-weight: 500; color: #333;");
        headerLayout->addWidget(headerLabel, columnWidths[i]);
    }

    tableContainerLayout->addWidget(tableHeader);
    QWidget *spacer = new QWidget();
    spacer->setFixedHeight(8); // Adjust this value to control the amount of space
    spacer->setStyleSheet("border-left: 1px; border-right: 1px;");
    tableContainerLayout->addWidget(spacer);

    // Create Tools Table - modern and clean
    QTableWidget *toolstable = new QTableWidget(this);
    toolstable->setStyleSheet(
        "QTableWidget {"
        "    border: 1px solid #d4d4d4;"
        "    gridline-color: #e6e6e6;"
        "    background-color: #ffffff;"
        "    selection-background-color: #e6f2ff;"
        "    padding-top: 18px;"
        "}"
        "QTableWidget::item {"
        "    padding: 6px 4px;"
        "    border-bottom: 1px solid #e6e6e6;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #e6f2ff;"
        "    color: #333;"
        "}"
        // Alternating row colors
        "QTableWidget::item:alternate {"
        "    background-color: #f9f9f9;"
        "}"
        );
    toolstable->verticalHeader()->setDefaultSectionSize(38);
    toolstable->setShowGrid(false); // Hide grid for cleaner look
    toolstable->setAlternatingRowColors(true);
    toolstable->setSelectionBehavior(QAbstractItemView::SelectRows);
    toolstable->verticalHeader()->setVisible(false);
    toolstable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Hide the default header since we're using our custom one
    toolstable->horizontalHeader()->setVisible(false);

    tableContainerLayout->addWidget(toolstable);
    tableLayout->addLayout(tableContainerLayout, 1); // Give table most of the space

    // Modern pagination bar - sleek and minimal
    QHBoxLayout *paginationLayout = new QHBoxLayout();
    paginationLayout->setContentsMargins(0, 10, 0, 0);

    // Pagination controls
    QString paginationButtonStyle =
        "QPushButton {"
        "    border: 1px solid #d4d4d4;"
        "    background-color: #ffffff;"
        "    color: #198754;"
        "    padding: 6px 12px;"
        "    min-width: 36px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #f5f5f5;"
        "}"
        "QPushButton:disabled {"
        "    color: #aaa;"
        "}";

    firstPageBtn = new QPushButton("«", this);
    firstPageBtn->setToolTip("Première page");
    firstPageBtn->setStyleSheet(paginationButtonStyle + "border-radius: 4px 0 0 4px; border-right: none;");

    prevPageBtn = new QPushButton("‹", this);
    prevPageBtn->setToolTip("Page précédente");
    prevPageBtn->setStyleSheet(paginationButtonStyle + "border-right: none; border-radius: 0;");

    pageInfoLabel = new QLabel("Page 1 / 1");
    pageInfoLabel->setAlignment(Qt::AlignCenter);
    pageInfoLabel->setStyleSheet(
        "padding: 6px 12px; border-top: 1px solid #d4d4d4; border-bottom: 1px solid #d4d4d4; background-color: #ffffff;"
        );
    pageInfoLabel->setMinimumWidth(100);

    nextPageBtn = new QPushButton("›", this);
    nextPageBtn->setToolTip("Page suivante");
    nextPageBtn->setStyleSheet(paginationButtonStyle + "border-radius: 0; border-left: none; border-right: none;");

    lastPageBtn = new QPushButton("»", this);
    lastPageBtn->setToolTip("Dernière page");
    lastPageBtn->setStyleSheet(paginationButtonStyle + "border-radius: 0 4px 4px 0; border-left: none;");

    // Items per page dropdown - clean and compact
    QLabel *itemsPerPageLabel = new QLabel("Afficher:");
    itemsPerPageLabel->setStyleSheet("color: #333;");

    itemsPerPageCombo = new QComboBox(this);
    itemsPerPageCombo->addItem("10", 10);
    itemsPerPageCombo->addItem("20", 20);
    itemsPerPageCombo->addItem("50", 50);
    itemsPerPageCombo->setFixedWidth(60);
    itemsPerPageCombo->setStyleSheet(
        "QComboBox {"
        "    padding: 6px 8px;"
        "    border: 1px solid #d4d4d4;"
        "    border-radius: 4px;"
        "    background-color: #ffffff;"
        "}"
        "QComboBox::drop-down {"
        "    border: 0px;"
        "    width: 20px;"
        "}"
        );

    // Export button - professional style
    exportPdfBtn = new QPushButton("Liste des ressources", this);
    exportPdfBtn->setIcon(QIcon(":/icons/svg/pdf.svg"));
    exportPdfBtn->setIconSize(QSize(16, 16));
    exportPdfBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #198754;"
        "    color: white;"
        "    padding: 6px 12px;"
        "    border-radius: 4px;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: #157347;"
        "}"
        );

    // Add pagination controls to layout
    paginationLayout->addWidget(firstPageBtn);
    paginationLayout->addWidget(prevPageBtn);
    paginationLayout->addWidget(pageInfoLabel);
    paginationLayout->addWidget(nextPageBtn);
    paginationLayout->addWidget(lastPageBtn);
    paginationLayout->addSpacing(15);
    paginationLayout->addWidget(itemsPerPageLabel);
    paginationLayout->addWidget(itemsPerPageCombo);
    paginationLayout->addStretch();
    paginationLayout->addWidget(exportPdfBtn);

    tableLayout->addLayout(paginationLayout);

    // Initialize ToolsManager
    toolsManager = new ToolsManager(toolstable, this);

    connect(toolsManager, &ToolsManager::dataChanged, this, &MainWindow::calculateStatistics);
    connect(toolsManager, &ToolsManager::dataChanged, this, &MainWindow::updateStatistics);

    // Initial data load
    calculateStatistics();
    updateStatistics();
    // Right side - Form Widget
    QWidget *formWidget = new QWidget();
    QVBoxLayout *formLayout = new QVBoxLayout(formWidget);

    // Section title
    QLabel *formTitle = new QLabel("Ajouter/Modifier Matériel");
    formTitle->setStyleSheet("font-size: 16px; font-weight: bold; margin-bottom: 10px;");
    formLayout->addWidget(formTitle);

    // Input fields
    QLineEdit *nomMaterielInput = new QLineEdit();
    QComboBox *categorieCombo = new QComboBox();
    categorieCombo->addItem(""); // Optional empty item
    categorieCombo->addItems(toolsManager->getCategories());
    categorieCombo->setEditable(false); // Allows custom categories if needed
    QLineEdit *descriptionInput = new QLineEdit();
    QSpinBox *stockInput = new QSpinBox();
    stockInput->setMinimum(0);
    QSpinBox *quantiteMaximaleInput = new QSpinBox();
    quantiteMaximaleInput->setMinimum(0);
    QPushButton *uploadImageButton = new QPushButton("Upload Image");
    QLineEdit *fournisseurInput = new QLineEdit();

    categorieCombo->setStyleSheet(
        "QComboBox {"
        "    padding: 8px;"
        "    border: 1px solid #ddd;"
        "    border-radius: 4px;"
        "}"
        "QComboBox::drop-down {"
        "    border: 0px;"
        "}"
        "QComboBox::down-arrow {"
        "    image: url(:/icons/svg/down-arrow.svg);" // Add your arrow icon
        "    width: 12px;"
        "    height: 12px;"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 1px solid #ddd;"
        "    selection-background-color: #f0f0f0;"
        "}"
        );
    // Add fields to the form
    QFormLayout *inputLayout = new QFormLayout();
    inputLayout->addRow("Nom Materiel:", nomMaterielInput);
    inputLayout->addRow("Catégorie:", categorieCombo);
    inputLayout->addRow("Description:", descriptionInput);
    inputLayout->addRow("Stock:", stockInput);
    inputLayout->addRow("Quantité Maximale:", quantiteMaximaleInput);
    inputLayout->addRow("Image:", uploadImageButton);
    inputLayout->addRow("Fournisseur:", fournisseurInput);
    formLayout->addLayout(inputLayout);

    // Add a submit button
    QPushButton *submitButton = new QPushButton("Ajouter");
    QPushButton *clearButton = new QPushButton("Effacer");

    // Add buttons to a horizontal layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(submitButton);
    formLayout->addLayout(buttonLayout);
    formLayout->addStretch();

    // Style the form
    formWidget->setStyleSheet(
        "QLineEdit, QSpinBox {"
        "    padding: 8px;"
        "    border: 1px solid #ddd;"
        "    border-radius: 4px;"
        "}"
        "QPushButton {"
        "    background-color: #198754;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #157347;"
        "}"
        );

    // Set a maximum width for the form widget to ensure it doesn't take too much space
    formWidget->setMaximumWidth(400);

    // Set sizes for the two sections
    tableWidget->setMinimumWidth(600);

    // Add both sections to the main layout with a splitter
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(tableWidget);
    splitter->addWidget(formWidget);
    splitter->setStretchFactor(0, 2);  // Table gets more space
    splitter->setStretchFactor(1, 1);  // Form gets less space
    mainLayout->addWidget(splitter);

    // Connect search and sort buttons to ToolsManager methods
    connect(searchButton, &QPushButton::clicked, this, [this, searchInput, searchFieldCombo]() {
        toolsManager->searchTools(searchInput->text().trimmed(),
                                  searchFieldCombo->currentText());
    });

    connect(sortAscButton, &QPushButton::clicked, this, [this, sortFieldCombo]() {
        toolsManager->sortTools(sortFieldCombo->currentText(), Qt::AscendingOrder);
    });

    connect(sortDescButton, &QPushButton::clicked, this, [this, sortFieldCombo]() {
        toolsManager->sortTools(sortFieldCombo->currentText(), Qt::DescendingOrder);
    });

    // Connect the upload button
    auto imagePath = std::make_shared<QString>();  // Shared pointer to QString

    connect(uploadImageButton, &QPushButton::clicked, this, [imagePath, uploadImageButton]() {
        *imagePath = QFileDialog::getOpenFileName(nullptr,
                                                  "Upload Image",
                                                  "",
                                                  "Images (*.png *.jpg *.jpeg)");

        if (!imagePath->isEmpty()) {
            uploadImageButton->setText("Image Selected");
            QMessageBox::information(nullptr, "Success", "Image uploaded successfully.");
        }
    });

    // Connect the submit button
    connect(submitButton, &QPushButton::clicked, this, [this, nomMaterielInput, categorieCombo, descriptionInput, stockInput, quantiteMaximaleInput, fournisseurInput, imagePath, submitButton, uploadImageButton]
            () {
                // Get input values
                QString nomMateriel = nomMaterielInput->text();
                QString categorie = categorieCombo->currentText();
                QString description = descriptionInput->text();
                int stock = stockInput->value();
                int quantiteMaximale = quantiteMaximaleInput->value();
                QString fournisseur = fournisseurInput->text();

                // Set values in ToolsManager
                toolsManager->setNomMateriel(nomMateriel);
                toolsManager->setCategorie(categorie);
                toolsManager->setDescription(description);
                toolsManager->setStock(stock);
                toolsManager->setQuantiteMaximale(quantiteMaximale);
                toolsManager->setUploadImage(*imagePath);
                toolsManager->setFournisseur(fournisseur);

                // Check if we're in edit mode
                if (submitButton->text() == "Modifier") {
                    // Update the tool
                    if (toolsManager->editTool()) {
                        // Reset form
                        submitButton->setText("Ajouter");
                        toolsManager->setId(-1);
                        nomMaterielInput->clear();
                        categorieCombo->setCurrentIndex(0);
                        descriptionInput->clear();
                        stockInput->setValue(0);
                        quantiteMaximaleInput->setValue(0);
                        uploadImageButton->setText("Upload Image");
                        fournisseurInput->clear();
                    }
                } else {
                    // Add the tool to the database
                    if (toolsManager->addTool()) {
                        // Reset form
                        nomMaterielInput->clear();
                        categorieCombo->setCurrentIndex(0);
                        descriptionInput->clear();
                        stockInput->setValue(0);
                        quantiteMaximaleInput->setValue(0);
                        uploadImageButton->setText("Upload Image");
                        fournisseurInput->clear();
                    }
                }
            });

    // Connect the clear button
    connect(clearButton, &QPushButton::clicked, this, [nomMaterielInput, categorieCombo, descriptionInput, stockInput, quantiteMaximaleInput, uploadImageButton, fournisseurInput, submitButton, this]() {
        nomMaterielInput->clear();
        categorieCombo->setCurrentIndex(-1);
        descriptionInput->clear();
        stockInput->setValue(0);
        quantiteMaximaleInput->setValue(0);
        uploadImageButton->setText("Upload Image");
        fournisseurInput->clear();
        submitButton->setText("Ajouter");
        toolsManager->setId(-1);
    });

    // Connect back button to slot
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showToolsPage);

    // Connect edit and delete signals from ToolsManager
    connect(toolsManager, &ToolsManager::editToolRequested,
            this, [=](int id) {
                // Fetch the tool data by ID
                QMap<QString, QVariant> toolData = toolsManager->getToolById(id);
                if (toolData.isEmpty()) {
                    QMessageBox::warning(this, "Erreur", "ID de matériel non trouvé.");
                    return;
                }

                // Pre-fill the form fields
                nomMaterielInput->setText(toolData["nomMateriel"].toString());
                QString currentCategory = toolData["categorie"].toString();
                int index = categorieCombo->findText(currentCategory);
                if (index >= 0) {
                    categorieCombo->setCurrentIndex(index);
                } else {
                    // If category doesn't exist in predefined list, add it
                    categorieCombo->addItem(currentCategory);
                    categorieCombo->setCurrentIndex(categorieCombo->count()-1);
                }
                descriptionInput->setText(toolData["description"].toString());
                stockInput->setValue(toolData["stock"].toInt());
                quantiteMaximaleInput->setValue(toolData["quantiteMaximale"].toInt());
                fournisseurInput->setText(toolData["fournisseur"].toString());

                // Set the image path (if any)
                QString imagePath = toolData["uploadImage"].toString();
                if (!imagePath.isEmpty()) {
                    uploadImageButton->setText("Image sélectionnée");
                }

                // Change the submit button text to "Modifier"
                submitButton->setText("Modifier");

                // Set the ID of the tool being edited
                toolsManager->setId(id);
            });
    // Connect pagination buttons to slots
    connect(toolsManager, &ToolsManager::deleteToolRequested,
            this, &MainWindow::onDeleteToolClicked);

    connect(firstPageBtn, &QPushButton::clicked, this, [this]() {
        toolsManager->goToPage(1);
    });

    connect(prevPageBtn, &QPushButton::clicked, this, [this]() {
        toolsManager->goToPage(toolsManager->getCurrentPage() - 1);
    });

    connect(nextPageBtn, &QPushButton::clicked, this, [this]() {
        toolsManager->goToPage(toolsManager->getCurrentPage() + 1);
    });

    connect(lastPageBtn, &QPushButton::clicked, this, [this]() {
        toolsManager->goToPage(toolsManager->getTotalPages());
    });

    connect(itemsPerPageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        int count = itemsPerPageCombo->itemData(index).toInt();
        toolsManager->setItemsPerPage(count);
    });
    connect(toolsManager, &ToolsManager::qrCodeRequested,
            this, &MainWindow::showToolQRCode);

    // Connect PDF export button
    connect(exportPdfBtn, &QPushButton::clicked, this, &MainWindow::exportToolsToPDF);

    // Connect pagination change signal from ToolsManager
    connect(toolsManager, &ToolsManager::paginationChanged, this, &MainWindow::updatePaginationControls);

}

void MainWindow::updatePaginationControls(int currentPage, int totalPages) {
    // Update the page info label
    pageInfoLabel->setText(QString("Page %1 / %2").arg(currentPage).arg(totalPages));

    // Enable/disable navigation buttons based on current position
    firstPageBtn->setEnabled(currentPage > 1);
    prevPageBtn->setEnabled(currentPage > 1);
    nextPageBtn->setEnabled(currentPage < totalPages);
    lastPageBtn->setEnabled(currentPage < totalPages);
}

void MainWindow::exportToolsToPDF() {
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    "Enregistrer la liste des ressources",
                                                    QString(),
                                                    "Fichiers PDF (*.pdf)");

    if (!filePath.isEmpty()) {
        // Ensure the file has a .pdf extension
        if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) {
            filePath += ".pdf";
        }

        if (toolsManager->exportToPDF(filePath)) {
            QMessageBox::information(this, "Succès", "La liste des ressources a été exportée avec succès.");
        } else {
            QMessageBox::warning(this, "Erreur", "Une erreur s'est produite lors de l'exportation de la liste.");
        }
    }
}
// Tools-related slots

void MainWindow::onDeleteToolClicked(int id) {
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(this, "Confirmer suppression",
                                    "Voulez-vous vraiment supprimer ce matériel?",
                                    QMessageBox::Yes|QMessageBox::No);

    if (confirm == QMessageBox::Yes) {
        toolsManager->setId(id);
        if (toolsManager->deleteTool()) {
            QMessageBox::information(this, "Succès", "Matériel supprimé avec succès");
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la suppression du matériel");
        }
    }
}

QImage MainWindow::generateQRCode(const QString &text, int scale, int border) {
    using namespace qrcodegen;

    QrCode qr = QrCode::encodeText(text.toUtf8().constData(), QrCode::Ecc::MEDIUM);

    QImage image(qr.getSize() * scale + border * 2,
                 qr.getSize() * scale + border * 2,
                 QImage::Format_RGB32);
    image.fill(Qt::white);

    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            if (qr.getModule(x, y)) {
                for (int sy = 0; sy < scale; sy++) {
                    for (int sx = 0; sx < scale; sx++) {
                        image.setPixel(border + x * scale + sx,
                                       border + y * scale + sy,
                                       qRgb(0, 0, 0));
                    }
                }
            }
        }
    }
    return image;
}

void MainWindow::showToolQRCode(int toolId) {
    if (!qrCodePage) {
        qrCodePage = new QWidget();
        stackedWidget->addWidget(qrCodePage);

        // Main layout: horizontal
        QHBoxLayout *mainLayout = new QHBoxLayout(qrCodePage);

        // === Left side: just the back button ===
        QVBoxLayout *leftLayout = new QVBoxLayout();
        QPushButton *backButton = new QPushButton;
        backButton->setIcon(QIcon(":/icons/svg/back.svg"));
        backButton->setStyleSheet("text-align: left;");
        connect(backButton, &QPushButton::clicked, this, &MainWindow::showToolsTablePage);
        leftLayout->addWidget(backButton, 0, Qt::AlignTop | Qt::AlignLeft);
        leftLayout->addStretch(); // Push it to the top
        mainLayout->addLayout(leftLayout);
        mainLayout->setStretch(0, 0); // Back button section: no horizontal stretch

        // === Center area: QR code and label centered vertically and horizontally ===
        QWidget *centerWidget = new QWidget();
        QVBoxLayout *centerLayout = new QVBoxLayout(centerWidget);
        centerLayout->setAlignment(Qt::AlignCenter); // Center contents

        qrCodeLabel = new QLabel();
        qrCodeLabel->setAlignment(Qt::AlignCenter);
        centerLayout->addWidget(qrCodeLabel);

        QLabel *instructionLabel = new QLabel("Scannez ce QR code pour voir les détails");
        instructionLabel->setAlignment(Qt::AlignCenter);
        QFont font = instructionLabel->font();
        font.setPointSize(12);
        instructionLabel->setFont(font);
        centerLayout->addWidget(instructionLabel);

        mainLayout->addWidget(centerWidget);
        mainLayout->setStretch(1, 1); // Center section: take all the remaining space
    }

    // Get tool data
    QMap<QString, QVariant> toolData = toolsManager->getToolById(toolId);
    if (toolData.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Outil non trouvé");
        return;
    }

    // Create JSON string with tool info
    QJsonObject toolJson;
    toolJson["id"] = toolData["id"].toInt();
    toolJson["nom"] = toolData["nomMateriel"].toString();
    toolJson["categorie"] = toolData["categorie"].toString();
    toolJson["stock"] = toolData["stock"].toInt();
    toolJson["stock max"] = toolData["quantiteMaximale"].toInt();
    toolJson["fournisseur"] = toolData["fournisseur"].toString();
    toolJson["description"] = toolData["description"].toString();
    QJsonDocument doc(toolJson);
    QString jsonString = doc.toJson(QJsonDocument::Compact);

    // Generate and display QR code
    QImage qrImage = generateQRCode(jsonString, 15, 4);
    qrCodeLabel->setPixmap(QPixmap::fromImage(qrImage).scaled(300, 300, Qt::KeepAspectRatio));
    stackedWidget->setCurrentWidget(qrCodePage);
}


// Navigation Functions
void MainWindow::showPatientsPage() {
    stackedWidget->setCurrentWidget(patientsPage);
    updateSidebarIcons(btnPatients);
}

void MainWindow::showPersonelPage() {
    stackedWidget->setCurrentWidget(personelPage);
    updateSidebarIcons(btnPersonel);
}

void MainWindow::showResearchPage() {
    stackedWidget->setCurrentWidget(researchesPage);
    updateSidebarIcons(btnResearches);
}

void MainWindow::showResearchersPage() {
    stackedWidget->setCurrentWidget(researchersPage);
    updateSidebarIcons(btnResearchers);
}

void MainWindow::showToolsPage() {
    stackedWidget->setCurrentWidget(toolsPage);
    updateSidebarIcons(btnTools);
    calculateStatistics();  // Refresh data when coming back
    updateStatistics();
}

void MainWindow::showVaccinsPage() {
    stackedWidget->setCurrentWidget(vaccinsPage);
    updateSidebarIcons(btnVaccins);
}

void MainWindow::showSettingsPage() {
    stackedWidget->setCurrentWidget(settingsPage);
    updateSidebarIcons(btnSettings);
}

void MainWindow::showToolsTablePage() {
    stackedWidget->setCurrentWidget(toolsTablePage);
}
// In mainwindow.cpp

void MainWindow::setupVaccinsPage() {
    QVBoxLayout *vaccinsLayout = new QVBoxLayout(vaccinsPage);

    // Add a button to navigate to the vaccins table page
    QPushButton *goToVaccinsTableButton = new QPushButton("Voir Vaccins", this);
    goToVaccinsTableButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #198754;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #157347;"
        "}"
        );
    vaccinsLayout->addWidget(goToVaccinsTableButton, 0, Qt::AlignCenter);

    // Connect the button to the slot
    connect(goToVaccinsTableButton, &QPushButton::clicked, this, &MainWindow::showVaccinsTablePage);
}

void MainWindow::setupVaccinsTablePage() {
    QVBoxLayout *vaccinsTableLayout = new QVBoxLayout(vaccinsTablePage);

    // Add buttons for CRUD operations
    QPushButton *addVaccinButton = new QPushButton("Ajouter Vaccin", this);
    QPushButton *editVaccinButton = new QPushButton("Modifier Vaccin", this);
    QPushButton *deleteVaccinButton = new QPushButton("Supprimer Vaccin", this);

    // Style the buttons
    QString buttonStyle =
        "QPushButton {"
        "    background-color: #198754;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #157347;"
        "}";
    addVaccinButton->setStyleSheet(buttonStyle);
    editVaccinButton->setStyleSheet(buttonStyle);
    deleteVaccinButton->setStyleSheet(buttonStyle);

    // Add a back button
    QPushButton *backButton = new QPushButton(this);
    backButton->setIcon(QIcon(":/icons/svg/back.svg"));
    backButton->setIconSize(QSize(24, 24));
    backButton->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    border: none;"
        "    padding: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(0, 0, 0, 0.1);"
        "    border-radius: 4px;"
        "}"
        );

    // Layout for buttons
    QHBoxLayout *topButtonLayout = new QHBoxLayout();
    topButtonLayout->addWidget(backButton);
    topButtonLayout->addStretch();
    topButtonLayout->addWidget(addVaccinButton);
    topButtonLayout->addWidget(editVaccinButton);
    topButtonLayout->addWidget(deleteVaccinButton);
    vaccinsTableLayout->addLayout(topButtonLayout);

    // Create the vaccins table
    QTableWidget *vaccinsTable = new QTableWidget(this);
    vaccinsTableLayout->addWidget(vaccinsTable);

    // Initialize VaccinManager
    vaccinManager = new VaccinManager(vaccinsTable, this);

    // Connect buttons to slots
    connect(addVaccinButton, &QPushButton::clicked, this, &MainWindow::onAddVaccinClicked);
    connect(editVaccinButton, &QPushButton::clicked, this, &MainWindow::onEditVaccinClicked);
    connect(deleteVaccinButton, &QPushButton::clicked, this, &MainWindow::onDeleteVaccinClicked);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showVaccinsPage);
}

void MainWindow::setupAddVaccinFormPage() {
    QVBoxLayout *formLayout = new QVBoxLayout(addVaccinFormPage);

    // Create input fields
    QLineEdit *nomVaccinInput = new QLineEdit();
    QLineEdit *referenceInput = new QLineEdit();
    QLineEdit *typeInput = new QLineEdit();
    QLineEdit *maladieChronicInput = new QLineEdit();
    QSpinBox *nbDoseInput = new QSpinBox();
    nbDoseInput->setMinimum(1);
    QSpinBox *quantiteInput = new QSpinBox();
    quantiteInput->setMinimum(0);
    QDateEdit *dateExpInput = new QDateEdit(QDate::currentDate().addYears(1));
    dateExpInput->setCalendarPopup(true);
    dateExpInput->setDisplayFormat("dd/MM/yyyy");

    // Add fields to the form
    QFormLayout *inputLayout = new QFormLayout();
    inputLayout->addRow("Nom Vaccin:", nomVaccinInput);
    inputLayout->addRow("Référence:", referenceInput);
    inputLayout->addRow("Type:", typeInput);
    inputLayout->addRow("Maladie Chronique:", maladieChronicInput);
    inputLayout->addRow("Nombre de Doses:", nbDoseInput);
    inputLayout->addRow("Quantité:", quantiteInput);
    inputLayout->addRow("Date d'Expiration:", dateExpInput);

    // Add a submit button
    QPushButton *submitButton = new QPushButton("Ajouter");
    QPushButton *backButton = new QPushButton("Retour");

    // Add buttons to a horizontal layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(submitButton);

    // Add input layout and buttons to the main layout
    formLayout->addLayout(inputLayout);
    formLayout->addLayout(buttonLayout);

    // Style the form
    addVaccinFormPage->setStyleSheet(
        "QLineEdit, QSpinBox, QDateEdit {"
        "    padding: 8px;"
        "    border: 1px solid #ddd;"
        "    border-radius: 4px;"
        "}"
        "QPushButton {"
        "    background-color: #198754;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #157347;"
        "}"
        );

    // Connect the submit button
    connect(submitButton, &QPushButton::clicked, this, [this, nomVaccinInput, referenceInput, typeInput, maladieChronicInput, nbDoseInput, quantiteInput, dateExpInput]() {
        // Get input values
        QString nomVaccin = nomVaccinInput->text();
        QString reference = referenceInput->text();
        QString type = typeInput->text();
        QString maladieChronique = maladieChronicInput->text();
        int nbDose = nbDoseInput->value();
        int quantite = quantiteInput->value();
        QDate dateExp = dateExpInput->date();

        // Set values in VaccinManager
        vaccinManager->setNomVaccin(nomVaccin);
        vaccinManager->setReference(reference);
        vaccinManager->setType(type);
        vaccinManager->setMaladieChronique(maladieChronique);
        vaccinManager->setNbDose(nbDose);
        vaccinManager->setQuantite(quantite);
        vaccinManager->setDateExp(dateExp);

        // Add the vaccine to the database
        if (vaccinManager->addVaccin()) {
            // Switch back to the table page
            stackedWidget->setCurrentWidget(vaccinsTablePage);
        }
    });

    // Connect the back button
    connect(backButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(vaccinsTablePage);
    });
}

void MainWindow::setupEditVaccinFormPage() {
    // Similar to setupAddVaccinFormPage, but for editing
    // You can copy the code from setupAddVaccinFormPage and modify it for editing
}

void MainWindow::showVaccinsTablePage() {
    stackedWidget->setCurrentWidget(vaccinsTablePage);
}

void MainWindow::onAddVaccinClicked() {
    stackedWidget->setCurrentWidget(addVaccinFormPage);
}

void MainWindow::onEditVaccinClicked() {
    // Implement logic for editing a vaccine
}

void MainWindow::onDeleteVaccinClicked() {
    // Implement logic for deleting a vaccine
}
void MainWindow::updateSidebarIcons(QPushButton *selectedButton) {
    btnPatients->setIcon(QIcon(":/icons/svg/patient.svg"));
    btnPersonel->setIcon(QIcon(":/icons/svg/personel.svg"));
    btnResearches->setIcon(QIcon(":/icons/svg/flask.svg"));
    btnResearchers->setIcon(QIcon(":/icons/svg/scientist.svg"));
    btnTools->setIcon(QIcon(":/icons/svg/microscope.svg"));
    btnVaccins->setIcon(QIcon(":/icons/svg/syringe.svg"));
    btnSettings->setIcon(QIcon(":/icons/svg/settings.svg"));

    if (selectedButton == btnPatients)
        btnPatients->setIcon(QIcon(":/icons/svg/patient-selected.svg"));
    else if (selectedButton == btnPersonel)
        btnPersonel->setIcon(QIcon(":/icons/svg/personel-selected.svg"));
    else if (selectedButton == btnResearches)
        btnResearches->setIcon(QIcon(":/icons/svg/flask-selected.svg"));
    else if (selectedButton == btnResearchers)
        btnResearchers->setIcon(QIcon(":/icons/svg/scientist-selected.svg"));
    else if (selectedButton == btnTools)
        btnTools->setIcon(QIcon(":/icons/svg/microscope-selected.svg"));
    else if (selectedButton == btnVaccins)
        btnVaccins->setIcon(QIcon(":/icons/svg/syringe-selected.svg"));
    else if (selectedButton == btnSettings)
        btnSettings->setIcon(QIcon(":/icons/svg/settings-selected.svg"));
}
