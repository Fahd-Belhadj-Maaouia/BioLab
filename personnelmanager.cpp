#include "personnelmanager.h"
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

PersonnelManager::PersonnelManager(QTableWidget *table, QObject *parent)
    : QObject(parent), personnelTable(table) {
    setupTable();
    loadPersonnel();
}

PersonnelManager::~PersonnelManager() {}

void PersonnelManager::setupTable() {
    QStringList headers = {
        "IDPER", "Nom", "Prénom", "Sexe", "CIN", "Numéro de Téléphone",
        "Email", "Adresse", "Date d'Ajout", "Rôle", "Email G", "Mot de Passe G"
    };

    personnelTable->setColumnCount(headers.size());
    personnelTable->setHorizontalHeaderLabels(headers);

    personnelTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    personnelTable->setSelectionMode(QAbstractItemView::SingleSelection);
    personnelTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    personnelTable->horizontalHeader()->setStretchLastSection(true);
    personnelTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);





    addPersonelForm = new QWidget();
    QFormLayout *formLayout = new QFormLayout(addPersonelForm);



    addPersonelForm->setStyleSheet(
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
void PersonnelManager::refreshTable() {
    personnelTable->clearContents(); // Efface le contenu actuel du tableau
    personnelTable->setRowCount(0);  // Réinitialise le nombre de lignes

    QSqlQuery query;
    query.exec("SELECT * FROM personnels"); // Remplacez "personnel" par le nom de votre table

    while (query.next()) {
        int row =personnelTable ->rowCount();
        personnelTable ->insertRow(row);

        for (int col = 0; col < 12; ++col) { // 12 colonnes dans votre tableau
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            personnelTable ->setItem(row, col, item);
        }
    }
}







bool PersonnelManager::validatePersonnelData() const {
    if (nom.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le nom est obligatoire");
        return false;
    }

    if (prenom.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le prénom est obligatoire");
        return false;
    }

    if (cin.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le CIN est obligatoire");
        return false;
    }

    if (numTel.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le numéro de téléphone est obligatoire");
        return false;
    }

    if (email.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "L'email est obligatoire");
        return false;
    }

    if (role.trimmed().isEmpty() || (role != "vaccinateur" && role != "secretaire")) {
        QMessageBox::warning(nullptr, "Erreur", "Le rôle doit être 'vaccinateur' ou 'secretaire'");
        return false;
    }

    return true;
}

int PersonnelManager::getNextId() {
    if (!dbConnection.createconnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return 1;
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT COALESCE(MAX(IDPER), 0) + 1 AS NEXT_ID FROM personnel");

    if (query.exec() && query.next()) {
        return query.value("NEXT_ID").toInt();
    } else {
        qDebug() << "Erreur lors de la recherche du prochain ID:"
                 << query.lastError().text();
        return 1;
    }
}

void PersonnelManager::loadPersonnel() {
    personnelTable->setRowCount(0);

    if (!dbConnection.createconnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return;
    }

    QSqlDatabase& db = dbConnection.getDatabase(); // Correction de la syntaxe
    QSqlQuery query(db); // Déclaration de la variable query
    query.prepare("SELECT IDPER, NOM, PRENOM, SEXE, CIN, NUMTEL, EMAIL, ADRESSE, "
                  "TO_CHAR(DATEAJOUT, 'DD/MM/YYYY') as DATEAJOUT_FORMATTED, ROLE, EMAILG, MOTDEPASSEG "
                  "FROM personnel ORDER BY IDPER");

    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            personnelTable->insertRow(row);

            personnelTable->setItem(row, 0, new QTableWidgetItem(query.value("IDPER").toString()));
            personnelTable->setItem(row, 1, new QTableWidgetItem(query.value("NOM").toString()));
            personnelTable->setItem(row, 2, new QTableWidgetItem(query.value("PRENOM").toString()));
            personnelTable->setItem(row, 3, new QTableWidgetItem(query.value("SEXE").toString()));
            personnelTable->setItem(row, 4, new QTableWidgetItem(query.value("CIN").toString()));
            personnelTable->setItem(row, 5, new QTableWidgetItem(query.value("NUMTEL").toString()));
            personnelTable->setItem(row, 6, new QTableWidgetItem(query.value("EMAIL").toString()));
            personnelTable->setItem(row, 7, new QTableWidgetItem(query.value("ADRESSE").toString()));
            personnelTable->setItem(row, 8, new QTableWidgetItem(query.value("DATEAJOUT_FORMATTED").toString()));
            personnelTable->setItem(row, 9, new QTableWidgetItem(query.value("ROLE").toString()));
            personnelTable->setItem(row, 10, new QTableWidgetItem(query.value("EMAILG").toString()));
            personnelTable->setItem(row, 11, new QTableWidgetItem(query.value("MOTDEPASSEG").toString()));

            row++;
        }
    } else {
        qDebug() << "Erreur lors du chargement du personnel:"
                 << query.lastError().text();
        QMessageBox::warning(nullptr, "Erreur",
                             "Impossible de charger le personnel");
    }
}

