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
#include "midi-windows/windowsmidiinputdevice.h"


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

private:
    Ui::MainWindow *ui;
    SerialDmxDevice *dmx;
    WindowsMidiInputDevice midi;
    QThread dmxThread;
};
#endif // MAINWINDOW_H
