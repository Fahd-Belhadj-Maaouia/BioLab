#include "mainwindow.h"
#include "toolsmanager.h"
#include "researchermanager.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QComboBox>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>

#include <QtCharts>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>

#include "connection.h"

#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    connection c;
    c.createconnection();

    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    // Central Widget and Main Layout
    centralWidget = new QWidget(this);
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
    sidebarWidget->setStyleSheet("background-color: #1a1a1a; border-radius: 15px;");

    sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(10, 10, 10, 10);
    sidebarLayout->setSpacing(8);

    // Logo
    QLabel *logoLabel = new QLabel(sidebarWidget);
    QPixmap logoPixmap(":/icons/svg/BIOLAB.svg");
    logoLabel->setPixmap(logoPixmap.scaledToWidth(100, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignLeft);
    logoLabel->setStyleSheet("margin-left: 10px; margin-top: 5px;");
    sidebarLayout->addWidget(logoLabel);

    // Sidebar Buttons
    btnPatients     = createSidebarButton("  Patients",    ":/icons/svg/patient.svg");
    btnPersonel     = createSidebarButton("  Personel",    ":/icons/svg/personel.svg");
    btnResearches   = createSidebarButton("  Recherche",   ":/icons/svg/flask.svg");
    btnResearchers  = createSidebarButton("  Chercheurs",  ":/icons/svg/scientist.svg");
    btnTools        = createSidebarButton("  Matériels",   ":/icons/svg/microscope.svg");
    btnVaccins      = createSidebarButton("  Vaccin",      ":/icons/svg/syringe.svg");
    btnSettings     = createSidebarButton("  Paramètres",  ":/icons/svg/settings.svg");

    // Grouping buttons
    sidebarButtonGroup = new QButtonGroup(this);
    QList<QPushButton*> buttons = { btnPatients, btnPersonel, btnResearches, btnResearchers, btnTools, btnVaccins, btnSettings };
    for (auto btn : buttons) {
        sidebarButtonGroup->addButton(btn);
        btn->setCheckable(true); // Enable toggle effect
        sidebarLayout->addWidget(btn);
    }
    sidebarButtonGroup->setExclusive(true);

    // Spacer and bottom icon
    sidebarLayout->addStretch();

    QLabel *PicLabel = new QLabel(sidebarWidget);
    QPixmap PicPixmap(":/icons/svg/Asset 2.svg");
    PicLabel->setPixmap(PicPixmap.scaled(22, 22, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    PicLabel->setStyleSheet("margin-left: 10px; margin-bottom: 10px;");
    sidebarLayout->addWidget(PicLabel);

    // Connect Signals
    connect(btnPatients,     &QPushButton::clicked, this, &MainWindow::showPatientsPage);
    connect(btnPersonel,     &QPushButton::clicked, this, &MainWindow::showPersonelPage);
    connect(btnResearches,   &QPushButton::clicked, this, &MainWindow::showResearchPage);
    connect(btnResearchers,  &QPushButton::clicked, this, &MainWindow::showresearchersPage);
    connect(btnTools,        &QPushButton::clicked, this, &MainWindow::showToolsPage);
    connect(btnVaccins,      &QPushButton::clicked, this, &MainWindow::showVaccinsPage);
    connect(btnSettings,     &QPushButton::clicked, this, &MainWindow::showSettingsPage);
}


QPushButton* MainWindow::createSidebarButton(const QString &text, const QString &iconPath) {
    QPushButton *button = new QPushButton(text, sidebarWidget);
    button->setIcon(QIcon(iconPath));
    button->setIconSize(QSize(24, 24));
    button->setCursor(Qt::PointingHandCursor);
    button->setStyleSheet(
        "QPushButton {"
        "    background: transparent;"
        "    color: white;"
        "    text-align: left;"
        "    padding: 10px 15px;"
        "    font-size: 15px;"
        "    border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.1);"
        "}"
        "QPushButton:checked {"
        "    background-color: #198754;"
        "    font-weight: bold;"
        "}"
        );
    return button;
}


void MainWindow::setupPages() {
    int ID = 1;
    stackedWidget = new QStackedWidget(this);

    // Create Pages
    patientsPage = new QWidget();
    researchersPage = new QWidget();
    settingsPage = new QWidget();
    vaccinsPage = new QWidget();
    personelPage = new QWidget();
    toolsPage = new QWidget();
    researchesPage = new QWidget();
    toolsTablePage = new QWidget();
    addToolFormPage = new QWidget();
    ResearchersTablePage = new QWidget();
    addResearcherFormPage = new QWidget();
    ModifyResearcherFormPage = new QWidget();

    researchersStatsPage = new QWidget();
    
    
    
    setupresearchersPage();
    setupResearchersStatsPage();
    
    
    // Setup Pages
    setupresearchersPage();
    setupResearchersTablePage();
    setupaddResearcherFormPage();
    setupModifyResearcherFormPage(ID);
    
    
    
    setupToolsPage();
    setupToolsTablePage();
    setupAddToolFormPage();


    // Add Pages to Stacked Widget
    stackedWidget->addWidget(patientsPage);
    stackedWidget->addWidget(researchersPage);
    stackedWidget->addWidget(vaccinsPage);
    stackedWidget->addWidget(personelPage);
    stackedWidget->addWidget(researchesPage);
    stackedWidget->addWidget(toolsPage);
    stackedWidget->addWidget(settingsPage);
    stackedWidget->addWidget(toolsTablePage);
    stackedWidget->addWidget(addToolFormPage);
    stackedWidget->addWidget(ResearchersTablePage);
    stackedWidget->addWidget(addResearcherFormPage);
    stackedWidget->addWidget(ModifyResearcherFormPage);

    stackedWidget->addWidget(researchersStatsPage);


    // Set Default Page
    stackedWidget->setCurrentWidget(patientsPage);
}

void MainWindow::setupresearchersPage() {
    QVBoxLayout *researcherLayout = new QVBoxLayout(researchersPage);

    // Add "Voir Chercheurs" Button
    QPushButton *goToResearchersTableButton = new QPushButton("Voir la table des chercheurs");
    goToResearchersTableButton->setStyleSheet(
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

    // Add "Statistics" Button
    QPushButton *goToStatsButton = new QPushButton("Voir Statistiques");
    goToStatsButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #1a1a1a;"  // ✅ couleur sidebar
        "    color: white;"
        "    padding: 10px 20px;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #333333;"  // légèrement plus clair au survol
        "}"
        );


    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(goToResearchersTableButton, 0, Qt::AlignCenter);
    buttonLayout->addWidget(goToStatsButton, 0, Qt::AlignCenter);

    researcherLayout->addLayout(buttonLayout);


    // Connect Buttons to Slots
    connect(goToResearchersTableButton, &QPushButton::clicked, this, &MainWindow::showResearchersTablePage);
    connect(goToStatsButton, &QPushButton::clicked, this, &MainWindow::showResearchersStatsPage);
}

void MainWindow::setupResearchersStatsPage() {
    QVBoxLayout *layout = new QVBoxLayout(researchersStatsPage);

    // Back Button
    QPushButton *backButton = new QPushButton(this);
    backButton->setIcon(QIcon(":/icons/svg/back.svg"));
    backButton->setIconSize(QSize(24, 24));
    backButton->setStyleSheet("background: transparent;");
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showresearchersPage);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);

    // Chart Setup
    QChartView *chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(600, 400);

    QChart *chart = new QChart();
    chart->setTitle("Researchers Statistics");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QSqlDatabase db = QSqlDatabase::database();
    qDebug() << "Database is open:" << db.isOpen();

    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database is not open.");
        return;
    }

    QSqlQuery query;
    QString sql = "SELECT Specialite, COUNT(*) FROM researcher GROUP BY Specialite";

    if (!query.exec(sql)) {
        qDebug() << "Query failed:" << query.lastError().text();
        QMessageBox::critical(this, "Query Error", query.lastError().text());
        return;
    }

    qDebug() << "Query executed successfully.";


    QBarSet *set = new QBarSet("Researchers by Specialty");
    QStringList categories;


    while (query.next()) {
        QString specialty = query.value(0).toString();
        int count = query.value(1).toInt();
        qDebug() << "Specialty:" << specialty << "| Count:" << count;
        *set << count;
        categories << specialty;
    }

    if (categories.isEmpty()) {
        *set << 1;
        categories << "No Data";
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);
    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chartView->setChart(chart);
    layout->addWidget(chartView);

    // Optional debug styling
    researchersStatsPage->setStyleSheet("background-color: lightgray;");
    chartView->setStyleSheet("background-color: white;");
}

