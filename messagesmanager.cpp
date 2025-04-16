#include "messagesmanager.h"
#include <QHeaderView>
#include <QSqlError>
#include <QDebug>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QDateTime>

MessagesManager::MessagesManager(QTableWidget *table, QObject *parent)
    : QObject(parent), messageTable(table), contactsList(nullptr), currentMessageId(-1),
    currentContactId(-1), currentContactType("") {
    if (messageTable) {
        setupTable();
        loadMessages();
    }
}

MessagesManager::~MessagesManager() {
    // Nettoyage géré par le destructeur de DatabaseConnection
}

void MessagesManager::setMessageTable(QTableWidget *table) {
    messageTable = table;
    if (messageTable) {
        setupTable();
        loadMessages();
    }
}

bool MessagesManager::createDatabaseConnection() {
    return dbConnection.createConnection();
}

QSqlDatabase& MessagesManager::getDatabaseConnection() {
    return dbConnection.getDatabase();
}

void MessagesManager::setupTable() {
    // Configuration des en-têtes du tableau
    QStringList headers = {
        "ID", "Titre", "Expéditeur", "Type", "Date", "Lu"
    };

    messageTable->setColumnCount(headers.size());
    messageTable->setHorizontalHeaderLabels(headers);

    // Paramètres de sélection et d'affichage
    messageTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    messageTable->setSelectionMode(QAbstractItemView::SingleSelection);
    messageTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Ajustement des colonnes
    messageTable->horizontalHeader()->setStretchLastSection(true);
    messageTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Connecter le signal de sélection de ligne
    connect(messageTable, &QTableWidget::itemSelectionChanged, this, [this]() {
        QList<QTableWidgetItem*> items = messageTable->selectedItems();
        if (!items.isEmpty()) {
            currentMessageId = messageTable->item(items.first()->row(), 0)->text().toInt();
        } else {
            currentMessageId = -1;
        }
    });
}

void MessagesManager::loadMessages() {
    qDebug() << "Début de loadMessages()";

    if (!messageTable) {
        qDebug() << "ERREUR: messageTable est NULL!";
        return;
    }

    // Réinitialiser le tableau
    messageTable->setRowCount(0);
    qDebug() << "Tableau réinitialisé";

    if (!dbConnection.createConnection()) {
        qDebug() << "ERREUR: Échec de connexion à la base de données";
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return;
    }
    qDebug() << "Connexion à la base de données réussie";

    QSqlQuery query(dbConnection.getDatabase());

    // Simplifier la requête pour tester
    QString queryStr =
        "SELECT m.ID_MESSAGE, m.TITRE, m.TYPE_EXPEDITEUR, "
        "m.ID_EXPEDITEUR, TO_CHAR(m.DATE_ENVOI, 'DD/MM/YYYY HH24:MI') as DATE_FORMATEE, "
        "m.EST_LU FROM messages m ORDER BY m.DATE_ENVOI DESC";

    query.prepare(queryStr);
    qDebug() << "Exécution de la requête:" << queryStr;

    if (query.exec()) {
        qDebug() << "Requête réussie, traitement des résultats";
        int row = 0;
        while (query.next()) {
            messageTable->insertRow(row);

            // Afficher chaque valeur pour débogage
            qDebug() << "Message ID:" << query.value("ID_MESSAGE").toString();
            qDebug() << "Titre:" << query.value("TITRE").toString();

            // Ajouter les éléments au tableau
            messageTable->setItem(row, 0, new QTableWidgetItem(query.value("ID_MESSAGE").toString()));
            messageTable->setItem(row, 1, new QTableWidgetItem(query.value("TITRE").toString()));

            // Pour l'expéditeur, simplifier temporairement
            QString expediteur = "ID: " + query.value("ID_EXPEDITEUR").toString();
            messageTable->setItem(row, 2, new QTableWidgetItem(expediteur));

            messageTable->setItem(row, 3, new QTableWidgetItem(query.value("TYPE_EXPEDITEUR").toString()));
            messageTable->setItem(row, 4, new QTableWidgetItem(query.value("DATE_FORMATEE").toString()));

            // Afficher si le message a été lu
            QString estLu = query.value("EST_LU").toInt() == 1 ? "Oui" : "Non";
            messageTable->setItem(row, 5, new QTableWidgetItem(estLu));

            row++;
        }
        qDebug() << "Nombre total de messages chargés:" << row;
    } else {
        qDebug() << "ERREUR lors du chargement des messages:"
                 << query.lastError().text();
        QMessageBox::warning(nullptr, "Erreur",
                             "Impossible de charger les messages: " + query.lastError().text());
    }

    // Forcer le redimensionnement des colonnes
    messageTable->resizeColumnsToContents();
    qDebug() << "Fin de loadMessages()";
}

