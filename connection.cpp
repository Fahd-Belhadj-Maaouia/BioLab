#include "connection.h"
#include <QDebug>
#include <QSqlError>  // ✅ Ceci est nécessaire

connection::connection() {}

bool connection::createconnection() {
    if (QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QODBC", "qt_sql_default_connection");

    db.setDatabaseName("Source_Projet2A");
    db.setUserName("amdouni");
    db.setPassword("123");

    if (db.open()) {
        qDebug() << "✅ Database connected successfully.";
        return true;
    } else {
        qDebug() << "❌ Failed to connect DB:" << db.lastError().text();
        return false;
    }
}

void connection::closeconnection() {
    db.close();
}
