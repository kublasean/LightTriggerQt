QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dmx-serial/dmxtoolbar.cpp \
    main.cpp \
    mainwindow.cpp \
    midi-windows/miditoolbar.cpp \
    midi-windows/windowsmididevicesmodel.cpp \
    midi-windows/windowsmidiinputdevice.cpp \
    midi-windows/windowsmidiutil.cpp \
    dmx-serial/availableserialdevicesmodel.cpp \
    dmx-serial/serialdmxdevice.cpp

HEADERS += \
    dmx-serial/dmxtoolbar.h \
    mainwindow.h \
    midi-windows/miditoolbar.h \
    midi-windows/windowsmididevicesmodel.h \
    midi-windows/windowsmidiinputdevice.h \
    midi-windows/windowsmidiutil.h \
    dmx-serial/availableserialdevicesmodel.h \
    dmx-serial/serialdmxdevice.h

FORMS += \
    mainwindow.ui

LIBS += -lWinmm

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets-resource.qrc

