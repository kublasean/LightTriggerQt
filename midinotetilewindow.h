#ifndef MIDINOTETILEWINDOW_H
#define MIDINOTETILEWINDOW_H

#include <QWindow>
#include <QBackingStore>
#include <QExposeEvent>
#include <QResizeEvent>
#include <QRect>
#include <QModelIndex>

#include "midinotemodel.h"

class MidiNoteTileWindow : public QWindow
{
    Q_OBJECT
public:
    MidiNoteTileWindow(MidiNoteModel *noteModel);
    QSize getPreferredSize() const;

    int totalWidth;
    int totalHeight;

public slots:
    //void renderLater();
    void render(const QRegion &renderArea);
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());


protected:
    //bool event(QEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;
    void exposeEvent(QExposeEvent *event) override;

private:
    QRect getTileRect(int index) const;

    QBackingStore *m_backingStore;
    MidiNoteModel *noteModel;

    const int tileSize = 10;
    const int pad = 2;
    const int numCols = 6;
    const int labelWidth = 36;

    int numRows;
};

#endif // MIDINOTETILEWINDOW_H
