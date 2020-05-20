isEqual(QT_MAJOR_VERSION, 5) {
    QT += widgets gui core x11extras
}

TARGET = floatwidget
TEMPLATE = app

target.source  += $$TARGET
target.path = /usr/bin
INSTALLS += target

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++14
CONFIG += qt warn_on
CONFIG += release
CONFIG += link_pkgconfig

QMAKE_CPPFLAGS *= $(shell dpkg-buildflags --get CPPFLAGS)
QMAKE_CFLAGS   *= $(shell dpkg-buildflags --get CFLAGS)
QMAKE_CXXFLAGS *= $(shell dpkg-buildflags --get CXXFLAGS)
QMAKE_LFLAGS   *= $(shell dpkg-buildflags --get LDFLAGS)
QMAKE_CFLAGS_DEBUG += -g
QMAKE_CXXFLAGS *= -fpermissive

SOURCES += \
    main.cpp \
    widget.cpp \
    contentwidget.cpp \
    floatball.cpp \
    toolbar.cpp \
    autohidewidget.cpp

HEADERS += \
    widget.h \
    contentwidget.h \
    floatball.h \
    toolbar.h \
    autohidewidget.h

unix {
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}
