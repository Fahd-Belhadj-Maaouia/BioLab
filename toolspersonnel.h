#ifndef TOOLSPERSONNEL_H
#define TOOLSPERSONNEL_H

#include <QObject>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QDateEdit>

class ToolsPersonnel : public QObject {
    Q_OBJECT

public:
    explicit ToolsPersonnel(QTableWidget *personnelTable, QWidget *parent = nullptr);
    ~ToolsPersonnel();

    // Getters
    int getId() const;
    QString getNom() const;
    QString getPrenom() const;
    QString getSexe() const;
    QString getCin() const;
    QString getNumTel() const;
    QString getEmail() const;
    QString getAdresse() const;
    QDate getDateAjout() const;
    QString getRole() const;
    QString getEmailG() const;
    QString getMotDePasseG() const;

    // Setters
    void setId(int id);
    void setNom(const QString &nom);
    void setPrenom(const QString &prenom);
    void setSexe(const QString &sexe);
    void setCin(const QString &cin);
    void setNumTel(const QString &numTel);
    void setEmail(const QString &email);
    void setAdresse(const QString &adresse);
    void setDateAjout(const QDate &dateAjout);
    void setRole(const QString &role);
    void setEmailG(const QString &emailG);
    void setMotDePasseG(const QString &motDePasseG);

    // CRUD Operations
    void loadPersonnel();  // Load personnel from database
    void addPersonnel();   // Add a new personnel
    void editPersonnel();  // Edit an existing personnel
    void deletePersonnel(); // Delete personnel

    // Form-related methods
    void showAddPersonnelForm();

private slots:
    void onAddPersonnelSubmitted(); // Slot for form submission

private:
    // Attributes
    int id;
    QString nom;
    QString prenom;
    QString sexe;
    QString cin;
    QString numTel;
    QString email;
    QString adresse;
    QDate dateAjout;
    QString role;
    QString emailG;
    QString motDePasseG;

    // Pointer to the personnel table widget
    QTableWidget *personnelTable;

    // Form-related members
    QWidget *addPersonnelForm;
    QLineEdit *nomInput;
    QLineEdit *prenomInput;
    QComboBox *sexeInput;
    QLineEdit *cinInput;
    QLineEdit *numTelInput;
    QLineEdit *emailInput;
    QLineEdit *adresseInput;
    QDateEdit *dateAjoutInput;
    QLineEdit *roleInput;
    QLineEdit *emailGInput;
    QLineEdit *motDePasseGInput;
    QPushButton *submitButton;
};

#endif // TOOLSPERSONNEL_H
