#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialManager(QObject *parent = nullptr);
    ~SerialManager();

    bool connectToArduino();
    bool isConnected() const;
    void sendCommand(const QString &command);

signals:
    void cardScanned(const QString &cardUID);

private slots:
    void handleReadyRead();

private:
    QSerialPort *serial;
};

#endif // SERIALMANAGER_H
