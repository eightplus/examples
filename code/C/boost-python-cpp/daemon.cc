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

#include "daemon.h"

#include <boost/python.hpp>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

using namespace boost::python;

namespace {

string get_username()
{
    std::string user_name = std::getenv("USER");

    if (user_name.empty()) {
        user_name = std::getenv("USERNAME");
    }

    if (user_name.empty()) {
        struct passwd *pwd;
        pwd = getpwuid(getuid());
        return std::string(pwd->pw_name);
    }

    return user_name;
}

}

Daemon::Daemon(std::string mark) : m_mark(mark)
{
    //std::cout << m_mark << std::endl;
}

Daemon::~Daemon()
{

}

void Daemon::detect_os_info()
{
    char szOSNameTmp[128] = "Ubuntu";
    char szReleaseTmp[128] = "16.04";
    char szCodeNameTmp[128] = "xenial";

    char szBuf[256] = "";

    FILE *stream = popen("lsb_release -i -r -c", "r");
    if (stream) {
        fread(szBuf, sizeof(char), sizeof(szBuf), stream);
        sscanf(szBuf,"%*s%*s%s%*s%s%*s%s", szOSNameTmp, szReleaseTmp, szCodeNameTmp);
        pclose(stream);
    }

    os_msg = std::string(szOSNameTmp) + " " + std::string(szReleaseTmp) + " " + std::string(szCodeNameTmp);
}

string Daemon::get_os_info() const
{
    return os_msg;
}

//boost_py_cpp_module为module名，必要和生成的so库目标名一样
BOOST_PYTHON_MODULE(boost_py_cpp_module)
{
    // boost::python::init          boost::python::def
    class_<Daemon, boost::noncopyable > ("Daemon", "This is the test daemon for python and C++", init<std::string>())//构造函数
            .def("detect_os_info", &Daemon::detect_os_info)//成员函数
            .def("get_os_info", &Daemon::get_os_info)//成员函数
            ;
    def("get_username", &get_username);//普通函数

}
