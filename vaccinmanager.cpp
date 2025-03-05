#include "vaccinmanager.h"
#include "connection.h"
#include <QHeaderView>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>


VaccinManager::VaccinManager(QTableWidget *table, QObject *parent)
    : QObject(parent), vaccinTable(table) {
    setupTable();
    loadVaccins();
}

VaccinManager::~VaccinManager() {
    // Nettoyage géré par le destructeur de DatabaseConnection
}

void VaccinManager::setupTable() {
    // Configuration des en-têtes du tableau
    QStringList headers = {
        "IDV", "Nom Vaccin", "Référence", "Type",
        "Maladie Chronique", "Nombre de Doses",
        "Quantité", "Date d'Expiration"
    };

    vaccinTable->setColumnCount(headers.size());
    vaccinTable->setHorizontalHeaderLabels(headers);

    // Paramètres de sélection et d'affichage
    vaccinTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    vaccinTable->setSelectionMode(QAbstractItemView::SingleSelection);
    vaccinTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Ajustement des colonnes
    vaccinTable->horizontalHeader()->setStretchLastSection(true);
    vaccinTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

bool VaccinManager::validateVaccinData() const {
    // Validation complète des données avant insertion/modification
    if (nomVaccin.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le nom du vaccin est obligatoire");
        return false;
    }

    if (reference.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "La référence est obligatoire");
        return false;
    }

    if (nbDose <= 0) {
        QMessageBox::warning(nullptr, "Erreur", "Le nombre de doses doit être positif");
        return false;
    }

    if (quantite < 0) {
        QMessageBox::warning(nullptr, "Erreur", "La quantité ne peut pas être négative");
        return false;
    }

    if (!dateExp.isValid()) {
        QMessageBox::warning(nullptr, "Erreur", "Date d'expiration invalide");
        return false;
    }

    // Optionnel : Vérification que la date n'est pas dans le passé
    if (dateExp < QDate::currentDate()) {
        QMessageBox::warning(nullptr, "Erreur", "La date d'expiration ne peut pas être dans le passé");
        return false;
    }

    return true;
}

int VaccinManager::getNextId() {
    // Obtenir le prochain ID disponible
    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return 1; // ID par défaut
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT COALESCE(MAX(IDV), 0) + 1 AS NEXT_ID FROM vaccins");

    if (query.exec() && query.next()) {
        return query.value("NEXT_ID").toInt();
    } else {
        qDebug() << "Erreur lors de la recherche du prochain ID:"
                 << query.lastError().text();
        return 1;
    }
}

void VaccinManager::loadVaccins() {
    // Réinitialiser le tableau
    vaccinTable->setRowCount(0);

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return;
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT * FROM vaccins ORDER BY IDV");

    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            vaccinTable->insertRow(row);

            // Ajout des éléments au tableau
            vaccinTable->setItem(row, 0, new QTableWidgetItem(query.value("IDV").toString()));
            vaccinTable->setItem(row, 1, new QTableWidgetItem(query.value("NOMVACCIN").toString()));
            vaccinTable->setItem(row, 2, new QTableWidgetItem(query.value("REFERENCE").toString()));
            vaccinTable->setItem(row, 3, new QTableWidgetItem(query.value("TYPE").toString()));
            vaccinTable->setItem(row, 4, new QTableWidgetItem(query.value("MALADIECHRONIQUE").toString()));
            vaccinTable->setItem(row, 5, new QTableWidgetItem(query.value("NBDOSE").toString()));
            vaccinTable->setItem(row, 6, new QTableWidgetItem(query.value("QUANTITE").toString()));

            // Conversion de la date pour l'affichage
            QDate expDate = QDate::fromString(query.value("DATEEXP").toString(), "yyyy-MM-dd");
            vaccinTable->setItem(row, 7, new QTableWidgetItem(expDate.toString("dd/MM/yyyy")));

            row++;
        }
    } else {
        qDebug() << "Erreur lors du chargement des vaccins:"
                 << query.lastError().text();
        QMessageBox::warning(nullptr, "Erreur",
                             "Impossible de charger les vaccins");
    }
}

bool VaccinManager::addVaccin() {
    // Validation des données avant insertion
    if (!validateVaccinData()) {
        return false;
    }

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return false;
    }

    QSqlDatabase& db = dbConnection.getDatabase();

    // Début de la transaction
    if (!db.transaction()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de démarrer la transaction");
        return false;
    }

    try {
        int nextId = getNextId();

        QSqlQuery query(db);
        query.prepare(
            "INSERT INTO vaccins "
            "(IDV, NOMVACCIN, REFERENCE, TYPE, MALADIECHRONIQUE, NBDOSE, QUANTITE, DATEEXP) "
            "VALUES (:IDV, :NOMVACCIN, :REFERENCE, :TYPE, :MALADIECHRONIQUE, :NBDOSE, :QUANTITE, "
            "TO_DATE(:DATEEXP, 'YYYY-MM-DD'))"
            );

        query.bindValue(":IDV", nextId);
        query.bindValue(":NOMVACCIN", nomVaccin);
        query.bindValue(":REFERENCE", reference);
        query.bindValue(":TYPE", type);
        query.bindValue(":MALADIECHRONIQUE", maladieChronique);
        query.bindValue(":NBDOSE", nbDose);
        query.bindValue(":QUANTITE", quantite);
        query.bindValue(":DATEEXP", dateExp.toString("yyyy-MM-dd"));

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        // Commit de la transaction
        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }

        // Recharger les données après insertion
        loadVaccins();

        QMessageBox::information(nullptr, "Succès", "Vaccin ajouté avec succès");
        return true;

    } catch (const std::exception& e) {
        // Rollback en cas d'erreur
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur",
                             QString("Erreur lors de l'ajout du vaccin: %1").arg(e.what()));
        return false;
    }
}

