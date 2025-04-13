#include "mainwindow.h"
#include <QApplication>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifndef QT_CHARTS_LIB
    qFatal("Qt Charts not available!");
#endif

    DatabaseConnection dbConnection;
    bool connectionResult = dbConnection.createConnection();

    if (connectionResult) {
        qDebug() << "Database connection established successfully";

    } else {
        qDebug() << "Failed to establish database connection";
    }

    MainWindow w;
    w.resize(1000, 600);
    w.show();

    return a.exec();
}
