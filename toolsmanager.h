#ifndef TOOLSMANAGER_H
#define TOOLSMANAGER_H

#include <QObject>
#include <QTableWidget>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>
#include <QMap>
#include <QVariant>
#include "connection.h"

class ToolsManager : public QObject {
    Q_OBJECT

public:
    explicit ToolsManager(QTableWidget *table, QObject *parent = nullptr);
    ~ToolsManager();

    // Setters for tool properties
    void setId(int id) { this->id = id; }
    void setNomMateriel(const QString &nom) { nomMateriel = nom; }
    void setCategorie(const QString &categorie) { this->categorie = categorie; }
    void setDescription(const QString &description) { this->description = description; }
    void setStock(int stock) { this->stock = stock; }
    void setQuantiteMaximale(int quantiteMax) { quantiteMaximale = quantiteMax; }
    void setUploadImage(const QString &imagePath) { uploadImage = imagePath; }
    void setFournisseur(const QString &fournisseur) { this->fournisseur = fournisseur; }
    void setIdPro(int idPro) { this->idPro = idPro; }

    // Database operations
    int getNextId();
    void loadTools();
    bool addTool();
    bool editTool();
    bool deleteTool();
    QMap<QString, QVariant> getToolById(int id);
    bool idProExists(int idPro);

signals:
    void editToolRequested(int id);
    void deleteToolRequested(int id);

private:
    bool validateToolData() const;
    void setupTable();
    QTableWidget *toolsTable;
    DatabaseConnection dbConnection;

    // Tool properties
    int id = 0;
    QString nomMateriel;
    QString categorie;
    QString description;
    int stock = 0;
    int quantiteMaximale = 0;
    QString uploadImage;
    QString fournisseur;
    int idPro = 0;
};

#endif // TOOLSMANAGER_H
