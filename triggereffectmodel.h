#ifndef TRIGGEREFFECTMODEL_H
#define TRIGGEREFFECTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QMap>
#include <QColor>

struct TriggerDetails {
    QString nickname;
    QColor color;       // color to send when triggered
    bool active;        // is the trigger enabled
    bool detected;      // have we seen this MIDI note before
};

class TriggerEffectModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TriggerEffectModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    static QString getNoteName(int note);
    static QString getDetailedNoteName(int note);

public slots:
    void onMidiNote(int note, int velocity);

signals:
    void sendColor(const QColor &color);

private:
    QVector<int> detectedNotesList;
    QVector<TriggerDetails> noteMap;

    static QStringList noteNames;
};

#endif // TRIGGEREFFECTMODEL_H
