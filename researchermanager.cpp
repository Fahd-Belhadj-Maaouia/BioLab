#include "researchermanager.h"
#include "connection.h"
#include <QSqlQuery>
#include <QHeaderView>
#include <QSqlError>
#include <QDebug>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>

#include <QRandomGenerator>
#include <QProcess>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

//#include <opencv2/opencv.hpp>



QString generateEmail(const QString &nom, const QString &prenom) {
    return nom.toLower() + "." + prenom.toLower() + "@biolab.com";
}

QString generatePassword(const QString &cin) {
    QString base = cin.left(4); // 4 premiers chiffres
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString randomPart;
    for (int i = 0; i < 4; ++i)
        randomPart += chars[QRandomGenerator::global()->bounded(chars.length())];
    return base + randomPart;
}




ResearcherManager::ResearcherManager(QTableWidget *researchertable, QWidget *parent)
    : QObject(parent), researchertable(researchertable) {
    researchertable->setColumnCount(10);
    researchertable->setHorizontalHeaderLabels({ "IDR", "Nom", "Prenom", "Sexe", "CIN", "Adresse", "NumTel", "Specialite" , "Datedenaissance" ,"Email"});
    researchertable->horizontalHeader()->setStyleSheet(
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

    researchertable->setStyleSheet(
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

    researchertable->setShowGrid(false);
    researchertable->verticalHeader()->setVisible(false);
    researchertable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    researchertable->horizontalHeader()->setVisible(true);
    researchertable->verticalHeader()->setDefaultSectionSize(40);
    researchertable->setWordWrap(false);
    researchertable->setRowCount(5);  // Add some rows for testing





    QSqlQuery query;

    int row = 0;

    while (query.next()) {
        // Set the number of rows in the table to accommodate the data
        researchertable->insertRow(row);

        // Fill in each column of the row with data from the query
        researchertable->setItem(row, 0, new QTableWidgetItem(query.value("IDP").toString()));
        researchertable->setItem(row, 1, new QTableWidgetItem(query.value("Nom").toString()));
        researchertable->setItem(row, 2, new QTableWidgetItem(query.value("Prenom").toString()));
        researchertable->setItem(row, 3, new QTableWidgetItem(query.value("Sexe").toString()));
        researchertable->setItem(row, 4, new QTableWidgetItem(query.value("CIN").toString()));
        researchertable->setItem(row, 5, new QTableWidgetItem(query.value("Adresse").toString()));
        researchertable->setItem(row, 6, new QTableWidgetItem(query.value("NumTel").toString()));
        researchertable->setItem(row, 7, new QTableWidgetItem(query.value("Specialite").toString()));
        researchertable->setItem(row, 8, new QTableWidgetItem(query.value("Datedenaissance").toString()));
        researchertable->setItem(row, 9, new QTableWidgetItem(query.value("Email").toString()));



        // Move to the next row
        row++;
    }



    // Load tools from the database
    loadResearcher();


    // Create the form widget
    addResearcherForm = new QWidget();
    QFormLayout *formLayout = new QFormLayout(addResearcherForm);



    // Add input fields
    NomInput = new QLineEdit();
    prenomInput = new QLineEdit();
    sexeInput = new QLineEdit();
    cinInput = new QLineEdit();
    adresseInput = new QLineEdit();
    numtelInput = new QLineEdit();
    specialiteInput = new QLineEdit();
    datedenaissanceInput = new QLineEdit();
    emailInput = new QLineEdit();




    // Add fields to the form
    formLayout->addRow("Nom:", NomInput);
    formLayout->addRow("prenom:", prenomInput);
    formLayout->addRow("sexe:", sexeInput);
    formLayout->addRow("cin:", cinInput);
    formLayout->addRow("adresse:", adresseInput);
    formLayout->addRow("numtel:", numtelInput);
    formLayout->addRow("specialite:", specialiteInput);
    formLayout->addRow("datedenaissance:", datedenaissanceInput);
    formLayout->addRow("email:", emailInput);






    // Style the form
    addResearcherForm->setStyleSheet(
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

ResearcherManager::~ResearcherManager() {
    // Destructor implementation (if needed)
}

// Getters
int ResearcherManager::getIDR() const {
    return IDR;
}

QString ResearcherManager::getNom() const {
    return Nom;
}

QString ResearcherManager::getprenom() const {
    return prenom;
}

QString ResearcherManager::getsexe() const {
    return sexe;
}
QString ResearcherManager::getcin() const {
    return cin;
}
QString ResearcherManager::getadresse() const {
    return adresse;
}
QString ResearcherManager::getnumtel() const {
    return numtel;
}
QString ResearcherManager::getspecialite() const {
    return specialite;
}
QString ResearcherManager::getdatedenaissance() const {
    return datedenaissance;
}
QString ResearcherManager::getemail() const {
    return email;
}


// Setters
void ResearcherManager::setIDR(int IDR) {
    this->IDR = IDR;
}

void ResearcherManager::setNom(const QString &Nom) {
    this->Nom = Nom;
}

void ResearcherManager::setprenom(const QString &prenom) {
    this->prenom = prenom;
}

void ResearcherManager::setsexe(const QString &sexe) {
    this->sexe = sexe;
}

void ResearcherManager::setcin(const QString &cin) {
    this->cin = cin;
}

void ResearcherManager::setadresse(const QString &adresse) {
    this->adresse = adresse;
}

void ResearcherManager::setnumtel(const QString &numtel) {
    this->numtel = numtel;
}

void ResearcherManager::setspecialite(const QString &specialite) {
    this->specialite = specialite;
}
void ResearcherManager::setdatedenaissance(const QString &datedenaissance) {
    this->datedenaissance = datedenaissance;
}
void ResearcherManager::setemail(const QString &email) {
    this->email = email;
}



void ResearcherManager::addResearcher(const QString &Nom, const QString &prenom, const QString &sexe,
                                      const QString &cin, const QString &adresse, const QString &numtel,
                                      const QString &specialite, const QString &datedenaissance, const QString &email) {
    // First check if CIN already exists
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM researcher WHERE CIN = :CIN");
    checkQuery.bindValue(":CIN", cin);

    if (!checkQuery.exec() || !checkQuery.next()) {
        QMessageBox::warning(nullptr, "Database Error", "Failed to check researcher existence.");
        return;
    }

    if (checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(nullptr, "Duplicate Entry", "A researcher with this CIN already exists.");
        return;
    }

    // Rest of your existing add code...
    QDate date = QDate::fromString(datedenaissance, "yyyy-MM-dd");
    if (!date.isValid()) {
        QMessageBox::warning(nullptr, "Invalid Date", "Date must be in format YYYY-MM-DD.");
        return;
    }

    QString formattedDate = date.toString("dd/MM/yyyy");

    QString emailg = generateEmail(Nom, prenom);
    QString motdepasseg = generatePassword(cin);


    QSqlQuery query;
    query.prepare(R"(
    INSERT INTO researcher
    (Nom, Prenom, Sexe, CIN, Adresse, NumTel, Specialite, Datedenaissance, Email, emailg, motdepasseg)
    VALUES (:Nom, :Prenom, :Sexe, :CIN, :Adresse, :NumTel, :Specialite, TO_DATE(:Datedenaissance, 'DD/MM/YYYY'), :Email, :emailg, :motdepasseg)
)");

    query.bindValue(":Nom", Nom);
    query.bindValue(":Prenom", prenom);
    query.bindValue(":Sexe", sexe);
    query.bindValue(":CIN", cin);
    query.bindValue(":Adresse", adresse);
    query.bindValue(":NumTel", numtel);
    query.bindValue(":Specialite", specialite);
    query.bindValue(":Datedenaissance", formattedDate);
    query.bindValue(":Email", email);
    query.bindValue(":emailg", emailg);
    query.bindValue(":motdepasseg", motdepasseg);


    if (!query.exec()) {
        qDebug() << "❌ Error adding researcher:" << query.lastError().text();
        QMessageBox::warning(nullptr, "Database Error",
                             QString("Failed to add researcher:\n%1").arg(query.lastError().text()));
    } else {
        qDebug() << "✅ Researcher added successfully.";
        loadResearcher();
        // ✅ Envoi de l'e-mail d'accès ici
        QString emailBody = "Bonjour " + prenom + " " + Nom + ",\n\n"
                                                              "Bienvenue chez BIOLAB.\n\n"
                                                              "Voici vos identifiants :\n"
                                                              "Email : " + emailg + "\n"
                                       "Mot de passe : " + motdepasseg + "\n\n"
                                            "Merci.";

        sendEmailViaSendGrid(email, "Bienvenue chez BIOLAB", emailBody);
    }
}


void ResearcherManager::loadResearcher() {
    researchertable->setRowCount(0);
    QSqlQuery query("SELECT IDR, Nom, Prenom, Sexe, CIN, Adresse, NumTel, Specialite, Datedenaissance, Email FROM researcher ORDER BY IDR DESC");
    int row = 0;
    while (query.next()) {
        researchertable->insertRow(row);
        for (int col = 0; col < 10; col++) {
            researchertable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}

void ResearcherManager::updateResearcher(int id, const QString &nom, const QString &prenom, const QString &sexe,
                                         const QString &cin, const QString &adresse, const QString &numTel,
                                         const QString &specialite, const QString &datedenaissance, const QString &email)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Database not open!";
        return;
    }

    // Start transaction
    db.transaction();

    try {
        // Verify researcher exists
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT 1 FROM researcher WHERE IDR = ?");
        checkQuery.addBindValue(id);
        if (!checkQuery.exec() || !checkQuery.next()) {
            throw std::runtime_error("Researcher check failed: " + checkQuery.lastError().text().toStdString());
        }

        // Prepare update query with correct column names
        QSqlQuery query;
        query.prepare("UPDATE researcher SET "
                      "Nom = ?, "
                      "Prenom = ?, "
                      "Sexe = ?, "
                      "CIN = ?, "
                      "Adresse = ?, "
                      "NumTel = ?, "
                      "Specialite = ?, "
                      "Datedenaissance = TO_DATE(?, 'DD/MM/YYYY'), "
                      "Email = ? "
                      "WHERE IDR = ?");

        // ✅ Format date properly to DD/MM/YYYY for Oracle
        QString formattedDate = datedenaissance;
        QDate parsedDate = QDate::fromString(datedenaissance, Qt::ISODate); // Handles YYYY-MM-DD or "1995-06-30"
        if (parsedDate.isValid()) {
            formattedDate = parsedDate.toString("dd/MM/yyyy");
        }

        // Bind values using positional parameters
        query.addBindValue(nom);
        query.addBindValue(prenom);
        query.addBindValue(sexe);
        query.addBindValue(cin);
        query.addBindValue(adresse);
        query.addBindValue(numTel);
        query.addBindValue(specialite);
        query.addBindValue(formattedDate);  // 🔁 Now always in DD/MM/YYYY format
        query.addBindValue(email);
        query.addBindValue(id);

        if (!query.exec()) {
            throw std::runtime_error("Update failed: " + query.lastError().text().toStdString());
        }

        if (query.numRowsAffected() <= 0) {
            throw std::runtime_error("No rows affected - ID might not exist");
        }

        db.commit();
        qDebug() << "Successfully updated researcher" << id;
        loadResearcher();
    } catch (const std::exception &e) {
        db.rollback();
        qDebug() << "Error:" << e.what();
        QMessageBox::warning(nullptr, "Database Error", QString("Failed to update researcher:\n%1").arg(e.what()));
    }
}



void ResearcherManager::deleteResearcher(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM researcher WHERE IDR=:IDR");
    query.bindValue(":IDR", id);  // Changed from :IDP to :IDR
    if (!query.exec()) {
        qDebug() << "Error deleting researcher:" << query.lastError().text();
        QMessageBox::warning(nullptr, "Database Error",
                             QString("Failed to delete researcher:\n%1").arg(query.lastError().text()));
    } else {
        loadResearcher();
    }
}

void ResearcherManager::searchResearcherByName(const QString &name) {
    researchertable->setRowCount(0);
    QSqlQuery query;
    query.prepare("SELECT IDR, Nom, Prenom, Sexe, CIN, Adresse, NumTel, Specialite, Datedenaissance, Email FROM researcher WHERE Nom LIKE :name OR Prenom LIKE :name ORDER BY IDR DESC");
    query.bindValue(":name", "%" + name + "%");

    int row = 0;
    if (query.exec()) {
        while (query.next()) {
            researchertable->insertRow(row);
            for (int col = 0; col < 10; col++) {
                researchertable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
            row++;
        }
    } else {
        qDebug() << "Search error:" << query.lastError().text();
    }
}

void ResearcherManager::searchResearcherByCIN(const QString &cin) {
    researchertable->setRowCount(0);
    QSqlQuery query;
    query.prepare("SELECT IDR, Nom, Prenom, Sexe, CIN, Adresse, NumTel, Specialite, Datedenaissance, Email FROM researcher WHERE CIN LIKE :cin ORDER BY IDR DESC");
    query.bindValue(":cin", "%" + cin + "%");

    int row = 0;
    if (query.exec()) {
        while (query.next()) {
            researchertable->insertRow(row);
            for (int col = 0; col < 10; col++) {
                researchertable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
            row++;
        }
    } else {
        qDebug() << "Search by CIN error:" << query.lastError().text();
    }
}


void ResearcherManager::searchResearcherBySpecialite(const QString &specialite) {
    researchertable->setRowCount(0);
    QSqlQuery query;
    query.prepare("SELECT IDR, Nom, Prenom, Sexe, CIN, Adresse, NumTel, Specialite, Datedenaissance, Email FROM researcher WHERE Specialite LIKE :specialite ORDER BY IDR DESC");
    query.bindValue(":specialite", "%" + specialite + "%");

    int row = 0;
    if (query.exec()) {
        while (query.next()) {
            researchertable->insertRow(row);
            for (int col = 0; col < 10; col++) {
                researchertable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
            row++;
        }
    } else {
        qDebug() << "Search by Specialite error:" << query.lastError().text();
    }
}

void ResearcherManager::sortResearchersByName(bool ascending) {
    researchertable->setRowCount(0);
    QSqlQuery query;

    QString order = ascending ? "ASC" : "DESC";
    query.prepare(QString(R"(
        SELECT IDR, Nom, Prenom, Sexe, CIN, Adresse, NumTel, Specialite, Datedenaissance, Email
        FROM researcher
        ORDER BY Nom %1
    )").arg(order));

    int row = 0;
    if (query.exec()) {
        while (query.next()) {
            researchertable->insertRow(row);
            for (int col = 0; col < 10; ++col) {
                researchertable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
            row++;
        }
    } else {
        qDebug() << "Sorting error:" << query.lastError().text();
    }
}

void ResearcherManager::sortResearchersBySpecialite(bool ascending) {
    researchertable->setRowCount(0);
    QSqlQuery query;

    QString order = ascending ? "ASC" : "DESC";
    query.prepare(QString(R"(
        SELECT IDR, Nom, Prenom, Sexe, CIN, Adresse, NumTel, Specialite, Datedenaissance, Email
        FROM researcher
        ORDER BY Specialite %1
    )").arg(order));

    int row = 0;
    if (query.exec()) {
        while (query.next()) {
            researchertable->insertRow(row);
            for (int col = 0; col < 10; ++col) {
                researchertable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
            row++;
        }
    } else {
        qDebug() << "Sorting by Specialite error:" << query.lastError().text();
    }
}



void ResearcherManager::filterByAgeRange(int minAge, int maxAge) {
    researchertable->setRowCount(0);
    QSqlQuery query;
    query.prepare(R"(
        SELECT IDR, Nom, Prenom, Sexe, CIN, Adresse, NumTel, Specialite, Datedenaissance, Email
        FROM researcher
        WHERE FLOOR(MONTHS_BETWEEN(SYSDATE, Datedenaissance) / 12) BETWEEN :minAge AND :maxAge
        ORDER BY IDR DESC
    )");
    query.bindValue(":minAge", minAge);
    query.bindValue(":maxAge", maxAge);

    int row = 0;
    if (query.exec()) {
        while (query.next()) {
            researchertable->insertRow(row);
            for (int col = 0; col < 10; col++) {
                researchertable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
            row++;
        }
    } else {
        qDebug() << "Age filter error:" << query.lastError().text();
    }
}

void ResearcherManager::sendEmailViaSendGrid(const QString &to, const QString &subject, const QString &bodyText) {
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QUrl url("https://api.sendgrid.com/v3/mail/send");
    QNetworkRequest request(url);

    // 🔐 Your actual API key here
    QString apiKey = "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";
    request.setRawHeader("Authorization", "Bearer " + apiKey.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject emailJson;
    emailJson["from"] = QJsonObject{{"email", "bio.lab.bio.lab01@gmail.com"}, {"name", "BIOLAB"}};
    emailJson["subject"] = subject;

    QJsonArray toList;
    toList.append(QJsonObject{{"email", to}});

    QJsonObject personalization;
    personalization["to"] = toList;

    emailJson["personalizations"] = QJsonArray{personalization};

    QJsonObject content;
    content["type"] = "text/plain";
    content["value"] = bodyText;

    emailJson["content"] = QJsonArray{content};

    QJsonDocument jsonDoc(emailJson);

    QNetworkReply *reply = manager->post(request, jsonDoc.toJson());

    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "✅ Email sent via SendGrid!";
        } else {
            qDebug() << "❌ SendGrid error:" << reply->errorString();
            qDebug() << "↪ Reply body:" << reply->readAll();
        }
        reply->deleteLater();
    });
}


