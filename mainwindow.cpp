#include "mainwindow.h"
#include "toolsmanager.h"
#include "vaccinmanager.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlError>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
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
    addToolFormPage = new QWidget();

    // Vaccin Pages
    vaccinsTablePage = new QWidget();
    addVaccinFormPage = new QWidget();
    editVaccinFormPage = new QWidget();  // Add this line to create the missing widget

    // Setup Pages
    setupToolsPage();
    setupToolsTablePage();
    setupAddToolFormPage();
    setupVaccinsPage();
    setupVaccinsTablePage();
    setupAddVaccinFormPage();
    setupEditVaccinFormPage();
    // Remove the empty function call: ();

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
    stackedWidget->addWidget(vaccinsTablePage);
    stackedWidget->addWidget(addVaccinFormPage);
    stackedWidget->addWidget(editVaccinFormPage);  // Add this line to add the widget to the stack

    // Set Default Page
    stackedWidget->setCurrentWidget(patientsPage);
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

// -----------------------------
// Vaccin Pages Implementation
// -----------------------------

void MainWindow::setupVaccinsPage() {
    qDebug() << "Début de setupVaccinsPage";

    // Vérification et allocation de vaccinsPage
    if (!vaccinsPage) {
        vaccinsPage = new QWidget(this);
        qDebug() << "vaccinsPage initialisé";
    }

    QVBoxLayout *vaccinsLayout = new QVBoxLayout(vaccinsPage);
    vaccinsLayout->setSpacing(15);

    // Titre
    QLabel *titleLabel = new QLabel("Résumé des Vaccins", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    vaccinsLayout->addWidget(titleLabel);

    // Tableau des vaccins
    QTableWidget *summaryTable = new QTableWidget(this);
    summaryTable->setColumnCount(4);
    summaryTable->setHorizontalHeaderLabels({"Nom", "Type", "Doses", "Quantité"});
    summaryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    /*if (vaccinManager) {
        qDebug() << "Chargement des données avec vaccinManager";
        vaccinManager->loadVaccinSummary(summaryTable);
    } else {
        qDebug() << "Erreur: vaccinManager est NULL";
    }*/

    vaccinsLayout->addWidget(summaryTable);

    // Section statistiques
    QWidget *statsWidget = new QWidget(this);
    QHBoxLayout *statsLayout = new QHBoxLayout(statsWidget);

    // Widget Répartition par âge
    QWidget *ageGroupWidget = new QWidget(this);
    QVBoxLayout *ageGroupLayout = new QVBoxLayout(ageGroupWidget);
    QLabel *ageGroupTitle = new QLabel("Taux de Vaccination par Tranche d'Âge", this);
    ageGroupTitle->setStyleSheet("font-weight: bold;");
    ageGroupLayout->addWidget(ageGroupTitle);

    QStringList ageGroups = {"0-12 ans", "13-25 ans", "26-45 ans", "46-65 ans", "65+ ans"};
    QVector<double> vaccinationRates = {45.5, 68.2, 72.3, 65.7, 82.1};

    for (int i = 0; i < ageGroups.size(); ++i) {
        QHBoxLayout *groupRowLayout = new QHBoxLayout();
        QLabel *groupLabel = new QLabel(ageGroups[i]);

        QProgressBar *rateBar = new QProgressBar();
        rateBar->setMaximumWidth(200);
        rateBar->setRange(0, 100);
        rateBar->setValue(vaccinationRates[i]);
        rateBar->setFormat(QString::number(vaccinationRates[i], 'f', 1) + " %");

        groupRowLayout->addWidget(groupLabel);
        groupRowLayout->addWidget(rateBar);
        ageGroupLayout->addLayout(groupRowLayout);
    }

    // Widget Notifications
    QWidget *notificationsWidget = new QWidget(this);
    QVBoxLayout *notificationsLayout = new QVBoxLayout(notificationsWidget);
    QLabel *notifTitle = new QLabel("Notifications", this);
    notifTitle->setStyleSheet("font-weight: bold;");
    notificationsLayout->addWidget(notifTitle);

    QStringList notifications = {
        "Rappel: Vaccination contre la grippe",
        "Nouveau vaccin COVID disponible",
        "Stock de vaccin hépatite B limité"
    };

    for (const QString &notif : notifications) {
        QLabel *notifLabel = new QLabel(notif);
        notifLabel->setStyleSheet("background-color: #f0f0f0; border-radius: 5px; padding: 8px;");
        notificationsLayout->addWidget(notifLabel);
    }

    statsLayout->addWidget(ageGroupWidget);
    statsLayout->addWidget(notificationsWidget);
    vaccinsLayout->addWidget(statsWidget);

    // Bouton "Afficher Plus"
    QPushButton *moreButton = new QPushButton("Afficher Plus", this);
    moreButton->setStyleSheet("background-color: #198754; color: white; padding: 10px 20px; border-radius: 8px;");
    vaccinsLayout->addWidget(moreButton, 0, Qt::AlignCenter);

    // Vérification et connexion du bouton
    if (!connect(moreButton, &QPushButton::clicked, this, &MainWindow::showVaccinsTablePage)) {
        qDebug() << "Erreur: connexion du bouton échouée";
    }

    qDebug() << "Fin de setupVaccinsPage";
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
    summaryTable->setColumnCount(4);
    summaryTable->setRowCount(0);
    summaryTable->setHorizontalHeaderLabels({"Nom", "Type", "Doses", "Quantité"});
    summaryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    summaryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    summaryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

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

    // Connect the update button
    connect(updateButton, &QPushButton::clicked, this, [this, nomVaccinInput, referenceInput, typeInput,
                                                        maladieChronicInput, nbDoseInput, quantiteInput, dateExpInput, idInput]() {
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

        // Set values in VaccinManager
        vaccinManager->setId(id);
        vaccinManager->setNomVaccin(nomVaccin);
        vaccinManager->setReference(reference);
        vaccinManager->setType(type);
        vaccinManager->setMaladieChronique(maladieChronique);
        vaccinManager->setNbDose(nbDose);
        vaccinManager->setQuantite(quantite);
        vaccinManager->setDateExp(dateExp);

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

QProgressBar* MainWindow::createVaccinationRateBar(double rate) {
    QProgressBar *rateBar = new QProgressBar();
    rateBar->setMaximumWidth(200);
    rateBar->setRange(0, 100);
    rateBar->setValue(rate);
    rateBar->setTextVisible(true);
    rateBar->setFormat(QString::number(rate, 'f', 1) + " %");
    rateBar->setStyleSheet(
        "QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; }"
        "QProgressBar::chunk { background-color: #4CAF50; }"
        );

    return rateBar;
}

QWidget* MainWindow::createNotificationsWidget() {
    QWidget *notificationsWidget = new QWidget(this);
    QVBoxLayout *notificationsLayout = new QVBoxLayout(notificationsWidget);

    QLabel *notifTitle = new QLabel("Notifications", this);
    notifTitle->setStyleSheet("font-weight: bold;");
    notificationsLayout->addWidget(notifTitle);

    QStringList notifications = {
        "Rappel: Vaccination contre la grippe",
        "Nouveau vaccin COVID disponible",
        "Stock de vaccin hépatite B limité"
    };

    for (const QString &notif : notifications) {
        QLabel *notifLabel = new QLabel(notif);
        notifLabel->setStyleSheet(
            "background-color: #f0f0f0; "
            "border-radius: 5px; "
            "padding: 8px; "
            "margin-bottom: 5px;"
            );
        notificationsLayout->addWidget(notifLabel);
    }
    notificationsLayout->addStretch();

    return notificationsWidget;
}

void MainWindow::addMoreButton(QVBoxLayout *vaccinsLayout) {
    QPushButton *moreButton = new QPushButton("Afficher Plus", this);
    moreButton->setStyleSheet(
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
    vaccinsLayout->addWidget(moreButton, 0, Qt::AlignCenter);

    connect(moreButton, &QPushButton::clicked, this, &MainWindow::showVaccinsTablePage);
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

    // Boutons d'action
    QPushButton *addVaccinButton = new QPushButton("Ajouter Vaccin", this);
    QPushButton *editVaccinButton = new QPushButton("Modifier Vaccin", this);
    QPushButton *deleteVaccinButton = new QPushButton("Supprimer Vaccin", this);

    // Style des boutons
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

    // Disposition des boutons
    topButtonLayout->addWidget(backButton);
    topButtonLayout->addStretch();
    topButtonLayout->addWidget(addVaccinButton);
    topButtonLayout->addWidget(editVaccinButton);
    topButtonLayout->addWidget(deleteVaccinButton);

    // Création du tableau
    QTableWidget *vaccinsTable = new QTableWidget(this);

    // Configuration du nombre de colonnes AVANT de définir les en-têtes
    vaccinsTable->setColumnCount(7);

    // Configuration des en-têtes EXPLICITE
    QStringList headers = {"ID", "Nom Vaccin", "Référence", "Type", "Maladie Chronique", "Nombre de Doses", "Quantité"};
    vaccinsTable->setHorizontalHeaderLabels(headers);

    // Configuration de l'en-tête CRUCIALE
    QHeaderView *horizontalHeader = vaccinsTable->horizontalHeader();
    horizontalHeader->setVisible(true);
    horizontalHeader->setStyleSheet(
        "QHeaderView {"
        "    background-color: white;"
        "}"
        "QHeaderView::section {"
        "    background-color: white;"
        "    color: black;"
        "    padding: 5px;"
        "    border: none;"
        "    border-bottom: 1px solid #e0e0e0;"
        "    font-weight: bold;"
        "}"
        );

    // IMPORTANT : Forcer l'affichage des sections d'en-tête
    horizontalHeader->setDefaultSectionSize(100);
    horizontalHeader->setSectionResizeMode(QHeaderView::Interactive);

    // Configuration des largeurs de colonnes
    vaccinsTable->setColumnWidth(0, 50);   // ID
    vaccinsTable->setColumnWidth(1, 150);  // Nom Vaccin
    vaccinsTable->setColumnWidth(2, 120);  // Référence
    vaccinsTable->setColumnWidth(3, 100);  // Type
    vaccinsTable->setColumnWidth(4, 200);  // Maladie Chronique
    vaccinsTable->setColumnWidth(5, 120);  // Nombre de Doses
    vaccinsTable->setColumnWidth(6, 100);  // Quantité

    // Style du tableau
    vaccinsTable->setStyleSheet(
        "QTableWidget {"
        "    background-color: white;"
        "}"
        "QTableWidget::item {"
        "    padding: 5px;"
        "    border-bottom: 1px solid #e0e0e0;"
        "}"
        );

    // Paramètres de sélection
    vaccinsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    vaccinsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    vaccinsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Ajout des widgets au layout
    vaccinsTableLayout->addLayout(topButtonLayout);
    vaccinsTableLayout->addWidget(vaccinsTable);

    // Initialisation du VaccinManager
    vaccinManager = new VaccinManager(vaccinsTable, this);

    // Connexion des signaux et slots
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
    nbDoseInput->setMaximum(10);
    QSpinBox *quantiteInput = new QSpinBox();
    quantiteInput->setMinimum(0);
    quantiteInput->setMaximum(10000);
    QDateEdit *dateExpInput = new QDateEdit(QDate::currentDate().addYears(1));
    dateExpInput->setCalendarPopup(true);
    dateExpInput->setDisplayFormat("dd/MM/yyyy");
    dateExpInput->setMinimumDate(QDate::currentDate().addDays(1));
    dateExpInput->setMaximumDate(QDate::currentDate().addYears(10));

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
            // Si l'ajout est réussi, vider les champs
            nomVaccinInput->clear();
            referenceInput->clear();
            typeInput->clear();
            maladieChronicInput->clear();
            nbDoseInput->setValue(1);
            quantiteInput->setValue(0);
            dateExpInput->setDate(QDate::currentDate().addYears(1));

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

// Vaccin-related slots
void MainWindow::showVaccinsTablePage() {
    stackedWidget->setCurrentWidget(vaccinsTablePage);
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
