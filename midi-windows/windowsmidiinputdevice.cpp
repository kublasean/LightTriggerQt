#include "windowsmidiinputdevice.h"
#include "windowsmidiutil.h"
#include "windows.h"

#include <QDebug>

// Unknown what thread(s) this will be called back on
void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    switch (wMsg) {
    case MIM_OPEN:
        qDebug() << "Opened MIDI";
        break;
    case MIM_CLOSE:
        qDebug() << "Closed MIDI";
        break;
    case MIM_ERROR:
        qDebug() << "MIDI ERROR";
        break;
    default:
        qDebug() << "MIDI other message";
    }
}


WindowsMidiInputDevice::WindowsMidiInputDevice(QObject *parent) : QObject(parent)
{
    devId = -1;
    midiHandle = nullptr;
}

WindowsMidiInputDevice::~WindowsMidiInputDevice()
{
    if (midiHandle != nullptr) {
        midiInStop(midiHandle);
        midiInClose(midiHandle);
    }
}

void WindowsMidiInputDevice::setDevId(int id)
{
    if (midiHandle == nullptr)
        devId = id;
}

void WindowsMidiInputDevice::start()
{
    if (devId == -1 || midiHandle != nullptr)
        return;

    MMRESULT err = midiInOpen(&midiHandle, devId, (DWORD_PTR)(void *)&MidiInProc, 0, CALLBACK_FUNCTION);
    if (err != MMSYSERR_NOERROR) {
        printMMRESULT(err);
        emit isConnected(false);
        return;
    }

    err = midiInStart(midiHandle);
    if (err != MMSYSERR_NOERROR) {
        printMMRESULT(err);
        emit isConnected(false);
        return;
    }

    emit isConnected(true);
}

void WindowsMidiInputDevice::stop()
{
    if (midiHandle != nullptr) {
        midiInStop(midiHandle);
        midiInClose(midiHandle);
        midiHandle = nullptr;
    }

    emit isConnected(false);
}


