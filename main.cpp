#include "mainwindow.h"
#include "loginwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Établir la connexion à la base
    connection c;
    if(!c.createconnection()) {
        QMessageBox::critical(nullptr, "Erreur", "Connexion à la base de données échouée");
        return -1;
    }

    // Créer la fenêtre de login
    LoginWindow loginWindow;

    // Créer la fenêtre principale mais ne pas l'afficher encore
    MainWindow mainWindow;
    mainWindow.hide(); // Cacher la fenêtre principale au démarrage

    // Connecter le signal de login réussi
    QObject::connect(&loginWindow, &LoginWindow::loginSuccessful,
                     [&](int userId, QString nom, QString prenom) {
                         mainWindow.setCurrentUser(userId, nom, prenom);
                         mainWindow.showMaximized();
                         loginWindow.hide(); // Cache la fenêtre de login plutôt que de la fermer
                     });

    // Connecter le signal de déconnexion
    QObject::connect(&mainWindow, &MainWindow::logoutRequested, [&]() {
        mainWindow.hide();
        loginWindow.show();
    });

    // Afficher la fenêtre de login au démarrage
    loginWindow.show();

    return a.exec();
}
