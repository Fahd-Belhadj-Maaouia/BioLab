#include "mainwindow.h"
#include "loginwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    connection c;
    bool test = c.createconnection();

    if (!test) {
        QMessageBox::critical(nullptr, QObject::tr("Database Error"),
                              QObject::tr("La connexion à la base de données a échoué."), QMessageBox::Cancel);
        return -1;
    }

    // Lancement de la fenêtre de login
    LoginWindow login;
    MainWindow w;

    QObject::connect(&login, &LoginWindow::loginSuccessful, [&]() {
        w.show();
    });

    login.show();

    return a.exec();
}
