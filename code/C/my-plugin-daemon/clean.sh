#!/bin/sh
# Run this to clear temporary files.

rm -f ./configure
rm -f ./Makefile.in
rm -f ./Makefile
rm -f ./aclocal.m4
rm -f ./src/Makefile.in
rm -f ./src/Makefile
rm -f ./src/my_daemon_glue.h
rm -f ./src/my_daemon_client.h
rm -f ./src/*.o
rm -f ./INSTALL
rm -f ./missing
rm -f ./install-sh
rm -f ./compile
rm -f ./config.h.in
rm -f ./config.h
rm -f ./config.h.in~
rm -f ./config.log
rm -f ./config.status
rm -f ./COPYING
rm -rf ./autom4te.cache
rm -rf ./src/.deps/
rm -rf ./src/.libs/
rm -f ./src/my-plugin-daemon
rm -f ./src/*.o
rm -f ./depcomp
rm -f ./stamp-h1
rm -f ./config.guess
rm -f ./config.sub
rm -f ./libtool
rm -f ./ltmain.sh
rm -f ./xmldocs.make
rm -rf ./m4
rm -f ./omf.make
rm -rf ./po

rm -f ./plugins/Makefile.in
rm -f ./plugins/Makefile
rm -f ./plugins/diskmonitor/Makefile.in
rm -f ./plugins/diskmonitor/Makefile
rm -f ./plugins/diskmonitor/*.o
rm -f ./plugins/diskmonitor/*.lo
rm -f ./plugins/diskmonitor/*.la

rm -f ./data/Makefile.in
rm -f ./data/Makefile
rm -f ./data/*.valid
rm -f ./data/*.xml