bool VaccinManager::editVaccin() {
    // Validation des données avant modification
    if (!validateVaccinData()) {
        return false;
    }

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return false;
    }

    QSqlDatabase& db = dbConnection.getDatabase();

    // Début de la transaction
    if (!db.transaction()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de démarrer la transaction");
        return false;
    }

    try {
        // Vérifier l'existence du vaccin
        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT COUNT(*) FROM vaccins WHERE IDV = :IDV");
        checkQuery.bindValue(":IDV", id);

        if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
            throw std::runtime_error("ID de vaccin non trouvé");
        }

        // Requête de mise à jour
        QSqlQuery query(db);
        query.prepare(
            "UPDATE vaccins SET "
            "NOMVACCIN = :NOMVACCIN, REFERENCE = :REFERENCE, "
            "TYPE = :TYPE, MALADIECHRONIQUE = :MALADIECHRONIQUE, "
            "NBDOSE = :NBDOSE, QUANTITE = :QUANTITE, "
            "DATEEXP = TO_DATE(:DATEEXP, 'YYYY-MM-DD') "
            "WHERE IDV = :IDV"
            );

        query.bindValue(":IDV", id);
        query.bindValue(":NOMVACCIN", nomVaccin);
        query.bindValue(":REFERENCE", reference);
        query.bindValue(":TYPE", type);
        query.bindValue(":MALADIECHRONIQUE", maladieChronique);
        query.bindValue(":NBDOSE", nbDose);
        query.bindValue(":QUANTITE", quantite);
        query.bindValue(":DATEEXP", dateExp.toString("yyyy-MM-dd"));

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        // Commit de la transaction
        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }

        // Recharger les données après modification
        loadVaccins();

        QMessageBox::information(nullptr, "Succès", "Vaccin modifié avec succès");
        return true;

    } catch (const std::exception& e) {
        // Rollback en cas d'erreur
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur",
                             QString("Erreur lors de la modification du vaccin: %1").arg(e.what()));
        return false;
    }
}

bool VaccinManager::deleteVaccin() {
    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return false;
    }

    QSqlDatabase& db = dbConnection.getDatabase();

    // Début de la transaction
    if (!db.transaction()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de démarrer la transaction");
        return false;
    }

    try {
        // Vérifier l'existence du vaccin
        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT COUNT(*) FROM vaccins WHERE IDV = :IDV");
        checkQuery.bindValue(":IDV", id);

        if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
            throw std::runtime_error("ID de vaccin non trouvé");
        }

        // Requête de suppression
        QSqlQuery query(db);
        query.prepare("DELETE FROM vaccins WHERE IDV = :IDV");
        query.bindValue(":IDV", id);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        // Commit de la transaction
        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }

        // Recharger les données après suppression
        loadVaccins();

        QMessageBox::information(nullptr, "Succès", "Vaccin supprimé avec succès");
        return true;

    } catch (const std::exception& e) {
        // Rollback en cas d'erreur
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur",
                             QString("Erreur lors de la suppression du vaccin: %1").arg(e.what()));
        return false;
    }
}

bool VaccinManager::loadVaccinSummary(QTableWidget *summaryTable) {
    if (!summaryTable) {
        qDebug() << "ERREUR : Pointeur de summaryTable est nul";
        return false;
    }

    QSqlDatabase db = dbConnection.getDatabase();

    // Liste des tables disponibles
    QSqlQuery queryTables(db);
    queryTables.prepare("SELECT table_name FROM user_tables");

    if (queryTables.exec()) {
        qDebug() << "Tables disponibles :";
        while (queryTables.next()) {
            qDebug() << queryTables.value(0).toString();
        }
    }

    // Requête avec nom de table dynamique
    QSqlQuery query(db);

    try {
        // Essayez différentes variations de noms de table
        QStringList tableNames = {
            "VACCINS",
            "vaccins",
            "Vaccins",
            "VOTRE_SCHEMA.VACCINS",
            "VOTRE_SCHEMA.vaccins"
        };

        bool tableFound = false;
        QString foundTableName;

        for (const QString& tableName : tableNames) {
            query.prepare(QString("SELECT NOMVACCIN, TYPE, NBDOSE, QUANTITE FROM %1 WHERE ROWNUM <= 4").arg(tableName));

            if (query.exec()) {
                tableFound = true;
                foundTableName = tableName;
                break;
            } else {
                qDebug() << "Échec avec la table :" << tableName
                         << "Erreur :" << query.lastError().text();
            }
        }

        if (!tableFound) {
            qDebug() << "ERREUR CRITIQUE : Aucune table de vaccins trouvée";
            return false;
        }

        // Réinitialisation du tableau
        summaryTable->clear();
        summaryTable->setRowCount(0);
        summaryTable->setColumnCount(4);
        summaryTable->setHorizontalHeaderLabels({"Nom", "Type", "Doses", "Quantité"});

        int row = 0;
        while (query.next()) {
            summaryTable->insertRow(row);
            summaryTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
            summaryTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
            summaryTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
            summaryTable->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
            row++;
        }

        qDebug() << "Chargement terminé avec" << row << "lignes depuis la table" << foundTableName;
        return true;
    }
    catch (const std::exception& e) {
        qDebug() << "Exception lors du chargement :" << e.what();
        return false;
    }
}
