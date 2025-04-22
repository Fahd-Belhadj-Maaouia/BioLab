#include "serialmanager.h"
#include <QDebug>

SerialManager::SerialManager(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &SerialManager::readSerialData);
}

SerialManager::~SerialManager()
{
    disconnectPort();
}

bool SerialManager::connectToPort(const QString &portName, int baudRate)
{
    if (serial->isOpen()) serial->close();

    serial->setPortName(portName);
    serial->setBaudRate(baudRate);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadOnly)) {
        qDebug() << "Connected to" << portName;
        return true;
    } else {
        qDebug() << "Failed to open port";
        return false;
    }
}

void SerialManager::disconnectPort()
{
    if (serial->isOpen())
        serial->close();
}

QStringList SerialManager::availablePorts()
{
    QStringList portList;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        portList << info.portName();
    }
    return portList;
}

void SerialManager::readSerialData()
{
    QByteArray data = serial->readAll();
    for (char c : data) {
        if (c != '\n' && c != '\r') {
            emit keyReceived(c);
        }
    }
}

