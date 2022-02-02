#include "availableserialdevicesmodel.h"

#include <QSerialPort>
#include <QDebug>

AvailableSerialDevicesModel::AvailableSerialDevicesModel(QObject *parent) : QAbstractListModel(parent)
{

}

int AvailableSerialDevicesModel::rowCount(const QModelIndex &parent) const
{
    return deviceList.length();
}

QVariant AvailableSerialDevicesModel::data(const QModelIndex &index, int role) const
{
    if (index.column() == 0 && index.row() >= 0 && index.row() < deviceList.length()) {
        switch (role) {
        case Qt::DisplayRole:
            return deviceList[index.row()].description();
        case Qt::ToolTipRole:
            return deviceList[index.row()].manufacturer();
        case Qt::UserRole:
            return deviceList[index.row()].portName();
        default:
            break;
        }
    }

    return QVariant();
}

QVariant AvailableSerialDevicesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return tr("Devices");
}

// TODO: replace model reset with only updating what has changed between old and new list
void AvailableSerialDevicesModel::updateDeviceList()
{
    beginResetModel();

    deviceList.clear();
    const QList<QSerialPortInfo> devices = QSerialPortInfo::availablePorts();
    for (int i=0; i<devices.length(); i++) {
        deviceList.append(devices[i]);

        qDebug() << devices[i].description()
                 << devices[i].manufacturer()
                 << devices[i].portName()
                 << devices[i].productIdentifier()
                 << devices[i].vendorIdentifier();
    }

    endResetModel();
}

