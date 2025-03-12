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
    patientstable->setColumnCount(8);
    patientstable->setHorizontalHeaderLabels({ "IDP", "Nom", "Prenom", "Sexe", "CIN", "Adresse", "NumTel", "Maladie Chronique"});
    patientstable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    background-color: #F5F5F7;"
        "    color: black;"
        "    font-weight: bold;"
        "    font-size: 12px;"
        "    padding: 8px;"
        "    border: none;"
        "    border-bottom: 1px solid #ddd;"
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
    patientstable->horizontalHeader()->setVisible(true);
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
        }
    }


    void PatientsManager::loadPatients() {
        patientstable->setRowCount(0);
        QSqlQuery query("SELECT IDP, Nom, Prenom, Sexe, CIN, Adresse, NumTel, MaladieChronique FROM patients ORDER BY IDP DESC");
        int row = 0;
        while (query.next()) {
            patientstable->insertRow(row);
            for (int col = 0; col < 8; col++) {
                patientstable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
            row++;
        }
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
            loadPatients();  // Reload the patients after updating
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
        }
    }


