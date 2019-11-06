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

#ifndef __MY_DAEMON_H
#define __MY_DAEMON_H

#include <glib-object.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MY_TYPE_DAEMON         (my_daemon_get_type())
#define MY_DAEMON(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), MY_TYPE_DAEMON, MyDaemon))
#define MY_DAEMON_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), MY_TYPE_DAEMON, MyDaemonClass))
#define MY_IS_DAEMON(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), MY_TYPE_DAEMON))
#define MY_IS_DAEMON_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), MY_TYPE_DAEMON))
#define MY_DAEMON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), MY_TYPE_DAEMON, MyDaemonClass))

typedef struct MyDaemonPrivate MyDaemonPrivate;

typedef struct
{
    GObject parent;
    MyDaemonPrivate *priv;
} MyDaemon;

typedef struct
{
    GObjectClass parent_class;
    /* signals */
    void (* plugin_activated) (MyDaemon *dbus, const char *name);
    void (* plugin_deactivated) (MyDaemon *dbus, const char *name);
} MyDaemonClass;

typedef enum
{
    MY_DAEMON_ERROR_GENERAL
} MyDaemonError;

#define MY_DAEMON_ERROR my_daemon_error_quark()

GQuark my_daemon_error_quark(void);
GType my_daemon_get_type(void) G_GNUC_CONST;

MyDaemon *my_daemon_new(void);
gboolean my_daemon_start(MyDaemon *dbus, GError **error);
gboolean my_daemon_stop(MyDaemon *dbus, GError **error);

#ifdef __cplusplus
}
#endif

#endif /* __MY_DAEMON_H */
