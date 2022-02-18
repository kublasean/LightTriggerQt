#include "devicesdialog.h"
#include "ui_devicesdialog.h"

#include "windows.h"

#include <QDebug>
#include <QSerialPortInfo>
#include <QSettings>

void printMMRESULT(MMRESULT err)
{
    wchar_t errBuf[MAXERRORLENGTH];
    if (midiInGetErrorTextW(err, errBuf, MAXERRORLENGTH) == MMSYSERR_NOERROR) {
        qDebug() << "MIDI error:" << QString::fromWCharArray(errBuf);
    } else {
        qDebug() << "Unknown MIDI error";
    }
}

DevicesDialog::DevicesDialog(SerialDmxDevice *dmx, WindowsMidiInputDevice *midi, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DevicesDialog)
{
    ui->setupUi(this);
    ui->serialPortDevices->setEditable(false);

    connect(ui->serialPortDevices, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DevicesDialog::onSelectedSerialPort);
    connect(this, &DevicesDialog::newSelectedSerialPortName, dmx, &SerialDmxDevice::newSelectedDevice);

    connect(ui->midiDevices, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DevicesDialog::onSelectedMidiDevice);
    connect(this, &DevicesDialog::newSelectedMidiProduct, midi, &WindowsMidiInputDevice::switchDevice);

    updateSerialPorts();
    updateMidiDevices();
}

DevicesDialog::~DevicesDialog()
{
    delete ui;
}

void DevicesDialog::updateSerialPorts()
{
    QSettings settings;
    QString lastSelected = settings.value("dmxSerialPort", QString()).toString();
    int lastSelectedRow = 0;

    qDebug() << "last selected:" << lastSelected;

    ui->serialPortDevices->blockSignals(true);      // prevent item changed signal until we've added all items
    ui->serialPortDevices->clear();
    ui->serialPortDevices->addItem(tr("No device"), QString());

    const QList<QSerialPortInfo> devices = QSerialPortInfo::availablePorts();
    for (int i=0; i<devices.length(); i++) {
        ui->serialPortDevices->addItem(devices[i].description(), devices[i].portName());
        qDebug() << devices[i].description()
                 << devices[i].manufacturer()
                 << devices[i].portName()
                 << devices[i].productIdentifier()
                 << devices[i].vendorIdentifier();

        if (lastSelected == devices[i].portName()) {
            lastSelectedRow = i+1; // plus 1 since we added "No device" as the first element
        }
    }

    ui->serialPortDevices->blockSignals(false);
    ui->serialPortDevices->setCurrentIndex(lastSelectedRow);
}

void DevicesDialog::updateMidiDevices()
{
    QSettings settings;
    QString lastSelected = settings.value("midiProductName", QString()).toString();
    int lastSelectedRow = 0;

    ui->midiDevices->blockSignals(true);
    ui->midiDevices->clear();
    ui->midiDevices->addItem(tr("No device"), -1);

    int numDevices = midiInGetNumDevs();
    MIDIINCAPS devInfo;
    qDebug() << "MIDI Devices:" << numDevices;

    for (int i=0; i<numDevices; i++) {
        MMRESULT err = midiInGetDevCapsW(i, &devInfo, sizeof(MIDIINCAPS));
        if (err != MMSYSERR_NOERROR) {
            printMMRESULT(err);
            continue;
        }

        QString productName = QString::fromWCharArray(devInfo.szPname);
        ui->midiDevices->addItem(productName, i);

        if (lastSelected == productName)
            lastSelectedRow = i+1;

        /*qDevInfo.devId = i;
        qDevInfo.manufacturerId = devInfo.wMid;
        qDevInfo.productId = devInfo.wPid;
        qDevInfo.productName = QString::fromWCharArray(devInfo.szPname);*/
    }

    ui->midiDevices->blockSignals(false);
    ui->midiDevices->setCurrentIndex(lastSelectedRow);
}

void DevicesDialog::onSelectedSerialPort(int row)
{
    if (row == -1)
        return;

    QString portName = ui->serialPortDevices->model()->data(ui->serialPortDevices->model()->index(row, 0), Qt::UserRole).toString();
    QSettings settings;
    settings.setValue("dmxSerialPort", portName);
    emit newSelectedSerialPortName(portName);
}

void DevicesDialog::onSelectedMidiDevice(int row)
{
    if (row == -1)
        return;

    auto index = ui->midiDevices->model()->index(row, 0);
    QString productName = ui->midiDevices->model()->data(index, Qt::DisplayRole).toString();
    int midiIndex = ui->midiDevices->model()->data(index, Qt::UserRole).toInt();

    QSettings settings;
    settings.setValue("midiProductName", productName);
    emit newSelectedMidiProduct(midiIndex);
}
