#ifndef TRIGGEREFFECTMODEL_H
#define TRIGGEREFFECTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QMap>
#include <QColor>

struct TriggerDetails {
    QString nickname;
    QColor color;
    bool active;
    int listIndex;
};

class TriggerEffectModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TriggerEffectModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public slots:
    void onMidiNote(int note, int velocity);

private:
    QList<int> triggerList;
    QMap<int, TriggerDetails> triggerMap;
};

#endif // TRIGGEREFFECTMODEL_H
