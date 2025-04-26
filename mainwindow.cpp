#include "mainwindow.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QDebug>
#include <QComboBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>
#include <QGridLayout>
#include <QGroupBox>
#include <QChartView>
#include <QtCharts>
#include <QBarSet>
#include <QBarSeries>
#include <QValueAxis>
#include <QPageSize>
#include <QImage>
#include <QByteArray>
#include <QString>
#include <QBuffer>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    personnelManager(new PersonnelManager(new QTableWidget(this), this))
{
    setupUI();
    connect(personnelManager, &PersonnelManager::statsUpdated, this, &MainWindow::updateStats);
}


MainWindow::~MainWindow() {}

void MainWindow::styleTableView(QTableWidget *tableView)
{
    tableView->setStyleSheet(
        "QTableWidget {"
        "   background-color: rgba(255, 255, 255, 200);"
        "   border: 2px solid #230a98;"
        "   border-radius: 8px;"
        "   alternate-background-color: #f0f5ff;"
        "   gridline-color: #c4c4c4;"
        "}"
        "QHeaderView::section {"
        "   background-color: #230a98;"
        "   color: white;"
        "   padding: 6px;"
        "   border: none;"
        "   font-weight: bold;"
        "   font-size: 10pt;"
        "}"
        "QTableWidget::item {"
        "   padding: 5px;"
        "}"
        "QTableWidget::item:selected {"
        "   background-color: #7359f0;"
        "   color: white;"
        "}"
        "QScrollBar::vertical {"
        "   border: 1px solid #c4c4c4;"
        "   background: #f0f0f0;"
        "   width: 12px;"
        "   margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #230a98;"
        "   min-height: 20px;"
        "   border-radius: 4px;"
        "}"
        );

    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    tableView->verticalHeader()->setDefaultSectionSize(35);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSortingEnabled(true);
}