int MessagesManager::getNextId() {
    // Obtenir le prochain ID disponible
    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return 1; // ID par défaut
    }

    QSqlQuery query(dbConnection.getDatabase());
    query.prepare("SELECT seq_message_id.NEXTVAL FROM DUAL");

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        qDebug() << "Erreur lors de la récupération du prochain ID:"
                 << query.lastError().text();
        return 1;
    }
}

bool MessagesManager::addMessage(const QString &titre, const QString &contenu,
                                 int idExpediteur, const QString &typeExpediteur,
                                 int idDestinataire, const QString &typeDestinataire) {
    // Validation des données
    if (titre.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le titre du message est obligatoire");
        return false;
    }

    if (contenu.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le contenu du message est obligatoire");
        return false;
    }

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return false;
    }

    QSqlDatabase& db = dbConnection.getDatabase();

    // Début de la transaction
    if (!db.transaction()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de démarrer la transaction");
        return false;
    }

    try {
        int nextId = getNextId();

        QSqlQuery query(db);
        query.prepare(
            "INSERT INTO messages "
            "(ID_MESSAGE, TITRE, CONTENU, DATE_ENVOI, ID_EXPEDITEUR, TYPE_EXPEDITEUR, ID_DESTINATAIRE, TYPE_DESTINATAIRE, EST_LU) "
            "VALUES (:ID_MESSAGE, :TITRE, :CONTENU, SYSDATE, :ID_EXPEDITEUR, :TYPE_EXPEDITEUR, :ID_DESTINATAIRE, :TYPE_DESTINATAIRE, 0)"
            );

        query.bindValue(":ID_MESSAGE", nextId);
        query.bindValue(":TITRE", titre);
        query.bindValue(":CONTENU", contenu);
        query.bindValue(":ID_EXPEDITEUR", idExpediteur);
        query.bindValue(":TYPE_EXPEDITEUR", typeExpediteur);
        query.bindValue(":ID_DESTINATAIRE", idDestinataire);
        query.bindValue(":TYPE_DESTINATAIRE", typeDestinataire);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        // Commit de la transaction
        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }

        // Recharger les messages après ajout
        loadMessages();

        QMessageBox::information(nullptr, "Succès", "Message envoyé avec succès");
        return true;

    } catch (const std::exception& e) {
        // Rollback en cas d'erreur
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur",
                             QString("Erreur lors de l'envoi du message: %1").arg(e.what()));
        return false;
    }
}

bool MessagesManager::addMessage(const QString &titre, const QString &contenu,
                                 int idExpediteur, const QString &typeExpediteur) {
    // Vérifier que la liste des contacts existe
    if (!contactsList) {
        QMessageBox::warning(nullptr, "Erreur", "Liste des contacts non initialisée");
        return false;
    }

    // Use the current contact ID and type from the UI context
    if (currentContactId <= 0) {
        QMessageBox::warning(nullptr, "Erreur", "Aucun destinataire sélectionné");
        return false;
    }

    // Récupérer le type de contact depuis l'interface utilisateur
    QString typeDestinataire = currentContactType;

    // Vérifier si le type est vide, essayer de le trouver dans la liste des contacts
    if (typeDestinataire.isEmpty()) {
        for (int i = 0; i < contactsList->count(); i++) {
            QListWidgetItem *item = contactsList->item(i);
            if (item->data(Qt::UserRole).toInt() == currentContactId) {
                typeDestinataire = item->data(Qt::UserRole + 1).toString();
                break;
            }
        }
    }

    // Vérifier que le type a bien été trouvé
    if (typeDestinataire.isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Type de destinataire introuvable");
        return false;
    }

    // Call the existing method with all 6 parameters
    return addMessage(titre, contenu, idExpediteur, typeExpediteur,
                      currentContactId, typeDestinataire);
}

