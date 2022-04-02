#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "devicesdialog.h"
#include "fixturewidget.h"
#include "scenewidget.h"
#include "recycler/recyclerview.h"
#include "channel-slider/channelslideradapter.h"

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
    connect(this, &MainWindow::newDmxValues, dmx, &SerialDmxDevice::newChannelValues);
    //connect(&notesModel, &MidiNoteModel::sendColor, dmx, &SerialDmxDevice::setColor);
    dmxThread.start();

    // MIDI device
    midi = new WindowsMidiInputDevice(settings.value("midiProductName").toString());
    connect(midi, &WindowsMidiInputDevice::newNoteEvent, &notesModel, &MidiNoteModel::onMidiNote);

    // Menu bar
    menuBar()->addAction(tr("Devices"), this, &MainWindow::openDeviceDialog);

    // Nav toolbar
    NavToolBar *toolbar = new NavToolBar(this);
    addToolBar(Qt::ToolBarArea::LeftToolBarArea, toolbar);
    connect(toolbar, &NavToolBar::appModeChanged, this, &MainWindow::appModeChanged);

    // Dock widgets
    midiWidget = new MidiDockWidget(&notesModel, this);
    connect(midiWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onNoteSelectionChanged);
    connect(midi, &WindowsMidiInputDevice::newNoteEvent, midiWidget, &MidiDockWidget::showNote);
    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, midiWidget);
    midiWidget->hide();

    fixtureDockWidget = new QDockWidget(tr("Fixtures"), this);
    fixtureDockWidget->setWidget(new FixtureWidget(&fixtureCache));
    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, fixtureDockWidget);

    // Channels list
    RecyclerView *channelsView = new RecyclerView(50);
    ChannelItemModel *channelsItemModel = new ChannelItemModel(this);
    ChannelSliderAdapter *channelsAdapter = new ChannelSliderAdapter(channelsItemModel);
    channelsView->setAdapter(channelsAdapter);
    channelsAdapter->setSelectionModel(channelsView->selectionModel());
    connect(channelsItemModel, &QAbstractItemModel::dataChanged, this, &MainWindow::onChannelDataChanged);

    // Central widget    
    stackedWidget = new QStackedWidget();
    stackedWidget->addWidget(channelsView);  // added first so becomes initially selected
    stackedWidget->addWidget(new SceneWidget());
    setCentralWidget(stackedWidget);

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

/*
void MainWindow::onNewColor(const QColor &color)
{
   QModelIndexList rows = midiWidget->selectionModel()->selectedRows();
   for (const QModelIndex &row : rows) {
        midiWidget->notesProxyModel()->setData(row, color, Qt::DecorationRole);
   }
}
*/

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

void MainWindow::appModeChanged(NavToolBar::AppMode mode)
{
    switch (mode) {
    case NavToolBar::AppMode::Project:
        midiWidget->hide();
        fixtureDockWidget->show();
        stackedWidget->setCurrentIndex(0);
        break;
    case NavToolBar::AppMode::Scenes:
        midiWidget->hide();
        fixtureDockWidget->hide();
        stackedWidget->setCurrentIndex(1);
        break;
    case NavToolBar::AppMode::Triggers:
        midiWidget->show();
        fixtureDockWidget->hide();
        stackedWidget->setCurrentIndex(1);
        break;
    }
}

void MainWindow::onChannelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if (!roles.contains(Qt::EditRole) && !roles.isEmpty())
        return;

    QVector<QPair<int, int> > newValues;

    for (int i=topLeft.row(); i<=bottomRight.row(); i++) {
        ChannelUserData userData = qvariant_cast<ChannelUserData>(topLeft.model()->index(i, 0).data(Qt::UserRole));
        if (userData.isHeader)
            continue;
        newValues.append(QPair<int, int>(userData.channel, userData.value));
    }

    emit newDmxValues(newValues);
}

