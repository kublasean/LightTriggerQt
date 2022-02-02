#include "windowsmididevicesmodel.h"
#include "Windows.h"
#include <QDebug>

WindowsMidiDevicesModel::WindowsMidiDevicesModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

int WindowsMidiDevicesModel::rowCount(const QModelIndex &) const
{
    return 0;
}

QVariant WindowsMidiDevicesModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

QVariant WindowsMidiDevicesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

void WindowsMidiDevicesModel::updateDeviceList()
{
    int numDevices = midiInGetNumDevs()+1; // TODO remove error test +1
    MIDIINCAPS devInfo;

    qDebug() << "MIDI Devices:" << numDevices;
    for (int i=0; i<numDevices; i++) {
        MMRESULT err = midiInGetDevCapsW(i, &devInfo, sizeof(MIDIINCAPS));
        if (err != MMSYSERR_NOERROR) {
            wchar_t errBuf[MAXERRORLENGTH];
            if (midiInGetErrorTextW(err, errBuf, MAXERRORLENGTH) == MMSYSERR_NOERROR) {
                qDebug() << "Device capability read error:" << QString::fromWCharArray(errBuf);
            } else {
                qDebug() << "Unknown MIDI device capability read error";
            }
            continue;
        }
        qDebug() << "MIDI Device" << i << Qt::endl
                 << "Manufacturer ID:" << devInfo.wMid << Qt::endl
                 << "Product ID:" << devInfo.wPid << Qt::endl
                 << "Product Name:" << QString::fromWCharArray(devInfo.szPname) << Qt::endl
                 << "Driver version:" << ((devInfo.vDriverVersion >> 4) & 0xFF) << "." << (devInfo.vDriverVersion & 0xFF) << Qt::endl;
    }
}


