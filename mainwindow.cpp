#include "mainwindow.h"
#include "toolsmanager.h"
#include "patients.h"
#include"connection.h"
#include"connectionh.h"
#include"arduinomanager.h"
#include"vaccinmanager.h"
#include"messagesmanager.h"
#include "researchermanager.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QComboBox>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlError>
#include <QComboBox>
#include <QTimer>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>
#include <QDir>
#include <QPrinter>
#include <QTextDocument>
#include <QDialogButtonBox>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QEasingCurve>
#include <QInputDialog>
#include <QGroupBox>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
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
    //setupArduinoConnection();



    // ✅ Load patients when the application starts
    patientsManager->loadPatients();
}

MainWindow::~MainWindow() {


}

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
    btnMessagerie = createSidebarButton("  Messagerie", ":/icons/svg/message.svg");
    btnSettings = createSidebarButton("  Paramètres", ":/icons/svg/settings.svg");
    // Button Group
    sidebarButtonGroup = new QButtonGroup(this);
    sidebarButtonGroup->addButton(btnPatients);
    sidebarButtonGroup->addButton(btnPersonel);
    sidebarButtonGroup->addButton(btnResearches);
    sidebarButtonGroup->addButton(btnResearchers);
    sidebarButtonGroup->addButton(btnTools);
    sidebarButtonGroup->addButton(btnVaccins);
    sidebarButtonGroup->addButton(btnMessagerie);
    sidebarButtonGroup->addButton(btnSettings);
    sidebarButtonGroup->setExclusive(true);
    // Add Buttons to Sidebar
    sidebarLayout->addWidget(btnPatients);
    sidebarLayout->addWidget(btnPersonel);
    sidebarLayout->addWidget(btnResearches);
    sidebarLayout->addWidget(btnResearchers);
    sidebarLayout->addWidget(btnTools);
    sidebarLayout->addWidget(btnVaccins);
    sidebarLayout->addWidget(btnMessagerie);
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
    connect(btnResearchers, &QPushButton::clicked, this, &MainWindow::showresearchersPage);
    connect(btnTools, &QPushButton::clicked, this, &MainWindow::showToolsPage);
    connect(btnVaccins, &QPushButton::clicked, this, &MainWindow::showVaccinsPage);
    connect(btnMessagerie, &QPushButton::clicked, this, &MainWindow::showMessagerieePage);
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
    addToolPage = new QWidget();
    PatientsTablePage = new QWidget();
    addPatientsFormPage = new QWidget();
    ModifyPatientFormPage = new QWidget();
    messagerieePage = new QWidget();  // Added messaging page
    vaccinsTablePage = new QWidget();
    addVaccinFormPage = new QWidget();
    editVaccinFormPage = new QWidget();
    ResearchersTablePage = new QWidget();
    addResearcherFormPage = new QWidget();
    ModifyResearcherFormPage = new QWidget();
    researchersStatsPage = new QWidget();



    // Setup Pages
    //setupresearchersPage();
    setupResearchersStatsPage();


    // Setup Pages
    setupresearchersPage();
    setupResearchersTablePage();
    setupaddResearcherFormPage();
    setupModifyResearcherFormPage(ID);
    setuppatientsPage();
    setupPatientsTablePage();
    setupaddPatientsFormPage();
    setupModifyPatientFormPage(ID);
    setupVaccinsPage();
    setupVaccinsTablePage();
    setupAddVaccinFormPage();  // Ajoutez cette ligne
    setupEditVaccinFormPage(); // Ajoutez aussi cette ligne si vous avez cette fonction
    setupToolsPage();
    setupToolsTablePage();
    setupAddToolPage();
    setupMessagerieePage();
    patientsManager->loadPatients();    // Added setup for messaging page




    // Add Pages to Stacked Widget
    stackedWidget->addWidget(patientsPage);
    stackedWidget->addWidget(researchersPage);
    stackedWidget->addWidget(vaccinsPage);
    stackedWidget->addWidget(personelPage);
    stackedWidget->addWidget(researchesPage);
    stackedWidget->addWidget(toolsPage);
    stackedWidget->addWidget(settingsPage);
    stackedWidget->addWidget(toolsTablePage);
    stackedWidget->addWidget(addToolPage);
    stackedWidget->addWidget(PatientsTablePage);
    stackedWidget->addWidget(addPatientsFormPage);
    stackedWidget->addWidget(ModifyPatientFormPage);
    stackedWidget->addWidget(vaccinsTablePage);
    stackedWidget->addWidget(addVaccinFormPage);
    stackedWidget->addWidget(editVaccinFormPage);
    stackedWidget->addWidget(messagerieePage);

    stackedWidget->addWidget(ResearchersTablePage);
    stackedWidget->addWidget(addResearcherFormPage);
    stackedWidget->addWidget(ModifyResearcherFormPage);
    stackedWidget->addWidget(researchersStatsPage);



    // Set Default Page
    stackedWidget->setCurrentWidget(patientsPage);
}

void MainWindow::setuppatientsPage() {
    QVBoxLayout *patientLayout = new QVBoxLayout(patientsPage);

    // Add "Voir Matériels" Button

    QPushButton *goToPatientsTableButton = new QPushButton("Voir Patients", this);
    goToPatientsTableButton->setStyleSheet(
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
    patientLayout->addWidget(goToPatientsTableButton, 0, Qt::AlignCenter);

    // Connect Button to Slot
    connect(goToPatientsTableButton, &QPushButton::clicked, this, &MainWindow::showPatientsTablePage);


}


void MainWindow::setupPatientsTablePage() {
    QVBoxLayout *layout = new QVBoxLayout(PatientsTablePage);
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    layout->setContentsMargins(10, 10, 10, 10);  // Add some margins
    layout->setSpacing(10);

    QPushButton *addPatientButton = new QPushButton("Ajouter Patients", this);
    QPushButton *editPatientButton = new QPushButton("Modifier Patients", this);
    QPushButton *deletePatientButton = new QPushButton("Supprimer Patients", this);


    // Add "Ajouter Materiels" Button
    QList<QPushButton*> buttons = {addPatientButton, editPatientButton, deletePatientButton};
    for (auto *btn : buttons) {
        btn->setStyleSheet(
            "QPushButton {"
            "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #198754, stop:1 #28a745);"
            "    color: white;"
            "    padding: 10px 20px;"
            "    border-radius: 10px;"
            "    font-size: 15px;"
            "    font-weight: bold;"
            "    font-family: 'Segoe UI';"
            "}"
            "QPushButton:hover {"
            "    background-color: #157347;"
            "}"
            );
    }
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

    QPushButton *pdfButton = new QPushButton("Exporter PDF", this);
    pdfButton->setIcon(QIcon(":/icons/svg/pdf.svg")); // optional icon
    pdfButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #dc3545;"
        "    color: white;"
        "    padding: 8px 16px;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #bb2d3b;"
        "}"
        );





    // Top Button Layout
    PatientsTablePage->setLayout(layout);

    QComboBox *searchFieldCombo = new QComboBox();
    searchFieldCombo->addItem("Nom", "Nom");
    searchFieldCombo->addItem("Prénom", "Prenom");
    searchFieldCombo->addItem("CIN", "CIN");
    searchFieldCombo->addItem("Téléphone", "NumTel");

    QLineEdit *searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Rechercher...");

    QPushButton *searchButton = new QPushButton("Rechercher");
    QPushButton *resetSearchButton = new QPushButton("Réinitialiser");

    // Sort Controls
    QComboBox *sortFieldCombo = new QComboBox();
    sortFieldCombo->addItem("Nom", "Nom");
    sortFieldCombo->addItem("Sexe", "Sexe");
    sortFieldCombo->addItem("Gouvernorat", "Adresse");

    QPushButton *sortAscButton = new QPushButton("Tri Croissant");
    QPushButton *sortDescButton = new QPushButton("Tri Décroissant");

    // Add controls to layout
    /*  controlsLayout->addWidget(new QLabel("Rechercher par:"));
    controlsLayout->addWidget(searchFieldCombo);
    controlsLayout->addWidget(searchInput);
    controlsLayout->addWidget(searchButton);
    controlsLayout->addWidget(resetSearchButton);
    controlsLayout->addSpacing(20);
    controlsLayout->addWidget(new QLabel("Trier par:"));
    controlsLayout->addWidget(sortFieldCombo);
    controlsLayout->addWidget(sortAscButton);
    controlsLayout->addWidget(sortDescButton);
*/
    layout->addLayout(controlsLayout);

    // ========== STATISTICS SECTION ==========
    // In setupPatientsTablePage() - replace the statistics section with this:
    // ========== SECTION STATISTIQUES ==========
    // Statistics Section
    // Statistics Section
    // ========== STATISTICS SECTION ==========
    // ========== STATISTICS SECTION ==========
    QWidget *statsHeader = new QWidget();
    QHBoxLayout *headerLayout = new QHBoxLayout(statsHeader);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(12);

    // Section Title
    QLabel *statsTitle = new QLabel(tr("STATISTIQUES PATIENTS"));
    statsTitle->setStyleSheet(
        "QLabel {"
        "  color: #212529;"
        "  font-size: 22px;"
        "  font-weight: 800;"
        "  letter-spacing: 1.2px;"
        "  font-family: 'Segoe UI', sans-serif;"
        "}"
        );
    headerLayout->addWidget(statsTitle);
    headerLayout->addStretch();

    // 🌗 Dark Mode Toggle
    QPushButton *toggleDarkBtn = new QPushButton(darkModeEnabled ? tr("Mode Clair") : tr("Mode Sombre"));
    toggleDarkBtn->setCursor(Qt::PointingHandCursor);
    toggleDarkBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #007bff;"
        "  color: white;"
        "  padding: 6px 14px;"
        "  border-radius: 8px;"
        "  font-weight: 600;"
        "  font-family: 'Segoe UI', sans-serif;"
        "}"
        "QPushButton:hover { background-color: #0056b3; }"
        );
    connect(toggleDarkBtn, &QPushButton::clicked, this, [=]() {
        darkModeEnabled = !darkModeEnabled;
        toggleDarkBtn->setText(darkModeEnabled ? tr("Mode Clair") : tr("Mode Sombre"));
        updateStatistics();  // Trigger full refresh
    });
    headerLayout->addWidget(toggleDarkBtn);

    layout->addWidget(statsHeader);

    // 📊 Card Layout Section
    statsContainer = new QWidget();
    statsLayout = new QHBoxLayout(statsContainer);
    statsLayout->setContentsMargins(0, 0, 0, 0);
    statsLayout->setSpacing(20);

    // 🎨 Chart Colors
    QStringList colors = {"#198754", "#6f42c1", "#fd7e14", "#20c997"};

    totalCard = createModernStatCard(tr("PATIENTS TOTAUX"), QMap<QString, int>{{tr("Nombre"), 0}}, colors[0]);
    genderCard = createModernStatCard(tr("GENRE"), QMap<QString, int>{{tr("Femme"), 0}, {tr("Homme"), 0}}, colors[1]);
    chronicCard = createModernStatCard(tr("MALADIES CHRONIQUES"), QMap<QString, int>{{tr("Avec"), 0}, {tr("Sans"), 0}}, colors[2]);
    cityCard = createModernStatCard(tr("RÉGION PRINCIPALE"), QMap<QString, int>{{"-", 0}}, colors[3]);

    statsLayout->addWidget(totalCard);
    statsLayout->addWidget(genderCard);
    statsLayout->addWidget(chronicCard);
    statsLayout->addWidget(cityCard);

    // Equal stretching
    for (int i = 0; i < statsLayout->count(); ++i)
        statsLayout->setStretch(i, 1);

    // Wrapper with background
    QWidget *statsWrapper = new QWidget();
    statsWrapper->setStyleSheet(
        QString("QWidget { background-color: %1; border-radius: 16px; padding: 24px; }")
            .arg(darkModeEnabled ? "#1e1e1e" : "#f8f9fa")
        );
    QVBoxLayout *wrapperLayout = new QVBoxLayout(statsWrapper);
    wrapperLayout->addWidget(statsContainer);
    layout->addWidget(statsWrapper);


    // ========== END STATISTICS SECTION ==========

    // ========== END STATISTICS SECTION ==========



    // ========== END STATISTICS SECTION ==========

    // ========== END STATISTICS SECTION ==========
    PatientsTablePage->setStyleSheet(
        // Existing table styles
        "QTableWidget {"
        "    background-color: #FFFFFF;"
        "    border: 1px solid #ddd;"
        "    border-radius: 8px;"
        "}"
        "QTableWidget::item {"
        "    border-bottom: 1px solid #ddd;"
        "    padding: 8px;"
        "    color: black;"
        "}"
        "QTableWidget::item:selected {"
        "    background: #EDEDED;"
        "}"
        "QHeaderView::section {"
        "    background-color: #F5F5F7;"
        "    color: black;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "    padding: 8px;"
        "    border: none;"
        "    border-bottom: 1px solid #ddd;"
        "    color: black;"
        "}"

        // Styles for search and sort controls
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

        "QLineEdit {"
        "    padding: 6px;"
        "    border: 1px solid #ddd;"
        "    border-radius: 4px;"
        "    min-width: 150px;"
        "    color: black;"
        "}"
        "QLineEdit:hover {"
        "    border-color: #198754;"
        "}"
        "QLineEdit:focus {"
        "    border-color: #198754;"
        "    border-width: 1px;"
        "    color: black;"
        "}"

        // Button styles (extending your existing button styles)
        "QPushButton {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #198754, stop:1 #28a745);"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border-radius: 10px;"
        "    font-size: 15px;"
        "    font-weight: bold;"
        "    font-family: 'Segoe UI';"
        "}"
        "QPushButton:hover {"
        "    background-color: #157347;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #0d5c33;"
        "    color: black;"
        "}"

        // Label styles
        "QLabel {"
        "    font-size: 13px;"
        "    color: #555;"
        "}"
        );

    QHBoxLayout *topButtonLayout = new QHBoxLayout();
    controlsLayout->addWidget(backButton);
    controlsLayout->addSpacing(20);
    controlsLayout->addWidget(pdfButton);
    controlsLayout->addStretch();
    controlsLayout->addWidget(addPatientButton);
    controlsLayout->addWidget(editPatientButton);
    controlsLayout->addWidget(deletePatientButton);


    topButtonLayout->addWidget(new QLabel("Rechercher par:"));
    topButtonLayout->addWidget(searchFieldCombo);
    topButtonLayout->addWidget(searchInput);
    topButtonLayout->addWidget(searchButton);
    topButtonLayout->addWidget(resetSearchButton);
    topButtonLayout->addSpacing(20);
    topButtonLayout->addWidget(new QLabel("Trier par:"));
    topButtonLayout->addWidget(sortFieldCombo);
    topButtonLayout->addWidget(sortAscButton);
    topButtonLayout->addWidget(sortDescButton);








    // Create the table widget
    QTableWidget *patientstable = new QTableWidget(this);

    // Set table properties
    patientstable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    patientstable->setColumnCount(9);
    patientstable->setHorizontalHeaderLabels({
        "IDP", "Nom", "Prenom", "Sexe", "CIN", "Adresse", "NumTel", "Maladie Chronique", "Historique"
    });

    // Add the table to the layout with stretch factor
    // Your existing button layout

    layout->addLayout(topButtonLayout, 0);    // Your statistics section
    layout->addWidget(patientstable, 1);
    // Initialize PatientsManager
    patientsManager = new class PatientsManager(patientstable, this);
    patientsManager->loadPatients();
    // In the dataChanged connection:
    // In setupPatientsTablePage() function
    connect(patientsManager, &PatientsManager::dataChanged, this, [this, patientstable]() {
        patientstable->setRowCount(0);
        patientstable->setColumnCount(9); // Ensure 9 columns exist

        QSqlQuery query("SELECT IDP, Nom, Prenom, Sexe, CIN, Adresse, NumTel, MaladieChronique FROM patients ORDER BY IDP DESC");

        int row = 0;
        while (query.next()) {
            patientstable->insertRow(row);

            // Set data for columns 0-7
            for (int col = 0; col < 8; col++) {
                QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                patientstable->setItem(row, col, item);
            }

            // Create history button for column 8
            QPushButton *historyBtn = new QPushButton("Historique Vaccin");
            QFontMetrics fm(historyBtn->font());
            int textWidth = fm.horizontalAdvance("Historique Vaccin") + 32; // Text width + padding
            historyBtn->setStyleSheet(QString(
                                          "QPushButton {"
                                          "    background-color: #20c997;"
                                          "    color: white;"
                                          "    border-radius: 6px;"
                                          "    padding: 6px 12px;"
                                          "    min-width: %1px;"  // Dynamic minimum width
                                          "    font-size: 12px;"
                                          "}"
                                          "QPushButton:hover { background-color: #1aa179; }"
                                          ).arg(textWidth));

            int patientId = query.value("IDP").toInt();
            connect(historyBtn, &QPushButton::clicked, this, [this, patientId]() {
                showVaccinationHistory(patientId);
            });

            patientstable->setCellWidget(row, 8, historyBtn);
            row++;
        }

        // Force column widths to show all content
        patientstable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        patientstable->horizontalHeader()->setStretchLastSection(true);
    });
    // Connect buttons
    connect(addPatientButton, &QPushButton::clicked, this, &MainWindow::onAddPatientClicked);
    connect(editPatientButton, &QPushButton::clicked, this, &MainWindow::onEditPatientClicked);
    connect(deletePatientButton, &QPushButton::clicked, this, &MainWindow::onDeletePatientClicked);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showPatientsPage);
    //connect(pdfButton, &QPushButton::clicked, this, &MainWindow::exportStatsToPDF);

    // Connect search and sort buttons
    connect(searchButton, &QPushButton::clicked, this, [this, searchFieldCombo, searchInput, patientstable]() {
        QString field = searchFieldCombo->currentData().toString();
        QString value = searchInput->text().trimmed();

        if (value.isEmpty()) {
            patientsManager->loadPatients();
            return;
        }

        auto results = patientsManager->searchPatients(field, value);
        patientstable->setRowCount(0);

        for (const auto &row : results) {
            int currentRow = patientstable->rowCount();
            patientstable->insertRow(currentRow);

            for (int col = 0; col < row.size(); col++) {
                patientstable->setItem(currentRow, col, new QTableWidgetItem(row[col]));
            }

            // Add "Historique Vaccin" button
            QPushButton *historyBtn = new QPushButton("Historique Vaccin");
            // Get font metrics for accurate text sizing
            QFontMetrics fm(historyBtn->font());
            int textWidth = fm.horizontalAdvance("Historique Vaccin") + 32; // Text width + padding

            historyBtn->setStyleSheet(QString(
                                          "QPushButton {"
                                          "    background-color: #20c997;"
                                          "    color: white;"
                                          "    border-radius: 6px;"
                                          "    padding: 6px 12px;"
                                          "    min-width: %1px;"  // Dynamic minimum width
                                          "    font-size: 12px;"
                                          "}"
                                          "QPushButton:hover { background-color: #1aa179; }"
                                          ).arg(textWidth));

            // Size policy for better scaling
            historyBtn->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
            // Extract patient ID from the row
            int patientId = row[0].toInt();
            historyBtn->setFixedSize(100, 32);  // Width x Height in pixels
            historyBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

            connect(historyBtn, &QPushButton::clicked, this, [this, patientId]() {
                showVaccinationHistory(patientId);
            });

            patientstable->setCellWidget(currentRow, 8, historyBtn);
        }

    });

    connect(resetSearchButton, &QPushButton::clicked, this, [this, searchInput]() {
        searchInput->clear();
        patientsManager->loadPatients();
    });

    connect(sortAscButton, &QPushButton::clicked, this, [this, sortFieldCombo, patientstable]() {
        QString field = sortFieldCombo->currentData().toString();
        auto results = patientsManager->sortPatients(field, true);

        patientstable->setRowCount(0);
        for (const auto &row : results) {
            int currentRow = patientstable->rowCount();
            patientstable->insertRow(currentRow);

            for (int col = 0; col < row.size(); col++) {
                patientstable->setItem(currentRow, col, new QTableWidgetItem(row[col]));
            }
            QPushButton *historyBtn = new QPushButton("Historique Vaccin");
            // Get font metrics for accurate text sizing
            QFontMetrics fm(historyBtn->font());
            int textWidth = fm.horizontalAdvance("Historique Vaccin") + 32; // Text width + padding

            historyBtn->setStyleSheet(QString(
                                          "QPushButton {"
                                          "    background-color: #20c997;"
                                          "    color: white;"
                                          "    border-radius: 6px;"
                                          "    padding: 6px 12px;"
                                          "    min-width: %1px;"  // Dynamic minimum width
                                          "    font-size: 12px;"
                                          "}"
                                          "QPushButton:hover { background-color: #1aa179; }"
                                          ).arg(textWidth));

            // Size policy for better scaling
            historyBtn->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
            int patientId = row[0].toInt();
            historyBtn->setFixedSize(100, 32);  // Width x Height in pixels
            historyBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            connect(historyBtn, &QPushButton::clicked, this, [this, patientId]() {
                showVaccinationHistory(patientId);
            });
            patientstable->setCellWidget(currentRow, 8, historyBtn);


        }
    });

    connect(sortDescButton, &QPushButton::clicked, this, [this, sortFieldCombo, patientstable]() {
        QString field = sortFieldCombo->currentData().toString();
        auto results = patientsManager->sortPatients(field, false);

        patientstable->setRowCount(0);
        for (const auto &row : results) {
            int currentRow = patientstable->rowCount();
            patientstable->insertRow(currentRow);

            for (int col = 0; col < row.size(); col++) {
                patientstable->setItem(currentRow, col, new QTableWidgetItem(row[col]));
            }
            QPushButton *historyBtn = new QPushButton("Historique Vaccin");
            // Get font metrics for accurate text sizing
            QFontMetrics fm(historyBtn->font());
            int textWidth = fm.horizontalAdvance("Historique Vaccin") + 32; // Text width + padding

            historyBtn->setStyleSheet(QString(
                                          "QPushButton {"
                                          "    background-color: #20c997;"
                                          "    color: white;"
                                          "    border-radius: 6px;"
                                          "    padding: 6px 12px;"
                                          "    min-width: %1px;"  // Dynamic minimum width
                                          "    font-size: 12px;"
                                          "}"
                                          "QPushButton:hover { background-color: #1aa179; }"
                                          ).arg(textWidth));

            // Size policy for better scaling
            historyBtn->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
            int patientId = row[0].toInt();
            historyBtn->setFixedSize(100, 32);  // Width x Height in pixels
            historyBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            connect(historyBtn, &QPushButton::clicked, this, [this, patientId]() {
                showVaccinationHistory(patientId);
            });
            patientstable->setCellWidget(currentRow, 8, historyBtn);
        }
    });

    // Update statistics when data changes
    connect(patientsManager, &PatientsManager::dataChanged, this, [this]() {
        this->updateStatistics();
    });

    // Initial update
    patientsManager->loadPatients();
    updateStatistics();
}


