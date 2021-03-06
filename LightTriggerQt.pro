QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 file_copies

# Use shadow build, no debug/release subfolders, so that OUT_PWD is actually location of deployed executable
CONFIG -= debug_and_release

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    channel-slider/channelitemmodel.cpp \
    channel-slider/channelslideradapter.cpp \
    channel-slider/channelsliderviewholder.cpp \
    channel-slider/customlistitem.cpp \
    detectedmidinotemodel.cpp \
    devicesdialog.cpp \
    fixturedetailscache.cpp \
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
    models/fixturemimedata.cpp \
    navtoolbar.cpp \
    openfixturefilereaderthread.cpp \
    recycler/recyclermimedata.cpp \
    recycler/recyclerview.cpp \
    recycler/recyclerviewadapter.cpp \
    recycler/recyclerviewpool.cpp \
    recycler/viewholder.cpp \
    scenewidget.cpp

HEADERS += \
    channel-slider/channelitemmodel.h \
    channel-slider/channelslideradapter.h \
    channel-slider/channelsliderviewholder.h \
    channel-slider/customlistitem.h \
    detectedmidinotemodel.h \
    devicesdialog.h \
    fixturedetailscache.h \
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
    models/fixturemimedata.h \
    navtoolbar.h \
    openfixturefilereaderthread.h \
    recycler/recyclermimedata.h \
    recycler/recyclerview.h \
    recycler/recyclerviewadapter.h \
    recycler/recyclerviewpool.h \
    recycler/viewholder.h \
    scenewidget.h

FORMS += \
    channel-slider/customlistitem.ui \
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