void MainWindow::updateResearchersStatsChart() {
    // Clean old layout
    QLayout *oldLayout = researchersStatsPage->layout();
    if (oldLayout)
        delete oldLayout;

    QVBoxLayout *layout = new QVBoxLayout(researchersStatsPage);

    // Back Button
    QPushButton *backButton = new QPushButton(this);
    backButton->setIcon(QIcon(":/icons/svg/back.svg"));
    backButton->setIconSize(QSize(24, 24));
    backButton->setStyleSheet("background: transparent;");
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showresearchersPage);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);

    // Chart Setup
    QChartView *chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(600, 400);

    QChart *chart = new QChart();
    chart->setTitle("Researchers Statistics");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database is not open.");
        return;
    }

    QSqlQuery query;
    if (!query.exec("SELECT Specialite, COUNT(*) FROM researcher GROUP BY Specialite")) {
        QMessageBox::critical(this, "Query Error", query.lastError().text());
        return;
    }

    QBarSet *set = new QBarSet("Researchers by Specialty");
    QStringList categories;

    while (query.next()) {
        QString specialty = query.value(0).toString();
        int count = query.value(1).toInt();
        *set << count;
        categories << specialty;
    }

    if (categories.isEmpty()) {
        *set << 1;
        categories << "No Data";
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);
    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chartView->setChart(chart);
    layout->addWidget(chartView);
}


void MainWindow::showResearchersStatsPage() {
    updateResearchersStatsChart();
    stackedWidget->setCurrentWidget(researchersStatsPage);
    updateSidebarIcons(btnResearchers);
}


