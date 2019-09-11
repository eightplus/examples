/*
 * Copyright (C) 2018 ~ 2020 kobe24_lixiang@126.com
 *
 * Authors:
 *  lixiang    kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "myinfo.pb.h"
#include <fstream>
#include <iostream>
 
using namespace std;
using namespace com::xiaoming::protobuf;
 
void printMsg(const BufferMessage &msg)
{
    cout << "id:" << msg.id() << endl;
    cout << "name:" << msg.name() << endl;
    if (msg.has_opt()) {
        cout << "opt:" << msg.opt() << endl;
    }
}
 
int main(int argc, char* argv[])
{
    BufferMessage msg;
    fstream input("./protobuf.log", ios::in | ios::binary);
    if (!msg.ParseFromIstream(&input)){// ParseFromIstream 从一个 fstream 流中读取信息并反序列化
        cerr << "read data from file error." << endl;
        return -1;
    }

    printMsg(msg);

    return 0;
}
