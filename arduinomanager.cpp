// ArduinoManager.cpp
#include "ArduinoManager.h"
#include "vaccinmanager.h" // Votre classe de gestion des vaccins

ArduinoManager::ArduinoManager(QObject *parent) : QObject(parent), connected(false)
{
    serialPort = new QSerialPort(this);

    // Connecter les signaux et slots pour la communication série
    connect(serialPort, &QSerialPort::readyRead, this, &ArduinoManager::readSerialData);
    connect(serialPort, &QSerialPort::errorOccurred, this, &ArduinoManager::handleSerialError);
}

ArduinoManager::~ArduinoManager()
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }
    delete serialPort;
}

bool ArduinoManager::connectToArduino()
{
    // Recherche automatique du port Arduino
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        // Vérifier si c'est probablement un Arduino (peut varier selon les systèmes)
        if ((info.hasVendorIdentifier() && info.vendorIdentifier() == 0x2341) ||  // Arduino
            (info.hasProductIdentifier() && info.description().contains("Arduino", Qt::CaseInsensitive)) ||
            (info.manufacturer().contains("Arduino", Qt::CaseInsensitive))) {

            return connectToPort(info.portName());
        }
    }

    emit errorOccurred("Aucun Arduino détecté. Veuillez connecter votre Arduino.");
    return false;
}

QStringList ArduinoManager::availablePorts() const
{
    QStringList portList;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        portList << info.portName() + " - " + info.description();
    }
    return portList;
}

bool ArduinoManager::connectToPort(const QString &portName)
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }

    serialPort->setPortName(portName);
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadWrite)) {
        connected = true;
        emit connectionStatusChanged(true);

        // Attendre que l'Arduino soit prêt après la connexion
        QTimer::singleShot(2000, [this]() {
            // Envoyer une commande de test
            serialPort->write("HELLO\n");
        });

        return true;
    } else {
        connected = false;
        emit connectionStatusChanged(false);
        emit errorOccurred("Erreur lors de la connexion au port " + portName + ": " + serialPort->errorString());
        return false;
    }
}

void ArduinoManager::disconnectFromArduino()
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }
    connected = false;
    emit connectionStatusChanged(false);
}

bool ArduinoManager::isConnected() const
{
    return connected && serialPort->isOpen();
}

void ArduinoManager::readSerialData()
{
    if (!serialPort->isOpen()) {
        return;
    }

    // Lire les données du port série
    const QByteArray data = serialPort->readAll();
    currentBuffer += QString::fromUtf8(data);

    // Traiter les lignes complètes
    int lineEnd;
    while ((lineEnd = currentBuffer.indexOf('\n')) != -1) {
        QString line = currentBuffer.left(lineEnd).trimmed();
        currentBuffer = currentBuffer.mid(lineEnd + 1);

        qDebug() << "Données reçues de l'Arduino:" << line;

        // Traiter les commandes
        if (line.startsWith("COULEUR:")) {
            QString color = line.mid(8).trimmed();
            emit colorDetected(color);

            // Vérifier l'expiration du vaccin pour cette couleur
            checkVaccinExpiration(color);
        }
        else if (line == "DEMARRAGE") {
            qDebug() << "Arduino démarré et prêt";
        }
    }
}

void ArduinoManager::handleSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::NoError) {
        return;
    }

    QString errorMessage = "Erreur de communication série: ";

    switch (error) {
    case QSerialPort::DeviceNotFoundError:
        errorMessage += "Périphérique non trouvé";
        break;
    case QSerialPort::PermissionError:
        errorMessage += "Permission refusée";
        break;
    case QSerialPort::OpenError:
        errorMessage += "Impossible d'ouvrir le périphérique";
        break;
    case QSerialPort::ResourceError:
        errorMessage += "Périphérique déconnecté";
        disconnectFromArduino();
        break;
    default:
        errorMessage += serialPort->errorString();
    }

    emit errorOccurred(errorMessage);
}
void ArduinoManager::sendCommand(const QString &command)
{
    if (!isConnected() || !serialPort->isOpen()) {
        return;
    }

    // Envoyer la commande à l'Arduino
    serialPort->write(command.toUtf8());
    serialPort->write("\n");
}

// Méthode améliorée pour vérifier l'expiration du vaccin
void ArduinoManager::checkVaccinExpiration(const QString &color)
{
    // Utiliser notre méthode pour obtenir les informations du vaccin
    QMap<QString, QVariant> vaccinData = getVaccinByColor(color);

    if (!vaccinData.isEmpty()) {
        // Extraire les informations du vaccin
        QString vaccinName = vaccinData["NOM"].toString();
        QString reference = vaccinData["REFERENCE"].toString();
        QString expirationDate = vaccinData["DATE_EXPIRATION"].toString();
        bool isExpired = vaccinData["EST_EXPIRE"].toBool();

        // Envoyer la commande appropriée à l'Arduino
        if (isExpired) {
            sendCommand("EXPIRE");
        } else {
            sendCommand("VALIDE");
        }

        // Émettre un signal avec les informations du vaccin
        emit vaccinExpirationStatus(isExpired, vaccinName, reference, expirationDate);
    } else {
        // Aucun vaccin trouvé avec cette couleur
        emit errorOccurred("Aucun vaccin trouvé avec la couleur: " + color);
    }
}

// Méthode pour récupérer les informations d'un vaccin par sa couleur
QMap<QString, QVariant> ArduinoManager::getVaccinByColor(const QString &color)
{
    QMap<QString, QVariant> vaccinData;

    // Créer une connexion à la base de données
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid() || !db.isOpen()) {
        emit errorOccurred("Erreur de connexion à la base de données");
        return vaccinData;
    }

    QSqlQuery query(db);

    // Adapter cette requête à votre structure de base de données
    // Assurez-vous que les noms de colonnes correspondent à votre schéma
    QString queryStr =
        "SELECT ID_VACCIN, NOMVACCIN as NOM, REFERENCE, COULEUR, "
        "TO_CHAR(DATEEXP, 'DD/MM/YYYY') as DATE_EXPIRATION, "
        "CASE WHEN DATEEXP < SYSDATE THEN 1 ELSE 0 END as EST_EXPIRE "
        "FROM vaccins "
        "WHERE UPPER(COULEUR) = UPPER(:COLOR)";

    query.prepare(queryStr);
    query.bindValue(":COLOR", color);

    if (query.exec() && query.next()) {
        vaccinData["ID_VACCIN"] = query.value("ID_VACCIN");
        vaccinData["NOM"] = query.value("NOM");
        vaccinData["REFERENCE"] = query.value("REFERENCE");
        vaccinData["COULEUR"] = query.value("COULEUR");
        vaccinData["DATE_EXPIRATION"] = query.value("DATE_EXPIRATION");
        vaccinData["EST_EXPIRE"] = query.value("EST_EXPIRE").toInt() == 1;

        qDebug() << "Vaccin trouvé pour la couleur" << color << ":" << vaccinData["NOM"].toString();
    }

    return vaccinData;
}
