#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dmxtoolbar.h"
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
    QColorDialog *colorPicker = new QColorDialog();
    colorPicker->setOption(QColorDialog::NoButtons);
    this->setCentralWidget(colorPicker);
    connect(colorPicker, &QColorDialog::currentColorChanged, dmx, &SerialDmxDevice::setColor);
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



