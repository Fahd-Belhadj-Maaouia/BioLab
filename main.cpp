#include "mainwindow.h"
#include "loginwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Création de l'objet de connexion
    connection c;
    bool test = c.createconnection();

    if (!test) {
        QMessageBox::critical(nullptr,
                              QObject::tr("Erreur de base de données"),
                              QObject::tr("La connexion à la base de données a échoué."),
                              QMessageBox::Cancel);
        return -1;
    }

    // Création des fenêtres
    LoginWindow login;
    MainWindow mainWindow;

    // Si login réussi, on ferme login et on affiche la page principale maximisée
    QObject::connect(&login, &LoginWindow::loginSuccessful, [&]() {
        login.close();
        mainWindow.showMaximized();  // ✅ Affiche avec barre de titre, pas en plein écran masqué
    });

    // Affiche la fenêtre de connexion
    login.show();

    return a.exec();
}
