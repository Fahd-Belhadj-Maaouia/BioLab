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

    bool connectToPort(const QString &portName, int baudRate = QSerialPort::Baud9600);
    void disconnectPort();
    QStringList availablePorts();

signals:
    void keyReceived(char key);

private slots:
    void readSerialData();

private:
    QSerialPort *serial;
};

#endif // SERIALMANAGER_H
