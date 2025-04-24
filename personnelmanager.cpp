#include "personnelmanager.h"
#include <QHeaderView>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>
#include <QPieSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QSqlRecord>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QTextCursor>
#include <QFileDialog>
#include <QStandardPaths>
#include <QPageSize>

PersonnelManager::PersonnelManager(QTableWidget *table, QObject *parent)
    : QObject(parent), personnelTable(table) {
    setupTable();
    loadPersonnel();
}

PersonnelManager::~PersonnelManager() {}

void PersonnelManager::setupTable() {
    personnelTable->setColumnCount(10);
    personnelTable->setHorizontalHeaderLabels({
        "ID", "Nom", "Prénom", "Sexe", "CIN", "Téléphone",
        "Email", "Ville", "Rôle", "Date Ajout"
    });

    personnelTable->setStyleSheet(R"(
        QTableWidget {
            background-color: white;
            alternate-background-color: #f8f9fa;
            gridline-color: #dee2e6;
            border: 1px solid #ced4da;
            border-radius: 5px;
        }
        QTableWidget::item {
            padding: 5px;
            border: none;
            border-bottom: 1px solid #dee2e6;
        }
        QHeaderView::section {
            background-color: #198754;
            color: white;
            padding: 8px;
            border: none;
        }
    )");

    personnelTable->setShowGrid(false);
    personnelTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    personnelTable->horizontalHeader()->setHighlightSections(false);
    personnelTable->verticalHeader()->setVisible(false);
    personnelTable->setAlternatingRowColors(true);
    personnelTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    personnelTable->horizontalHeader()->setStretchLastSection(true);
}

QChart* PersonnelManager::createVillePieChart() {
    QPieSeries *series = new QPieSeries();
    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT ADRESSE, COUNT(*) FROM personnels GROUP BY ADRESSE");

    // Palette de couleurs cohérentes
    QVector<QColor> sliceColors = {
        QColor(25, 135, 84),   // Vert
        QColor(220, 53, 69),   // Rouge
        QColor(255, 193, 7),   // Jaune
        QColor(13, 110, 253),  // Bleu
        QColor(108, 117, 125), // Gris
        QColor(111, 66, 193),  // Violet
        QColor(253, 126, 20),  // Orange
        QColor(32, 201, 151),  // Turquoise
        QColor(214, 51, 132),  // Rose
        QColor(0, 123, 255)    // Bleu clair
    };

    if (query.exec()) {
        int colorIndex = 0;
        while (query.next()) {
            QPieSlice *slice = series->append(query.value(0).toString(), query.value(1).toInt());
            slice->setLabel(QString("%1\n%2").arg(query.value(0).toString()).arg(query.value(1).toInt()));
            slice->setLabelVisible(true);
            slice->setLabelPosition(QPieSlice::LabelOutside);

            // Appliquer une couleur de la palette en boucle
            slice->setColor(sliceColors[colorIndex % sliceColors.size()]);
            colorIndex++;
        }
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition par ville");
    chart->legend()->setVisible(false);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundBrush(QBrush(Qt::transparent));
    chart->setMargins(QMargins(0, 0, 0, 0));
    return chart;
}

QChart* PersonnelManager::createGenderBarChart() {
    QBarSeries *series = new QBarSeries();
    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT SEXE, COUNT(*) FROM personnels GROUP BY SEXE");

    QStringList categories;
    QBarSet *setHomme = new QBarSet("Homme");
    QBarSet *setFemme = new QBarSet("Femme");

    if (query.exec()) {
        while (query.next()) {
            if (query.value(0).toString() == "Homme") {
                *setHomme << query.value(1).toInt();
            } else {
                *setFemme << query.value(1).toInt();
            }
        }
    }

    setHomme->setColor(QColor(35, 107, 142));
    setFemme->setColor(QColor(214, 51, 132));
    series->append(setHomme);
    series->append(setFemme);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition par genre");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundBrush(QBrush(Qt::transparent));

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    return chart;
}

QPair<int, int> PersonnelManager::getRoleCounts() {
    QPair<int, int> counts(0, 0);
    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT ROLE, COUNT(*) FROM personnels GROUP BY ROLE");

    if (query.exec()) {
        while (query.next()) {
            if (query.value(0).toString() == "infirmier")
                counts.first = query.value(1).toInt();
            else
                counts.second = query.value(1).toInt();
        }
    }
    return counts;
}

int PersonnelManager::getTotalPersonnel() {
    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT COUNT(*) FROM personnels");
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

QStringList PersonnelManager::getVillesTunisie() {
    return {
        "Ariana", "Béja", "Ben Arous", "Bizerte", "Gabès", "Gafsa",
        "Jendouba", "Kairouan", "Kasserine", "Kébili", "Le Kef", "Mahdia",
        "La Manouba", "Médenine", "Monastir", "Nabeul", "Sfax", "Sidi Bouzid",
        "Siliana", "Sousse", "Tataouine", "Tozeur", "Tunis", "Zaghouan"
    };
}



void PersonnelManager::loadMiniTable(QTableWidget *table) {
    table->setRowCount(0);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Nom", "Prénom", "Ville", "Rôle"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT nom, prenom, adresse, role FROM personnels ORDER BY dateajout DESC LIMIT 5");

    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            table->insertRow(row);
            for (int col = 0; col < 4; col++) {
                QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                item->setTextAlignment(Qt::AlignCenter);
                table->setItem(row, col, item);
            }
            row++;
        }
    }
}
void PersonnelManager::loadPersonnel(const QString& sortColumn, const QString& sortOrder) {
    personnelTable->setRowCount(0);

    if (!dbConnection.createconnection()) {
        QMessageBox::critical(nullptr, "Erreur", "Connexion à la base de données échouée");
        return;
    }

    QSqlQuery query(dbConnection.getDatabase());
    QString queryStr = QString(
                           "SELECT IDPER, NOM, PRENOM, SEXE, CIN, NUMTEL, EMAIL, ADRESSE, ROLE, TO_CHAR(DATEAJOUT, 'DD/MM/YYYY') "
                           "FROM personnels ORDER BY %1 %2").arg(sortColumn).arg(sortOrder);

    if (query.exec(queryStr)) {
        int row = 0;
        while (query.next()) {
            personnelTable->insertRow(row);
            for (int col = 0; col < 10; ++col) {
                personnelTable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
            row++;
        }
    }
}

void PersonnelManager::filterByVille(const QString& ville) {
    personnelTable->setRowCount(0);

    if (!dbConnection.createconnection()) {
        QMessageBox::critical(nullptr, "Erreur", "Connexion à la base de données échouée");
        return;
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT IDPER, NOM, PRENOM, SEXE, CIN, NUMTEL, EMAIL, ADRESSE, ROLE, TO_CHAR(DATEAJOUT, 'DD/MM/YYYY') "
                  "FROM personnels WHERE ADRESSE = :ville ORDER BY DATEAJOUT DESC");
    query.bindValue(":ville", ville);

    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            personnelTable->insertRow(row);
            for (int col = 0; col < 10; ++col) {
                personnelTable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
            row++;
        }
    }
}