void MainWindow::setupResearchersTablePage() {
    QVBoxLayout *layout = new QVBoxLayout(ResearchersTablePage);



    // Add "Ajouter Materiels" Button
    QPushButton *addResearcherButton = new QPushButton("Ajouter Chercheurs", this);
    addResearcherButton->setStyleSheet(
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
    
    // Add "Modifier Materiels" Button
    QPushButton *editResearcherButton = new QPushButton("Modifier Chercheurs", this);
    editResearcherButton->setStyleSheet(
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
    
    // Add "Supprimer Materiels" Button
    QPushButton *deleteResearcherButton = new QPushButton("Supprimer Chercheurs", this);
    deleteResearcherButton->setStyleSheet(
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
    
    // Add Back Button
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
    
    // Top Button Layout
    ResearchersTablePage->setLayout(layout);
    
    QHBoxLayout *topButtonLayout = new QHBoxLayout();
    topButtonLayout->addWidget(backButton);
    topButtonLayout->addStretch();
    topButtonLayout->addWidget(addResearcherButton);
    topButtonLayout->addWidget(editResearcherButton);
    topButtonLayout->addWidget(deleteResearcherButton);


    // Add Export PDF Button
    QPushButton *exportPDFButton = new QPushButton("📄 Exporter PDF", this);
    exportPDFButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #1a1a1a;"  // ✅ couleur sidebar
        "    color: white;"
        "    padding: 10px 20px;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #333333;"  // survol
        "}"
        );


    topButtonLayout->addWidget(exportPDFButton);

    // Connect to export function
    connect(exportPDFButton, &QPushButton::clicked, this, &MainWindow::exportResearchersTableToPDF);


    
    layout->addLayout(topButtonLayout);
    
    // Set layout for researchertablePage
    
    //layout->addStretch(1);
    
    
    
    
    
    QTableWidget *researcherstable = new QTableWidget(this);
    researcherstable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    researcherstable->horizontalHeader()->setVisible(true);
    researcherstable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    researcherstable->verticalHeader()->setVisible(false); // Optional: hides row numbers
    researcherstable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "background-color: white;"        // Optional: make header background white
        "color: black;"                   // ✅ Make header text black
        "font-weight: bold;"              // Optional: make it bold
        "padding: 5px;"
        "border: 1px solid lightgray;"
        "}"
        );

    researcherstable->setColumnCount(10);
    researcherstable->setHorizontalHeaderLabels({"IDR", "Nom", "Prenom", "Sexe", "CIN", "Adresse", "NumTel", "Specialite", "Datedenaissance", "Email"});
    layout->addWidget(researcherstable);

    // Now set the layout of the PatientsTablePage (no need for an extra QWidget)
    ResearchersTablePage->setLayout(layout);
    // Add search widget
    setupSearchWidget();

    
    // Set thepatientsManager->loadPatients(); PatientsTablePage as the central widget of the MainWindow
    
    // Initialize PatientsManager with the table
    researchersManager = new class ResearcherManager(researcherstable, this);
    
    
    
    
    
    
    
    
    
    
    // Connect Buttons to Slots
    connect(addResearcherButton, &QPushButton::clicked, this, &MainWindow::onAddResearcherClicked);
    connect(editResearcherButton, &QPushButton::clicked, this, &MainWindow::onEditResearcherClicked);
    connect(deleteResearcherButton, &QPushButton::clicked, this, &MainWindow::onDeleteResearcherClicked);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showresearchersPage);
    
}

void MainWindow::exportResearchersTableToPDF() {
    QString filePath = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF files (*.pdf)");
    if (filePath.isEmpty())
        return;

    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize::A4);
    writer.setResolution(300);

    QPainter painter(&writer);

    // Setup fonts
    QFont titleFont("Arial", 16, QFont::Bold);
    QFont headerFont("Arial", 10, QFont::Bold);
    QFont cellFont("Arial", 9);

    int margin = 50;
    int x = margin;
    int y = margin;

    QTableWidget *table = researchersManager->getTableWidget();
    if (!table) {
        QMessageBox::warning(this, "Error", "Table not found.");
        return;
    }

    // Title
    painter.setFont(titleFont);
    painter.drawText(x, y, "Liste des Chercheurs");
    y += 40;

    // Setup for table rendering
    painter.setFont(headerFont);
    int rowHeight = 45;
    int colCount = table->columnCount();
    int tableWidth = writer.width() - 2 * margin;
    int colWidth = tableWidth / colCount;

    // Draw headers with background
    painter.fillRect(x, y, tableWidth, rowHeight, QColor("#007bff"));
    painter.setPen(Qt::white);
    for (int col = 0; col < colCount; ++col) {
        painter.drawText(x + col * colWidth + 5, y + 28, table->horizontalHeaderItem(col)->text());
    }

    // Grid lines for header
    painter.setPen(Qt::black);
    for (int col = 0; col <= colCount; ++col) {
        painter.drawLine(x + col * colWidth, y, x + col * colWidth, y + rowHeight);
    }
    painter.drawLine(x, y, x + tableWidth, y);
    painter.drawLine(x, y + rowHeight, x + tableWidth, y + rowHeight);

    y += rowHeight;

    // Table body
    painter.setFont(cellFont);
    for (int row = 0; row < table->rowCount(); ++row) {
        for (int col = 0; col < colCount; ++col) {
            QTableWidgetItem *item = table->item(row, col);
            if (item)
                painter.drawText(x + col * colWidth + 5, y + 28, item->text());
        }

        // Grid lines
        for (int col = 0; col <= colCount; ++col) {
            painter.drawLine(x + col * colWidth, y, x + col * colWidth, y + rowHeight);
        }
        painter.drawLine(x, y + rowHeight, x + tableWidth, y + rowHeight);

        y += rowHeight;

        // Page break
        if (y > writer.height() - margin - rowHeight) {
            writer.newPage();
            y = margin;
        }
    }

    painter.end();
    QMessageBox::information(this, "Exporté", "Le PDF a été généré avec succès !");
}