bool PersonnelManager::addPersonnel() {
    if (!validatePersonnelData()) {
        return false;
    }

    if (!dbConnection.createconnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
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
            "INSERT INTO personnels "
            "(IDPER, NOM, PRENOM, SEXE, CIN, NUMTEL, EMAIL, ADRESSE, DATEAJOUT, ROLE, EMAILG, MOTDEPASSEG) "
            "VALUES (:IDPER, :NOM, :PRENOM, :SEXE, :CIN, :NUMTEL, :EMAIL, :ADRESSE, TO_DATE(:DATEAJOUT, 'DD/MM/YYYY'), :ROLE, :EMAILG, :MOTDEPASSEG)"
            );

        query.bindValue(":IDPER", nextId);
        query.bindValue(":NOM", nom);
        query.bindValue(":PRENOM", prenom);
        query.bindValue(":SEXE", sexe);
        query.bindValue(":CIN", cin);
        query.bindValue(":NUMTEL", numTel);
        query.bindValue(":EMAIL", email);
        query.bindValue(":ADRESSE", adresse);
        query.bindValue(":DATEAJOUT", dateAjout.toString("dd/MM/yyyy"));
        query.bindValue(":ROLE", role);
        query.bindValue(":EMAILG", emailG);
        query.bindValue(":MOTDEPASSEG", motDePasseG);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }

        loadPersonnel();

        QMessageBox::information(nullptr, "Succès", "Personnel ajouté avec succès");
        return true;

    } catch (const std::exception& e) {
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur",
                             QString("Erreur lors de l'ajout du personnel: %1").arg(e.what()));
        return false;
    }
}

bool PersonnelManager::editPersonnel() {
    if (!validatePersonnelData()) {
        return false;
    }

    if (!dbConnection.createconnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return false;
    }

    QSqlDatabase& db = dbConnection.getDatabase();

    if (!db.transaction()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de démarrer la transaction");
        return false;
    }

    try {
        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT COUNT(*) FROM personnels WHERE IDPER = :IDPER");
        checkQuery.bindValue(":IDPER", idPer);

        if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
            throw std::runtime_error("ID de personnel non trouvé");
        }

        QSqlQuery query(db);
        query.prepare(
            "UPDATE personnels SET "
            "NOM = :NOM, PRENOM = :PRENOM, SEXE = :SEXE, CIN = :CIN, "
            "NUMTEL = :NUMTEL, EMAIL = :EMAIL, ADRESSE = :ADRESSE, "
            "DATEAJOUT = TO_DATE(:DATEAJOUT, 'DD/MM/YYYY'), ROLE = :ROLE, "
            "EMAILG = :EMAILG, MOTDEPASSEG = :MOTDEPASSEG "
            "WHERE IDPER = :IDPER"
            );

        query.bindValue(":IDPER", idPer);
        query.bindValue(":NOM", nom);
        query.bindValue(":PRENOM", prenom);
        query.bindValue(":SEXE", sexe);
        query.bindValue(":CIN", cin);
        query.bindValue(":NUMTEL", numTel);
        query.bindValue(":EMAIL", email);
        query.bindValue(":ADRESSE", adresse);
        query.bindValue(":DATEAJOUT", dateAjout.toString("dd/MM/yyyy"));
        query.bindValue(":ROLE", role);
        query.bindValue(":EMAILG", emailG);
        query.bindValue(":MOTDEPASSEG", motDePasseG);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }

        loadPersonnel();

        QMessageBox::information(nullptr, "Succès", "Personnels modifié avec succès");
        return true;

    } catch (const std::exception& e) {
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur",
                             QString("Erreur lors de la modification du personnels: %1").arg(e.what()));
        return false;
    }
}

