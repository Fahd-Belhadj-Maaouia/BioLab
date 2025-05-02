#include "patients.h"
#include <QSqlQuery>
#include <QHeaderView>
#include <QSqlError>
#include <QDebug>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>


PatientsManager::PatientsManager(QTableWidget *patientstable, QWidget *parent)
    : QObject(parent), patientstable(patientstable) {
    patientstable->setColumnCount(9);
    patientstable->setHorizontalHeaderLabels({
        "IDP", "Nom", "Prenom", "Sexe", "CIN", "Adresse", "NumTel", "Maladie Chronique", "Historique"
    });

    patientstable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    background-color: #F5F5F7 !important;"
        "    color: black !important;"
        "    padding: 8px !important;"
        "    border-bottom: 1px solid #ddd !important;"
        "}"
        );

    patientstable->setStyleSheet(
        "QTableWidget {"
        "    background-color: #FFFFFF;"
        "    border: 1px solid #ddd;"
        "    border-radius: 8px;"
        "}"
        "QTableWidget::item {"
        "    border-bottom: 1px solid #ddd;"
        "    padding: 8px;"
        "    color: black;"
        "}"
        "QTableWidget::item:selected {"
        "    background: #EDEDED;"
        "}"
        );

    patientstable->setShowGrid(false);
    patientstable->verticalHeader()->setVisible(false);
    patientstable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    patientstable->horizontalHeader()->setVisible(true);  // Force header visibility
    patientstable->horizontalHeader()->setHighlightSections(false);  // Prevent selection highlight
    patientstable->horizontalHeader()->setMinimumHeight(30);  // Force minimum header height
    patientstable->verticalHeader()->setDefaultSectionSize(40);
    patientstable->setWordWrap(false);
    patientstable->setRowCount(5);  // Add some rows for testing





    QSqlQuery query;

    int row = 0;

    while (query.next()) {
        // Set the number of rows in the table to accommodate the data
        patientstable->insertRow(row);

        // Fill in each column of the row with data from the query
        patientstable->setItem(row, 0, new QTableWidgetItem(query.value("IDP").toString()));
        patientstable->setItem(row, 1, new QTableWidgetItem(query.value("Nom").toString()));
        patientstable->setItem(row, 2, new QTableWidgetItem(query.value("Prenom").toString()));
        patientstable->setItem(row, 3, new QTableWidgetItem(query.value("Sexe").toString()));
        patientstable->setItem(row, 4, new QTableWidgetItem(query.value("CIN").toString()));
        patientstable->setItem(row, 5, new QTableWidgetItem(query.value("Adresse").toString()));
        patientstable->setItem(row, 6, new QTableWidgetItem(query.value("NumTel").toString()));
        patientstable->setItem(row, 7, new QTableWidgetItem(query.value("MaladieChronique").toString()));

        // Move to the next row
        row++;
    }



    // Load tools from the database
    loadPatients();


    // Create the form widget
    addPatientForm = new QWidget();
    QFormLayout *formLayout = new QFormLayout(addPatientForm);



    // Add input fields
    NomInput = new QLineEdit();
    prenomInput = new QLineEdit();
    sexeInput = new QLineEdit();
    cinInput = new QLineEdit();
    adresseInput = new QLineEdit();
    numtelInput = new QLineEdit();
    maladiechroniqueInput = new QLineEdit();


    // Add fields to the form
    formLayout->addRow("Nom:", NomInput);
    formLayout->addRow("prenom:", prenomInput);
    formLayout->addRow("sexe:", sexeInput);
    formLayout->addRow("cin:", cinInput);
    formLayout->addRow("adresse:", adresseInput);
    formLayout->addRow("numtel:", numtelInput);
    formLayout->addRow("maladiechronique:", maladiechroniqueInput);




    // Style the form
    addPatientForm->setStyleSheet(
        "QLineEdit, QSpinBox {"
        "    padding: 8px;"
        "    border: 1px solid #ddd;"
        "    border-radius: 4px;"
        "}"
        "QPushButton {"
        "    background-color: #198754;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #157347;"
        "}"
        );
}
PatientsManager::~PatientsManager() {
        // Destructor implementation (if needed)
    }

    // Getters
    int PatientsManager::getIDP() const {
        return IDP;
    }

    QString PatientsManager::getNom() const {
        return Nom;
    }

    QString PatientsManager::getprenom() const {
        return prenom;
    }

    QString PatientsManager::getsexe() const {
        return sexe;
    }
    QString PatientsManager::getcin() const {
        return cin;
    }
    QString PatientsManager::getadresse() const {
        return adresse;
    }
    QString PatientsManager::getnumtel() const {
        return numtel;
    }
    QString PatientsManager::getmaladiechronique() const {
        return maladiechronique;
    }


    // Setters
    void PatientsManager::setIDP(int IDP) {
        this->IDP = IDP;
    }

    void PatientsManager::setNom(const QString &Nom) {
        this->Nom = Nom;
    }

    void PatientsManager::setprenom(const QString &prenom) {
        this->prenom = prenom;
    }

    void PatientsManager::setsexe(const QString &sexe) {
        this->sexe = sexe;
    }

    void PatientsManager::setcin(const QString &cin) {
        this->cin = cin;
    }

    void PatientsManager::setadresse(const QString &adresse) {
        this->adresse = adresse;
    }

    void PatientsManager::setnumtel(const QString &numtel) {
        this->numtel = numtel;
    }

    void PatientsManager::setmaladiechronique(const QString &maladiechronique) {
        this->maladiechronique = maladiechronique;
    }


    QList<QStringList> PatientsManager::searchPatients(const QString &field, const QString &value) {
        QList<QStringList> results;
        QSqlQuery query;

        QString sql = QString("SELECT IDP, Nom, Prenom, Sexe, CIN, Adresse, NumTel, MaladieChronique FROM patients WHERE %1 LIKE :value").arg(field);
        query.prepare(sql);
        query.bindValue(":value", "%" + value + "%");

        if (query.exec()) {
            while (query.next()) {
                QStringList row;
                for (int i = 0; i < 8; i++) {
                    row << query.value(i).toString();
                }
                results.append(row);
            }
        } else {
            qDebug() << "Search error:" << query.lastError().text();
        }

        return results;
    }

    QList<QStringList> PatientsManager::sortPatients(const QString &field, bool ascending) {
        QList<QStringList> results;
        QSqlQuery query;

        QString order = ascending ? "ASC" : "DESC";
        QString sql = QString("SELECT IDP, Nom, Prenom, Sexe, CIN, Adresse, NumTel, MaladieChronique FROM patients ORDER BY %1 %2").arg(field, order);

        if (query.exec(sql)) {
            while (query.next()) {
                QStringList row;
                for (int i = 0; i < 8; i++) {
                    row << query.value(i).toString();
                }
                results.append(row);
            }
        } else {
            qDebug() << "Sort error:" << query.lastError().text();
        }

        return results;
    }




    QMap<QString, int> PatientsManager::getGenderDistribution() {
        QMap<QString, int> distribution;
        QSqlQuery query("SELECT Sexe, COUNT(*) FROM patients GROUP BY Sexe");

        while (query.next()) {
            distribution[query.value(0).toString()] = query.value(1).toInt();
        }

        // Ensure we have entries for both genders even if count is 0
        if (!distribution.contains("Homme")) distribution["Homme"] = 0;
        if (!distribution.contains("Femme")) distribution["Femme"] = 0;

        return distribution;
    }

    QMap<QString, int> PatientsManager::getChronicDiseaseDistribution() {
        QMap<QString, int> distribution;
        QSqlQuery query("SELECT "
                        "CASE WHEN MaladieChronique IS NULL OR MaladieChronique = '' THEN 'Sans' ELSE 'Avec' END, "
                        "COUNT(*) FROM patients GROUP BY "
                        "CASE WHEN MaladieChronique IS NULL OR MaladieChronique = '' THEN 'Sans' ELSE 'Avec' END");

        while (query.next()) {
            distribution[query.value(0).toString()] = query.value(1).toInt();
        }

        // Ensure we have both entries even if count is 0
        if (!distribution.contains("Avec")) distribution["Avec"] = 0;
        if (!distribution.contains("Sans")) distribution["Sans"] = 0;

        return distribution;
    }

    QMap<QString, int> PatientsManager::getCityDistribution() {
        QMap<QString, int> distribution;
        QSqlQuery query("SELECT Adresse, COUNT(*) FROM patients GROUP BY Adresse ORDER BY COUNT(*) DESC");

        while (query.next()) {
            distribution[query.value(0).toString()] = query.value(1).toInt();
        }

        return distribution;
    }






    void PatientsManager::addPatient(const QString &Nom, const QString &prenom, const QString &sexe,
                                     const QString &cin, const QString &adresse, const QString &numtel,
                                     const QString &maladiechronique) {
        if (Nom.trimmed().isEmpty() || prenom.trimmed().isEmpty() || sexe.trimmed().isEmpty() ||
            cin.trimmed().isEmpty() || adresse.trimmed().isEmpty() || numtel.trimmed().isEmpty()) {
            QMessageBox::warning(nullptr, "Validation Error", "Please fill all required fields.");
            return;
        }

        QSqlQuery query;
        query.prepare("INSERT INTO patients (Nom, Prenom, Sexe, CIN, Adresse, NumTel, MaladieChronique) "
                      "VALUES (:Nom, :Prenom, :Sexe, :CIN, :Adresse, :NumTel, :MaladieChronique)");
        query.bindValue(":Nom", Nom);
        query.bindValue(":Prenom", prenom);
        query.bindValue(":Sexe", sexe);
        query.bindValue(":CIN", cin);
        query.bindValue(":Adresse", adresse);
        query.bindValue(":NumTel", numtel);
        query.bindValue(":MaladieChronique", maladiechronique);

        if (!query.exec()) {
            qDebug() << "Error adding patient:" << query.lastError().text();
        } else {
            loadPatients();
            emit dataChanged();
        }
    }




    void PatientsManager::loadPatients() {
        qDebug() << "Loading patients from database...";

        patientstable->setRowCount(0);  // ✅ Clear table before reloading

        QSqlQuery query("SELECT IDP, Nom, Prenom, Sexe, CIN, Adresse, NumTel, MaladieChronique FROM patients ORDER BY IDP DESC");

        int row = 0;
        while (query.next()) {
            patientstable->insertRow(row);
            for (int col = 0; col < 8; col++) {
                patientstable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
            row++;
        }

        qDebug() << "Patients loaded successfully!";
        emit dataChanged();
    }


    void PatientsManager::updatePatient(int id) {
        // Retrieve the new data from the input fields
        QString nom = NomInput->text();
        QString prenom = prenomInput->text();
        QString sexe = sexeInput->text();
        QString cin = cinInput->text();
        QString adresse = adresseInput->text();
        QString numTel = numtelInput->text();
        QString maladieChronique = maladiechroniqueInput->text();

        QSqlQuery query;
        query.prepare("UPDATE patients SET Nom=:Nom, Prenom=:Prenom, Sexe=:Sexe, CIN=:CIN, Adresse=:Adresse, NumTel=:NumTel, MaladieChronique=:MaladieChronique WHERE IDP=:IDP");
        query.bindValue(":Nom", nom);
        query.bindValue(":Prenom", prenom);
        query.bindValue(":Sexe", sexe);
        query.bindValue(":CIN", cin);
        query.bindValue(":Adresse", adresse);
        query.bindValue(":NumTel", numTel);
        query.bindValue(":MaladieChronique", maladieChronique);
        query.bindValue(":IDP", id);

        if (!query.exec()) {
            qDebug() << "Error updating patient:" << query.lastError().text();
        } else {
            loadPatients();
            emit dataChanged();            // Reload the patients after updating
        }
    }


    void PatientsManager::deletePatient(int id) {
        QSqlQuery query;
        query.prepare("DELETE FROM patients WHERE IDP=:IDP");
        query.bindValue(":IDP", id);
        if (!query.exec()) {
            qDebug() << "Error deleting patient:" << query.lastError().text();
        } else {
            loadPatients();
            emit dataChanged();

        }
    }




    QList<QStringList> PatientsManager::getAllPatientsForExport() {
        QList<QStringList> patients;

        QSqlQuery query("SELECT IDP, Nom, Prenom, Sexe, CIN, Adresse, NumTel, MaladieChronique FROM patients ORDER BY Nom, Prenom");

        if (!query.exec()) {
            qDebug() << "Error fetching patients for export:" << query.lastError().text();
            return patients;
        }

        while (query.next()) {
            QStringList patient;
            patient << query.value("IDP").toString()
                    << query.value("Nom").toString()
                    << query.value("Prenom").toString()
                    << query.value("Sexe").toString()
                    << query.value("CIN").toString()
                    << query.value("Adresse").toString()
                    << query.value("NumTel").toString()
                    << query.value("MaladieChronique").toString();
            patients.append(patient);
        }

        return patients;
    }


