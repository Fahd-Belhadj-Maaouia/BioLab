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
#include <QComboBox>
#include <QChartView>
#include <QChart>
#include <QPieSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QSqlRecord>
#include <QValueAxis>
#include <QCryptographicHash>
#include <QBuffer>
#include <QPixmap>
#include "connectionn.h"
#include "QPrinter"

class PersonnelManager : public QObject
{
    Q_OBJECT

public:
    explicit PersonnelManager(QTableWidget *table, QObject *parent = nullptr);
    ~PersonnelManager();

    // Setters
    void setIdPer(int id) { idPer = id; }
    void setNom(const QString &nom) { this->nom = nom.trimmed(); }
    void setPrenom(const QString &prenom) { this->prenom = prenom.trimmed(); }
    void setSexe(const QString &sexe) { this->sexe = sexe.trimmed(); }
    void setCin(const QString &cin) { this->cin = cin.trimmed(); }
    void setNumTel(const QString &numTel) { this->numTel = numTel.trimmed(); }
    void setEmail(const QString &email) { this->email = email.trimmed(); }
    void setAdresse(const QString &adresse) { this->adresse = adresse.trimmed(); }
    void setRole(const QString &role) { this->role = role.trimmed(); }
    void setDateAjout(const QDate &dateAjout) { this->dateAjout = dateAjout; }
    void setPhotoBase64(const QString &photoBase64) { this->photoBase64 = photoBase64; }
    void setEmailG(const QString &emailG) { this->emailG = emailG; }
    void setMotDePasseG(const QString &motDePasseG) { this->motDePasseG = motDePasseG; }

    // Getters
    QTableWidget* getTable() const { return personnelTable; }
    connectionn& getConnectionn() { return dbConnectionn; }
    int getIdPer() const { return idPer; }
    QString getNom() const { return nom; }
    QString getPrenom() const { return prenom; }
    QString getSexe() const { return sexe; }
    QString getCin() const { return cin; }
    QString getNumTel() const { return numTel; }
    QString getEmail() const { return email; }
    QString getAdresse() const { return adresse; }
    QDate getDateAjout() const { return dateAjout; }
    QString getRole() const { return role; }
    QString getEmailG() const { return emailG; }
    QString getMotDePasseG() const { return motDePasseG; }
    QString getPhotoBase64() const { return photoBase64; }
    void setQuestionIndex(int index);
    void setReponseSecrete(const QString &reponse);
    int getQuestionIndex() const;
    QString getReponseSecrete() const;

    // Fonctions de gestion des données
    QChart* createVillePieChart();
    QChart* createGenderBarChart();
    QPair<int, int> getRoleCounts();
    int getTotalPersonnel();
    QStringList getVillesTunisie();
    int getLastInsertedId() const;
    int getNextId();
    QMap<QString, QVariant> getPersonnelById(int id);

    // Fonctions d'affichage
    void loadPersonnel(const QString& sortColumn = "DATEAJOUT", const QString& sortOrder = "DESC");
    void filterByVille(const QString& ville);
    void searchPersonnel(const QString &searchTerm);
    void refreshTable();
    bool exportToPDF(const QString &filePath);

    // Fonctions CRUD
    bool addPersonnel();
    bool editPersonnel();
    bool deletePersonnel();

    // Utilitaires
    static QByteArray imageToBase64(const QPixmap &pixmap);
    static QPixmap base64ToImage(const QString &base64);


signals:
    void personnelDataChanged();
    void statsUpdated();

private:
    bool validatePersonnelData() const;
    void setupTable();
    QString generateEmail();
    QString generatePassword();


    // Membres
    QTableWidget *personnelTable;
    connectionn dbConnectionn;

    // Données du personnel
    int idPer;
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
    QString photoBase64;
    int questionIndex;
    QString reponseSecrete;

};

#endif // PERSONNELMANAGER_H
