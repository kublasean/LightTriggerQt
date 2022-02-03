#include "dmxtoolbar.h"

#include <QComboBox>
#include <QLabel>

DmxToolBar::DmxToolBar(SerialDmxDevice *dmx, const QString &title, QWidget *parent)
    : QToolBar(title, parent)
{
    connect(this, &DmxToolBar::newSerialDmxDevice, dmx, &SerialDmxDevice::newSelectedDevice);

    // Found serial ports dropdown
    QComboBox *deviceDropDown = new QComboBox();
    deviceDropDown->setModel(&serialDevices);
    addWidget(new QLabel(tr("DMX Device ")));
    addWidget(deviceDropDown);
    connect(deviceDropDown, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DmxToolBar::dmxDeviceSelected);
    connect(dmx, &SerialDmxDevice::isConnected, deviceDropDown, &QComboBox::setDisabled);

    // Refresh available serial ports list
    QAction *refreshDeviceAction = addAction(tr("Refresh"));
    refreshDeviceAction->setShortcut(tr("Ctrl+R"));
    connect(refreshDeviceAction, &QAction::triggered, &serialDevices, &AvailableSerialDevicesModel::updateDeviceList);
    connect(dmx, &SerialDmxDevice::isConnected, refreshDeviceAction, &QAction::setDisabled);

    addSeparator();

    // Connection/disconnection from selected serial port
    connectDeviceAction = addAction(tr("Connect"));
    disconnectDeviceAction = addAction(tr("Disconnect"));
    connect(connectDeviceAction, &QAction::triggered, dmx, &SerialDmxDevice::start);
    connect(disconnectDeviceAction, &QAction::triggered, dmx, &SerialDmxDevice::stop);
    connect(dmx, &SerialDmxDevice::isConnected, connectDeviceAction, &QAction::setDisabled);

    statusIcon = new QToolButton();
    statusIcon->setDisabled(true);
    addWidget(statusIcon);

    serialDevices.updateDeviceList();
}

void DmxToolBar::dmxDeviceSelected(int row)
{
    bool noDevices = row == -1;
    disconnectDeviceAction->setDisabled(noDevices);
    connectDeviceAction->setDisabled(noDevices);

    QIcon icon;
    icon.addFile(":/assets/device-normal/baseline_help_black_24dp.png");
    statusIcon->setIcon(icon);

    if (!noDevices)
        emit newSerialDmxDevice(serialDevices.data(serialDevices.index(row, 0), Qt::UserRole).toString());
}

void DmxToolBar::onDmxStatus(bool isConnected)
{
    QIcon icon;
    if (isConnected) {
        icon.addFile(":/assets/device-active/baseline_check_circle_black_24dp.png");
    } else {
        icon.addFile(":/assets/device-disabled/baseline_cancel_black_24dp.png");
    }
    statusIcon->setIcon(icon);
    disconnectDeviceAction->setDisabled(!isConnected);
}
