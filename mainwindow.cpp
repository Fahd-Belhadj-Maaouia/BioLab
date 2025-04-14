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
#include "buttondelegate.h"  // Add this line with your other includes
#include<QCheckBox>
#include<QListWidgetItem>
#include <QInputDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include<QListWidget>





// mainwindow.cpp (constructor)
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    todoManager(new ToDoList(this))
{

    stackedWidget = new QStackedWidget(this);
    setupUI();
}

// mainwindow.cpp

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

void MainWindow::handleDeleteRow(const QModelIndex &index) {
    qDebug() << "Delete requested for row:" << index.row();
    // TODO: Add your delete confirmation + delete logic here
}

// mainwindow.cpp

void MainWindow::refreshResearchTable() {
    QSqlQueryModel *newModel = ProjetDeRecherche::Post();
    tableView->setModel(newModel);
    tableView->hideColumn(0);

    int actionsColumn = newModel->columnCount();
    newModel->insertColumn(actionsColumn);
    newModel->setHeaderData(actionsColumn, Qt::Horizontal, "Actions");

    tableView->setItemDelegateForColumn(actionsColumn, buttonDelegate); // reuse the same one
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
    researchesFormUpdatePage = new QWidget();

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


    setupResearchCards();
    setupResearchesTablePage();
    setupResearchesFormAddPage();


    stackedWidget->addWidget(patientsPage);
    stackedWidget->addWidget(researchersPage);
    stackedWidget->addWidget(vaccinsPage);
    stackedWidget->addWidget(personelPage);
    stackedWidget->addWidget(researchesPage);
    stackedWidget->addWidget(toolsPage);
    stackedWidget->addWidget(settingsPage);
    stackedWidget->addWidget(researchesTablePage);
    stackedWidget->addWidget(researchesFormAddPage);
    stackedWidget->addWidget(researchesFormUpdatePage);
}
//-------------------------------SETUP RESEARCH CARDS-----------------------------

