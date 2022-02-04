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

int TriggerEffectModel::rowCount(const QModelIndex &) const
{
    return detectedNotesList.length();
}

QVariant TriggerEffectModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= detectedNotesList.length()) {
        return QVariant();
    }

    int note = detectedNotesList[index.row()];
    const TriggerDetails &details = noteMap[note];

    switch (role) {
    case Qt::DisplayRole:
        return details.nickname;
    case Qt::ToolTipRole:
        return getDetailedNoteName(note);
    default:
        return QVariant();
    }
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


