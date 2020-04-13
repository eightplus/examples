/*
 * Copyright (C) 2011 ~ 2020 lixiang
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

/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

/*
gcc -o run main.c $(pkg-config --cflags --libs glib-2.0 gobject-2.0 gtk+-3.0)
*/

#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <glib-object.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <signal.h>
#include <errno.h>

struct data_handle
{
    GtkApplication *app;
    GtkWidget *gui;
};

static GtkBuilder *builder = NULL;//UI builder
GError *error = NULL;
GdkScreen *screen = NULL;//屏幕尺寸全局变量

/*
 * Function Name: free_sources_handler
 * Description:  释放内存和退出
 */
void free_sources_handler()
{
    g_object_unref(G_OBJECT(builder));
    if (error != NULL) {
        g_clear_error(&error);
    }
    exit(0);
}

/*
 * Function Name: delete_event_handler
 * Description:  主界面销毁的响应函数(点击标题栏上的关闭按钮时对该信号处理)
 */
void delete_event_handler(GtkWidget *widget, gpointer data)
{
    struct data_handle *handle = data;
    g_object_unref(G_OBJECT(builder));
    if (error != NULL) {
        g_clear_error(&error);
    }
    g_application_quit(G_APPLICATION(handle->app));
}

//销毁组件时返回FALSE，如果取消一个delete-event返回TRUE，这样就可以用来确认，是否真的要退出
static gboolean cb_main_window_delete(GtkWidget *window, GdkEvent *event, gpointer data)
{
    return FALSE;
}

/*
 * Function Name: sigint_handler
 * Description:  关闭进程的响应函数
 */
void sigint_handler(gint sig)
{
    free_sources_handler();
}

/*
 * Function Name: sigterm_handler
 * Description:  关闭进程的响应函数
 */
void sigterm_handler(int arg)
{
    free_sources_handler();
}

/*
 * Function Name: on_min_button_clicked
 * Description:  最小化按钮的响应函数
 * param0:  按钮对象
 * param0:  传递的数据
 */
void on_min_button_clicked(GtkButton *button, gpointer user_data)
{
    gtk_window_iconify(GTK_WINDOW(gtk_builder_get_object(builder, "mainwindow")));
}

/*
 * Function Name: set_gtk_css_style
 * Description:  关联css样式
 */
void set_gtk_css_style()
{
    GtkCssProvider *provider = NULL;
    GFile *file = NULL;
    GError *css_error = NULL;

    char cssPath[] = "style.css";
    file = g_file_new_for_path(cssPath);
    if (file != NULL) {
        if (provider == NULL) {
            provider = gtk_css_provider_new();
        }
        gtk_css_provider_load_from_file(provider, file, NULL);
        gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
        gtk_style_context_reset_widgets(screen);
        gsize bytes_written, bytes_read;
        gtk_css_provider_load_from_path(provider, g_filename_to_utf8(cssPath, strlen(cssPath), &bytes_read, &bytes_written, &css_error), NULL);
    }

    if (NULL != css_error) {
        g_clear_error(&css_error);
    }
    if (NULL != file) {
        g_object_unref(file);
        file = NULL;
    }
}

/*
 * Function Name: handle_drag
 * Description:  界面拖动
 */
static gboolean handle_drag(GtkWidget *widget, GdkEventButton *event, GdkWindowEdge edge)
{
    if (event->button == 1) {//左键单击
        gtk_window_begin_move_drag(GTK_WINDOW(gtk_widget_get_toplevel(widget)), event->button, event->x_root, event->y_root, event->time);
    }
    return FALSE;
}

/*
 * Function Name: app_startup_handler
 * Description:  界面初始化
 */
static void app_startup_handler(GApplication *app, gpointer data)
{
    struct data_handle *handle = data;
    handle->app = GTK_APPLICATION(app);

    GtkWidget *main_window;
    guint retval;

    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigterm_handler);

    builder = gtk_builder_new();
    retval = gtk_builder_add_from_file(builder, "mainwindow.ui", &error);
    if (retval == 0) {
        return;
    }
    gtk_builder_connect_signals(builder, NULL);

    screen = gdk_screen_get_default();
    set_gtk_css_style();

    main_window = GTK_WIDGET(gtk_builder_get_object(builder, "mainwindow"));
    gtk_application_add_window(app , GTK_WINDOW(main_window));
    handle->gui = main_window;

    gtk_window_set_decorated(GTK_WINDOW(main_window), FALSE);//无边框处理，隐藏默认的标题栏
    gtk_window_set_title(GTK_WINDOW(main_window), "Application Demo");//设置窗口标题
    gtk_window_set_default_size(GTK_WINDOW(main_window), 500, 400);//设置窗口默认大小

    gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER_ALWAYS);//控制窗口在屏幕上的初始位置
    gtk_window_set_modal(GTK_WINDOW(main_window), TRUE);
    gtk_widget_realize(main_window);
    gtk_window_set_resizable(main_window, FALSE);//阻止用户拖拽窗口边框改变其大小
    //delete_event的回调函数返回FALSE时，gtk_widget_destroy会被调用，进而触发destroy信号的发出
    g_signal_connect(main_window, "destroy", G_CALLBACK(delete_event_handler), handle);
    g_signal_connect_swapped(main_window, "delete_event", G_CALLBACK(cb_main_window_delete), handle);

    //实现按下鼠标左键拖动窗口
    gtk_widget_add_events(main_window, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(main_window), "button-press-event", G_CALLBACK(handle_drag), NULL);

    GtkButton *button = GTK_BUTTON(gtk_builder_get_object(builder, "min_button"));
    gtk_widget_set_size_request(GTK_WIDGET(button), 36, 36);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(on_min_button_clicked), NULL);

    button = GTK_BUTTON(gtk_builder_get_object(builder, "close_button"));
    gtk_widget_set_size_request(GTK_WIDGET(button), 36, 36);
    g_signal_connect(button, "clicked", G_CALLBACK(delete_event_handler), handle);

    gtk_application_set_app_menu(handle->app, NULL);
    gtk_widget_show_all(GTK_WIDGET(handle->gui));
}

/*
 * Function Name: app_activate_handler
 * Description:  置顶，确保窗口在屏幕上并且对用户是可见的
 */
static void app_activate_handler(GApplication *app, gpointer data)
{
    gtk_window_present(GTK_WINDOW(((struct data_handle *)data)->gui));
}


/*
 * Function Name: main
 * Description:  主函数
 */
int main(int argc, char **argv)
{
    GtkApplication *app;
    struct data_handle *handle;
    gint status;
    app = gtk_application_new("com.eightplus.application", G_APPLICATION_FLAGS_NONE);
    handle = g_malloc(sizeof(struct data_handle));

    g_signal_connect(app, "startup", G_CALLBACK(app_startup_handler), handle);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate_handler), handle);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    g_free(handle);

    return status;
}