void PersonnelManager::searchPersonnel(const QString &searchTerm) {
    personnelTable->setRowCount(0);

    if (!dbConnection.createconnection()) {
        QMessageBox::critical(nullptr, "Erreur", "Connexion à la base de données échouée");
        return;
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT IDPER, NOM, PRENOM, SEXE, CIN, NUMTEL, EMAIL, ADRESSE, ROLE, TO_CHAR(DATEAJOUT, 'DD/MM/YYYY') "
                  "FROM personnels WHERE NOM LIKE :term OR PRENOM LIKE :term OR CIN LIKE :term "
                  "ORDER BY DATEAJOUT DESC");
    query.bindValue(":term", "%" + searchTerm + "%");

    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            personnelTable->insertRow(row);
            for (int col = 0; col < 10; ++col) {
                personnelTable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
            row++;
        }
    }
}

void PersonnelManager::refreshTable() {
    loadPersonnel();
    emit statsUpdated();
}

QMap<QString, QVariant> PersonnelManager::getPersonnelById(int id) {
    QMap<QString, QVariant> data;

    if (!dbConnection.createconnection()) {
        QMessageBox::critical(nullptr, "Erreur", "Connexion à la base de données échouée");
        return data;
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT * FROM personnels WHERE IDPER = :IDPER");
    query.bindValue(":IDPER", id);

    if (query.exec() && query.next()) {
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); ++i) {
            data[record.fieldName(i)] = query.value(i);
        }
    } else {
        qDebug() << "Erreur lors de la récupération:" << query.lastError().text();
    }

    return data;
}