void MessagesManager::setCurrentContactType(const QString &type) {
    currentContactType = type;
}

bool MessagesManager::markAsRead(int messageId) {
    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return false;
    }

    QSqlDatabase& db = dbConnection.getDatabase();

    // Début de la transaction
    if (!db.transaction()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de démarrer la transaction");
        return false;
    }

    try {
        QSqlQuery query(db);
        query.prepare("UPDATE messages SET EST_LU = 1 WHERE ID_MESSAGE = :ID_MESSAGE");
        query.bindValue(":ID_MESSAGE", messageId);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        // Commit de la transaction
        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }

        // Recharger les messages après modification
        loadMessages();
        return true;

    } catch (const std::exception& e) {
        // Rollback en cas d'erreur
        db.rollback();
        qDebug() << "Erreur lors du marquage comme lu:" << e.what();
        return false;
    }
}

bool MessagesManager::deleteMessage(int messageId) {
    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return false;
    }

    QSqlDatabase& db = dbConnection.getDatabase();

    // Début de la transaction
    if (!db.transaction()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de démarrer la transaction");
        return false;
    }

    try {
        QSqlQuery query(db);
        query.prepare("DELETE FROM messages WHERE ID_MESSAGE = :ID_MESSAGE");
        query.bindValue(":ID_MESSAGE", messageId);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        // Commit de la transaction
        if (!db.commit()) {
            throw std::runtime_error("Impossible de valider la transaction");
        }

        // Recharger les messages après suppression
        loadMessages();

        QMessageBox::information(nullptr, "Succès", "Message supprimé avec succès");
        return true;

    } catch (const std::exception& e) {
        // Rollback en cas d'erreur
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur",
                             QString("Erreur lors de la suppression du message: %1").arg(e.what()));
        return false;
    }
}

QMap<QString, QVariant> MessagesManager::getMessageById(int id) {
    QMap<QString, QVariant> messageData;

    if (!dbConnection.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Impossible de se connecter à la base de données");
        return messageData;
    }

    QSqlQuery query(dbConnection.getDatabase());

    // Obtenir les détails du message et le nom de l'expéditeur
    QString queryStr =
        "SELECT m.*, "
        "CASE "
        "   WHEN m.TYPE_EXPEDITEUR = 'CHERCHEUR' THEN "
        "       (SELECT c.NOM || ' ' || c.PRENOM FROM chercheurs c WHERE c.IDC = m.ID_EXPEDITEUR) "
        "   WHEN m.TYPE_EXPEDITEUR = 'PERSONNEL' THEN "
        "       (SELECT p.NOM || ' ' || p.PRENOM FROM personnel p WHERE p.IDPER = m.ID_EXPEDITEUR) "
        "END AS NOM_EXPEDITEUR, "
        "TO_CHAR(m.DATE_ENVOI, 'DD/MM/YYYY HH24:MI') as DATE_FORMATEE "
        "FROM messages m "
        "WHERE m.ID_MESSAGE = :ID_MESSAGE";

    query.prepare(queryStr);
    query.bindValue(":ID_MESSAGE", id);

    if (query.exec() && query.next()) {
        messageData["id"] = query.value("ID_MESSAGE");
        messageData["titre"] = query.value("TITRE");
        messageData["contenu"] = query.value("CONTENU");
        messageData["date_envoi"] = query.value("DATE_FORMATEE");
        messageData["id_expediteur"] = query.value("ID_EXPEDITEUR");
        messageData["type_expediteur"] = query.value("TYPE_EXPEDITEUR");
        messageData["est_lu"] = query.value("EST_LU");
        messageData["nom_expediteur"] = query.value("NOM_EXPEDITEUR");

        // Marquer comme lu automatiquement lors de la consultation
        markAsRead(id);
    } else {
        qDebug() << "Erreur lors de la récupération du message:" << query.lastError().text();
    }

    return messageData;
}

