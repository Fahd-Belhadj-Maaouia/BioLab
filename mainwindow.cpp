#include "mainwindow.h"
#include <QHeaderView>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include<QMessageBox>
#include<QTextEdit>
#include "projetderecherche.h"
#include <QSqlError>





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    stackedWidget = new QStackedWidget(this);
    setupUI();

}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *contentLayout = new QHBoxLayout();

    setupSidebar();
    setupPages();

    contentWrapper = new QWidget(this);
    contentWrapper->setStyleSheet(
        "background-color: #f8f8ff;"
        "border-radius: 20px;"
        "margin-top: 0px;"        // Moves it over the sidebar
        "padding: 20px;"
        );

    QVBoxLayout *contentWrapperLayout = new QVBoxLayout(contentWrapper);
    contentWrapperLayout->setContentsMargins(0, 0, 0, 0);
    contentWrapperLayout->addWidget(stackedWidget); // Add pages inside

    contentLayout->addWidget(sidebarWidget);
    contentLayout->addWidget(contentWrapper, 1); // Use contentWrapper instead of stackedWidget

    mainLayout->addLayout(contentLayout);
}


void MainWindow::setupSidebar()
{
    sidebarWidget = new QWidget(this);
    sidebarWidget->setFixedWidth(180);
    sidebarWidget->setStyleSheet(
        "background-color: #1a1a1a; border-radius: 15px;"
        );

    sidebarLayout = new QVBoxLayout(sidebarWidget);

    QLabel *logoLabel = new QLabel(this);
    QPixmap logoPixmap(":/icons/svg/BIOLAB.svg");  // Replace with the correct path to your logo image
    logoLabel->setPixmap(logoPixmap);
    logoLabel->setAlignment(Qt::AlignLeft);  // Center the logo in the sidebar

    logoLabel->setStyleSheet("QLabel {"
                             "margin-left: 20px;"  // Moves the logo 20px from the left
                             "margin-top: 10px;"   // Moves the logo 10px from the top (optional)
                             "}");

    sidebarLayout->addWidget(logoLabel);

    btnPatients = new QPushButton("  Patients");
    btnPatients->setIcon(QIcon(":/icons/svg/patient.svg"));
    btnPatients->setIconSize(QSize(28,28));

    btnPersonel = new QPushButton("  Personel");
    btnPersonel->setIcon(QIcon(":/icons/svg/personel.svg"));
    btnPersonel->setIconSize(QSize(28,28));

    btnResearches = new QPushButton("  Recherche");
    btnResearches->setIcon(QIcon(":/icons/svg/flask.svg"));
    btnResearches->setIconSize(QSize(28,28));

    btnResearchers = new QPushButton("  Chercheurs");
    btnResearchers->setIcon(QIcon(":/icons/svg/scientist.svg"));
    btnResearchers->setIconSize(QSize(28,28));

    btnTools = new QPushButton("  Matériels");
    btnTools->setIcon(QIcon(":/icons/svg/microscope.svg"));
    btnTools->setIconSize(QSize(28,28));

    btnVaccins = new QPushButton("  Vaccin");
    btnVaccins->setIcon(QIcon(":/icons/svg/syringe.svg"));
    btnVaccins->setIconSize(QSize(28,28));

    btnSettings = new QPushButton("  Paramètres");
    btnSettings->setIcon(QIcon(":/icons/svg/settings.svg"));
    btnSettings->setIconSize(QSize(28,28));

    QLabel *PicLabel = new QLabel(this);
    QPixmap PicPixmap(":/icons/svg/Asset 2.svg");  // Replace with the correct path to your logo image
    PicLabel->setPixmap(PicPixmap.scaled(22, 22, Qt::KeepAspectRatio));  // Adjust the size as needed
    PicLabel->setAlignment(Qt::AlignLeft);  // Center the logo in the sidebar

    // Optionally, adjust the margins
    PicLabel->setStyleSheet("QLabel {"
                            "margin-left: 8px;"  // Moves the logo 20px from the left
                            "margin-bottom: 50px;" // Add some space at the bottom (optional)
                            "}");



    QString buttonStyle =
        "QPushButton { background: transparent; color: white; text-align: left; padding: 15px; font-size: 16px; border-radius: 8px; }"
        "QPushButton:hover { background: rgba(255, 255, 255, 0.1); }"
        "QPushButton:checked { background: #198754; color: white; font-weight: bold; }"
        "QPushButton::icon { margin-right: 10px; padding: 5px; icon-size: 20px; }"
        ;

    btnPatients->setStyleSheet(buttonStyle);
    btnResearchers->setStyleSheet(buttonStyle);
    btnSettings->setStyleSheet(buttonStyle);
    btnVaccins->setStyleSheet(buttonStyle);
    btnResearches->setStyleSheet(buttonStyle);
    btnTools->setStyleSheet(buttonStyle);
    btnPersonel->setStyleSheet(buttonStyle);

    // Ensure only one button is selected at a time
    sidebarButtonGroup = new QButtonGroup(this);
    sidebarButtonGroup->addButton(btnPatients);
    sidebarButtonGroup->addButton(btnResearchers);
    sidebarButtonGroup->addButton(btnSettings);
    sidebarButtonGroup->addButton(btnVaccins);
    sidebarButtonGroup->addButton(btnPersonel);
    sidebarButtonGroup->addButton(btnResearches);
    sidebarButtonGroup->addButton(btnTools);
    sidebarButtonGroup->setExclusive(true);  // ✅ Only one selection at a time

    sidebarLayout->addWidget(btnPatients);
    sidebarLayout->addWidget(btnPersonel);
    sidebarLayout->addWidget(btnResearches);
    sidebarLayout->addWidget(btnResearchers);
    sidebarLayout->addWidget(btnTools);
    sidebarLayout->addWidget(btnVaccins);
    sidebarLayout->addWidget(btnSettings);

    sidebarLayout->addStretch();

    connect(btnPatients, &QPushButton::clicked, this, &MainWindow::showPatientsPage);
    connect(btnResearchers, &QPushButton::clicked, this, &MainWindow::showResearchersPage);
    connect(btnVaccins, &QPushButton::clicked, this, &MainWindow::showVaccinsPage);
    connect(btnPersonel, &QPushButton::clicked, this, &MainWindow::showPersonelPage);
    connect(btnTools, &QPushButton::clicked, this, &MainWindow::showToolsPage);
    connect(btnResearches, &QPushButton::clicked, this, &MainWindow::showResearchPage);
    connect(btnSettings, &QPushButton::clicked, this, &MainWindow::showSettingsPage);
    // Add the image to the bottom of the sidebar (after the buttons)
    sidebarLayout->addStretch();  // This makes sure the image stays at the bottom
    sidebarLayout->addWidget(PicLabel);

}


