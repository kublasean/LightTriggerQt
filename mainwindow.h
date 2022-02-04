#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QToolButton>
#include <QAction>
#include <QThread>
#include <QColor>

#include "dmx-serial/serialdmxdevice.h"
#include "midi-windows/windowsmidiinputdevice.h"
#include "triggereffectmodel.h"


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
    void sendColor(const QColor &color);

private slots:
    void onMidiNote(int note, int velocity);

private:
    Ui::MainWindow *ui;
    SerialDmxDevice *dmx;
    QThread dmxThread;
    WindowsMidiInputDevice midi;
    TriggerEffectModel model;
};
#endif // MAINWINDOW_H
