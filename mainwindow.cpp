#include "mainwindow.h"
#include "toolsmanager.h"
#include "patients.h"

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



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();


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
    PatientsTablePage = new QWidget();
    addPatientsFormPage = new QWidget();
    ModifyPatientFormPage = new QWidget();

    // Setup Pages
    setuppatientsPage();
   // setupaddPatientsFormPage();
    setupPatientsTablePage();
    setupaddPatientsFormPage();
    setupModifyPatientFormPage(ID);

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
    stackedWidget->addWidget(PatientsTablePage);
    stackedWidget->addWidget(addPatientsFormPage);
    stackedWidget->addWidget(ModifyPatientFormPage);


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

    // In the dataChanged connection:
    // In setupPatientsTablePage() function
    connect(patientsManager, &PatientsManager::dataChanged, this, [this, patientstable]() {
        patientstable->setRowCount(0);

        QSqlQuery query("SELECT IDP, Nom, Prenom, Sexe, CIN, Adresse, NumTel, MaladieChronique FROM patients ORDER BY IDP DESC");

        int row = 0;
        while (query.next()) {
            patientstable->insertRow(row);

            // Set data for columns
            for (int col = 0; col < 8; col++) {
                patientstable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }

            // Create button with DPI-aware sizing
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



            int patientId = query.value("IDP").toInt();
            connect(historyBtn, &QPushButton::clicked, this, [this, patientId]() {
                showVaccinationHistory(patientId);
            });

            patientstable->setCellWidget(row, 8, historyBtn);
            row++;
        }

    });
    // Connect buttons
    connect(addPatientButton, &QPushButton::clicked, this, &MainWindow::onAddPatientClicked);
    connect(editPatientButton, &QPushButton::clicked, this, &MainWindow::onEditPatientClicked);
    connect(deletePatientButton, &QPushButton::clicked, this, &MainWindow::onDeletePatientClicked);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showPatientsPage);
    connect(pdfButton, &QPushButton::clicked, this, &MainWindow::exportStatsToPDF);

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

void MainWindow::showResearchersPage() {
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
void MainWindow::showPatientsTablePage() {
    stackedWidget->setCurrentWidget(PatientsTablePage);
    patientsManager->loadPatients();
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
