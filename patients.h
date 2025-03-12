#ifndef PATIENTS_H
#define PATIENTS_H


#include <QObject>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QByteArray>


class PatientsManager : public QObject {
    Q_OBJECT

public:
    explicit PatientsManager(QTableWidget *patientstable, QWidget *parent = nullptr);
    ~PatientsManager();




    void addPatient(const QString &Nom, const QString &prenom, const QString &sexe,
                    const QString &cin, const QString &adresse, const QString &numtel,
                    const QString &maladiechronique);
    void loadPatients();
    void updatePatient(int id);
    void deletePatient(int id);

    // Getters
    int getIDP() const;
    QString getNom() const;
    QString getprenom() const;
    QString getsexe() const;
    QString getcin() const;
    QString getadresse() const;
    QString getnumtel() const;
    QString getmaladiechronique() const;



    // Setters
    void setIDP(int IDP);
    void setNom(const QString &Nom);
    void setprenom(const QString &prenom);
    void setsexe(const QString &sexe);
    void setcin(const QString &cin);
    void setadresse(const QString &adresse);
    void setnumtel(const QString &numtel);
    void setmaladiechronique(const QString &maladiechronique);




private:
    // Attributes
    int IDP;
    QString Nom;
    QString prenom;
    QString sexe;
    QString cin;
    QString adresse;
    QString numtel;
    QString maladiechronique;




    // Pointer to the tools table widget
    QTableWidget *patientstable;

    // Form-related members
    QWidget *addPatientForm;
    QLineEdit *NomInput;
    QLineEdit *prenomInput;
    QLineEdit *sexeInput;
    QLineEdit *cinInput;
    QLineEdit *adresseInput;
    QLineEdit *numtelInput;
    QLineEdit *maladiechroniqueInput;

};



#endif // PATIENTS_H


