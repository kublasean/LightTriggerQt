#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QThread>
#include <QColor>
#include <QItemSelection>
#include <QStackedWidget>


#include "dmx/serialdmxdevice.h"
#include "midi/windowsmidiinputdevice.h"
#include "midinotemodel.h"
#include "mididockwidget.h"
#include "navtoolbar.h"
#include "fixturedetailscache.h"


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
    void newDmxValues(const QVector<QPair<int, int> > &values);

private slots:
    void openDeviceDialog();
    void onNoteSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void appModeChanged(NavToolBar::AppMode newMode);
    void onChannelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());

private:
    Ui::MainWindow *ui;
    QDockWidget *fixtureDockWidget;
    MidiDockWidget *midiWidget;
    QStackedWidget *stackedWidget;

    SerialDmxDevice *dmx;
    QThread dmxThread;
    WindowsMidiInputDevice *midi;

    MidiNoteModel notesModel;

    FixtureDetailsCache fixtureCache;
};
#endif // MAINWINDOW_H