QString PersonnelManager::generateEmail() {
    QString baseEmail = prenom.toLower() + "." + role.toLower();
    QString domain = "@blcentre.tn";
    QString finalEmail = baseEmail + domain;

    QSqlDatabase db = dbConnection.getDatabase();
    QSqlQuery query(db);

    int suffix = 1;
    while (true) {
        query.prepare("SELECT COUNT(*) FROM PERSONNELS WHERE EMAILG = :email");
        query.bindValue(":email", finalEmail);
        if (!query.exec()) {
            qDebug() << "Erreur lors de la vérification d'email:" << query.lastError().text();
            break; // on sort en cas d’erreur
        }

        if (query.next() && query.value(0).toInt() == 0) {
            break; // email unique trouvé
        }

        finalEmail = baseEmail + QString::number(suffix) + domain;
        suffix++;
    }

    return finalEmail;
}

bool PersonnelManager::exportToPDF(const QString &filePath) {
    QString finalFilePath = filePath;
    if (!finalFilePath.endsWith(".pdf", Qt::CaseInsensitive)) {
        finalFilePath += ".pdf";
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Portrait);
    printer.setOutputFileName(finalFilePath);

    QString html = R"(
    <html>
    <head>
    <meta charset='utf-8'>
    <style>
        body {
            font-family: 'Arial', sans-serif;
            font-size: 12pt;
            margin: 0;
            padding: 0;
        }
        .container {
            width: 95%;
            margin: 0 auto;
        }
        .header {
            color: #198754;
            font-size: 20pt;
            font-weight: bold;
            text-align: center;
            margin-bottom: 15px;
        }
        .subheader {
            color: #6c757d;
            font-size: 12pt;
            text-align: center;
            margin-bottom: 20px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            font-size: 11pt;
        }
        th {
        background-color: #198754;
        color: white;
        font-weight: bold;

        }
        th,td{
        padding: 8px 12px;
        border: 1px solid #ccc;
        text-align: left;
        min-width: 80px; /* 👈 largeur minimale de chaque colonne */
        word-wrap: break-word;
        }
        .footer {
            margin-top: 20px;
            font-size: 9pt;
            text-align: right;
            color: #666;
        }
    </style>
    </head>
    <body>
    <div class='container'>
        <div class='header'>LISTE DU PERSONNEL - BIOLAB</div>
        <div class='subheader'>Généré le )" + QDate::currentDate().toString("dd/MM/yyyy") + R"(</div>
        <table>
            <thead>
                <tr>)";

    for (int col = 0; col < personnelTable->columnCount(); ++col) {
        html += "<th>" + personnelTable->horizontalHeaderItem(col)->text().toHtmlEscaped() + "</th>";
    }
    html += "</tr></thead><tbody>";

    for (int row = 0; row < personnelTable->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < personnelTable->columnCount(); ++col) {
            QTableWidgetItem* item = personnelTable->item(row, col);
            html += "<td>" + (item ? item->text().toHtmlEscaped() : "&nbsp;") + "</td>";
        }
        html += "</tr>";
    }

    html += R"(</tbody>
        </table>
        <div class='footer'>Système de Gestion BIOLAB - © )" + QDate::currentDate().toString("yyyy") + R"(</div>
    </div>
    </body>
    </html>
    )";

    QTextDocument doc;
    doc.setHtml(html);

    // Impression directe avec mise à l’échelle correcte
    doc.print(&printer);

    return QFile::exists(finalFilePath);
}



bool PersonnelManager::validatePersonnelData() const {
    // Vérification des espaces blancs
    if (nom.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le nom ne peut pas être vide");
        return false;
    }
    if (prenom.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le prénom ne peut pas être vide");
        return false;
    }
    if (cin.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le CIN ne peut pas être vide");
        return false;
    }
    if (numTel.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le numéro de téléphone ne peut pas être vide");
        return false;
    }
    if (email.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "L'email ne peut pas être vide");
        return false;
    }

    // Reste de la validation...
    return true;
}

int PersonnelManager::getNextId() {
    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT COALESCE(MAX(IDPER), 0) + 1 FROM personnels");

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}


QString PersonnelManager::generatePassword() {
    return QString("%1%2%3")
    .arg(cin.right(4))
        .arg(numTel.right(4))
        .arg(nom.left(2).toLower());
}

