// ArduinoManager.h
#ifndef ARDUINOMANAGER_H
#define ARDUINOMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QDateTime>
#include <QDebug>

class ArduinoManager : public QObject
{
    Q_OBJECT
public:
    explicit ArduinoManager(QObject *parent = nullptr);
    ~ArduinoManager();

    QStringList availablePorts() const;
    bool connectToPort(const QString &portName);
    bool connectToArduino();
    void disconnectFromArduino();
    bool isConnected() const;

    // Méthode pour envoyer une commande à l'Arduino
    void sendCommand(const QString &command);

signals:
    void connectionStatusChanged(bool connected);
    void colorDetected(const QString &color);
    void vaccinExpirationStatus(bool isExpired, const QString &vaccinName, const QString &reference, const QString &expDate);
    void errorOccurred(const QString &errorMessage);

private slots:
    void readSerialData();
    void handleSerialError(QSerialPort::SerialPortError error);

private:
    QSerialPort *serialPort;
    QString currentBuffer;
    bool connected;

    // Méthode améliorée pour vérifier l'expiration du vaccin
    void checkVaccinExpiration(const QString &color);
    // Obtenir les informations du vaccin depuis la base de données
    QMap<QString, QVariant> getVaccinByColor(const QString &color);
};

#endif // ARDUINOMANAGER_H
