#include "mainwindow.h"
#include "toolsmanager.h"
#include "personnelmanager.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QDebug>
#include <QComboBox>
#include <QSqlDatabase>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), toolsManager(new ToolsManager(new QTableWidget(this), this)),
    personnelManager(new PersonnelManager(new QTableWidget(this), this)) {
    setupUI();
}

MainWindow::~MainWindow() {}

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

    QLabel *logoLabel = new QLabel(this);
    QPixmap logoPixmap(":/icons/svg/BIOLAB.svg");
    logoLabel->setPixmap(logoPixmap);
    logoLabel->setAlignment(Qt::AlignLeft);
    logoLabel->setStyleSheet("QLabel { margin-left: 20px; margin-top: 10px; }");
    sidebarLayout->addWidget(logoLabel);

    btnPatients = createSidebarButton("  Patients", ":/icons/svg/patient.svg");
    btnPersonel = createSidebarButton("  Personel", ":/icons/svg/personel.svg");
    btnResearches = createSidebarButton("  Recherche", ":/icons/svg/flask.svg");
    btnResearchers = createSidebarButton("  Chercheurs", ":/icons/svg/scientist.svg");
    btnTools = createSidebarButton("  Matériels", ":/icons/svg/microscope.svg");
    btnVaccins = createSidebarButton("  Vaccin", ":/icons/svg/syringe.svg");
    btnSettings = createSidebarButton("  Paramètres", ":/icons/svg/settings.svg");

    sidebarButtonGroup = new QButtonGroup(this);
    sidebarButtonGroup->addButton(btnPatients);
    sidebarButtonGroup->addButton(btnPersonel);
    sidebarButtonGroup->addButton(btnResearches);
    sidebarButtonGroup->addButton(btnResearchers);
    sidebarButtonGroup->addButton(btnTools);
    sidebarButtonGroup->addButton(btnVaccins);
    sidebarButtonGroup->addButton(btnSettings);
    sidebarButtonGroup->setExclusive(true);

    sidebarLayout->addWidget(btnPatients);
    sidebarLayout->addWidget(btnPersonel);
    sidebarLayout->addWidget(btnResearches);
    sidebarLayout->addWidget(btnResearchers);
    sidebarLayout->addWidget(btnTools);
    sidebarLayout->addWidget(btnVaccins);
    sidebarLayout->addWidget(btnSettings);

    QLabel *PicLabel = new QLabel(this);
    QPixmap PicPixmap(":/icons/svg/Asset 2.svg");
    PicLabel->setPixmap(PicPixmap.scaled(22, 22, Qt::KeepAspectRatio));
    PicLabel->setStyleSheet("QLabel { margin-left: 8px; margin-bottom: 50px; }");
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(PicLabel);

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

    setupPersonelPage();
    setupPersonelTablePage();
    setupAddPersonelFormPage();
    setupToolsPage();
   // setupToolsTablePage();
    //setupAddToolFormPage();

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

    stackedWidget->setCurrentWidget(patientsPage);
}

