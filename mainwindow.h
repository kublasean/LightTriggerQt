#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QThread>
#include <QColor>
#include <QItemSelection>
#include <QStackedWidget>


#include "dmx-serial/serialdmxdevice.h"
#include "midi-windows/windowsmidiinputdevice.h"
#include "midinotemodel.h"
#include "mididockwidget.h"
#include "navtoolbar.h"



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

private slots:
    void openDeviceDialog();
    void onNoteSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void appModeChanged(NavToolBar::AppMode newMode);

private:
    Ui::MainWindow *ui;
    MidiDockWidget *midiWidget;
    QStackedWidget *stackedWidget;

    SerialDmxDevice *dmx;
    QThread dmxThread;
    WindowsMidiInputDevice *midi;

    MidiNoteModel notesModel;
};
#endif // MAINWINDOW_H