void MainWindow::setCurrentUser(int userId, const QString &nom, const QString &prenom)
{
    currentUserId = userId;
    currentUserName = nom + " " + prenom;

    // Charger les données initiales
    personnelManager->loadPersonnel();
    updateStats(); // Force la mise à jour des graphiques
}

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);

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

    setupSidebar();
    setupPages();

    contentWrapperLayout->addWidget(stackedWidget);

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
    logoLabel->setPixmap(QPixmap(":/icons/svg/BIOLAB.svg"));
    logoLabel->setAlignment(Qt::AlignLeft);
    logoLabel->setStyleSheet("QLabel { margin-left: 20px; margin-top: 10px; }");
    sidebarLayout->addWidget(logoLabel);

    // Boutons principaux
    btnPatients = createSidebarButton("  Patients", ":/icons/svg/patient.svg");
    btnPersonel = createSidebarButton("  Personel", ":/icons/svg/personel.svg");
    btnResearches = createSidebarButton("  Recherche", ":/icons/svg/flask.svg");
    btnResearchers = createSidebarButton("  Chercheurs", ":/icons/svg/scientist.svg");
    btnTools = createSidebarButton("  Matériels", ":/icons/svg/microscope.svg");
    btnVaccins = createSidebarButton("  Vaccin", ":/icons/svg/syringe.svg");
    btnSettings = createSidebarButton("  Paramètres", ":/icons/svg/settings.svg");

    // Groupe de boutons
    sidebarButtonGroup = new QButtonGroup(this);
    sidebarButtonGroup->addButton(btnPatients);
    sidebarButtonGroup->addButton(btnPersonel);
    sidebarButtonGroup->addButton(btnResearches);
    sidebarButtonGroup->addButton(btnResearchers);
    sidebarButtonGroup->addButton(btnTools);
    sidebarButtonGroup->addButton(btnVaccins);
    sidebarButtonGroup->addButton(btnSettings);
    sidebarButtonGroup->setExclusive(true);

    // Ajout des boutons principaux
    sidebarLayout->addWidget(btnPatients);
    sidebarLayout->addWidget(btnPersonel);
    sidebarLayout->addWidget(btnResearches);
    sidebarLayout->addWidget(btnResearchers);
    sidebarLayout->addWidget(btnTools);
    sidebarLayout->addWidget(btnVaccins);
    sidebarLayout->addWidget(btnSettings);

    // Bouton de déconnexion
    btnLogout = new QPushButton("  Déconnexion", this);
    btnLogout->setIcon(QIcon(":/icons/svg/logout.svg")); // Ajoutez une icône si disponible
    btnLogout->setStyleSheet(
        "QPushButton {"
        "    background: transparent;"
        "    color: #e74c3c;"
        "    text-align: left;"
        "    padding: 15px;"
        "    font-size: 16px;"
        "    border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "    background: rgba(231, 76, 60, 0.1);"
        "}"
        "QPushButton::icon {"
        "    margin-right: 10px;"
        "}"
        );

    // Image en bas
    QLabel *PicLabel = new QLabel(this);
    PicLabel->setPixmap(QPixmap(":/icons/svg/Asset 2.svg").scaled(22, 22, Qt::KeepAspectRatio));
    PicLabel->setStyleSheet("QLabel { margin-left: 8px; margin-bottom: 50px; }");

    // Organisation finale
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(btnLogout);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(PicLabel);

    // Connexions
    connect(btnPersonel, &QPushButton::clicked, this, &MainWindow::showPersonelPage);
    connect(btnLogout, &QPushButton::clicked, this, &MainWindow::logout);
    connect(btnPatients, &QPushButton::clicked, this, &MainWindow::showPatientsPage);
    connect(btnResearches, &QPushButton::clicked, this, &MainWindow::showResearchesPage);
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
void MainWindow::setupPersonelPage() {
    personelPage = new QWidget();
    QGridLayout *layout = new QGridLayout(personelPage);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(20);

    // 1. Graphique des villes (haut gauche)
    QGroupBox *villeBox = new QGroupBox("Répartition par ville");
    villeBox->setStyleSheet("QGroupBox { font-weight: bold; font-size: 16px; color: #333; border: 1px solid #e0e0e0; border-radius: 8px; }");
    villeChartView = new QChartView(personnelManager->createVillePieChart());
    villeChartView->setRenderHint(QPainter::Antialiasing);
    villeChartView->setMinimumSize(400, 300);
    QVBoxLayout *villeLayout = new QVBoxLayout(villeBox);
    villeLayout->addWidget(villeChartView);
    layout->addWidget(villeBox, 0, 0);

    // 2. Graphique des genres (haut droit)
    QGroupBox *genderBox = new QGroupBox("Répartition par genre");
    genderBox->setStyleSheet("QGroupBox { font-weight: bold; font-size: 16px; color: #333; border: 1px solid #e0e0e0; border-radius: 8px; }");
    genderChartView = new QChartView(personnelManager->createGenderBarChart());
    genderChartView->setRenderHint(QPainter::Antialiasing);
    genderChartView->setMinimumSize(400, 300);
    QVBoxLayout *genderLayout = new QVBoxLayout(genderBox);
    genderLayout->addWidget(genderChartView);
    layout->addWidget(genderBox, 0, 1);

    // 3. Statistiques (bas gauche)
    QGroupBox *statsBox = new QGroupBox("Statistiques");
    statsBox->setStyleSheet("QGroupBox { font-weight: bold; font-size: 16px; color: #333; border: 1px solid #e0e0e0; border-radius: 8px; }");
    QVBoxLayout *statsLayout = new QVBoxLayout(statsBox);

    int total = personnelManager->getTotalPersonnel();
    auto roleCounts = personnelManager->getRoleCounts();

    totalLabel = new QLabel(QString("Total personnel: %1").arg(total));
    totalLabel->setStyleSheet("font-size: 14px; color: #333; padding: 8px; font-weight: bold;");

    infirmiersLabel = new QLabel(QString("Infirmiers: %1").arg(roleCounts.first));
    infirmiersLabel->setStyleSheet("font-size: 14px; color: #198754; padding: 8px; font-weight: bold;");

    secretairesLabel = new QLabel(QString("Secrétaires: %1").arg(roleCounts.second));
    secretairesLabel->setStyleSheet("font-size: 14px; color: #6c757d; padding: 8px; font-weight: bold;");

    statsLayout->addWidget(totalLabel);
    statsLayout->addWidget(infirmiersLabel);
    statsLayout->addWidget(secretairesLabel);
    layout->addWidget(statsBox, 1, 0);

    // 4. Bouton Voir plus seulement (bas droit)
    QGroupBox *buttonBox = new QGroupBox("Personnel");
    buttonBox->setStyleSheet("QGroupBox { font-weight: bold; font-size: 16px; color: #333; border: 1px solid #e0e0e0; border-radius: 8px; }");
    QVBoxLayout *buttonLayout = new QVBoxLayout(buttonBox);
    buttonLayout->setAlignment(Qt::AlignCenter);

    voirPlusButton = new QPushButton("Voir plus");
    voirPlusButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #198754;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border-radius: 6px;"
        "    font-size: 14px;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: #157347;"
        "}"
        );

    buttonLayout->addWidget(voirPlusButton);
    layout->addWidget(buttonBox, 1, 1);

    connect(voirPlusButton, &QPushButton::clicked, this, &MainWindow::showPersonnelTablePage);

    stackedWidget->addWidget(personelPage);
}

void MainWindow::setupPages() {
    stackedWidget = new QStackedWidget(this);

    patientsPage = new QWidget();
    researchersPage = new QWidget();
    settingsPage = new QWidget();
    vaccinsPage = new QWidget();
    personelPage = new QWidget();
    toolsPage = new QWidget();
    researchesPage = new QWidget();
    toolsTablePage = new QWidget();
    addToolFormPage = new QWidget();
    PersonelTablePage = new QWidget();
    addPersonelFormPage = new QWidget();
    editPersonelFormPage = new QWidget();

    setupPersonelPage();
    setupPersonelTablePage();
    setupAddPersonelFormPage();
    setupEditPersonelFormPage();

    stackedWidget->addWidget(patientsPage);
    stackedWidget->addWidget(researchersPage);
    stackedWidget->addWidget(vaccinsPage);
    stackedWidget->addWidget(personelPage);
    stackedWidget->addWidget(researchesPage);
    stackedWidget->addWidget(toolsPage);
    stackedWidget->addWidget(settingsPage);
    stackedWidget->addWidget(toolsTablePage);
    stackedWidget->addWidget(addToolFormPage);
    stackedWidget->addWidget(PersonelTablePage);
    stackedWidget->addWidget(addPersonelFormPage);
    stackedWidget->addWidget(editPersonelFormPage);

    stackedWidget->setCurrentWidget(patientsPage);
}

