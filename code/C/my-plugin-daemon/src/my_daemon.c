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
#include <gio/gio.h> // 它很重要，否则安装deb包后运行时，在加载plugin的过程中报段错误，但本地编译和运行的时候没有问题，我目前没有想通该原因，望看到的人指导下我

#include "my_daemon.h"
#include "my_daemon_glue.h"
#include "my_plugin_info.h"

#define PLUGIN_DBUS_PATH "/org/freedesktop/MyPluginDaemon"
#define DEFAULT_SETTINGS_PREFIX "org.freedesktop.MyPluginDaemon" //插件gschema文件的前缀
#define PLUGIN_EXT ".my-plugin" //用于匹配配置说明文件，比如diskmonitor.my-plugin

#define MY_DAEMON_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), MY_TYPE_DAEMON, MyDaemonPrivate))

struct MyDaemonPrivate
{
    DBusGConnection *connection;
    GSList *plugins;
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

static void attempt_activate_plugin(MyPluginInfo *info, gpointer user_data)
{
    if (my_plugin_info_get_enabled(info)) {
        gboolean res;
        res = my_plugin_info_activate(info);
        if (res) {
            g_debug("Plugin %s: active", my_plugin_info_get_id(info));
        }
        else {
            g_debug("Plugin %s: activation failed", my_plugin_info_get_id(info));
        }
    }
    else {
        g_debug("Plugin %s: inactive", my_plugin_info_get_id(info));
    }
}

static gint compare_plugin_id(MyPluginInfo *a, MyPluginInfo *b)
{
    const char *id1;
    const char *id2;

    id1 = my_plugin_info_get_id(a);
    id2 = my_plugin_info_get_id(b);

    if (id1 == NULL || id2 == NULL) {
        return -1;
    }

    return strcmp(id1, id2);
}

static void on_plugin_activated(MyPluginInfo *info, MyDaemon *manager)
{
    const char *plugin_id;
    plugin_id = my_plugin_info_get_id(info);
    g_debug("MyDaemon: emitting plugin-activated %s", plugin_id);
    g_signal_emit(manager, signals [PLUGIN_ACTIVATED], 0, plugin_id);
}

static void on_plugin_deactivated(MyPluginInfo *info, MyDaemon *manager)
{
    const char *plugin_id;
    plugin_id = my_plugin_info_get_id(info);
    g_debug("MyDaemon: emitting plugin-deactivated %s", plugin_id);
    g_signal_emit(manager, signals [PLUGIN_DEACTIVATED], 0, plugin_id);
}

static gboolean is_item_in_schema(const char * const *items, const char *item)
{
    while (*items) {
        if (g_strcmp0(*items++, item) == 0)
            return TRUE;
    }
    return FALSE;
}

static gboolean is_schema(const char *schema)
{
    return is_item_in_schema(g_settings_list_schemas(), schema);
}

static void get_plugin_info_from_file(MyDaemon *manager, const char *filename)
{
    MyPluginInfo *info;
    char *schema_path;
    GSList *l;

    //从/usr/lib/x86_64-linux-gnu/my-plugin-daemon/diskmonitor.my-plugin文件中获取插件的相关信息
    info = my_plugin_info_from_file(filename);
    if (info == NULL) {
        goto out;
    }
    else {
        g_debug("plugin name:%s", my_plugin_info_get_name(info));
        g_debug("plugin description%s", my_plugin_info_get_description(info));
        g_debug("plugin website:%s", my_plugin_info_get_website(info));
        g_debug("plugin copyright:%s", my_plugin_info_get_copyright(info));
        g_debug("plugin id:%s", my_plugin_info_get_id(info));
        char **authors = my_plugin_info_get_authors(info);
        for (int i = 0; authors && authors[i]; i++)
            g_debug("plugin authors:%s", authors[i]);
    }

    l = g_slist_find_custom(manager->priv->plugins, info, (GCompareFunc)compare_plugin_id);
    if (l != NULL) {
        goto out;
    }

    schema_path = g_strdup_printf("%s.plugins.%s", DEFAULT_SETTINGS_PREFIX, my_plugin_info_get_id(info));
    g_debug("schema_path: %s", schema_path);
    if (is_schema(schema_path)) {
        manager->priv->plugins = g_slist_prepend(manager->priv->plugins, g_object_ref(info));
        g_signal_connect(info, "activated", G_CALLBACK(on_plugin_activated), manager);
        g_signal_connect(info, "deactivated", G_CALLBACK(on_plugin_deactivated), manager);
        my_plugin_info_from_schema(info, schema_path);
    }
    else {
        g_warning("Ignoring unknown module '%s'", schema_path);
    }
    g_free(schema_path);

out:
    if (info != NULL) {
        g_object_unref(info);
    }
}

static void load_all_plugins(MyDaemon *manager)
{
    // MY_PLUGINDIR见Makefile.am中-DMY_PLUGINDIR
    g_debug("load_all_plugins: %s, %s", MY_PLUGINDIR, G_DIR_SEPARATOR_S);

    const char *path = MY_PLUGINDIR G_DIR_SEPARATOR_S;// /usr/lib/x86_64-linux-gnu/my-plugin-daemon/
    GError *error;
    GDir *d;
    const char *name;

    g_debug("Loading settings plugins from dir: %s", path);

    error = NULL;
    d = g_dir_open(path, 0, &error);
    if (d == NULL) {
        g_warning("open %s error: %s", path, error->message);
        g_error_free(error);
        return;
    }

    while ((name = g_dir_read_name(d))) {
        char *filename;
        g_debug("###name: %s", name);
        if (!g_str_has_suffix(name, PLUGIN_EXT)) {
            continue;
        }

        filename = g_build_filename(path, name, NULL);
        if (g_file_test(filename, G_FILE_TEST_IS_REGULAR)) {
            get_plugin_info_from_file(manager, filename);
        }
        g_free(filename);
    }
    g_dir_close(d);

    g_slist_foreach(manager->priv->plugins, (GFunc)attempt_activate_plugin, NULL);
}

static void unload_plugin(MyPluginInfo *info, gpointer user_data)
{
    if (my_plugin_info_get_enabled(info)) {
        my_plugin_info_deactivate(info);
    }
    g_object_unref(info);
}

static void unload_all_plugins(MyDaemon *dbus)
{
     g_slist_foreach(dbus->priv->plugins, (GFunc)unload_plugin, NULL);
     g_slist_free (dbus->priv->plugins);
     dbus->priv->plugins = NULL;
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
    gboolean ret;

    g_debug("Starting plugin dbus");

    ret = FALSE;

    if (!g_module_supported()) {
        g_warning("my-plugin-daemon is not able to initialize the plugins.");
        g_set_error(error,
                    MY_DAEMON_ERROR,
                    MY_DAEMON_ERROR_GENERAL,
                    "Plugins not supported");
        goto out;
    }

    load_all_plugins(dbus);

    ret = TRUE;

out:
    return ret;
}

gboolean my_daemon_stop(MyDaemon *dbus, GError **error)
{
    g_debug("Stopping plugin dbus");

    unload_all_plugins(dbus);

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

MyDaemon *my_daemon_new(void)
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
