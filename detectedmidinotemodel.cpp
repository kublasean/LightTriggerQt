#include "detectedmidinotemodel.h"

DetectedMidiNoteModel::DetectedMidiNoteModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

bool DetectedMidiNoteModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex noteIndex = sourceModel()->index(sourceRow, 0, sourceParent);
    return sourceModel()->data(noteIndex, Qt::UserRole).toBool();
}

bool DetectedMidiNoteModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    return left.row() < right.row();
}