void MainWindow::setupPersonelTablePage() {
    QVBoxLayout *mainLayout = new QVBoxLayout(PersonelTablePage);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // Header
    QWidget *headerWidget = new QWidget();
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    backButton = new QPushButton();
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

    QLabel *titleLabel = new QLabel("Gestion du personnel");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333;");

    headerLayout->addWidget(backButton);
    headerLayout->addWidget(titleLabel, 1);
    headerLayout->addStretch();
    mainLayout->addWidget(headerWidget);

    // CRUD buttons
    QHBoxLayout *crudLayout = new QHBoxLayout();
    crudLayout->setSpacing(10);

    QString buttonStyle =
        "QPushButton {"
        "    background-color: #198754;"
        "    color: white;"
        "    padding: 10px 15px;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "    min-width: 120px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #157347;"
        "}";

    QPushButton *addButton = new QPushButton("Ajouter");
    QPushButton *editButton = new QPushButton("Modifier");
    QPushButton *deleteButton = new QPushButton("Supprimer");

    addButton->setStyleSheet(buttonStyle);
    editButton->setStyleSheet(buttonStyle);
    deleteButton->setStyleSheet(buttonStyle);

    crudLayout->addWidget(addButton);
    crudLayout->addWidget(editButton);
    crudLayout->addWidget(deleteButton);
    crudLayout->addStretch();
    mainLayout->addLayout(crudLayout);

    // Dans setupPersonelTablePage(), après les autres boutons CRUD
    QPushButton *exportButton = new QPushButton("Exporter PDF");
    exportButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #dc3545;"
        "    color: white;"
        "    padding: 10px 15px;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "    min-width: 120px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #bb2d3b;"
        "}"
        );
    crudLayout->addWidget(exportButton);  // Ajoutez ce bouton au layout

    // Connectez le signal (cette partie doit être après avoir créé exportButton)
    connect(exportButton, &QPushButton::clicked, this, [this]() {
        QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
                              "/personnel_biolab_" +
                              QDate::currentDate().toString("yyyyMMdd") +
                              ".pdf";

        QString fileName = QFileDialog::getSaveFileName(
            this,
            "Exporter en PDF",
            defaultPath,
            "Fichiers PDF (*.pdf)"
            );

        if (!fileName.isEmpty()) {
            if (personnelManager->exportToPDF(fileName)) {
                QMessageBox::information(this, "Succès", "Le PDF a été généré avec succès!");
            } else {
                QMessageBox::warning(this, "Erreur", "Échec lors de la génération du PDF");
            }
        }
    });

    // Search and filter bar
    QHBoxLayout *searchFilterLayout = new QHBoxLayout();

    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Rechercher...");
    searchInput->setClearButtonEnabled(true);
    searchInput->setStyleSheet(
        "QLineEdit {"
        "    padding: 8px;"
        "    border: 1px solid #ddd;"
        "    border-radius: 6px;"
        "    min-width: 250px;"
        "}"
        );

    // Ville filter combo
    villeFilterCombo = new QComboBox();
    villeFilterCombo->addItem("Toutes les villes");
    villeFilterCombo->addItems(personnelManager->getVillesTunisie());
    villeFilterCombo->setStyleSheet(
        "QComboBox {"
        "    padding: 8px;"
        "    border: 1px solid #ddd;"
        "    border-radius: 6px;"
        "    min-width: 180px;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "}"
        );

    // Sort button
    sortRecentButton = new QPushButton("Trier par plus récent");
    sortRecentButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #230a98;"
        "    color: white;"
        "    padding: 8px 15px;"
        "    border-radius: 6px;"
        "    font-size: 13px;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: #3a1fc2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1a0775;"
        "}");

    searchFilterLayout->addWidget(searchInput);
    searchFilterLayout->addWidget(villeFilterCombo);
    searchFilterLayout->addStretch();
    searchFilterLayout->addWidget(sortRecentButton);
    mainLayout->addLayout(searchFilterLayout);

    // Table configuration
    QTableWidget *personnelTable = personnelManager->getTable();
    personnelTable->setParent(PersonelTablePage);
    personnelTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    styleTableView(personnelTable);
    mainLayout->addWidget(personnelTable);

    // Connect signals
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showPersonelPage);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddPersonelClicked);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::onEditPersonelClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeletePersonelClicked);
    connect(searchInput, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    connect(villeFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onFilterByVille);
    connect(sortRecentButton, &QPushButton::clicked, this, &MainWindow::onSortByRecent);

    // Initial data load
    personnelManager->loadPersonnel();
}

void MainWindow::showPersonnelTablePage() {
    personnelManager->loadPersonnel(); // Appel sans paramètres
    stackedWidget->setCurrentWidget(PersonelTablePage);
    updateSidebarIcons(btnPersonel);
}

