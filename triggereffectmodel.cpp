#include "triggereffectmodel.h"
#include <QDebug>

#define NUM_MIDI_NOTES 128

QStringList TriggerEffectModel::noteNames = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};

QString TriggerEffectModel::getNoteName(int note)
{
    return noteNames.at(note % 12) + QString::number(((note / 12) - 1));
}

QString TriggerEffectModel::getDetailedNoteName(int note)
{
    return getNoteName(note) + " (" + QString::number(note) + ")";
}

TriggerEffectModel::TriggerEffectModel(QObject *parent)
    : QAbstractListModel(parent)
{
    noteMap.resize(NUM_MIDI_NOTES);
    for (int i=0; i<NUM_MIDI_NOTES; i++) {
        noteMap[i].active = false;
        noteMap[i].detected = false;
        noteMap[i].color = QColor::fromRgb(0, 0, 0);
        noteMap[i].nickname = getDetailedNoteName(i);
    }
}

Qt::ItemFlags TriggerEffectModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

int TriggerEffectModel::rowCount(const QModelIndex &) const
{
    return detectedNotesList.length();
}

QVariant TriggerEffectModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index)) {
        return QVariant();
    }

    int note = detectedNotesList[index.row()];
    const TriggerDetails &details = noteMap[note];

    switch (role) {
    case Qt::EditRole:
    case Qt::DisplayRole:
        return details.nickname;
    case Qt::ToolTipRole:
        return getDetailedNoteName(note);
    case Qt::UserRole:
        return details.color;
    default:
        return QVariant();
    }
}

bool TriggerEffectModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!checkIndex(index))
        return false;

    int note = detectedNotesList[index.row()];

    switch (role) {
    case Qt::EditRole:
        noteMap[note].nickname = value.toString();
        break;
    case Qt::UserRole:
        noteMap[note].active = true;
        noteMap[note].color = value.value<QColor>();
        break;
    default:
        return false;
    }

    emit dataChanged(index, index, {role});
    return true;
}

QVariant TriggerEffectModel::headerData(int, Qt::Orientation, int) const
{
    return tr("Triggers");
}

void TriggerEffectModel::onMidiNote(int note, int velocity)
{
    if (note < 0 || note > 127)
        return;

    // First time we've seen this note, add it to the list
    if (!noteMap[note].detected) {

        // Find where we will insert the new note in the sorted list
        // TODO: find some easy way to do a binary search here?
        int i, pos;
        for (i=detectedNotesList.length()-1; i>=0; i--) {
            if (note >= detectedNotesList[i])
                break;
        }
        pos = i+1;
        qDebug() << "insert at" << pos;

        beginInsertRows(QModelIndex(), pos, pos);

        noteMap[note].detected = true;
        detectedNotesList.insert(pos, note);

        endInsertRows();
        return;
    }

    if (noteMap[note].active)
        emit sendColor(velocity == 0 ? QColor::fromRgb(0,0,0) : noteMap[note].color);
}


