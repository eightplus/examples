NULL =

mpd_gschemas_in = \
	org.freedesktop.MyPluginDaemon.plugins.diskmonitor.gschema.xml.in \
	$(NULL)

@INTLTOOL_XML_NOMERGE_RULE@

gsettings_SCHEMAS = $(mpd_gschemas_in:.xml.in=.xml)

@GSETTINGS_RULES@

EXTRA_DIST = 				\
	$(mpd_gschemas_in)		\
	$(NULL)

DISTCLEANFILES = 			\
	$(gsettings_SCHEMAS)			\
	$(NULL)

MAINTAINERCLEANFILES =			\
	*~				\
	Makefile.in