void MainWindow::setupSearchWidget() {
    QWidget *searchWidget = new QWidget();
    QHBoxLayout *searchLayout = new QHBoxLayout(searchWidget);

    QComboBox *searchTypeCombo = new QComboBox();
    searchTypeCombo->addItem("🔍 Choisir un type de recherche");
    searchTypeCombo->addItem("Recherche par Nom");
    searchTypeCombo->addItem("Recherche par CIN");
    searchTypeCombo->addItem("Recherche par Spécialité");
    searchTypeCombo->setCurrentIndex(0);
    searchTypeCombo->setStyleSheet(
        "QComboBox {"
        "    padding: 6px 12px;"
        "    font-size: 14px;"
        "    color: black;"                   // 👈 combo box text color
        "    border: 1px solid #ccc;"
        "    border-radius: 6px;"
        "    background-color: white;"
        "}"
        "QComboBox QAbstractItemView {"
        "    background-color: white;"
        "    color: black;"                  // 👈 dropdown text color
        "    selection-background-color: #007bff;"
        "    selection-color: white;"
        "    font-size: 14px;"
        "    padding: 8px;"
        "}"
        );




    QLineEdit *searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Rechercher...");

    QPushButton *searchButton = new QPushButton("Chercher");
    QPushButton *resetButton = new QPushButton("Réinitialiser");

    QComboBox *alphabetSortCombo = new QComboBox();
    alphabetSortCombo->addItem("🔤 Filtrage par nom");
    alphabetSortCombo->addItem("A → Z");
    alphabetSortCombo->addItem("Z → A");

    QComboBox *ageFilterCombo = new QComboBox();
    ageFilterCombo->addItem("🎂 Filtrage par age");
    ageFilterCombo->addItem("Age < 25");
    ageFilterCombo->addItem("Age 25 - 35");
    ageFilterCombo->addItem("Age > 35");

    QComboBox *specialiteSortCombo = new QComboBox();
    specialiteSortCombo->addItem("🔬 Filtrage par spécialité");
    specialiteSortCombo->addItem("A → Z");
    specialiteSortCombo->addItem("Z → A");



    searchLayout->addWidget(searchTypeCombo);
    searchLayout->addWidget(searchInput);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(resetButton);
    searchLayout->addWidget(alphabetSortCombo);
    searchLayout->addWidget(ageFilterCombo);
    searchLayout->addWidget(specialiteSortCombo);  // 👈 Add this line


    // Add search widget to the main layout
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(ResearchersTablePage->layout());
    if (mainLayout) {
        mainLayout->insertWidget(1, searchWidget); // Insert after the top button layout
    }

    searchWidget->setStyleSheet(
        "QComboBox, QLineEdit {"
        "    padding: 10px 14px;"
        "    border: 1px solid #ccc;"
        "    border-radius: 6px;"
        "    font-size: 15px;"
        "    color: #2c3e50;"
        "    background-color: #fdfdfd;"
        "}"
        "QComboBox::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 30px;"
        "    border-left: 1px solid #ccc;"
        "}"
        "QComboBox::down-arrow {"
        "    image: url(:/icons/down-arrow.png);" // optionnel: icône personnalisée
        "    width: 12px;"
        "    height: 12px;"
        "}"
        "QComboBox QAbstractItemView {"
        "    background-color: white;"
        "    border: 1px solid #ddd;"
        "    selection-background-color: #198754;"
        "    selection-color: white;"
        "    font-size: 14px;"
        "}"
        "QPushButton {"
        "    background-color: #198754;"
        "    color: white;"
        "    padding: 10px 18px;"
        "    border-radius: 6px;"
        "    font-size: 15px;"
        "    font-weight: bold;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: #157347;"
        "}"
        );

    specialiteSortCombo->setStyleSheet(searchTypeCombo->styleSheet());

    alphabetSortCombo->setStyleSheet(searchTypeCombo->styleSheet());
    ageFilterCombo->setStyleSheet(searchTypeCombo->styleSheet());




    connect(searchButton, &QPushButton::clicked, this, [this, searchTypeCombo, searchInput]() {
        QString searchTerm = searchInput->text().trimmed();
        if (searchTerm.isEmpty()) {
            researchersManager->loadResearcher();
            return;
        }

        int selectedIndex = searchTypeCombo->currentIndex();
        if (selectedIndex == 1) {
            researchersManager->searchResearcherByName(searchTerm);
        } else if (selectedIndex == 2) {
            researchersManager->searchResearcherByCIN(searchTerm);
        } else if (selectedIndex == 3) {
            researchersManager->searchResearcherBySpecialite(searchTerm);
        }
    });

    connect(resetButton, &QPushButton::clicked, this, [this, searchInput]() {
        searchInput->clear();
        researchersManager->loadResearcher();
    });
    connect(alphabetSortCombo, &QComboBox::currentTextChanged, this, [this](const QString &selected) {
        if (selected == "A → Z") {
            researchersManager->sortResearchersByName(true);
        } else if (selected == "Z → A") {
            researchersManager->sortResearchersByName(false);
        } else {
            researchersManager->loadResearcher(); // default or no sorting
        }
    });

    connect(specialiteSortCombo, &QComboBox::currentTextChanged, this, [this](const QString &selected) {
        if (selected == "A → Z") {
            researchersManager->sortResearchersBySpecialite(true);
        } else if (selected == "Z → A") {
            researchersManager->sortResearchersBySpecialite(false);
        } else {
            researchersManager->loadResearcher(); // default reset
        }
    });


    connect(ageFilterCombo, &QComboBox::currentTextChanged, this, [this, ageFilterCombo]() {
        QString selected = ageFilterCombo->currentText();
        if (selected == "🎂 Filter by Age") {
            researchersManager->loadResearcher();
        } else if (selected == "Age < 25") {
            researchersManager->filterByAgeRange(0, 24);
        } else if (selected == "Age 25 - 35") {
            researchersManager->filterByAgeRange(25, 35);
        } else if (selected == "Age > 35") {
            researchersManager->filterByAgeRange(36, 150);
        }
    });

}


