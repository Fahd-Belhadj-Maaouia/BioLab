#ifndef RESEARCHERMANAGER_H
#define RESEARCHERMANAGER_H



#include <QObject>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QByteArray>


class ResearcherManager : public QObject {
    Q_OBJECT

public:
    explicit ResearcherManager(QTableWidget *researchertable, QWidget *parent = nullptr);
    ~ResearcherManager();




    void addResearcher(const QString &Nom, const QString &prenom, const QString &sexe,
                    const QString &cin, const QString &adresse, const QString &numtel,
                    const QString &specialite, const QString &datedenaissance, const QString &email );
    void loadResearcher();
    void updateResearcher(int id, const QString &nom, const QString &prenom, const QString &sexe,
                          const QString &cin, const QString &adresse, const QString &numTel,
                          const QString &specialite, const QString &datedenaissance, const QString &email);
    void deleteResearcher(int id);

    // Getters
    int getIDR() const;
    QString getNom() const;
    QString getprenom() const;
    QString getsexe() const;
    QString getcin() const;
    QString getadresse() const;
    QString getnumtel() const;
    QString getspecialite() const;
    QString getdatedenaissance() const;
    QString getemail() const;
    QTableWidget* getTableWidget() const { return researchertable; }




    // Setters
    void setIDR(int IDR);
    void setNom(const QString &Nom);
    void setprenom(const QString &prenom);
    void setsexe(const QString &sexe);
    void setcin(const QString &cin);
    void setadresse(const QString &adresse);
    void setnumtel(const QString &numtel);
    void setspecialite(const QString &specialite);
    void setdatedenaissance(const QString &datedenaissance);
    void setemail(const QString &email);

    void searchResearcherByName(const QString &name);
    void searchResearcherByCIN(const QString &cin);
    void searchResearcherBySpecialite(const QString &specialite);
    void sortResearchersByName(bool ascending);
    void sortResearchersBySpecialite(bool ascending);
    void filterByAgeRange(int minAge, int maxAge);
    void sendEmailViaSendGrid(const QString &to, const QString &subject, const QString &bodyText);






private:
    // Attributes
    int IDR;
    QString Nom;
    QString prenom;
    QString sexe;
    QString cin;
    QString adresse;
    QString numtel;
    QString specialite;
    QString datedenaissance;
    QString email;



    // Pointer to the tools table widget
    QTableWidget *researchertable;

    // Form-related members
    QWidget *addResearcherForm;
    QLineEdit *NomInput;
    QLineEdit *prenomInput;
    QLineEdit *sexeInput;
    QLineEdit *cinInput;
    QLineEdit *adresseInput;
    QLineEdit *numtelInput;
    QLineEdit *specialiteInput;
    QLineEdit *datedenaissanceInput;
    QLineEdit *emailInput;


};

#endif // RESEARCHERMANAGER_H
