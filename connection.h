#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>

class connection
{
public:
    connection();
    bool createconnection();
    void closeconnection();
    QSqlDatabase& getDatabase() ;
    bool isOpen() const;
    bool isValid() const;
    bool reconnect();

private:
    QSqlDatabase db;
};

#endif // CONNECTION_H
