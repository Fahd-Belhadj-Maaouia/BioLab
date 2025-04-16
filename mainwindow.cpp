#include "mainwindow.h"
#include "toolsmanager.h"
#include "vaccinmanager.h"
#include"messagesmanager.h"
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
    connect(btnResearchers, &QPushButton::clicked, this, &MainWindow::showResearchersPage);
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
    stackedWidget = new QStackedWidget(this);

    // Create Pages
    patientsPage = new QWidget();
    researchersPage = new QWidget();
    settingsPage = new QWidget();
    vaccinsPage = new QWidget();
    personelPage = new QWidget();
    toolsPage = new QWidget();
    researchesPage = new QWidget();
    messagerieePage = new QWidget();  // Added messaging page

    // Tools Pages
    toolsTablePage = new QWidget();
    addToolFormPage = new QWidget();

    // Vaccin Pages
    vaccinsTablePage = new QWidget();
    addVaccinFormPage = new QWidget();
    editVaccinFormPage = new QWidget();

    // Setup Pages
    setupToolsPage();
    setupToolsTablePage();
    setupAddToolFormPage();
    setupVaccinsPage();
    setupVaccinsTablePage();
    setupAddVaccinFormPage();
    setupEditVaccinFormPage();
    setupMessagerieePage();  // Added setup for messaging page

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
    stackedWidget->addWidget(editVaccinFormPage);
    stackedWidget->addWidget(messagerieePage);  // Added messaging page to stack

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
    vaccinsPage->setLayout(vaccinsLayout);
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

    // Chargement des données avec vaccinManager
    /*if (vaccinManager) {
        qDebug() << "Chargement des données avec vaccinManager";
        vaccinManager->loadVaccinSummary(summaryTable);
    } else {
        qDebug() << "Erreur: vaccinManager est NULL";
    }*/

    vaccinsLayout->addWidget(summaryTable);

    // Section statistiques et notifications côte à côte
    QWidget *statsWidget = new QWidget(this);
    QHBoxLayout *statsLayout = new QHBoxLayout(statsWidget);
    statsLayout->setSpacing(30); // Espace plus large entre les statistiques et les notifications
    statsLayout->setContentsMargins(0, 10, 0, 10);

    // Widget statistiques par type de vaccin (côté gauche)
    QWidget *vaccTypeWidget = new QWidget(this);
    QVBoxLayout *vaccTypeLayout = new QVBoxLayout(vaccTypeWidget);
    vaccTypeLayout->setContentsMargins(0, 0, 0, 0);

    // Titre des statistiques par type
    QLabel *typeStatsTitle = new QLabel("Distribution des Vaccins par Type", this);
    QFont titleFont = typeStatsTitle->font();
    titleFont.setBold(true);
    typeStatsTitle->setFont(titleFont);
    typeStatsTitle->setAlignment(Qt::AlignLeft);
    vaccTypeLayout->addWidget(typeStatsTitle);

    // Créer un widget pour contenir les statistiques
    QWidget *statsContainer = new QWidget();
    vaccTypeLayout->addWidget(statsContainer);

    // Appeler la fonction pour afficher les statistiques
    try {
        // Essayer d'afficher les statistiques de manière sécurisée
        QTimer::singleShot(100, this, [this, statsContainer]() {
            displayVaccinTypeStats(statsContainer);
        });
    } catch (std::exception &e) {
        qDebug() << "Erreur lors de l'affichage des statistiques:" << e.what();
        QLabel *errorLabel = new QLabel("Erreur: Impossible de charger les statistiques");
        vaccTypeLayout->addWidget(errorLabel);
    }

    // Ajouter le widget des statistiques au layout horizontal avec un ratio de 2
    statsLayout->addWidget(vaccTypeWidget, 2);

    // Ajouter un spacer pour pousser les notifications plus à droite
    statsLayout->addStretch(1);

    // Widget Notifications (côté droit)
    QWidget *notificationsContainerWidget = new QWidget(this);
    QVBoxLayout *notificationsContainerLayout = new QVBoxLayout(notificationsContainerWidget);
    notificationsContainerLayout->setContentsMargins(0, 0, 0, 0);

    // Titre des notifications
    QLabel *notificationsTitle = new QLabel("Notifications", this);
    notificationsTitle->setFont(titleFont);
    notificationsTitle->setAlignment(Qt::AlignLeft);
    notificationsContainerLayout->addWidget(notificationsTitle);

    // Créer un widget animé pour contenir les notifications
    QWidget *notificationsAnimatedContainer = new QWidget(this);
    QVBoxLayout *notificationsAnimatedLayout = new QVBoxLayout(notificationsAnimatedContainer);
    notificationsAnimatedLayout->setContentsMargins(0, 0, 0, 0);

    // Appliquer le style animé au conteneur
    applyAnimatedContainerStyle(notificationsAnimatedContainer);

    // Ajouter le widget animé au conteneur des notifications
    notificationsContainerLayout->addWidget(notificationsAnimatedContainer);

    // Définir un widget pour contenir les notifications qui seront chargées plus tard
    QWidget *notificationsPlaceholder = new QWidget(this);
    QVBoxLayout *notificationsPlaceholderLayout = new QVBoxLayout(notificationsPlaceholder);
    notificationsPlaceholderLayout->setContentsMargins(10, 10, 10, 10);
    notificationsPlaceholder->setObjectName("notificationsPlaceholder"); // Pour le retrouver facilement
    notificationsPlaceholder->setMinimumWidth(250); // Largeur minimale du widget de notifications
    notificationsPlaceholder->setMaximumWidth(300); // Largeur maximale
    notificationsAnimatedLayout->addWidget(notificationsPlaceholder);

    // Ajouter le conteneur de notifications au layout horizontal avec un ratio de 1
    statsLayout->addWidget(notificationsContainerWidget, 1);

    // Ajouter le widget contenant les stats et notifications au layout principal
    vaccinsLayout->addWidget(statsWidget);

    // Chargement des notifications avec un délai
    if (vaccinManager) {
        QTimer::singleShot(500, this, &MainWindow::loadNotifications);
    } else {
        qDebug() << "🚨 vaccinManager n'est pas initialisé, pas de notifications affichées.";
        QLabel *errorLabel = new QLabel("Impossible de charger les notifications", this);
        notificationsPlaceholderLayout->addWidget(errorLabel);
    }

    // Bouton "Afficher Plus"
    QPushButton *moreButton = new QPushButton("Afficher Plus", this);
    moreButton->setStyleSheet("background-color: #198754; color: white; padding: 10px 20px; border-radius: 8px;");
    vaccinsLayout->addWidget(moreButton, 0, Qt::AlignCenter);

    // Vérification et connexion du bouton
    if (!connect(moreButton, &QPushButton::clicked, this, &MainWindow::showVaccinsTablePage)) {
        qDebug() << "Erreur: connexion du bouton échouée";
    }

    // Bouton "Exporter en PDF"
    QPushButton *exportPdfButton = new QPushButton("Exporter en PDF", this);
    exportPdfButton->setStyleSheet("background-color: #0d6efd; color: white; padding: 10px 20px; border-radius: 8px;");
    vaccinsLayout->addWidget(exportPdfButton, 0, Qt::AlignCenter);

    // Connexion du bouton d'export
    connect(exportPdfButton, &QPushButton::clicked, this, &MainWindow::showVaccinTypeStats);
    qDebug() << "Fin de setupVaccinsPage";
}
void MainWindow::styleNotificationsWidget(QWidget *notificationsWidget) {
    // Style global pour le widget des notifications
    notificationsWidget->setStyleSheet(
        "QWidget { background-color: transparent; }"
        );

    // Parcourir tous les widgets enfants pour appliquer des styles spécifiques
    QList<QWidget*> children = notificationsWidget->findChildren<QWidget*>();
    for (QWidget *child : children) {
        // Style pour les frames contenant les notifications - Fond bleu au lieu de blanc
        if (QFrame *frame = qobject_cast<QFrame*>(child)) {
            if (frame->property("notificationType") == "warning") {
                frame->setStyleSheet(
                    "QFrame {"
                    "  background-color: #f0f8ff;"  // Bleu très clair
                    "  border: 1px solid #ffc107;"
                    "  border-radius: 6px;"
                    "  margin-bottom: 12px;"
                    "  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);"  // Ombre plus prononcée
                    "}"
                    );
            } else if (frame->property("notificationType") == "expiry") {
                frame->setStyleSheet(
                    "QFrame {"
                    "  background-color: #e6f2ff;"  // Bleu très clair
                    "  border: 1px solid #dc3545;"
                    "  border-radius: 6px;"
                    "  margin-bottom: 12px;"
                    "  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);"  // Ombre plus prononcée
                    "}"
                    );
            } else if (frame->property("notificationType") == "info") {
                frame->setStyleSheet(
                    "QFrame {"
                    "  background-color: #e1f5fe;"  // Bleu clair
                    "  border: 1px solid #17a2b8;"
                    "  border-radius: 6px;"
                    "  margin-bottom: 12px;"
                    "  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);"  // Ombre plus prononcée
                    "}"
                    );
            } else {
                frame->setStyleSheet(
                    "QFrame {"
                    "  background-color: #e3f2fd;"  // Bleu clair
                    "  border: 1px solid #6c757d;"
                    "  border-radius: 6px;"
                    "  margin-bottom: 12px;"
                    "  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);"  // Ombre plus prononcée
                    "}"
                    );
            }

            // Ajouter un effet d'ombre plus prononcé avec QGraphicsDropShadowEffect
            QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(frame);
            shadowEffect->setBlurRadius(10);
            shadowEffect->setColor(QColor(0, 0, 0, 80));
            shadowEffect->setOffset(0, 3);
            frame->setGraphicsEffect(shadowEffect);
        }

        // Style pour les étiquettes dans les notifications - Style d'écriture modifié
        if (QLabel *label = qobject_cast<QLabel*>(child)) {
            if (label->property("notificationType") == "warning") {
                label->setStyleSheet(
                    "QLabel {"
                    "  color: #ffc107;"
                    "  font-weight: bold;"
                    "  font-size: 14px;"  // Taille de police augmentée
                    "  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;"  // Police moderne
                    "  padding: 10px;"
                    "  border-bottom: 1px solid #ffc107;"
                    "  background-color: #fff8e5;"
                    "  border-top-left-radius: 5px;"
                    "  border-top-right-radius: 5px;"
                    "}"
                    );
            } else if (label->property("notificationType") == "expiry") {
                label->setStyleSheet(
                    "QLabel {"
                    "  color: #dc3545;"
                    "  font-weight: bold;"
                    "  font-size: 14px;"  // Taille de police augmentée
                    "  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;"  // Police moderne
                    "  padding: 10px;"
                    "  border-bottom: 1px solid #dc3545;"
                    "  background-color: #ffebee;"
                    "  border-top-left-radius: 5px;"
                    "  border-top-right-radius: 5px;"
                    "}"
                    );
            } else if (label->property("notificationType") == "info") {
                label->setStyleSheet(
                    "QLabel {"
                    "  color: #17a2b8;"
                    "  font-weight: bold;"
                    "  font-size: 14px;"  // Taille de police augmentée
                    "  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;"  // Police moderne
                    "  padding: 10px;"
                    "  border-bottom: 1px solid #17a2b8;"
                    "  background-color: #e6f7fa;"
                    "  border-top-left-radius: 5px;"
                    "  border-top-right-radius: 5px;"
                    "}"
                    );
            } else {
                label->setStyleSheet(
                    "QLabel {"
                    "  color: #333333;"
                    "  font-size: 13px;"  // Taille de police augmentée
                    "  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;"  // Police moderne
                    "  padding: 10px;"
                    "}"
                    );
            }
        }

        // Style pour tout autre texte descriptif - Style d'écriture modifié
        if (QLabel *textLabel = qobject_cast<QLabel*>(child)) {
            if (!textLabel->property("notificationType").isValid()) { // S'assurer que c'est une étiquette de description
                textLabel->setStyleSheet(
                    "QLabel {"
                    "  border: none;"
                    "  background-color: transparent;"
                    "  padding: 5px 10px;"
                    "  font-size: 13px;"  // Taille de police augmentée
                    "  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;"  // Police moderne
                    "  color: #0d47a1;"  // Couleur de texte bleu foncé
                    "  line-height: 1.4;"  // Espace entre les lignes
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
    // S'assurer qu'il y a un layout pour parentWidget
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(parentWidget->layout());
    if (!layout) {
        layout = new QVBoxLayout(parentWidget);
    } else {
        // Nettoyer le layout existant
        QLayoutItem *child;
        while ((child = layout->takeAt(0)) != nullptr) {
            if (child->widget()) {
                child->widget()->deleteLater();
            }
            delete child;
        }
    }

    // Get statistics from VaccinManager
    QMap<QString, int> typeStats = vaccinManager->getVaccinTypeStats();

    // Calculate total for percentage
    int totalVaccins = 0;
    for (auto it = typeStats.begin(); it != typeStats.end(); ++it) {
        totalVaccins += it.value();
    }

    if (totalVaccins == 0) {
        layout->addWidget(new QLabel("Aucune donnée disponible."));
    } else {
        // Créer le graphique circulaire
        chartView = createPieChart(typeStats, totalVaccins);
        layout->addWidget(chartView);

        // Ajouter un libellé pour le total
        QLabel *totalLabel = new QLabel("<b>Total: " + QString::number(totalVaccins) + " unités</b>");
        totalLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(totalLabel);

        // Configuration du timer pour mise à jour automatique (toutes les 5 secondes)
        if (!updateTimer) {
            updateTimer = new QTimer(this);
            connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateVaccinTypeStats);
            updateTimer->start(5000); // 5000 ms = 5 secondes
        }
    }
}

QChartView* MainWindow::createPieChart(const QMap<QString, int> &typeStats, int totalVaccins) {
    // Créer la série pour le graphique circulaire
    QPieSeries *series = new QPieSeries();

    // Définir un tableau de couleurs bleu graduées
    QStringList blueGradients = {
        "#0d47a1", // Bleu très foncé
        "#1976d2", // Bleu foncé
        "#2196f3", // Bleu moyen
        "#42a5f5", // Bleu clair
        "#64b5f6", // Bleu très clair
        "#90caf9", // Bleu pâle
        "#bbdefb"  // Bleu très pâle
    };

    int colorIndex = 0;

    // Ajouter les données au graphique
    for (auto it = typeStats.begin(); it != typeStats.end(); ++it) {
        QString type = it.key();
        int quantity = it.value();
        double percentage = (totalVaccins > 0) ? (quantity * 100.0 / totalVaccins) : 0;

        // Ajouter une tranche au graphique
        QPieSlice *slice = series->append(type + " - " + QString::number(quantity) + " (" +
                                              QString::number(percentage, 'f', 1) + "%)", quantity);

        // Appliquer la couleur à la tranche
        slice->setBrush(QColor(blueGradients[colorIndex % blueGradients.size()]));
        slice->setPen(QPen(Qt::white, 1));

        // Augmenter l'index de couleur
        colorIndex++;

        // Ajouter une animation lors du survol
        slice->setLabelVisible(true);
        slice->setExploded(false);
        slice->setExplodeDistanceFactor(0.1);

        // Connecter le signal de survol pour l'animation
        connect(slice, &QPieSlice::hovered, [slice](bool hovered) {
            slice->setExploded(hovered);
            slice->setLabelVisible(true);
        });
    }

    // Créer le graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Distribution des Vaccins par Type");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setTheme(QChart::ChartThemeLight);

    // Créer la vue du graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(300, 300);

    return chartView;
}

void MainWindow::updateVaccinTypeStats() {
    // Vérifier si le graphique est visible
    if (vaccinsPage && vaccinsPage->isVisible() && chartView) {
        QWidget *statsContainer = vaccinsPage->findChild<QWidget*>("statsContainer");
        if (statsContainer) {
            qDebug() << "Mise à jour automatique des statistiques de vaccins...";
            displayVaccinTypeStats(statsContainer);
        }
    }
}

// Modifier aussi la fonction showVaccinTypeStats pour utiliser le graphique circulaire
void MainWindow::showVaccinTypeStats() {
    // Create dialog for statistics
    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques par Type de Vaccin");
    statsDialog->setMinimumSize(600, 500);

    QVBoxLayout *mainLayout = new QVBoxLayout(statsDialog);

    // Add title
    QLabel *titleLabel = new QLabel("Distribution des Vaccins par Type");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Get statistics from VaccinManager
    QMap<QString, int> typeStats = vaccinManager->getVaccinTypeStats();

    // Calculate total for percentage
    int totalVaccins = 0;
    for (auto it = typeStats.begin(); it != typeStats.end(); ++it) {
        totalVaccins += it.value();
    }

    if (totalVaccins == 0) {
        mainLayout->addWidget(new QLabel("Aucune donnée disponible."));
    } else {
        // Créer et ajouter le graphique circulaire
        QChartView *dialogChartView = createPieChart(typeStats, totalVaccins);
        mainLayout->addWidget(dialogChartView);
    }

    // Add export button
    QPushButton *exportButton = new QPushButton("Exporter les Statistiques");
    // Nouvelle connexion qui passe les données à la fonction d'exportation PDF
    connect(exportButton, &QPushButton::clicked, [this, typeStats, totalVaccins]() {
        exportVaccinStatsToPDF(typeStats, totalVaccins);
    });

    // Add close button
    QPushButton *closeButton = new QPushButton("Fermer");
    connect(closeButton, &QPushButton::clicked, [statsDialog]() {
        statsDialog->close();
    });

    // Add buttons to layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(exportButton);
    buttonLayout->addWidget(closeButton);
    mainLayout->addLayout(buttonLayout);

    // Show dialog
    statsDialog->setLayout(mainLayout);
    statsDialog->exec();
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
    vaccinsTable->setColumnCount(8);

    QStringList headers = {"ID", "Nom Vaccin", "Référence", "Type", "Maladie Chronique",
                           "Nombre de Doses", "Quantité", "Date d'Expiration"};
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
    });

    connect(resetButton, &QPushButton::clicked, this, [this, searchInput, sortTypeCombo, sortOrderCombo]() {
        searchInput->clear();
        VaccinManager::SortType sortType = static_cast<VaccinManager::SortType>(sortTypeCombo->currentData().toInt());
        bool ascending = sortOrderCombo->currentData().toBool();

        // Recharger tous les vaccins avec les paramètres de tri actuels
        vaccinManager->loadVaccins(sortType, ascending);
    });

    // Permettre la recherche en appuyant sur Entrée dans le champ de recherche
    connect(searchInput, &QLineEdit::returnPressed, searchButton, &QPushButton::click);

    // Tri direct lorsque les options de tri changent
    connect(sortTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, sortTypeCombo, sortOrderCombo]() {
        VaccinManager::SortType sortType = static_cast<VaccinManager::SortType>(sortTypeCombo->currentData().toInt());
        bool ascending = sortOrderCombo->currentData().toBool();
        vaccinManager->loadVaccins(sortType, ascending);
    });

    connect(sortOrderCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, sortTypeCombo, sortOrderCombo]() {
        VaccinManager::SortType sortType = static_cast<VaccinManager::SortType>(sortTypeCombo->currentData().toInt());
        bool ascending = sortOrderCombo->currentData().toBool();
        vaccinManager->loadVaccins(sortType, ascending);
    });
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
    newMessageBtn->setIcon(QIcon(":/icons/svg/message.svg"));

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
