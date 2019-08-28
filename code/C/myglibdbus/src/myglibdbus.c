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


#include "myglibdbus.h"
#include "myglibdbus-glue.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

typedef void*(*PFUNC)(void*);
static guint signals[SIG_LAST_SIGNAL] = { 0 };
void signal_handler(int signo);

gint info_alert_dbus_signal_sendmsg(MyGlibDbus *dbus, gchar *info)
{
    g_signal_emit(dbus,
        signals[SIG_INFO_ALERT],
        0,
        info);
    return 0;
}


/*
实现类类型的定义
G_DEFINE_TYPE可以让 GObject 库的数据类型系统能够识别我们所定义的MyGlibDbus类类型，它接受三个参数，第一个参数是类名，即MyGlibDbus；第二个参数则是类的成员函数（面向对象术语称之为“方法”或“行为”）名称的前缀，例如myglibdbus_get_type函数即为MyGlibDbus类的一个成员函数，“myglibdbus” 是它的前缀，第二个参数确定了类成员，即MyGlibDbus类相应的成员函数都是以myglibdbus开头，如myglibdbus_get_type、myglibdbus_init、myglibdbus_class_init；第三个参数则指明MyGlibDbus类类型的父类型为G_TYPE_OBJECT
*/
G_DEFINE_TYPE(MyGlibDbus, myglibdbus, G_TYPE_OBJECT);


MyGlibDbus *myglibdbus_new(void)
{
    return (MyGlibDbus*) g_object_new(MYGLIBDBUS_TYPE_OBJECET, NULL);
}

//声明类的函数(类成员的构造函数)
void myglibdbus_init(MyGlibDbus *dbus)
{

}

/*
 * 声明类的函数(类结构的构造函数，与类成员构造函数区别在于，该构造函数只在该类定义时运行一次，常用来进行消息信号的初始化等。而myglibdbus_init则在创建成员时都会调用一次(如obj = g_object_new))
 * g_signal_new的第一个参数是信号的名字，与XML文件匹配，不管XML中是info_alert还是是InfoAlert，则此处都需要写为info_alert，规则基本就是大写转小写、单词间加下划线，且第一个字符必须是字母
*/
void myglibdbus_class_init(MyGlibDbusClass * kclass)
{
    signals[SIG_INFO_ALERT] = g_signal_new("info_alert",
                        G_OBJECT_CLASS_TYPE(kclass),
                        G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
                        0,
                        NULL,
                        NULL,
			g_cclosure_marshal_VOID__STRING,/*the function to translate arrays of parameter values to signal emissions into C language callback invocations. 在gmarshal.h中定，如果结构参数复杂需自行定义*/
                  	G_TYPE_NONE,/*返回值，因为信号没有返回，所以为NONE*/
			1,/*参数数目*/
                        G_TYPE_STRING);/*参数类型*/
}


//实现method
gint myglibdbus_work(MyGlibDbus *dbus, gchar *msg, DBusGMethodInvocation *ret_value, GError **error)
{
    gint ret = 0;
    g_printf("msg:%s\n", msg);
    info_alert_dbus_signal_sendmsg(dbus, "work method");
    ret = 24;
    dbus_g_method_return(ret_value, ret);

    //if error, call g_set_error ??? 
    return 0;
}

gint myglibdbus_receive(MyGlibDbus *dbus, gchar *msg, DBusGMethodInvocation *ret_value, GError **error)
{
    gchar *result;
    g_printf("msg:%s\n", msg);
    info_alert_dbus_signal_sendmsg(dbus, "receive method");
    result = g_strdup("hahaha!!!");
    dbus_g_method_return(ret_value, result);
    g_free(result);
    return 0;
}

gint myglibdbus_exit(MyGlibDbus *dbus, DBusGMethodInvocation *ret_value, GError **error)
{
    gint ret = 0;

    dbus_g_method_return(ret_value, ret);

    exit(0);
    return ret;
}

