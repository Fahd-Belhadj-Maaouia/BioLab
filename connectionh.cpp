#include "connectionh.h"
#include <QSqlError>
#include <QDebug>
#include <QRandomGenerator>

DatabaseConnection::DatabaseConnection() {}

DatabaseConnection::~DatabaseConnection() {
    closeConnection();
}

bool DatabaseConnection::createConnection(
    const QString& driverName,
    const QString& databaseName,
    const QString& userName,
    const QString& password
    ) {
    // Fermer toute connexion existante
    if (db.isOpen()) {
        closeConnection();
    }

    // Générer un nom de connexion unique
    QString connectionName = QString("ODBC%1")
                                 .arg(QRandomGenerator::global()->generate());

    // Ajouter la base de données avec un nom de connexion unique
    db = QSqlDatabase::addDatabase(driverName, connectionName);
    db.setDatabaseName(databaseName);
    db.setUserName(userName);
    db.setPassword(password);

    // Tentative d'ouverture de la connexion avec gestion des erreurs améliorée
    try {
        if (!db.open()) {
            qDebug() << "Erreur de connexion:" << db.lastError().text();
            QSqlDatabase::removeDatabase(connectionName);
            return false;
        }

        qDebug() << "Connexion établie avec succès. Nom:" << connectionName;
        return true;

    } catch (const std::exception& e) {
        qDebug() << "Exception lors de la connexion:" << e.what();
        return false;
    }
}

void DatabaseConnection::closeConnection() {
    if (db.isOpen()) {
        QString connectionName = db.connectionName();

        // Rollback de toute transaction en cours
        if (db.transaction()) {
            db.rollback();
        }

        db.close();
        QSqlDatabase::removeDatabase(connectionName);

        qDebug() << "Connexion fermée:" << connectionName;
    }
}

QSqlDatabase& DatabaseConnection::getDatabase() {
    return db;
}