#include <algorithm> // ADD THIS AT THE TOP OF YOUR FILE IF NOT PRESENT

void MainWindow::updateStatistics() {
    if (!patientsManager || !statsLayout) return;

    // Fetch distributions
    QMap<QString, int> genderDist = patientsManager->getGenderDistribution();
    QMap<QString, int> chronicDist = patientsManager->getChronicDiseaseDistribution();
    QMap<QString, int> cityDist = patientsManager->getCityDistribution();

    int totalPatients = genderDist.value("Homme", 0) + genderDist.value("Femme", 0);
    int withChronic = chronicDist.value("Avec", 0);
    int withoutChronic = totalPatients - withChronic;

    // Filter valid cities only
    QMap<QString, int> filteredCities;
    for (auto it = cityDist.begin(); it != cityDist.end(); ++it) {
        if (it.value() > 0) {
            filteredCities.insert(it.key(), it.value());
        }
    }

    // Clear current cards
    QLayoutItem* child;
    while ((child = statsLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    // Add refreshed stat cards
    statsLayout->addWidget(createModernStatCard(tr("PATIENTS TOTAUX"),
                                                {{tr("Nombre"), totalPatients}},
                                                "#198754"));

    statsLayout->addWidget(createModernStatCard(tr("GENRE"),
                                                {
                                                    {tr("Femme"), genderDist.value("Femme", 0)},
                                                    {tr("Homme"), genderDist.value("Homme", 0)}
                                                },
                                                "#6f42c1"));

    statsLayout->addWidget(createModernStatCard(tr("MALADIES CHRONIQUES"),
                                                {
                                                    {tr("Avec"), withChronic},
                                                    {tr("Sans"), withoutChronic}
                                                },
                                                "#fd7e14"));

    statsLayout->addWidget(createModernStatCard(tr("RÉGION PRINCIPALE"),
                                                filteredCities,
                                                "#20c997"));

    // Equal spacing
    for (int i = 0; i < statsLayout->count(); ++i) {
        statsLayout->setStretch(i, 1);
    }

    // Apply fade-in animation
    QGraphicsOpacityEffect *fadeEffect = new QGraphicsOpacityEffect(this);
    statsContainer->setGraphicsEffect(fadeEffect);

    QPropertyAnimation *fadeAnim = new QPropertyAnimation(fadeEffect, "opacity");
    fadeAnim->setDuration(300);
    fadeAnim->setStartValue(0);
    fadeAnim->setEndValue(1);
    fadeAnim->setEasingCurve(QEasingCurve::InOutQuad);
    fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);
}


void MainWindow::exportStatsToPDF() {
    if (!patientsManager) {
        QMessageBox::warning(this, "Error", "Patient manager not initialized!");
        return;
    }

    // Get save file path
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Export Patient Report",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
            "/Patient_Report_" + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".pdf",
        "PDF Files (*.pdf)"
        );

    if (filePath.isEmpty()) return;

    // Initialize PDF writer
    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(120);
    writer.setTitle("Patient Management Report");
    writer.setCreator("BioLab Management System");

    QPainter painter(&writer);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // Styles
    QFont titleFont("Arial", 18, QFont::Bold);
    QFont headerFont("Arial", 12, QFont::Bold);
    QFont contentFont("Arial", 10);
    QFont smallFont("Arial", 8);

    // Colors
    QColor titleColor(25, 135, 84); // Green
    QColor headerColor(50, 50, 50);
    QColor textColor(20, 20, 20);
    QColor lightGray(240, 240, 240);

    // Layout metrics
    const int margin = 40;
    int y = margin;
    const int lineHeight = 20;
    const int pageWidth = writer.width() - 2 * margin;

    // ===== PAGE 1: COVER =====
    painter.setFont(titleFont);
    painter.setPen(titleColor);
    painter.drawText(margin, y, "Patient Management Report");
    y += lineHeight * 2;

    painter.setFont(contentFont);
    painter.setPen(textColor);
    painter.drawText(margin, y, "Generated: " + QDateTime::currentDateTime().toString("MMMM d, yyyy h:mm AP"));
    y += lineHeight * 3;

    // Get statistics using the same query structure as loadPatients()
    QSqlQuery countQuery("SELECT COUNT(*) FROM patients");
    int totalPatients = 0;
    if (countQuery.next()) {
        totalPatients = countQuery.value(0).toInt();
    }

    QSqlQuery genderQuery("SELECT Sexe, COUNT(*) FROM patients GROUP BY Sexe");
    QMap<QString, int> genderDist;
    while (genderQuery.next()) {
        genderDist[genderQuery.value(0).toString()] = genderQuery.value(1).toInt();
    }

    QSqlQuery chronicQuery("SELECT COUNT(*) FROM patients WHERE MaladieChronique IS NOT NULL AND MaladieChronique != ''");
    int withChronic = 0;
    if (chronicQuery.next()) {
        withChronic = chronicQuery.value(0).toInt();
    }
    double chronicPercentage = totalPatients > 0 ? (withChronic * 100.0) / totalPatients : 0;

    // Draw summary stats
    painter.setFont(headerFont);
    painter.drawText(margin, y, "Summary Statistics:");
    y += lineHeight;

    painter.setFont(contentFont);
    painter.drawText(margin, y, QString("• Total Patients: %1").arg(totalPatients));
    y += lineHeight;

    // Handle gender distribution (using same labels as in your table)
    painter.drawText(margin, y, QString("• Male Patients: %1").arg(genderDist.value("Homme", 0)));
    y += lineHeight;
    painter.drawText(margin, y, QString("• Female Patients: %1").arg(genderDist.value("Femme", 0)));
    y += lineHeight;

    painter.drawText(margin, y, QString("• Patients with Chronic Conditions: %1%").arg(qRound(chronicPercentage)));
    y += lineHeight * 2;

    // ===== PAGE 2: PATIENT DATA =====
    writer.newPage();
    y = margin;

    painter.setFont(titleFont);
    painter.setPen(titleColor);
    painter.drawText(margin, y, "Patient Records");
    y += lineHeight * 2;

    // Get patient data using the same query as loadPatients()
    QSqlQuery patientQuery("SELECT IDP, Nom, Prenom, Sexe, CIN, Adresse, NumTel, MaladieChronique FROM patients ORDER BY IDP DESC");

    if (!patientQuery.isActive()) {
        painter.setFont(contentFont);
        painter.drawText(margin, y, "Error: Could not retrieve patient data from database.");
        painter.end();
        QMessageBox::warning(this, "Database Error", "Failed to execute patient data query.");
        return;
    }

    if (!patientQuery.first()) {
        painter.setFont(contentFont);
        painter.drawText(margin, y, "No patient records found in database.");
        painter.end();
        QMessageBox::information(this, "Export Complete", "PDF generated with empty dataset.");
        return;
    }

    // Table setup - matching your table columns
    QStringList headers = {"ID", "Last Name", "First Name", "Gender", "CIN", "Address", "Phone", "Chronic Condition"};
    QVector<int> colWidths = {40, 100, 100, 60, 80, 120, 80, 120};

    // Draw table header
    painter.setFont(headerFont);
    painter.setPen(Qt::NoPen);
    painter.setBrush(lightGray);
    painter.drawRect(margin, y, pageWidth, lineHeight * 1.5);

    painter.setPen(headerColor);
    int x = margin;
    for (int i = 0; i < headers.size(); i++) {
        painter.drawText(x + 5, y + lineHeight, headers[i]);
        x += colWidths[i];
    }
    y += lineHeight * 1.5 + 5;

    // Draw rows
    painter.setFont(contentFont);
    painter.setPen(textColor);
    bool alternate = false;

    do {
        // Check for page break
        if (y > writer.height() - margin - lineHeight * 3) {
            writer.newPage();
            y = margin;

            // Redraw header
            painter.setFont(headerFont);
            painter.setPen(Qt::NoPen);
            painter.setBrush(lightGray);
            painter.drawRect(margin, y, pageWidth, lineHeight * 1.5);

            painter.setPen(headerColor);
            x = margin;
            for (int i = 0; i < headers.size(); i++) {
                painter.drawText(x + 5, y + lineHeight, headers[i]);
                x += colWidths[i];
            }
            y += lineHeight * 1.5 + 5;

            painter.setFont(contentFont);
            alternate = false;
        }

        // Draw row background
        painter.setPen(Qt::NoPen);
        painter.setBrush(alternate ? QColor(248, 249, 250) : Qt::white);
        painter.drawRect(margin, y, pageWidth, lineHeight * 1.2);
        alternate = !alternate;

        // Draw cells - matching your table columns
        painter.setPen(textColor);
        x = margin;
        for (int col = 0; col < 8; col++) {
            QString text = patientQuery.value(col).toString();
            if (text.length() > 15) text = text.left(12) + "...";
            painter.drawText(x + 5, y + lineHeight, text);
            x += colWidths[col];
        }
        y += lineHeight * 1.2;
    } while (patientQuery.next());

    painter.end();

    // Show success message
    QMessageBox::information(this, "Export Successful",
                             QString("Patient report successfully saved to:\n%1").arg(filePath));
}







#include <algorithm> // ADD THIS AT THE TOP OF YOUR FILE IF NOT PRESENT

// ADD

QWidget* MainWindow::createModernStatCard(const QString &title, const QMap<QString, int> &data, const QString &accentColor) {
    QWidget *card = new QWidget();
    card->setMinimumWidth(260);
    card->setMaximumWidth(300);

    QString bgColor = darkModeEnabled ? "#1e1e1e" : "#ffffff";
    QString textColor = darkModeEnabled ? "#f1f3f5" : "#212529";
    QString borderColor = darkModeEnabled ? "#343a40" : "#dee2e6";
    QString subTextColor = darkModeEnabled ? "#adb5bd" : "#6c757d";

    card->setStyleSheet(QString(
                            "QWidget { background-color: %1; border-radius: 18px; padding: 12px; border: 1px solid %2; box-shadow: 0px 4px 12px rgba(0,0,0,0.05); }"
                            "QLabel#title { font-size: 14px; color: %3; font-weight: 700; text-transform: uppercase; letter-spacing: 1px; font-family: 'Segoe UI'; margin-bottom: 2px; }"
                            "QLabel#value { font-size: 28px; font-weight: 800; color: %4; font-family: 'Segoe UI'; margin: 2px 0; }"
                            "QLabel#subtitle { font-size: 12px; color: %5; font-style: italic; font-family: 'Segoe UI'; margin-top: 2px; }"
                            ).arg(bgColor, borderColor, subTextColor, textColor, subTextColor));

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setAlignment(Qt::AlignTop);
    layout->setSpacing(6);

    QLabel *titleLabel = new QLabel(title.toUpper());
    titleLabel->setObjectName("title");
    layout->addWidget(titleLabel, 0, Qt::AlignLeft);

    if (title == tr("PATIENTS TOTAUX")) {
        QLabel *valueLabel = new QLabel(QString::number(data.value(tr("Nombre"))));
        valueLabel->setObjectName("value");
        layout->addWidget(valueLabel, 0, Qt::AlignCenter);

        QLabel *sub = new QLabel(tr("Nombre total des patients enregistrés"));
        sub->setObjectName("subtitle");
        layout->addWidget(sub, 0, Qt::AlignCenter);

    } else if (title == tr("GENRE") || title == tr("MALADIES CHRONIQUES")) {
        QPieSeries *series = new QPieSeries();
        series->setHoleSize(0.55);
        series->setPieSize(0.9);
        series->setLabelsVisible(false);  // We'll handle labels manually

        int total = 0;
        for (int value : data.values()) total += value;
        if (total == 0) total = 1;

        // Create slices
        QList<QColor> sliceColors = {
            QColor(accentColor),
            QColor(accentColor).lighter(150),
            QColor(accentColor).darker(120),
            QColor(accentColor).lighter(180)
        };

        int colorIndex = 0;
        for (auto it = data.begin(); it != data.end(); ++it) {
            QPieSlice *slice = series->append(it.key(), it.value());
            slice->setColor(sliceColors[colorIndex % sliceColors.size()]);
            colorIndex++;
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->legend()->hide();
        chart->setBackgroundVisible(false);
        chart->setMargins(QMargins(0, 0, 0, 0));
        chart->setContentsMargins(0, 0, 0, 0);
        chart->setAnimationOptions(QChart::AllAnimations);

        // Custom chart view that will handle the percentage labels
        // Custom chart view that will handle the percentage labels
        class PieChartView : public QChartView {
        public:
            PieChartView(QChart *chart, QPieSeries *series, const QString &textColor)
                : QChartView(chart), m_series(series), m_textColor(textColor) {
                setRenderHint(QPainter::Antialiasing);
            }

        protected:
            void drawForeground(QPainter *painter, const QRectF &rect) override {
                QChartView::drawForeground(painter, rect);

                painter->save();
                painter->setRenderHint(QPainter::Antialiasing);

                QRectF plotArea = chart()->plotArea();
                QPointF center = plotArea.center();

                // Distance from center where the text appears (inside the arc)
                qreal innerRadius = qMin(plotArea.width(), plotArea.height()) * 0.30;

                for (QPieSlice *slice : m_series->slices()) {
                    if (slice->value() == 0) continue;

                    // Calculate angle in radians to position label
                    qreal angle = slice->startAngle() + slice->angleSpan() / 2.0;
                    qreal rad = qDegreesToRadians(angle);

                    QPointF labelPos = center + QPointF(innerRadius * cos(rad), innerRadius * sin(rad));

                    // Draw percentage label
                    //int percent = qRound((slice->value() * 100.0) / m_series->sum());
                    //QString text = QString("%1%").arg(percent);

                    QFont font("Segoe UI", 10, QFont::Bold);
                    painter->setFont(font);
                    painter->setPen(QColor(m_textColor));

                    QFontMetrics fm(font);
                    //QRect textRect = fm.boundingRect(text);
                    //QPointF textTopLeft(labelPos.x() - textRect.width() / 2, labelPos.y() - textRect.height() / 2);

                    // painter->drawText(QRectF(textTopLeft, textRect.size()), Qt::AlignCenter, text);
                }

                painter->restore();
            }

        private:
            QPieSeries *m_series;
            QString m_textColor;
        };


        PieChartView *chartView = new PieChartView(chart, series, textColor);
        chartView->setFixedHeight(150);
        chartView->setStyleSheet("background-color: transparent;");

        QVBoxLayout *chartWithLegendLayout = new QVBoxLayout();
        chartWithLegendLayout->addWidget(chartView);

        // Compact legend layout
        QHBoxLayout *legendLayout = new QHBoxLayout();
        legendLayout->setSpacing(8);
        legendLayout->setAlignment(Qt::AlignCenter);

        QList<QPieSlice*> slices = series->slices();
        for (QPieSlice *slice : slices) {
            QWidget *legendItem = new QWidget();
            legendItem->setFixedHeight(18);
            QHBoxLayout *itemLayout = new QHBoxLayout(legendItem);
            itemLayout->setContentsMargins(0, 0, 0, 0);
            itemLayout->setSpacing(4);

            QLabel *colorBox = new QLabel();
            colorBox->setFixedSize(10, 10);
            colorBox->setStyleSheet(QString("background-color: %1; border-radius: 2px;").arg(slice->brush().color().name()));

            int percent = qRound((slice->percentage()) * 100);
            QString labelText = QString("%1 (%2%)").arg(slice->label()).arg(percent);
            QLabel *label = new QLabel(labelText);

            label->setStyleSheet(QString("color: %1; font-size: 10px; font-family: 'Segoe UI';").arg(textColor));

            itemLayout->addWidget(colorBox);
            itemLayout->addWidget(label);
            legendLayout->addWidget(legendItem);
        }

        chartWithLegendLayout->addLayout(legendLayout);
        layout->addLayout(chartWithLegendLayout);

        QLabel *sub = new QLabel(title == tr("GENRE") ? tr("Répartition femmes / hommes") : tr("Avec ou sans maladie chronique"));
        sub->setObjectName("subtitle");
        layout->addWidget(sub, 0, Qt::AlignCenter);

    } else if (title == tr("RÉGION PRINCIPALE")) {
        // Get the top region
        QString topRegion = "-";
        int topCount = 0;
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (it.value() > topCount) {
                topCount = it.value();
                topRegion = it.key();
            }
        }

        // Compact region display
        QWidget *regionDisplay = new QWidget();
        QHBoxLayout *regionLayout = new QHBoxLayout(regionDisplay);
        regionLayout->setContentsMargins(0, 0, 0, 0);
        regionLayout->setSpacing(12);

        // Smaller region info
        QVBoxLayout *infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(4);

        QLabel *regionName = new QLabel(topRegion);
        regionName->setStyleSheet(QString("font-size: 16px; font-weight: bold; color: %1;").arg(accentColor));

        QLabel *patientCount = new QLabel(QString("%1 patients").arg(topCount));
        patientCount->setStyleSheet(QString("font-size: 13px; color: %1;").arg(subTextColor));

        // Compact progress bar
        QWidget *progressContainer = new QWidget();
        progressContainer->setFixedHeight(6);
        progressContainer->setStyleSheet("background-color: #e9ecef; border-radius: 3px;");

        QWidget *progressBar = new QWidget(progressContainer);
        progressBar->setFixedWidth(qMin(topCount * 2, 180));
        progressBar->setStyleSheet(QString("background-color: %1; border-radius: 3px;").arg(accentColor));

        QHBoxLayout *progressLayout = new QHBoxLayout(progressContainer);
        progressLayout->setContentsMargins(0, 0, 0, 0);
        progressLayout->addWidget(progressBar);

        infoLayout->addWidget(regionName);
        infoLayout->addWidget(patientCount);
        infoLayout->addWidget(progressContainer);
        regionLayout->addLayout(infoLayout);

        layout->addWidget(regionDisplay);

        // Compact subtitle
        if (data.size() > 1) {
            QStringList regions;
            for (auto it = data.begin(); it != data.end(); ++it) {
                regions << QString("%1: %2").arg(it.key()).arg(it.value());
            }

            QLabel *sub = new QLabel(regions.join(" • "));
            sub->setObjectName("subtitle");
            sub->setStyleSheet("font-size: 11px;");
            sub->setWordWrap(true);
            layout->addWidget(sub);
        } else {
            QLabel *sub = new QLabel(tr("Région la plus représentée"));
            sub->setObjectName("subtitle");
            sub->setStyleSheet("font-size: 11px;");
            layout->addWidget(sub);
        }
    }

    return card;
}
















































// Update your updateStatistics() function:


#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

