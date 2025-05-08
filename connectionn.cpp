#include "connectionn.h"

connectionn::connectionn() {}

bool connectionn::createconnectionn() {
    bool test = false;
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("wael");
    db.setPassword("123");
    if (db.open()) test = true;
    return test;
}

void connectionn::closeconnectionn() {
    db.close();
}

QSqlDatabase& connectionn::getDatabase() {
    return db;
}

bool connectionn::isOpen() const {
    return db.isOpen();
}

bool connectionn::isValid() const {
    return db.isValid();
}

bool connectionn::reconnect() {
    if (isOpen()) closeconnectionn();
    return createconnectionn();
}
