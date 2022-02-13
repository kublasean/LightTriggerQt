#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QThread>
#include <QColor>
#include <QColorDialog>
#include <QItemSelection>

#include "dmx-serial/serialdmxdevice.h"
#include "midi-windows/windowsmidiinputdevice.h"
#include "midinotemodel.h"
#include "mididockwidget.h"


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
    void onNewColor(const QColor &color);
    void onNoteSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
    Ui::MainWindow *ui;
    SerialDmxDevice *dmx;
    QThread dmxThread;
    WindowsMidiInputDevice *midi;
    QColorDialog *colorPicker;
    MidiDockWidget *midiWidget;

    MidiNoteModel notesModel;
};
#endif // MAINWINDOW_H
