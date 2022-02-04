#include "windowsmidiinputdevice.h"
#include "windowsmidiutil.h"
#include "windows.h"

#include <QDebug>

void processMidiData(DWORD_PTR data, WindowsMidiInputDevice *self)
{
    // Least byte contains the MIDI status (4-bit command and 4-bit channel)
    //uint8_t channel = data & 0x0F;
    uint8_t cmd = data & 0xF0;

    uint8_t note, velocity;
    switch (cmd) {

    // Note off
    case 0x80:
        qDebug() << "Note off";
        note = (data >> 8) & 0xFF;
        velocity = 0;
        break;

    // Note on
    case 0x90:
        qDebug() << "Note on";
        note = (data >> 8) & 0xFF;
        velocity = (data >> 16) & 0xFF;
        break;

    // Ignore other commands
    default:
        return;
    }

    qDebug() << "Note:" << note << "Velocity:" << velocity;
    emit self->newNoteEvent(note, velocity);
}

// Unknown what thread(s) this will be called back on
void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR userData, DWORD_PTR data, DWORD_PTR timestamp)
{
    WindowsMidiInputDevice *self = (WindowsMidiInputDevice *)(void *)userData; // eek

    switch (wMsg) {
    case MIM_ERROR:
        qDebug() << "MIDI ERROR";
        break;
    case MIM_DATA:
        processMidiData(data, self);
        break;
    default:
        //qDebug() << "MIDI other message";
        break;
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

    MMRESULT err = midiInOpen(&midiHandle, devId, (DWORD_PTR)(void *)&MidiInProc, (DWORD_PTR)(void *)this, CALLBACK_FUNCTION);
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


