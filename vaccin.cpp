#include "vaccin.h"

// Constructeurs
Vaccin::Vaccin() : id(0), nom(""), fabricant(""), doses(0), efficacite(0.0) {}

Vaccin::Vaccin(int id, const QString &nom, const QString &fabricant, int doses, double efficacite)
    : id(id), nom(nom), fabricant(fabricant), doses(doses), efficacite(efficacite) {}

// Getters
int Vaccin::getId() const { return id; }
QString Vaccin::getNom() const { return nom; }
QString Vaccin::getFabricant() const { return fabricant; }
int Vaccin::getDoses() const { return doses; }
double Vaccin::getEfficacite() const { return efficacite; }

// Setters
void Vaccin::setId(int id) { this->id = id; }
void Vaccin::setNom(const QString &nom) { this->nom = nom; }
void Vaccin::setFabricant(const QString &fabricant) { this->fabricant = fabricant; }
void Vaccin::setDoses(int doses) { this->doses = doses; }
void Vaccin::setEfficacite(double efficacite) { this->efficacite = efficacite; }
