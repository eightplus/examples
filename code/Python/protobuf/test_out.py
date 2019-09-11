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

    fp = None
    alumni_book = AlumniBook()
    try:
        fp = open(sys.argv[1], 'r')
        alumni_book.ParseFromString(fp.read())
    except IOError,error:
        print error
    finally:
        if fp != None:
            fp.close()

    for mate in alumni_book.mate:
        print "----------mate----------"
        print "name:", mate.name
        print "id:", mate.id
        print "email:", mate.email
        for phone in mate.phone:
            print "phone.number:", phone.number
            print "phone.type:", phone.type
