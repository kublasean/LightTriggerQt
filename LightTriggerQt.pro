QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    devicesdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    midi-windows/windowsmidiinputdevice.cpp \
    midi-windows/windowsmidiutil.cpp \
    dmx-serial/serialdmxdevice.cpp \
    triggereffectmodel.cpp

HEADERS += \
    devicesdialog.h \
    mainwindow.h \
    midi-windows/windowsmidiinputdevice.h \
    midi-windows/windowsmidiutil.h \
    dmx-serial/serialdmxdevice.h \
    triggereffectmodel.h

FORMS += \
    devicesdialog.ui \
    mainwindow.ui

LIBS += -lWinmm

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets-resource.qrc

