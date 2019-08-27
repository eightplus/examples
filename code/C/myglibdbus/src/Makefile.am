#AUTOMAKE_OPTIONS= foreign

AM_CPPFLAGS = \
	$(MY_GLIB_DBUS_CFLAGS)
	$(DBUS_CFLAGS) \
	$(DBUS_GLIB_CFLAGS)


#(1)使用libs
LIBS = \
	$(DBUS_LIBS) \
	$(DBUS_GLIB_LIBS) \
	-ldbus-glib-1 \
	-lgobject-2.0 \
	-lgthread-2.0 \
	-lglib-2.0
 
noinst_PROGRAMS = server
 
server_SOURCES = \
	server.c \
	myglibdbus.c \
	myglibdbus.h \
	myglibdbus-glue.h

#(2.1)使用pkg，且是全局的
#server_LDADD = $(MY_GLIB_DBUS_LIBS)


#(2.2)使用pkg，分开的
#server_CFLAGS = \
#	$(GLIB_CFLAGS) \
#	$(GTHREAD_CFLAGS) \
#	$(GDBUS_CFLAGS)
#server_LDADD = \
#        $(GLIB_LIBS) \
#        $(GTHREAD_LIBS) \
#        $(GDBUS_LIBS)


BUILT_SOURCES = myglibdbus-glue.h
 
myglibdbus-glue.h: org.freedesktop.myglibdbus.xml
	$(LIBTOOL) --mode=execute dbus-binding-tool --prefix=myglibdbus --mode=glib-server --output=myglibdbus-glue.h $(srcdir)/org.freedesktop.myglibdbus.xml
 
CLEANFILES = $(BUILT_SOURCES)
 
EXTRA_DIST = org.freedesktop.myglibdbus.xml



noinst_PROGRAMS += client
client_SOURCES = client.c

client_CFLAGS = \
	$(GMODULE_CFLAGS)
client_LDADD = \
        $(GMODULE_LIBS)

# vim: ts=8
