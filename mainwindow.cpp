#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "devicesdialog.h"
#include "midinotetileview.h"

#include <QDebug>
#include <QColorDialog>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QSettings settings;

    detectedNotesModel.setSourceModel(&notesModel);
    detectedNotesModel.setDynamicSortFilter(true);

    ui->setupUi(this);
    ui->noteListView->setModel(&detectedNotesModel);
    ui->noteListView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    midi = new WindowsMidiInputDevice(settings.value("midiProductName").toString());

    dmx = new SerialDmxDevice(settings.value("dmxSerialPort").toString());
    dmx->moveToThread(&dmxThread);
    connect(&dmxThread, &QThread::started, dmx, &SerialDmxDevice::init);
    connect(&dmxThread, &QThread::finished, dmx, &QObject::deleteLater);
    connect(dmx, &SerialDmxDevice::debugMessage, ui->statusbar, &QStatusBar::showMessage);
    dmxThread.start();

    menuBar()->addAction(tr("Devices"), this, &MainWindow::openDeviceDialog);

    // Color dialog
    colorPicker = new QColorDialog();
    colorPicker->setOption(QColorDialog::NoButtons);
    ui->horizontalLayout->addWidget(colorPicker);
    connect(colorPicker, &QColorDialog::currentColorChanged, this, &MainWindow::onNewColor);
    connect(ui->noteListView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onNoteSelectionChanged);


    connect(midi, &WindowsMidiInputDevice::newNoteEvent, &notesModel, &MidiNoteModel::onMidiNote);
    connect(&notesModel, &MidiNoteModel::sendColor, dmx, &SerialDmxDevice::setColor);

    new MidiNoteTileView(&notesModel, ui->frame);
}


MainWindow::~MainWindow()
{
    delete ui;
    dmxThread.quit();

    // Wait one second then kill the thread
    if (!dmxThread.wait(1000)) {
        dmxThread.terminate();
        qDebug() << "Killed thread";
    } else {
        qDebug() << "Quit thread gracefully";
    }
}

void MainWindow::openDeviceDialog()
{
    DevicesDialog dialog(dmx, midi, this);
    dialog.exec();
}

void MainWindow::onNewColor(const QColor &color)
{
   QModelIndexList rows = ui->noteListView->selectionModel()->selectedRows();
   for (const QModelIndex &row : rows) {
        detectedNotesModel.setData(row, color, Qt::DecorationRole);
   }
}

void MainWindow::onNoteSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (selected.isEmpty()) {
        colorPicker->setCurrentColor(QColor());
        colorPicker->setEnabled(false);
    } else {
        auto index = selected.first().topLeft();
        qDebug() << "index row:" << index.row();
        colorPicker->setCurrentColor(detectedNotesModel.data(index, Qt::DecorationRole).value<QColor>());
        colorPicker->setEnabled(true);
    }
}
