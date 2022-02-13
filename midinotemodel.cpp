#include "midinotemodel.h"
#include <QDebug>
#include <QBrush>
#include <QApplication>
#include <QPalette>

QStringList MidiNoteModel::noteNames = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};

QString MidiNoteModel::getNoteName(int note)
{
    return noteNames.at(note % 12) + QString::number(((note / 12) - 1));
}

QString MidiNoteModel::getDetailedNoteName(int note)
{
    return getNoteName(note) + " (" + QString::number(note) + ")";
}

MidiNoteModel::MidiNoteModel(QObject *parent)
    : QAbstractListModel(parent)
{
    noteMap.resize(NUM_MIDI_NOTES);
    for (int i=0; i<NUM_MIDI_NOTES; i++) {
        noteMap[i].active = false;
        noteMap[i].detected = false;
        noteMap[i].color = QColor::fromRgb(0, 0, 0, 0);
        noteMap[i].nickname = "";
        noteMap[i].pressed = false;
    }

    highlightColor = qApp->palette().color(QPalette::Highlight);
}

Qt::ItemFlags MidiNoteModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

int MidiNoteModel::rowCount(const QModelIndex &) const
{
    return noteMap.length();
}

QVariant MidiNoteModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index)) {
        return QVariant();
    }

    int note = index.row();
    const MidiNoteEffect &details = noteMap[note];

    switch (role) {
    case Qt::EditRole:
        return details.nickname;
    case Qt::DisplayRole:
        return getNoteName(note) + " " + details.nickname;
    case Qt::ToolTipRole:
        return tr("MIDI note number: ") + QString::number(note);
    case Qt::UserRole:
    {
        QVariant tmp;
        tmp.setValue(details);
        return tmp;
    }
    case Qt::BackgroundRole:
        return details.pressed ? highlightColor : QVariant();
    default:
        return QVariant();
    }
}

bool MidiNoteModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!checkIndex(index))
        return false;

    int note = index.row();

    switch (role) {
    case Qt::EditRole:
        noteMap[note].nickname = value.toString();
        break;
    case Qt::DecorationRole:
        noteMap[note].active = true;
        noteMap[note].color = value.value<QColor>();
        break;
    case Qt::BackgroundRole:
        noteMap[note].pressed = value.toBool();
        break;
    default:
        return false;
    }

    emit dataChanged(index, index, {role});
    return true;
}

QVariant MidiNoteModel::headerData(int, Qt::Orientation, int) const
{
    return QVariant();
}

void MidiNoteModel::onMidiNote(int note, int velocity)
{
    QModelIndex noteIndex = index(note);
    if (!checkIndex(index(note)))
        return;

    noteMap[note].detected = true;
    noteMap[note].pressed = velocity != 0;
    emit dataChanged(noteIndex, noteIndex, {Qt::UserRole, Qt::BackgroundRole});


    if (noteMap[note].active)
        emit sendColor(noteMap[note].pressed ? noteMap[note].color : QColor::fromRgb(0,0,0));
}