bool PersonnelManager::deletePersonnel() {
    // Vérifiez que l'ID est valide
    if (idPer <= 0) {
        QMessageBox::warning(nullptr, "Erreur", "ID de personnel invalide.");
        return false;
    }

    // Connexion à la base de données
    if (!dbConnection.createconnection()) {
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
        // Vérifiez que l'ID existe dans la base de données
        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT COUNT(*) FROM personnels WHERE IDPER = :IDPER");
        checkQuery.bindValue(":IDPER", idPer);

        if (!checkQuery.exec()) {
            throw std::runtime_error("Erreur lors de la vérification de l'ID : " + checkQuery.lastError().text().toStdString());
        }

        if (!checkQuery.next() || checkQuery.value(0).toInt() == 0) {
            throw std::runtime_error("ID de personnel non trouvé");
        }

        // Supprimez le personnel
        QSqlQuery query(db);
        query.prepare("DELETE FROM personnels WHERE IDPER = :IDPER");
        query.bindValue(":IDPER", idPer);

        if (!query.exec()) {
            throw std::runtime_error("Erreur lors de la suppression : " + query.lastError().text().toStdString());
        }

        // Validez la transaction
        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }

        // Rafraîchissez le tableau
        loadPersonnel();

        // Affichez un message de succès
        QMessageBox::information(nullptr, "Succès", "Personnel supprimé avec succès");
        return true;

    } catch (const std::exception& e) {
        // En cas d'erreur, annulez la transaction
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur",
                             QString("Erreur lors de la suppression du personnel : %1").arg(e.what()));
        return false;
    }
}

bool PersonnelManager::loadPersonnelSummary(QTableWidget *summaryTable) {
    if (!summaryTable) {
        qDebug() << "ERREUR : Pointeur de summaryTable est nul";
        return false;
    }

    if (!dbConnection.createconnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return false;
    }

    QSqlDatabase db = dbConnection.getDatabase();

    summaryTable->clear();
    summaryTable->setRowCount(0);
    summaryTable->setColumnCount(4);
    summaryTable->setHorizontalHeaderLabels({"Nom", "Prénom", "Rôle", "Date d'Ajout"});

    QSqlQuery query(db);
    query.prepare("SELECT NOM, PRENOM, ROLE, TO_CHAR(DATEAJOUT, 'DD/MM/YYYY') as DATEAJOUT_FORMATTED "
                  "FROM personnels WHERE ROWNUM <= 4");

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

QMap<QString, QVariant> PersonnelManager::getPersonnelById(int id) {
    QMap<QString, QVariant> personnelsData;

    if (!dbConnection.createconnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return personnelsData;
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT * FROM personnels"); {
        qDebug() << "Erreur lors de la récupération du personnel:" << query.lastError().text();
    }

    return personnelsData;
}

QMap<QString, int> PersonnelManager::getPersonnelRoleStats() {
    QMap<QString, int> roleStats;

    if (!dbConnection.createconnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return roleStats;
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT ROLE, COUNT(*) as TOTAL FROM personnels GROUP BY ROLE ORDER BY TOTAL DESC");

    if (query.exec()) {
        while (query.next()) {
            QString role = query.value("ROLE").toString();
            int count = query.value("TOTAL").toInt();
            roleStats[role] = count;
        }
    } else {
        qDebug() << "Erreur lors de la récupération des statistiques de rôle:"
                 << query.lastError().text();
    }

    return roleStats;
}

QStringList PersonnelManager::getRecentlyAddedPersonnel(int daysThreshold) {
    QStringList recentPersonnel;

    if (!dbConnection.createconnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return recentPersonnel;
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT NOM, PRENOM, TO_CHAR(DATEAJOUT, 'DD/MM/YYYY') as DATEAJOUT_FORMATTED "
                  "FROM personnels "
                  "WHERE DATEAJOUT BETWEEN SYSDATE - :threshold AND SYSDATE "
                  "ORDER BY DATEAJOUT DESC");
    query.bindValue(":threshold", daysThreshold);

    if (query.exec()) {
        while (query.next()) {
            QString nom = query.value("NOM").toString();
            QString prenom = query.value("PRENOM").toString();
            QString dateAjout = query.value("DATEAJOUT_FORMATTED").toString();
            recentPersonnel.append(QString("%1 %2 (Ajouté le %3)").arg(nom, prenom, dateAjout));
        }
    } else {
        qDebug() << "Erreur lors de la récupération du personnel récemment ajouté:"
                 << query.lastError().text();
    }

    return recentPersonnel;
}
