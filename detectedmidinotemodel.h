#ifndef DETECTEDMIDINOTEMODEL_H
#define DETECTEDMIDINOTEMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>

class DetectedMidiNoteModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit DetectedMidiNoteModel(QObject *parent = nullptr);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

#endif // DETECTEDMIDINOTEMODEL_H
