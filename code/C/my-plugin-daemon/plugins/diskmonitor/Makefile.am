plugin_LTLIBRARIES = libdiskmonitor.la

libdiskmonitor_la_SOURCES = 		\
	my_diskmonitor_plugin.c	\
	my_diskmonitor_plugin.h	\
	my_diskmonitor_space.c	\
	my_diskmonitor_space.h	\
	my_diskmonitor_dialog.c		\
	my_diskmonitor_dialog.h

libdiskmonitor_la_CPPFLAGS = 					\
	-I$(top_srcdir)/src			\
	-DMY_PLUGIN_LOCALEDIR=\""$(datadir)/locale"\"	\
	$(AM_CPPFLAGS)

libdiskmonitor_la_CFLAGS = 		\
	$(PLUGIN_MODULE_CFLAGS)	\
	$(GIOUNIX_CFLAGS)		\
	$(AM_CFLAGS)

libdiskmonitor_la_LDFLAGS = $(MPD_PLUGIN_LDFLAGS)

libdiskmonitor_la_LIBADD = $(PLUGIN_MODULE_LIBS) $(GIOUNIX_LIBS)

plugin_in_files = diskmonitor.my-plugin.in

plugin_DATA = $(plugin_in_files:.my-plugin.in=.my-plugin)

EXTRA_DIST = $(plugin_in_files)

CLEANFILES = $(plugin_DATA)

DISTCLEANFILES = $(plugin_DATA)

@MPD_INTLTOOL_PLUGIN_RULE@
