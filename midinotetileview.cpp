#include "midinotetileview.h"
#include "midinotemodel.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QSizePolicy>
#include <QAbstractItemModel>

int calcNumRows(int numCols)
{
    int extraRow = NUM_MIDI_NOTES % numCols == 0 ? 0 : 1;
    return NUM_MIDI_NOTES / numCols + extraRow;
}

MidiNoteTileView::MidiNoteTileView(MidiNoteModel *sourceModel, QWidget *parent)
    : QWidget(parent)
{
    model = sourceModel;

    //setPalette(Qt::white);
    setAutoFillBackground(true);
    setAttribute(Qt::WA_StaticContents, true);
    setMouseTracking(true);

    numRows = calcNumRows(numCols);

    int totalWidth = numCols*tileSize + (numCols-1)*pad;
    int totalHeight = numRows*tileSize + (numRows-1)*pad;

    setGeometry(0, 0, totalWidth, totalHeight);
    parent->setFixedSize(totalWidth, totalHeight);
    parent->updateGeometry();

    connect(model, &QAbstractItemModel::dataChanged, this, &MidiNoteTileView::dataChanged);
}

int MidiNoteTileView::getTilePos(int rowOrCol) const
{
    return rowOrCol*tileSize + rowOrCol*pad;
}

int MidiNoteTileView::getIndex(int xVal, int yVal) const
{
    int col = xVal / (tileSize + pad);
    int row = yVal / (tileSize + pad);

    qDebug() << xVal << yVal;
    qDebug() << "col:" << col << "row:" << row;

    return row * numCols + col;
}

QRect MidiNoteTileView::getTileRect(int index) const
{
    return QRect(getTilePos(index % numCols), getTilePos(index / numCols), tileSize, tileSize);
}

QPoint MidiNoteTileView::getTilePoint(int index) const
{
    return QPoint(getTilePos(index % numCols), getTilePos(index / numCols));
}

void MidiNoteTileView::mouseMoveEvent(QMouseEvent *event)
{
    //int index = getIndex(event->x(), event->y());
    //update(getTileRect(index));
}

void MidiNoteTileView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if (!roles.contains(Qt::UserRole))
        return;

    QRect drawArea(getTilePoint(topLeft.row()), getTilePoint(bottomRight.row()) + QPoint(tileSize, tileSize));
    update(drawArea);
}

void MidiNoteTileView::paintEvent(QPaintEvent *event)
{
    qDebug() << "Paint Event" << event->rect();

    QPainter painter(this);

    int beginIndex = getIndex(event->rect().topLeft().x(), event->rect().topLeft().y());
    int endIndex = getIndex(event->rect().bottomRight().x(), event->rect().bottomRight().y());

    int beginRow = beginIndex / numCols, beginCol = beginIndex % numCols;
    int endRow = endIndex / numCols, endCol = endIndex % numCols;

    qDebug() << "begin row" << beginRow << "end row" << endRow;
    qDebug() << "begin col" << beginCol << "end col" << endCol;

    for (int row=beginRow; row<=endRow; row++) {
        for (int col=beginCol; col<=endCol; col++) {

            int note = row * numCols + col;
            if (note < 0 || note >= model->rowCount())
                return;

            auto noteIndex = model->index(note);
            MidiNoteEffect details = qvariant_cast<MidiNoteEffect>(model->data(noteIndex, Qt::UserRole));
            //QBrush brush(details.pressed ? this->palette().highlight() : QColor("grey"));
            painter.fillRect(getTileRect(note), details.pressed ? palette().highlight() : (details.detected ? palette().dark() : palette().light()));
        }
    }
}