void MainWindow::setupaddResearcherFormPage() {
    addResearcherFormPage = new QWidget();
    QVBoxLayout *formLayout = new QVBoxLayout(addResearcherFormPage);

    QLabel *titleLabel = new QLabel("Ajout d'un Chercheur");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
    formLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    QLabel *nomLabel = new QLabel("Nom:");
    QLineEdit *nomInput = new QLineEdit();
    QLabel *prenomLabel = new QLabel("Prénom:");
    QLineEdit *prenomInput = new QLineEdit();
    QLabel *sexeLabel = new QLabel("Sexe:");
    QComboBox *sexeInput = new QComboBox();
    sexeInput->addItem("Homme");  // Male
    sexeInput->addItem("Femme");  // Female
    QLabel *cinLabel = new QLabel("CIN:");
    QLineEdit *cinInput = new QLineEdit();
    QLabel *adresseLabel = new QLabel("Gouvernorat:");
    QComboBox *adresseInput = new QComboBox();
    // Add all the governorates of Tunisia to the dropdown
    adresseInput->addItems({
        "Ariana", "Beja", "Ben Arous", "Bizerte", "Gabes", "Gafsa", "Jendouba", "Kairouan",
        "Kasserine", "Kebili", "Kef", "Mahdia", "Manouba", "Medenine", "Monastir", "Nabeul",
        "Sfax", "Sidi Bouzid", "Siliana", "Tataouine", "Tozeur", "Tunis", "Zaghouan"
    });
    sexeInput->setCurrentIndex(0);
    adresseInput->setCurrentIndex(0);


    QLabel *numtelLabel = new QLabel("Numéro de Téléphone:");
    QLineEdit *numtelInput = new QLineEdit();
    QLabel *specialiteLabel = new QLabel("Spécialité:");
    QLineEdit *specialiteInput = new QLineEdit();
    QLabel *datedenaissanceLabel = new QLabel("Date de naissance:");
    QLineEdit *datedenaissanceInput = new QLineEdit();
    QLabel *emailLabel = new QLabel("Email:");
    QLineEdit *emailInput = new QLineEdit();

    QFormLayout *inputLayout = new QFormLayout();
    inputLayout->addRow(nomLabel, nomInput);
    inputLayout->addRow(prenomLabel, prenomInput);
    inputLayout->addRow(sexeLabel, sexeInput);
    inputLayout->addRow(cinLabel, cinInput);
    inputLayout->addRow(adresseLabel, adresseInput);
    inputLayout->addRow(numtelLabel, numtelInput);
    inputLayout->addRow(specialiteLabel, specialiteInput);
    inputLayout->addRow(datedenaissanceLabel, datedenaissanceInput);
    inputLayout->addRow(emailLabel, emailInput);

    formLayout->addLayout(inputLayout);

    QPushButton *submitButton = new QPushButton("Ajouter");
    QPushButton *backButton = new QPushButton("Retour");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(submitButton);
    formLayout->addLayout(buttonLayout);

    addResearcherFormPage->setStyleSheet(
        "QLabel {"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    margin-top: 5px;"
        "    color: black"
        "}"
        "QLineEdit {"
        "    padding: 8px;"
        "    border: 1px solid #ddd;"
        "    border-radius: 4px;"
        "    margin-bottom: 5px;"
        "    color: black;"

        "}"
        "QComboBox {"
        "    padding: 8px;"
        "    border: 1px solid #ddd;"  // Border color same as QLineEdit
        "    border-radius: 4px;"  // Rounded corners for a modern look
        "    margin-bottom: 5px;"
        "    color: black;"  // Black text color
        "    font-size: 14px;"  // Consistent font size
        "    font-family: Arial;"  // Consistent font family
        "    transition: border-color 0.3s ease, box-shadow 0.3s ease;"  // Smooth transitions
        "}"
        "QComboBox:hover {"
        "    border-color: #198754;"  // Green border on hover for a modern feel
        "    box-shadow: 0 0 5px rgba(0, 155, 74, 0.3);"
        "}"
        "QComboBox::drop-down {"
        "    border: 1px solid #ddd;"
        "    background-color: #f7f7f7;"  // Lighter gray background for the dropdown
        "    border-top-right-radius: 4px;"
        "    border-top-left-radius: 4px;"
        "}"
        "QComboBox::down-arrow {"
        "    border-radius: 3px;"
        "    width: 15px;"
        "    height: 15px;"
        "    image: url(:/images/down-arrow.svg);"
        "}"
        "QComboBox::down-arrow:hover {"
        "    background-color: #198754;"  // Green color on hover for the arrow
        "    border-radius: 3px;"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 1px solid #ddd;"
        "    border-radius: 4px;"
        "    selection-background-color: #198754;"  // Green background for selected item
        "    selection-color: white;"  // White text color for selected item // White background for the list
        "    padding: 5px 0;"  // Padding around the list
        "}"
        "QComboBox QAbstractItemView::item {"
        "    padding: 10px 15px;"
        "    font-size: 14px;"
        "    color: black;"  // Default text color
        "}"
        "QComboBox QAbstractItemView::item:hover {"
        "    background-color: #f1f1f1;"  // Slight gray background on hover for items
        "    color: #198754;"  // Green text color on hover
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

    connect(submitButton, &QPushButton::clicked, this, [this, nomInput, prenomInput, sexeInput, cinInput, adresseInput, numtelInput, specialiteInput, datedenaissanceInput, emailInput]() {
        if (!researchersManager) {
            qDebug() << "Error: researchersManager is not initialized!";
            return;
        }

        // Debugging - Print Input Values
        qDebug() << "Nom: " << nomInput->text();
        qDebug() << "Prenom: " << prenomInput->text();
        qDebug() << "Sexe: " << sexeInput->currentText();
        qDebug() << "CIN: " << cinInput->text();
        qDebug() << "Adresse: " << adresseInput->currentText();
        qDebug() << "NumTel: " << numtelInput->text();
        qDebug() << " Specialite: " << specialiteInput->text();
        qDebug() << " Datedenaissance: " << datedenaissanceInput->text();
        qDebug() << " Email: " << emailInput->text();


        // Debug which field is empty
        if (nomInput->text().trimmed().isEmpty()) qDebug() << "Nom is empty!";
        if (prenomInput->text().trimmed().isEmpty()) qDebug() << "Prenom is empty!";
        if (sexeInput->currentText().trimmed().isEmpty()) qDebug() << "Sexe is empty!";
        if (cinInput->text().trimmed().isEmpty()) qDebug() << "CIN is empty!";
        if (adresseInput->currentText().trimmed().isEmpty()) qDebug() << "Adresse is empty!";
        if (numtelInput->text().trimmed().isEmpty()) qDebug() << "NumTel is empty!";



        if (nomInput->text().trimmed().isEmpty() || prenomInput->text().trimmed().isEmpty() ||
            sexeInput->currentText().trimmed().isEmpty() || cinInput->text().trimmed().isEmpty() ||
            adresseInput->currentText().trimmed().isEmpty() || numtelInput->text().trimmed().isEmpty()) {

            QMessageBox::warning(nullptr, "Validation Error", "Please fill all required fields.");
            qDebug() << "Validation Failed: Some fields are empty!";
            return;
        }


        // Insert values into patientsManager
        researchersManager->setNom(nomInput->text());
        researchersManager->setprenom(prenomInput->text());
        researchersManager->setsexe(sexeInput->currentText());
        researchersManager->setcin(cinInput->text());
        researchersManager->setadresse(adresseInput->currentText());
        researchersManager->setnumtel(numtelInput->text());
        researchersManager->setspecialite(specialiteInput->text());
        researchersManager->setdatedenaissance(datedenaissanceInput->text());
        researchersManager->setemail(emailInput->text());


        // Now call addPatient with the correct arguments
        researchersManager->addResearcher(
            researchersManager->getNom(),
            researchersManager->getprenom(),
            researchersManager->getsexe(),
            researchersManager->getcin(),
            researchersManager->getadresse(),
            researchersManager->getnumtel(),
            researchersManager->getspecialite(),
            researchersManager->getdatedenaissance(),
            researchersManager->getemail()
            );
        researchersManager->loadResearcher();

        stackedWidget->setCurrentWidget(ResearchersTablePage);
    });




    connect(backButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(ResearchersTablePage);
    });
}

