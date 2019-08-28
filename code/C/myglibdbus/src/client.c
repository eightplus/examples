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

// gcc client.c `pkg-config --cflags --libs dbus-glib-1 dbus-1 gobject-2.0 glib-2.0` -o client -g -O0


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus-glib.h>
#include <glib/giochannel.h>

#define MYGLIBDBUS_NAME	"org.freedesktop.myglibdbus"
#define MYGLIBDBUS_PATH	"/org/freedesktop/myglibdbus"
#define MYGLIBDBUS_INTERFACE "org.freedesktop.myglibdbus"

//接收dbus服务发送的signal
static void handler_info_alert(DBusGProxy *proxy, const char *msg, gpointer user_data)
{
    printf("Received signal and it says: %s\n", msg);
}

//调用dbus服务的method
static void send_message(DBusGProxy *remote_object)
{
    //dbus_g_proxy_call_no_reply (proxy, "work", G_TYPE_STRING, "Hello world!", G_TYPE_INVALID);//调用dbus服务的work函数
    GError *err = NULL;
    int ret = 0;
    if (!dbus_g_proxy_call(remote_object, "work", &err,
        G_TYPE_STRING, "Hello world!", G_TYPE_INVALID,
        G_TYPE_INT, &ret, G_TYPE_INVALID)) {
        if (err != NULL) {
            if(err->domain == DBUS_GERROR && err->code == DBUS_GERROR_REMOTE_EXCEPTION)
                printf("dbus send exception %s:%s",dbus_g_error_get_name(err), err->message);
            else
                printf("dbus send Error : %s\n", err->message);
            g_clear_error(&err);
        }
    }
    else {
        printf("send_message return %d\n", ret);
    }
}

static void receive_message(DBusGProxy *remote_object)
{
    GError *err = NULL;
    gchar *reply_str = NULL;
    if (!dbus_g_proxy_call(remote_object, "receive", &err,
        G_TYPE_STRING, "I am lixiang!", G_TYPE_INVALID,
        G_TYPE_STRING, &reply_str, G_TYPE_INVALID)) {
        if (err != NULL) {
            if(err->domain == DBUS_GERROR && err->code == DBUS_GERROR_REMOTE_EXCEPTION)
                printf("dbus receive exception %s:%s",dbus_g_error_get_name(err), err->message);
            else
                printf("dbus receive Error : %s\n", err->message);
            g_clear_error(&err);
        }
    }
    else {
        if (reply_str) {
            printf("receive_message return %s\n", reply_str);
            g_free(reply_str);
        }
    }
}

static void get_list_data(DBusGProxy *remote_object)
{
    GError *err = NULL;
    char **reply_list;
    char **reply_ptr;
    if (!dbus_g_proxy_call(remote_object, "get_list", &err,
                            G_TYPE_STRING, "ready to get list!", G_TYPE_INVALID,
                            G_TYPE_STRV, &reply_list, G_TYPE_INVALID)) {
        if (err != NULL) {
            if(err->domain == DBUS_GERROR && err->code == DBUS_GERROR_REMOTE_EXCEPTION)
                printf("dbus get_list exception %s:%s",dbus_g_error_get_name(err), err->message);
            else
                printf("dbus get_list Error : %s\n", err->message);
            g_clear_error(&err);
        }
    }
    else {
        printf("list data:\n");
        for (reply_ptr = reply_list; *reply_ptr; reply_ptr++)
            printf ("\"%s\" ", *reply_ptr);
        printf("\n");
        g_strfreev(reply_list);
    }
}