bool PersonnelManager::addPersonnel() {
    // Vérification des champs obligatoires
    if (nom.trimmed().isEmpty() || prenom.trimmed().isEmpty() ||
        cin.isEmpty() || numTel.isEmpty()) {
        QMessageBox::critical(nullptr, "Erreur", "Les champs obligatoires (Nom, Prénom, CIN, Téléphone) doivent être remplis");
        return false;
    }

    // Validation des formats
    QRegularExpression numRegex("^\\d{8}$");
    if (!numRegex.match(cin).hasMatch()) {
        QMessageBox::critical(nullptr, "Erreur", "Le CIN doit contenir exactement 8 chiffres");
        return false;
    }
    if (!numRegex.match(numTel).hasMatch()) {
        QMessageBox::critical(nullptr, "Erreur", "Le numéro de téléphone doit contenir exactement 8 chiffres");
        return false;
    }

    // Vérification de la connexion à la base de données
    QSqlDatabase& db = dbConnection.getDatabase();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Erreur", "Connexion à la base de données perdue");
        return false;
    }

    // Début de la transaction
    if (!db.transaction()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de démarrer la transaction: " + db.lastError().text());
        return false;
    }

    try {
        // Génération du nouvel ID
        idPer = getNextId();
        if (idPer <= 0) {
            throw std::runtime_error("Impossible de générer un ID valide pour le personnel");
        }

        // Génération des identifiants
        emailG = generateEmail();
        motDePasseG = generatePassword();

        // Préparation de la requête SQL avec le schéma complet
        QString sql = "INSERT INTO WAEL.PERSONNELS "  // Notez le changement ici (PERSONNELS au lieu de PERSONNEL)
                      "(IDPER, NOM, PRENOM, SEXE, CIN, NUMTEL, EMAIL, ADRESSE, "
                      "DATEAJOUT, ROLE, EMAILG, MOTDEPASSEG, PHOTO_BASE64, "
                      "QUESTION_INDEX, REPONSE) "  // Notez les noms de colonnes exacts
                      "VALUES (:id, :nom, :prenom, :sexe, :cin, :tel, :email, :adresse, "
                      "TO_DATE(:date, 'DD/MM/YYYY'), :role, :emailg, :mdp, :photo, "
                      ":question_index, :reponse)";

        QSqlQuery query(db);
        if (!query.prepare(sql)) {
            throw std::runtime_error("Échec de la préparation de la requête: " +
                                     query.lastError().text().toStdString() +
                                     "\nRequête: " + sql.toStdString());
        }

        // Binding des valeurs
        query.bindValue(":id", idPer);
        query.bindValue(":nom", nom.trimmed());
        query.bindValue(":prenom", prenom.trimmed());
        query.bindValue(":sexe", sexe.isEmpty() ? QVariant(QVariant::String) : sexe);
        query.bindValue(":cin", cin);
        query.bindValue(":tel", numTel);
        query.bindValue(":email", email.trimmed().isEmpty() ? QVariant(QVariant::String) : email.trimmed());
        query.bindValue(":adresse", adresse.isEmpty() ? QVariant(QVariant::String) : adresse);
        query.bindValue(":date", dateAjout.toString("dd/MM/yyyy"));
        query.bindValue(":role", role.isEmpty() ? QVariant(QVariant::String) : role);
        query.bindValue(":emailg", emailG);
        query.bindValue(":mdp", motDePasseG);
        query.bindValue(":photo", photoBase64.isEmpty() ? QVariant(QVariant::String) : photoBase64);
        query.bindValue(":question_index", questionIndex);
        query.bindValue(":reponse", reponseSecrete.isEmpty() ? QVariant(QVariant::String) : reponseSecrete);

        // Exécution de la requête
        if (!query.exec()) {
            QString error = query.lastError().text();
            if (error.contains("ORA-00001", Qt::CaseInsensitive)) {
                throw std::runtime_error("Un personnel avec ce CIN existe déjà dans la base de données");
            } else if (error.contains("table or view does not exist", Qt::CaseInsensitive)) {
                throw std::runtime_error("Table introuvable. Vérifiez:\n"
                                         "- Que la table WAEL.PERSONNELS existe\n"
                                         "- Que l'utilisateur a les droits nécessaires\n"
                                         "Erreur complète: " + error.toStdString());
            }
            throw std::runtime_error("Erreur d'exécution: " + error.toStdString());
        }

        // Validation de la transaction
        if (!db.commit()) {
            throw std::runtime_error("Échec de validation: " + db.lastError().text().toStdString());
        }

        // Mise à jour des données
        loadPersonnel();
        emit personnelDataChanged();
        emit statsUpdated();

        return true;

    } catch (const std::exception& e) {
        db.rollback();
        qCritical() << "Erreur dans addPersonnel:" << e.what();
        QMessageBox::critical(nullptr, "Erreur BD",
                              QString::fromStdString(e.what()) +
                                  "\nCode erreur: " + db.lastError().nativeErrorCode());
        return false;
    }
}

void PersonnelManager::setQuestionIndex(int index) {
    questionIndex = index;
}

