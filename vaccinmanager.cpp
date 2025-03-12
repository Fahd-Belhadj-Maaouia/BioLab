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
    // Utiliser TO_CHAR pour formater la date directement dans la requête SQL
    query.prepare("SELECT IDV, NOMVACCIN, REFERENCE, TYPE, MALADIECHRONIQUE, NBDOSE, QUANTITE, "
                  "TO_CHAR(DATEEXP, 'DD/MM/YYYY') as DATEEXP_FORMATTED "
                  "FROM vaccins ORDER BY IDV");

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

            // Utiliser directement la date formatée par Oracle
            vaccinTable->setItem(row, 7, new QTableWidgetItem(query.value("DATEEXP_FORMATTED").toString()));

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
            "TO_DATE(:DATEEXP, 'DD/MM/YYYY'))"
            );

        query.bindValue(":IDV", nextId);
        query.bindValue(":NOMVACCIN", nomVaccin);
        query.bindValue(":REFERENCE", reference);
        query.bindValue(":TYPE", type);
        query.bindValue(":MALADIECHRONIQUE", maladieChronique);
        query.bindValue(":NBDOSE", nbDose);
        query.bindValue(":QUANTITE", quantite);
        // Format de date pour Oracle lors de l'insertion
        query.bindValue(":DATEEXP", dateExp.toString("dd/MM/yyyy"));

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
            "DATEEXP = TO_DATE(:DATEEXP, 'DD/MM/YYYY') "
            "WHERE IDV = :IDV"
            );

        query.bindValue(":IDV", id);
        query.bindValue(":NOMVACCIN", nomVaccin);
        query.bindValue(":REFERENCE", reference);
        query.bindValue(":TYPE", type);
        query.bindValue(":MALADIECHRONIQUE", maladieChronique);
        query.bindValue(":NBDOSE", nbDose);
        query.bindValue(":QUANTITE", quantite);
        // Format de date pour Oracle lors de la modification
        query.bindValue(":DATEEXP", dateExp.toString("dd/MM/yyyy"));

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

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return false;
    }

    QSqlDatabase db = dbConnection.getDatabase();

    // Réinitialisation du tableau
    summaryTable->clear();
    summaryTable->setRowCount(0);
    summaryTable->setColumnCount(4);
    summaryTable->setHorizontalHeaderLabels({"Nom", "Type", "Doses", "Quantité"});

    QSqlQuery query(db);
    query.prepare("SELECT NOMVACCIN, TYPE, NBDOSE, QUANTITE FROM vaccins WHERE ROWNUM <= 4");

    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            summaryTable->insertRow(row);
            summaryTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
            summaryTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
            summaryTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
            summaryTable->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
            row++;
        }

        qDebug() << "Chargement terminé avec" << row << "lignes";
        return true;
    } else {
        qDebug() << "Erreur lors du chargement du résumé:" << query.lastError().text();
        return false;
    }
}
QMap<QString, QVariant> VaccinManager::getVaccinById(int id) {
    QMap<QString, QVariant> vaccinData;

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return vaccinData;
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT * FROM vaccins WHERE IDV = :id");  // Make sure to use IDV instead of id
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        vaccinData["id"] = query.value("IDV");
        vaccinData["nom_vaccin"] = query.value("NOMVACCIN");
        vaccinData["reference"] = query.value("REFERENCE");
        vaccinData["type"] = query.value("TYPE");
        vaccinData["maladie_chronique"] = query.value("MALADIECHRONIQUE");
        vaccinData["nb_dose"] = query.value("NBDOSE");
        vaccinData["quantite"] = query.value("QUANTITE");
        vaccinData["date_exp"] = query.value("DATEEXP");
    } else {
        qDebug() << "Error fetching vaccin:" << query.lastError().text();
    }

    return vaccinData;
}
// Add this method to your VaccinManager class in vaccinmanager.h
QMap<QString, int> VaccinManager::getVaccinTypeStats() {
    QMap<QString, int> typeStats;

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return typeStats;
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT TYPE, SUM(QUANTITE) as TOTAL FROM vaccins GROUP BY TYPE ORDER BY TOTAL DESC");

    if (query.exec()) {
        while (query.next()) {
            QString type = query.value("TYPE").toString();
            int quantity = query.value("TOTAL").toInt();
            typeStats[type] = quantity;
        }
    } else {
        qDebug() << "Erreur lors de la récupération des statistiques de type:"
                 << query.lastError().text();
    }

    return typeStats;
}

// Add this method to VaccinManager to get soon-to-expire vaccines
QStringList VaccinManager::getSoonToExpireVaccins(int daysThreshold) {
    QStringList expiringVaccins;

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return expiringVaccins;
    }

    QSqlQuery query(dbConnection.getDatabase());
    // Get vaccines expiring within daysThreshold days
    query.prepare("SELECT NOMVACCIN, TO_CHAR(DATEEXP, 'DD/MM/YYYY') as DATEEXP_FORMATTED "
                  "FROM vaccins "
                  "WHERE DATEEXP BETWEEN SYSDATE AND SYSDATE + :threshold "
                  "ORDER BY DATEEXP");
    query.bindValue(":threshold", daysThreshold);

    if (query.exec()) {
        while (query.next()) {
            QString vaccin = query.value("NOMVACCIN").toString();
            QString expDate = query.value("DATEEXP_FORMATTED").toString();
            expiringVaccins.append(QString("Attention: %1 expire le %2").arg(vaccin, expDate));
        }
    } else {
        qDebug() << "Erreur lors de la récupération des vaccins expirants:"
                 << query.lastError().text();
    }

    return expiringVaccins;
}
