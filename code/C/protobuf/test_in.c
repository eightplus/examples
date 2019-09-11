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
 
int main(int argc, char* argv[])
{
    BufferMessage msg;

    msg.set_id(2019);
    msg.set_name("lixiang");
    msg.set_opt(1024);

    fstream output("./protobuf.log", ios::out | ios::trunc | ios::binary);
    if (!msg.SerializeToOstream(&output)){// SerializeToOstream 将对象序列化后写入一个 fstream 流
        cerr << "save data error." << endl;
        return -1;
    }

    return 0;
}
