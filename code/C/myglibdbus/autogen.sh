#!/bin/sh
# Run this to generate all the initial makefiles, etc.

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.
test "$srcdir" || srcdir=.

PKG_NAME="myglibdbus"

(test -f $srcdir/configure.ac) || {
    echo -n "**Error**: Directory "\`$srcdir\'" does not look like the"
    echo " top-level $PKG_NAME directory"
    exit 1
}

REQUIRED_AUTOMAKE_VERSION=1.9

aclocal
autoconf
autoheader

(test -f $srcdir/NEWS) || {
    touch NEWS
}
(test -f $srcdir/NEWS) || {
    touch README
}
(test -f $srcdir/NEWS) || {
    touch AUTHORS
}
(test -f $srcdir/NEWS) || {
    touch ChangeLog
}
automake --add-missing
