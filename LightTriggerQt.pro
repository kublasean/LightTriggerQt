QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    detectedmidinotemodel.cpp \
    devicesdialog.cpp \
    fixturewidget.cpp \
    main.cpp \
    mainwindow.cpp \
    midi-windows/windowsmidiinputdevice.cpp \
    midi-windows/windowsmidiutil.cpp \
    dmx-serial/serialdmxdevice.cpp \
    mididockwidget.cpp \
    midinotemodel.cpp \
    midinotetileview.cpp \
    navtoolbar.cpp \
    scenewidget.cpp

HEADERS += \
    detectedmidinotemodel.h \
    devicesdialog.h \
    fixturewidget.h \
    mainwindow.h \
    midi-windows/windowsmidiinputdevice.h \
    midi-windows/windowsmidiutil.h \
    dmx-serial/serialdmxdevice.h \
    mididockwidget.h \
    midinotemodel.h \
    midinotetileview.h \
    navtoolbar.h \
    scenewidget.h

FORMS += \
    devicesdialog.ui \
    fixturewidget.ui \
    mainwindow.ui \
    mididockwidget.ui \
    scenewidget.ui

LIBS += -lWinmm

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets-resource.qrc

