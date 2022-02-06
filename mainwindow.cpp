#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dmx-serial/dmxtoolbar.h"
#include "midi-windows/miditoolbar.h"

#include <QToolBar>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QBitmap>
#include <QDebug>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->noteListView->setModel(&effectModel);

    dmx = new SerialDmxDevice();
    dmx->moveToThread(&dmxThread);
    connect(&dmxThread, &QThread::started, dmx, &SerialDmxDevice::init);
    connect(&dmxThread, &QThread::finished, dmx, &QObject::deleteLater);
    dmxThread.start();

    connect(dmx, &SerialDmxDevice::debugMessage, ui->statusbar, &QStatusBar::showMessage);

    addToolBar(new DmxToolBar(dmx, tr("DMX")));
    addToolBarBreak();
    addToolBar(new MidiToolBar(&midi, tr("MIDI")));

    // Color dialog
    colorPicker = new QColorDialog();
    colorPicker->setOption(QColorDialog::NoButtons);
    ui->horizontalLayout->addWidget(colorPicker);
    connect(colorPicker, &QColorDialog::currentColorChanged, this, &MainWindow::onNewColor);
    connect(ui->noteListView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onNoteSelectionChanged);


    connect(&midi, &WindowsMidiInputDevice::newNoteEvent, &effectModel, &TriggerEffectModel::onMidiNote);
    connect(&effectModel, &TriggerEffectModel::sendColor, dmx, &SerialDmxDevice::setColor);

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

void MainWindow::onNewColor(const QColor &color)
{
    QModelIndexList rows = ui->noteListView->selectionModel()->selectedRows();
    for (const QModelIndex &row : rows) {
        effectModel.setData(row, color, Qt::UserRole);
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
        colorPicker->setCurrentColor(effectModel.data(index, Qt::UserRole).value<QColor>());
        colorPicker->setEnabled(true);
    }
}



