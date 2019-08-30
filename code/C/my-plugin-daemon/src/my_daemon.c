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

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <glib.h>
#include <glib/gi18n.h>
#include <glib-object.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>

#include "my_daemon.h"
#include "my_daemon_glue.h"

#define PLUGIN_DBUS_PATH "/org/freedesktop/MyPluginDaemon"

#define MY_DAEMON_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), MY_TYPE_DAEMON, MyDaemonPrivate))

struct MyDaemonPrivate
{
    DBusGConnection *connection;
};

enum {
    PLUGIN_ACTIVATED,
    PLUGIN_DEACTIVATED,
    LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0, };

static void my_daemon_class_init(MyDaemonClass *klass);
static void my_daemon_init(MyDaemon *plugin_manager);
static void my_daemon_finalize(GObject *object);

G_DEFINE_TYPE (MyDaemon, my_daemon, G_TYPE_OBJECT)

static gpointer dbus_object = NULL;

GQuark my_daemon_error_quark(void)
{
    static GQuark ret = 0;
    if (ret == 0) {
        ret = g_quark_from_static_string("my_daemon_error");
    }

    return ret;
}

static gboolean register_manager(MyDaemon *dbus)
{
    GError *error = NULL;

    dbus->priv->connection = dbus_g_bus_get(DBUS_BUS_SESSION, &error);
    if (dbus->priv->connection == NULL) {
        if (error != NULL) {
            g_critical("error getting system bus: %s", error->message);
            g_error_free(error);
        }
        return FALSE;
    }

    dbus_g_connection_register_g_object(dbus->priv->connection, PLUGIN_DBUS_PATH, G_OBJECT(dbus));

    return TRUE;
}

/*
  Example:
  dbus-send --session --dest=org.freedesktop.MyPluginDaemon \
  --type=method_call --print-reply --reply-timeout=2000 \
  /org/freedesktop/MyPluginDaemon \
  org.freedesktop.MyPluginDaemon.start
*/
gboolean my_daemon_start(MyDaemon *dbus, GError **error)
{
    g_debug("Starting plugin dbus");

    return TRUE;
}

gboolean my_daemon_stop(MyDaemon *dbus, GError **error)
{
    g_debug("Stopping plugin dbus");

    return TRUE;
}

static GObject *my_daemon_constructor(GType type, guint n_construct_properties, GObjectConstructParam *construct_properties)
{
    MyDaemon *dbus;
    MyDaemonClass *klass;

    klass = MY_DAEMON_CLASS(g_type_class_peek(MY_TYPE_DAEMON));

    dbus = MY_DAEMON(G_OBJECT_CLASS(my_daemon_parent_class)->constructor(type, n_construct_properties, construct_properties));

    return G_OBJECT(dbus);
}

static void my_daemon_dispose(GObject *object)
{
    MyDaemon *dbus;
    GError *error;

    dbus = MY_DAEMON(object);

    my_daemon_stop(dbus, &error);

    G_OBJECT_CLASS(my_daemon_parent_class)->dispose(object);
}

static void my_daemon_class_init(MyDaemonClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->constructor = my_daemon_constructor;
    object_class->dispose = my_daemon_dispose;
    object_class->finalize = my_daemon_finalize;

    signals [PLUGIN_ACTIVATED] =
            g_signal_new ("plugin-activated",
                          G_TYPE_FROM_CLASS(object_class),
                          G_SIGNAL_RUN_LAST,
                          G_STRUCT_OFFSET (MyDaemonClass, plugin_activated),
                          NULL,
                          NULL,
                          g_cclosure_marshal_VOID__STRING,
                          G_TYPE_NONE,
                          1, G_TYPE_STRING);
    signals [PLUGIN_DEACTIVATED] =
            g_signal_new ("plugin-deactivated",
                          G_TYPE_FROM_CLASS(object_class),
                          G_SIGNAL_RUN_LAST,
                          G_STRUCT_OFFSET (MyDaemonClass, plugin_deactivated),
                          NULL,
                          NULL,
                          g_cclosure_marshal_VOID__STRING,
                          G_TYPE_NONE,
                          1, G_TYPE_STRING);

    g_type_class_add_private(klass, sizeof(MyDaemonPrivate));

    dbus_g_object_type_install_info(MY_TYPE_DAEMON, &dbus_glib_my_daemon_object_info);
}

static void my_daemon_init(MyDaemon *dbus)
{

    dbus->priv = MY_DAEMON_GET_PRIVATE(dbus);
}

static void my_daemon_finalize(GObject *object)
{
    MyDaemon *dbus;

    g_return_if_fail(object != NULL);
    g_return_if_fail(MY_IS_DAEMON(object));

    dbus = MY_DAEMON(object);

    g_return_if_fail(dbus->priv != NULL);

    G_OBJECT_CLASS(my_daemon_parent_class)->finalize(object);
}

MyDaemon *my_daemon_new (void)
{
    if (dbus_object != NULL) {
        g_object_ref(dbus_object);
    }
    else {
        gboolean res;

        dbus_object = g_object_new(MY_TYPE_DAEMON, NULL);
        g_object_add_weak_pointer(dbus_object, (gpointer *)&dbus_object);
        res = register_manager(dbus_object);
        if (!res) {
            g_object_unref(dbus_object);
            return NULL;
        }
    }

    return MY_DAEMON(dbus_object);
}
