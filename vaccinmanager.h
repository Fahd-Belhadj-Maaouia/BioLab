#ifndef VACCINMANAGER_H
#define VACCINMANAGER_H
#include <QObject>
#include <QTableWidget>
#include <QSqlQuery>
#include <QDate>
#include <QMessageBox>
#include "connection.h"

class VaccinManager : public QObject {
    Q_OBJECT
public:
    explicit VaccinManager(QTableWidget *table, QObject *parent = nullptr);
    ~VaccinManager();

    // Setters for vaccin properties
    void setId(int id) { this->id = id; }
    void setNomVaccin(const QString &nom) { nomVaccin = nom; }
    void setReference(const QString &ref) { reference = ref; }
    void setType(const QString &t) { type = t; }
    void setMaladieChronique(const QString &maladie) { maladieChronique = maladie; }
    void setNbDose(int doses) { nbDose = doses; }
    void setQuantite(int qty) { quantite = qty; }
    void setDateExp(const QDate &date) { dateExp = date; }
    QMap<QString, QVariant> getVaccinById(int id);
    QMap<QString, int> getVaccinTypeStats();
    QStringList getSoonToExpireVaccins(int daysThreshold = 7);






    // Database operations
    int getNextId();
    void loadVaccins();
    bool addVaccin();
    bool editVaccin();
    bool deleteVaccin();
    bool loadVaccinSummary(QTableWidget *summaryTable);
signals:
    void editVaccinRequested(int id);
    void deleteVaccinRequested(int id);
    void showVaccinTypeStats();


private:
    // Ajoutez cette méthode de validation
    bool validateVaccinData() const;  // Déclaration ajoutée ici

    void setupTable();
    QTableWidget *vaccinTable;
    DatabaseConnection dbConnection;

    // Vaccin properties
    int id = 0;
    QString nomVaccin;
    QString reference;
    QString type;
    QString maladieChronique;
    int nbDose = 0;
    int quantite = 0;
    QDate dateExp;
};
#endif // VACCINMANAGER_H
