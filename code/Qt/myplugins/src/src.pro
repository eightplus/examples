
TEMPLATE  = app

TARGET = plugin-demo

QT += core gui widgets

CONFIG += c++11 link_pkgconfig
PKGCONFIG       += glib-2.0 libmatemixer

HEADERS += \
    mainwindow.h \
    contentwidget.h \
    plugininterface.h \
    pluginmanager.h

SOURCES += main.cpp \
    mainwindow.cpp \
    contentwidget.cpp \
    pluginmanager.cpp

unix {
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}
