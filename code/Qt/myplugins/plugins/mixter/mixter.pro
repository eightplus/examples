PLUGIN_NAME 	= mixter

#LANGUAGE = C++

QT              += widgets
TEMPLATE        = lib
CONFIG          += plugin c++11 link_pkgconfig
PKGCONFIG       += glib-2.0 libmatemixer

TARGET          = $$qtLibraryTarget($$PLUGIN_NAME)
DESTDIR         = $$_PRO_FILE_PWD_/../

QMAKE_CFLAGS_ISYSTEM = ""

#QMAKE_CXXFLAGS += -fpermissive
#LIBS += -ldl

#INCLUDEPATH += /usr/include/mate-mixer/
#LIBS += -L /usr/lib -lmatemixer

HEADERS += \
    mixterplugin.h \
    mixterwidget.h

SOURCES += \
    mixterplugin.cpp \
    mixterwidget.cpp