void MainWindow::setupResearchCards()
{
    QVBoxLayout *researchLayout = qobject_cast<QVBoxLayout*>(researchesPage->layout());
    if (!researchLayout) return;

    // Clear existing widgets
    QLayoutItem* item;
    while ((item = researchLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // ========== TOP SECTION - 3 STAT CARDS ==========
    QWidget *topCardsContainer = new QWidget();
    QHBoxLayout *topCardsLayout = new QHBoxLayout(topCardsContainer);
    topCardsLayout->setSpacing(20);
    topCardsLayout->setContentsMargins(0, 0, 0, 0);

    // Create 3 statistic cards
    QStringList statTitles = {"Ongoing Projects", "Completed Tasks", "Team Members"};
    QStringList statValues = {"12", "24", "8"};
    QStringList statIcons = {":/icons/projects.svg", ":/icons/completed.svg", ":/icons/team.svg"};

    for (int i = 0; i < 3; i++) {
        QWidget *card = new QWidget();
        card->setMinimumSize(200, 200);
        card->setStyleSheet(
            "QWidget {"
            "   background-color: #ffffff;"
            "   border-radius: 15px;"
            "   padding: 15px;"
            "   border: 1px solid #e0e0e0;"
            "}"
            );

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setAlignment(Qt::AlignCenter);

        // Icon
        QLabel *icon = new QLabel();
        icon->setPixmap(QPixmap(statIcons[i]).scaled(40, 40, Qt::KeepAspectRatio));
        icon->setAlignment(Qt::AlignCenter);

        // Value
        QLabel *value = new QLabel(statValues[i]);
        value->setStyleSheet(
            "QLabel {"
            "   font-size: 32px;"
            "   font-weight: bold;"
            "   color: #2c3e50;"
            "}"
            );
        value->setAlignment(Qt::AlignCenter);

        // Title
        QLabel *title = new QLabel(statTitles[i]);
        title->setStyleSheet(
            "QLabel {"
            "   font-size: 14px;"
            "   color: #7f8c8d;"
            "}"
            );
        title->setAlignment(Qt::AlignCenter);

        cardLayout->addWidget(icon);
        cardLayout->addWidget(value);
        cardLayout->addWidget(title);
        cardLayout->addStretch();

        topCardsLayout->addWidget(card);
    }
    topCardsLayout->addStretch();

    // ========== BOTTOM SECTION - TO-DO LISTS ==========
    QWidget *bottomCardsContainer = new QWidget();
    QHBoxLayout *bottomCardsLayout = new QHBoxLayout(bottomCardsContainer);
    bottomCardsLayout->setSpacing(20);
    bottomCardsLayout->setContentsMargins(0, 0, 0, 0);

    // Create the task lists
    QListWidget *todoList = new QListWidget();
    QListWidget *completedList = new QListWidget();


    todoManager->loadTasksFromDB(todoList, completedList);

    // Style the lists with black text
    QString listStyle =
        "QListWidget {"
        "   border: none;"
        "   background: transparent;"
        "}"
        "QListWidget::item {"
        "   color: black;"  // Changed to black
        "   padding: 5px;"
        "}"
        "QListWidget::item:selected {"
        "   background-color: #e0e0e0;"
        "   border-radius: 5px;"
        "}";

    todoList->setStyleSheet(listStyle);
    completedList->setStyleSheet(listStyle);

    todoManager->loadTasksFromDB(todoList, completedList);  // <-- ADD THIS LINE


    // Create and add the cards
    bottomCardsLayout->addWidget(createTaskCard("To-Do List", todoList));
    bottomCardsLayout->addWidget(createTaskCard("Completed", completedList));
    bottomCardsLayout->addStretch();

    // ========== MAIN LAYOUT ==========
    researchLayout->addWidget(topCardsContainer);
    researchLayout->addSpacing(30);
    researchLayout->addWidget(bottomCardsContainer);
    researchLayout->addStretch();

    // Add the "Afficher plus" button
    QPushButton *afficherPlusButton = new QPushButton("Afficher plus");
    afficherPlusButton->setFlat(true);
    afficherPlusButton->setStyleSheet(
        "QPushButton {"
        "   color: #007BFF;"
        "   font-size: 16px;"
        "   padding: 8px 16px;"
        "   margin-top: 10px;"
        "   text-align: center;"
        "}"
        "QPushButton:hover {"
        "   color: #0056b3;"
        "}"
        );
    connect(afficherPlusButton, &QPushButton::clicked, this, &MainWindow::showResearchTablePage);
    researchLayout->addWidget(afficherPlusButton, 0, Qt::AlignCenter);
}




//-------------------------------CREATE TASK CARD------------------------------------------------------------

QWidget* MainWindow::createTaskCard(const QString &title, QListWidget *taskList)
{
    QWidget *card = new QWidget();
    card->setMinimumSize(250, 250);
    card->setStyleSheet("QWidget { background-color: #ffffff; border-radius: 15px; padding: 10px; border: 1px solid #e0e0e0; }");

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #333; padding-bottom: 5px;");

    taskList->setStyleSheet(
        "QListWidget {"
        "   border: none;"
        "   background: transparent;"
        "}"
        "QListWidget::item {"
        "   color: black;"  // Ensured text is black
        "   padding: 5px;"
        "}"
        "QListWidget::item:selected {"
        "   background-color: #e0e0e0;"
        "   border-radius: 5px;"
        "}"
        );

    // Button container
    QWidget *buttonContainer = new QWidget();
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonContainer);
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    // Add button
    QPushButton *addButton = new QPushButton("+");
    addButton->setFixedSize(30, 30);
    addButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border-radius: 15px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
        );

    // Delete button
    QPushButton *deleteButton = new QPushButton("×");
    deleteButton->setFixedSize(30, 30);
    deleteButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #f44336;"
        "   color: white;"
        "   border-radius: 15px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #d32f2f;"
        "}"
        );
    deleteButton->setEnabled(false);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();

    // Card layout
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(taskList);
    cardLayout->addWidget(buttonContainer);

    // Connect signals - now using todoManager
    connect(taskList, &QListWidget::itemSelectionChanged, [=](){
        deleteButton->setEnabled(taskList->currentItem() != nullptr);
    });

    connect(addButton, &QPushButton::clicked, [=](){
        bool ok;
        QString text = QInputDialog::getText(this,
                                             "Add Task",
                                             "Task name:",
                                             QLineEdit::Normal,
                                             "",
                                             &ok);
        if (ok && !text.isEmpty()) {
            bool isCompleted = (title == "Completed");
            todoManager->addTaskToDB(text, isCompleted);  // Using todoManager
            todoManager->addTaskToDB(text, isCompleted);
            todoManager->loadTasksFromDB(todoList, completedList);  // Refresh the lists
        }
    });

    connect(deleteButton, &QPushButton::clicked, [=](){
        QListWidgetItem *item = taskList->currentItem();
        if (item) {
            bool isCompleted = (title == "Completed");
            todoManager->deleteTaskFromDB(item->text(), isCompleted);
            todoManager->loadTasksFromDB(todoList, completedList);  // Refresh the lists
        }
    });

    // Double-click to move between lists
    if (title == "To-Do List") {
        connect(taskList, &QListWidget::itemDoubleClicked, [=](QListWidgetItem *item){
            todoManager->moveTaskInDB(item->text(), true);  // Move to completed
            emit taskMovedToCompleted(item->text());  // Signal to update other list
            delete item;
        });
    } else if (title == "Completed") {
        connect(taskList, &QListWidget::itemDoubleClicked, [=](QListWidgetItem *item){
            todoManager->moveTaskInDB(item->text(), false);  // Move back to todo
            emit taskMovedToTodo(item->text());  // Signal to update other list
            delete item;
        });
    }

    return card;
}


