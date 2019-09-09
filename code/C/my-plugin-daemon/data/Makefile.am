NULL =

mpd_gschemas_in = \
	org.freedesktop.MyPluginDaemon.plugins.diskmonitor.gschema.xml.in \
	$(NULL)

@INTLTOOL_XML_NOMERGE_RULE@

gsettings_SCHEMAS = $(mpd_gschemas_in:.xml.in=.xml)

@GSETTINGS_RULES@

@INTLTOOL_DESKTOP_RULE@
desktopdir = $(sysconfdir)/xdg/autostart
desktop_in_files = my-plugin-daemon.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)

servicedir = $(datadir)/dbus-1/services
service_in_files = org.freedesktop.MyPluginDaemon.service.in
service_DATA = $(service_in_files:.service.in=.service)

org.freedesktop.MyPluginDaemon.service: org.freedesktop.MyPluginDaemon.service.in Makefile
	$(AM_V_GEN) sed -e "s|\@libexecdir\@|$(libexecdir)|" $< > $@


EXTRA_DIST = 				\
	$(mpd_gschemas_in)		\
	$(service_in_files)		\
	$(desktop_in_files)		\
	$(NULL)

DISTCLEANFILES = 			\
	$(gsettings_SCHEMAS)			\
	$(service_DATA)			\
	$(desktop_DATA)			\
	$(NULL)

MAINTAINERCLEANFILES =			\
	*~				\
	Makefile.in