void MainWindow::setupPages()
{
    stackedWidget = new QStackedWidget(this);

    patientsPage = new QWidget();
    researchersPage = new QWidget();
    settingsPage = new QWidget();
    vaccinsPage = new QWidget();
    personelPage = new QWidget();
    toolsPage = new QWidget();
    researchesPage = new QWidget();
    researchesTablePage = new QWidget();
    researchesFormAddPage = new QWidget();

    QVBoxLayout *patientsLayout = new QVBoxLayout(patientsPage);
    QVBoxLayout *researchersLayout = new QVBoxLayout(researchersPage);
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsPage);
    QVBoxLayout *vaccinsLayout = new QVBoxLayout(vaccinsPage);
    QVBoxLayout *personelLayout = new QVBoxLayout(personelPage);
    QVBoxLayout *researchesLayout = new QVBoxLayout(researchesPage);
    QVBoxLayout *toolsLayout = new QVBoxLayout(toolsPage);
    QVBoxLayout *researchesTableLayout = new QVBoxLayout(researchesTablePage);
    QVBoxLayout *researchesFormAddPageLayout = new QVBoxLayout(researchesFormAddPage);




    QLabel *patientsLabel = new QLabel("Patients Page");
    patientsLayout->addWidget(patientsLabel);

    QLabel *personelLabel = new QLabel("personel Page");
    personelLayout->addWidget(personelLabel);

    QLabel *researchersLabel = new QLabel("Chercheurs Page");
    researchersLayout->addWidget(researchersLabel);

    QLabel *settingsLabel = new QLabel("Paramètres Page");
    settingsLayout->addWidget(settingsLabel);

    QLabel *vaccinsLabel = new QLabel("Vaccin Page");
    vaccinsLayout->addWidget(vaccinsLabel);

    QLabel *toolsLabel = new QLabel("Tools Page");
    toolsLayout->addWidget(toolsLabel);

    QLabel *researchesLabel = new QLabel("Researches Page");
    researchesLayout->addWidget(researchesLabel);

    QLabel *researchesTableLabel = new QLabel("Researcehs Table Page");
    researchesTableLayout->addWidget(researchesTableLabel);


    setupTable();
    setupResearchCards();
    setupResearchesTablePage();
    setupResearchesFormAddPage();
    researchersLayout->addWidget(dataTable);

    stackedWidget->addWidget(patientsPage);
    stackedWidget->addWidget(researchersPage);
    stackedWidget->addWidget(vaccinsPage);
    stackedWidget->addWidget(personelPage);
    stackedWidget->addWidget(researchesPage);
    stackedWidget->addWidget(toolsPage);
    stackedWidget->addWidget(settingsPage);
    stackedWidget->addWidget(researchesTablePage);
    stackedWidget->addWidget(researchesFormAddPage);
}


