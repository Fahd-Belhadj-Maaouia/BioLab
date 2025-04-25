#include "connection.h"


connection::connection() {}

bool connection::createconnection() {
    bool test =false;
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("houssem");
    db.setPassword("123");
    if (db.open()) test=true;
    return test;
}

void connection::closeconnection() {db.close();}
