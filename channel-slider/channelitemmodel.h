#ifndef CHANNELITEMMODEL_H
#define CHANNELITEMMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "models/fixturedetails.h"

struct ChannelUserData {
    QString name;
    int channel;
    int value;
    bool enabled;
    bool isHeader;
    QString fixtureId;

    ChannelUserData() {
        name = "";
        channel = -1;
        value = 0;
        enabled = false;
        isHeader = false;
        fixtureId = -1;
    }

    ChannelUserData(QString fixtureName) {
        isHeader = true;
        name = fixtureName;
        value = -1;
        enabled = true;
        fixtureId = fixtureName;
        channel = -1;
    }
};

Q_DECLARE_METATYPE(ChannelUserData);

class ChannelItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ChannelItemModel(QObject *parent = nullptr);

    Qt::DropActions supportedDropActions() const override;

    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    QStringList mimeTypes() const override;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int, Qt::Orientation, int = Qt::DisplayRole) const override { return QVariant(); }

    int rowCount(const QModelIndex &) const override { return channels.count(); }

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool removeFixture(int startDataPos);


private:
    QVector<ChannelUserData> channels;
    QMap<QString, ActiveFixture> placedFixtures;
};

#endif // CHANNELITEMMODEL_H