int MessagesManager::getSelectedMessageId() const {
    return currentMessageId;
}

void MessagesManager::showReadMessageDialog(int messageId, int currentUserId, const QString &currentUserType, QWidget *parent) {
    // Récupérer les données du message
    QMap<QString, QVariant> messageData = getMessageById(messageId);

    if (messageData.isEmpty()) {
        QMessageBox::warning(parent, "Erreur", "Message non trouvé");
        return;
    }

    // Créer la boîte de dialogue dynamiquement
    QDialog dialog(parent);
    dialog.setWindowTitle("Lecture du message");
    dialog.setMinimumSize(600, 400);

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);

    // Titre du message
    QLabel *titleLabel = new QLabel(messageData["titre"].toString());
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(titleFont.pointSize() + 2);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    // Informations sur l'expéditeur et la date
    QHBoxLayout *infoLayout = new QHBoxLayout();
    QLabel *fromLabel = new QLabel("De: " + messageData["nom_expediteur"].toString());
    QLabel *dateLabel = new QLabel("Date: " + messageData["date_envoi"].toString());
    infoLayout->addWidget(fromLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(dateLabel);
    mainLayout->addLayout(infoLayout);

    // Contenu du message
    QTextBrowser *contentBrowser = new QTextBrowser();
    contentBrowser->setText(messageData["contenu"].toString());
    mainLayout->addWidget(contentBrowser);

    // Boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *replyButton = new QPushButton("Répondre");
    QPushButton *closeButton = new QPushButton("Fermer");

    buttonLayout->addStretch();
    buttonLayout->addWidget(replyButton);
    buttonLayout->addWidget(closeButton);
    mainLayout->addLayout(buttonLayout);

    // Connexions des boutons
    QObject::connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    QObject::connect(replyButton, &QPushButton::clicked, [this, &dialog, messageData, currentUserId, currentUserType]() {
        // Fermer cette boîte de dialogue
        dialog.accept();

        // Ouvrir la boîte de dialogue de réponse
        showNewMessageDialog(currentUserId, currentUserType, dialog.parentWidget());
    });

    // Exécuter la boîte de dialogue
    dialog.exec();
}

void MessagesManager::showNewMessageDialog(int currentUserId, const QString &currentUserType, QWidget *parent) {
    // Créer la boîte de dialogue dynamiquement
    QDialog dialog(parent);
    dialog.setWindowTitle("Nouveau message");
    dialog.setMinimumSize(600, 400);

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);

    // Titre
    QLabel *dialogTitle = new QLabel("Nouveau message");
    QFont titleFont = dialogTitle->font();
    titleFont.setBold(true);
    titleFont.setPointSize(titleFont.pointSize() + 2);
    dialogTitle->setFont(titleFont);
    dialogTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(dialogTitle);

    // Formulaire
    QFormLayout *formLayout = new QFormLayout();

    QLabel *titleLabel = new QLabel("Titre:");
    QLineEdit *titleEdit = new QLineEdit();

    QLabel *contentLabel = new QLabel("Message:");
    QTextEdit *contentEdit = new QTextEdit();

    formLayout->addRow(titleLabel, titleEdit);
    formLayout->addRow(contentLabel, contentEdit);

    mainLayout->addLayout(formLayout);

    // Boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *cancelButton = new QPushButton("Annuler");
    QPushButton *sendButton = new QPushButton("Envoyer");

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(sendButton);
    mainLayout->addLayout(buttonLayout);

    // Connexions des boutons
    QObject::connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    QObject::connect(sendButton, &QPushButton::clicked, [this, &dialog, titleEdit, contentEdit, currentUserId, currentUserType]() {
        QString titre = titleEdit->text();
        QString contenu = contentEdit->toPlainText();

        if (this->addMessage(titre, contenu, currentUserId, currentUserType)) {
            dialog.accept();
        }
    });

    // Exécuter la boîte de dialogue
    dialog.exec();
}
