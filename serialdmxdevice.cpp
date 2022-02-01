#include "serialdmxdevice.h"
#include <QDebug>

SerialDmxDevice::SerialDmxDevice(QObject *parent)
    : QObject{parent}
{
    isValid = false;
    buffer.fill(0xFF, 513);
    buffer[0] = 0x00;   // One byte start code
}

SerialDmxDevice::~SerialDmxDevice()
{
    if (sendTimer != nullptr) {
        sendTimer->stop();
    }
    if (dev != nullptr && dev->isOpen()) {
        dev->close();
    }
    qDebug() << "SerialDmxDevice destructor fin";
}

void SerialDmxDevice::init()
{
    dev = new QSerialPort(this);
    isValid = true;

    // https://en.wikipedia.org/wiki/DMX512#Protocol
    dev->setBaudRate(250000);
    dev->setDataBits(QSerialPort::Data8);
    dev->setParity(QSerialPort::NoParity);
    dev->setFlowControl(QSerialPort::NoFlowControl);
    dev->setStopBits(QSerialPort::TwoStop);
    connect(dev, &QSerialPort::errorOccurred, this, &SerialDmxDevice::onError);

    sendTimer = new QTimer(this);
    sendTimer->setInterval(20);
    connect(sendTimer, &QTimer::timeout, this, &SerialDmxDevice::sendData);
}

void SerialDmxDevice::start()
{
    if (!isValid) {
        emit debugMessage(tr("Serial port was not initialized before connecting"));
        return;
    }

    sendTimer->start();
    if (dev->isOpen()) {
        emit isConnected(true);
        return;
    }

    bool didOpen = dev->open(QIODevice::WriteOnly);
    if (!didOpen) {
        emit debugMessage(dev->errorString());
    }
    emit isConnected(didOpen);
}

void SerialDmxDevice::stop()
{
    if (!isValid) {
        return;
    }

    sendTimer->stop();
    if (dev->isOpen()) {
        dev->close();
    }

    emit isConnected(false);
}

void SerialDmxDevice::newSelectedDevice(QString portName)
{
    if (!isValid) {
        return;
    }

    if (dev->isOpen()) {
        stop();
    }

    dev->setPortName(portName);
}

void SerialDmxDevice::sendData()
{
    if (!isValid) {
        emit debugMessage(tr("Cannot write to invalid device"));
        return;
    }

    dev->setBreakEnabled(true);
    dev->setBreakEnabled(false);
    dev->write(buffer);

    if (!dev->waitForBytesWritten(1000)) {
        qDebug() << "Synch error";
    }
}

void SerialDmxDevice::onError(QSerialPort::SerialPortError error)
{
    qDebug() << error << dev->errorString();
}