static void get_array_data(DBusGProxy *remote_object)
{
    GError *err = NULL;
    GValueArray *reply_struct;
    guint i;
    if (!dbus_g_proxy_call(remote_object, "get_array", &err,
                            G_TYPE_INVALID,
                            G_TYPE_VALUE_ARRAY, &reply_struct, G_TYPE_INVALID)) {
        if (err != NULL) {
            if(err->domain == DBUS_GERROR && err->code == DBUS_GERROR_REMOTE_EXCEPTION)
                printf("dbus get_array exception %s:%s",dbus_g_error_get_name(err), err->message);
            else
                printf("dbus get_array Error : %s\n", err->message);
            g_clear_error(&err);
        }
    }
    else {
        printf("array data:\n");
        for (i = 0; i < reply_struct->n_values; i++) {
            GValue strval = { 0, };
            g_value_init(&strval, G_TYPE_STRING);
            if (!g_value_transform(g_value_array_get_nth(reply_struct, i), &strval))
                g_value_set_static_string(&strval, "(couldn't transform to string)");
            g_print("%s: %s\n", g_type_name(G_VALUE_TYPE(g_value_array_get_nth(reply_struct, i))), g_value_get_string(&strval));
        }
        g_value_array_free(reply_struct);
        printf("\n");
    }
}

static void print_dict(gpointer key, gpointer value, gpointer data)
{
    printf ("key:%s, value:%s\n", (char *)key, (char *)value);
}

static void get_dict_data(DBusGProxy *remote_object)
{
    GError *err = NULL;
    GHashTable *reply_dict;
    if (!dbus_g_proxy_call(remote_object, "get_dict", &err,
                              G_TYPE_INVALID,
                              DBUS_TYPE_G_STRING_STRING_HASHTABLE, &reply_dict, G_TYPE_INVALID)) {
        if (err != NULL) {
            if(err->domain == DBUS_GERROR && err->code == DBUS_GERROR_REMOTE_EXCEPTION)
                printf("dbus get_dict exception %s:%s",dbus_g_error_get_name(err), err->message);
            else
                printf("dbus get_dict Error : %s\n", err->message);
            g_clear_error(&err);
        }
    }
    else {
        printf("dict data:\n");
        g_hash_table_foreach(reply_dict, print_dict, NULL);
        g_hash_table_destroy(reply_dict);
        printf("\n");
    }
}

static void shell_help(void)
{
    printf("\s\tsend message\n"
           "\r\treceive message\n"
           "\tq\tQuit\n");
}

static gboolean handler_gio_channel(GIOChannel *source, GIOCondition condition, gpointer data)
{
    int rc;
    char buf[1024];
    DBusGProxy *remote_object = (DBusGProxy *)data;
    if (condition != G_IO_IN) {
        return TRUE;
    }
    /* we've received something on stdin */
    rc = fscanf(stdin, "%s", buf);
    if (rc <= 0) {
        printf("null\n");
        return TRUE;
    }
    if (!strcmp(buf, "h")) {
        shell_help();
    }
    else if (!strcmp(buf, "?")) {
        shell_help();
    }
    else if (!strcmp(buf, "s")) {
        send_message(remote_object);
    }
    else if (!strcmp(buf, "r")) {
        receive_message(remote_object);
    }
    else if (!strcmp(buf, "a")) {
        get_list_data(remote_object);
    }
    else if (!strcmp(buf, "b")) {
        get_array_data(remote_object);
    }
    else if (!strcmp(buf, "c")) {
        get_dict_data(remote_object);
    }
    else if (!strcmp(buf, "q")) {
        exit(0);
    }
    else {
        printf("Unknown command %s\n", buf);
    }

    return TRUE;
}

int main()
{
    DBusGConnection *bus;
    DBusGProxy *remote_object;
    GError *error = NULL;
    GMainLoop *mainloop;
    GIOChannel *chan;
    guint source;

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
    dbus_g_proxy_connect_signal(remote_object, "info_alert", G_CALLBACK (handler_info_alert), NULL, NULL);

    //监控输入事件
    chan = g_io_channel_unix_new(0);
    source = g_io_add_watch(chan, G_IO_IN, handler_gio_channel, remote_object);

    //进入到主循环中
    g_main_loop_run(mainloop);

    g_object_unref(G_OBJECT(remote_object));
 
    return 0;
}
