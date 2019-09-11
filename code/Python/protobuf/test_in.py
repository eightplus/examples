#!/usr/bin/python
# -*- coding: utf-8 -*-

### BEGIN LICENSE
# Copyright (C) 2019 ~ 2020 lixiang
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

import sys
from myinfo_pb2 import AlumniBook, ClassMate

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print "usage: python %s filename." % sys.argv[0]
        sys.exit(-1)
    alumni_book = AlumniBook()

    fp = None
    try:
        fp = open(sys.argv[1], 'a+')
        alumni_book.ParseFromString(fp.read())
    except IOError,error:
        print "file error:", error
    finally:
        if fp != None:
            fp.close()

    mate = alumni_book.mate.add()
    mate.id = 349098792
    mate.name='lixiang'
    mate.email = 'kobe24_lixiang@126.com'
    phone = mate.phone.add()
    phone.number = '18007310000'
    phone.type = ClassMate.MOBILE

    fp = None
    try:
        fp = open(sys.argv[1], 'w')
        fp.write(alumni_book.SerializeToString())
    except IOError,error:
        print error
    finally:
        if fp != None:
            fp.close()
