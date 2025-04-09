#include "projetderecherche.h"
#include <qsqlerror.h>

ProjetDeRecherche::ProjetDeRecherche(){

}

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

QSqlQueryModel * ProjetDeRecherche::Post()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT IDpro, titre, sponsor, participants, objectif, localisation, description, DateDEBUT, DATEFIN, COUT FROM PROJETDERECHERCHES");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDPRO"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("TITRE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("SPONSOR"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("PARTICIPANTS"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("OBJECTIF"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("LOCALISATION"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("DESCRIPTION"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("DateDEBUT"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("DATEFIN"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("COUT"));


    return model;
}


bool ProjetDeRecherche::Delete(int IDPRO)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PROJETDERECHERCHES WHERE IDPRO = :ID");
    query.bindValue(":ID", IDPRO);

    if (!query.exec()) {
        qDebug() << "Error deleting project:" << query.lastError().text();
        return false;
    }

    return true;
}


bool ProjetDeRecherche::Update(int IDPRO)
{
    QSqlQuery query;
    query.prepare("UPDATE PROJETDERECHERCHES SET "
                  "TITRE = :TITRE, "
                  "SPONSOR = :SPONSOR, "
                  "PARTICIPANTS = :PARTICIPANTS, "
                  "OBJECTIF = :OBJECTIF, "
                  "LOCALISATION = :LOCALISATION, "
                  "DESCRIPTION = :DESCRIPTION, "
                  "DATEDEBUT = :DATEDEBUT, "
                  "DATEFIN = :DATEFIN, "
                  "COUT = :COUT "
                  "WHERE IDPRO = :IDPRO");

    query.bindValue(":TITRE", TITRE);
    query.bindValue(":SPONSOR", SPONSOR);
    query.bindValue(":PARTICIPANTS", PARTICIPANTS);
    query.bindValue(":OBJECTIF", OBJECTIF);
    query.bindValue(":LOCALISATION", LOCALISATION);
    query.bindValue(":DESCRIPTION", DESCRIPTION);
    query.bindValue(":DATEDEBUT", DATE_DEBUT);
    query.bindValue(":DATEFIN", DATE_FIN);
    query.bindValue(":COUT", COUT);
    query.bindValue(":IDPRO", IDPRO);

    return query.exec();
}
