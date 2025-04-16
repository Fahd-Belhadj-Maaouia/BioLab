#ifndef MESSAGESMANAGER_H
#define MESSAGESMANAGER_H

#include <QObject>
#include <QTableWidget>
#include <QListWidget>
#include <QSqlQuery>
#include <QMessageBox>
#include <QMap>
#include <QVariant>
#include "connection.h"

class MessagesManager : public QObject
{
    Q_OBJECT
public:
    explicit MessagesManager(QTableWidget *table = nullptr, QObject *parent = nullptr);
    ~MessagesManager();

    void setMessageTable(QTableWidget *table);
    void setContactsList(QListWidget *list) { contactsList = list; }
    void loadMessages();

    // Database connection methods
    bool createDatabaseConnection();
    QSqlDatabase& getDatabaseConnection();

    // Existing message methods
    bool addMessage(const QString &titre, const QString &contenu,
                    int idExpediteur, const QString &typeExpediteur,
                    int idDestinataire, const QString &typeDestinataire);

    // New overloaded version with 4 parameters
    bool addMessage(const QString &titre, const QString &contenu,
                    int idExpediteur, const QString &typeExpediteur);

    bool markAsRead(int messageId);
    bool deleteMessage(int messageId);
    QMap<QString, QVariant> getMessageById(int id);
    int getSelectedMessageId() const;

    int getCurrentContactId() const { return currentContactId; }
    void setCurrentContactId(int id) { currentContactId = id; }

    const QString& getCurrentContactType() const { return currentContactType; }
    void setCurrentContactType(const QString &type);

    // Dialog methods
    void showReadMessageDialog(int messageId, int currentUserId, const QString &currentUserType, QWidget *parent = nullptr);
    void showNewMessageDialog(int currentUserId, const QString &currentUserType, QWidget *parent = nullptr);

private:
    QTableWidget *messageTable;
    QListWidget *contactsList;  // Ajout de la liste des contacts
    DatabaseConnection dbConnection;
    int currentMessageId;
    int currentContactId;
    QString currentContactType;

    void setupTable();
    int getNextId();
};

#endif // MESSAGESMANAGER_H