gboolean myglibdbus_get_list(MyGlibDbus *dbus, const char *msg, DBusGMethodInvocation *ret_value, /*char ***ret, */GError **error)
{
    char **ret;
    printf("get_list arg:%s\n", msg);
    ret = g_new(char *, 3);
    (ret)[0] = g_strdup("Hello");
    (ret)[1] = g_strdup("China");
    (ret)[2] = NULL;
    dbus_g_method_return(ret_value, ret);
    g_strfreev(ret);

    return TRUE;
}

gboolean myglibdbus_get_array(MyGlibDbus *dbus, DBusGMethodInvocation *ret_value, /*GValueArray **ret, */GError **error)
{
    GValueArray *ret;
    ret = g_value_array_new(2);
    g_value_array_prepend(ret, NULL);
    g_value_init(g_value_array_get_nth(ret, 0), G_TYPE_STRING);
    g_value_set_string(g_value_array_get_nth(ret, 0), "kobe");
    g_value_array_prepend(ret, NULL);
    g_value_init(g_value_array_get_nth(ret, 0), G_TYPE_UINT);
    g_value_set_uint(g_value_array_get_nth(ret, 0), 24);
    dbus_g_method_return(ret_value, ret);
    g_value_array_free(ret);

    return TRUE;
}

gboolean myglibdbus_get_dict(MyGlibDbus *dbus, DBusGMethodInvocation *ret_value, /*GHashTable **ret, */GError **error)
{
    GHashTable *ret;
    ret = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(ret, "name", "lixiang");
    g_hash_table_insert(ret, "id", "1234567890");
    dbus_g_method_return(ret_value, ret);
    g_hash_table_destroy(ret);

    return TRUE;
}

static GMainLoop *loop = NULL;
MyGlibDbus * dbus;

int dbus_init(void)
{
    DBusGConnection * connection = NULL;
    DBusConnection *connect;
    DBusGProxy * dbus_proxy = NULL;
    GError * error = NULL;
    guint request_name_result;
    gint ret;
    dbus_g_thread_init();

    dbus = (MyGlibDbus*)myglibdbus_new();
    loop = g_main_loop_new(NULL, FALSE);
	
    connection = dbus_g_bus_get(DBUS_BUS_SESSION, &error);
    if(connection == NULL){
        g_error("%s", error->message);
        goto out;
    }

    //申请一个会话总线  DBUS_SERVICE_DBUS: org.freedesktop.DBus    DBUS_PATH_DBUS: /org/freedesktop/DBus   DBUS_INTERFACE_DBUS:org.freedesktop.DBus
    dbus_proxy = dbus_g_proxy_new_for_name(connection, DBUS_SERVICE_DBUS,
                                            DBUS_PATH_DBUS, DBUS_INTERFACE_DBUS);

    //调用dbusdaemon的函数“RequestName”，申请一个DBUS名为org.freedesktop.myglibdbus的注册信息
    ret = dbus_g_proxy_call(dbus_proxy, "RequestName", &error,
            G_TYPE_STRING, MYGLIBDBUS_NAME,
                            G_TYPE_UINT,
                            DBUS_NAME_FLAG_DO_NOT_QUEUE,
                            G_TYPE_INVALID,
                            G_TYPE_UINT, &request_name_result,
                            G_TYPE_INVALID);
    if(!ret){
        g_error("RequestName failed:%s", error->message);
        g_error_free(error);
        exit(EXIT_FAILURE);
    }

    g_object_unref(G_OBJECT(dbus_proxy));

    /* already running */
    if(request_name_result != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
        exit(EXIT_FAILURE);

    dbus_g_object_type_install_info(MYGLIBDBUS_TYPE_OBJECET,
                    &dbus_glib_myglibdbus_object_info);//向dbus-glib登记对象信息
    //申请之前定义的一个对象MyGlibDbus，将该对象与bus绑定
    dbus_g_connection_register_g_object(connection, MYGLIBDBUS_PATH, G_OBJECT(dbus));

    signal(SIGINT, &signal_handler);
    g_main_loop_run(loop);//申请创建一个主循环
	
out:
    g_main_loop_unref(loop);
    g_object_unref(dbus);
    return 1;
}


void signal_handler(int signo)
{
    g_main_loop_unref(loop);
    g_object_unref(dbus);
    fprintf(stderr, "get out now\n");
    exit(1);
}
