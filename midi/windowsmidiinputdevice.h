#ifndef WINDOWSMIDIINPUTDEVICE_H
#define WINDOWSMIDIINPUTDEVICE_H

#include <QObject>

#include "windows.h"

class WindowsMidiInputDevice : public QObject
{
    Q_OBJECT
public:
    explicit WindowsMidiInputDevice(QString intialProductName = QString(), QObject *parent = nullptr);
    ~WindowsMidiInputDevice();

public slots:
    void switchDevice(int id);
    void start();
    void stop();

signals:
    void isConnected(bool connected);
    void newNoteEvent(int note, int velocity);

private:
    int devId;
    HMIDIIN midiHandle;
};

#endif // WINDOWSMIDIINPUTDEVICE_H
