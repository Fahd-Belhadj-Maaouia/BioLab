#ifndef VACCIN_H
#define VACCIN_H

#include <QString>

class Vaccin {
public:
    // Constructeurs
    Vaccin();
    Vaccin(int id, const QString &nom, const QString &fabricant, int doses, double efficacite);

    // Getters
    int getId() const;
    QString getNom() const;
    QString getFabricant() const;
    int getDoses() const;
    double getEfficacite() const;

    // Setters
    void setId(int id);
    void setNom(const QString &nom);
    void setFabricant(const QString &fabricant);
    void setDoses(int doses);
    void setEfficacite(double efficacite);

private:
    int id;
    QString nom;
    QString fabricant;
    int doses;
    double efficacite;
};

#endif // VACCIN_H
