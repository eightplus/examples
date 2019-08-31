NULL =

AM_CPPFLAGS = \
	-DDATADIR=\""$(datadir)"\"				\
        -DMY_PLUGIN_LOCALEDIR=\""$(datadir)/locale"\"   \
        -DLIBEXECDIR=\""$(libexecdir)"\"                        \
        -DMY_PLUGINDIR=\""$(plugindir)"\"               \
	$(WARN_CFLAGS)						\
	$(DISABLE_DEPRECATED_CFLAGS)				\
	$(DAEMON_MODULE_CFLAGS)				\
	$(NULL)

xpddir = $(libexecdir)

xpd_PROGRAMS = \
	my-plugin-daemon

my_daemon_glue.h: org.freedesktop.myplugindaemon.xml Makefile.am
	$(AM_V_GEN) dbus-binding-tool --prefix=my_daemon --mode=glib-server $< > $@

my_daemon_client.h: org.freedesktop.myplugindaemon.xml Makefile.am
	$(AM_V_GEN) dbus-binding-tool --prefix=my_daemon --mode=glib-client $< > $@

# if dbus-binding-tool in Makefile.am, then need add this
BUILT_SOURCES = 			\
	my_daemon_glue.h	\
	my_daemon_client.h	\
	$(NULL)

# /usr/include/my-plugin-daemon/my_daemon_client.h & /usr/include/my-plugin-daemon/my_plugin.h
apidir   = $(includedir)/my-plugin-daemon
api_DATA = 				\
	my_daemon_client.h	\
	my_plugin.h

my_plugin_daemon_SOURCES =		\
	main.c				\
	my_daemon.c	\
	my_daemon.h	\
        my_plugin.c          \
        my_plugin.h          \
        my_plugin_info.c     \
        my_plugin_info.h     \
        my_module.c          \
        my_module.h          \
	$(NULL)

my_plugin_daemon_CPPFLAGS = \
	$(AM_CPPFLAGS)

my_plugin_daemon_CFLAGS = 		\
	$(AM_CFLAGS)

my_plugin_daemon_LDFLAGS = 	\
	$(AM_LDFLAGS)

my_plugin_daemon_LDADD = 		\
	$(DAEMON_MODULE_LIBS)

EXTRA_DIST = 				\
	$(Dbusapi_DATA)			\
	org.freedesktop.myplugindaemon.xml	\
	$(NULL)

CLEANFILES = 				\
	$(BUILT_SOURCES)		\
	$(NULL)

# vim: ts=8
