#include "channelitemmodel.h"
#include "models/fixturemimedata.h"

#include <QDebug>

ChannelItemModel::ChannelItemModel(QObject *parent)
    : QAbstractListModel{parent}
{
    channels.resize(512);

    for (int i=0; i<512; i++) {
        ChannelUserData userData;
        userData.channel = i;
        userData.enabled = false;
        userData.name = "";
        userData.value = 0;
        userData.fixtureId = -1;
        userData.isHeader = false;
        channels[i] = userData;
    }
}

Qt::ItemFlags ChannelItemModel::flags(const QModelIndex &index) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid)) {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags itemFlags = QAbstractListModel::flags(index) | Qt::ItemIsDropEnabled;

    ChannelUserData userData = qvariant_cast<ChannelUserData>(data(index, Qt::UserRole));

    if (!userData.enabled || userData.isHeader) {
        itemFlags &= ~Qt::ItemIsEnabled;
        itemFlags &= ~Qt::ItemIsSelectable;
    }

    return itemFlags;
}

Qt::DropActions ChannelItemModel::supportedDropActions() const
{
    return Qt::DropAction::CopyAction | Qt::DropAction::MoveAction;
}

QStringList ChannelItemModel::mimeTypes() const
{
    return { FixtureMimeData::channelFormat() };
}

// Ownership is not taken of the QMimeData returned
QMimeData *ChannelItemModel::mimeData(const QModelIndexList &indexes) const
{
    if (!indexes.first().isValid())
        return nullptr;

    int headerRow = indexes.first().row();
    if (!channels[headerRow].isHeader)
        return nullptr;

    QString fixtureId = channels[headerRow].fixtureId;
    if (!placedFixtures.contains(fixtureId))
        return nullptr;

    FixtureMimeData *mimeData = new FixtureMimeData(placedFixtures.value(fixtureId), headerRow);
    return mimeData;
}

bool ChannelItemModel::canDropMimeData(const QMimeData *mimeData, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    if (!checkIndex(parent, CheckIndexOption::IndexIsValid)) {
        return false;
    }

    if (action != Qt::DropAction::CopyAction && action != Qt::MoveAction)
        return false;

    const FixtureMimeData *fixtureMimeData = qobject_cast<const FixtureMimeData *>(mimeData);
    if (fixtureMimeData == nullptr)
        return false;

    for (int i=0; i<fixtureMimeData->fixture.mode.channels.count(); i++) {
        int row = i + parent.row();
        if (row >= rowCount(QModelIndex()))
            return false;

        if (channels[row].enabled || channels[row].isHeader)
            return false;
    }

    return true;
}

bool ChannelItemModel::dropMimeData(const QMimeData *mimeData, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    qDebug() << "DROP AT" << parent.row() << row << column;

    if (!canDropMimeData(mimeData, action, row, column, parent))
        return false;

    const FixtureMimeData *fixtureMimeData = qobject_cast<const FixtureMimeData *>(mimeData);
    Q_ASSERT(fixtureMimeData != nullptr);

    ActiveFixture fixture = fixtureMimeData->fixture;
    while (placedFixtures.contains(fixture.fixtureId())) {
        fixture.id += 1;
    }

    // Add row for the header
    beginInsertRows(parent, parent.row(), parent.row());
    channels.insert(parent.row(), ChannelUserData(fixture.fixtureId()));
    placedFixtures.insert(fixture.fixtureId(), fixture);

    for (int i=0; i<fixture.mode.channels.count(); i++) {
        int row = parent.row() + 1 + i;
        ChannelUserData userData;
        userData.isHeader = false;
        userData.enabled = true;
        userData.channel = channels[row].channel;
        userData.value = 0;
        userData.name = fixture.mode.channels[i].name;
        userData.fixtureId = fixture.fixtureId();
        channels[row] = userData;
    }

    endInsertRows();
    emit dataChanged(index(parent.row()+1), index(parent.row()+fixture.mode.channels.count()));
    return true;
}

bool ChannelItemModel::removeFixture(int startDataPos)
{
    if (!channels[startDataPos].isHeader)
        return false;

    QString fixtureId = channels[startDataPos].fixtureId;

    beginRemoveRows(index(startDataPos), startDataPos, startDataPos);
    channels.remove(startDataPos);

    int i;
    for (i = startDataPos; i < channels.count() && channels[i].fixtureId == fixtureId; i++) {
        channels[i].enabled = false;
        channels[i].value = 0;
        channels[i].name = "";
        channels[i].fixtureId = "";
        channels[i].isHeader = false;
    }

    placedFixtures.remove(fixtureId);

    endRemoveRows();
    emit dataChanged(index(startDataPos), index(i-1));
    return true;
}

bool ChannelItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{   
    if (!checkIndex(index, CheckIndexOption::IndexIsValid)) {
        return false;
    }

    int channel = channels[index.row()].channel;

    switch (role) {
    case Qt::UserRole:
        if (!value.canConvert<ChannelUserData>())
            return false;
        channels[index.row()] = qvariant_cast<ChannelUserData>(value);
        channels[index.row()].channel = channel;
        break;
    case Qt::EditRole:
        channels[index.row()].value = value.toInt();
        break;
    default:
        return false;
    }

    emit dataChanged(index, index, {role});
    return true;
}

QVariant ChannelItemModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid)) {
        return QVariant();
    }

    QVariant ret;

    switch (role) {
    case Qt::UserRole:
        ret.setValue(channels[index.row()]);
        break;
    case Qt::DisplayRole:
        ret = channels[index.row()].name;
        break;
    case Qt::EditRole:
        ret = channels[index.row()].value;
        break;
    default:
        break;
    }

    return ret;
}

