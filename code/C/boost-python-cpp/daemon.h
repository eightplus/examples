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

#ifndef _DAEMON_H_
#define _DAEMON_H_

#include <string>

using namespace std;

class Daemon
{
public:
    Daemon(std::string mark);
    virtual ~Daemon();

    void detect_os_info();
    string get_os_info() const;

private:
    std::string m_mark;
    std::string os_msg;
};

#endif // _DAEMON_H_