void MainWindow::showVaccinationHistory(int patientId) {
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Historique de Vaccination - Patient ID: " + QString::number(patientId));
    dialog->resize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // Create table to show vaccination history
    QTableWidget *table = new QTableWidget(dialog);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Vaccin", "Date", "Doses Totales", "Prochaine Dose"});
    table->horizontalHeader()->setStretchLastSection(true);

    // Function to send SMS
    // Function to send SMS
    auto sendVaccinationSMS = [](const QString& phoneNumber, const QString& patientName,
                                 const QString& vaccineName, const QDate& nextDoseDate) {
        QString cleanedNumber = phoneNumber.trimmed().remove(QRegularExpression("[^0-9+]"));
        if (cleanedNumber.startsWith("2")) {
            cleanedNumber = "+216" + cleanedNumber;
        } else if (cleanedNumber.startsWith("0")) {
            cleanedNumber = "+216" + cleanedNumber.mid(1);
        } else if (!cleanedNumber.startsWith("+216")) {
            cleanedNumber = "+216" + cleanedNumber;
        }

        if (!QRegularExpression("^\\+216[0-9]{8}$").match(cleanedNumber).hasMatch()) {
            qWarning() << "Invalid Tunisian number:" << cleanedNumber;
            return false;
        }

        QString message = QString(
                              "Bonjour %1,\n"
                              "Rappel: Votre dose de %2 est prévue le %3\n"
                              "Centre Médical BioLab\n"
                              "Contact: +216 51 588 576\n")
                              .arg(patientName, vaccineName, nextDoseDate.toString("dd/MM/yyyy"));

        const QString accountSid = "ACbaa59420b0be9423a398f091c01c6062";
        const QString authToken = "c4d51bc770f7b173cff7b47577d3985a";
        const QString fromNumber = "+19706606312";  // <-- YOUR Twilio Number

        QProcess curl;
        QStringList args = {
            "-X", "POST",
            QString("https://api.twilio.com/2010-04-01/Accounts/%1/Messages.json").arg(accountSid),
            "--data-urlencode", QString("To=%1").arg(cleanedNumber),
            "--data-urlencode", QString("From=%1").arg(fromNumber),
            "--data-urlencode", QString("Body=%1").arg(message),
            "-u", QString("%1:%2").arg(accountSid, authToken)
        };

        curl.start("curl", args);
        if (!curl.waitForFinished(5000)) {
            qWarning() << "SMS sending timed out";
            return false;
        }

        QByteArray response = curl.readAllStandardOutput();
        qDebug() << "Twilio response:" << response;

        if (response.contains("\"error_code\"")) {
            qWarning() << "Twilio error:" << response;
            return false;
        }

        qDebug() << "SMS successfully sent to" << cleanedNumber;
        return true;
    };

    // Load vaccination data into the table
    auto loadVaccinationData = [=]() {
        table->clearContents();
        table->setRowCount(0);

        QSqlQuery query;
        query.prepare("SELECT V.NOMVACCIN, VACC.DATE_DE_VACCINATION, V.NBDOSE "
                      "FROM VACCINATIONS VACC "
                      "JOIN VACCINS V ON V.IDV = VACC.IDV "
                      "WHERE VACC.IDP = :idp "
                      "ORDER BY VACC.DATE_DE_VACCINATION");
        query.bindValue(":idp", patientId);

        if (query.exec()) {
            int row = 0;
            while (query.next()) {
                table->insertRow(row);
                table->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));

                QDate vaccDate = query.value(1).toDate();
                table->setItem(row, 1, new QTableWidgetItem(vaccDate.toString("dd/MM/yyyy")));

                int totalDoses = query.value(2).toInt();
                table->setItem(row, 2, new QTableWidgetItem(QString::number(totalDoses)));

                if (totalDoses > 1) {
                    QDate nextDoseDate = vaccDate.addDays(30);
                    table->setItem(row, 3, new QTableWidgetItem(nextDoseDate.toString("dd/MM/yyyy")));
                    if (QDate::currentDate().daysTo(nextDoseDate) <= 7) {
                        for (int col = 0; col < 4; col++) {
                            table->item(row, col)->setBackground(QColor(255, 255, 150));
                        }
                    }
                } else {
                    table->setItem(row, 3, new QTableWidgetItem("Complet"));
                }
                row++;
            }
        } else {
            QMessageBox::warning(dialog, "Erreur", "Impossible de charger l'historique: " + query.lastError().text());
        }
    };

    loadVaccinationData();

    // Add button to create new vaccination
    QPushButton *addBtn = new QPushButton("Ajouter Vaccination", dialog);
    addBtn->setStyleSheet(
        "QPushButton { background-color: #198754; color: white; padding: 8px 25px; border-radius: 8px; font-size: 10px; }"
        "QPushButton:hover { background-color: #157347; }");

    connect(addBtn, &QPushButton::clicked, this, [=]() {
        QDialog addDialog(dialog);
        addDialog.setWindowTitle("Nouvelle Vaccination");
        QFormLayout *form = new QFormLayout(&addDialog);

        QComboBox *vaccinCombo = new QComboBox();
        QSqlQuery vaccinQuery("SELECT IDV, NOMVACCIN, NBDOSE FROM VACCINS");
        while (vaccinQuery.next()) {
            vaccinCombo->addItem(
                vaccinQuery.value(1).toString() + " (" + vaccinQuery.value(2).toString() + " doses)",
                vaccinQuery.value(0));
        }

        QDateEdit *dateEdit = new QDateEdit(QDate::currentDate());
        dateEdit->setCalendarPopup(true);
        dateEdit->setDisplayFormat("dd/MM/yyyy");

        form->addRow("Vaccin:", vaccinCombo);
        form->addRow("Date:", dateEdit);

        QPushButton *saveBtn = new QPushButton("Enregistrer");
        form->addRow(saveBtn);

        connect(saveBtn, &QPushButton::clicked, this, [=, &addDialog]() {
            int idv = vaccinCombo->currentData().toInt();
            QDate date = dateEdit->date();

            QSqlQuery checkQuery;
            checkQuery.prepare("SELECT COUNT(*) FROM VACCINATIONS WHERE IDP = :idp AND IDV = :idv");
            checkQuery.bindValue(":idp", patientId);
            checkQuery.bindValue(":idv", idv);

            if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
                QMessageBox::warning(&addDialog, "Attention", "Ce patient a déjà reçu ce vaccin!");
                return;
            }

            QSqlQuery insert;
            insert.prepare("INSERT INTO VACCINATIONS (IDP, IDV, DATE_DE_VACCINATION) VALUES (:idp, :idv, :date)");
            insert.bindValue(":idp", patientId);
            insert.bindValue(":idv", idv);
            insert.bindValue(":date", date);

            if (insert.exec()) {
                QSqlQuery dosesQuery;
                dosesQuery.prepare("SELECT NBDOSE FROM VACCINS WHERE IDV = :idv");
                dosesQuery.bindValue(":idv", idv);

                if (dosesQuery.exec() && dosesQuery.next()) {
                    int totalDoses = dosesQuery.value(0).toInt();
                    if (totalDoses > 1) {
                        QDate nextDoseDate = date.addDays(30);

                        QSqlQuery patientQuery;
                        patientQuery.prepare("SELECT NOM, PRENOM, NUMTEL FROM PATIENTS WHERE IDP = :idp");
                        patientQuery.bindValue(":idp", patientId);

                        if (patientQuery.exec() && patientQuery.next()) {
                            QString patientName = patientQuery.value(0).toString() + " " + patientQuery.value(1).toString();
                            QString phoneNumber = patientQuery.value(2).toString();
                            QString vaccineName = vaccinCombo->currentText().split(" (").first();

                            sendVaccinationSMS(phoneNumber, patientName, vaccineName, nextDoseDate);
                        }
                    }
                }

                QMessageBox::information(&addDialog, "Succès", "Vaccination enregistrée!");
                addDialog.accept();
                loadVaccinationData();
            } else {
                QMessageBox::critical(&addDialog, "Erreur", "Échec de l'enregistrement: " + insert.lastError().text());
            }
        });

        addDialog.exec();
    });

    layout->addWidget(table);
    layout->addWidget(addBtn);
    dialog->exec();
}












void MainWindow::setupaddPatientsFormPage() {
    addPatientsFormPage = new QWidget();
    QVBoxLayout *formLayout = new QVBoxLayout(addPatientsFormPage);

    QLabel *titleLabel = new QLabel("Ajout d'un Patient");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
    formLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    // Create input fields
    QLabel *nomLabel = new QLabel("Nom*:");
    QLineEdit *nomInput = new QLineEdit();
    nomInput->setPlaceholderText("Entrez le nom");
    nomInput->setValidator(new QRegularExpressionValidator(QRegularExpression("[A-Za-zÀ-ÿ\\s'-]+"), nomInput)); // Allows letters, spaces, apostrophes and hyphens

    QLabel *prenomLabel = new QLabel("Prénom*:");
    QLineEdit *prenomInput = new QLineEdit();
    prenomInput->setPlaceholderText("Entrez le prénom");
    prenomInput->setValidator(new QRegularExpressionValidator(QRegularExpression("[A-Za-zÀ-ÿ\\s'-]+"), prenomInput));

    QLabel *sexeLabel = new QLabel("Sexe*:");
    QComboBox *sexeInput = new QComboBox();
    sexeInput->addItem("Homme");
    sexeInput->addItem("Femme");

    QLabel *cinLabel = new QLabel("CIN*:");
    QLineEdit *cinInput = new QLineEdit();
    cinInput->setPlaceholderText("Entrez le CIN (8 chiffres)");
    cinInput->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d{8}"), cinInput)); // Exactly 8 digits
    cinInput->setMaxLength(8);

    QLabel *adresseLabel = new QLabel("Gouvernorat*:");
    QComboBox *adresseInput = new QComboBox();
    adresseInput->addItems({
        "Ariana", "Beja", "Ben Arous", "Bizerte", "Gabes", "Gafsa", "Jendouba", "Kairouan",
        "Kasserine", "Kebili", "Kef", "Mahdia", "Manouba", "Medenine", "Monastir", "Nabeul",
        "Sfax", "Sidi Bouzid", "Siliana", "Tataouine", "Tozeur", "Tunis", "Zaghouan"
    });

    QLabel *numtelLabel = new QLabel("Numéro de Téléphone*:");
    QLineEdit *numtelInput = new QLineEdit();
    numtelInput->setPlaceholderText("Entrez le numéro (8 chiffres)");
    numtelInput->setValidator(new QRegularExpressionValidator(QRegularExpression("[259]\\d{7}"), numtelInput)); // Starts with 2,5 or 9 followed by 7 digits
    numtelInput->setMaxLength(8);

    QLabel *maladiechroniqueLabel = new QLabel("Maladie Chronique:");
    QLineEdit *maladiechroniqueInput = new QLineEdit();
    maladiechroniqueInput->setPlaceholderText("Facultatif");

    // Set default values
    sexeInput->setCurrentIndex(0);
    adresseInput->setCurrentIndex(0);

    // Create form layout
    QFormLayout *inputLayout = new QFormLayout();
    inputLayout->addRow(nomLabel, nomInput);
    inputLayout->addRow(prenomLabel, prenomInput);
    inputLayout->addRow(sexeLabel, sexeInput);
    inputLayout->addRow(cinLabel, cinInput);
    inputLayout->addRow(adresseLabel, adresseInput);
    inputLayout->addRow(numtelLabel, numtelInput);
    inputLayout->addRow(maladiechroniqueLabel, maladiechroniqueInput);
    formLayout->addLayout(inputLayout);

    // Create buttons
    QPushButton *submitButton = new QPushButton("Ajouter");
    QPushButton *backButton = new QPushButton("Retour");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(submitButton);
    formLayout->addLayout(buttonLayout);

    addPatientsFormPage->setStyleSheet(
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

    connect(submitButton, &QPushButton::clicked, this, [this, nomInput, prenomInput, sexeInput, cinInput, adresseInput, numtelInput, maladiechroniqueInput]() {
        if (!patientsManager) {
            QMessageBox::critical(nullptr, "Erreur", "Système non initialisé!");
            return;
        }

        // Validate each field with specific messages
        if (nomInput->text().trimmed().isEmpty()) {
            QMessageBox::warning(nullptr, "Validation", "Le nom est obligatoire!");
            nomInput->setFocus();
            return;
        }

        if (prenomInput->text().trimmed().isEmpty()) {
            QMessageBox::warning(nullptr, "Validation", "Le prénom est obligatoire!");
            prenomInput->setFocus();
            return;
        }

        if (cinInput->text().trimmed().isEmpty() || cinInput->text().length() != 8) {
            QMessageBox::warning(nullptr, "Validation", "Le CIN doit contenir exactement 8 chiffres!");
            cinInput->setFocus();
            return;
        }

        if (numtelInput->text().trimmed().isEmpty() || numtelInput->text().length() != 8 ||
            !(numtelInput->text().startsWith('2') || numtelInput->text().startsWith('5') || numtelInput->text().startsWith('9'))) {
            QMessageBox::warning(nullptr, "Validation", "Le numéro de téléphone doit commencer par 2, 5 ou 9 et contenir 8 chiffres!");
            numtelInput->setFocus();
            return;
        }

        // All fields are valid, proceed with adding patient
        patientsManager->setNom(nomInput->text());
        patientsManager->setprenom(prenomInput->text());
        patientsManager->setsexe(sexeInput->currentText());
        patientsManager->setcin(cinInput->text());
        patientsManager->setadresse(adresseInput->currentText());
        patientsManager->setnumtel(numtelInput->text());
        patientsManager->setmaladiechronique(maladiechroniqueInput->text());

        patientsManager->addPatient(
            patientsManager->getNom(),
            patientsManager->getprenom(),
            patientsManager->getsexe(),
            patientsManager->getcin(),
            patientsManager->getadresse(),
            patientsManager->getnumtel(),
            patientsManager->getmaladiechronique()
            );

        patientsManager->loadPatients();
        stackedWidget->setCurrentWidget(PatientsTablePage);

        // Clear form for next entry
        nomInput->clear();
        prenomInput->clear();
        cinInput->clear();
        numtelInput->clear();
        maladiechroniqueInput->clear();
        sexeInput->setCurrentIndex(0);
        adresseInput->setCurrentIndex(0);
    });

    connect(backButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(PatientsTablePage);
    });
}





void MainWindow::setupModifyPatientFormPage(const int ID) {
    ModifyPatientFormPage = new QWidget();
    QVBoxLayout *formLayout = new QVBoxLayout(ModifyPatientFormPage);

    QLabel *titleLabel = new QLabel("Modifier Patient");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
    formLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    // Labels and inputs (same as add patient form)
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
    QLabel *maladiechroniqueLabel = new QLabel("Maladie Chronique:");
    QLineEdit *maladiechroniqueInput = new QLineEdit();

    // Load existing patient data
    loadPatientData(ID, nomInput, prenomInput, sexeInput, cinInput, adresseInput, numtelInput, maladiechroniqueInput);

    QFormLayout *inputLayout = new QFormLayout();
    inputLayout->addRow(nomLabel, nomInput);
    inputLayout->addRow(prenomLabel, prenomInput);
    inputLayout->addRow(sexeLabel, sexeInput);
    inputLayout->addRow(cinLabel, cinInput);
    inputLayout->addRow(adresseLabel, adresseInput);
    inputLayout->addRow(numtelLabel, numtelInput);
    inputLayout->addRow(maladiechroniqueLabel, maladiechroniqueInput);
    formLayout->addLayout(inputLayout);

    QPushButton *submitButton = new QPushButton("Modifier");
    QPushButton *backButton = new QPushButton("Retour");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(submitButton);
    formLayout->addLayout(buttonLayout);

    // Set styles (same as before)
    ModifyPatientFormPage->setStyleSheet(
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

    // Connect submit button to modify patient slot
    connect(submitButton, &QPushButton::clicked, this, [this, ID, nomInput, prenomInput, sexeInput, cinInput, adresseInput, numtelInput, maladiechroniqueInput](){
        onModifyPatientSubmit(ID, nomInput, prenomInput, sexeInput, cinInput, adresseInput, numtelInput, maladiechroniqueInput);
        //setCentralWidget(patientsPage);
        stackedWidget->setCurrentWidget(PatientsTablePage);
    });

    connect(backButton, &QPushButton::clicked, this, [this]() {
        // setCentralWidget(patientsPage);
        stackedWidget->setCurrentWidget(PatientsTablePage);
    });
}














// Tools-related slots
void MainWindow::setupToolsPage() {
    QVBoxLayout *toolsLayout = new QVBoxLayout(toolsPage);
    toolsLayout->setSpacing(20);

    // Add statistics section at the top
    ToolstatsContainer = new QWidget();
    ToolstatsContainer->setStyleSheet(R"(
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

    QHBoxLayout *statsLayout = new QHBoxLayout(ToolstatsContainer);
    statsLayout->setContentsMargins(0, 0, 0, 0);
    statsLayout->setSpacing(10);

    // Setup the statistics charts
    setupStatisticsSection();
    statsLayout->addWidget(pieChartView, 1);
    statsLayout->addWidget(barChartView, 1);


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


    toolsLayout->addWidget(ToolstatsContainer);
    toolsLayout->addWidget(goToToolsTableButton, 0, Qt::AlignCenter);


    connect(goToToolsTableButton, &QPushButton::clicked, this, &MainWindow::showToolsTablePage);


}
void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);

    // Refresh statistics when tools page is shown
    if (stackedWidget->currentWidget() == toolsPage) {
        calculateStatistics();
        ToolsUpdateStatistics();
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
void MainWindow::ToolsUpdateStatistics() {
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


    // Top bar with back button and add button
    QHBoxLayout *topBarLayout = new QHBoxLayout();

    // Back button
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

    // Add button
    QPushButton *addButton = new QPushButton("Ajouter Ressources", this);
    addButton->setStyleSheet(
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

    topBarLayout->addWidget(backButton);
    topBarLayout->addStretch();
    topBarLayout->addWidget(addButton);
    tableLayout->addLayout(topBarLayout);

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
    QStringList headers = {"Nom Matériel", "Catégorie","Description", "Stock", "Stock Max","Image","Fournisseur","Position Fournisseur", "Nom Projet", "Actions"};
    int columnWidths[] = {12, 13, 12,8, 8,10,7,10, 10, 25}; // percentages

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
    connect(toolsManager, &ToolsManager::dataChanged, this, &MainWindow::ToolsUpdateStatistics);

    // Initial data load
    calculateStatistics();
    ToolsUpdateStatistics();
    // Right side - Form Widget

    // Add buttons to a horizontal layout



    // Add both sections to the main layout with a splitter
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(tableWidget);

    splitter->setStretchFactor(0, 2);  // Table gets more space
        // Form gets less space
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

    // Connect the add button to show the add tool page
    connect(addButton, &QPushButton::clicked, this, &MainWindow::showAddToolPage);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showToolsPage);
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

void MainWindow::showToolsPage() {
    stackedWidget->setCurrentWidget(toolsPage);
    updateSidebarIcons(btnTools);
    calculateStatistics();  // Refresh data when coming back
    ToolsUpdateStatistics();
}

void MainWindow::showToolsTablePage() {
    stackedWidget->setCurrentWidget(toolsTablePage);
}

void MainWindow::setupAddToolPage() {
    // Create the add tool page widget
    addToolPage = new QWidget();
    stackedWidget->addWidget(addToolPage);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(addToolPage);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // Top bar with back button
    QHBoxLayout *topBarLayout = new QHBoxLayout();
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
    topBarLayout->addWidget(backButton);
    topBarLayout->addStretch();
    mainLayout->addLayout(topBarLayout);

    // Form container
    QWidget *formContainer = new QWidget();
    QHBoxLayout *formLayout = new QHBoxLayout(formContainer);
    formLayout->setSpacing(30);

    // Left side inputs
    QWidget *leftInputs = new QWidget();
    QFormLayout *leftForm = new QFormLayout(leftInputs);
    leftForm->setSpacing(15);

    // Right side inputs
    QWidget *rightInputs = new QWidget();
    QFormLayout *rightForm = new QFormLayout(rightInputs);
    rightForm->setSpacing(15);

    // Create input fields
    QLineEdit *nomMaterielInput = new QLineEdit();
    QComboBox *categorieCombo = new QComboBox();
    categorieCombo->addItem(""); // Optional empty item
    categorieCombo->addItems(toolsManager->getCategories());
    categorieCombo->setEditable(false);
    QLineEdit *descriptionInput = new QLineEdit();
    QSpinBox *stockInput = new QSpinBox();
    stockInput->setMinimum(0);
    QSpinBox *quantiteMaximaleInput = new QSpinBox();
    quantiteMaximaleInput->setMinimum(0);
    QPushButton *uploadImageButton = new QPushButton("Upload Image");
    QLineEdit *fournisseurCombo = new QLineEdit();
    QLineEdit *positionFournisseurInput = new QLineEdit();

    // Style the inputs
    QString inputStyle =
        "QLineEdit, QSpinBox, QComboBox {"
        "    padding: 8px;"
        "    border: 1px solid #ddd;"
        "    border-radius: 4px;"
        "}"
        "QPushButton {"
        "    background-color: #198754;"
        "    color: white;"
        "    padding: 8px 16px;"
        "    border-radius: 4px;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: #157347;"
        "}";

    nomMaterielInput->setStyleSheet(inputStyle);
    categorieCombo->setStyleSheet(inputStyle);
    descriptionInput->setStyleSheet(inputStyle);
    stockInput->setStyleSheet(inputStyle);
    quantiteMaximaleInput->setStyleSheet(inputStyle);
    uploadImageButton->setStyleSheet(inputStyle);
    fournisseurCombo->setStyleSheet(inputStyle);
    positionFournisseurInput->setStyleSheet(inputStyle);

    // Add fields to left form
    leftForm->addRow("Nom Materiel:", nomMaterielInput);
    leftForm->addRow("Catégorie:", categorieCombo);
    leftForm->addRow("Description:", descriptionInput);
    leftForm->addRow("Stock:", stockInput);

    // Add fields to right form
    rightForm->addRow("Quantité Maximale:", quantiteMaximaleInput);
    rightForm->addRow("Image:", uploadImageButton);
    rightForm->addRow("Fournisseur:", fournisseurCombo);
    rightForm->addRow("Position Fournisseur:", positionFournisseurInput);

    // Add forms to the container
    formLayout->addWidget(leftInputs);
    formLayout->addWidget(rightInputs);

    // Add form container to main layout
    mainLayout->addWidget(formContainer);

    // Add buttons at the bottom
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *clearButton = new QPushButton("Effacer");
    QPushButton *submitButton = new QPushButton("Ajouter");

    clearButton->setStyleSheet(inputStyle);
    submitButton->setStyleSheet(inputStyle);

    buttonLayout->addStretch();
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(submitButton);
    mainLayout->addLayout(buttonLayout);

    // Connect the back button
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showToolsTablePage);

    // Connect the upload button
    auto imagePath = std::make_shared<QString>();
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
    connect(submitButton, &QPushButton::clicked, this, [this, nomMaterielInput, categorieCombo, descriptionInput, stockInput, quantiteMaximaleInput, fournisseurCombo, positionFournisseurInput, imagePath, submitButton, uploadImageButton]() {
        // Get input values
        QString nomMateriel = nomMaterielInput->text();
        QString categorie = categorieCombo->currentText();
        QString description = descriptionInput->text();
        int stock = stockInput->value();
        int quantiteMaximale = quantiteMaximaleInput->value();
        QString fournisseur = fournisseurCombo->text();
        QString positionFournisseur = positionFournisseurInput->text();

        // Set values in ToolsManager
        toolsManager->setNomMateriel(nomMateriel);
        toolsManager->setCategorie(categorie);
        toolsManager->setDescription(description);
        toolsManager->setStock(stock);
        toolsManager->setQuantiteMaximale(quantiteMaximale);
        toolsManager->setUploadImage(*imagePath);
        toolsManager->setFournisseur(fournisseur);
        toolsManager->setPositionFournisseur(positionFournisseur);

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
                fournisseurCombo->clear();
                positionFournisseurInput->clear();
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
                fournisseurCombo->clear();
                positionFournisseurInput->clear();
            }
        }
    });

    // Connect the clear button
    connect(clearButton, &QPushButton::clicked, this, [nomMaterielInput, categorieCombo, descriptionInput, stockInput, quantiteMaximaleInput, uploadImageButton, fournisseurCombo, positionFournisseurInput, submitButton, this]() {
        nomMaterielInput->clear();
        categorieCombo->setCurrentIndex(-1);
        descriptionInput->clear();
        stockInput->setValue(0);
        quantiteMaximaleInput->setValue(0);
        uploadImageButton->setText("Upload Image");
        fournisseurCombo->clear();
        positionFournisseurInput->clear();
        submitButton->setText("Ajouter");
        toolsManager->setId(-1);
    });

    // Connect edit signal from ToolsManager
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

                // Handle category
                QString currentCategory = toolData["categorie"].toString();
                int catIndex = categorieCombo->findText(currentCategory);
                if (catIndex >= 0) {
                    categorieCombo->setCurrentIndex(catIndex);
                } else {
                    categorieCombo->addItem(currentCategory);
                    categorieCombo->setCurrentIndex(categorieCombo->count()-1);
                }

                descriptionInput->setText(toolData["description"].toString());
                stockInput->setValue(toolData["stock"].toInt());
                quantiteMaximaleInput->setValue(toolData["quantiteMaximale"].toInt());
                fournisseurCombo->setText(toolData["fournisseur"].toString());
                positionFournisseurInput->setText(toolData["positionFournisseur"].toString());

                // Handle image
                QString imagePath = toolData["uploadImage"].toString();
                if (!imagePath.isEmpty()) {
                    uploadImageButton->setText("Image sélectionnée");
                }

                // Update submit button
                submitButton->setText("Modifier");
                toolsManager->setId(id); // Set the ID for editing

                // Show the add tool page
                showAddToolPage();
            });
}

