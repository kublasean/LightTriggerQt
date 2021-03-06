#ifndef SERIALDMXDEVICE_H
#define SERIALDMXDEVICE_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>
#include <QTimer>
#include <QColor>

class SerialDmxDevice : public QObject
{
    Q_OBJECT
public:
    explicit SerialDmxDevice(QString initialPortName = QString(), QObject *parent = nullptr);
    ~SerialDmxDevice();

public slots:
    void init();
    void newSelectedDevice(QString portName);
    void start();
    void stop();

    void newChannelValues(const QVector<QPair<int,int> > &values);

signals:
    void debugMessage(QString message, int timeout=0);
    void isConnected(bool status);

private slots:
    void onError(QSerialPort::SerialPortError error);
    void sendData();

private:
    const QString initialPortName;
    bool isValid;
    QSerialPort *dev;
    QByteArray buffer;
    QTimer *sendTimer;
};

#endif // SERIALDMXDEVICE_H
