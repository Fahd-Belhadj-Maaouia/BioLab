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
    QSqlQuery query;
    query.prepare("INSERT INTO PROJETDERECHERCHES (TITRE, SPONSOR, PARTICIPANTS, OBJECTIF, LOCALISATION, DESCRIPTION, DATEDEBUT, DATEFIN, COUT) "
                  "VALUES (:TITRE, :SPONSOR, :PARTICIPANTS, :OBJECTIF, :LOCALISATION, :DESCRIPTION, :DATEDEBUT, :DATEFIN, :COUT)");

    query.bindValue(":TITRE", TITRE);
    query.bindValue(":SPONSOR", SPONSOR);
    query.bindValue(":PARTICIPANTS", PARTICIPANTS);
    query.bindValue(":OBJECTIF", OBJECTIF);
    query.bindValue(":LOCALISATION", LOCALISATION);
    query.bindValue(":DESCRIPTION", DESCRIPTION);
    query.bindValue(":DATEDEBUT", DATE_DEBUT);
    query.bindValue(":DATEFIN", DATE_FIN);
    query.bindValue(":COUT", COUT);


    return query.exec();
}

/*QSqlQueryModel * ProjetDeRecherche::Post()
{
    //ToDo
}

bool ProjetDeRecherche::Delete(int ID)
{
    //ToDo
}
*/
