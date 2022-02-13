#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "devicesdialog.h"
#include "navtoolbar.h"

#include <QDebug>
#include <QColorDialog>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QSettings settings;

    ui->setupUi(this);

    // DMX device
    dmx = new SerialDmxDevice(settings.value("dmxSerialPort").toString());
    dmx->moveToThread(&dmxThread);
    connect(&dmxThread, &QThread::started, dmx, &SerialDmxDevice::init);
    connect(&dmxThread, &QThread::finished, dmx, &QObject::deleteLater);
    connect(dmx, &SerialDmxDevice::debugMessage, ui->statusbar, &QStatusBar::showMessage);
    connect(&notesModel, &MidiNoteModel::sendColor, dmx, &SerialDmxDevice::setColor);
    dmxThread.start();

    // MIDI device
    midi = new WindowsMidiInputDevice(settings.value("midiProductName").toString());
    connect(midi, &WindowsMidiInputDevice::newNoteEvent, &notesModel, &MidiNoteModel::onMidiNote);

    // Menu bar
    menuBar()->addAction(tr("Devices"), this, &MainWindow::openDeviceDialog);

    // Nav toolbar
    addToolBar(Qt::ToolBarArea::LeftToolBarArea, new NavToolBar(this));

    // Dock widgets
    midiWidget = new MidiDockWidget(&notesModel, this);
    connect(midiWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onNoteSelectionChanged);
    connect(midi, &WindowsMidiInputDevice::newNoteEvent, midiWidget, &MidiDockWidget::showNote);
    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, midiWidget);

    // Color dialog
    //colorPicker = new QColorDialog();
    /*colorPicker->setOption(QColorDialog::NoButtons);
    ui->horizontalLayout->addWidget(colorPicker);
    connect(colorPicker, &QColorDialog::currentColorChanged, this, &MainWindow::onNewColor);*/
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
   /*QModelIndexList rows = midiWidget->selectionModel()->selectedRows();
   for (const QModelIndex &row : rows) {
        midiWidget->notesProxyModel()->setData(row, color, Qt::DecorationRole);
   }*/
}

void MainWindow::onNoteSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    /*if (selected.isEmpty()) {
        colorPicker->setCurrentColor(QColor());
        colorPicker->setEnabled(false);
    } else {
        auto index = selected.first().topLeft();
        qDebug() << "index row:" << index.row();
        colorPicker->setCurrentColor(midiWidget->notesProxyModel()->data(index, Qt::DecorationRole).value<QColor>());
        colorPicker->setEnabled(true);
    }*/
}
