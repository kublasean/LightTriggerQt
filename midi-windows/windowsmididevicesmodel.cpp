#include "windowsmididevicesmodel.h"
#include "windowsmidiutil.h"
#include "Windows.h"
#include <QDebug>

WindowsMidiDevicesModel::WindowsMidiDevicesModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

int WindowsMidiDevicesModel::rowCount(const QModelIndex &) const
{
    return deviceList.length();
}

QVariant WindowsMidiDevicesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= deviceList.length())
        return QVariant();

    switch (role) {
    case Qt::ToolTipRole:
        return deviceList[index.row()].productId;
    case Qt::UserRole:
        return deviceList[index.row()].devId;
    case Qt::DisplayRole:
        return deviceList[index.row()].productName;
    default:
        return QVariant();
    }
}

QVariant WindowsMidiDevicesModel::headerData(int, Qt::Orientation , int) const
{
    return tr("MIDI Devices");
}

void WindowsMidiDevicesModel::updateDeviceList()
{
    beginResetModel();
    deviceList.clear();

    int numDevices = midiInGetNumDevs();
    MIDIINCAPS devInfo;

    qDebug() << "MIDI Devices:" << numDevices;
    for (int i=0; i<numDevices; i++) {
        MMRESULT err = midiInGetDevCapsW(i, &devInfo, sizeof(MIDIINCAPS));
        if (err != MMSYSERR_NOERROR) {
            printMMRESULT(err);
            continue;
        }

        MidiDevInfo qDevInfo;
        qDevInfo.devId = i;
        qDevInfo.manufacturerId = devInfo.wMid;
        qDevInfo.productId = devInfo.wPid;
        qDevInfo.productName = QString::fromWCharArray(devInfo.szPname);
        deviceList.append(qDevInfo);
    }

    endResetModel();
}


