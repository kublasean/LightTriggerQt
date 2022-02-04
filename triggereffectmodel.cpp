#include "triggereffectmodel.h"

TriggerEffectModel::TriggerEffectModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

int TriggerEffectModel::rowCount(const QModelIndex &parent) const
{
    return triggerList.length();
}

QVariant TriggerEffectModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= triggerList.length()) {
        return QVariant();
    }

    int note = triggerList[index.row()];
    const TriggerDetails &details = triggerMap[note];

    switch (role) {
    case Qt::DisplayRole:
        return details.nickname;
    default:
        return QVariant();
    }
}

QVariant TriggerEffectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return tr("Triggers");
}

void TriggerEffectModel::onMidiNote(int note, int velocity)
{
    if (!triggerMap.contains(note)) {
        beginInsertRows(QModelIndex(), triggerList.length(), triggerList.length());

        TriggerDetails details;
        details.active = false;
        details.color = QColor::fromRgb(0, 0, 0);
        details.nickname = QString::number(note);
        details.listIndex = triggerList.length();
        triggerMap.insert(note, details);
        triggerList.append(note);

        endInsertRows();
    }
}