void MainWindow::showPersonelPage() {
    stackedWidget->setCurrentWidget(personelPage);
    updateSidebarIcons(btnPersonel);
}
void MainWindow::showPatientsPage() {
    stackedWidget->setCurrentWidget(patientsPage);
    updateSidebarIcons(btnPatients);
}

void MainWindow::showResearchesPage() {
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

void MainWindow::onSearchTextChanged(const QString &text) {
    personnelManager->searchPersonnel(text);
}

void MainWindow::onFilterByVille(int index) {
    QString ville = villeFilterCombo->itemText(index);
    if (ville == "Toutes les villes") {
        personnelManager->loadPersonnel(); // Appel sans paramètres (utilisera les valeurs par défaut)
    } else {
        personnelManager->filterByVille(ville);
    }
}

void MainWindow::onSortByRecent() {
    personnelManager->loadPersonnel("DATEAJOUT", "DESC"); // Appel avec paramètres explicites
}

void MainWindow::onAddPersonelClicked() {
    // Réinitialiser les champs du formulaire
    nomEdit->clear();
    prenomEdit->clear();
    cinEdit->clear();
    numTelEdit->clear();
    emailEdit->clear();
    villeEdit->setCurrentIndex(0);
    roleEdit->setCurrentIndex(0);
    sexeEdit->setCurrentIndex(0);
    dateAjoutEdit->setDate(QDate::currentDate());

    stackedWidget->setCurrentWidget(addPersonelFormPage);
}

void MainWindow::onEditPersonelClicked() {
    QTableWidget *table = personnelManager->getTable();
    if (table->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un membre du personnel à modifier");
        return;
    }

    int row = table->currentRow();
    int id = table->item(row, 0)->text().toInt();

    QMap<QString, QVariant> personnelData = personnelManager->getPersonnelById(id);
    if (personnelData.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les données du personnel");
        return;
    }

    // Remplir le formulaire d'édition
    nomEdit->setText(personnelData["NOM"].toString());
    prenomEdit->setText(personnelData["PRENOM"].toString());
    cinEdit->setText(personnelData["CIN"].toString());
    numTelEdit->setText(personnelData["NUMTEL"].toString());
    emailEdit->setText(personnelData["EMAIL"].toString());
    villeEdit->setCurrentText(personnelData["ADRESSE"].toString());
    roleEdit->setCurrentText(personnelData["ROLE"].toString());
    sexeEdit->setCurrentText(personnelData["SEXE"].toString());
    dateAjoutEdit->setDate(QDate::fromString(personnelData["DATEAJOUT"].toString(), "dd/MM/yyyy"));

    personnelManager->setIdPer(id);
    stackedWidget->setCurrentWidget(editPersonelFormPage);
}

void MainWindow::onDeletePersonelClicked() {
    QTableWidget *table = personnelManager->getTable();
    if (table->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un membre du personnel à supprimer");
        return;
    }

    int row = table->currentRow();
    int id = table->item(row, 0)->text().toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Êtes-vous sûr de vouloir supprimer ce membre du personnel?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        personnelManager->setIdPer(id);
        if (personnelManager->deletePersonnel()) {
            QMessageBox::information(this, "Succès", "Personnel supprimé avec succès");
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la suppression du personnel");
        }
    }
}

void MainWindow::onBackToSummaryClicked() {
    stackedWidget->setCurrentWidget(personelPage);
}