void MainWindow::showAddToolPage() {
    stackedWidget->setCurrentWidget(addToolPage);
}


//_____________________end tools_______________
void MainWindow::loadPatientData(int patientID, QLineEdit *nomInput, QLineEdit *prenomInput, QComboBox *sexeInput, QLineEdit *cinInput, QComboBox *adresseInput, QLineEdit *numtelInput, QLineEdit *maladiechroniqueInput) {
    QSqlQuery query;
    query.prepare("SELECT Nom, Prenom, Sexe, CIN, Adresse, NumTel, MaladieChronique FROM patients WHERE IDP = :IDP");
    query.bindValue(":IDP", patientID);

    if (query.exec()) {
        if (query.next()) {
            nomInput->setText(query.value("Nom").toString());
            prenomInput->setText(query.value("Prenom").toString());
            sexeInput->setCurrentText(query.value("Sexe").toString());
            cinInput->setText(query.value("CIN").toString());
            adresseInput->setCurrentText(query.value("Adresse").toString());
            numtelInput->setText(query.value("NumTel").toString());
            maladiechroniqueInput->setText(query.value("MaladieChronique").toString());
        }
    } else {
        qDebug() << "Error loading patient data:" << query.lastError().text();
    }
}



bool MainWindow::patientExists(int patientID) {
    QSqlQuery query;
    query.prepare("SELECT id FROM patients WHERE id = :id");
    query.bindValue(":id", patientID);

    qDebug() << "SQL Query: " << query.lastQuery(); // Debug the query to see the full query being executed

    if (!query.exec()) {
        qDebug() << "❌ Error checking patient existence:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        qDebug() << "🟢 Patient ID" << patientID << "exists in database";
        return true;
    }

    qDebug() << "❌ Patient ID" << patientID << "does not exist in database";
    return false;
}





void MainWindow::onAddPatientClicked() {
    stackedWidget->setCurrentWidget(addPatientsFormPage);
}

void MainWindow::onEditPatientClicked() {
    bool ok;
    int patientID = QInputDialog::getInt(this,
                                         "Modifier Patient",
                                         "Entrez l'ID du patient à modifier:",
                                         1, 1, 10000, 1, &ok);

    if (ok) {
        qDebug() << "🟢 Recherche du patient avec IDP:" << patientID;

        // Vérifier si le patient existe dans la base de données
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM patients WHERE IDP = :id");
        query.bindValue(":id", patientID);

        if (query.exec()) {
            if (query.next()) {
                int count = query.value(0).toInt();

                if (count > 0) {
                    // Patient existe, procéder à la modification
                    qDebug() << "🟢 Patient trouvé, préparation du formulaire";
                    setupModifyPatientFormPage(patientID);

                    if (ModifyPatientFormPage) {
                        stackedWidget->addWidget(ModifyPatientFormPage);
                        stackedWidget->setCurrentWidget(ModifyPatientFormPage);
                        qDebug() << "🟢 Affichage du formulaire de modification";
                    } else {
                        qDebug() << "❌ Erreur: ModifyPatientFormPage non créé";
                        QMessageBox::critical(this, "Erreur", "Le formulaire de modification n'a pas pu être chargé.");
                    }
                } else {
                    qDebug() << "❌ Aucun patient avec IDP:" << patientID;
                    QMessageBox::warning(this, "ID Invalide", "Aucun patient avec cet ID trouvé.");

                    // Debug: Afficher quelques IDs existants
                    QSqlQuery idQuery("SELECT IDP FROM patients LIMIT 5");
                    QStringList existingIds;
                    while (idQuery.next()) {
                        existingIds << idQuery.value(0).toString();
                    }
                    qDebug() << "IDs existants (5 premiers):" << existingIds.join(", ");
                }
            }
        } else {
            qDebug() << "❌ Erreur de requête:" << query.lastError().text();
            QMessageBox::critical(this, "Erreur Base de Données",
                                  "Erreur lors de la vérification du patient:\n" + query.lastError().text());
        }
    }
}


void MainWindow::onDeletePatientClicked() {
    bool ok;
    int id = QInputDialog::getInt(this, "Supprimer Patient", "Entrez l'ID du patient à supprimer:", 1, 1, 10000, 1, &ok);
    if (ok) {
        patientsManager->deletePatient(id);
    }
}

void MainWindow::onModifyPatientSubmit(int patientID, QLineEdit *nomInput, QLineEdit *prenomInput, QComboBox *sexeInput, QLineEdit *cinInput, QComboBox *adresseInput, QLineEdit *numtelInput, QLineEdit *maladiechroniqueInput) {
    QString nom = nomInput->text();
    QString prenom = prenomInput->text();
    QString sexe = sexeInput->currentText();
    QString cin = cinInput->text();
    QString adresse = adresseInput->currentText();
    QString numtel = numtelInput->text();
    QString maladiechronique = maladiechroniqueInput->text();

    if (nom.isEmpty() || prenom.isEmpty() || cin.isEmpty() || numtel.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please fill in all required fields.");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE patients SET Nom = :Nom, Prenom = :Prenom, Sexe = :Sexe, CIN = :CIN, Adresse = :Adresse, NumTel = :NumTel, MaladieChronique = :MaladieChronique WHERE IDP = :IDP");

    query.bindValue(":Nom", nom);
    query.bindValue(":Prenom", prenom);
    query.bindValue(":Sexe", sexe);
    query.bindValue(":CIN", cin);
    query.bindValue(":Adresse", adresse);
    query.bindValue(":NumTel", numtel);
    query.bindValue(":MaladieChronique", maladiechronique);
    query.bindValue(":IDP", patientID);

    if (!query.exec()) {
        qDebug() << "Error updating patient:" << query.lastError().text();
    } else {
        QMessageBox::information(this, "Success", "Patient information updated successfully!");
    }
    patientsManager->loadPatients();
}












void MainWindow::setupVaccinsPage() {
    qDebug() << "Initialisation dynamique de la page vaccins";

    // Initialisation de la page si nécessaire
    if (!vaccinsPage) {
        vaccinsPage = new QWidget(this);
    }

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(vaccinsPage);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // 1. En-tête avec titre
    QLabel *titleLabel = new QLabel("Tableau de Bord des Vaccins", vaccinsPage);
    titleLabel->setStyleSheet("QLabel {"
                              "font-size: 24px;"
                              "font-weight: bold;"
                              "color: #2c3e50;"
                              "padding-bottom: 10px;"
                              "border-bottom: 2px solid #3498db;"
                              "}");
    mainLayout->addWidget(titleLabel);

    // 2. Section "Bibliothèque des Vaccins"
    QLabel *libraryLabel = new QLabel("Bibliothèque des Vaccins", vaccinsPage);
    libraryLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #34495e;");
    mainLayout->addWidget(libraryLabel);

    // 3. Conteneur pour stats et notifications
    QWidget *statsNotifContainer = new QWidget(vaccinsPage);
    QHBoxLayout *statsNotifLayout = new QHBoxLayout(statsNotifContainer);
    statsNotifLayout->setSpacing(20);
    statsNotifLayout->setContentsMargins(0, 0, 0, 0);

    // 3.1 Carte des statistiques (2/3 de largeur)
    QGroupBox *statsGroup = new QGroupBox("Répartition par Type de Vaccin", statsNotifContainer);
    statsGroup->setStyleSheet(groupBoxStyle("#3498db"));

    QVBoxLayout *statsLayout = new QVBoxLayout(statsGroup);

    QWidget *statsContent = new QWidget();
    statsContent->setObjectName("statsContent");
    statsLayout->addWidget(statsContent);

    // 3.2 Carte des notifications (1/3 de largeur)
    QGroupBox *notifGroup = new QGroupBox("Notifications", statsNotifContainer);
    notifGroup->setStyleSheet(groupBoxStyle("#e74c3c"));

    QVBoxLayout *notifLayout = new QVBoxLayout(notifGroup);

    // Créer le placeholder pour les notifications
    QWidget *notifContent = new QWidget();
    notifContent->setObjectName("notificationsPlaceholder");
    notifLayout->addWidget(notifContent);

    statsNotifLayout->addWidget(statsGroup, 2); // 2/3 de l'espace
    statsNotifLayout->addWidget(notifGroup, 1); // 1/3 de l'espace
    mainLayout->addWidget(statsNotifContainer);

    // 4. Section "Liste Comptée des Vaccins"
    QLabel *listLabel = new QLabel("Liste Comptée des Vaccins", vaccinsPage);
    listLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #34495e;");
    mainLayout->addWidget(listLabel);

    // 5. Tableau dynamique des vaccins
    QTableWidget *vaccinsTable = new QTableWidget(vaccinsPage);
    vaccinsTable->setColumnCount(8);
    vaccinsTable->setHorizontalHeaderLabels({"ID", "Nom", "Référence", "Type", "Doses", "Quantité", "Expiration", "Couleur"});
    vaccinsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vaccinsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    vaccinsTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Style du tableau
    vaccinsTable->setStyleSheet(tableStyle());
    mainLayout->addWidget(vaccinsTable);


    // 7. Boutons d'action en bas de page
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *viewAllBtn = new QPushButton("Afficher Plus", vaccinsPage);
    viewAllBtn->setStyleSheet(buttonStyle("#2ecc71"));
    viewAllBtn->setCursor(Qt::PointingHandCursor);
    connect(viewAllBtn, &QPushButton::clicked, this, &MainWindow::showVaccinsTablePage);

    QPushButton *pdfBtn = new QPushButton("Exporter PDF", vaccinsPage);
    pdfBtn->setStyleSheet(buttonStyle("#3498db"));
    pdfBtn->setCursor(Qt::PointingHandCursor);
    connect(pdfBtn, &QPushButton::clicked, this, &MainWindow::exportVaccinsTableToPDF);

    buttonLayout->addWidget(viewAllBtn);
    buttonLayout->addWidget(pdfBtn);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    // Initialisation du gestionnaire de vaccins - IMPORTANT
    vaccinManager = new VaccinManager(vaccinsTable, this);
    vaccinManager->loadVaccins();

    // Affichage des stats et notifications
    displayVaccinTypeStats(statsContent);
    loadNotifications();

    // Timer pour rafraîchissement automatique
    QTimer *refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, [this]() {
        vaccinManager->loadVaccins();
        updateVaccinTypeStats();
        loadNotifications();
    });
    refreshTimer->start(30000); // 30 secondes

    qDebug() << "Page vaccins initialisée avec succès";
}

QString MainWindow::groupBoxStyle(const QString &color) {
    return QString(
               "QGroupBox {"
               "border: 2px solid %1;"
               "border-radius: 8px;"
               "margin-top: 10px;"
               "padding-top: 15px;"
               "background: white;"
               "}"
               "QGroupBox::title {"
               "subcontrol-origin: margin;"
               "left: 10px;"
               "padding: 0 5px;"
               "color: %1;"
               "font-weight: bold;"
               "}").arg(color);
}

QString MainWindow::tableStyle() {
    return QString(
        "QTableWidget {"
        "background-color: white;"
        "border: 1px solid #ddd;"
        "border-radius: 5px;"
        "}"
        "QHeaderView::section {"
        "background-color: #3498db;"
        "color: white;"
        "padding: 10px;"
        "border: none;"
        "font-weight: bold;"
        "}"
        "QTableWidget::item {"
        "padding: 8px;"
        "border-bottom: 1px solid #eee;"
        "}"
        "QTableWidget::item:selected {"
        "background-color: #d6eaf8;"
        "}");
}

QString MainWindow::buttonStyle(const QString &color) {
    return QString(
               "QPushButton {"
               "background-color: %1;"
               "color: white;"
               "border: none;"
               "border-radius: 5px;"
               "padding: 8px 16px;"
               "font-weight: bold;"
               "min-width: 100px;"
               "}"
               "QPushButton:hover {"
               "background-color: %2;"
               "}").arg(color, QColor(color).darker(120).name());
}



void MainWindow::styleNotificationsWidget(QWidget *notificationsWidget) {
    // Basic transparent background
    notificationsWidget->setStyleSheet("QWidget { background-color: transparent; }");

    // Find all child widgets
    QList<QWidget*> children = notificationsWidget->findChildren<QWidget*>();
    for (QWidget *child : children) {
        // Style notification frames
        if (QFrame *frame = qobject_cast<QFrame*>(child)) {
            QString backgroundColor, borderColor;

            // Set colors based on notification type
            if (frame->property("notificationType") == "warning") {
                backgroundColor = "#f8f9fa";  // Light gray background
                borderColor = "#ffc107";      // Warning yellow
            } else if (frame->property("notificationType") == "expiry") {
                backgroundColor = "#f8f9fa";
                borderColor = "#dc3545";      // Danger red
            } else if (frame->property("notificationType") == "info") {
                backgroundColor = "#f8f9fa";
                borderColor = "#17a2b8";      // Info blue
            } else {
                backgroundColor = "#f8f9fa";
                borderColor = "#6c757d";      // Default gray
            }

            // Apply simpler frame style
            frame->setStyleSheet(QString(
                                     "QFrame {"
                                     "  background-color: %1;"
                                     "  border-left: 4px solid %2;"
                                     "  border-radius: 2px;"
                                     "  margin-bottom: 8px;"
                                     "}"
                                     ).arg(backgroundColor, borderColor));

            // Remove shadow effect to simplify
            frame->setGraphicsEffect(nullptr);
        }

        // Style notification labels
        if (QLabel *label = qobject_cast<QLabel*>(child)) {
            QColor textColor;

            if (label->property("notificationType") == "warning") {
                textColor = QColor("#ffc107");  // Warning yellow
            } else if (label->property("notificationType") == "expiry") {
                textColor = QColor("#dc3545");  // Danger red
            } else if (label->property("notificationType") == "info") {
                textColor = QColor("#17a2b8");  // Info blue
            } else {
                textColor = QColor("#333333");  // Default dark gray
            }

            // Apply simpler label style
            label->setStyleSheet(QString(
                                     "QLabel {"
                                     "  color: %1;"
                                     "  font-weight: bold;"
                                     "  font-size: 13px;"
                                     "  font-family: 'Segoe UI', sans-serif;"
                                     "  padding: 8px;"
                                     "}"
                                     ).arg(textColor.name()));
        }

        // Style description text
        if (QLabel *textLabel = qobject_cast<QLabel*>(child)) {
            if (!textLabel->property("notificationType").isValid()) {
                textLabel->setStyleSheet(
                    "QLabel {"
                    "  border: none;"
                    "  background-color: transparent;"
                    "  padding: 4px 8px 8px 8px;"
                    "  font-size: 12px;"
                    "  font-family: 'Segoe UI', sans-serif;"
                    "  color: #212529;"
                    "}"
                    );
            }
        }
    }
}

