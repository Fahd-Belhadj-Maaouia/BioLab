#ifndef TOOLSMANAGER_H
#define TOOLSMANAGER_H

#include <QObject>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QByteArray>

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

    // CRUD Operations
    void loadTools();
    void addTool();
    void editTool();
    void deleteTool();

    // Form-related methods
    void showAddToolForm();

private slots:
    void onAddToolSubmitted();
    void onUploadImageClicked();

private:
    int id;
    QString nomMateriel;
    QString categorie;
    QString description;
    int stock;
    int quantiteMaximale;
    QByteArray uploadImage;
    QString fournisseur;

    QTableWidget *toolstable;

    QWidget *addToolForm;
    QLineEdit *nomMaterielInput;
    QLineEdit *categorieInput;
    QLineEdit *descriptionInput;
    QSpinBox *stockInput;
    QSpinBox *quantiteMaximaleInput;
    QPushButton *uploadImageButton;
    QLineEdit *fournisseurInput;
    QByteArray uploadImageData;
};

#endif // TOOLSMANAGER_H