void MainWindow::setupResearchCards()
{
    QVBoxLayout *researchLayout = qobject_cast<QVBoxLayout*>(researchesPage->layout());

    if (!researchLayout) return; // Ensure layout exists

    // Example card data
    struct ResearchCardData {
        QString title;
        QString description;
        QString iconPath;
    };

    QVector<ResearchCardData> researchData = {
        {"Vaccin COVID-19", "Recherche avancée sur les variants et la réponse immunitaire.", ":/icons/svg/vaccine.svg"},
        {"Thérapie Génique", "Nouvelles approches pour traiter les maladies génétiques.", ":/icons/svg/dna.svg"},
        {"Biotechnologie", "Développement de techniques innovantes pour la santé.", ":/icons/svg/microscope.svg"}
    };

    for (const auto &data : researchData) {
        QWidget *card = new QWidget();
        QHBoxLayout *cardLayout = new QHBoxLayout(card);

        QLabel *iconLabel = new QLabel();
        QPixmap iconPixmap(data.iconPath);
        iconLabel->setPixmap(iconPixmap.scaled(40, 40, Qt::KeepAspectRatio));
        iconLabel->setFixedSize(50, 50);

        QLabel *titleLabel = new QLabel("<b>" + data.title + "</b>");
        QLabel *descLabel = new QLabel(data.description);
        descLabel->setWordWrap(true);

        QVBoxLayout *textLayout = new QVBoxLayout();
        textLayout->addWidget(titleLabel);
        textLayout->addWidget(descLabel);

        cardLayout->addWidget(iconLabel);
        cardLayout->addLayout(textLayout);

        // Styling for the card
        card->setStyleSheet(
            "QWidget {"
            "   background-color: #ffffff;"
            "   border-radius: 20px;"
            "   padding: 12px;"
            "   border: 1px solid #ddd;"
            "} "
            "QLabel { font-size: 14px; }"
            );

        researchLayout->addWidget(card);
    }

    // Create and add the "Afficher plus" button
    QPushButton *afficherPlusButton = new QPushButton("Afficher plus");
    afficherPlusButton->setFlat(true);  // Makes the button text-only
    afficherPlusButton->setStyleSheet(
        "QPushButton {"
        "   color: #007BFF;"  // Text color (blue)
        "   font-size: 16px;"
        "   padding: 5px;"
        "   text-align: center;"
        "}"
        "QPushButton:hover {"
        "   color: #0056b3;"  // Darker blue on hover
        "}"
        );

    // Optionally, connect the button to a slot if you want to show more content or perform an action
    connect(afficherPlusButton, &QPushButton::clicked, this, &MainWindow::showResearchTablePage);

    // Add the button to the layout
    researchLayout->addWidget(afficherPlusButton);
}



