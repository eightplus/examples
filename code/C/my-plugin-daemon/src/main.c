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
#include <unistd.h>
#include <libintl.h>
#include <errno.h>
#include <locale.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>

#include "my_daemon.h"

#define PLUGIN_DBUS_NAME "org.freedesktop.MyPluginDaemon"

static gboolean debug = FALSE;
static int fds[2];

static GOptionEntry entries[] = {
        { "debug", 0, 0, G_OPTION_ARG_NONE, &debug, N_("Enable debugging code"), NULL },
        { NULL }
};

static DBusHandlerResult bus_message_handler(DBusConnection *connection, DBusMessage *message, void *user_data)
{
    if (dbus_message_is_signal(message,
                                DBUS_INTERFACE_LOCAL,
                                "Disconnected")) {
        gtk_main_quit();
        return DBUS_HANDLER_RESULT_HANDLED;
    }
    else if (dbus_message_is_signal(message,
                                     DBUS_INTERFACE_DBUS,
                                     "NameLost")) {
        g_warning("D-Bus name lost, quitting");
        gtk_main_quit();
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

static DBusGConnection *get_session_bus(void)
{
    GError *error;
    DBusGConnection *bus;
    DBusConnection *connection;

    error = NULL;
    bus = dbus_g_bus_get(DBUS_BUS_SESSION, &error);
    if (bus == NULL) {
        g_warning("Couldn't connect to session bus: %s", error->message);
        g_error_free(error);
        goto out;
    }

    connection = dbus_g_connection_get_connection(bus);
    dbus_connection_add_filter(connection,
                                (DBusHandleMessageFunction)
                                bus_message_handler,
                                NULL, NULL);

    dbus_connection_set_exit_on_disconnect(connection, FALSE);

out:
    return bus;
}

static gboolean request_connect_dbus(DBusGConnection *connection)
{
    DBusGProxy *bus_proxy;
    gboolean ret;
    GError *error;
    guint result;
    guint32 flags;

    ret = FALSE;
    error = NULL;

    //申请一个会话总线  DBUS_SERVICE_DBUS: org.freedesktop.DBus    DBUS_PATH_DBUS: /org/freedesktop/DBus   DBUS_INTERFACE_DBUS:org.freedesktop.DBus
    bus_proxy = dbus_g_proxy_new_for_name(connection,
                                           DBUS_SERVICE_DBUS,
                                           DBUS_PATH_DBUS,
                                           DBUS_INTERFACE_DBUS);
    if (bus_proxy == NULL) {
        g_warning("Could not construct bus_proxy object");
        goto out;
    }

    flags = DBUS_NAME_FLAG_DO_NOT_QUEUE | DBUS_NAME_FLAG_ALLOW_REPLACEMENT;

    //调用dbusdaemon的函数“RequestName”，申请一个DBUS名为org.freedesktop.MyPluginDaemon的注册信息
    gboolean res = dbus_g_proxy_call(bus_proxy,
                             "RequestName",
                             &error,
                             G_TYPE_STRING, PLUGIN_DBUS_NAME,
                             G_TYPE_UINT, flags,
                             G_TYPE_INVALID,
                             G_TYPE_UINT, &result,
                             G_TYPE_INVALID);
    if (!res) {
        if (error != NULL) {
            g_warning("Failed to acquire %s: %s", PLUGIN_DBUS_NAME, error->message);
            g_error_free(error);
        }
        else {
            g_warning("Failed to acquire %s", PLUGIN_DBUS_NAME);
        }
        goto out;
    }

    if (result != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        if (error != NULL) {
            g_warning("Failed to acquire %s: %s", PLUGIN_DBUS_NAME, error->message);
            g_error_free(error);
        }
        else {
            g_warning("Failed to acquire %s", PLUGIN_DBUS_NAME);
        }
        goto out;
    }

    ret = TRUE;
    g_object_unref(bus_proxy);

    g_debug("Successfully connected to D-Bus");

out:
    return ret;
}

static void on_term_signal(int signal)
{
    close(fds[1]);
    fds[1] = -1;
}

static gboolean on_term_signal_pipe_closed(GIOChannel *source, GIOCondition condition, gpointer data)
{
    MyDaemon *manager;

    manager = MY_DAEMON(data);

    fds[0] = -1;

    gtk_main_quit();

    return FALSE;
}

static void watch_for_term_signal(MyDaemon *manager)
{
    GIOChannel *channel;

    if (-1 == pipe(fds) ||
        -1 == fcntl(fds[0], F_SETFD, FD_CLOEXEC) ||
        -1 == fcntl(fds[1], F_SETFD, FD_CLOEXEC)) {
        g_error("Could not create pipe: %s", g_strerror(errno));
        exit(EXIT_FAILURE);
    }

    channel = g_io_channel_unix_new(fds[0]);
    g_io_channel_set_encoding(channel, NULL, NULL);
    g_io_channel_set_buffered(channel, FALSE);
    g_io_add_watch(channel, G_IO_HUP, on_term_signal_pipe_closed, manager);
    g_io_channel_unref(channel);

    signal(SIGTERM, on_term_signal);
}

static void my_log_default_handler(const gchar *log_domain,
                         GLogLevelFlags log_level,
                         const gchar *message,
                         gpointer unused_data)
{
    if ((log_level & G_LOG_LEVEL_MASK) == G_LOG_LEVEL_DEBUG && ! debug) {
        return;
    }

    g_log_default_handler(log_domain,
                           log_level,
                           message,
                           unused_data);
}

static void parse_args(int *argc, char ***argv)
{
    GError *error = NULL;
    GOptionContext *context;
    context = g_option_context_new(NULL);

    g_option_context_add_main_entries(context, entries, NULL);
    g_option_context_add_group(context, gtk_get_option_group(FALSE));

    if (!g_option_context_parse(context, argc, argv, &error)) {
        if (error != NULL) {
            g_warning("%s", error->message);
            g_error_free(error);
        }
        else {
            g_warning("Unable to initialize GTK+");
        }
        exit(EXIT_FAILURE);
    }

    g_option_context_free(context);

    if (debug)
        g_setenv("G_MESSAGES_DEBUG", "all", FALSE);
}

int main(int argc, char *argv[])
{
    MyDaemon *daemon = NULL;
    DBusGConnection *connection = NULL;
    GError *error = NULL;

    bindtextdomain(GETTEXT_PACKAGE, MY_PLUGIN_LOCALEDIR);//见Makefile.am中-DMY_PLUGIN_LOCALEDIR
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
    setlocale(LC_ALL, "");

    parse_args(&argc, &argv);

    if (!gtk_init_check(NULL, NULL)) {
        g_warning("Unable to initialize GTK+");
        exit(EXIT_FAILURE);
    }

    g_log_set_default_handler(my_log_default_handler, NULL);

    connection = get_session_bus();
    if (connection == NULL) {
        g_warning("Could not get a connection to the session bus");
        goto out;
    }

    if (!request_connect_dbus(connection)) {
        goto out;
    }

    daemon = my_daemon_new();
    if (daemon == NULL) {
        g_warning("Unable to register object");
        goto out;
    }

    watch_for_term_signal(daemon);

    if (!my_daemon_start(daemon, &error)) {
        g_warning("Unable to start: %s", error->message);
        g_error_free(error);
        goto out;
    }

    gtk_main();

out:
    if (connection != NULL) {
        dbus_g_connection_unref(connection);
    }

    if (daemon != NULL) {
        g_object_unref(daemon);
    }

    g_debug("MyDaemon finished");

    return 0;
}
