#include "serialmanager.h"
#include <QDebug>

SerialManager::SerialManager(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &SerialManager::handleReadyRead);
}

SerialManager::~SerialManager()
{
    if(serial->isOpen()) {
        serial->close();
    }
    delete serial;
}

bool SerialManager::connectToArduino()
{
    qDebug() << "Scanning available serial ports...";
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Port:" << info.portName()
        << "Description:" << info.description()
        << "Manufacturer:" << info.manufacturer();

        if (info.description().contains("Arduino", Qt::CaseInsensitive) ||
            info.manufacturer().contains("Arduino", Qt::CaseInsensitive) ||
            info.portName().startsWith("COM")) {

            serial->setPort(info);
            if (serial->open(QIODevice::ReadWrite)) {
                serial->setBaudRate(QSerialPort::Baud9600);
                serial->setDataBits(QSerialPort::Data8);
                serial->setParity(QSerialPort::NoParity);
                serial->setStopBits(QSerialPort::OneStop);
                serial->setFlowControl(QSerialPort::NoFlowControl);
                qDebug() << "Connected to Arduino on" << info.portName();
                return true;
            } else {
                qDebug() << "Failed to open port:" << serial->errorString();
            }
        }
    }

    qDebug() << "Could not find or connect to Arduino";
    return false;
}


void SerialManager::handleReadyRead() {
    while (serial->canReadLine()) {
        QString line = QString::fromUtf8(serial->readLine()).trimmed();
        qDebug() << "Raw serial data:" << line;  // Debug log



        // Handle both formats for robustness:
        if (line.startsWith("UID tag:")) {         // Format "UID tag:73 7A 5D 29"
            QString cardUID = line.mid(8).trimmed();
            emit cardScanned(cardUID);
        }
        else if (line.startsWith("UID tag : ")) {  // Format "UID tag : 73 7A 5D 29"
            QString cardUID = line.mid(10).trimmed();
            emit cardScanned(cardUID);
        }
    }
}

bool SerialManager::isConnected() const
{
    return serial && serial->isOpen();
}

void SerialManager::sendCommand(const QString &command)
{
    if(isConnected()) {
        serial->write(command.toUtf8());
        serial->flush();
        qDebug() << "Command sent to Arduino:" << command;
    }
}
