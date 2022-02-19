QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 file_copies

# Use shadow build, no debug/release subfolders, so that OUT_PWD is actually location of deployed executable
CONFIG -= debug_and_release

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    detectedmidinotemodel.cpp \
    devicesdialog.cpp \
    fixturedetailwidget.cpp \
    fixtureitemdelegate.cpp \
    fixturewidget.cpp \
    main.cpp \
    mainwindow.cpp \
    midi/windowsmidiinputdevice.cpp \
    dmx/serialdmxdevice.cpp \
    mididockwidget.cpp \
    midinotemodel.cpp \
    midinotetilewindow.cpp \
    navtoolbar.cpp \
    openfixturefilereaderthread.cpp \
    scenewidget.cpp

HEADERS += \
    detectedmidinotemodel.h \
    devicesdialog.h \
    fixturedetailwidget.h \
    fixtureitemdelegate.h \
    fixturewidget.h \
    mainwindow.h \
    midi/windowsmidiinputdevice.h \
    dmx/serialdmxdevice.h \
    mididockwidget.h \
    midinotemodel.h \
    midinotetilewindow.h \
    models/fixturedetails.h \
    navtoolbar.h \
    openfixturefilereaderthread.h \
    scenewidget.h

FORMS += \
    devicesdialog.ui \
    fixturedetailwidget.ui \
    fixturewidget.ui \
    mainwindow.ui \
    mididockwidget.ui \
    scenewidget.ui

LIBS += -lWinmm

# Copy fixture config files
COPIES += fixture_files
fixture_files.files = $$PWD/fixture-profiles
fixture_files.path = $$OUT_PWD

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets-resource.qrc