void MainWindow::applyAnimatedContainerStyle(QWidget *container) {
    // Style de base pour le conteneur des notifications
    container->setObjectName("notificationsContainer");
    container->setStyleSheet(
        "#notificationsContainer {"
        "  background-color: #e8f5ff;"
        "  border: 2px solid #2185d0;"
        "  border-radius: 10px;"
        "  padding: 5px;"
        "  margin: 10px;"
        "  box-shadow: 0 4px 8px rgba(33, 133, 208, 0.3);"
        "}"
        );

    // Ajouter l'effet de survol avec QGraphicsDropShadowEffect
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(container);
    shadowEffect->setBlurRadius(12);
    shadowEffect->setColor(QColor(33, 133, 208, 120));
    shadowEffect->setOffset(0, 0);
    container->setGraphicsEffect(shadowEffect);

    // Animation de l'effet d'ombre - plus lente et avec moins d'amplitude
    QPropertyAnimation *animation = new QPropertyAnimation(shadowEffect, "blurRadius");
    animation->setDuration(3500);  // Plus lent (3.5 secondes)
    animation->setStartValue(10);  // Valeur de départ plus élevée
    animation->setEndValue(14);    // Valeur finale plus basse (moins d'amplitude)
    animation->setLoopCount(-1);   // Animation en boucle
    animation->setEasingCurve(QEasingCurve::InOutSine); // Courbe plus douce

    // Animation pour alterner entre deux directions - plus lente et avec moins d'amplitude
    QPropertyAnimation *reverseAnimation = new QPropertyAnimation(shadowEffect, "blurRadius");
    reverseAnimation->setDuration(3500);  // Plus lent (3.5 secondes)
    reverseAnimation->setStartValue(14);  // Valeur de départ plus élevée
    reverseAnimation->setEndValue(10);    // Valeur finale plus basse (moins d'amplitude)
    reverseAnimation->setLoopCount(-1);   // Animation en boucle
    reverseAnimation->setEasingCurve(QEasingCurve::InOutSine); // Courbe plus douce

    // Séquencer les animations
    QSequentialAnimationGroup *group = new QSequentialAnimationGroup(container);
    group->addAnimation(animation);
    group->addAnimation(reverseAnimation);
    group->start();

    // Animation subtile de bordure - avec des changements moins prononcés
    QWidget *borderEffect = new QWidget(container);
    borderEffect->setObjectName("borderEffect");
    borderEffect->resize(container->size());
    borderEffect->setStyleSheet(
        "#borderEffect {"
        "  border: 2px solid rgba(33, 133, 208, 0.6);"
        "  border-radius: 10px;"
        "  background-color: transparent;"
        "}"
        );

    QPropertyAnimation *borderOpacity = new QPropertyAnimation(borderEffect, "styleSheet");
    borderOpacity->setDuration(4000);  // Plus lent (4 secondes)
    borderOpacity->setStartValue("#borderEffect { border: 2px solid rgba(33, 133, 208, 0.5); border-radius: 10px; background-color: transparent; }");
    borderOpacity->setEndValue("#borderEffect { border: 2px solid rgba(33, 133, 208, 0.7); border-radius: 10px; background-color: transparent; }");
    borderOpacity->setLoopCount(-1);
    borderOpacity->setEasingCurve(QEasingCurve::InOutSine);

    QPropertyAnimation *reverseBorderOpacity = new QPropertyAnimation(borderEffect, "styleSheet");
    reverseBorderOpacity->setDuration(4000);  // Plus lent (4 secondes)
    reverseBorderOpacity->setStartValue("#borderEffect { border: 2px solid rgba(33, 133, 208, 0.7); border-radius: 10px; background-color: transparent; }");
    reverseBorderOpacity->setEndValue("#borderEffect { border: 2px solid rgba(33, 133, 208, 0.5); border-radius: 10px; background-color: transparent; }");
    reverseBorderOpacity->setLoopCount(-1);
    reverseBorderOpacity->setEasingCurve(QEasingCurve::InOutSine);

    QSequentialAnimationGroup *borderGroup = new QSequentialAnimationGroup(borderEffect);
    borderGroup->addAnimation(borderOpacity);
    borderGroup->addAnimation(reverseBorderOpacity);
    borderGroup->start();

    // Assurer que l'effet de bordure est au-dessus
    borderEffect->raise();

    // Connecter le redimensionnement du conteneur à l'effet de bordure
    container->installEventFilter(new ResizeEventFilter(borderEffect));
}
void MainWindow::loadNotifications() {
    qDebug() << "🔵 Chargement des notifications...";

    // Si vaccinManager est encore non initialisé, afficher un message d'erreur
    if (!vaccinManager) {
        qDebug() << "🚨 vaccinManager est toujours NULL";
        return;
    }

    // Trouver le placeholder des notifications
    QWidget *notificationsPlaceholder = vaccinsPage->findChild<QWidget*>("notificationsPlaceholder");
    if (!notificationsPlaceholder) {
        qDebug() << "❌ Placeholder des notifications non trouvé";
        return;
    }

    // Créer un widget conteneur avec effet animé pour contenir toutes les notifications
    QWidget *animatedContainer = new QWidget(notificationsPlaceholder);
    QVBoxLayout *animatedLayout = new QVBoxLayout(animatedContainer);
    animatedLayout->setContentsMargins(10, 10, 10, 10);
    animatedLayout->setSpacing(10);

    // Appliquer le style animé au conteneur
    applyAnimatedContainerStyle(animatedContainer);

    // Créer et ajouter les notifications au conteneur animé
    QWidget *notificationsWidget = createNotificationsWidget();
    if (notificationsWidget) {
        // Appliquer un style aux notifications individuelles
        styleNotificationsWidget(notificationsWidget);

        // Ajouter les notifications au layout du conteneur animé
        animatedLayout->addWidget(notificationsWidget);

        // Animation d'entrée pour les notifications
        QPropertyAnimation *entryAnimation = new QPropertyAnimation(notificationsWidget, "opacity");
        entryAnimation->setDuration(800);
        entryAnimation->setStartValue(0.0);
        entryAnimation->setEndValue(1.0);
        entryAnimation->setEasingCurve(QEasingCurve::OutCubic);
        entryAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        // Ajouter le conteneur animé au layout du placeholder
        QVBoxLayout *placeholderLayout = qobject_cast<QVBoxLayout*>(notificationsPlaceholder->layout());
        if (placeholderLayout) {
            // Effacer tout contenu précédent
            QLayoutItem *child;
            while ((child = placeholderLayout->takeAt(0)) != nullptr) {
                delete child->widget();
                delete child;
            }

            placeholderLayout->addWidget(animatedContainer);
        }
    } else {
        qDebug() << "❌ Impossible de créer les notifications.";
    }
}
// Si vous n'avez pas déjà cette fonction, vous devez l'ajouter à votre classe
QProgressBar* MainWindow::createVaccinationRateBar(double percentage) {
    QProgressBar *bar = new QProgressBar();
    bar->setMaximumWidth(200);
    bar->setRange(0, 100);
    bar->setValue(percentage);
    bar->setFormat(QString::number(percentage, 'f', 1) + " %");
    return bar;
}
void MainWindow::displayVaccinTypeStats(QWidget *parentWidget) {
    // Nettoyer le layout existant
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(parentWidget->layout());
    if (!layout) {
        layout = new QVBoxLayout(parentWidget);
        layout->setContentsMargins(5, 5, 5, 5);
        layout->setSpacing(5);
    } else {
        QLayoutItem *child;
        while ((child = layout->takeAt(0))) {
            if (child->widget()) {
                child->widget()->deleteLater();
            }
            delete child;
        }
    }

    if (!vaccinManager) {
        QLabel *errorLabel = new QLabel("Erreur: Gestionnaire de vaccins non initialisé");
        errorLabel->setStyleSheet("color: #e74c3c; font-style: italic;");
        layout->addWidget(errorLabel);
        return;
    }

    // Récupérer les données dynamiques
    QMap<QString, int> typeStats = vaccinManager->getVaccinTypeStats();
    int totalVaccins = 0;
    for (const auto &count : typeStats) {
        totalVaccins += count;
    }

    if (totalVaccins == 0) {
        QLabel *emptyLabel = new QLabel("Aucune donnée de vaccin disponible");
        emptyLabel->setStyleSheet("color: #7f8c8d; font-style: italic;");
        emptyLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(emptyLabel);
        return;
    }

    // Création du graphique circulaire
    QPieSeries *series = new QPieSeries();
    QStringList colors = {"#3498db", "#2ecc71", "#e74c3c", "#f39c12", "#9b59b6", "#1abc9c"};

    int colorIndex = 0;
    for (auto it = typeStats.begin(); it != typeStats.end(); ++it) {
        double percentage = (it.value() * 100.0) / totalVaccins;
        QString label = QString("%1 (%2%)").arg(it.key()).arg(QString::number(percentage, 'f', 1));

        QPieSlice *slice = series->append(label, it.value());
        slice->setBrush(QColor(colors[colorIndex % colors.size()]));
        slice->setBorderColor(Qt::white);

        // Configuration du style pour chaque slice
        slice->setLabelVisible(true);
        slice->setLabelFont(QFont("Arial", 9)); // Police plus grande
        slice->setLabelArmLengthFactor(0.1); // Longueur du bras du label

        // Effet de survol
        connect(slice, &QPieSlice::hovered, [slice](bool isHovered) {
            slice->setExploded(isHovered);
            if (isHovered) {
                slice->setLabelFont(QFont("Arial", 10, QFont::Bold));
            } else {
                slice->setLabelFont(QFont("Arial", 9));
            }
        });

        colorIndex++;
    }

    // Configuration du graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition par Type");
    chart->setTitleFont(QFont("Arial", 12, QFont::Bold)); // Police de titre plus grande
    chart->legend()->setVisible(false);
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setBackgroundBrush(QBrush(Qt::transparent));
    chart->setMargins(QMargins(0, 0, 0, 0));

    // Position des labels
    series->setLabelsPosition(QPieSlice::LabelOutside);

    // Vue du graphique
    QChartView *chartView = new QChartView(chart, parentWidget);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("background: transparent; border: none;");
    chartView->setMinimumSize(300, 200); // Dimensions minimales augmentées
    chartView->setMaximumSize(450, 300); // Dimensions maximales augmentées

    layout->addWidget(chartView, 0, Qt::AlignCenter);

    // Label du total
    QLabel *totalLabel = new QLabel(QString("Total: %1 unités").arg(totalVaccins), parentWidget);
    totalLabel->setStyleSheet(
        "font-weight: bold;"
        "font-size: 14px;" // Taille de police augmentée
        "color: #2c3e50;"
        "background: #f8f9fa;"
        "border-radius: 3px;"
        "padding: 5px;" // Padding augmenté
        );
    totalLabel->setAlignment(Qt::AlignCenter);
    totalLabel->setMaximumWidth(250); // Largeur maximale augmentée
    layout->addWidget(totalLabel, 0, Qt::AlignCenter);
}

void MainWindow::updateVaccinTypeStats() {
    if (!vaccinsPage || !vaccinManager) return;

    QWidget *statsContainer = vaccinsPage->findChild<QWidget*>("statsContent");
    if (statsContainer) {
        displayVaccinTypeStats(statsContainer);
    }
}

void MainWindow::exportVaccinStatsToPDF(const QMap<QString, int>& typeStats, int totalVaccins) {
    // Demander à l'utilisateur où sauvegarder le fichier
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter les statistiques",
                                                    QDir::homePath() + "/statistiques_vaccins_" +
                                                        QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm") + ".pdf",
                                                    "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty()) {
        return; // L'utilisateur a annulé
    }

    // Créer un document PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(30, 30, 30, 30), QPageLayout::Millimeter);

    // Créer un document HTML qui sera rendu en PDF
    QString html = "<html><body style='font-family: Arial;'>";

    // Titre et en-tête
    html += "<h1 style='text-align: center;'>Statistiques de Vaccination</h1>";
    html += "<h2 style='text-align: center;'>Distribution des Vaccins par Type - " +
            QDateTime::currentDateTime().toString("dd/MM/yyyy") + "</h2>";

    // Commencer le tableau
    html += "<table width='100%' cellspacing='0' cellpadding='5' style='margin: 20px auto; border-collapse: collapse;'>";

    // En-têtes de colonnes
    html += "<tr style='background-color: #f2f2f2;'>";
    html += "<th style='border: 1px solid #ddd; text-align: left; padding: 8px;'>Type de Vaccin</th>";
    html += "<th style='border: 1px solid #ddd; text-align: right; padding: 8px;'>Quantité</th>";
    html += "<th style='border: 1px solid #ddd; text-align: right; padding: 8px;'>Pourcentage</th>";
    html += "</tr>";

    // Données
    for (auto it = typeStats.begin(); it != typeStats.end(); ++it) {
        QString type = it.key();
        int quantity = it.value();
        double percentage = (totalVaccins > 0) ? (quantity * 100.0 / totalVaccins) : 0;

        html += "<tr>";
        html += "<td style='border: 1px solid #ddd; text-align: left; padding: 8px;'>" + type + "</td>";
        html += "<td style='border: 1px solid #ddd; text-align: right; padding: 8px;'>" +
                QString::number(quantity) + " unités</td>";
        html += "<td style='border: 1px solid #ddd; text-align: right; padding: 8px;'>" +
                QString::number(percentage, 'f', 1) + " %</td>";
        html += "</tr>";
    }

    // Ligne Total
    html += "<tr style='font-weight: bold; background-color: #f2f2f2;'>";
    html += "<td style='border: 1px solid #ddd; text-align: left; padding: 8px;'>Total</td>";
    html += "<td style='border: 1px solid #ddd; text-align: right; padding: 8px;'>" +
            QString::number(totalVaccins) + " unités</td>";
    html += "<td style='border: 1px solid #ddd; text-align: right; padding: 8px;'>100.0 %</td>";
    html += "</tr>";

    // Fermer le tableau
    html += "</table>";

    // Pied de page
    html += "<p style='text-align: center; margin-top: 30px; font-size: 12px;'>";
    html += "Généré le " + QDateTime::currentDateTime().toString("dd/MM/yyyy à hh:mm:ss");
    html += "</p>";

    html += "</body></html>";

    // Rendre le HTML dans le PDF
    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);

    // Informer l'utilisateur
    QMessageBox::information(this, "Export réussi",
                             "Les statistiques ont été exportées avec succès vers:\n" + fileName);
}
void MainWindow::exportVaccinsTableToPDF() {
    // Vérifier que le tableau existe
    if (!vaccinManager || !vaccinManager->getTableWidget()) {
        QMessageBox::warning(this, "Export impossible", "Aucun tableau de vaccins à exporter.");
        return;
    }

    QTableWidget* vaccinsTable = vaccinManager->getTableWidget();

    // Demander à l'utilisateur où sauvegarder le fichier
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter le tableau de vaccins",
                                                    QDir::homePath() + "/tableau_vaccins_" +
                                                        QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm") + ".pdf",
                                                    "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) {
        return; // L'utilisateur a annulé
    }

    // Créer un document PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    // Créer un document HTML qui sera rendu en PDF
    QString html = "<html><body style='font-family: Arial;'>";

    // Titre et en-tête
    html += "<h1 style='text-align: center;'>Registre des Vaccins</h1>";
    html += "<h3 style='text-align: center;'>Généré le " +
            QDateTime::currentDateTime().toString("dd/MM/yyyy à hh:mm") + "</h3>";

    // Commencer le tableau
    html += "<table width='100%' cellspacing='0' cellpadding='3' style='margin: 20px auto; border-collapse: collapse;'>";

    // En-têtes de colonnes
    html += "<tr style='background-color: #F5F5F7;'>";
    for (int col = 0; col < vaccinsTable->columnCount(); ++col) {
        QString headerText = vaccinsTable->horizontalHeaderItem(col)->text();
        html += "<th style='border: 1px solid #ddd; text-align: left; padding: 8px; font-weight: bold;'>"
                + headerText + "</th>";
    }
    html += "</tr>";

    // Parcourir les données du tableau
    for (int row = 0; row < vaccinsTable->rowCount(); ++row) {
        // Appliquer une couleur alternée aux lignes pour une meilleure lisibilité
        QString rowStyle = (row % 2 == 0) ? "" : " background-color: #f9f9f9;";
        html += "<tr style='border: 1px solid #ddd;" + rowStyle + "'>";

        for (int col = 0; col < vaccinsTable->columnCount(); ++col) {
            QTableWidgetItem* item = vaccinsTable->item(row, col);
            QString value = item ? item->text() : "";

            // Alignement différent selon le type de colonne
            QString alignment = "left";
            if (col == 0 || col == 5 || col == 6) { // ID, Nombre de doses, Quantité
                alignment = "center";
            }

            html += "<td style='border: 1px solid #ddd; text-align: " + alignment +
                    "; padding: 8px;'>" + value + "</td>";
        }
        html += "</tr>";
    }

    // Fermer le tableau
    html += "</table>";

    // Pied de page
    html += "<p style='text-align: center; margin-top: 30px; font-size: 12px; color: #666;'>";
    html += "Document confidentiel - Gestion des Vaccins";
    html += "</p>";
    html += "</body></html>";

    // Rendre le HTML dans le PDF
    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);

    // Informer l'utilisateur
    QMessageBox::information(this, "Export réussi",
                             "Le tableau des vaccins a été exporté avec succès vers:\n" + fileName);
}
void MainWindow::addPageTitle(QVBoxLayout *vaccinsLayout) {
    QLabel *titleLabel = new QLabel("Résumé des Vaccins", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    vaccinsLayout->addWidget(titleLabel);
}

void MainWindow::addVaccinSummaryTable(QVBoxLayout *vaccinsLayout) {
    QTableWidget *summaryTable = createVaccinSummaryTable();

    if (!vaccinManager->loadVaccinSummary(summaryTable)) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger le résumé des vaccins");
        // Optionnel : Ajouter un message dans la table ou un état vide
    }

    vaccinsLayout->addWidget(summaryTable);
}


QTableWidget* MainWindow::createVaccinSummaryTable() {
    QTableWidget *summaryTable = new QTableWidget(this);

    // Configure table structure
    summaryTable->setColumnCount(4);
    summaryTable->setHorizontalHeaderLabels({"Nom", "Type", "Doses", "Quantité"});
    summaryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Configure selection and edit properties
    summaryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    summaryTable->setSelectionMode(QAbstractItemView::SingleSelection);
    summaryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Configure appearance
    summaryTable->setAlternatingRowColors(true);
    summaryTable->setShowGrid(true);
    summaryTable->verticalHeader()->setVisible(false);

    return summaryTable;
}
void MainWindow::addStatisticsSection(QVBoxLayout *vaccinsLayout) {
    QWidget *statsWidget = new QWidget(this);
    QHBoxLayout *statsLayout = new QHBoxLayout(statsWidget);

    statsLayout->addWidget(createAgeGroupVaccinationWidget());
    statsLayout->addWidget(createNotificationsWidget());

    vaccinsLayout->addWidget(statsWidget);
}

