    #ifndef PROJETDERECHERCHE_H
    #define PROJETDERECHERCHE_H
    #include <QString>
    #include <QSqlQuery>
    #include <QSqlQueryModel>
    #include <QDate>

    class ProjetDeRecherche
    {
        QString TITRE, SPONSOR, PARTICIPANTS, OBJECTIF, LOCALISATION, DESCRIPTION;
        int COUT;
        QDate DATE_DEBUT, DATE_FIN;

    public:

        //Constructeurs
        ProjetDeRecherche();
        ProjetDeRecherche(QString,QString,QString,QString,QString,QString,int,QDate,QDate);
        //Getters
        QString getTITRE(){return TITRE;}
        QString getSPONSOR(){return SPONSOR;}
        QString getPARTICIPANTS(){return PARTICIPANTS;}
        QString getOBJECTIF(){return OBJECTIF;}
        QString getLOCALISATION(){return LOCALISATION;}
        QString getDESCRIPTION(){return DESCRIPTION;}
        int getCOUT(){return COUT;}
        QDate getDATE_DEBUT(){return DATE_DEBUT;}
        QDate getDATE_FIN(){return DATE_FIN;}
        //Setters
        void setTITRE(QString T){TITRE=T;}
        void setSPONSOR(QString S){SPONSOR=S;}
        void setPARTICIPANTS(QString P){PARTICIPANTS=P;}
        void setOBJECTIF(QString O){OBJECTIF=O;}
        void setLOCALISATION(QString L){LOCALISATION=L;}
        void setDESCRIPTION(QString D){DESCRIPTION=D;}
        void setCOUT(int C){COUT=C;}
        void setDATE_DEBUT(QDate DD){DATE_DEBUT=DD;}
        void setDATE_FIN(QDate DF){DATE_FIN=DF;}
        //Functions
        bool Add();
        static QSqlQueryModel * Post();
        static bool Delete(int);
        bool Update(int IDPRO);
        static ProjetDeRecherche getById(int IDPRO);




    };

    #endif // PROJETDERECHERCHE_H
