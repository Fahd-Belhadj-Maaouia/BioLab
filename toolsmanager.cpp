#include "toolsmanager.h"
#include "connection.h"
#include <QHeaderView>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>

ToolsManager::ToolsManager(QTableWidget *table, QObject *parent)
    : QObject(parent), toolsTable(table) {
    setupTable();
    loadTools();
}

ToolsManager::~ToolsManager() {
    // Cleanup handled by DatabaseConnection destructor
}

void ToolsManager::setupTable() {
    QStringList headers = {
        "ID", "Nom Matériel", "Catégorie", "Description",
        "Stock", "Quantité Maximale", "Image", "Fournisseur", "ID Projet"
    };

    toolsTable->setColumnCount(headers.size());
    toolsTable->setHorizontalHeaderLabels(headers);

    toolsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    toolsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    toolsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    toolsTable->horizontalHeader()->setStretchLastSection(true);
    toolsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

bool ToolsManager::validateToolData() const {
    if (nomMateriel.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le nom du matériel est obligatoire");
        return false;
    }

    if (categorie.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "La catégorie est obligatoire");
        return false;
    }

    if (stock < 0) {
        QMessageBox::warning(nullptr, "Erreur", "Le stock ne peut pas être négatif");
        return false;
    }

    if (quantiteMaximale < stock) {
        QMessageBox::warning(nullptr, "Erreur", "La quantité maximale doit être supérieure ou égale au stock");
        return false;
    }

    return true;
}

int ToolsManager::getNextId() {
    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Impossible de se connecter à la base de données");
        return 1; // Default ID
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT COALESCE(MAX(idR), 0) + 1 AS NEXT_ID FROM RessourcesMedicales");

    if (query.exec() && query.next()) {
        return query.value("NEXT_ID").toInt();
    } else {
        qDebug() << "Erreur lors de la recherche du prochain ID:" << query.lastError().text();
        return 1;
    }
}

void ToolsManager::loadTools() {
    toolsTable->setRowCount(0);

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Impossible de se connecter à la base de données");
        return;
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT idR, nomMateriel, categorie, description, stock, quantiteMaximale, uploadImage, fournisseur, idPro FROM RessourcesMedicales ORDER BY idR");

    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            toolsTable->insertRow(row);

            toolsTable->setItem(row, 0, new QTableWidgetItem(query.value("idR").toString()));
            toolsTable->setItem(row, 1, new QTableWidgetItem(query.value("nomMateriel").toString()));
            toolsTable->setItem(row, 2, new QTableWidgetItem(query.value("categorie").toString()));
            toolsTable->setItem(row, 3, new QTableWidgetItem(query.value("description").toString()));
            toolsTable->setItem(row, 4, new QTableWidgetItem(query.value("stock").toString()));
            toolsTable->setItem(row, 5, new QTableWidgetItem(query.value("quantiteMaximale").toString()));
            toolsTable->setItem(row, 6, new QTableWidgetItem(query.value("uploadImage").toString()));
            toolsTable->setItem(row, 7, new QTableWidgetItem(query.value("fournisseur").toString()));
            toolsTable->setItem(row, 8, new QTableWidgetItem(query.value("idPro").toString()));

            row++;
        }
    } else {
        qDebug() << "Erreur lors du chargement des outils:" << query.lastError().text();
        QMessageBox::warning(nullptr, "Erreur", "Impossible de charger les outils");
    }
}

bool ToolsManager::addTool() {
    if (!validateToolData()) {
        return false;
    }

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Impossible de se connecter à la base de données");
        return false;
    }

    QSqlDatabase& db = dbConnection.getDatabase();

    if (!db.transaction()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de démarrer la transaction");
        return false;
    }

    try {
        int nextId = getNextId();

        QSqlQuery query(db);
        query.prepare(
            "INSERT INTO RessourcesMedicales "
            "(idR, nomMateriel, categorie, description, stock, quantiteMaximale, uploadImage, fournisseur) "
            "VALUES (:idR, :nomMateriel, :categorie, :description, :stock, :quantiteMaximale, :uploadImage, :fournisseur)"
            );

        query.bindValue(":idR", nextId);
        query.bindValue(":nomMateriel", nomMateriel);
        query.bindValue(":categorie", categorie);
        query.bindValue(":description", description);
        query.bindValue(":stock", stock);
        query.bindValue(":quantiteMaximale", quantiteMaximale);
        query.bindValue(":uploadImage", uploadImage);
        query.bindValue(":fournisseur", fournisseur);


        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }

        loadTools();
        QMessageBox::information(nullptr, "Succès", "Outil ajouté avec succès");
        return true;

    } catch (const std::exception& e) {
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur", QString("Erreur lors de l'ajout de l'outil: %1").arg(e.what()));
        return false;
    }
}

bool ToolsManager::editTool() {
    if (!validateToolData()) {
        return false;
    }

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Impossible de se connecter à la base de données");
        return false;
    }

    QSqlDatabase& db = dbConnection.getDatabase();

    if (!db.transaction()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de démarrer la transaction");
        return false;
    }

    try {
        QSqlQuery query(db);
        query.prepare(
            "UPDATE RessourcesMedicales SET "
            "nomMateriel = :nomMateriel, categorie = :categorie, description = :description, "
            "stock = :stock, quantiteMaximale = :quantiteMaximale, uploadImage = :uploadImage, "
            "fournisseur = :fournisseur"
            "WHERE idR = :idR"
            );

        query.bindValue(":idR", id);
        query.bindValue(":nomMateriel", nomMateriel);
        query.bindValue(":categorie", categorie);
        query.bindValue(":description", description);
        query.bindValue(":stock", stock);
        query.bindValue(":quantiteMaximale", quantiteMaximale);
        query.bindValue(":uploadImage", uploadImage);
        query.bindValue(":fournisseur", fournisseur);
        query.bindValue(":idPro", idPro);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }

        loadTools();
        QMessageBox::information(nullptr, "Succès", "Outil modifié avec succès");
        return true;

    } catch (const std::exception& e) {
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur", QString("Erreur lors de la modification de l'outil: %1").arg(e.what()));
        return false;
    }
}

bool ToolsManager::deleteTool() {
    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Impossible de se connecter à la base de données");
        return false;
    }

    QSqlDatabase& db = dbConnection.getDatabase();

    if (!db.transaction()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de démarrer la transaction");
        return false;
    }

    try {
        QSqlQuery query(db);
        query.prepare("DELETE FROM RessourcesMedicales WHERE idR = :idR");
        query.bindValue(":idR", id);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }

        loadTools();
        QMessageBox::information(nullptr, "Succès", "Outil supprimé avec succès");
        return true;

    } catch (const std::exception& e) {
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur", QString("Erreur lors de la suppression de l'outil: %1").arg(e.what()));
        return false;
    }
}

QMap<QString, QVariant> ToolsManager::getToolById(int id) {
    QMap<QString, QVariant> toolData;

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Impossible de se connecter à la base de données");
        return toolData;
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT * FROM RessourcesMedicales WHERE idR = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        toolData["id"] = query.value("idR");
        toolData["nomMateriel"] = query.value("nomMateriel");
        toolData["categorie"] = query.value("categorie");
        toolData["description"] = query.value("description");
        toolData["stock"] = query.value("stock");
        toolData["quantiteMaximale"] = query.value("quantiteMaximale");
        toolData["uploadImage"] = query.value("uploadImage");
        toolData["fournisseur"] = query.value("fournisseur");
        toolData["idPro"] = query.value("idPro");
    } else {
        qDebug() << "Erreur lors de la récupération de l'outil:" << query.lastError().text();
    }

    return toolData;
}
