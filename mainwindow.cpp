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
    // Add "Ajouter Materiels" Button
    QPushButton *addPatientButton = new QPushButton("Ajouter Patients", this);
    addPatientButton->setStyleSheet(
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
    QPushButton *editPatientButton = new QPushButton("Modifier Patients", this);
    editPatientButton->setStyleSheet(
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
    QPushButton *deletePatientButton = new QPushButton("Supprimer Patients", this);
    deletePatientButton->setStyleSheet(
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
    PatientsTablePage->setLayout(layout);

    QHBoxLayout *topButtonLayout = new QHBoxLayout();
    topButtonLayout->addWidget(backButton);
    topButtonLayout->addStretch();
    topButtonLayout->addWidget(addPatientButton);
    topButtonLayout->addWidget(editPatientButton);
    topButtonLayout->addWidget(deletePatientButton);


    layout->addLayout(topButtonLayout);

    // Set layout for researchertablePage

    layout->addStretch(1);





    QTableWidget *patientstable = new QTableWidget(this);
    patientstable->setColumnCount(8);
    patientstable->setHorizontalHeaderLabels({"IDP", "Nom", "Prenom", "Sexe", "CIN", "Adresse", "NumTel", "Maladie Chronique"});
    layout->addWidget(patientstable);

    // Now set the layout of the PatientsTablePage (no need for an extra QWidget)
    PatientsTablePage->setLayout(layout);

    // Set thepatientsManager->loadPatients(); PatientsTablePage as the central widget of the MainWindow

    // Initialize PatientsManager with the table
    patientsManager = new class PatientsManager(patientstable, this);










    // Connect Buttons to Slots
    connect(addPatientButton, &QPushButton::clicked, this, &MainWindow::onAddPatientClicked);
    connect(editPatientButton, &QPushButton::clicked, this, &MainWindow::onEditPatientClicked);
    connect(deletePatientButton, &QPushButton::clicked, this, &MainWindow::onDeletePatientClicked);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showPatientsPage);

}







void MainWindow::setupaddPatientsFormPage() {
    addPatientsFormPage = new QWidget();
    QVBoxLayout *formLayout = new QVBoxLayout(addPatientsFormPage);

    QLabel *titleLabel = new QLabel("Ajout d'un Patient");
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
    QLabel *maladiechroniqueLabel = new QLabel("Maladie Chronique:");
    QLineEdit *maladiechroniqueInput = new QLineEdit();

    QFormLayout *inputLayout = new QFormLayout();
    inputLayout->addRow(nomLabel, nomInput);
    inputLayout->addRow(prenomLabel, prenomInput);
    inputLayout->addRow(sexeLabel, sexeInput);
    inputLayout->addRow(cinLabel, cinInput);
    inputLayout->addRow(adresseLabel, adresseInput);
    inputLayout->addRow(numtelLabel, numtelInput);
    inputLayout->addRow(maladiechroniqueLabel, maladiechroniqueInput);
    formLayout->addLayout(inputLayout);

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
            qDebug() << "Error: patientsManager is not initialized!";
            return;
        }

        // Debugging - Print Input Values
        qDebug() << "Nom: " << nomInput->text();
        qDebug() << "Prenom: " << prenomInput->text();
        qDebug() << "Sexe: " << sexeInput->currentText();
        qDebug() << "CIN: " << cinInput->text();
        qDebug() << "Adresse: " << adresseInput->currentText();
        qDebug() << "NumTel: " << numtelInput->text();
        qDebug() << "Maladie Chronique: " << maladiechroniqueInput->text();

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
        patientsManager->setNom(nomInput->text());
        patientsManager->setprenom(prenomInput->text());
        patientsManager->setsexe(sexeInput->currentText());
        patientsManager->setcin(cinInput->text());
        patientsManager->setadresse(adresseInput->currentText());
        patientsManager->setnumtel(numtelInput->text());
        patientsManager->setmaladiechronique(maladiechroniqueInput->text());

        // Now call addPatient with the correct arguments
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
        setCentralWidget(patientsPage);
    });

    connect(backButton, &QPushButton::clicked, this, [this]() {
        setCentralWidget(patientsPage);
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








void MainWindow::onAddPatientClicked() {
    stackedWidget->setCurrentWidget(addPatientsFormPage);
}

void MainWindow::onEditPatientClicked() {
    bool ok;
    int patientID = QInputDialog::getInt(this, "Modifier Patient", "Entrez l'ID du patient à modifier:", 1, 1, 10000, 1, &ok);
    if (ok) {
        setupModifyPatientFormPage(patientID);
        setCentralWidget(ModifyPatientFormPage);
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
void MainWindow::showPatientsTablePage() {
    stackedWidget->setCurrentWidget(PatientsTablePage);
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
