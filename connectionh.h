#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <QSqlDatabase>
#include <QString>

class DatabaseConnection {
public:
    DatabaseConnection(); // Constructor
    ~DatabaseConnection(); // Destructor

    // Establish database connection
    bool createConnection(
        const QString& driverName = "QODBC",
        const QString& databaseName = "Source_Projet2A",
        const QString& userName = "wael",
        const QString& password = "123"
        );

    // Close existing database connection
    void closeConnection();

    // Get current database reference
    QSqlDatabase& getDatabase();

private:
    QSqlDatabase db; // Database object
};

#endif // DATABASE_CONNECTION_H