void MainWindow::setupAddPersonelFormPage()
{
    addPersonelFormPage = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(addPersonelFormPage);
    mainLayout->setSpacing(15);

    // Titre
    QLabel *titleLabel = new QLabel("Ajout d'un Personnel");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px; color: #333;");
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    // Zone scrollable
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    QWidget *formContainer = new QWidget();
    QVBoxLayout *formLayout = new QVBoxLayout(formContainer);
    formLayout->setSpacing(10);

    // Layout des champs
    QFormLayout *inputLayout = new QFormLayout();
    inputLayout->setSpacing(10);
    inputLayout->setLabelAlignment(Qt::AlignRight);

    // Champs formulaire
    QLineEdit *nomInput = new QLineEdit();
    QLineEdit *prenomInput = new QLineEdit();
    QComboBox *sexeInput = new QComboBox();
    QLineEdit *cinInput = new QLineEdit();
    QComboBox *villeInput = new QComboBox();
    QLineEdit *numTelInput = new QLineEdit();
    QLineEdit *emailInput = new QLineEdit();
    QComboBox *roleInput = new QComboBox();
    QDateEdit *dateAjoutInput = new QDateEdit(QDate::currentDate());
    QComboBox *questionSecreteInput = new QComboBox();
    QLineEdit *reponseSecreteInput = new QLineEdit();

    // Validation des entrées
    QRegularExpressionValidator *nomValidator = new QRegularExpressionValidator(
        QRegularExpression("^[a-zA-ZÀ-ÿ-]+$"), nomInput);
    QRegularExpressionValidator *prenomValidator = new QRegularExpressionValidator(
        QRegularExpression("^[a-zA-ZÀ-ÿ-]+$"), prenomInput);
    QRegularExpressionValidator *cinValidator = new QRegularExpressionValidator(
        QRegularExpression("^\\d{8}$"), cinInput);
    QRegularExpressionValidator *telValidator = new QRegularExpressionValidator(
        QRegularExpression("^\\d{8}$"), numTelInput);

    nomInput->setValidator(nomValidator);
    prenomInput->setValidator(prenomValidator);
    cinInput->setValidator(cinValidator);
    numTelInput->setValidator(telValidator);

    // Liste des questions secrètes
    QStringList questions = { "Quel était le nom de votre premier animal de compagnie?",
                             "Quel est le nom de jeune fille de votre mère?",
                             "Dans quelle ville êtes-vous né(e)?",
                             "Quel était votre surnom d'enfance?",
                             "Quel est votre livre préféré?",
                             "Quel est le nom de votre meilleur ami d'enfance?",
                             "Quel était le nom de votre école primaire?",
                             "Quel est votre plat préféré?",
                             "Quel est le modèle de votre première voiture?",
                             "Quel est votre sport préféré?",
                             "Quel est le nom de votre professeur préféré?",
                             "Quel est votre groupe de musique préféré?",
                             "Quel est votre film préféré?",
                             "Quel était votre rêve professionnel étant enfant?",
                             "Quel est le nom de votre premier employeur?",
                             "Quel est votre destination de vacances préférée?",
                             "Quel est votre hobby préféré?",
                             "Quel est votre artiste préféré?",
                             "Quel est votre jeu vidéo préféré?",
                             "Quel est votre citation préférée?"
    };

    for (int i = 0; i < questions.size(); ++i) {
        questionSecreteInput->addItem(questions[i], i);
    }

    // Section Photo
    QLabel *photoSectionLabel = new QLabel("Photo d'identité");
    photoSectionLabel->setStyleSheet("font-weight: bold; color: #2c3e50; margin-top: 15px;");
    formLayout->addWidget(photoSectionLabel);

    QLabel *photoLabel = new QLabel();
    QPushButton *uploadPhotoButton = new QPushButton("Choisir une photo");
    QLabel *photoInfoLabel = new QLabel("Formats supportés: JPG, PNG (Max. 2MB)");
    photoInfoLabel->setStyleSheet("font-size: 12px; color: #7f8c8d;");

    photoLabel->setFixedSize(150, 180);
    photoLabel->setStyleSheet(
        "border: 2px dashed #95a5a6; background-color: #f9f9f9; padding: 5px;"
        );
    photoLabel->setAlignment(Qt::AlignCenter);
    photoLabel->setText("Aucune photo\nsélectionnée");

    QHBoxLayout *photoLayout = new QHBoxLayout();
    photoLayout->addWidget(photoLabel);

    QVBoxLayout *photoButtonLayout = new QVBoxLayout();
    photoButtonLayout->addWidget(uploadPhotoButton);
    photoButtonLayout->addWidget(photoInfoLabel);
    photoButtonLayout->addStretch();
    photoLayout->addLayout(photoButtonLayout);

    formLayout->addLayout(photoLayout);

    // Peuplement des combobox (inchangé)
    sexeInput->addItems({"Homme", "Femme"});
    villeInput->addItems(personnelManager->getVillesTunisie());
    roleInput->addItems({"infirmier", "secretaire"});
    dateAjoutInput->setDisplayFormat("dd/MM/yyyy");
    dateAjoutInput->setCalendarPopup(true);

    // Ajout des champs (inchangé)
    inputLayout->addRow("Nom* (sans espaces):", nomInput);
    inputLayout->addRow("Prénom* (sans espaces):", prenomInput);
    inputLayout->addRow("Sexe:", sexeInput);
    inputLayout->addRow("CIN* (8 chiffres):", cinInput);
    inputLayout->addRow("Ville:", villeInput);
    inputLayout->addRow("Téléphone* (8 chiffres):", numTelInput);
    inputLayout->addRow("Email:", emailInput);
    inputLayout->addRow("Rôle:", roleInput);
    inputLayout->addRow("Date d'ajout:", dateAjoutInput);
    inputLayout->addRow("Question secrète:", questionSecreteInput);
    inputLayout->addRow("Réponse secrète*:", reponseSecreteInput);

    formLayout->addLayout(inputLayout);
    formLayout->addStretch();

    // Configuration scroll
    scrollArea->setWidget(formContainer);
    mainLayout->addWidget(scrollArea);

    // Boutons (style inchangé)
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *submitButton = new QPushButton("Ajouter");
    QPushButton *backButton = new QPushButton("Retour");
    buttonLayout->addStretch();
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(submitButton);
    mainLayout->addLayout(buttonLayout);

    addPersonelFormPage->setStyleSheet(R"(
        QLabel { font-size: 14px; font-weight: bold; margin-top: 5px; color: #333; }
        QLineEdit, QDateEdit { padding: 8px; border: 1px solid #ddd; border-radius: 4px; margin-bottom: 5px; background-color: white; }
        QComboBox { padding: 8px; border: 1px solid #ddd; border-radius: 4px; margin-bottom: 5px; background-color: white; }
        QComboBox::drop-down { subcontrol-origin: padding; subcontrol-position: top right; width: 15px; border-left: 1px solid #ddd; }
        QPushButton { background-color: #198754; color: white; padding: 10px 20px; border-radius: 8px; min-width: 100px; }
        QPushButton:hover { background-color: #157347; }
        QScrollArea { border: none; }
        QLineEdit:invalid { background-color: #FFDDDD; }
    )");

    // Gestion photo MODIFIÉE
    connect(uploadPhotoButton, &QPushButton::clicked, this, [this, photoLabel]() {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        "Choisir une photo",
                                                        QDir::homePath(),
                                                        "Images (*.png *.jpg *.jpeg);;Tous les fichiers (*)");

        if (!fileName.isEmpty()) {
            try {
                QFile file(fileName);
                if (file.size() > 2 * 1024 * 1024) {
                    throw std::runtime_error("La photo doit faire moins de 2MB");
                }

                QImage image(fileName);
                if (image.isNull()) {
                    throw std::runtime_error("Impossible de charger l'image");
                }

                // Redimensionnement
                if (image.width() > 500) {
                    image = image.scaledToWidth(500, Qt::SmoothTransformation);
                }

                // Vérification visage
                QString tempPath = QCoreApplication::applicationDirPath() + "/temp_face_check.jpg";
                image.save(tempPath, "JPG", 80);

                QProcess faceCheck;
                faceCheck.start("python", QStringList() << "check_face.py" << tempPath);

                if (!faceCheck.waitForFinished(3000) || faceCheck.exitCode() != 0) {
                    throw std::runtime_error("Aucun visage détecté ou photo invalide");
                }

                // Affichage vignette
                QPixmap pixmap = QPixmap::fromImage(image);
                pixmap = pixmap.scaled(photoLabel->width(), photoLabel->height(),
                                       Qt::KeepAspectRatio, Qt::SmoothTransformation);
                photoLabel->setPixmap(pixmap);
                photoLabel->setProperty("photoPath", fileName);
                QFile::remove(tempPath);

            } catch (const std::exception& e) {
                QMessageBox::warning(this, "Erreur photo", e.what());
                photoLabel->setText("Aucune photo\nsélectionnée");
                photoLabel->setProperty("photoPath", "");
                QFile::remove(QCoreApplication::applicationDirPath() + "/temp_face_check.jpg");
            }
        }
    });

    // Gestion soumission MODIFIÉE
    connect(submitButton, &QPushButton::clicked, this,
            [this, nomInput, prenomInput, cinInput, numTelInput,
             emailInput, roleInput, villeInput, sexeInput, dateAjoutInput,
             photoLabel, questionSecreteInput, reponseSecreteInput]() {

                // Validation (inchangée)
                QStringList errors;
                if (nomInput->text().trimmed().isEmpty())
                    errors << "Nom obligatoire";
                else if (nomInput->text().contains(" "))
                    errors << "Le nom ne doit pas contenir d'espaces";

                if (prenomInput->text().trimmed().isEmpty())
                    errors << "Prénom obligatoire";
                else if (prenomInput->text().contains(" "))
                    errors << "Le prénom ne doit pas contenir d'espaces";

                if (cinInput->text().length() != 8)
                    errors << "CIN doit avoir exactement 8 chiffres";

                if (numTelInput->text().length() != 8)
                    errors << "Téléphone doit avoir exactement 8 chiffres";

                if (reponseSecreteInput->text().trimmed().isEmpty())
                    errors << "Réponse secrète obligatoire";

                if (!errors.isEmpty()) {
                    QMessageBox::warning(this, "Erreurs de validation", errors.join("\n"));
                    return;
                }

                // Conversion photo MODIFIÉE
                QString photoBase64 = "";
                QString photoPath = photoLabel->property("photoPath").toString();
                if (!photoPath.isEmpty()) {
                    photoBase64 = convertImageToBase64(photoPath);
                    if (photoBase64.isEmpty()) {
                        QMessageBox::warning(this, "Erreur", "Échec de la conversion de la photo");
                        return;
                    }
                }

                // Configuration manager (inchangée)
                personnelManager->setNom(nomInput->text().trimmed());
                personnelManager->setPrenom(prenomInput->text().trimmed());
                personnelManager->setSexe(sexeInput->currentText());
                personnelManager->setCin(cinInput->text());
                personnelManager->setAdresse(villeInput->currentText());
                personnelManager->setNumTel(numTelInput->text());
                personnelManager->setEmail(emailInput->text().trimmed());
                personnelManager->setRole(roleInput->currentText());
                personnelManager->setDateAjout(dateAjoutInput->date());
                personnelManager->setPhotoBase64(photoBase64);
                personnelManager->setQuestionIndex(questionSecreteInput->currentData().toInt());
                personnelManager->setReponseSecrete(reponseSecreteInput->text().trimmed());

                // Tentative d'ajout (inchangée)
                if (personnelManager->addPersonnel()) {
                    QMessageBox::information(this, "Succès", "Personnel ajouté avec succès!");
                    // Réinitialisation (inchangée)
                } else {
                    QMessageBox::warning(this, "Erreur", "Échec de l addition du personnel");
                }
            });

    connect(backButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(PersonelTablePage);
    });
}

void MainWindow::setupEditPersonelFormPage() {
    editPersonelFormPage = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(editPersonelFormPage);
    mainLayout->setSpacing(15);

    // Titre
    QLabel *titleLabel = new QLabel("Modification d'un Personnel");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px; color: #333;");
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    // Zone scrollable
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    QWidget *formContainer = new QWidget();
    QVBoxLayout *formLayout = new QVBoxLayout(formContainer);
    formLayout->setSpacing(10);

    // Layout des champs
    QFormLayout *inputLayout = new QFormLayout();
    inputLayout->setSpacing(10);
    inputLayout->setLabelAlignment(Qt::AlignRight);

    // Champs formulaire
    nomEdit = new QLineEdit();
    prenomEdit = new QLineEdit();
    sexeEdit = new QComboBox();
    cinEdit = new QLineEdit();
    villeEdit = new QComboBox();
    numTelEdit = new QLineEdit();
    emailEdit = new QLineEdit();
    roleEdit = new QComboBox();
    dateAjoutEdit = new QDateEdit(QDate::currentDate());

    // Validation des entrées
    QRegularExpressionValidator *cinValidator = new QRegularExpressionValidator(QRegularExpression("^\\d{8}$"), cinEdit);
    QRegularExpressionValidator *telValidator = new QRegularExpressionValidator(QRegularExpression("^\\d{8}$"), numTelEdit);
    cinEdit->setValidator(cinValidator);
    numTelEdit->setValidator(telValidator);

    // Section Photo
    QLabel *photoSectionLabel = new QLabel("Photo d'identité");
    photoSectionLabel->setStyleSheet("font-weight: bold; color: #2c3e50; margin-top: 15px;");
    formLayout->addWidget(photoSectionLabel);

    QLabel *photoLabel = new QLabel();
    photoLabel->setFixedSize(150, 180);
    photoLabel->setStyleSheet(
        "border: 2px dashed #95a5a6; background-color: #f9f9f9; padding: 5px;"
        );
    photoLabel->setAlignment(Qt::AlignCenter);
    photoLabel->setText("Aucune photo\nsélectionnée");

    QPushButton *uploadPhotoButton = new QPushButton("Changer la photo");
    QLabel *photoInfoLabel = new QLabel("Formats supportés: JPG, PNG (Max. 2MB)");
    photoInfoLabel->setStyleSheet("font-size: 12px; color: #7f8c8d;");

    QHBoxLayout *photoLayout = new QHBoxLayout();
    photoLayout->addWidget(photoLabel);

    QVBoxLayout *photoButtonLayout = new QVBoxLayout();
    photoButtonLayout->addWidget(uploadPhotoButton);
    photoButtonLayout->addWidget(photoInfoLabel);
    photoButtonLayout->addStretch();
    photoLayout->addLayout(photoButtonLayout);

    formLayout->addLayout(photoLayout);

    // Peuplement des combobox
    sexeEdit->addItems({"Homme", "Femme"});
    villeEdit->addItems(personnelManager->getVillesTunisie());
    roleEdit->addItems({"infirmier", "secretaire"});
    dateAjoutEdit->setDisplayFormat("dd/MM/yyyy");
    dateAjoutEdit->setCalendarPopup(true);

    // Ajout des champs
    inputLayout->addRow("Nom*:", nomEdit);
    inputLayout->addRow("Prénom*:", prenomEdit);
    inputLayout->addRow("Sexe:", sexeEdit);
    inputLayout->addRow("CIN* (8 chiffres):", cinEdit);
    inputLayout->addRow("Ville:", villeEdit);
    inputLayout->addRow("Téléphone* (8 chiffres):", numTelEdit);
    inputLayout->addRow("Email:", emailEdit);
    inputLayout->addRow("Rôle:", roleEdit);
    inputLayout->addRow("Date d'ajout:", dateAjoutEdit);

    formLayout->addLayout(inputLayout);
    formLayout->addStretch();

    // Configuration scroll
    scrollArea->setWidget(formContainer);
    mainLayout->addWidget(scrollArea);

    // Boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *submitButton = new QPushButton("Enregistrer");
    QPushButton *cancelButton = new QPushButton("Annuler");

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(submitButton);
    mainLayout->addLayout(buttonLayout);

    // Style
    editPersonelFormPage->setStyleSheet(R"(
        QLabel { font-size: 14px; font-weight: bold; margin-top: 5px; color: #333; }
        QLineEdit, QDateEdit { padding: 8px; border: 1px solid #ddd; border-radius: 4px; margin-bottom: 5px; background-color: white; }
        QComboBox { padding: 8px; border: 1px solid #ddd; border-radius: 4px; margin-bottom: 5px; background-color: white; }
        QComboBox::drop-down { subcontrol-origin: padding; subcontrol-position: top right; width: 15px; border-left: 1px solid #ddd; }
        QPushButton { padding: 10px 20px; border-radius: 8px; min-width: 100px; }
        QPushButton#submitButton { background-color: #198754; color: white; }
        QPushButton#submitButton:hover { background-color: #157347; }
        QPushButton#cancelButton { background-color: #f8f9fa; color: #6c757d; border: 1px solid #dee2e6; }
        QPushButton#cancelButton:hover { background-color: #e2e6ea; }
        QScrollArea { border: none; }
    )");

    submitButton->setObjectName("submitButton");
    cancelButton->setObjectName("cancelButton");

    // Gestion photo
    connect(uploadPhotoButton, &QPushButton::clicked, this, [this, photoLabel]() {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        "Choisir une photo",
                                                        QDir::homePath(),
                                                        "Images (*.png *.jpg *.jpeg);;Tous les fichiers (*)");

        if (!fileName.isEmpty()) {
            try {
                QFile file(fileName);
                if (file.size() > 2 * 1024 * 1024) {
                    throw std::runtime_error("La photo doit faire moins de 2MB");
                }

                QImage image(fileName);
                if (image.isNull()) {
                    throw std::runtime_error("Impossible de charger l'image");
                }

                // Redimensionnement
                if (image.width() > 500) {
                    image = image.scaledToWidth(500, Qt::SmoothTransformation);
                }

                // Vérification visage
                QString tempPath = QCoreApplication::applicationDirPath() + "/temp_face_check.jpg";
                image.save(tempPath, "JPG", 80);

                QProcess faceCheck;
                faceCheck.start("python", QStringList() << "check_face.py" << tempPath);

                if (!faceCheck.waitForFinished(3000) || faceCheck.exitCode() != 0) {
                    throw std::runtime_error("Aucun visage détecté ou photo invalide");
                }

                // Affichage vignette
                QPixmap pixmap = QPixmap::fromImage(image);
                pixmap = pixmap.scaled(photoLabel->width(), photoLabel->height(),
                                       Qt::KeepAspectRatio, Qt::SmoothTransformation);
                photoLabel->setPixmap(pixmap);
                photoLabel->setProperty("photoPath", fileName);
                QFile::remove(tempPath);

            } catch (const std::exception& e) {
                QMessageBox::warning(this, "Erreur photo", e.what());
                photoLabel->setText("Aucune photo\nsélectionnée");
                photoLabel->setProperty("photoPath", "");
                QFile::remove(QCoreApplication::applicationDirPath() + "/temp_face_check.jpg");
            }
        }
    });

    connect(submitButton, &QPushButton::clicked, this, [this, photoLabel]() {
        // Validation
        QStringList errors;
        if (nomEdit->text().trimmed().isEmpty()) errors << "Nom obligatoire";
        if (prenomEdit->text().trimmed().isEmpty()) errors << "Prénom obligatoire";
        if (cinEdit->text().length() != 8) errors << "CIN doit avoir exactement 8 chiffres";
        if (numTelEdit->text().length() != 8) errors << "Téléphone doit avoir exactement 8 chiffres";

        if (!errors.isEmpty()) {
            QMessageBox::warning(this, "Erreurs de validation", errors.join("\n"));
            return;
        }

        // Configuration du manager
        personnelManager->setNom(nomEdit->text().trimmed());
        personnelManager->setPrenom(prenomEdit->text().trimmed());
        personnelManager->setSexe(sexeEdit->currentText());
        personnelManager->setCin(cinEdit->text());
        personnelManager->setAdresse(villeEdit->currentText());
        personnelManager->setNumTel(numTelEdit->text());
        personnelManager->setEmail(emailEdit->text().trimmed());
        personnelManager->setRole(roleEdit->currentText());
        personnelManager->setDateAjout(dateAjoutEdit->date());

        // Conversion photo si nouvelle
        QString photoPath = photoLabel->property("photoPath").toString();
        if (!photoPath.isEmpty()) {
            QString photoBase64 = convertImageToBase64(photoPath);
            if (photoBase64.isEmpty()) {
                QMessageBox::warning(this, "Erreur", "Échec de la conversion de la photo");
                return;
            }
            personnelManager->setPhotoBase64(photoBase64);
        }

        if (personnelManager->editPersonnel()) {
            QMessageBox::information(this, "Succès", "Modifications enregistrées avec succès!");
            stackedWidget->setCurrentWidget(PersonelTablePage);
        } else {
            QMessageBox::critical(this, "Erreur",
                                  "Échec de la modification.\n"
                                  "Raisons possibles :\n"
                                  "- CIN déjà utilisé par un autre membre\n"
                                  "- Problème de connexion à la base\n"
                                  "- Données invalides");
        }
    });

    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(PersonelTablePage);
    });
}

void MainWindow::updateStats() {
    // Mettre à jour les graphiques
    villeChartView->setChart(personnelManager->createVillePieChart());
    genderChartView->setChart(personnelManager->createGenderBarChart());

    // Mettre à jour les labels
    int total = personnelManager->getTotalPersonnel();
    auto roleCounts = personnelManager->getRoleCounts();

    totalLabel->setText(QString("Total personnel: %1").arg(total));
    infirmiersLabel->setText(QString("Infirmiers: %1").arg(roleCounts.first));
    secretairesLabel->setText(QString("Secrétaires: %1").arg(roleCounts.second));

}


QString MainWindow::convertImageToBase64(const QString &imagePath)
{
    QFile imageFile(imagePath);
    if (!imageFile.open(QIODevice::ReadOnly))
        return QString();

    QByteArray imageData = imageFile.readAll();
    return QString::fromLatin1(imageData.toBase64());
}

void MainWindow::logout()
{
    if (QMessageBox::question(this, "Déconnexion",
                              "Êtes-vous sûr de vouloir vous déconnecter?",
                              QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
        emit logoutRequested();
    }
}
