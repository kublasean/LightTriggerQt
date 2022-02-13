#ifndef MIDINOTEMODEL_H
#define MIDINOTEMODEL_H

#define NUM_MIDI_NOTES 128

#include <QAbstractListModel>
#include <QObject>
#include <QMap>
#include <QColor>

struct MidiNoteEffect {
    QString nickname;
    QColor color;       // color to send when triggered
    bool active;        // is the effect enabled
    bool detected;      // have we seen this MIDI note before
    bool pressed;       // is the note pressed
};

Q_DECLARE_METATYPE(MidiNoteEffect);

class MidiNoteModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MidiNoteModel(QObject *parent = nullptr);

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
    QVector<MidiNoteEffect> noteMap;
    QColor highlightColor;

    static QStringList noteNames;
};

#endif // MIDINOTEMODEL_H
