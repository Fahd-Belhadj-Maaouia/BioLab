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

    // Enumération pour les types de tri
    enum SortType {
        SORT_BY_NAME,
        SORT_BY_REFERENCE,
        SORT_BY_EXPIRATION
    };

    QTableWidget* getTableWidget() const {
        return vaccinTable;  // Ou le nom de votre variable membre pour le tableau
    }

    // Setters for vaccin properties
    void setId(int id) { this->id = id; }
    void setNomVaccin(const QString &nom) { nomVaccin = nom; }
    void setReference(const QString &ref) { reference = ref; }
    void setType(const QString &t) { type = t; }
    void setMaladieChronique(const QString &maladie) { maladieChronique = maladie; }
    void setNbDose(int doses) { nbDose = doses; }
    void setQuantite(int qty) { quantite = qty; }
    void setDateExp(const QDate &date) { dateExp = date; }
    void setCouleur(const QString &value) { couleur = value; }
    // Accesseurs pour les propriétés du vaccin
    int getId() const { return id; }
    QString getNomVaccin() const { return nomVaccin; }
    QString getReference() const { return reference; }
    QString getType() const { return type; }
    QString getMaladieChronique() const { return maladieChronique; }
    int getNbDose() const { return nbDose; }
    int getQuantite() const { return quantite; }
    QDate getDateExp() const { return dateExp; }
    QString getCouleur() const { return couleur; }





    bool isVaccinExpiredByColor(const QString &color);
    QStringList getExpiredVaccinsByColor(const QString &color);
    bool findVaccinByColor(const QString &color);

    QMap<QString, QVariant> getVaccinById(int id);
    QMap<QString, int> getVaccinTypeStats();
    QStringList getSoonToExpireVaccins(int daysThreshold = 7);

    // Database operations
    int getNextId();
    void loadVaccins(SortType sortType = SORT_BY_NAME, bool ascending = true);
    bool addVaccin();
    bool editVaccin();
    bool deleteVaccin();
    bool loadVaccinSummary(QTableWidget *summaryTable);
    bool searchVaccins(const QString& searchText, const QString& searchType,
                       SortType sortType = SORT_BY_NAME, bool ascending = true);
    int getTotalVaccinCount();

signals:
    void editVaccinRequested(int id);
    void deleteVaccinRequested(int id);
    void showVaccinTypeStats();
    void dataModified();  // Nouveau signal pour notifier les changements de données
    int getSelectedRow();
    int getSelectedVaccinId();

private:
    // Ajoutez cette méthode de validation
    bool validateVaccinData() const;
    void setupTable();
    QTableWidget *vaccinTable;
    DatabaseConnection dbConnection;

    // Current sorting settings
    SortType currentSortType = SORT_BY_NAME;
    bool sortAscending = true;

    // Vaccin properties
    int id = 0;
    QString nomVaccin;
    QString reference;
    QString type;
    QString maladieChronique;
    int nbDose = 0;
    int quantite = 0;
    QDate dateExp;
    QString couleur;  // Nouvelle propriété pour stocker la couleur

};

#endif // VACCINMANAGER_H
