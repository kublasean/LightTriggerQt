#include "mididockwidget.h"
#include "ui_mididockwidget.h"
#include "midinotetileview.h"

#include <QItemSelectionModel>
#include <QGraphicsDropShadowEffect>

MidiDockWidget::MidiDockWidget(MidiNoteModel *notesModel, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::MidiDockWidget)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, pal.color(QPalette::Base));
    setPalette(pal);
    setAutoFillBackground(true);

    detectedNotesModel.setSourceModel(notesModel);
    detectedNotesModel.setDynamicSortFilter(true);

    ui->setupUi(this);
    ui->noteListView->setModel(&detectedNotesModel);
    ui->noteListView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->horizontalLayout->setAlignment(ui->tileViewFrame, Qt::AlignTop);
    new MidiNoteTileView(notesModel, ui->tileViewFrame);
}

MidiDockWidget::~MidiDockWidget()
{
    delete ui;
}

QItemSelectionModel *MidiDockWidget::selectionModel() const
{
    return ui->noteListView->selectionModel();
}

DetectedMidiNoteModel *MidiDockWidget::notesProxyModel()
{
    return &detectedNotesModel;
}

void MidiDockWidget::showNote(int note, int velocity)
{
    ui->label->setText("MIDI note: " + QString::number(note) + ", velocity: " + QString::number(velocity));
    ui->label->setEnabled(velocity != 0);
}
