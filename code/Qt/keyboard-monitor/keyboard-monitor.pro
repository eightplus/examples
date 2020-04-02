isEqual(QT_MAJOR_VERSION, 5) {
    QT += widgets gui core x11extras
}

TARGET = keyboard-monitor
TEMPLATE = app

target.path = /usr/bin
target.source += $$TARGET

CONFIG += c++14
CONFIG += qt warn_on
CONFIG += release
CONFIG += link_pkgconfig
PKGCONFIG += xcb

LIBS += -lX11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CFLAGS += -D_GNU_SOURCE
QMAKE_CPPFLAGS *= $(shell dpkg-buildflags --get CPPFLAGS)
QMAKE_CFLAGS   *= $(shell dpkg-buildflags --get CFLAGS)
QMAKE_CXXFLAGS *= $(shell dpkg-buildflags --get CXXFLAGS)
QMAKE_LFLAGS   *= $(shell dpkg-buildflags --get LDFLAGS)

unix {
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}

SOURCES += \
        main.cpp \
        widget.cpp \
        native-event-filter-factory.cpp

HEADERS += \
        widget.h \
        native-event-filter-factory.h
