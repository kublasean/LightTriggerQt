#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QToolButton>
#include <QAction>
#include <QThread>
#include <QColor>

#include "serialdmxdevice.h"
#include "midi-windows/windowsmididevicesmodel.h"
#include "models/availableserialdevicesmodel.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void newSerialDmxDevice(QString portName);

private slots:
    void dmxDeviceSelected(int index);
    void onDmxStatus(bool isConnected);

private:
    Ui::MainWindow *ui;
    QToolButton *statusIcon;
    QAction *disconnectDeviceAction, *connectDeviceAction;
    SerialDmxDevice *dmx;
    QThread dmxThread;
    AvailableSerialDevicesModel serialDevices;
    WindowsMidiDevicesModel midiDevices;
};
#endif // MAINWINDOW_H
