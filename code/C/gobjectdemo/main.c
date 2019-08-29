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

#include <glib.h>
#include <glib-unix.h>
#include <stdio.h>

#include "my_demo.h"

static GMainLoop *loop = NULL;

static gboolean signal_handler(gpointer user_data)
{
    g_main_loop_quit(loop);
    return FALSE;
}

void handler_receive_msg(GObject *sender, char *name, gpointer data)
{
    MyDemo *demo = G_TYPE_CHECK_INSTANCE_CAST(sender, my_demo_get_type(), MyDemo);
    printf("handler: [%s] from [%s: %ld]\n", name, demo->priv->name, demo->priv->age);
}

int main(int argc, char* argv[])
{
#if !GLIB_CHECK_VERSION(2,32,0)
    if (! g_thread_supported ())
        g_thread_init (NULL);
#endif
#if !GLIB_CHECK_VERSION(2,36,0)
    g_type_init ();
#endif

    loop = g_main_loop_new(NULL, FALSE);
    g_unix_signal_add_full(G_PRIORITY_DEFAULT,
                                    SIGINT,
                                    signal_handler,
                                    NULL,
                                    NULL);
    int i;
    MyDemo *demos[3];
    int len = sizeof(demos) / sizeof(demos[0]);

    for (i = 0; i < len; i++) {
        printf("construction demos[%d]\n", i);
        //创建对象
        //demos[i]= g_object_new(MY_TYPE_DEMO, NULL);
        /*demos[i]= g_object_new(MY_TYPE_DEMO,
                               "name", "kobe",
                               "age", i,
                               NULL);*/
        gchar *name = g_strdup_printf("kobe%d", i);
        demos[i] = my_demo_new(name, i);
        g_free(name);
        //绑定信号
        g_signal_connect(demos[i], "broadcast_msg", G_CALLBACK(handler_receive_msg), NULL);
        //发送信号
        g_signal_emit_by_name(demos[i], "broadcast_msg", "lixiang");
    }

    //析构
    for(i = 0; i < len; i++){
        printf("destruction demos[%d]\n", i);
        g_object_unref(demos[i]);
    }

    g_main_loop_run(loop);
    g_main_loop_unref(loop);

    return 0;
}
