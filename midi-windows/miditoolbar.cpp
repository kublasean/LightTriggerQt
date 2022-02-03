#include "miditoolbar.h"

#include <QComboBox>
#include <QLabel>

MidiToolBar::MidiToolBar(WindowsMidiInputDevice *midi, const QString &title, QWidget *parent)
    : QToolBar(title, parent)
{
    this->midi = midi;

    // Available MIDI device dropdown
    QComboBox *deviceDropDown = new QComboBox();
    deviceDropDown->setModel(&midiDevices);
    addWidget(new QLabel(tr("MIDI Device ")));
    addWidget(deviceDropDown);
    connect(deviceDropDown, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MidiToolBar::deviceSelected);
    connect(midi, &WindowsMidiInputDevice::isConnected, deviceDropDown, &QComboBox::setDisabled);

    // Refresh available MIDI device list
    QAction *refreshDeviceAction = addAction(tr("Refresh"));
    refreshDeviceAction->setShortcut(tr("Ctrl+R"));
    connect(refreshDeviceAction, &QAction::triggered, &midiDevices, &WindowsMidiDevicesModel::updateDeviceList);
    connect(midi, &WindowsMidiInputDevice::isConnected, refreshDeviceAction, &QAction::setDisabled);

    addSeparator();

    // Connection/disconnection from selected serial port
    connectDeviceAction = addAction(tr("Connect"));
    disconnectDeviceAction = addAction(tr("Disconnect"));
    connect(connectDeviceAction, &QAction::triggered, midi, &WindowsMidiInputDevice::start);
    connect(disconnectDeviceAction, &QAction::triggered, midi, &WindowsMidiInputDevice::stop);
    connect(midi, &WindowsMidiInputDevice::isConnected, connectDeviceAction, &QAction::setDisabled);
    connect(midi, &WindowsMidiInputDevice::isConnected, disconnectDeviceAction, &QAction::setEnabled);

    midiDevices.updateDeviceList();
}

void MidiToolBar::deviceSelected(int row)
{
    bool noDevices = row == -1;
    disconnectDeviceAction->setDisabled(noDevices);
    connectDeviceAction->setDisabled(noDevices);

    if (!noDevices) {
        midi->setDevId(midiDevices.data(midiDevices.index(row), Qt::UserRole).toInt());
    }
}