QWidget* MainWindow::createAgeGroupVaccinationWidget() {
    QWidget *ageGroupWidget = new QWidget(this);
    QVBoxLayout *ageGroupLayout = new QVBoxLayout(ageGroupWidget);

    QLabel *ageGroupTitle = new QLabel("Taux de Vaccination par Tranche d'Âge", this);
    ageGroupTitle->setStyleSheet("font-weight: bold;");
    ageGroupLayout->addWidget(ageGroupTitle);

    // Idéalement, ces données viendraient d'une source externe
    QStringList ageGroups = {"0-12 ans", "13-25 ans", "26-45 ans", "46-65 ans", "65+ ans"};
    QVector<double> vaccinationRates = {45.5, 68.2, 72.3, 65.7, 82.1};

    for (int i = 0; i < ageGroups.size(); ++i) {
        ageGroupLayout->addLayout(createVaccinationRateRow(ageGroups[i], vaccinationRates[i]));
    }

    return ageGroupWidget;
}
void MainWindow::setupEditVaccinFormPage() {
    editVaccinFormPage = new QWidget(this);
    QVBoxLayout *formLayout = new QVBoxLayout(editVaccinFormPage);

    // Create input fields
    QLineEdit *nomVaccinInput = new QLineEdit();
    QLineEdit *referenceInput = new QLineEdit();
    QLineEdit *typeInput = new QLineEdit();
    QLineEdit *maladieChronicInput = new QLineEdit();
    QSpinBox *nbDoseInput = new QSpinBox();
    nbDoseInput->setMinimum(1);
    nbDoseInput->setMaximum(10);
    QSpinBox *quantiteInput = new QSpinBox();
    quantiteInput->setMinimum(0);
    quantiteInput->setMaximum(10000);
    QDateEdit *dateExpInput = new QDateEdit(QDate::currentDate().addYears(1));
    dateExpInput->setCalendarPopup(true);
    dateExpInput->setDisplayFormat("dd/MM/yyyy");
    dateExpInput->setMinimumDate(QDate::currentDate().addDays(1));
    dateExpInput->setMaximumDate(QDate::currentDate().addYears(10));

    // Ajout du champ de couleur avec QComboBox
    QComboBox *couleurInput = new QComboBox();
    couleurInput->addItem("Rouge", "Rouge");
    couleurInput->addItem("Vert", "Vert");
    couleurInput->addItem("Bleu", "Bleu");
    couleurInput->addItem("Jaune", "Jaune");
    couleurInput->addItem("Orange", "Orange");
    couleurInput->addItem("Violet", "Violet");

    // Tooltips pour guider l'utilisateur
    nomVaccinInput->setPlaceholderText("Entrez le nom du vaccin");
    nomVaccinInput->setToolTip("50 caractères maximum");

    referenceInput->setPlaceholderText("Format: XXX-0000");
    referenceInput->setToolTip("Trois lettres majuscules, un tiret, quatre chiffres (ex: VCC-1234)");

    // Validator pour la référence
    QRegularExpression refRegex("^[A-Z]{3}-\\d{4}$");
    QRegularExpressionValidator *refValidator = new QRegularExpressionValidator(refRegex, this);
    referenceInput->setValidator(refValidator);

    // Changer la couleur du champ en fonction de la validité
    connect(referenceInput, &QLineEdit::textChanged, [referenceInput, refRegex](const QString &text) {
        if (text.isEmpty() || refRegex.match(text).hasMatch()) {
            referenceInput->setStyleSheet("QLineEdit { background-color: white; }");
        } else {
            referenceInput->setStyleSheet("QLineEdit { background-color: #FFCCCC; }");
        }
    });

    // Limiter la longueur du nom du vaccin
    nomVaccinInput->setMaxLength(50);

    // Save vaccine ID (hidden from user)
    QLineEdit *idInput = new QLineEdit();
    idInput->setVisible(false);

    // Add fields to the form
    QFormLayout *inputLayout = new QFormLayout();
    inputLayout->addRow("Nom Vaccin *:", nomVaccinInput);
    inputLayout->addRow("Référence *:", referenceInput);
    inputLayout->addRow("Type *:", typeInput);
    inputLayout->addRow("Maladie Chronique:", maladieChronicInput);
    inputLayout->addRow("Nombre de Doses *:", nbDoseInput);
    inputLayout->addRow("Quantité *:", quantiteInput);
    inputLayout->addRow("Date d'Expiration *:", dateExpInput);
    inputLayout->addRow("Couleur:", couleurInput); // Ajout du champ couleur au formulaire

    // Label explicatif
    QLabel *infoLabel = new QLabel("Les champs marqués d'un * sont obligatoires");
    infoLabel->setStyleSheet("color: #666; font-style: italic;");
    inputLayout->addRow(infoLabel);

    // Add buttons
    QPushButton *updateButton = new QPushButton("Mettre à jour");
    QPushButton *backButton = new QPushButton("Retour");

    // Add buttons to a horizontal layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(updateButton);

    // Add input layout and buttons to the main layout
    formLayout->addLayout(inputLayout);
    formLayout->addLayout(buttonLayout);

    // Style the form
    editVaccinFormPage->setStyleSheet(
        "QLineEdit, QSpinBox, QDateEdit, QComboBox {"
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

    // Connect the update button
    connect(updateButton, &QPushButton::clicked, this, [this, nomVaccinInput, referenceInput, typeInput,
                                                        maladieChronicInput, nbDoseInput, quantiteInput, dateExpInput, idInput, couleurInput]() {
        // Validation préliminaire côté UI
        bool isValid = true;

        if (nomVaccinInput->text().trimmed().isEmpty()) {
            nomVaccinInput->setStyleSheet("QLineEdit { background-color: #FFCCCC; }");
            isValid = false;
        }

        if (referenceInput->text().trimmed().isEmpty()) {
            referenceInput->setStyleSheet("QLineEdit { background-color: #FFCCCC; }");
            isValid = false;
        }

        if (typeInput->text().trimmed().isEmpty()) {
            typeInput->setStyleSheet("QLineEdit { background-color: #FFCCCC; }");
            isValid = false;
        }

        if (!isValid) {
            QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires");
            return;
        }

        // Get input values
        int id = idInput->text().toInt();
        QString nomVaccin = nomVaccinInput->text();
        QString reference = referenceInput->text();
        QString type = typeInput->text();
        QString maladieChronique = maladieChronicInput->text();
        int nbDose = nbDoseInput->value();
        int quantite = quantiteInput->value();
        QDate dateExp = dateExpInput->date();
        QString couleur = couleurInput->currentData().toString(); // Récupération du nom de la couleur

        // Set values in VaccinManager
        vaccinManager->setId(id);
        vaccinManager->setNomVaccin(nomVaccin);
        vaccinManager->setReference(reference);
        vaccinManager->setType(type);
        vaccinManager->setMaladieChronique(maladieChronique);
        vaccinManager->setNbDose(nbDose);
        vaccinManager->setQuantite(quantite);
        vaccinManager->setDateExp(dateExp);
        vaccinManager->setCouleur(couleur); // Ajout de la couleur au VaccinManager

        // Update the vaccine in the database
        if (vaccinManager->editVaccin()) {
            // Return to the table page on success
            stackedWidget->setCurrentWidget(vaccinsTablePage);
        }
    });

    // Connect the back button
    connect(backButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(vaccinsTablePage);
    });

    // Stocker les widgets pour pouvoir les retrouver plus tard
    editVaccinFormPage->setProperty("idInput", QVariant::fromValue(idInput));
    editVaccinFormPage->setProperty("nomVaccinInput", QVariant::fromValue(nomVaccinInput));
    editVaccinFormPage->setProperty("referenceInput", QVariant::fromValue(referenceInput));
    editVaccinFormPage->setProperty("typeInput", QVariant::fromValue(typeInput));
    editVaccinFormPage->setProperty("maladieChronicInput", QVariant::fromValue(maladieChronicInput));
    editVaccinFormPage->setProperty("nbDoseInput", QVariant::fromValue(nbDoseInput));
    editVaccinFormPage->setProperty("quantiteInput", QVariant::fromValue(quantiteInput));
    editVaccinFormPage->setProperty("dateExpInput", QVariant::fromValue(dateExpInput));
    editVaccinFormPage->setProperty("couleurInput", QVariant::fromValue(couleurInput)); // Stocker le widget de couleur

    // Réinitialiser le style lorsqu'on entre du texte
    connect(nomVaccinInput, &QLineEdit::textEdited, [nomVaccinInput]() {
        nomVaccinInput->setStyleSheet("QLineEdit { background-color: white; }");
    });

    connect(typeInput, &QLineEdit::textEdited, [typeInput]() {
        typeInput->setStyleSheet("QLineEdit { background-color: white; }");
    });
}
void MainWindow::onEditVaccinClicked() {
    // Obtenir la table des vaccins
    QTableWidget *vaccinsTable = nullptr;

    // Trouver la table de manière plus fiable
    QList<QTableWidget*> tables = vaccinsTablePage->findChildren<QTableWidget*>();
    if (!tables.isEmpty()) {
        vaccinsTable = tables.first();
    }

    if (!vaccinsTable) {
        QMessageBox::warning(this, "Erreur", "Table des vaccins non trouvée");
        return;
    }

    // Vérifier qu'une ligne est sélectionnée
    if (vaccinsTable->selectedItems().isEmpty()) {
        QMessageBox::information(this, "Information", "Veuillez sélectionner un vaccin à modifier");
        return;
    }

    // Obtenir l'ID du vaccin sélectionné
    int row = vaccinsTable->selectedItems().first()->row();
    int id = vaccinsTable->item(row, 0)->text().toInt();

    // Charger les données du vaccin
    QMap<QString, QVariant> vaccinData = vaccinManager->getVaccinById(id);
    if (vaccinData.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les données du vaccin");
        return;
    }

    // Obtenir les widgets du formulaire d'édition
    QLineEdit *idInput = editVaccinFormPage->property("idInput").value<QLineEdit*>();
    QLineEdit *nomVaccinInput = editVaccinFormPage->property("nomVaccinInput").value<QLineEdit*>();
    QLineEdit *referenceInput = editVaccinFormPage->property("referenceInput").value<QLineEdit*>();
    QLineEdit *typeInput = editVaccinFormPage->property("typeInput").value<QLineEdit*>();
    QLineEdit *maladieChronicInput = editVaccinFormPage->property("maladieChronicInput").value<QLineEdit*>();
    QSpinBox *nbDoseInput = editVaccinFormPage->property("nbDoseInput").value<QSpinBox*>();
    QSpinBox *quantiteInput = editVaccinFormPage->property("quantiteInput").value<QSpinBox*>();
    QDateEdit *dateExpInput = editVaccinFormPage->property("dateExpInput").value<QDateEdit*>();

    // Vérifier que tous les widgets sont correctement récupérés
    if (!idInput || !nomVaccinInput || !referenceInput || !typeInput ||
        !maladieChronicInput || !nbDoseInput || !quantiteInput || !dateExpInput) {
        QMessageBox::warning(this, "Erreur", "Impossible de récupérer les champs du formulaire");
        return;
    }

    // Remplir le formulaire avec les données du vaccin
    idInput->setText(QString::number(vaccinData["id"].toInt()));
    nomVaccinInput->setText(vaccinData["nom_vaccin"].toString());
    referenceInput->setText(vaccinData["reference"].toString());
    typeInput->setText(vaccinData["type"].toString());
    maladieChronicInput->setText(vaccinData["maladie_chronique"].toString());
    nbDoseInput->setValue(vaccinData["nb_dose"].toInt());
    quantiteInput->setValue(vaccinData["quantite"].toInt());

    // Gestion sécurisée de la date
    QDate dateExp = QDate::currentDate().addYears(1); // Date par défaut
    if (vaccinData.contains("date_exp") && vaccinData["date_exp"].isValid()) {
        QVariant dateVar = vaccinData["date_exp"];
        if (dateVar.canConvert<QDate>()) {
            dateExp = dateVar.toDate();
        }
    }
    dateExpInput->setDate(dateExp);

    // Afficher le formulaire d'édition
    stackedWidget->setCurrentWidget(editVaccinFormPage);
}


QHBoxLayout* MainWindow::createVaccinationRateRow(const QString &ageGroup, double rate) {
    QHBoxLayout *groupRowLayout = new QHBoxLayout();

    QLabel *groupLabel = new QLabel(ageGroup);
    QProgressBar *rateBar = createVaccinationRateBar(rate);

    groupRowLayout->addWidget(groupLabel);
    groupRowLayout->addWidget(rateBar);

    return groupRowLayout;
}
void MainWindow::showExpirationPopup(const QString &message) {
    QMessageBox *popup = new QMessageBox(this);
    popup->setAttribute(Qt::WA_DeleteOnClose);
    popup->setWindowTitle("Alerte d'expiration de vaccin");
    popup->setIcon(QMessageBox::Warning);
    popup->setText(message);

    // Utilisation de QDialogButtonBox pour une meilleure gestion des boutons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, popup);
    connect(buttonBox, &QDialogButtonBox::accepted, popup, &QMessageBox::close);

    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(popup->layout());
    if (layout) {
        layout->addWidget(buttonBox);
    }

    // Style moderne et élégant sans animations complexes
    popup->setStyleSheet(
        "QMessageBox {"
        "   background-color: #f8f9fa;"
        "   border: 2px solid #4a6fa5;"
        "   border-radius: 12px;"
        "   padding: 15px;"
        "}"
        "QMessageBox QLabel {"
        "   color: #2c3e50;"
        "   font-family: 'Segoe UI', 'Helvetica', sans-serif;"
        "   font-weight: 600;"
        "   font-size: 15px;"
        "   margin: 10px 0px;"
        "}"
        "QPushButton {"
        "   background-color: #4a6fa5;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 10px 20px;"
        "   font-weight: bold;"
        "   font-family: 'Segoe UI', 'Helvetica', sans-serif;"
        "   font-size: 13px;"
        "   min-width: 100px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #3a5985;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #2c4a76;"
        "}"
        "QMessageBox QIcon {"
        "   padding: 10px;"
        "}"
        );

    // Ouverture non bloquante
    popup->open();

    // Fermeture automatique après 10 secondes
    QTimer::singleShot(10000, popup, &QMessageBox::close);
}
QWidget* MainWindow::createNotificationsWidget() {
    qDebug() << "🔵 Début de createNotificationsWidget()";

    QWidget *notificationsWidget = new QWidget(this);
    QVBoxLayout *notificationsLayout = new QVBoxLayout(notificationsWidget);

    QLabel *notifTitle = new QLabel("Notifications", this);
    notifTitle->setStyleSheet("font-weight: bold;");
    notificationsLayout->addWidget(notifTitle);

    qDebug() << "✅ Vérification de vaccinManager...";
    if (!vaccinManager) {
        qDebug() << "🚨 Erreur: vaccinManager est NULL";
        QLabel *errorLabel = new QLabel("Erreur : Impossible de charger les notifications", this);
        notificationsLayout->addWidget(errorLabel);
        return notificationsWidget;  // Retourne un widget vide pour éviter le crash
    }

    qDebug() << "✅ Récupération des vaccins expirant...";
    QStringList expiringVaccins;
    try {
        expiringVaccins = vaccinManager->getSoonToExpireVaccins(7);
    } catch (...) {
        qDebug() << "❌ Exception attrapée lors de getSoonToExpireVaccins()";
        expiringVaccins.clear();  // Évite le crash en rendant la liste vide
    }


    qDebug() << "📊 Nombre de vaccins expirant:" << expiringVaccins.size();
    if (expiringVaccins.isEmpty()) {
        QLabel *noExpiringLabel = new QLabel("Aucun vaccin n'expire dans les 7 prochains jours", this);
        notificationsLayout->addWidget(noExpiringLabel);
    } else {
        for (const QString &notif : expiringVaccins) {
            qDebug() << "📌 Ajout de la notification :" << notif;
            QLabel *notifLabel = new QLabel(notif);
            notificationsLayout->addWidget(notifLabel);
            showExpirationPopup(notif);
        }
    }

    notificationsLayout->addStretch();
    qDebug() << "✅ Fin de createNotificationsWidget()";

    return notificationsWidget;
}
void MainWindow::setupVaccinsTablePage() {
    QVBoxLayout *vaccinsTableLayout = new QVBoxLayout(vaccinsTablePage);

    // Layout des boutons
    QHBoxLayout *topButtonLayout = new QHBoxLayout();

    // Bouton Retour
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

    // Boutons d'action avec style cohérent
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

    QString exportButtonStyle =
        "QPushButton {"
        "    background-color: #0d6efd;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #0b5ed7;"
        "}";

    QPushButton *addVaccinButton = new QPushButton("Ajouter Vaccin", this);
    QPushButton *editVaccinButton = new QPushButton("Modifier Vaccin", this);
    QPushButton *deleteVaccinButton = new QPushButton("Supprimer Vaccin", this);
    QPushButton *exportPdfButton = new QPushButton("Exporter PDF", this);

    addVaccinButton->setStyleSheet(buttonStyle);
    editVaccinButton->setStyleSheet(buttonStyle);
    deleteVaccinButton->setStyleSheet(buttonStyle);
    exportPdfButton->setStyleSheet(exportButtonStyle);

    // Disposition des boutons
    topButtonLayout->addWidget(backButton);
    topButtonLayout->addStretch();
    topButtonLayout->addWidget(addVaccinButton);
    topButtonLayout->addWidget(editVaccinButton);
    topButtonLayout->addWidget(deleteVaccinButton);
    topButtonLayout->addWidget(exportPdfButton);

    // Interface de recherche
    QHBoxLayout *searchLayout = new QHBoxLayout();

    QLabel *searchLabel = new QLabel("Rechercher:", this);
    QLineEdit *searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("Entrez votre terme de recherche...");
    searchInput->setStyleSheet(
        "QLineEdit {"
        "    padding: 8px;"
        "    border: 1px solid #ddd;"
        "    border-radius: 4px;"
        "    min-width: 250px;"
        "}"
        );

    // Style des combo box
    QString comboStyle =
        "QComboBox {"
        "    padding: 8px;"
        "    border: 1px solid #ddd;"
        "    border-radius: 4px;"
        "}";

    QComboBox *searchTypeCombo = new QComboBox(this);
    searchTypeCombo->addItem("Tous", "all");
    searchTypeCombo->addItem("Nom", "nom");
    searchTypeCombo->addItem("Référence", "reference");
    searchTypeCombo->addItem("Type", "type");
    searchTypeCombo->addItem("Couleur", "couleur");  // Ajout de l'option pour filtrer par couleur
    searchTypeCombo->setStyleSheet(comboStyle);

    QComboBox *sortTypeCombo = new QComboBox(this);
    sortTypeCombo->addItem("Trier par Nom", VaccinManager::SORT_BY_NAME);
    sortTypeCombo->addItem("Trier par Référence", VaccinManager::SORT_BY_REFERENCE);
    sortTypeCombo->addItem("Trier par Date d'Expiration", VaccinManager::SORT_BY_EXPIRATION);
    sortTypeCombo->setStyleSheet(comboStyle);

    QComboBox *sortOrderCombo = new QComboBox(this);
    sortOrderCombo->addItem("Ascendant", true);
    sortOrderCombo->addItem("Descendant", false);
    sortOrderCombo->setStyleSheet(comboStyle);

    QPushButton *searchButton = new QPushButton("Rechercher", this);
    searchButton->setStyleSheet(buttonStyle);

    QPushButton *resetButton = new QPushButton("Réinitialiser", this);
    resetButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #6c757d;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #5a6268;"
        "}"
        );

    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchInput);
    searchLayout->addWidget(searchTypeCombo);
    searchLayout->addWidget(sortTypeCombo);
    searchLayout->addWidget(sortOrderCombo);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(resetButton);
    searchLayout->addStretch();

    // Création du tableau avec le nouveau style
    QTableWidget *vaccinsTable = new QTableWidget(this);
    vaccinsTable->setColumnCount(9);  // Maintenant 9 colonnes pour inclure la couleur

    QStringList headers = {"ID", "Nom Vaccin", "Référence", "Type", "Maladie Chronique",
                           "Nombre de Doses", "Quantité", "Date d'Expiration", "Couleur"};  // Ajout de "Couleur"
    vaccinsTable->setHorizontalHeaderLabels(headers);

    // Application du style PatientManager
    vaccinsTable->setStyleSheet(
        "QTableWidget {"
        "    background-color: #FFFFFF;"
        "    border: 1px solid #ddd;"
        "    border-radius: 8px;"
        "}"
        "QTableWidget::item {"
        "    border-bottom: 1px solid #ddd;"
        "    padding: 8px;"
        "    color: black;"
        "}"
        "QTableWidget::item:selected {"
        "    background: #EDEDED;"
        "}"
        );

    vaccinsTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    background-color: #F5F5F7;"
        "    color: black;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "    padding: 8px;"
        "    border: none;"
        "    border-bottom: 1px solid #ddd;"
        "}"
        );

    // Configuration de l'apparence du tableau
    vaccinsTable->setShowGrid(false);
    vaccinsTable->verticalHeader()->setVisible(false);
    vaccinsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vaccinsTable->horizontalHeader()->setVisible(true);
    vaccinsTable->verticalHeader()->setDefaultSectionSize(40);
    vaccinsTable->setWordWrap(false);

    // Paramètres de sélection
    vaccinsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    vaccinsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    vaccinsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Ajout des widgets au layout
    vaccinsTableLayout->addLayout(topButtonLayout);
    vaccinsTableLayout->addLayout(searchLayout);
    vaccinsTableLayout->addWidget(vaccinsTable);

    // Initialisation du VaccinManager
    vaccinManager = new VaccinManager(vaccinsTable, this);

    // Appel à updateVaccinTypeStats pour mettre à jour les statistiques
    updateVaccinTypeStats();

    // Connexion des signaux et slots
    connect(addVaccinButton, &QPushButton::clicked, this, &MainWindow::onAddVaccinClicked);
    connect(editVaccinButton, &QPushButton::clicked, this, &MainWindow::onEditVaccinClicked);
    connect(deleteVaccinButton, &QPushButton::clicked, this, &MainWindow::onDeleteVaccinClicked);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showVaccinsPage);
    connect(exportPdfButton, &QPushButton::clicked, this, &MainWindow::exportVaccinsTableToPDF);

    // Connexion des signaux pour la recherche avec tri
    connect(searchButton, &QPushButton::clicked, this, [this, searchInput, searchTypeCombo, sortTypeCombo, sortOrderCombo]() {
        QString searchText = searchInput->text();
        QString searchType = searchTypeCombo->currentData().toString();
        VaccinManager::SortType sortType = static_cast<VaccinManager::SortType>(sortTypeCombo->currentData().toInt());
        bool ascending = sortOrderCombo->currentData().toBool();

        vaccinManager->searchVaccins(searchText, searchType, sortType, ascending);

        // Mise à jour des statistiques après une recherche
        updateVaccinTypeStats();
    });

    connect(resetButton, &QPushButton::clicked, this, [this, searchInput, sortTypeCombo, sortOrderCombo]() {
        searchInput->clear();
        VaccinManager::SortType sortType = static_cast<VaccinManager::SortType>(sortTypeCombo->currentData().toInt());
        bool ascending = sortOrderCombo->currentData().toBool();

        // Recharger tous les vaccins avec les paramètres de tri actuels
        vaccinManager->loadVaccins(sortType, ascending);

        // Mise à jour des statistiques après réinitialisation
        updateVaccinTypeStats();
    });

    // Permettre la recherche en appuyant sur Entrée dans le champ de recherche
    connect(searchInput, &QLineEdit::returnPressed, searchButton, &QPushButton::click);

    // Tri direct lorsque les options de tri changent
    connect(sortTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, sortTypeCombo, sortOrderCombo]() {
        VaccinManager::SortType sortType = static_cast<VaccinManager::SortType>(sortTypeCombo->currentData().toInt());
        bool ascending = sortOrderCombo->currentData().toBool();
        vaccinManager->loadVaccins(sortType, ascending);

        // Mise à jour des statistiques après changement de tri
        updateVaccinTypeStats();
    });

    connect(sortOrderCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, sortTypeCombo, sortOrderCombo]() {
        VaccinManager::SortType sortType = static_cast<VaccinManager::SortType>(sortTypeCombo->currentData().toInt());
        bool ascending = sortOrderCombo->currentData().toBool();
        vaccinManager->loadVaccins(sortType, ascending);

        // Mise à jour des statistiques après changement d'ordre de tri
        updateVaccinTypeStats();
    });

    // Mettre à jour les notifications également
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
    nbDoseInput->setMaximum(10);
    QSpinBox *quantiteInput = new QSpinBox();
    quantiteInput->setMinimum(0);
    quantiteInput->setMaximum(10000);
    QDateEdit *dateExpInput = new QDateEdit(QDate::currentDate().addYears(1));
    dateExpInput->setCalendarPopup(true);
    dateExpInput->setDisplayFormat("dd/MM/yyyy");
    dateExpInput->setMinimumDate(QDate::currentDate().addDays(1));
    dateExpInput->setMaximumDate(QDate::currentDate().addYears(10));

    // Ajout du champ couleur avec QComboBox
    QComboBox *couleurInput = new QComboBox();
    couleurInput->addItem("Rouge", "Rouge");
    couleurInput->addItem("Vert", "Vert");
    couleurInput->addItem("Bleu", "Bleu");
    couleurInput->addItem("Jaune", "Jaune");
    couleurInput->addItem("Orange", "Orange");
    couleurInput->addItem("Violet", "Violet");
    couleurInput->setCurrentIndex(0); // Rouge par défaut

    // Tooltips pour guider l'utilisateur
    nomVaccinInput->setPlaceholderText("Entrez le nom du vaccin");
    nomVaccinInput->setToolTip("50 caractères maximum");

    referenceInput->setPlaceholderText("Format: XXX-0000");
    referenceInput->setToolTip("Trois lettres majuscules, un tiret, quatre chiffres (ex: VCC-1234)");

    // Validator pour la référence
    QRegularExpression refRegex("^[A-Z]{3}-\\d{4}$");
    QRegularExpressionValidator *refValidator = new QRegularExpressionValidator(refRegex, this);
    referenceInput->setValidator(refValidator);

    // Changer la couleur du champ en fonction de la validité
    connect(referenceInput, &QLineEdit::textChanged, [referenceInput, refRegex](const QString &text) {
        if (text.isEmpty() || refRegex.match(text).hasMatch()) {
            referenceInput->setStyleSheet("QLineEdit { background-color: white; }");
        } else {
            referenceInput->setStyleSheet("QLineEdit { background-color: #FFCCCC; }");
        }
    });

    // Limiter la longueur du nom du vaccin
    nomVaccinInput->setMaxLength(50);

    // Add fields to the form
    QFormLayout *inputLayout = new QFormLayout();
    inputLayout->addRow("Nom Vaccin *:", nomVaccinInput);
    inputLayout->addRow("Référence *:", referenceInput);
    inputLayout->addRow("Type *:", typeInput);
    inputLayout->addRow("Maladie Chronique:", maladieChronicInput);
    inputLayout->addRow("Nombre de Doses *:", nbDoseInput);
    inputLayout->addRow("Quantité *:", quantiteInput);
    inputLayout->addRow("Date d'Expiration *:", dateExpInput);
    inputLayout->addRow("Couleur:", couleurInput);  // Ajout du champ couleur au formulaire

    // Label explicatif
    QLabel *infoLabel = new QLabel("Les champs marqués d'un * sont obligatoires");
    infoLabel->setStyleSheet("color: #666; font-style: italic;");
    inputLayout->addRow(infoLabel);

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
        "QLineEdit, QSpinBox, QDateEdit, QComboBox {"  // Ajout de QComboBox au style
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
    connect(submitButton, &QPushButton::clicked, this, [this, nomVaccinInput, referenceInput, typeInput,
                                                        maladieChronicInput, nbDoseInput, quantiteInput,
                                                        dateExpInput, couleurInput]() {
        // Validation préliminaire côté UI
        bool isValid = true;

        if (nomVaccinInput->text().trimmed().isEmpty()) {
            nomVaccinInput->setStyleSheet("QLineEdit { background-color: #FFCCCC; }");
            isValid = false;
        }

        if (referenceInput->text().trimmed().isEmpty()) {
            referenceInput->setStyleSheet("QLineEdit { background-color: #FFCCCC; }");
            isValid = false;
        }

        if (typeInput->text().trimmed().isEmpty()) {
            typeInput->setStyleSheet("QLineEdit { background-color: #FFCCCC; }");
            isValid = false;
        }

        if (!isValid) {
            QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires");
            return;
        }

        // Get input values
        QString nomVaccin = nomVaccinInput->text();
        QString reference = referenceInput->text();
        QString type = typeInput->text();
        QString maladieChronique = maladieChronicInput->text();
        int nbDose = nbDoseInput->value();
        int quantite = quantiteInput->value();
        QDate dateExp = dateExpInput->date();
        QString couleur = couleurInput->currentData().toString();  // Récupération du nom de la couleur

        // Set values in VaccinManager
        vaccinManager->setNomVaccin(nomVaccin);
        vaccinManager->setReference(reference);
        vaccinManager->setType(type);
        vaccinManager->setMaladieChronique(maladieChronique);
        vaccinManager->setNbDose(nbDose);
        vaccinManager->setQuantite(quantite);
        vaccinManager->setDateExp(dateExp);
        vaccinManager->setCouleur(couleur);  // Ajout de la couleur au VaccinManager

        // Add the vaccine to the database
        if (vaccinManager->addVaccin()) {
            // Si l'ajout est réussi, vider les champs
            nomVaccinInput->clear();
            referenceInput->clear();
            typeInput->clear();
            maladieChronicInput->clear();
            nbDoseInput->setValue(1);
            quantiteInput->setValue(0);
            dateExpInput->setDate(QDate::currentDate().addYears(1));
            couleurInput->setCurrentIndex(0);  // Réinitialiser la couleur à la valeur par défaut

            // Retourner à la page du tableau
            stackedWidget->setCurrentWidget(vaccinsTablePage);
        }
    });

    // Connect the back button
    connect(backButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(vaccinsTablePage);
    });

    // Réinitialiser le style lorsqu'on entre du texte
    connect(nomVaccinInput, &QLineEdit::textEdited, [nomVaccinInput]() {
        nomVaccinInput->setStyleSheet("QLineEdit { background-color: white; }");
    });

    connect(typeInput, &QLineEdit::textEdited, [typeInput]() {
        typeInput->setStyleSheet("QLineEdit { background-color: white; }");
    });
}
// Si vous avez une fonction comme celle-ci ailleurs dans votre code
void MainWindow::loadVaccinDataToEditForm(int vaccinId) {
    QMap<QString, QVariant> vaccinData = vaccinManager->getVaccinById(vaccinId);

    if (!vaccinData.isEmpty()) {
        QLineEdit *idInput = editVaccinFormPage->property("idInput").value<QLineEdit*>();
        QLineEdit *nomVaccinInput = editVaccinFormPage->property("nomVaccinInput").value<QLineEdit*>();
        QLineEdit *referenceInput = editVaccinFormPage->property("referenceInput").value<QLineEdit*>();
        QLineEdit *typeInput = editVaccinFormPage->property("typeInput").value<QLineEdit*>();
        QLineEdit *maladieChronicInput = editVaccinFormPage->property("maladieChronicInput").value<QLineEdit*>();
        QSpinBox *nbDoseInput = editVaccinFormPage->property("nbDoseInput").value<QSpinBox*>();
        QSpinBox *quantiteInput = editVaccinFormPage->property("quantiteInput").value<QSpinBox*>();
        QDateEdit *dateExpInput = editVaccinFormPage->property("dateExpInput").value<QDateEdit*>();
        QLineEdit *couleurInput = editVaccinFormPage->property("couleurInput").value<QLineEdit*>(); // Récupérer le widget couleur

        idInput->setText(vaccinData["id"].toString());
        nomVaccinInput->setText(vaccinData["nom_vaccin"].toString());
        referenceInput->setText(vaccinData["reference"].toString());
        typeInput->setText(vaccinData["type"].toString());
        maladieChronicInput->setText(vaccinData["maladie_chronique"].toString());
        nbDoseInput->setValue(vaccinData["nb_dose"].toInt());
        quantiteInput->setValue(vaccinData["quantite"].toInt());
        dateExpInput->setDate(vaccinData["date_exp"].toDate());
        couleurInput->setText(vaccinData["couleur"].toString()); // Définir la valeur du champ couleur

        stackedWidget->setCurrentWidget(editVaccinFormPage);
    }
}

