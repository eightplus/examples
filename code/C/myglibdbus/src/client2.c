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

// dbus-binding-tool --mode=glib-client --prefix=myglibdbus org.freedesktop.myglibdbus.xml > myglibdbus_proxy.h
// gcc client2.c myglibdbus_proxy.h `pkg-config --cflags --libs dbus-glib-1 dbus-1 gobject-2.0 glib-2.0` -o client2 -g -O0


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus-glib.h>

#include "myglibdbus_proxy.h"

#define MYGLIBDBUS_NAME	"org.freedesktop.myglibdbus"
#define MYGLIBDBUS_PATH	"/org/freedesktop/myglibdbus"
#define MYGLIBDBUS_INTERFACE "org.freedesktop.myglibdbus"

//接收dbus服务发送的signal
static void handler_info_alert(DBusGProxy *proxy, const char *msg, gpointer user_data)
{
    printf("Received signal and it says: %s\n", msg);
}

int main()
{
    DBusGConnection *bus;
    DBusGProxy *remote_object;
    GError *error = NULL;
    GMainLoop *mainloop;
    int ret = 0;

    //申请创建主循环
    mainloop = g_main_loop_new(NULL, FALSE);

    //和SESSION BUS建立连接
    bus = dbus_g_bus_get (DBUS_BUS_SESSION, &error);
    if (!bus) {
        printf("Couldn't connect to session bus: %s", error->message);
        return 1;
    }
    //获取一个对象代理
    remote_object = dbus_g_proxy_new_for_name (bus,
                                    MYGLIBDBUS_NAME,
                                    MYGLIBDBUS_PATH,
                                    MYGLIBDBUS_INTERFACE);

    if (!remote_object) {
        printf("Failed to get name owner: %s", error->message);
        return 1;
    }

    //增加一个信号，通知对象调用者需要捕获指定信号
    dbus_g_proxy_add_signal(remote_object, "info_alert", G_TYPE_STRING, G_TYPE_INVALID);

    //连接信号,将处理函数handler连接到指定的信号上。
    dbus_g_proxy_connect_signal(remote_object, "info_alert", G_CALLBACK(handler_info_alert), NULL, NULL);
    org_freedesktop_myglibdbus_work(remote_object, "test code", &ret, &error);
    if (error != NULL) {
        if(error->domain == DBUS_GERROR && error->code == DBUS_GERROR_REMOTE_EXCEPTION)
            printf("dbus send exception %s:%s", dbus_g_error_get_name(error), error->message);
        else
            printf("dbus send Error : %s\n", error->message);
        g_clear_error(&error);
    }
    printf("send_message return %d\n", ret);

    //进入到主循环中
    g_main_loop_run(mainloop);

    g_object_unref(G_OBJECT(remote_object));
 
    return 0;
}
