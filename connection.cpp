#include "connection.h"

connection::connection() {}

bool connection::createconnection() {
    bool test = false;
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("wael");
    db.setPassword("123");
    if (db.open()) test = true;
    return test;
}

void connection::closeconnection() {
    db.close();
}

QSqlDatabase& connection::getDatabase() {
    return db;
}

bool connection::isOpen() const {
    return db.isOpen();
}

bool connection::isValid() const {
    return db.isValid();
}

bool connection::reconnect() {
    if (isOpen()) closeconnection();
    return createconnection();
}