void MainWindow::updateSidebarIcons(QPushButton *selectedButton)
{
    // Default icons for unselected buttons
    btnPatients->setIcon(QIcon(":/icons/svg/patient.svg"));
    btnPersonel->setIcon(QIcon(":/icons/svg/personel.svg"));
    btnResearches->setIcon(QIcon(":/icons/svg/flask.svg"));
    btnResearchers->setIcon(QIcon(":/icons/svg/scientist.svg"));
    btnTools->setIcon(QIcon(":/icons/svg/microscope.svg"));
    btnVaccins->setIcon(QIcon(":/icons/svg/syringe.svg"));
    btnSettings->setIcon(QIcon(":/icons/svg/settings.svg"));

    // Set the new icon for the selected button
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

void MainWindow::setupResearchesTablePage()
{
    if (!researchesTablePage) return;

    // Clear existing layout if any
    QLayout *existingLayout = researchesTablePage->layout();
    if (existingLayout) {
        QLayoutItem *item;
        while ((item = existingLayout->takeAt(0))) {
            delete item->widget();
            delete item;
        }
        delete existingLayout;
    }

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(researchesTablePage);
    researchesTablePage->setLayout(mainLayout);

    // 1. Create and configure the table view
    tableView = new QTableView();  // Use the member variable
    tableView->setModel(ProjetDeRecherche::Post());  // Use your Post() method

    // Table configuration
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // Read-only
    tableView->verticalHeader()->setVisible(false); // Hide row numbers

    // Auto-resize columns
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tableView->resizeColumnsToContents();

    // Optional: Enable sorting
    tableView->setSortingEnabled(true);

    // 2. Create button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Add Button
    QPushButton *addButton = new QPushButton("Add New Project");
    addButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   padding: 8px 16px;"
        "   border: none;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:hover { background-color: #45a049; }"
    );
    connect(addButton, &QPushButton::clicked, this, &MainWindow::showResearchFormAdd);

    // Refresh Button
    QPushButton *refreshButton = new QPushButton("Refresh");
    refreshButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #2196F3;"
        "   color: white;"
        "   padding: 8px 16px;"
        "   border: none;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:hover { background-color: #0b7dda; }"
    );
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        tableView->setModel(ProjetDeRecherche::Post()); // Refresh model
        tableView->setStyleSheet(
            "QTableView { color: black; }"              // All text in table
            "QTableView::item { color: black; }"        // Individual cells
            "QHeaderView::section { color: black; }"    // Column headers
            );
    });

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addStretch();

    // 3. Add widgets to main layout
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(tableView);

    // Optional: Set stretch factors
    mainLayout->setStretch(0, 0); // Buttons don't stretch
    mainLayout->setStretch(1, 1); // Table takes all remaining space
}

