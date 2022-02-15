#include "mididockwidget.h"
#include "ui_mididockwidget.h"
#include "midinotetilewindow.h"

#include <QItemSelectionModel>
#include <QGraphicsDropShadowEffect>
#include <QSizePolicy>

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


    MidiNoteTileWindow *window = new MidiNoteTileWindow(notesModel);
    QWidget *container = QWidget::createWindowContainer(window, nullptr, Qt::Widget);
    container->setFixedSize(window->getPreferredSize());
    ui->horizontalLayout->insertWidget(0, container, 0, Qt::AlignTop);
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
