#include "toolsmanager.h"
#include "connection.h"
#include <QSqlQuery>
#include <QHeaderView>
#include <QSqlError>
#include <QDebug>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include<QSqlQuery>

ToolsManager::ToolsManager(QTableWidget *toolstable, QWidget *parent)
    : QObject(parent), toolstable(toolstable) {
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

    loadTools();

    addToolForm = new QWidget();
    QFormLayout *formLayout = new QFormLayout(addToolForm);

    nomMaterielInput = new QLineEdit();
    categorieInput = new QLineEdit();
    descriptionInput = new QLineEdit();
    stockInput = new QSpinBox();
    stockInput->setMinimum(0);
    quantiteMaximaleInput = new QSpinBox();
    quantiteMaximaleInput->setMinimum(0);
    uploadImageButton = new QPushButton("Upload Image");
    fournisseurInput = new QLineEdit();

    formLayout->addRow("Nom Materiel:", nomMaterielInput);
    formLayout->addRow("Catégorie:", categorieInput);
    formLayout->addRow("Description:", descriptionInput);
    formLayout->addRow("Stock:", stockInput);
    formLayout->addRow("Quantité Maximale:", quantiteMaximaleInput);
    formLayout->addRow("Image:", uploadImageButton);
    formLayout->addRow("Fournisseur:", fournisseurInput);

    QPushButton *submitButton = new QPushButton("Ajouter");
    formLayout->addWidget(submitButton);

    connect(submitButton, &QPushButton::clicked, this, &ToolsManager::onAddToolSubmitted);
    connect(uploadImageButton, &QPushButton::clicked, this, &ToolsManager::onUploadImageClicked);

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

ToolsManager::~ToolsManager() {}

int ToolsManager::getId() const {
    return id;
}

QString ToolsManager::getNomMateriel() const {
    return nomMateriel;
}

QString ToolsManager::getCategorie() const {
    return categorie;
}

QString ToolsManager::getDescription() const {
    return description;
}

int ToolsManager::getStock() const {
    return stock;
}

int ToolsManager::getQuantiteMaximale() const {
    return quantiteMaximale;
}

QByteArray ToolsManager::getUploadImage() const {
    return uploadImage;
}

QString ToolsManager::getFournisseur() const {
    return fournisseur;
}

void ToolsManager::setId(int id) {
    this->id = id;
}

void ToolsManager::setNomMateriel(const QString &nomMateriel) {
    this->nomMateriel = nomMateriel;
}

void ToolsManager::setCategorie(const QString &categorie) {
    this->categorie = categorie;
}

void ToolsManager::setDescription(const QString &description) {
    this->description = description;
}

void ToolsManager::setStock(int stock) {
    this->stock = stock;
}

void ToolsManager::setQuantiteMaximale(int quantiteMaximale) {
    this->quantiteMaximale = quantiteMaximale;
}

void ToolsManager::setUploadImage(const QByteArray &uploadImage) {
    this->uploadImage = uploadImage;
}

void ToolsManager::setFournisseur(const QString &fournisseur) {
    this->fournisseur = fournisseur;
}

void ToolsManager::loadTools() {
    toolstable->setRowCount(0);

    connection conn;
    if (!conn.createconnection()) {
        qDebug() << "Failed to connect to the database.";
        return;
    }

    QSqlQuery query;
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

    conn.closeconnection();
}

void ToolsManager::addTool() {
    connection conn;
    if (!conn.createconnection()) {
        qDebug() << "Failed to connect to the database.";
        return;
    }

    qDebug() << "Nom Materiel:" << nomMateriel;
    qDebug() << "Categorie:" << categorie;
    qDebug() << "Description:" << description;
    qDebug() << "Stock:" << stock;
    qDebug() << "Quantite Maximale:" << quantiteMaximale;
    qDebug() << "Fournisseur:" << fournisseur;
    qDebug() << "Upload Image Size:" << uploadImage.size() << "bytes";

    QSqlQuery query;
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
        loadTools();
    } else {
        qDebug() << "Failed to add tool:" << query.lastError().text();
    }

    conn.closeconnection();
}

void ToolsManager::editTool() {
    connection conn;
    if (!conn.createconnection()) {
        qDebug() << "Failed to connect to the database.";
        return;
    }

    QSqlQuery query;
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
        loadTools();
    } else {
        qDebug() << "Failed to update tool:" << query.lastError().text();
    }

    conn.closeconnection();
}

void ToolsManager::deleteTool() {
    connection conn;
    if (!conn.createconnection()) {
        qDebug() << "Failed to connect to the database.";
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM MATERIELS WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "Tool deleted successfully.";
        loadTools();
    } else {
        qDebug() << "Failed to delete tool:" << query.lastError().text();
    }

    conn.closeconnection();
}

void ToolsManager::showAddToolForm() {
    addToolForm->show();
}

void ToolsManager::onAddToolSubmitted() {
    QString nomMateriel = nomMaterielInput->text();
    QString categorie = categorieInput->text();
    QString description = descriptionInput->text();
    int stock = stockInput->value();
    int quantiteMaximale = quantiteMaximaleInput->value();
    QString fournisseur = fournisseurInput->text();

    setNomMateriel(nomMateriel);
    setCategorie(categorie);
    setDescription(description);
    setStock(stock);
    setQuantiteMaximale(quantiteMaximale);
    setFournisseur(fournisseur);
    setUploadImage(uploadImageData);

    addTool();

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
