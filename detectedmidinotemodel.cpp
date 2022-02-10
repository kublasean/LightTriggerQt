#include "detectedmidinotemodel.h"

#include "midinotemodel.h"

DetectedMidiNoteModel::DetectedMidiNoteModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

bool DetectedMidiNoteModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex noteIndex = sourceModel()->index(sourceRow, 0, sourceParent);

    MidiNoteEffect details = qvariant_cast<MidiNoteEffect>(sourceModel()->data(noteIndex, Qt::UserRole));
    return details.detected;
}

bool DetectedMidiNoteModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    return left.row() < right.row();
}
