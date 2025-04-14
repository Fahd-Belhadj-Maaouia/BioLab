#include "connection.h"


connection::connection() {}

bool connection::createconnection() {
    db = QSqlDatabase::addDatabase("QODBC", "main_connection"); // <-- add connection name
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("Recherche");
    db.setPassword("esprit18");

    if (db.open()) {
        qDebug() << "Database connection opened successfully.";
        return true;
    } else {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return false;
    }
}

void connection::closeconnection() {db.close();}

