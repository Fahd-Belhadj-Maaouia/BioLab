#include <QApplication>
#include "connection.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    connection c;
    if (!c.createconnection()) {
        qDebug() << "❌ Failed to open the database!";
        return -1;
    } else {
        qDebug() << "✅ Database connection opened successfully!";
    }

    MainWindow w;
    w.show();

    return a.exec();
}