void MainWindow::setupModifyResearcherFormPage(const int ID) {
    // Clear any existing layout and widgets
    if (ModifyResearcherFormPage->layout()) {
        QLayoutItem* child;
        while ((child = ModifyResearcherFormPage->layout()->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
        delete ModifyResearcherFormPage->layout();
    }

    QVBoxLayout *formLayout = new QVBoxLayout(ModifyResearcherFormPage);

    QLabel *titleLabel = new QLabel("Modifier Chercheur");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
    formLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    // Create form inputs
    QLabel *nomLabel = new QLabel("Nom:");
    QLineEdit *nomInput = new QLineEdit();
    QLabel *prenomLabel = new QLabel("Prénom:");
    QLineEdit *prenomInput = new QLineEdit();
    QLabel *sexeLabel = new QLabel("Sexe:");
    QComboBox *sexeInput = new QComboBox();
    sexeInput->addItem("Homme");
    sexeInput->addItem("Femme");
    QLabel *cinLabel = new QLabel("CIN:");
    QLineEdit *cinInput = new QLineEdit();
    QLabel *adresseLabel = new QLabel("Gouvernorat:");
    QComboBox *adresseInput = new QComboBox();
    adresseInput->addItems({
        "Ariana", "Beja", "Ben Arous", "Bizerte", "Gabes", "Gafsa", "Jendouba", "Kairouan",
        "Kasserine", "Kebili", "Kef", "Mahdia", "Manouba", "Medenine", "Monastir", "Nabeul",
        "Sfax", "Sidi Bouzid", "Siliana", "Tataouine", "Tozeur", "Tunis", "Zaghouan"
    });
    QLabel *numtelLabel = new QLabel("Numéro de Téléphone:");
    QLineEdit *numtelInput = new QLineEdit();
    QLabel *specialiteLabel = new QLabel("Spécialité:");
    QLineEdit *specialiteInput = new QLineEdit();
    QLabel *datedenaissanceLabel = new QLabel("Date de naissance:");
    QLineEdit *datedenaissanceInput = new QLineEdit();
    QLabel *emailLabel = new QLabel("Email:");
    QLineEdit *emailInput = new QLineEdit();

    // Load existing data
    loadResearcherData(ID, nomInput, prenomInput, sexeInput, cinInput, adresseInput,
                       numtelInput, specialiteInput, datedenaissanceInput, emailInput);

    // Add fields to form
    QFormLayout *inputLayout = new QFormLayout();
    inputLayout->addRow(nomLabel, nomInput);
    inputLayout->addRow(prenomLabel, prenomInput);
    inputLayout->addRow(sexeLabel, sexeInput);
    inputLayout->addRow(cinLabel, cinInput);
    inputLayout->addRow(adresseLabel, adresseInput);
    inputLayout->addRow(numtelLabel, numtelInput);
    inputLayout->addRow(specialiteLabel, specialiteInput);
    inputLayout->addRow(datedenaissanceLabel, datedenaissanceInput);
    inputLayout->addRow(emailLabel, emailInput);
    formLayout->addLayout(inputLayout);

    // Add buttons
    QPushButton *submitButton = new QPushButton("Modifier");
    QPushButton *backButton = new QPushButton("Retour");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(submitButton);
    formLayout->addLayout(buttonLayout);

    // Set stylesheet
    ModifyResearcherFormPage->setStyleSheet(
        "QLabel {"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    margin-top: 5px;"
        "    color: black;"
        "}"
        "QLineEdit {"
        "    padding: 8px;"
        "    border: 1px solid #ddd;"
        "    border-radius: 4px;"
        "    margin-bottom: 5px;"
        "    color: black;"
        "}"
        "QComboBox {"
        "    padding: 8px;"
        "    border: 1px solid #ddd;"
        "    border-radius: 4px;"
        "    margin-bottom: 5px;"
        "    color: black;"
        "    font-size: 14px;"
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

    connect(submitButton, &QPushButton::clicked, this, [this, ID, nomInput, prenomInput, sexeInput, cinInput,
                                                        adresseInput, numtelInput, specialiteInput,
                                                        datedenaissanceInput, emailInput]() {
        // Extract text from widgets
        QString nom = nomInput->text();
        QString prenom = prenomInput->text();
        QString sexe = sexeInput->currentText();
        QString cin = cinInput->text();
        QString adresse = adresseInput->currentText();
        QString numtel = numtelInput->text();
        QString specialite = specialiteInput->text();
        QString datedenaissance = datedenaissanceInput->text().split("T")[0]; // Fix: Remove time part
        QString email = emailInput->text();

        // Update the researcher data in the manager first
        researchersManager->setNom(nom);
        researchersManager->setprenom(prenom);
        researchersManager->setsexe(sexe);
        researchersManager->setcin(cin);
        researchersManager->setadresse(adresse);
        researchersManager->setnumtel(numtel);
        researchersManager->setspecialite(specialite);
        researchersManager->setdatedenaissance(datedenaissance);
        researchersManager->setemail(email);

        // Then call update with extracted strings (not widgets!)
        researchersManager->updateResearcher(ID, nom, prenom, sexe, cin, adresse, numtel, specialite, datedenaissance, email);
        researchersManager->loadResearcher();

        // Return to table view
        stackedWidget->setCurrentWidget(ResearchersTablePage);
    });
    // Connect the back button to return to the table page
    connect(backButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(ResearchersTablePage);
    });
}

void MainWindow::loadResearcherData(int researcherID, QLineEdit *nomInput, QLineEdit *prenomInput, QComboBox *sexeInput, QLineEdit *cinInput, QComboBox *adresseInput, QLineEdit *numtelInput, QLineEdit *specialiteInput, QLineEdit *datedenaissanceInput, QLineEdit *emailInput) {
    QSqlQuery query;
    query.prepare("SELECT Nom, Prenom, Sexe, CIN, Adresse, NumTel, Specialite, Datedenaissance, Email FROM researcher WHERE IDR = :IDR");
    query.bindValue(":IDR", researcherID);

    if (query.exec()) {
        if (query.next()) {
            nomInput->setText(query.value("Nom").toString());
            prenomInput->setText(query.value("Prenom").toString());
            sexeInput->setCurrentText(query.value("Sexe").toString());
            cinInput->setText(query.value("CIN").toString());
            adresseInput->setCurrentText(query.value("Adresse").toString());
            numtelInput->setText(query.value("NumTel").toString());
            specialiteInput->setText(query.value("Specialite").toString());
            datedenaissanceInput->setText(query.value("Datedenaissance").toString());
            emailInput->setText(query.value("Email").toString());

        }
    } else {
        qDebug() << "Error loading researcher data:" << query.lastError().text();
    }
}


void MainWindow::onAddResearcherClicked() {
    stackedWidget->setCurrentWidget(addResearcherFormPage);
}

void MainWindow::onEditResearcherClicked() {
    bool ok;
    int researcherID = QInputDialog::getInt(this, "Modifier Chercheur", "Entrez l'ID du chercheur à modifier:", 1, 1, 10000, 1, &ok);

    if (ok) {
        // First check if the researcher exists
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM researcher WHERE IDR = :IDR");
        checkQuery.bindValue(":IDR", researcherID);

        if (!checkQuery.exec() || !checkQuery.next()) {
            QMessageBox::warning(this, "Erreur", "Impossible de vérifier l'existence du chercheur.");
            return;
        }

        if (checkQuery.value(0).toInt() == 0) {
            QMessageBox::warning(this, "Non trouvé", "Aucun chercheur trouvé avec cet ID.");
            return;
        }

        // Create or reuse the modify form
        if (ModifyResearcherFormPage == nullptr) {
            ModifyResearcherFormPage = new QWidget();
            stackedWidget->addWidget(ModifyResearcherFormPage);
        }

        // Setup the form with the researcher's data
        setupModifyResearcherFormPage(researcherID);

        // Switch to the modify page
        stackedWidget->setCurrentWidget(ModifyResearcherFormPage);
    }
}

void MainWindow::onDeleteResearcherClicked() {
    bool ok;
    int id = QInputDialog::getInt(this, "Supprimer Chercheur", "Entrez l'ID du chercheur à supprimer:", 1, 1, 10000, 1, &ok);
    if (ok) {
        researchersManager->deleteResearcher(id);
    }
}





void MainWindow::onModifyResearcherSubmit(int researcherID, QLineEdit *nomInput, QLineEdit *prenomInput, QComboBox *sexeInput, QLineEdit *cinInput, QComboBox *adresseInput, QLineEdit *numtelInput, QLineEdit *specialiteInput, QLineEdit *datedenaissanceInput, QLineEdit *emailInput)
{
    // Extract text from widgets
    QString nom = nomInput->text();
    QString prenom = prenomInput->text();
    QString sexe = sexeInput->currentText();  // For QComboBox, use currentText()
    QString cin = cinInput->text();
    QString adresse = adresseInput->currentText();
    QString numtel = numtelInput->text();
    QString specialite = specialiteInput->text();
    QString datedenaissance = datedenaissanceInput->text();
    QString email = emailInput->text();

    // Validate required fields
    if (nom.isEmpty() || prenom.isEmpty() || cin.isEmpty() || numtel.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please fill in all required fields.");
        return;
    }

    // Pass extracted strings instead of widgets
    researchersManager->updateResearcher(
        researcherID,
        nom,
        prenom,
        sexe,
        cin,
        adresse,
        numtel,
        specialite,
        datedenaissance,
        email
        );

    QMessageBox::information(this, "Success", "Researcher information updated successfully!");
}




void MainWindow::setupToolsPage() {
    QVBoxLayout *toolsLayout = new QVBoxLayout(toolsPage);

    // Add "Voir Matériels" Button
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
    toolsLayout->addWidget(goToToolsTableButton, 0, Qt::AlignCenter);

    // Connect Button to Slot
    connect(goToToolsTableButton, &QPushButton::clicked, this, &MainWindow::showToolsTablePage);
}

void MainWindow::setupToolsTablePage() {
    QVBoxLayout *toolsTableLayout = new QVBoxLayout(toolsTablePage);

    // Add "Ajouter Materiels" Button
    QPushButton *addMaterialButton = new QPushButton("Ajouter Materiels", this);
    addMaterialButton->setStyleSheet(
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

    // Add "Modifier Materiels" Button
    QPushButton *editMaterialButton = new QPushButton("Modifier Materiels", this);
    editMaterialButton->setStyleSheet(
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

    // Add "Supprimer Materiels" Button
    QPushButton *deleteMaterialButton = new QPushButton("Supprimer Materiels", this);
    deleteMaterialButton->setStyleSheet(
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

    // Add Back Button
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

    // Top Button Layout
    QHBoxLayout *topButtonLayout = new QHBoxLayout();
    topButtonLayout->addWidget(backButton);
    topButtonLayout->addStretch();
    topButtonLayout->addWidget(addMaterialButton);
    topButtonLayout->addWidget(editMaterialButton);
    topButtonLayout->addWidget(deleteMaterialButton);
    toolsTableLayout->addLayout(topButtonLayout);

    // Create Tools Table
    QTableWidget *toolstable = new QTableWidget(this);
    toolsTableLayout->addWidget(toolstable);

    // Initialize ToolsManager
    toolsManager = new ToolsManager(toolstable, this);

    // Connect Buttons to Slots
    connect(addMaterialButton, &QPushButton::clicked, this, &MainWindow::onAddToolClicked);
    connect(editMaterialButton, &QPushButton::clicked, this, &MainWindow::onEditToolClicked);
    connect(deleteMaterialButton, &QPushButton::clicked, this, &MainWindow::onDeleteToolClicked);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showToolsPage);
}

void MainWindow::setupAddToolFormPage() {
    QVBoxLayout *formLayout = new QVBoxLayout(addToolFormPage);

    // Create input fields
    QLineEdit *nomMaterielInput = new QLineEdit();
    QLineEdit *categorieInput = new QLineEdit();
    QLineEdit *descriptionInput = new QLineEdit();
    QSpinBox *stockInput = new QSpinBox();
    stockInput->setMinimum(0);
    QSpinBox *quantiteMaximaleInput = new QSpinBox();
    quantiteMaximaleInput->setMinimum(0);
    QPushButton *uploadImageButton = new QPushButton("Upload Image");
    QLineEdit *fournisseurInput = new QLineEdit();

    // Add fields to the form
    QFormLayout *inputLayout = new QFormLayout();
    inputLayout->addRow("Nom Materiel:", nomMaterielInput);
    inputLayout->addRow("Catégorie:", categorieInput);
    inputLayout->addRow("Description:", descriptionInput);
    inputLayout->addRow("Stock:", stockInput);
    inputLayout->addRow("Quantité Maximale:", quantiteMaximaleInput);
    inputLayout->addRow("Image:", uploadImageButton);
    inputLayout->addRow("Fournisseur:", fournisseurInput);

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
    addToolFormPage->setStyleSheet(
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
    // Connect the upload button
    // Store the image data
    connect(uploadImageButton, &QPushButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Upload Image", "", "Images (*.png *.jpg *.jpeg)");
        if (!filePath.isEmpty()) {
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly)) {
                uploadImageData = file.readAll(); // Use the member variable
                file.close();
                QMessageBox::information(this, "Success", "Image uploaded successfully.");
                qDebug() << "Image size:" << uploadImageData.size() << "bytes";
            } else {
                QMessageBox::warning(this, "Error", "Failed to open image file.");
            }
        }
    });
    // Connect the submit button
    connect(submitButton, &QPushButton::clicked, this, [this, nomMaterielInput, categorieInput, descriptionInput, stockInput, quantiteMaximaleInput, fournisseurInput]() {
        // Get input values
        QString nomMateriel = nomMaterielInput->text();
        QString categorie = categorieInput->text();
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
        toolsManager->setFournisseur(fournisseur);
        toolsManager->setUploadImage(uploadImageData);
        // Add the tool to the database
        toolsManager->addTool();

        // Switch back to the table page
        stackedWidget->setCurrentWidget(toolsTablePage);

    });

    // Connect the back button
    connect(backButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(toolsTablePage);
    });
}

// Tools-related slots
void MainWindow::onAddToolClicked() {
    stackedWidget->setCurrentWidget(addToolFormPage);
}

void MainWindow::onEditToolClicked() {
    // Example: Edit an existing tool
    bool ok;
    int id = QInputDialog::getInt(this, "Modifier Materiel", "Entrez l'ID du matériel à modifier:", 1, 1, 100, 1, &ok);
    if (ok) {
        toolsManager->setId(id);
        toolsManager->setNomMateriel("Microscope Modifié");
        toolsManager->setCategorie("Équipement Modifié");
        toolsManager->setDescription("Description Modifiée");
        toolsManager->setStock(10);
        toolsManager->setQuantiteMaximale(20);
        toolsManager->setUploadImage(QByteArray()); // Add image data if needed
        toolsManager->setFournisseur("Fournisseur B");
        toolsManager->editTool();
    }
}

void MainWindow::onDeleteToolClicked() {
    // Example: Delete a tool
    bool ok;
    int id = QInputDialog::getInt(this, "Supprimer Materiel", "Entrez l'ID du matériel à supprimer:", 1, 1, 100, 1, &ok);
    if (ok) {
        toolsManager->setId(id);
        toolsManager->deleteTool();
    }
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

void MainWindow::showresearchersPage() {
    stackedWidget->setCurrentWidget(researchersPage);
    updateSidebarIcons(btnResearchers);
}

void MainWindow::showToolsPage() {
    stackedWidget->setCurrentWidget(toolsPage);
    updateSidebarIcons(btnTools);
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
void MainWindow::showResearchersTablePage() {
    stackedWidget->setCurrentWidget(ResearchersTablePage);
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
