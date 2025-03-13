#ifndef PERSONNELMANAGER_H
#define PERSONNELMANAGER_H

#include <QObject>
#include <QTableWidget>
#include <QSqlQuery>
#include <QDate>
#include <QMessageBox>
#include <QMap>
#include <QVariant>
#include <QStringList>
#include "connection.h"

class PersonnelManager : public QObject {
    Q_OBJECT

public:
    explicit PersonnelManager(QTableWidget *table, QObject *parent = nullptr);
    ~PersonnelManager();

    // Setters for personnel properties
    void setIdPer(int id) { this->idPer = id; }
    void setNom(const QString &nom) { this->nom = nom; }
    void setPrenom(const QString &prenom) { this->prenom = prenom; }
    void setSexe(const QString &sexe) { this->sexe = sexe; }
    void setCin(const QString &cin) { this->cin = cin; }
    void setNumTel(const QString &numTel) { this->numTel = numTel; }
    void setEmail(const QString &email) { this->email = email; }
    void setAdresse(const QString &adresse) { this->adresse = adresse; }
    void setDateAjout(const QDate &dateAjout) { this->dateAjout = dateAjout; }
    void setRole(const QString &role) { this->role = role; }
    void setEmailG(const QString &emailG) { this->emailG = emailG; }
    void setMotDePasseG(const QString &motDePasseG) { this->motDePasseG = motDePasseG; }
    void refreshTable();

    // Getters for personnel properties
    QMap<QString, QVariant> getPersonnelById(int id);
    QMap<QString, int> getPersonnelRoleStats();
    QStringList getRecentlyAddedPersonnel(int daysThreshold = 7);

    // Database operations
    int getNextId();
    void loadPersonnel();
    bool addPersonnel();
    bool editPersonnel();
    bool deletePersonnel();
    bool loadPersonnelSummary(QTableWidget *summaryTable);

signals:
    void editPersonnelRequested(int id);
    void deletePersonnelRequested(int id);
    void showPersonnelRoleStats();

private:
    bool validatePersonnelData() const;
    void setupTable();
    QTableWidget *personnelTable;
    connection dbConnection;

    int idPer = 0;
    QString nom;
    QString prenom;
    QString sexe;
    QString cin;
    QString numTel;
    QString email;
    QString adresse;
    QDate dateAjout;
    QString role;
    QString emailG;
    QString motDePasseG;
    QWidget *addPersonelForm;
};

#endif // PERSONNELMANAGER_H
