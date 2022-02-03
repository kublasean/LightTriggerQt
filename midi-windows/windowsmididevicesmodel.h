#ifndef WINDOWSMIDIDEVICESMODEL_H
#define WINDOWSMIDIDEVICESMODEL_H

#include <QObject>
#include <QAbstractListModel>

struct MidiDevInfo {
    int devId;
    int manufacturerId;
    int productId;
    QString productName;
};

class WindowsMidiDevicesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WindowsMidiDevicesModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

signals:

public slots:
    void updateDeviceList();

private:
    QList<MidiDevInfo> deviceList;

};

#endif // WINDOWSMIDIDEVICESMODEL_H
