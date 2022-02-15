#include "midinotetilewindow.h"

#include <QPainter>
#include <QDebug>
#include <QApplication>
#include <QPalette>

MidiNoteTileWindow::MidiNoteTileWindow(MidiNoteModel *noteModel)
{
    m_backingStore = new QBackingStore(this);
    this->noteModel = noteModel;
    numRows = noteModel->rowCount() / numCols + (noteModel->rowCount() % numCols == 0 ? 0 : 1);

    connect(noteModel, &QAbstractItemModel::dataChanged, this, &MidiNoteTileWindow::dataChanged);
}

QSize MidiNoteTileWindow::getPreferredSize() const
{
    return QSize(numCols*tileSize + (numCols-1)*pad + labelWidth, numRows*tileSize + (numRows-1)*pad);
}

QRect MidiNoteTileWindow::getTileRect(int index) const
{
    int row = index / numCols;
    int col = index % numCols;
    return QRect(col*tileSize + col*pad + labelWidth, row*tileSize + row*pad, tileSize, tileSize);
}

/*
bool MidiNoteTileWindow::event(QEvent *event)
{
    if (event->type() == QEvent::UpdateRequest) {
        qDebug() << "Update Request Event";
        render(QRect(0, 0, width(), height()));
        return true;
    }
    return QWindow::event(event);
}

void MidiNoteTileWindow::renderLater()
{
    requestUpdate();
}
*/

void MidiNoteTileWindow::resizeEvent(QResizeEvent *resizeEvent)
{
    qDebug() << "Resize Event" << resizeEvent->size();
    m_backingStore->resize(resizeEvent->size());

    qDebug() << "New Size" << size();

    QRect bg(0, 0, width(), height());
    QPainter painter;

    m_backingStore->beginPaint(bg);
    painter.begin(m_backingStore->paintDevice());

    // Paint background
    painter.fillRect(bg, qApp->palette().base());

    // Paint octave labels every other row
    painter.setPen(qApp->palette().dark().color());

    for (int row=0; row<numRows; row+=2) {
        QRect rect(0, row*tileSize + row*pad - 3, labelWidth-5, tileSize*2+pad);
        painter.drawText(rect, Qt::AlignTop | Qt::AlignRight, MidiNoteModel::getNoteName(row*numCols));
    }

    QBrush brush(qApp->palette().midlight());
    for (int i=0; i<noteModel->rowCount(); i++) {
        painter.fillRect(getTileRect(i), brush);
    }

    painter.end();
    m_backingStore->endPaint();
}

void MidiNoteTileWindow::exposeEvent(QExposeEvent *expose)
{
    qDebug() << "Expose event" << expose->region();
    render(expose->region());
}

void MidiNoteTileWindow::render(const QRegion &region)
{
    qDebug() << "Render Event" << region;

    if (!isExposed())
        return;

    m_backingStore->flush(region);

}

void MidiNoteTileWindow::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if (!roles.contains(Qt::UserRole))
        return;

    QBrush highlight(qApp->palette().highlight());
    QBrush detected(qApp->palette().dark());
    //QBrush normal(qApp->palette().midlight());

    QRegion region;
    for (int i=topLeft.row(); i<=bottomRight.row(); i++) {
        region = region.united(getTileRect(i));
    }

    qDebug() << "Data changed paint region:" << region;

    QPainter painter;
    m_backingStore->beginPaint(region);
    painter.begin(m_backingStore->paintDevice());

    for (int i=topLeft.row(); i<=bottomRight.row(); i++) {
        auto index = noteModel->index(i);
        MidiNoteEffect details = qvariant_cast<MidiNoteEffect>(noteModel->data(index, Qt::UserRole));
        painter.fillRect(getTileRect(i), details.pressed ? highlight : detected);

    }

    painter.end();
    m_backingStore->endPaint();
    render(region);
}

