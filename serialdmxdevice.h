#ifndef SERIALDMXDEVICE_H
#define SERIALDMXDEVICE_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>
#include <QTimer>

class SerialDmxDevice : public QObject
{
    Q_OBJECT
public:
    explicit SerialDmxDevice(QObject *parent = nullptr);
    ~SerialDmxDevice();

public slots:
    void init();
    void newSelectedDevice(QString portName);
    void start();
    void stop();

signals:
    void debugMessage(QString message, int timeout=0);
    void isConnected(bool status);

private slots:
    void onError(QSerialPort::SerialPortError error);
    void sendData();

private:
    bool isValid;
    QSerialPort *dev;
    QByteArray buffer;
    QTimer *sendTimer;
};

#endif // SERIALDMXDEVICE_H
