#include "projetderecherche.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>

ProjetDeRecherche::ProjetDeRecherche() {}

ProjetDeRecherche::ProjetDeRecherche(QString TITRE, QString SPONSOR, QString PARTICIPANTS, QString OBJECTIF,
                                     QString LOCALISATION, QString DESCRIPTION, int COUT, QDate DATE_DEBUT, QDate DATE_FIN)
{
    this->TITRE = TITRE;
    this->SPONSOR = SPONSOR;
    this->PARTICIPANTS = PARTICIPANTS;
    this->OBJECTIF = OBJECTIF;
    this->LOCALISATION = LOCALISATION;
    this->DESCRIPTION = DESCRIPTION;
    this->COUT = COUT;
    this->DATE_DEBUT = DATE_DEBUT;
    this->DATE_FIN = DATE_FIN;
}

bool ProjetDeRecherche::Add()
{
    QSqlQuery query(QSqlDatabase::database("main_connection"));
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

    if (!query.exec()) {
        qDebug() << "Add Error:" << query.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel *ProjetDeRecherche::Post()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlDatabase db = QSqlDatabase::database("main_connection");

    if (!db.isOpen()) {
        qDebug() << "Database not open in Post().";
        return model;
    }

    model->setQuery("SELECT IDpro, titre, sponsor, participants, objectif, localisation, description, DateDEBUT, DATEFIN, COUT FROM PROJETDERECHERCHES", db);

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
    QSqlQuery query(QSqlDatabase::database("main_connection"));
    query.prepare("DELETE FROM PROJETDERECHERCHES WHERE IDPRO = :ID");
    query.bindValue(":ID", IDPRO);

    if (!query.exec()) {
        qDebug() << "Delete Error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool ProjetDeRecherche::Update(int IDPRO)
{
    QSqlQuery query(QSqlDatabase::database("main_connection"));
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

    if (!query.exec()) {
        qDebug() << "Update Error:" << query.lastError().text();
        return false;
    }

    return true;
}

ProjetDeRecherche ProjetDeRecherche::getById(int IDPRO)
{
    QSqlQuery query(QSqlDatabase::database("main_connection"));
    query.prepare("SELECT TITRE, SPONSOR, PARTICIPANTS, OBJECTIF, LOCALISATION, DESCRIPTION, DATEDEBUT, DATEFIN, COUT "
                  "FROM PROJETDERECHERCHES WHERE IDPRO = :IDPRO");
    query.bindValue(":IDPRO", IDPRO);

    if (!query.exec()) {
        qDebug() << "GetById Error:" << query.lastError().text();
        return ProjetDeRecherche();
    }

    if (query.next()) {
        return ProjetDeRecherche(
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString(),
            query.value(5).toString(),
            query.value(8).toInt(), // COUT
            query.value(6).toDate(),
            query.value(7).toDate()
            );
    }

    return ProjetDeRecherche();
}
