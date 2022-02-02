#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    connect(dmx, &SerialDmxDevice::isConnected, this, &MainWindow::onDmxStatus);
    connect(this, &MainWindow::newSerialDmxDevice, dmx, &SerialDmxDevice::newSelectedDevice);

    QToolBar *deviceToolBar = addToolBar(tr("Device"));
    QComboBox *deviceDropDown = new QComboBox();
    deviceDropDown->setModel(&serialDevices);
    deviceToolBar->addWidget(new QLabel(tr("DMX Device ")));
    deviceToolBar->addWidget(deviceDropDown);
    connect(deviceDropDown, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::dmxDeviceSelected);
    connect(dmx, &SerialDmxDevice::isConnected, deviceDropDown, &QComboBox::setDisabled);

    QAction *refreshDeviceAction = deviceToolBar->addAction(tr("Refresh"));
    refreshDeviceAction->setShortcut(tr("Ctrl+R"));
    connect(refreshDeviceAction, &QAction::triggered, &serialDevices, &AvailableSerialDevicesModel::updateDeviceList);
    connect(dmx, &SerialDmxDevice::isConnected, refreshDeviceAction, &QAction::setDisabled);

    deviceToolBar->addSeparator();

    connectDeviceAction = deviceToolBar->addAction(tr("Connect"));
    disconnectDeviceAction = deviceToolBar->addAction(tr("Disconnect"));
    connect(connectDeviceAction, &QAction::triggered, dmx, &SerialDmxDevice::start);
    connect(disconnectDeviceAction, &QAction::triggered, dmx, &SerialDmxDevice::stop);
    connect(dmx, &SerialDmxDevice::isConnected, connectDeviceAction, &QAction::setDisabled);

    statusIcon = new QToolButton();
    statusIcon->setDisabled(true);
    deviceToolBar->addWidget(statusIcon);

    // Color dialog
    QColorDialog *colorPicker = new QColorDialog();
    colorPicker->setOption(QColorDialog::NoButtons);
    this->setCentralWidget(colorPicker);
    connect(colorPicker, &QColorDialog::currentColorChanged, dmx, &SerialDmxDevice::setColor);

    serialDevices.updateDeviceList();
    midiDevices.updateDeviceList();
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

void MainWindow::dmxDeviceSelected(int row)
{
    bool noDevices = row == -1;
    disconnectDeviceAction->setDisabled(noDevices);
    connectDeviceAction->setDisabled(noDevices);

    QIcon icon;
    icon.addFile(":/assets/device-normal/baseline_help_black_24dp.png");
    statusIcon->setIcon(icon);

    if (!noDevices)
        emit newSerialDmxDevice(serialDevices.data(serialDevices.index(row, 0), Qt::UserRole).toString());
}

void MainWindow::onDmxStatus(bool isConnected)
{
    QIcon icon;
    if (isConnected) {
        icon.addFile(":/assets/device-active/baseline_check_circle_black_24dp.png");
    } else {
        icon.addFile(":/assets/device-disabled/baseline_cancel_black_24dp.png");
    }
    statusIcon->setIcon(icon);
    disconnectDeviceAction->setDisabled(!isConnected);
}

