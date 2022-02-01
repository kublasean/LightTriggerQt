#ifndef AVAILABLEDEVICESMODEL_H
#define AVAILABLEDEVICESMODEL_H

#include <QAbstractListModel>
#include <QSerialPortInfo>

class AvailableDevicesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    AvailableDevicesModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public slots:
    void updateDeviceList();

private:
    QList<QSerialPortInfo> deviceList;
};

#endif // AVAILABLEDEVICESMODEL_H
