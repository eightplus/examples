#-------------------------------------------------
#
# Project created by QtCreator 2019-08-15T18:49:19
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mympvplayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix {
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        core.cpp \
        myprocess.cpp \
        videowindow.cpp \
        bottomwidget.cpp \
        mediadata.cpp \
        global.cpp \
        preferences.cpp \
        timeslider.cpp \
        mediasettings.cpp \
        utils.cpp \
        inforeader.cpp

HEADERS += \
        mainwindow.h \
        core.h \
        myprocess.h \
        videowindow.h \
        bottomwidget.h \
        mediadata.h \
        global.h \
        preferences.h \
        timeslider.h \
        mediasettings.h \
        utils.h \
        inforeader.h

FORMS += \
        mainwindow.ui
