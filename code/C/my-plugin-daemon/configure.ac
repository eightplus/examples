AC_PREREQ([2.60])

m4_define([mpd_api_version_major],[1])
m4_define([mpd_api_version_minor],[0])
m4_define([mpd_api_version_micro],[0])
m4_define([mpd_api_version],
	[mpd_api_version_major.mpd_api_version_minor.mpd_api_version_micro])

AC_INIT([my-plugin-daemon],
	[myd_api_version_major.myd_api_version_minor.myd_api_version_micro],
	[https://eightplus.github.io/])

AC_CONFIG_SRCDIR([src/my_daemon.c])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.9 no-dist-gzip dist-xz tar-ustar check-news])
AM_MAINTAINER_MODE([enable])

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_STDC_HEADERS
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_LIBTOOL

AC_HEADER_STDC

AC_SUBST(VERSION)

AC_CONFIG_HEADERS([config.h])

IT_PROG_INTLTOOL([0.50.1])

GETTEXT_PACKAGE=my-plugin-daemon
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Name of default gettext domain])

# GLib min/max required versions
AC_DEFINE([GLIB_VERSION_MAX_ALLOWED], [GLIB_VERSION_2_36],
	[Warn on use of APIs added after GLib 2.36])
AC_DEFINE([GLIB_VERSION_MIN_REQUIRED], [GLIB_VERSION_2_36],
	[Warn on use of APIs deprecated before GLib 2.36])

AM_GLIB_GNU_GETTEXT

AS_AC_EXPAND(LIBEXECDIR, "$libexecdir")

# every plugin's Makefile.am will call this
MPD_INTLTOOL_PLUGIN_RULE='%.my-plugin:   %.my-plugin.in   $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*.po) ; LC_ALL=C $(INTLTOOL_MERGE) -d -u -c $(top_builddir)/po/.intltool-merge-cache $(top_srcdir)/po $< [$]@'
AC_SUBST([MPD_INTLTOOL_PLUGIN_RULE])

dnl ---------------------------------------------------------------------------
dnl - Dependencies
dnl ---------------------------------------------------------------------------

DBUS_GLIB_REQUIRED_VERSION=0.74
GLIB_REQUIRED_VERSION=2.36.0
GIO_REQUIRED_VERSION=2.26.0
GTK_REQUIRED_VERSION=3.18.9

EXTRA_COMPILE_WARNINGS(yes)

#AC_MSG_CHECKING([which gtk+ version to compile against])
#AC_ARG_WITH([gtk],
#  [AS_HELP_STRING([--with-gtk=2.0|3.0],[which gtk+ version to compile against (default: 2.0)])],
#  [case "$with_gtk" in
#     2.0|3.0) ;;
#     *) AC_MSG_ERROR([invalid gtk version specified]) ;;
#   esac],
#  [with_gtk=2.0])
#AC_MSG_RESULT([$with_gtk])

#case "$with_gtk" in
#  2.0) GTK_API_VERSION=2.0
#       GTK_REQUIRED_VERSION=2.24.0
#       LIBCANBERRA_PC=libcanberra-gtk
#       ;;
#  3.0) GTK_API_VERSION=3.0
#       GTK_REQUIRED_VERSION=3.0.0
#       LIBCANBERRA_PC=libcanberra-gtk3
#       ;;
#esac

PKG_CHECK_MODULES(DAEMON_MODULE,
	glib-2.0 >= $GLIB_REQUIRED_VERSION
	gtk+-3.0 >= $GTK_REQUIRED_VERSION
	gio-2.0 >= $GIO_REQUIRED_VERSION
	gmodule-2.0
	gthread-2.0
	dbus-glib-1 >= $DBUS_GLIB_REQUIRED_VERSION
)
#	gtk+-$GTK_API_VERSION >= $GTK_REQUIRED_VERSION


PKG_CHECK_MODULES(PLUGIN_MODULE,
        gtk+-3.0 >= $GTK_REQUIRED_VERSION
        gio-2.0 >= $GIO_REQUIRED_VERSION
        dbus-glib-1 >= $DBUS_GLIB_REQUIRED_VERSION
)
#       gtk+-$GTK_API_VERSION >= $GTK_REQUIRED_VERSION

MPD_PLUGIN_LDFLAGS="-export_dynamic -module -avoid-version -no-undefined"
case $host_os in
  darwin*)
    MPD_PLUGIN_LDFLAGS="${MPD_PLUGIN_LDFLAGS} -Wl,-bundle_loader,\$(top_builddir)/src/my-plugin-daemon"
    ;;
esac
AC_SUBST([MPD_PLUGIN_LDFLAGS])


PKG_CHECK_MODULES([DCONF], [dconf >= 0.13.4],
   [AC_DEFINE([HAVE_DCONF_0_13], [1], [Use DCONF >= 0.13.4])],
   [PKG_CHECK_MODULES([DCONF], [dconf >= 0.10.0],
       [AC_DEFINE([HAVE_DCONF_0_10], [1], [Use DCONF 0.10.0])
    ])
])
AC_SUBST(DCONF_CFLAGS)
AC_SUBST(DCONF_LIBS)

AC_PATH_PROG(GLIB_GENMARSHAL, glib-genmarshal)

GLIB_GSETTINGS

dnl ---------------------------------------------------------------------------
dnl - Check for D-Bus
dnl ---------------------------------------------------------------------------

