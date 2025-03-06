#include "projetderecherche.h"

ProjetDeRecherche::ProjetDeRecherche(QString TITRE,QString SPONSOR,QString PARTICIPANTS,QString OBJECTIF,QString LOCALISATION,QString DESCRIPTION,int COUT,QDate DATE_DEBUT,QDate DATE_FIN) {
    this->TITRE=TITRE;
    this->SPONSOR=SPONSOR;
    this->PARTICIPANTS=PARTICIPANTS;
    this->OBJECTIF=OBJECTIF;
    this->LOCALISATION=LOCALISATION;
    this->DESCRIPTION=DESCRIPTION;
    this->COUT=COUT;
    this->DATE_DEBUT=DATE_DEBUT;
    this->DATE_FIN=DATE_FIN;
}

bool ProjetDeRecherche::Add()
{
    //ToDo
}

QSqlQueryModel * ProjetDeRecherche::Post()
{
    //ToDo
}

bool ProjetDeRecherche::Delete(int ID)
{
    //ToDo
}