// Tools-related slots




// Vaccin-related slots
void MainWindow::showVaccinsTablePage() {
    stackedWidget->setCurrentWidget(vaccinsTablePage);
}
void MainWindow::showPatientsTablePage() {
    stackedWidget->setCurrentWidget(PatientsTablePage);
}


void MainWindow::onAddVaccinClicked() {
    stackedWidget->setCurrentWidget(addVaccinFormPage);
}



void MainWindow::onDeleteVaccinClicked() {
    // Get the currently selected row in the vaccins table
    QTableWidget *vaccinsTable = vaccinsTablePage->findChild<QTableWidget*>();
    if (!vaccinsTable) {
        QMessageBox::warning(this, "Erreur", "Table des vaccins non trouvée");
        return;
    }

    QList<QTableWidgetItem*> selectedItems = vaccinsTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::information(this, "Information", "Veuillez sélectionner un vaccin à supprimer");
        return;
    }

    // Get the ID from the first column of the selected row
    int row = selectedItems.first()->row();
    int id = vaccinsTable->item(row, 0)->text().toInt();
    QString nomVaccin = vaccinsTable->item(row, 1)->text();

    // Ask for confirmation before deleting
    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Confirmation de suppression",
                                         "Êtes-vous sûr de vouloir supprimer le vaccin \"" + nomVaccin + "\" ?",
                                         QMessageBox::Yes | QMessageBox::No);

    if (confirmation == QMessageBox::Yes) {
        vaccinManager->setId(id);
        if (vaccinManager->deleteVaccin()) {
            // Refresh the table or remove the row
            vaccinsTable->removeRow(row);
        }
    }
}

///////////////////////////////////////////
///

void MainWindow::setupMessagerieePage()
{
    // Afficher un message de débogage pour suivre l'exécution
    qDebug() << "Début de setupMessagerieePage()";

    // Créer la page de messagerie
    messagerieePage = new QWidget();

    QVBoxLayout *mainLayout = new QVBoxLayout(messagerieePage);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // En-tête de la page
    QLabel *headerLabel = new QLabel("Messagerie");
    headerLabel->setStyleSheet("font-size: 22px; font-weight: bold; margin-bottom: 15px;");

    // Boutons d'action
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *newMessageBtn = new QPushButton("Nouveau message");
    //
    //newMessageBtn->setIcon(QIcon(":/icons/svg/message.svg"));

    QPushButton *deleteMessageBtn = new QPushButton("Supprimer");
    deleteMessageBtn->setIcon(QIcon(":/icons/svg/delete.svg"));

    QPushButton *refreshBtn = new QPushButton("Actualiser");
    refreshBtn->setIcon(QIcon(":/icons/svg/refresh.svg"));

    buttonLayout->addWidget(newMessageBtn);
    buttonLayout->addWidget(deleteMessageBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(refreshBtn);

    // Créer un splitter pour diviser l'interface
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Liste des contacts à gauche
    contactsList = new QListWidget();
    contactsList->setMinimumWidth(200);
    contactsList->setMaximumWidth(300);
    contactsList->setAlternatingRowColors(true);
    contactsList->setStyleSheet(
        "QListWidget {"
        "   border: 1px solid #ccc;"
        "   border-radius: 5px;"
        "   background-color: #f8f8f8;"
        "}"
        "QListWidget::item {"
        "   padding: 10px;"
        "   border-bottom: 1px solid #eee;"
        "}"
        "QListWidget::item:selected {"
        "   background-color: #e0e0ff;"
        "   color: #000;"
        "}"
        );

    // Conteneur pour la conversation à droite
    conversationContainer = new QWidget();
    QVBoxLayout *conversationLayout = new QVBoxLayout(conversationContainer);
    conversationLayout->setSpacing(10);
    conversationLayout->setContentsMargins(0, 0, 0, 0);

    // Zone de défilement pour les messages
    messagesScrollArea = new QScrollArea();
    messagesScrollArea->setWidgetResizable(true);
    messagesScrollArea->setFrameShape(QFrame::NoFrame);
    messagesScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    messagesScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Widget contenant les messages
    messagesWidget = new QWidget();
    messagesLayout = new QVBoxLayout(messagesWidget);
    messagesLayout->setAlignment(Qt::AlignTop);
    messagesLayout->setSpacing(15);
    messagesLayout->setContentsMargins(15, 15, 15, 15);

    messagesScrollArea->setWidget(messagesWidget);

    // Zone de saisie de message
    QHBoxLayout *inputLayout = new QHBoxLayout();
    messageInputField = new QTextEdit();
    messageInputField->setMaximumHeight(80);
    messageInputField->setPlaceholderText("Écrivez votre message ici...");
    messageInputField->setStyleSheet(
        "QTextEdit {"
        "   border: 1px solid #ccc;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "}"
        );

    sendMessageButton = new QPushButton("Envoyer");
    sendMessageButton->setIcon(QIcon(":/icons/svg/send.svg"));
    sendMessageButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4a86e8;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 16px;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #3a76d8;"
        "}"
        );

    inputLayout->addWidget(messageInputField, 4);
    inputLayout->addWidget(sendMessageButton, 1);

    // Ajouter tous les éléments au layout de la conversation
    conversationLayout->addWidget(messagesScrollArea, 1);
    conversationLayout->addLayout(inputLayout);

    // Ajouter les widgets au splitter
    splitter->addWidget(contactsList);
    splitter->addWidget(conversationContainer);
    splitter->setStretchFactor(0, 1);  // Liste de contacts
    splitter->setStretchFactor(1, 3);  // Conversation

    // Initialiser le tableau (garder pour compatibilité avec le code existant)
    messageTable = new QTableWidget();
    messageTable->hide();  // Le cacher car on utilise la nouvelle interface

    // Ajouter les widgets au layout principal
    mainLayout->addWidget(headerLabel);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(splitter, 1);

    // Créer le gestionnaire de messages avec debug
    qDebug() << "Avant création du MessagesManager";
    messagesManager = new MessagesManager(messageTable, this);
    qDebug() << "MessagesManager créé avec succès";

    // Ajout de la ligne qui cause le plantage, avec debug
    qDebug() << "Avant setContactsList";
    messagesManager->setContactsList(contactsList);
    qDebug() << "setContactsList appelé avec succès";

    // Connecter les signaux
    connect(newMessageBtn, &QPushButton::clicked, this, &MainWindow::onNewMessageButtonClicked);
    connect(deleteMessageBtn, &QPushButton::clicked, this, &MainWindow::onDeleteMessageButtonClicked);
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::onRefreshMessagesButtonClicked);
    connect(contactsList, &QListWidget::currentRowChanged, this, &MainWindow::onContactSelected);
    connect(sendMessageButton, &QPushButton::clicked, this, &MainWindow::onSendMessageClicked);

    // Initialiser la conversation vide
    clearConversation();

    qDebug() << "Fin de setupMessagerieePage()";
}
void MainWindow::showMessagerieePage()
{
    qDebug() << "Affichage de la page messagerie";

    // Charger les contacts et messages
    loadContacts();

    // Charger aussi les messages dans le tableau (pour compatibilité)
    if (messagesManager) {
        qDebug() << "Chargement des messages via messagesManager";
        messagesManager->loadMessages();
    } else {
        qDebug() << "ERREUR: messagesManager est NULL!";
    }

    // Changer la page active
    stackedWidget->setCurrentWidget(messagerieePage);
}

void MainWindow::loadContacts() {
    // Vider la liste actuelle
    contactsList->clear();

    if (!messagesManager || !messagesManager->createDatabaseConnection()) {
        qDebug() << "Erreur: Impossible de se connecter à la base de données";
        return;
    }

    QSqlDatabase& db = messagesManager->getDatabaseConnection();
    QSqlQuery query(db);

    // Requête pour récupérer les chercheurs
    QString queryChercheurs =
        "SELECT IDC as ID, NOM || ' ' || PRENOM as NOM_COMPLET, 'CHERCHEUR' as TYPE "
        "FROM chercheurs "
        "ORDER BY NOM, PRENOM";

    // Requête pour récupérer le personnel
    QString queryPersonnel =
        "SELECT IDPER as ID, NOM || ' ' || PRENOM as NOM_COMPLET, 'PERSONNEL' as TYPE "
        "FROM personnels "
        "ORDER BY NOM, PRENOM";

    // Exécuter la requête pour les chercheurs
    if (query.exec(queryChercheurs)) {
        while (query.next()) {
            int id = query.value("ID").toInt();
            QString nom = query.value("NOM_COMPLET").toString();
            QString type = query.value("TYPE").toString();

            QListWidgetItem *item = new QListWidgetItem(type + ": " + nom);
            item->setData(Qt::UserRole, id);        // ID du contact
            item->setData(Qt::UserRole + 1, type);  // Type de contact (CHERCHEUR/PERSONNEL)
            contactsList->addItem(item);
        }
    } else {
        qDebug() << "Erreur lors du chargement des chercheurs:" << query.lastError().text();
    }

    // Exécuter la requête pour le personnel
    if (query.exec(queryPersonnel)) {
        while (query.next()) {
            int id = query.value("ID").toInt();
            QString nom = query.value("NOM_COMPLET").toString();
            QString type = query.value("TYPE").toString();

            QListWidgetItem *item = new QListWidgetItem(type + ": " + nom);
            item->setData(Qt::UserRole, id);        // ID du contact
            item->setData(Qt::UserRole + 1, type);  // Type de contact
            contactsList->addItem(item);
        }
    } else {
        qDebug() << "Erreur lors du chargement du personnel:" << query.lastError().text();
    }
}

void MainWindow::onContactSelected(int row)
{
    if (row >= 0) {
        QListWidgetItem *item = contactsList->item(row);
        int contactId = item->data(Qt::UserRole).toInt();
        QString contactType = item->data(Qt::UserRole + 1).toString();

        // Mettre à jour le gestionnaire de messages avec les informations du contact
        if (messagesManager) {
            messagesManager->setCurrentContactId(contactId);
            messagesManager->setCurrentContactType(contactType);
        }

        displayConversation(contactId);
    }
}

