#-------------------------------------------------
#
# Project created by QtCreator 2019-12-30T19:09:48
#
#-------------------------------------------------

QT       += core gui x11extras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = inputpass
TEMPLATE = app

CONFIG += c++14
CONFIG += qt warn_on
CONFIG += release
CONFIG += link_pkgconfig
PKGCONFIG += x11 xext

#LIBS += -lX11 -lXext

QMAKE_CPPFLAGS *= $(shell dpkg-buildflags --get CPPFLAGS)
QMAKE_CFLAGS   *= $(shell dpkg-buildflags --get CFLAGS)
QMAKE_CXXFLAGS *= $(shell dpkg-buildflags --get CXXFLAGS)
QMAKE_LFLAGS   *= $(shell dpkg-buildflags --get LDFLAGS)
QMAKE_CFLAGS_DEBUG += -g

SOURCES += main.cpp\
        widget.cpp \
        utils.cpp

HEADERS  += widget.h \
        utils.h

FORMS    += widget.ui

unix {
    UI_DIR = $$PWD/.ui
    MOC_DIR = $$PWD/.moc
    OBJECTS_DIR = $$PWD/.obj
    RCC_DIR = $$PWD/.rcc
}

RESOURCES += \
    res.qrc
