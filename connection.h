#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class connection {
public:
    connection();
    bool createconnection();
    void closeconnection();

private:
    QSqlDatabase db;  // Declare db as a member variable
};

#endif // CONNECTION_H
