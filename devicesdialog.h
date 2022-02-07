#ifndef DEVICESDIALOG_H
#define DEVICESDIALOG_H

#include <QDialog>

#include "dmx-serial/serialdmxdevice.h"
#include "midi-windows/windowsmidiinputdevice.h"

namespace Ui {
class DevicesDialog;
}

class DevicesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DevicesDialog(SerialDmxDevice *dmx, WindowsMidiInputDevice *midi, QWidget *parent = nullptr);
    ~DevicesDialog();

signals:
    void newSelectedSerialPortName(const QString &portName);
    void newSelectedMidiProduct(int midiIndex);

private slots:
    void updateSerialPorts();
    void updateMidiDevices();

    void onSelectedSerialPort(int index);
    void onSelectedMidiDevice(int index);

private:
    Ui::DevicesDialog *ui;
};

#endif // DEVICESDIALOG_H
