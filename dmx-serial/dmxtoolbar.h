#ifndef DMXTOOLBAR_H
#define DMXTOOLBAR_H

#include <QToolBar>
#include <QToolButton>

#include "serialdmxdevice.h"
#include "availableserialdevicesmodel.h"

class DmxToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit DmxToolBar(SerialDmxDevice *dmx, const QString &title, QWidget *parent = nullptr);

signals:
    void newSerialDmxDevice(QString portName);

private slots:
    void dmxDeviceSelected(int row);
    void onDmxStatus(bool isConnected);

private:
    QToolButton *statusIcon;
    QAction *disconnectDeviceAction, *connectDeviceAction;
    AvailableSerialDevicesModel serialDevices;

};

#endif // DMXTOOLBAR_H
