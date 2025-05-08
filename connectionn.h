#ifndef CONNECTIONN_H
#define CONNECTIONN_H

#include <QSqlDatabase>

class connectionn
{
public:
    connectionn();
    bool createconnectionn();
    void closeconnectionn();
    QSqlDatabase& getDatabase() ;
    bool isOpen() const;
    bool isValid() const;
    bool reconnect();

private:
    QSqlDatabase db;
};

#endif // CONNECTIONN_H