void MainWindow::setupPersonelPage() {
    QVBoxLayout *personelLayout = new QVBoxLayout(personelPage);

    QPushButton *goToPersonelTableButton = new QPushButton("Voir Personels", this);
    goToPersonelTableButton->setStyleSheet(
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
    personelLayout->addWidget(goToPersonelTableButton, 0, Qt::AlignCenter);

    connect(goToPersonelTableButton, &QPushButton::clicked, this, &MainWindow::showPersonelTablePage);
}

void MainWindow::setupPersonelTablePage() {
    QVBoxLayout *layout = new QVBoxLayout(PersonelTablePage);

    QPushButton *addPersonelButton = new QPushButton("Ajouter Personel", this);
    addPersonelButton->setStyleSheet(
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

    QPushButton *editPersonelButton = new QPushButton("Modifier Personel", this);
    editPersonelButton->setStyleSheet(
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

    QPushButton *deletePersonelButton = new QPushButton("Supprimer Personel", this);
    deletePersonelButton->setStyleSheet(
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

    QHBoxLayout *topButtonLayout = new QHBoxLayout();
    topButtonLayout->addWidget(backButton);
    topButtonLayout->addStretch();
    topButtonLayout->addWidget(addPersonelButton);
    topButtonLayout->addWidget(editPersonelButton);
    topButtonLayout->addWidget(deletePersonelButton);


    layout->addLayout(topButtonLayout);

    QTableWidget *personelTable = new QTableWidget(this);


    personelTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    background-color: #F5F5F7;"
        "    color: #555;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "    padding: 8px;"
        "    border: none;"
        "    border-bottom: 1px solid #ddd;"
        "}"
        );
    personelTable->setStyleSheet(
        "QTableWidget {"
        "    background-color: #FFFFFF;"
        "    border: 1px solid #ddd;"
        "    border-radius: 8px;"
        "    color:     Black"
        "}"
        "QTableWidget::item {"
        "    border-bottom: 1px solid #ddd;"
        "    padding: 8px;"
        "    color:     Black"
        "}"
        "QTableWidget::item:selected {"
        "    background: #EDEDED;"

        "}"
        );

    personelTable->setColumnCount(12);
    personelTable->setHorizontalHeaderLabels({"IDPER", "Nom", "Prénom", "Sexe", "CIN", "Numéro de Téléphone", "Email", "Adresse", "Date d'Ajout", "Rôle", "Email G", "Mot de Passe G"});
    layout->addWidget(personelTable);
    personnelManager = new PersonnelManager(personelTable , this);







    connect(addPersonelButton, &QPushButton::clicked, this, &MainWindow::onAddPersonelClicked);
    connect(editPersonelButton, &QPushButton::clicked, this, &MainWindow::onEditPersonelClicked);
    connect(deletePersonelButton, &QPushButton::clicked, this, &MainWindow::onDeletePersonelClicked);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showPersonelPage);
}

void MainWindow::setupAddPersonelFormPage() {
    QVBoxLayout *formLayout = new QVBoxLayout(addPersonelFormPage);

    QLineEdit *nomInput = new QLineEdit();
    QLineEdit *prenomInput = new QLineEdit();
    QLineEdit *cinInput = new QLineEdit();
    QLineEdit *numTelInput = new QLineEdit();
    QLineEdit *emailInput = new QLineEdit();
    QLineEdit *adresseInput = new QLineEdit();
    QComboBox *roleInput = new QComboBox();
    roleInput->addItems({"vaccinateur", "secretaire"});
    QLineEdit *emailGInput = new QLineEdit();
    QLineEdit *motDePasseGInput = new QLineEdit();

    QFormLayout *inputLayout = new QFormLayout();
    inputLayout->addRow("Nom:", nomInput);
    inputLayout->addRow("Prénom:", prenomInput);
    inputLayout->addRow("CIN:", cinInput);
    inputLayout->addRow("Numéro de Téléphone:", numTelInput);
    inputLayout->addRow("Email:", emailInput);
    inputLayout->addRow("Adresse:", adresseInput);
    inputLayout->addRow("Rôle:", roleInput);
    inputLayout->addRow("Email G:", emailGInput);
    inputLayout->addRow("Mot de Passe G:", motDePasseGInput);

    QPushButton *submitButton = new QPushButton("Ajouter");
    QPushButton *backButton = new QPushButton("Retour");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(submitButton);

    formLayout->addLayout(inputLayout);
    formLayout->addLayout(buttonLayout);

    addPersonelFormPage->setStyleSheet(
        "QLineEdit, QComboBox {"
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

    connect(submitButton, &QPushButton::clicked, this, [this, nomInput, prenomInput, cinInput, numTelInput, emailInput, adresseInput, roleInput, emailGInput, motDePasseGInput]() {
        QString nom = nomInput->text();
        QString prenom = prenomInput->text();
        QString cin = cinInput->text();
        QString numTel = numTelInput->text();
        QString email = emailInput->text();
        QString adresse = adresseInput->text();
        QString role = roleInput->currentText();
        QString emailG = emailGInput->text();
        QString motDePasseG = motDePasseGInput->text();

        personnelManager->setNom(nom);
        personnelManager->setPrenom(prenom);
        personnelManager->setCin(cin);
        personnelManager->setNumTel(numTel);
        personnelManager->setEmail(email);
        personnelManager->setAdresse(adresse);
        personnelManager->setRole(role);
        personnelManager->setEmailG(emailG);
        personnelManager->setMotDePasseG(motDePasseG);

        personnelManager->addPersonnel();
        personnelManager->refreshTable(); // Rafraîchir le tableau après l'ajout

        stackedWidget->setCurrentWidget(PersonelTablePage);
    });

    connect(backButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(PersonelTablePage);
    });
}

void MainWindow::setupToolsPage() {
    QVBoxLayout *toolsLayout = new QVBoxLayout(toolsPage);

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

    connect(goToToolsTableButton, &QPushButton::clicked, this, &MainWindow::showToolsTablePage);
}



void MainWindow::onAddToolClicked() {
    stackedWidget->setCurrentWidget(addToolFormPage);
}

void MainWindow::onAddPersonelClicked() {
    stackedWidget->setCurrentWidget(addPersonelFormPage);
}


void MainWindow::onEditToolClicked() {
    bool ok;
    int id = QInputDialog::getInt(this, "Modifier Matériel", "Entrez l'ID du matériel à modifier:", 1, 1, 100, 1, &ok);
    if (ok) {
        toolsManager->setId(id);
        toolsManager->setNomMateriel("Nom Modifié");
        toolsManager->setDescription("Description Modifiée");
        toolsManager->setQuantiteMaximale(10); // Exemple de quantité modifiée
        toolsManager->editTool();
    }
}



void MainWindow::onEditPersonelClicked() {
    bool ok;
    int id = QInputDialog::getInt(this, "Modifier Matériel", "Entrez l'ID du matériel à modifier:", 1, 1, 100, 1, &ok);
    if (ok) {
        toolsManager->setId(id);
        toolsManager->setNomMateriel("Nom Modifié");
        toolsManager->setDescription("Description Modifiée");
        toolsManager->setQuantiteMaximale(10); // Exemple de quantité modifiée
        toolsManager->editTool();
    }
}




void MainWindow::onDeleteToolClicked() {
    bool ok;
    int id = QInputDialog::getInt(this, "Supprimer Matériel", "Entrez l'ID du matériel à supprimer:", 1, 1, 100, 1, &ok);
    if (ok) {
        toolsManager->setId(id);
        toolsManager->deleteTool();
    }
}


void MainWindow::onDeletePersonelClicked() {
    bool ok;
    int id = QInputDialog::getInt(this, "Supprimer Personnel", "Entrez l'ID du personnel à supprimer :", 1, 1, 100, 1, &ok);
    if (ok) {
        personnelManager->setIdPer(id); // Définir l'ID du personnel à supprimer
        if (personnelManager->deletePersonnel()) {
            qDebug() << "Personnel supprimé avec succès !";
        } else {
            qDebug() << "Échec de la suppression du personnel.";
        }
    }
}
void MainWindow::showPatientsPage() {
    stackedWidget->setCurrentWidget(patientsPage);
    updateSidebarIcons(btnPatients);
}

void MainWindow::showPersonelPage() {
    stackedWidget->setCurrentWidget(personelPage);
    if (personnelManager) {
        personnelManager->refreshTable();
    } else {
        qDebug() << "Erreur: PersonnelManager n'est pas initialisé.";
    updateSidebarIcons(btnPersonel);
}
}

void MainWindow::showResearchPage() {
    stackedWidget->setCurrentWidget(researchesPage);
    updateSidebarIcons(btnResearches);
}

void MainWindow::showResearchersPage () {
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

void MainWindow::showPersonelTablePage() {
    if (personnelManager) {
        personnelManager->refreshTable();
    } else {
        qDebug() << "Erreur: PersonnelManager n'est pas initialisé.";
        updateSidebarIcons(btnPersonel);
    }
    stackedWidget->setCurrentWidget(PersonelTablePage);
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
