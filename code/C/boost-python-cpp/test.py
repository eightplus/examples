#!/usr/bin/python
# -*- coding: utf-8 -*-

### BEGIN LICENSE
# Author: lixiang
# Copyright (C) 2013 ~ 2019 kobe24_lixiang@126.com
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 3, as published
# by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranties of
# MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
# PURPOSE.  See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program.  If not, see <http://www.gnu.org/licenses/>.
### END LICENSE

import os
import sys
import ctypes
try:
    libc = ctypes.CDLL('libc.so.6')
    libc.prctl(15, 'ydaemon', 0, 0, 0)
except: pass
import boost_py_cpp_module

def get_os():
    iface = boost_py_cpp_module.Daemon("OS")
    iface.detect_os_info()

    data = iface.get_os_info()

    return data

def get_username():
    try:
        import boost_py_cpp_module
        username = boost_py_cpp_module.get_username()
        return username
    except:
        return "root"



if __name__ == '__main__':
    print "OS info: ", get_os()
    print "User name: ", get_username()
