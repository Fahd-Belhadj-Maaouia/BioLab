#include "mainwindow.h"
#include "loginwindow.h"
#include "connection.h"
#include <QApplication>
#include "connectionn.h"


#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1) Connexion à la base
    connection c;
    if(!c.createconnection()) {
        QMessageBox::critical(nullptr, "Erreur", "Connexion à la base de données échouée");
        return -1;
    }
    QMessageBox::information(nullptr, "Info", "Connected to database");

    // 2) Création des fenêtres
    LoginWindow loginWindow;
    MainWindow mainWindow;
    mainWindow.hide();

    // 3) Connexion des signaux/slots AVANT d'afficher la fenêtre de login
    QObject::connect(&loginWindow, &LoginWindow::loginSuccessful,
                     [&](int userId, QString nom, QString prenom) {
                         mainWindow.setCurrentUser(userId, nom, prenom);
                         mainWindow.resize(1000, 600);
                         mainWindow.show();
                         loginWindow.hide();
                     });

    QObject::connect(&mainWindow, &MainWindow::logoutRequested, [&]() {
        mainWindow.hide();
        loginWindow.show();
    });

    // 4) Affichage de la fenêtre de login
    loginWindow.show();

    return a.exec();
}