void MainWindow::displayConversation(int contactId) {
    // Store the current contact ID in MessagesManager
    if (messagesManager) {
        messagesManager->setCurrentContactId(contactId);
    }

    // Stocker l'ID du contact actuel
    currentContactId = contactId;
    QString contactType;

    // Récupérer le type du contact sélectionné
    for (int i = 0; i < contactsList->count(); i++) {
        QListWidgetItem *item = contactsList->item(i);
        if (item->data(Qt::UserRole).toInt() == contactId) {
            contactType = item->data(Qt::UserRole + 1).toString();
            break;
        }
    }

    // Effacer la conversation précédente
    clearConversation();

    if (!messagesManager || !messagesManager->createDatabaseConnection()) {
        qDebug() << "Erreur: Impossible de se connecter à la base de données";
        return;
    }

    QSqlDatabase& db = messagesManager->getDatabaseConnection();
    QSqlQuery query(db);

    // Requête pour obtenir tous les messages entre l'utilisateur actuel et le contact
    // Suppose qu'on a un ID utilisateur stocké (par exemple 1) et son type (par exemple 'PERSONNEL')
    int currentUserId = 1; // À remplacer par l'ID de l'utilisateur connecté
    QString currentUserType = "PERSONNEL"; // À remplacer par le type de l'utilisateur connecté

    QString queryStr =
        "SELECT m.ID_MESSAGE, m.CONTENU, m.ID_EXPEDITEUR, m.TYPE_EXPEDITEUR, "
        "TO_CHAR(m.DATE_ENVOI, 'DD/MM/YYYY HH24:MI') as DATE_FORMATEE "
        "FROM messages m "
        "WHERE (m.ID_EXPEDITEUR = :contactId AND m.TYPE_EXPEDITEUR = :contactType AND m.ID_DESTINATAIRE = :currentUserId AND m.TYPE_DESTINATAIRE = :currentUserType) "
        "OR (m.ID_EXPEDITEUR = :currentUserId AND m.TYPE_EXPEDITEUR = :currentUserType AND m.ID_DESTINATAIRE = :contactId AND m.TYPE_DESTINATAIRE = :contactType) "
        "ORDER BY m.DATE_ENVOI ASC";

    query.prepare(queryStr);
    query.bindValue(":contactId", contactId);
    query.bindValue(":contactType", contactType);
    query.bindValue(":currentUserId", currentUserId);
    query.bindValue(":currentUserType", currentUserType);

    if (query.exec()) {
        while (query.next()) {
            int expediteurId = query.value("ID_EXPEDITEUR").toInt();
            QString expediteurType = query.value("TYPE_EXPEDITEUR").toString();
            QString contenu = query.value("CONTENU").toString();
            QString dateTime = query.value("DATE_FORMATEE").toString();

            // Déterminer si c'est un message envoyé par l'utilisateur actuel
            bool isCurrentUser = (expediteurId == currentUserId && expediteurType == currentUserType);

            // Récupérer le nom de l'expéditeur
            QString senderName;
            if (isCurrentUser) {
                senderName = "Vous";
            } else {
                // Récupérer le nom depuis la liste des contacts
                for (int i = 0; i < contactsList->count(); i++) {
                    QListWidgetItem *item = contactsList->item(i);
                    if (item->data(Qt::UserRole).toInt() == expediteurId &&
                        item->data(Qt::UserRole + 1).toString() == expediteurType) {
                        senderName = item->text();
                        break;
                    }
                }

                // Si nom non trouvé, utiliser un format par défaut
                if (senderName.isEmpty()) {
                    senderName = expediteurType + " ID: " + QString::number(expediteurId);
                }
            }

            // Ajouter le message à l'interface
            addMessageBubble(contenu, senderName, dateTime, isCurrentUser);
        }
    } else {
        qDebug() << "Erreur lors du chargement des messages:" << query.lastError().text();
    }

    // Faire défiler jusqu'au bas de la conversation
    QTimer::singleShot(100, [this]() {
        if (messagesScrollArea && messagesScrollArea->verticalScrollBar()) {
            messagesScrollArea->verticalScrollBar()->setValue(
                messagesScrollArea->verticalScrollBar()->maximum());
        }
    });
}
void MainWindow::clearConversation()
{
    // Supprimer tous les messages existants
    QLayoutItem *child;
    while ((child = messagesLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }

    // Ajouter un message d'information si aucun contact n'est sélectionné
    if (currentContactId == 0) {
        QLabel *noContactLabel = new QLabel("Sélectionnez un contact pour afficher la conversation");
        noContactLabel->setAlignment(Qt::AlignCenter);
        noContactLabel->setStyleSheet("color: #888; font-style: italic;");
        messagesLayout->addWidget(noContactLabel);
        messagesLayout->addStretch();
    }
}

void MainWindow::addMessageBubble(const QString &message, const QString &sender,
                                  const QString &timestamp, bool isCurrentUser)
{
    // Créer le widget pour le message
    QWidget *bubbleWidget = new QWidget();
    QVBoxLayout *bubbleLayout = new QVBoxLayout(bubbleWidget);
    bubbleLayout->setContentsMargins(5, 5, 5, 5);

    // Créer le contenu du message
    QLabel *messageLabel = new QLabel(message);
    messageLabel->setWordWrap(true);
    messageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    // Créer le label pour l'expéditeur et l'horodatage
    QLabel *infoLabel = new QLabel(sender + " • " + timestamp);
    infoLabel->setStyleSheet("color: #888; font-size: 10px;");

    // Appliquer le style selon l'expéditeur
    if (isCurrentUser) {
        bubbleWidget->setStyleSheet(getOutgoingMessageStyle());
        bubbleLayout->setAlignment(Qt::AlignRight);
    } else {
        bubbleWidget->setStyleSheet(getIncomingMessageStyle());
        bubbleLayout->setAlignment(Qt::AlignLeft);
    }

    // Assembler le message
    bubbleLayout->addWidget(messageLabel);
    bubbleLayout->addWidget(infoLabel);

    // Ajouter le message à la conversation
    QHBoxLayout *rowLayout = new QHBoxLayout();
    if (isCurrentUser) {
        rowLayout->addStretch();
        rowLayout->addWidget(bubbleWidget);
    } else {
        rowLayout->addWidget(bubbleWidget);
        rowLayout->addStretch();
    }

    messagesLayout->addLayout(rowLayout);
}

QString MainWindow::getIncomingMessageStyle()
{
    return "QWidget {"
           "   background-color: #f1f1f1;"
           "   border-radius: 10px;"
           "   padding: 10px;"
           "   max-width: 70%;"
           "}";
}

QString MainWindow::getOutgoingMessageStyle()
{
    return "QWidget {"
           "   background-color: #e1f5fe;"
           "   border-radius: 10px;"
           "   padding: 10px;"
           "   max-width: 70%;"
           "}";
}

void MainWindow::onSendMessageClicked() {
    QString messageText = messageInputField->toPlainText().trimmed();
    if (!messageText.isEmpty() && currentContactId > 0) {
        // Récupérer le type du contact
        QString contactType;
        for (int i = 0; i < contactsList->count(); i++) {
            QListWidgetItem *item = contactsList->item(i);
            if (item->data(Qt::UserRole).toInt() == currentContactId) {
                contactType = item->data(Qt::UserRole + 1).toString();
                break;
            }
        }

        // ID et type de l'utilisateur actuel (à adapter selon votre contexte)
        int currentUserId = 1; // Exemple: ID de l'utilisateur connecté
        QString currentUserType = "PERSONNEL"; // Exemple: type de l'utilisateur connecté

        // Titre du message (peut être adapté)
        QString titre = "Message à " + QString::number(currentContactId);

        // Enregistrer le message dans la base de données
        if (messagesManager && messagesManager->addMessage(titre, messageText,
                                                           currentUserId, currentUserType,
                                                           currentContactId, contactType)) {
            // Ajouter le message à l'interface
            QString currentDateTime = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm");
            addMessageBubble(messageText, "Vous", currentDateTime, true);

            // Vider le champ de saisie
            messageInputField->clear();

            // Faire défiler jusqu'au bas de la conversation
            QTimer::singleShot(100, [this]() {
                if (messagesScrollArea && messagesScrollArea->verticalScrollBar()) {
                    messagesScrollArea->verticalScrollBar()->setValue(
                        messagesScrollArea->verticalScrollBar()->maximum());
                }
            });
        }
    }
}
void MainWindow::onRefreshMessagesButtonClicked()
{
    qDebug() << "Entrée dans onRefreshMessagesButtonClicked";

    // Rafraîchir la liste des contacts
    loadContacts();

    // Rafraîchir la conversation actuelle si un contact est sélectionné
    if (currentContactId > 0) {
        displayConversation(currentContactId);
    }

    // Conserver la compatibilité avec l'ancien code
    if (messagesManager) {
        qDebug() << "Rechargement des messages dans le tableau";
        messagesManager->loadMessages();
    } else {
        qDebug() << "ERREUR: messagesManager est NULL";
    }
}
// Ajoutez ces implémentations dans votre fichier mainwindow.cpp

void MainWindow::onReadMessageButtonClicked()
{
    qDebug() << "Entrée dans onReadMessageButtonClicked";

    // Vérifier si un message est sélectionné dans le tableau
    if (messageTable->selectedItems().isEmpty()) {
        QMessageBox::information(this, "Information", "Veuillez sélectionner un message à lire.");
        return;
    }

    int row = messageTable->currentRow();
    if (row >= 0) {
        // Récupérer l'ID de l'expéditeur (contactId)
        int senderId = messageTable->item(row, 1)->text().toInt(); // Ajustez l'index de colonne selon votre modèle

        // Marquer le message comme lu dans la base de données si nécessaire
        // messagesManager->markAsRead(messageId);

        // Afficher la conversation avec cet expéditeur
        displayConversation(senderId);

        // Sélectionner le contact dans la liste
        for (int i = 0; i < contactsList->count(); i++) {
            QListWidgetItem *item = contactsList->item(i);
            if (item->data(Qt::UserRole).toInt() == senderId) {
                contactsList->setCurrentRow(i);
                break;
            }
        }
    }
}

void MainWindow::onNewMessageButtonClicked()
{
    qDebug() << "Entrée dans onNewMessageButtonClicked";

    // Préparer une liste pour stocker tous les destinataires possibles
    QStringList destinataires;
    QMap<QString, QPair<int, QString>> destinataireMap; // Pour stocker l'ID et le type de chaque destinataire

    // Utiliser la liste des contacts déjà chargée
    for (int i = 0; i < contactsList->count(); i++) {
        QListWidgetItem *item = contactsList->item(i);
        int contactId = item->data(Qt::UserRole).toInt();
        QString contactType = item->data(Qt::UserRole + 1).toString();
        QString displayName = item->text();

        destinataires << displayName;
        destinataireMap[displayName] = qMakePair(contactId, contactType);
    }

    // Afficher une boîte de dialogue pour sélectionner un destinataire
    bool ok;
    QString selectedItem = QInputDialog::getItem(this, "Nouveau message",
                                                 "Sélectionnez un destinataire:",
                                                 destinataires,
                                                 0, false, &ok);

    if (ok && !selectedItem.isEmpty()) {
        // Récupérer l'ID et le type du contact sélectionné
        int contactId = destinataireMap[selectedItem].first;
        QString contactType = destinataireMap[selectedItem].second;

        // Mettre à jour les informations du contact sélectionné dans messagesManager
        if (messagesManager) {
            messagesManager->setCurrentContactId(contactId);
            messagesManager->setCurrentContactType(contactType);
        }

        // Sélectionner le contact dans la liste
        for (int i = 0; i < contactsList->count(); i++) {
            QListWidgetItem *listItem = contactsList->item(i);
            if (listItem->data(Qt::UserRole).toInt() == contactId) {
                contactsList->setCurrentRow(i);
                break;
            }
        }

        // Mettre le focus sur le champ de saisie
        messageInputField->setFocus();
    }
}

void MainWindow::onDeleteMessageButtonClicked()
{
    qDebug() << "Entrée dans onDeleteMessageButtonClicked";

    // Vérifier si un message est sélectionné dans le tableau
    if (messageTable->selectedItems().isEmpty()) {
        QMessageBox::information(this, "Information", "Veuillez sélectionner un message à supprimer.");
        return;
    }

    // Confirmation de suppression
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                              "Êtes-vous sûr de vouloir supprimer ce message ?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        int row = messageTable->currentRow();
        if (row >= 0) {
            // Récupérer l'ID du message
            int messageId = messageTable->item(row, 0)->text().toInt(); // Ajustez l'index de colonne selon votre modèle

            // Supprimer le message via le gestionnaire
            // messagesManager->deleteMessage(messageId);

            // Rafraîchir la liste des messages
            messagesManager->loadMessages();

            // Si la conversation actuellement affichée est affectée, la rafraîchir
            if (currentContactId > 0) {
                displayConversation(currentContactId);
            }
        }
    }
}
/*

void MainWindow::setupArduinoPanel()
{
    // Créer un widget pour contenir les contrôles Arduino
    QWidget *arduinoWidget = new QWidget();
    QVBoxLayout *arduinoLayout = new QVBoxLayout(arduinoWidget);

    // Créer les éléments d'interface
    QLabel *titleLabel = new QLabel("Détection Arduino");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");

    portSelector = new QComboBox();
    portSelector->addItems(arduinoManager->availablePorts());

    connectButton = new QPushButton("Connecter");
    statusLabel = new QLabel("Non connecté");
    statusLabel->setStyleSheet("color: red;");

    detectedColorLabel = new QLabel("Aucune couleur détectée");

    // Ajouter un label pour afficher les informations du vaccin
    vaccinInfoLabel = new QLabel("Aucun vaccin détecté");
    vaccinInfoLabel->setWordWrap(true);
    vaccinInfoLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    vaccinInfoLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    vaccinInfoLabel->setMinimumHeight(100);

    QPushButton *refreshButton = new QPushButton("Rafraîchir les ports");

    // Créer le panneau de contrôle Arduino
    QGridLayout *controlLayout = new QGridLayout();
    controlLayout->addWidget(new QLabel("Port:"), 0, 0);
    controlLayout->addWidget(portSelector, 0, 1);
    controlLayout->addWidget(connectButton, 0, 2);
    controlLayout->addWidget(refreshButton, 1, 2);
    controlLayout->addWidget(new QLabel("Statut:"), 2, 0);
    controlLayout->addWidget(statusLabel, 2, 1, 1, 2);
    controlLayout->addWidget(new QLabel("Couleur détectée:"), 3, 0);
    controlLayout->addWidget(detectedColorLabel, 3, 1, 1, 2);

    // Ajouter une section pour les informations du vaccin
    QLabel *vaccinInfoTitle = new QLabel("Informations du vaccin:");
    vaccinInfoTitle->setStyleSheet("font-weight: bold;");
    controlLayout->addWidget(vaccinInfoTitle, 4, 0);
    controlLayout->addWidget(vaccinInfoLabel, 5, 0, 1, 3);

    // Ajouter au layout principal
    arduinoLayout->addWidget(titleLabel);
    arduinoLayout->addLayout(controlLayout);
    arduinoLayout->addStretch();

    // Créer un groupe pour encadrer le panneau
    QGroupBox *arduinoGroup = new QGroupBox("Détection Arduino");
    arduinoGroup->setLayout(arduinoLayout);

    // Ajouter à la barre latérale ou une autre partie de votre interface
    // Par exemple, on peut l'ajouter à droite du tableau de vaccins
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(vaccinsTablePage, 3); // Votre tableau existant
    mainLayout->addWidget(arduinoGroup, 1);     // Panneau Arduino

    // Remplacer le layout existant de votre page principale
    QWidget *container = new QWidget();
    container->setLayout(mainLayout);

    // Ajouter cette page à votre stackedWidget
    stackedWidget->addWidget(container);
    stackedWidget->setCurrentWidget(container);

    // Connecter les signaux et slots
    connect(connectButton, &QPushButton::clicked, this, [this]() {
        if (arduinoManager->isConnected()) {
            arduinoManager->disconnectFromArduino();
            connectButton->setText("Connecter");
        } else {
            QString portName = portSelector->currentText().split(" - ").first();
            if (arduinoManager->connectToPort(portName)) {
                connectButton->setText("Déconnecter");
            }
        }
    });

    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        portSelector->clear();
        portSelector->addItems(arduinoManager->availablePorts());
    });

    connect(arduinoManager, &ArduinoManager::connectionStatusChanged, this, [this](bool connected) {
        if (connected) {
            statusLabel->setText("Connecté");
            statusLabel->setStyleSheet("color: green;");
            connectButton->setText("Déconnecter");
        } else {
            statusLabel->setText("Non connecté");
            statusLabel->setStyleSheet("color: red;");
            connectButton->setText("Connecter");
        }
    });
}
void MainWindow::setupArduinoConnection()
{
    // Créer le gestionnaire Arduino
    arduinoManager = new ArduinoManager(this);

    // Connecter les signaux et slots
    connect(arduinoManager, &ArduinoManager::colorDetected, this, &MainWindow::handleColorDetection);
    connect(arduinoManager, &ArduinoManager::vaccinExpirationStatus, this, &MainWindow::handleVaccinExpiration);
    connect(arduinoManager, &ArduinoManager::errorOccurred, this, &MainWindow::handleArduinoError);

    // Configurer le panneau de contrôle Arduino
    setupArduinoPanel();
}

void MainWindow::handleColorDetection(const QString &color)
{
    // Mettre à jour l'affichage de la couleur détectée
    detectedColorLabel->setText(color);
    detectedColorLabel->setStyleSheet("font-weight: bold; color: " + color + ";");

    // Rechercher le vaccin correspondant à cette couleur
    bool found = vaccinManager->findVaccinByColor(color);

    if (found) {
        // Récupérer les informations du vaccin trouvé
        QString vaccinName = vaccinManager->getNomVaccin();
        QString reference = vaccinManager->getReference();
        QDate expDate = vaccinManager->getDateExp();
        QString expDateStr = expDate.toString("dd/MM/yyyy");

        // Vérifier si le vaccin est expiré
        bool isExpired = expDate < QDate::currentDate();

        // Mettre à jour les informations affichées
        QString infoText = QString("Vaccin: %1\nRéférence: %2\nExpiration: %3\nStatut: %4")
                               .arg(vaccinName)
                               .arg(reference)
                               .arg(expDateStr)
                               .arg(isExpired ? "EXPIRÉ" : "VALIDE");

        vaccinInfoLabel->setText(infoText);

        // Appliquer une mise en forme selon le statut d'expiration
        if (isExpired) {
            vaccinInfoLabel->setStyleSheet("color: red; font-weight: bold;");

            // Envoyer une commande à l'Arduino pour activer le buzzer en mode expiré
            if (arduinoManager->isConnected()) {
                arduinoManager->sendCommand("EXPIRE");
            }

            // Afficher une notification d'expiration
            QMessageBox::warning(this, "Vaccin expiré",
                                 QString("Le vaccin %1 (Réf: %2) est expiré depuis le %3!")
                                     .arg(vaccinName)
                                     .arg(reference)
                                     .arg(expDateStr));
        } else {
            vaccinInfoLabel->setStyleSheet("color: green; font-weight: bold;");

            // Ne pas envoyer de commande pour activer le buzzer si le vaccin est valide
            // Afficher uniquement la notification de validité
            QMessageBox::information(this, "Vaccin valide",
                                     QString("Le vaccin %1 (Réf: %2) est valide jusqu'au %3.")
                                         .arg(vaccinName)
                                         .arg(reference)
                                         .arg(expDateStr));
        }
    } else {
        // Aucun vaccin trouvé pour cette couleur
        vaccinInfoLabel->setText("Aucun vaccin associé à cette couleur");
        vaccinInfoLabel->setStyleSheet("color: black;");
    }
}
void MainWindow::handleVaccinExpiration(bool isExpired, const QString &vaccinName, const QString &reference, const QString &expDate)
{
    // Mettre à jour le label d'informations sur le vaccin
    QString infoText = QString("Vaccin: %1\nRéférence: %2\nExpiration: %3\nStatut: %4")
                           .arg(vaccinName)
                           .arg(reference)
                           .arg(expDate)
                           .arg(isExpired ? "EXPIRÉ" : "VALIDE");

    vaccinInfoLabel->setText(infoText);

    // Appliquer une mise en forme selon le statut d'expiration
    if (isExpired) {
        vaccinInfoLabel->setStyleSheet("color: red; font-weight: bold;");

        // Afficher une notification d'expiration
        QMessageBox::warning(this, "Vaccin expiré",
                             QString("Le vaccin %1 (Réf: %2) est expiré depuis le %3!")
                                 .arg(vaccinName)
                                 .arg(reference)
                                 .arg(expDate));

        // Vous pourriez vouloir ajouter ici la commande pour activer le buzzer
        if (arduinoManager->isConnected()) {
            arduinoManager->sendCommand("EXPIRE");
        }
    } else {
        vaccinInfoLabel->setStyleSheet("color: green; font-weight: bold;");

        // Afficher une notification de validité sans activer le buzzer
        QMessageBox::information(this, "Vaccin valide",
                                 QString("Le vaccin %1 (Réf: %2) est valide jusqu'au %3.")
                                     .arg(vaccinName)
                                     .arg(reference)
                                     .arg(expDate));
    }
}
void MainWindow::handleArduinoError(const QString &errorMessage)
{
    // Afficher l'erreur dans la barre d'état ou dans une boîte de dialogue
    statusBar()->showMessage("Erreur Arduino: " + errorMessage, 5000);

    // Mettre à jour le statut de connexion si nécessaire
    if (!arduinoManager->isConnected()) {
        statusLabel->setText("Non connecté");
        statusLabel->setStyleSheet("color: red;");
        connectButton->setText("Connecter");
    }
}
*/
// Navigation Functions
void MainWindow::showPatientsPage() {
    stackedWidget->setCurrentWidget(patientsPage);
    updateSidebarIcons(btnPatients);
    patientsManager->loadPatients();
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



void MainWindow::showVaccinsPage() {
    stackedWidget->setCurrentWidget(vaccinsPage);
    updateSidebarIcons(btnVaccins);
}


void MainWindow::showSettingsPage() {
    stackedWidget->setCurrentWidget(settingsPage);
    updateSidebarIcons(btnSettings);

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

    if (selectedButton == btnPatients){
        btnPatients->setIcon(QIcon(":/icons/svg/patient-selected.svg"));
    patientsManager->loadPatients();}

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



//mouheb



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
                "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #198754, stop:1 #28a745);"
                "    color: white;"
                "    padding: 10px 20px;"
                "    border-radius: 10px;"
                "    font-size: 15px;"
                "    font-weight: bold;"
                "    font-family: 'Segoe UI';"
                "}"
                "QPushButton:hover {"
                "    background-color: #157347;"
                "}"
                );

    // Add "Modifier Materiels" Button
    QPushButton *editResearcherButton = new QPushButton("Modifier Chercheurs", this);
    editResearcherButton->setStyleSheet(
                "QPushButton {"
                "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #198754, stop:1 #28a745);"
                "    color: white;"
                "    padding: 10px 20px;"
                "    border-radius: 10px;"
                "    font-size: 15px;"
                "    font-weight: bold;"
                "    font-family: 'Segoe UI';"
                "}"
                "QPushButton:hover {"
                "    background-color: #157347;"
                "}"
                );

    // Add "Supprimer Materiels" Button
    QPushButton *deleteResearcherButton = new QPushButton("Supprimer Chercheurs", this);
    deleteResearcherButton->setStyleSheet(
                "QPushButton {"
                "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #198754, stop:1 #28a745);"
                "    color: white;"
                "    padding: 10px 20px;"
                "    border-radius: 10px;"
                "    font-size: 15px;"
                "    font-weight: bold;"
                "    font-family: 'Segoe UI';"
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
        "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #198754, stop:1 #28a745);"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border-radius: 10px;"
        "    font-size: 15px;"
        "    font-weight: bold;"
        "    font-family: 'Segoe UI';"
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



