#ifndef MIDINOTETILEVIEW_H
#define MIDINOTETILEVIEW_H

#include <QWidget>
#include "midinotemodel.h"

class MidiNoteTileView : public QWidget
{
    Q_OBJECT
public:
    explicit MidiNoteTileView(MidiNoteModel *model, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    int getTilePos(int rowOrCol) const;
    int getIndex(int row, int col) const;
    QRect getTileRect(int index) const;
    QPoint getTilePoint(int index) const;

    int tileSize = 20;
    int pad = 5;
    const int numCols = 6;
    int numRows;

    MidiNoteModel *model;
};

#endif // MIDINOTETILEVIEW_H