void PersonnelManager::setReponseSecrete(const QString &reponse) {
    reponseSecrete = reponse;
}

int PersonnelManager::getQuestionIndex() const {
    return questionIndex;
}

QString PersonnelManager::getReponseSecrete() const {
    return reponseSecrete;
}

bool PersonnelManager::editPersonnel() {
    // Validation ID
    if (idPer <= 0) {
        QMessageBox::critical(nullptr, "Erreur", "ID personnel invalide");
        return false;
    }

    // Validation données
    if (nom.trimmed().isEmpty() || prenom.trimmed().isEmpty() ||
        cin.isEmpty() || numTel.isEmpty()) {
        QMessageBox::critical(nullptr, "Erreur", "Tous les champs obligatoires doivent être remplis");
        return false;
    }

    QRegularExpression numRegex("^\\d{8}$");
    if (!numRegex.match(cin).hasMatch() || !numRegex.match(numTel).hasMatch()) {
        QMessageBox::critical(nullptr, "Erreur", "Le CIN et le téléphone doivent contenir exactement 8 chiffres");
        return false;
    }

    // Vérification connexion BD
    QSqlDatabase& db = dbConnection.getDatabase();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Erreur", "Connexion à la base de données perdue");
        return false;
    }

    // Début transaction
    db.transaction();

    try {
        // Construction requête dynamique
        QString sql = "UPDATE personnels SET "
                      "NOM = :nom, PRENOM = :prenom, SEXE = :sexe, "
                      "CIN = :cin, NUMTEL = :tel, EMAIL = :email, "
                      "ADRESSE = :adresse, DATEAJOUT = TO_DATE(:date, 'DD/MM/YYYY'), "
                      "ROLE = :role";

        // Gestion conditionnelle de la photo
        if (!photoBase64.isNull()) {
            sql += photoBase64.isEmpty() ? ", PHOTO_BASE64 = NULL" : ", PHOTO_BASE64 = :photo";
        }

        sql += " WHERE IDPER = :id";

        QSqlQuery query(db);
        if (!query.prepare(sql)) {
            throw std::runtime_error("Échec préparation requête: " + query.lastError().text().toStdString());
        }

        // Binding des valeurs
        query.bindValue(":nom", nom.trimmed());
        query.bindValue(":prenom", prenom.trimmed());
        query.bindValue(":sexe", sexe);
        query.bindValue(":cin", cin);
        query.bindValue(":tel", numTel);
        query.bindValue(":email", email.trimmed());
        query.bindValue(":adresse", adresse);
        query.bindValue(":date", dateAjout.toString("dd/MM/yyyy"));
        query.bindValue(":role", role);
        query.bindValue(":id", idPer);

        if (!photoBase64.isNull() && !photoBase64.isEmpty()) {
            query.bindValue(":photo", photoBase64);
        }

        // Exécution
        if (!query.exec()) {
            QString error = query.lastError().text();
            if (error.contains("unique constraint", Qt::CaseInsensitive)) {
                throw std::runtime_error("Un personnel avec ce CIN existe déjà");
            }
            throw std::runtime_error("Erreur d'exécution: " + error.toStdString());
        }

        // Vérification lignes affectées
        if (query.numRowsAffected() <= 0) {
            throw std::runtime_error("Aucune modification effectuée - ID non trouvé");
        }

        // Commit
        if (!db.commit()) {
            throw std::runtime_error("Échec commit: " + db.lastError().text().toStdString());
        }

        // Mise à jour UI
        loadPersonnel();
        emit personnelDataChanged();
        emit statsUpdated();

        return true;

    } catch (const std::exception& e) {
        db.rollback();
        qCritical() << "Erreur editPersonnel:" << e.what();
        QMessageBox::critical(nullptr, "Erreur", e.what());
        return false;
    }
}



bool PersonnelManager::deletePersonnel() {
    if (idPer <= 0) return false;

    QSqlDatabase& db = dbConnection.getDatabase();
    if (!db.transaction()) return false;

    try {
        QSqlQuery query(db);
        query.prepare("DELETE FROM personnels WHERE IDPER = :IDPER");
        query.bindValue(":IDPER", idPer);

        if (!query.exec()) throw std::runtime_error(query.lastError().text().toStdString());
        if (!db.commit()) throw std::runtime_error("Commit failed");

        loadPersonnel();
        emit personnelDataChanged();
        emit statsUpdated();
        return true;
    } catch (const std::exception& e) {
        db.rollback();
        QMessageBox::critical(nullptr, "Erreur", QString::fromStdString(e.what()));
        return false;
    }
}


