#ifndef TOOLSMANAGER_H
#define TOOLSMANAGER_H

#include <QObject>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QWidget>
#include <QByteArray>
#include "connection.h"

class ToolsManager : public QObject {
    Q_OBJECT

public:
    explicit ToolsManager(QTableWidget *toolstable, QWidget *parent = nullptr);
    ~ToolsManager();

    // Getters
    int getId() const;
    QString getNomMateriel() const;
    QString getCategorie() const;
    QString getDescription() const;
    int getStock() const;
    int getQuantiteMaximale() const;
    QByteArray getUploadImage() const;
    QString getFournisseur() const;

    // Setters
    void setId(int id);
    void setNomMateriel(const QString &nomMateriel);
    void setCategorie(const QString &categorie);
    void setDescription(const QString &description);
    void setStock(int stock);
    void setQuantiteMaximale(int quantiteMaximale);
    void setUploadImage(const QByteArray &uploadImage);
    void setFournisseur(const QString &fournisseur);

    // Database operations
    void loadTools();
    void addTool();
    void editTool();
    void deleteTool();

    // UI Methods
    void showAddToolForm();

private slots:
    void onAddToolSubmitted();
    void onUploadImageClicked();

private:
    // Database connection
    DatabaseConnection dbConnection;

    // Table widget
    QTableWidget *toolstable;

    // Form widgets
    QWidget *addToolForm;
    QLineEdit *nomMaterielInput;
    QLineEdit *categorieInput;
    QLineEdit *descriptionInput;
    QSpinBox *stockInput;
    QSpinBox *quantiteMaximaleInput;
    QPushButton *uploadImageButton;
    QLineEdit *fournisseurInput;

    // Tool properties
    int id = 0;
    QString nomMateriel;
    QString categorie;
    QString description;
    int stock = 0;
    int quantiteMaximale = 0;
    QByteArray uploadImage;
    QString fournisseur;
    QByteArray uploadImageData;
};

#endif // TOOLSMANAGER_H