dnl - Are we specifying a different dbus root ?
AC_ARG_WITH(dbus-sys,
        [AC_HELP_STRING([--with-dbus-sys=<dir>],
        [where D-BUS system.d directory is])])
AC_ARG_WITH(dbus-services,
        [AC_HELP_STRING([--with-dbus-services=<dir>],
        [where D-BUS services directory is])])
if ! test -z "$with_dbus_sys" ; then
        DBUS_SYS_DIR="$with_dbus_sys"
else
        DBUS_SYS_DIR='${sysconfdir}/dbus-1/system.d'
fi
AC_SUBST(DBUS_SYS_DIR)

# ---------------------------------------------------------------------------
# PolicyKit
# ---------------------------------------------------------------------------

POLKIT_REQUIRED=0.97
DBUS_REQUIRED=1.1.2

# PolicyKit detection; defaults to 'auto' (use it if it's available)
#
POLKIT_CFLAGS=
POLKIT_LIBS=
AC_ARG_ENABLE(polkit,
              AS_HELP_STRING([--enable-polkit],
                             [Enable PolicyKit support (auto)]),
              enable_polkit=$enableval,
              enable_polkit=auto)

if test "x$enable_polkit" = "xno" ; then
  HAVE_POLKIT=no
else
  HAVE_POLKIT=no
  PKG_CHECK_MODULES(POLKIT, polkit-gobject-1 >= $POLKIT_REQUIRED dbus-1 >= $DBUS_REQUIRED, HAVE_POLKIT=yes, HAVE_POLKIT=no)

  if test "x$enable_polkit" = "xyes" -a "x$HAVE_POLKIT" = "xno" ; then
    AC_MSG_ERROR(PolicyKit support explicity enabled but not available)
  fi

  if test "x$HAVE_POLKIT" = "xyes" ; then
    AC_DEFINE(HAVE_POLKIT, 1, [Defined if PolicyKit support is enabled])
  fi
fi
AM_CONDITIONAL(HAVE_POLKIT, test "x$HAVE_POLKIT" = "xyes")
AC_SUBST(POLKIT_CFLAGS)
AC_SUBST(POLKIT_LIBS)

# ---------------------------------------------------------------------------
# Plugins
# ---------------------------------------------------------------------------

plugindir='$(libdir)/my-plugin-daemon'
AC_SUBST([plugindir])

#dnl ---------------------------------------------------------------------------
#dnl - Finish
#dnl ---------------------------------------------------------------------------


# Turn on the additional warnings last, so warnings don't affect other tests.

AC_ARG_ENABLE(more-warnings,
	[AC_HELP_STRING([--enable-more-warnings],
	[Maximum compiler warnings])],
	set_more_warnings="$enableval",[
        	if test -d $srcdir/.git; then
        		set_more_warnings=yes
              	else
                  	set_more_warnings=no
              	fi
        ])
AC_MSG_CHECKING(for more warnings)
if test "$GCC" = "yes" -a "$set_more_warnings" != "no"; then
        AC_MSG_RESULT(yes)
        CFLAGS="\
        -Wall \
        -Wchar-subscripts -Wmissing-declarations -Wmissing-prototypes \
        -Wnested-externs -Wpointer-arith \
        -Wcast-align -Wsign-compare \
        $CFLAGS"

        for option in -Wno-strict-aliasing -Wno-sign-compare; do
                SAVE_CFLAGS="$CFLAGS"
                CFLAGS="$CFLAGS $option"
                AC_MSG_CHECKING([whether gcc understands $option])
                AC_TRY_COMPILE([], [],
                        has_option=yes,
                        has_option=no,)
                if test $has_option = no; then
                        CFLAGS="$SAVE_CFLAGS"
                fi
                AC_MSG_RESULT($has_option)
                unset has_option
                unset SAVE_CFLAGS
        done
        unset option
else
        AC_MSG_RESULT(no)
fi

#
# Enable Debug
#
AC_ARG_ENABLE(debug,
	[AC_HELP_STRING([--enable-debug],
	[turn on debugging])],
	, enable_debug=yes)
if test "$enable_debug" = "yes"; then
	DEBUG_CFLAGS="-DG_ENABLE_DEBUG"
else
	if test "x$enable_debug" = "xno"; then
		DEBUG_CFLAGS="-DG_DISABLE_ASSERT -DG_DISABLE_CHECKS"
  	else
    		DEBUG_CFLAGS=""
  	fi
fi
AC_SUBST(DEBUG_CFLAGS)

AC_OUTPUT([
Makefile
src/Makefile
plugins/Makefile
plugins/diskmonitor/Makefile
data/Makefile
data/org.freedesktop.MyPluginDaemon.plugins.diskmonitor.gschema.xml
po/Makefile.in
])


dnl ---------------------------------------------------------------------------
dnl - Show summary
dnl ---------------------------------------------------------------------------

echo "
                my-plugin-daemon $VERSION
                =============================
    prefix:                   ${prefix}
    exec_prefix:              ${exec_prefix}
    libdir:                   ${libdir}
    bindir:                   ${bindir}
    sbindir:                  ${sbindir}
    sysconfdir:               ${sysconfdir}
    sysconfsubdir:            ${sysconfsubdir}
    localstatedir:            ${localstatedir}
    plugindir:                ${plugindir}
    datadir:                  ${datadir}
    source code location:     ${srcdir}
    compiler:                 ${CC}
    cflags:                   ${CFLAGS}
"

#echo "
