#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([myglibdbus], [1.0.0], [https://eightplus.github.io/])


#AC_CONFIG_SRCDIR是一个安全行检查，用于确认其所指定位置确有一个src/server.c。我们需要把它的参数设定为项目中一个独一无二的代码文件
AC_CONFIG_SRCDIR([src/server.c])
AC_CONFIG_HEADERS([config.h])

#生成 Makefile 的过程通常产生非常长的信息，我们一般不需要看这么多，而且大量无用的信息刷屏可能会让你忽视夹在其中的警告和错误信息。
#AM_SILENT_RULES([yes]) 将屏蔽这些消息，只输出一些总结信息。一个真正的程序员可能会使用 AM_SILENT_RULES([no])，然后运行 ./configure --enable-silent-rules 来保证政治正确性
#如果你开启了静默模式，但还是希望能检查所有输出信息，可以使用 make V=1 命令（V 表示 Verbose）
#AM_SILENT_RULES([yes])

AM_INIT_AUTOMAKE([1.9 no-dist-gzip dist-xz tar-ustar check-news -Wall foreign])
AM_MAINTAINER_MODE([enable])


# Checks for programs.
AC_PROG_CC


#PKG_PROG_PKG_CONFIG，它会在项目中加入用于检查和引用库的工具 —— pkg-config。这个宏并非 AC_ 或者 AM_ 开头，而是 PKG_，说明这是一个由 pkg-config 提供的功能。在“库”部分，我们用 pkg-config(PKG_CHECK_MODULES)引入我们需要的库。
#PKG_CHECK_MODULES 的第一个参数 APP_SKELETON 为程序名，第二个参数列出了一系列 pkg-config 模块。一个模块代表了 pkg-config 可以识别的一个库。这个宏使用 pkg-config 查找计算机中的库，然后生成两个变量：APP_SKELETON_CFLAGS —— 包含了库的引用信息（类似于 gcc 中的 -I 参数）；APP_SKELETON_LIBS —— 包含了库的链接信息（类似于 gcc 中的 -L 和 -l 参数）。这些变量可用于 Makefile 的编辑
#
PKG_PROG_PKG_CONFIG

# Checks for libraries.
# GLib min/max required versions
AC_DEFINE([GLIB_VERSION_MAX_ALLOWED], [GLIB_VERSION_2_36],
        [Warn on use of APIs added after GLib 2.36])
AC_DEFINE([GLIB_VERSION_MIN_REQUIRED], [GLIB_VERSION_2_36],
        [Warn on use of APIs deprecated before GLib 2.36])

DBUS_GLIB_REQUIRED_VERSION=0.74
GLIB_REQUIRED_VERSION=2.36.0

#这个宏表示我们将使用 glib-2.0,gthread-2.0和dbus-glib-1三个库。如果你不知道需要用到库的模块名，可以到 /usr/share/pkgconfig 和 /usr/lib/pkgconfig 中查找以 .pc 结尾的文件（译者注：也可以在终端中输入 pkg-config --list-all 查看）
#(1)将所有用到的库绑定到 MY_GLIB_DBUS 变量中
PKG_CHECK_MODULES(MY_GLIB_DBUS,
        glib-2.0 >= $GLIB_REQUIRED_VERSION
        gthread-2.0
        dbus-glib-1 >= $DBUS_GLIB_REQUIRED_VERSION
)

#(2)更为稳妥的做法应该为将每个库分离对待，这样有利于模块的灵活调用。
#PKG_CHECK_MODULES([GLIB], [
#    glib-2.0
#])
#PKG_CHECK_MODULES([GTHREAD], [
#    gthread-2.0
#])
#PKG_CHECK_MODULES([GDBUS], [
#    dbus-glib-1
#])

PKG_CHECK_MODULES([GMODULE], [
    gmodule-2.0
])




#(3)
## Dbus lib detection
#PKG_CHECK_MODULES(DBUS, dbus-1 >= 1.1, have_dbus=yes, have_dbus=no)
#if test x$have_dbus = xno ; then
#	AC_MSG_ERROR([DBus lib not found])
#fi
#AM_CONDITIONAL(HAVE_DBUS, test x$have_dbus = xyes)
#AC_SUBST(DBUS_CFLAGS)
#AC_SUBST(DBUS_LIBS)
#
## Glib lib detection
#PKG_CHECK_MODULES(DBUS_GLIB, gobject-2.0 >= 2.6, have_glib=yes, have_glib=no)
#if test x$have_glib = xno ; then
#	AC_MSG_ERROR([GLib lib not found])
#fi
#AM_CONDITIONAL(HAVE_GLIB, test x$have_glib = xyes)
#AC_SUBST(DBUS_GLIB_CFLAGS)
#AC_SUBST(DBUS_GLIB_LIBS)




AC_PATH_PROG(GLIB_GENMARSHAL, glib-genmarshal)


# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