void MainWindow::setupResearchesFormAddPage()
{
    // Vérification et configuration de la page researchesFormAddPage
    if (!researchesFormAddPage) return;
    QVBoxLayout *formLayout = qobject_cast<QVBoxLayout*>(researchesFormAddPage->layout());
    if (!formLayout) {
        formLayout = new QVBoxLayout(researchesFormAddPage);
        researchesFormAddPage->setLayout(formLayout);
    }

    // Ajouter une carte blanche derrière le formulaire
    QFrame *card = new QFrame();
    card->setStyleSheet("background-color: #FFFFFF; border-radius: 10px; padding: 15px;");
    card->setMinimumHeight(300); // Adjust this value as needed
    QVBoxLayout *cardLayout = new QVBoxLayout(card);

    QLabel *titleLabel = new QLabel("Ajouter une Recherche");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: black;");
    cardLayout->addWidget(titleLabel);

    QStringList labels = {"TITRE", "SPONSOR", "PARTICIPANTS", "OBJECTIF", "LOCALISATION", "DESCRIPTION", "DATE_DEBUT", "DATE_FIN", "COUT"};
    QMap<QString, QWidget*> inputFields;

    QGridLayout *gridLayout = new QGridLayout();
    int row = 0, col = 0;

    for (const QString &labelText : labels) {
        QLabel *label = new QLabel(labelText + ":");
        label->setStyleSheet("color: black;");
        QWidget *input = new QLineEdit();  // All fields are QLineEdit now

        static_cast<QLineEdit*>(input)->setPlaceholderText("Entrez " + labelText.toLower());
        gridLayout->addWidget(label, row, col);
        gridLayout->addWidget(input, row, col + 1);

        col += 2;
        if (col >= 4) { // Move to the next row after 2 columns
            col = 0;
            row++;
        }

        input->setStyleSheet("background-color: #f8f8ff;");
        inputFields[labelText] = input;
    }

    cardLayout->addLayout(gridLayout);

    QPushButton *submitButton = new QPushButton("Ajouter");
    submitButton->setStyleSheet("background-color: #007BFF; color: white; padding: 10px; border-radius: 5px;");
    cardLayout->addWidget(submitButton);

    formLayout->addWidget(card);

    connect(submitButton, &QPushButton::clicked, this, [=]() {
        QString titre = qobject_cast<QLineEdit*>(inputFields["TITRE"])->text();
        QString sponsor = qobject_cast<QLineEdit*>(inputFields["SPONSOR"])->text();
        QString participants = qobject_cast<QLineEdit*>(inputFields["PARTICIPANTS"])->text();
        QString objectif = qobject_cast<QLineEdit*>(inputFields["OBJECTIF"])->text();
        QString localisation = qobject_cast<QLineEdit*>(inputFields["LOCALISATION"])->text();
        QString description = qobject_cast<QLineEdit*>(inputFields["DESCRIPTION"])->text();
        QString date_debut = qobject_cast<QLineEdit*>(inputFields["DATE_DEBUT"])->text();
        QString date_fin = qobject_cast<QLineEdit*>(inputFields["DATE_FIN"])->text();
        int cout = qobject_cast<QLineEdit*>(inputFields["COUT"])->text().toInt();

        // Validate fields
        if (titre.isEmpty() || sponsor.isEmpty() || participants.isEmpty() || objectif.isEmpty() ||
            localisation.isEmpty() || description.isEmpty() || date_debut.isEmpty() || date_fin.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs !");
            return;
        }

        // Create an object and insert it into the database
        ProjetDeRecherche projet(titre, sponsor, participants, objectif, localisation, description, cout, QDate::fromString(date_debut, "yyyy-MM-dd"), QDate::fromString(date_fin, "yyyy-MM-dd"));

        if (projet.Add()) {
            QMessageBox::information(this, "Succès", "Recherche ajoutée avec succès !");

            // Clear fields
            for (const QString &labelText : labels) {
                QWidget *input = inputFields[labelText];
                if (qobject_cast<QLineEdit*>(input)) {
                    qobject_cast<QLineEdit*>(input)->clear();
                }
            }
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'ajout de la recherche !");
        }
    });
}














