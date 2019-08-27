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


#ifndef __MY_GLIB_DBUS_H__
#define __MY_GLIB_DBUS_H__

#include <glib-object.h>
#include <glib.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>


G_BEGIN_DECLS

/* signal */
enum {
    SIG_INFO_ALERT,
    SIG_LAST_SIGNAL,
};

#define MYGLIBDBUS_NAME	"org.freedesktop.myglibdbus"
#define MYGLIBDBUS_PATH	"/org/freedesktop/myglibdbus"
#define MYGLIBDBUS_INTERFACE "org.freedesktop.myglibdbus"

GType myglibdbus_get_type(void);//向GObject库所提供的类型管理系统提供要注册的MyGlibDbus类类型的相关信息,可以不实现，但必须要声明
#define MYGLIBDBUS_TYPE_OBJECET	(myglibdbus_get_type())

#define MY_GLIB_DBUS(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), MYGLIBDBUS_TYPE_OBJECET, MyGlibDbus))
#define MY_GLIB_DBUS_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST((k), MYGLIBDBUS_TYPE_OBJECET, MyGlibDbusClass))
#define MY_GLIB_IS_DBUS(o)	(G_TYPE_CHECK_INSTANCE_TYPE((o), MYGLIBDBUS_TYPE_OBJECET))
#define MY_GLIB_IS_DBUS_CLASS(k)	(G_TYPE_CHECK_CLASS_TPYE((k), MYGLIBDBUS_TYPE_OBJECET))
#define MY_GLIB_DBUS_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS((o), MYGLIBDBUS_TYPE_OBJECET, MyGlibDbusClass))

//定义对象： 在 GObject 中，类是两个结构体的组合，一个是实例结构体，另一个是类结构体，上例中MyGlibDbus是实例结构体，MyGlibDbusClass是类结构体
typedef struct _MyGlibDbusPrivate MyGlibDbusPrivate;
typedef struct _MyGlibDbus MyGlibDbus;
typedef struct _MyGlibDbusClass MyGlibDbusClass;


struct _MyGlibDbus {
    GObject parent;
    MyGlibDbusPrivate *priv;
};

struct _MyGlibDbusClass {
    GObjectClass parent_class;
    /* signals */
    //void (*event)(MyGlibDbus* dbus, GValue *value);
};


MyGlibDbus *myglibdbus_new(void);

gint myglibdbus_work(MyGlibDbus *dbus, gchar *msg, DBusGMethodInvocation *ret_value, GError **error);
gint *myglibdbus_receive(MyGlibDbus *dbus, gchar *msg, DBusGMethodInvocation *ret_value, GError **error);
gint myglibdbus_exit(MyGlibDbus *dbus, DBusGMethodInvocation *ret_value, GError **error);

G_END_DECLS

int dbus_init(void);
gint info_alert_dbus_signal_sendmsg(MyGlibDbus *dbus, gchar *info);

#endif // __MY_GLIB_DBUS_H__
