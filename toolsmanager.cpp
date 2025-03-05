#include "toolsmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

ToolsManager::ToolsManager(QTableWidget *toolstable, QWidget *parent)
    : QObject(parent), toolstable(toolstable) {
    // Initialize the tools table
    toolstable->setColumnCount(8);
    toolstable->setHorizontalHeaderLabels({"ID", "Nom", "Catégorie", "Description", "Stock", "Quantité Maximale", "Image", "Fournisseur"});
    toolstable->horizontalHeader()->setStyleSheet(
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
    toolstable->setStyleSheet(
        "QTableWidget {"
        "    background-color: #FFFFFF;"
        "    border: 1px solid #ddd;"
        "    border-radius: 8px;"
        "}"
        "QTableWidget::item {"
        "    border-bottom: 1px solid #ddd;"
        "    padding: 8px;"
        "    color: #333;"
        "}"
        "QTableWidget::item:selected {"
        "    background: #EDEDED;"
        "}"
        );
    toolstable->setShowGrid(false);
    toolstable->verticalHeader()->setVisible(false);
    toolstable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    toolstable->verticalHeader()->setDefaultSectionSize(40);
    toolstable->setWordWrap(false);

    // Load tools from the database
    loadTools();

    // Create the form widget
    addToolForm = new QWidget();
    QFormLayout *formLayout = new QFormLayout(addToolForm);

    // Add input fields
    nomMaterielInput = new QLineEdit();
    categorieInput = new QLineEdit();
    descriptionInput = new QLineEdit();
    stockInput = new QSpinBox();
    stockInput->setMinimum(0);
    quantiteMaximaleInput = new QSpinBox();
    quantiteMaximaleInput->setMinimum(0);
    uploadImageButton = new QPushButton("Upload Image");
    fournisseurInput = new QLineEdit();

    // Add fields to the form
    formLayout->addRow("Nom Materiel:", nomMaterielInput);
    formLayout->addRow("Catégorie:", categorieInput);
    formLayout->addRow("Description:", descriptionInput);
    formLayout->addRow("Stock:", stockInput);
    formLayout->addRow("Quantité Maximale:", quantiteMaximaleInput);
    formLayout->addRow("Image:", uploadImageButton);
    formLayout->addRow("Fournisseur:", fournisseurInput);

    // Add a submit button
    QPushButton *submitButton = new QPushButton("Ajouter");
    formLayout->addWidget(submitButton);

    // Connect the submit button
    connect(submitButton, &QPushButton::clicked, this, &ToolsManager::onAddToolSubmitted);

    // Connect the upload button
    connect(uploadImageButton, &QPushButton::clicked, this, &ToolsManager::onUploadImageClicked);

    // Style the form
    addToolForm->setStyleSheet(
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
}

ToolsManager::~ToolsManager() {
    // Clean up dynamically allocated widgets
    delete addToolForm;
}

// Getters implementation
int ToolsManager::getId() const { return id; }
QString ToolsManager::getNomMateriel() const { return nomMateriel; }
QString ToolsManager::getCategorie() const { return categorie; }
QString ToolsManager::getDescription() const { return description; }
int ToolsManager::getStock() const { return stock; }
int ToolsManager::getQuantiteMaximale() const { return quantiteMaximale; }
QByteArray ToolsManager::getUploadImage() const { return uploadImage; }
QString ToolsManager::getFournisseur() const { return fournisseur; }

// Setters implementation
void ToolsManager::setId(int newId) { id = newId; }
void ToolsManager::setNomMateriel(const QString &nom) { nomMateriel = nom; }
void ToolsManager::setCategorie(const QString &cat) { categorie = cat; }
void ToolsManager::setDescription(const QString &desc) { description = desc; }
void ToolsManager::setStock(int stockValue) { stock = stockValue; }
void ToolsManager::setQuantiteMaximale(int qteMax) { quantiteMaximale = qteMax; }
void ToolsManager::setUploadImage(const QByteArray &image) { uploadImage = image; }
void ToolsManager::setFournisseur(const QString &four) { fournisseur = four; }

void ToolsManager::loadTools() {
    // Clear the table
    toolstable->setRowCount(0);

    // Open database connection
    if (!dbConnection.createConnection()) {
        qDebug() << "Failed to connect to the database.";
        return;
    }

    // Query the database
    QSqlQuery query(dbConnection.getDatabase());
    if (query.exec("SELECT * FROM MATERIELS")) {
        while (query.next()) {
            int row = toolstable->rowCount();
            toolstable->insertRow(row);

            for (int col = 0; col < 8; ++col) {
                QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                toolstable->setItem(row, col, item);
            }
        }
    } else {
        qDebug() << "Query failed:" << query.lastError().text();
    }
}

void ToolsManager::addTool() {
    // Open database connection
    if (!dbConnection.createConnection()) {
        qDebug() << "Failed to connect to the database.";
        return;
    }

    // Debug: Print the values being inserted
    qDebug() << "Nom Materiel:" << nomMateriel;
    qDebug() << "Categorie:" << categorie;
    qDebug() << "Description:" << description;
    qDebug() << "Stock:" << stock;
    qDebug() << "Quantite Maximale:" << quantiteMaximale;
    qDebug() << "Fournisseur:" << fournisseur;
    qDebug() << "Upload Image Size:" << uploadImage.size() << "bytes";

    // Insert new tool into the database
    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("INSERT INTO MATERIELS (NOMMATERIEL, CATEGORIE, DESCRIPTION, STOCK, QUANTITEMAXIMALE, UPLOADIMAGE, FOURNISSEUR) "
                  "VALUES (:nomMateriel, :categorie, :description, :stock, :quantiteMaximale, :uploadImage, :fournisseur)");
    query.bindValue(":nomMateriel", nomMateriel);
    query.bindValue(":categorie", categorie);
    query.bindValue(":description", description);
    query.bindValue(":stock", stock);
    query.bindValue(":quantiteMaximale", quantiteMaximale);
    query.bindValue(":uploadImage", uploadImage);
    query.bindValue(":fournisseur", fournisseur);

    if (query.exec()) {
        qDebug() << "Tool added successfully.";
        loadTools(); // Refresh the table
    } else {
        qDebug() << "Failed to add tool:" << query.lastError().text();
    }
}

void ToolsManager::editTool() {
    // Open database connection
    if (!dbConnection.createConnection()) {
        qDebug() << "Failed to connect to the database.";
        return;
    }

    // Update tool in the database
    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("UPDATE MATERIELS SET NOMMATERIEL = :nomMateriel, CATEGORIE = :categorie, DESCRIPTION = :description, "
                  "STOCK = :stock, QUANTITEMAXIMALE = :quantiteMaximale, UPLOADIMAGE = :uploadImage, FOURNISSEUR = :fournisseur "
                  "WHERE ID = :id");
    query.bindValue(":id", id);
    query.bindValue(":nomMateriel", nomMateriel);
    query.bindValue(":categorie", categorie);
    query.bindValue(":description", description);
    query.bindValue(":stock", stock);
    query.bindValue(":quantiteMaximale", quantiteMaximale);
    query.bindValue(":uploadImage", uploadImage);
    query.bindValue(":fournisseur", fournisseur);

    if (query.exec()) {
        qDebug() << "Tool updated successfully.";
        loadTools(); // Refresh the table
    } else {
        qDebug() << "Failed to update tool:" << query.lastError().text();
    }
}

void ToolsManager::deleteTool() {
    // Open database connection
    if (!dbConnection.createConnection()) {
        qDebug() << "Failed to connect to the database.";
        return;
    }

    // Delete tool from the database
    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("DELETE FROM MATERIELS WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "Tool deleted successfully.";
        loadTools(); // Refresh the table
    } else {
        qDebug() << "Failed to delete tool:" << query.lastError().text();
    }
}

void ToolsManager::showAddToolForm() {
    addToolForm->show();
}

void ToolsManager::onAddToolSubmitted() {
    // Get input values
    QString nomMateriel = nomMaterielInput->text();
    QString categorie = categorieInput->text();
    QString description = descriptionInput->text();
    int stock = stockInput->value();
    int quantiteMaximale = quantiteMaximaleInput->value();
    QString fournisseur = fournisseurInput->text();

    // Set values in ToolsManager
    setNomMateriel(nomMateriel);
    setCategorie(categorie);
    setDescription(description);
    setStock(stock);
    setQuantiteMaximale(quantiteMaximale);
    setFournisseur(fournisseur);
    setUploadImage(uploadImageData);

    // Add the tool to the database
    addTool();

    // Clear the form
    nomMaterielInput->clear();
    categorieInput->clear();
    descriptionInput->clear();
    stockInput->setValue(0);
    quantiteMaximaleInput->setValue(0);
    fournisseurInput->clear();
    uploadImageData.clear();
}

void ToolsManager::onUploadImageClicked() {
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Upload Image", "", "Images (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            uploadImageData = file.readAll();
            file.close();
            QMessageBox::information(nullptr, "Success", "Image uploaded successfully.");
        } else {
            QMessageBox::warning(nullptr, "Error", "Failed to open image file.");
        }
    }
}