//---------------------------------------------------------------------------------------------------------------------------

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

void MainWindow::setupResearchesTablePage() {
    if (!researchesTablePage) return;

    // Clear existing layout
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

    // Create table view with minimal styling
    tableView = new QTableView();
    tableView->setStyleSheet("QTableView { color: black; }");

    // Set model and configure columns
    QSqlQueryModel *model = ProjetDeRecherche::Post();
    tableView->setModel(model);
    tableView->hideColumn(0); // Hide ID column

    // Add actions column
    model->insertColumn(model->columnCount());
    model->setHeaderData(model->columnCount() - 1, Qt::Horizontal, "Actions");

    // Table configuration
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->verticalHeader()->setVisible(false);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tableView->resizeColumnsToContents();
    tableView->setSortingEnabled(true);
    tableView->horizontalHeader()->setStretchLastSection(true);

    // Create and set button delegate
    buttonDelegate = new ButtonDelegate(tableView); // only once

    connect(buttonDelegate, &ButtonDelegate::deleteClicked, this, [this](const QModelIndex &index) {
        int row = index.row();
        int id = tableView->model()->data(tableView->model()->index(row, 0)).toInt();

        qDebug() << " Delete triggered, ID:" << id;

        if (ProjetDeRecherche::Delete(id)) {
            qDebug() << "Deleted successfully";
            refreshResearchTable(); // reload model with same delegate
        } else {
            qDebug() << "Delete FAILED for ID:" << id;
            QMessageBox::warning(this, "Error", "Delete failed");
        }
    });



    connect(buttonDelegate, &ButtonDelegate::updateClicked, this, [this](const QModelIndex &index) {
        int row = index.row();
        int id = tableView->model()->data(tableView->model()->index(row, 0)).toInt();

        // Clear any existing content from the update page
        QLayout* oldLayout = researchesFormUpdatePage->layout();
        if (oldLayout) {
            QLayoutItem* item;
            while ((item = oldLayout->takeAt(0))) {
                delete item->widget();
                delete item;
            }
            delete oldLayout;
        }

        // Setup the form with the project data
        this->setupResearchesFormUpdatePage(id);

        // Explicitly navigate to the update page
        stackedWidget->setCurrentWidget(researchesFormUpdatePage);
        qDebug() << "Navigated to update page for project ID:" << id;
    });


    // Add control buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *addButton = new QPushButton("Add New Project");
    addButton->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; padding: 8px 16px; "
        "border: none; border-radius: 4px; }"
        "QPushButton:hover { background-color: #45a049; }"
        );
    connect(addButton, &QPushButton::clicked, this, &MainWindow::showResearchFormAdd);

    QPushButton *refreshButton = new QPushButton("Refresh");
    refreshButton->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; padding: 8px 16px; "
        "border: none; border-radius: 4px; }"
        "QPushButton:hover { background-color: #0b7dda; }"
        );
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        refreshResearchTable();
    });


    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(tableView);
    mainLayout->setStretch(0, 0);
    mainLayout->setStretch(1, 1);
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

        input->setStyleSheet("background-color: #f8f8ff; color: black;");
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
void MainWindow::setupResearchesFormUpdatePage(int projectId)
{
    // Vérification et configuration de la page researchesFormUpdatePage
    if (!researchesFormUpdatePage) return;
    QVBoxLayout *formLayout = qobject_cast<QVBoxLayout*>(researchesFormUpdatePage->layout());
    if (!formLayout) {
        formLayout = new QVBoxLayout(researchesFormUpdatePage);
        researchesFormUpdatePage->setLayout(formLayout);
    }

    // Ajouter une carte blanche derrière le formulaire
    QFrame *card = new QFrame();
    card->setStyleSheet("background-color: #FFFFFF; border-radius: 10px; padding: 15px;");
    card->setMinimumHeight(300); // Adjust this value as needed
    QVBoxLayout *cardLayout = new QVBoxLayout(card);

    QLabel *titleLabel = new QLabel("Mettre à jour une Recherche");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: black;");
    cardLayout->addWidget(titleLabel);

    QStringList labels = {"TITRE", "SPONSOR", "PARTICIPANTS", "OBJECTIF", "LOCALISATION", "DESCRIPTION", "DATE_DEBUT", "DATE_FIN", "COUT"};
    QMap<QString, QWidget*> inputFields;

    QGridLayout *gridLayout = new QGridLayout();
    int row = 0, col = 0;

    // Fetch the project data from the database
    ProjetDeRecherche projet = ProjetDeRecherche::getById(projectId);  // Assuming you have a method getById to retrieve the project by ID

    // Fill the fields with the current project data
    for (const QString &labelText : labels) {
        QLabel *label = new QLabel(labelText + ":");
        label->setStyleSheet("color: black;");
        QWidget *input = new QLineEdit();  // All fields are QLineEdit now

        // Set the placeholder text and current value from the project
        static_cast<QLineEdit*>(input)->setPlaceholderText("Entrez " + labelText.toLower());

        if (labelText == "TITRE") {
            static_cast<QLineEdit*>(input)->setText(projet.getTITRE());
        } else if (labelText == "SPONSOR") {
            static_cast<QLineEdit*>(input)->setText(projet.getSPONSOR());
        } else if (labelText == "PARTICIPANTS") {
            static_cast<QLineEdit*>(input)->setText(projet.getPARTICIPANTS());
        } else if (labelText == "OBJECTIF") {
            static_cast<QLineEdit*>(input)->setText(projet.getOBJECTIF());
        } else if (labelText == "LOCALISATION") {
            static_cast<QLineEdit*>(input)->setText(projet.getLOCALISATION());
        } else if (labelText == "DESCRIPTION") {
            static_cast<QLineEdit*>(input)->setText(projet.getDESCRIPTION());
        } else if (labelText == "DATE_DEBUT") {
            static_cast<QLineEdit*>(input)->setText(projet.getDATE_DEBUT().toString("yyyy-MM-dd"));
        } else if (labelText == "DATE_FIN") {
            static_cast<QLineEdit*>(input)->setText(projet.getDATE_FIN().toString("yyyy-MM-dd"));
        } else if (labelText == "COUT") {
            static_cast<QLineEdit*>(input)->setText(QString::number(projet.getCOUT()));
        }

        gridLayout->addWidget(label, row, col);
        gridLayout->addWidget(input, row, col + 1);

        col += 2;
        if (col >= 4) { // Move to the next row after 2 columns
            col = 0;
            row++;
        }

        input->setStyleSheet("background-color: #f8f8ff; color: black;");
        inputFields[labelText] = input;
    }

    cardLayout->addLayout(gridLayout);

    QPushButton *submitButton = new QPushButton("Mettre à jour");
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

        // Create an object and update it in the database
        ProjetDeRecherche projet(titre, sponsor, participants, objectif, localisation, description, cout, QDate::fromString(date_debut, "yyyy-MM-dd"), QDate::fromString(date_fin, "yyyy-MM-dd"));

        if (projet.Update(projectId)) {
            QMessageBox::information(this, "Succès", "Recherche mise à jour avec succès !");

            // Optionally: Clear the fields or navigate back to the main page
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la mise à jour de la recherche !");
        }
    });
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