void MainWindow::setupTable()
{
    // Create a QTableWidget with 4 rows & 7 columns
    dataTable = new QTableWidget(4, 7, this);

    // Lowercase headers to match the screenshot
    dataTable->setHorizontalHeaderLabels(
        {"nom", "prénom", "cin", "sexe", "date de naissance", "spécialité", "numtel"}
        );

    // ─────────────────────────────────────────────
    // Header Styling (Soft Gray, Bold Text, Single Bottom Border)
    // ─────────────────────────────────────────────
    dataTable->horizontalHeader()->setStyleSheet(
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

    // ─────────────────────────────────────────────
    // Table Styling (Rounded Corners, No Grid Lines)
    // ─────────────────────────────────────────────
    dataTable->setStyleSheet(
        "QTableWidget {"
        "    background-color: #FFFFFF;"
        "    border: 1px solid #ddd;"
        "    border-radius: 8px;"
        "}"
        /* Each cell has a bottom divider */
        "QTableWidget::item {"
        "    border-bottom: 1px solid #ddd;"
        "    padding: 8px;"
        "    color: #333;"
        "}"
        /* Subtle highlight on selection */
        "QTableWidget::item:selected {"
        "    background: #EDEDED;"
        "}"
        );

    // Hide the default grid lines (for a cleaner look)
    dataTable->setShowGrid(false);

    // Hide the vertical header (row numbers)
    dataTable->verticalHeader()->setVisible(false);

    // Stretch columns to fill the table width
    dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Set row height
    dataTable->verticalHeader()->setDefaultSectionSize(40);

    // Disable text wrapping for each cell
    dataTable->setWordWrap(false);

    // ─────────────────────────────────────────────
    // Sample Data
    // ─────────────────────────────────────────────
    QStringList row1 = {"Dr.Fahd", "Belhadj", "45865879", "Male", "JJ/MM/YYYY", "EXP", "99999999"};
    QStringList row2 = {"Dr.Houssem", "Chelbi", "49886111", "Male", "JJ/MM/YYYY", "EXP", "99999999"};
    QStringList row3 = {"Dr.Layth", "Lihidheb", "98745632", "Male", "JJ/MM/YYYY", "EXP", "99999999"};
    QStringList row4 = {"Dr.Mouheb", "Amdouni", "96968735", "Male", "JJ/MM/YYYY", "EXP", "99999999"};
    QStringList rows[] = {row1, row2, row3, row4};

    // Populate the table
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 7; ++j) {
            QTableWidgetItem *item = new QTableWidgetItem(rows[i][j]);

            // Make the "sexe" column bold
            if (j == 3) {
                QFont boldFont;
                boldFont.setBold(true);
                item->setFont(boldFont);
            }

            dataTable->setItem(i, j, item);
        }
    }
}



// Navigation Functions
void MainWindow::showPatientsPage()
{
    stackedWidget->setCurrentWidget(patientsPage);
    updateSidebarIcons(btnPatients);  // Update the icon for btnPatients
}


void MainWindow::showPersonelPage()
{
    stackedWidget->setCurrentWidget(personelPage);
    updateSidebarIcons(btnPersonel);
}

void MainWindow::showResearchPage()
{
    stackedWidget->setCurrentWidget(researchesPage);
    updateSidebarIcons(btnResearches);
}

void MainWindow::showResearchersPage()
{
    stackedWidget->setCurrentWidget(researchersPage);
    updateSidebarIcons(btnResearchers);
}

void MainWindow::showToolsPage()
{
    stackedWidget->setCurrentWidget(toolsPage);
    updateSidebarIcons(btnTools);
}

void MainWindow::showVaccinsPage()
{
    stackedWidget->setCurrentWidget(vaccinsPage);
    updateSidebarIcons(btnVaccins);
}

void MainWindow::showSettingsPage()
{
    stackedWidget->setCurrentWidget(settingsPage);
    updateSidebarIcons(btnSettings);
}

void MainWindow::showResearchTablePage()
{
    stackedWidget->setCurrentWidget(researchesTablePage);
}
void MainWindow::showResearchFormAdd()
{
    stackedWidget->setCurrentWidget(researchesFormAddPage);
}
