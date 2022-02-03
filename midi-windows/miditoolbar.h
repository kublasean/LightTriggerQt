#ifndef MIDITOOLBAR_H
#define MIDITOOLBAR_H

#include <QToolBar>

#include "midi-windows/windowsmididevicesmodel.h"
#include "midi-windows/windowsmidiinputdevice.h"

class MidiToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit MidiToolBar(WindowsMidiInputDevice *midi, const QString &title, QWidget *parent = nullptr);

private slots:
    void deviceSelected(int row);

private:
    WindowsMidiInputDevice *midi;
    WindowsMidiDevicesModel midiDevices;
    QAction *disconnectDeviceAction, *connectDeviceAction;
};

#endif // MIDITOOLBAR_H
