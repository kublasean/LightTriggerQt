#include "windowsmidiutil.h"
#include <QDebug>


void printMMRESULT(MMRESULT err)
{
    wchar_t errBuf[MAXERRORLENGTH];
    if (midiInGetErrorTextW(err, errBuf, MAXERRORLENGTH) == MMSYSERR_NOERROR) {
        qDebug() << "MIDI error:" << QString::fromWCharArray(errBuf);
    } else {
        qDebug() << "Unknown MIDI error";
    }
}



