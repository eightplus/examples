#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <glib.h>

// gcc main.c -o run `pkg-config --cflags --libs glib-2.0`

typedef struct _MyData {
    gint min_number;
    gint max_number;
} MyData;

static gboolean io_watch_handle_keyboard(GIOChannel *channel, GIOCondition condition, gpointer data)
//static gboolean io_watch_handle_keyboard(GIOChannel *channel, GIOCondition condition, MyData *data)
{
    gsize len = 0;
    gchar *buffer = NULL;
    MyData *m_data = (MyData *)data;

    if (g_io_channel_read_line(channel, &buffer, &len, NULL, NULL) == G_IO_STATUS_NORMAL) {
    //if (g_io_channel_read_line(channel, &buffer, NULL, NULL, NULL) == G_IO_STATUS_NORMAL) {
        int number = atoi(buffer);
        if (number < m_data->min_number || number > m_data->max_number) {
            g_printerr("Out of range\n");
        }
        else {
            g_print("Input Number: %d\n", number);
        }
    }

    if (len > 0) {
        g_print("Read data's length: %d\n", len);
    }
    g_free(buffer);

    return TRUE;
}

int main(int argc, char *argv[])
{
    MyData data;
    data.min_number = 0;
    data.max_number = 100;

    GMainLoop *main_loop = g_main_loop_new(NULL, FALSE);

    GIOChannel *channel;
    channel = g_io_channel_unix_new(fileno(stdin));//将标准输入描述符转化成GIOChannel
    //channel = g_io_channel_unix_new(1);
    if (channel) {
        g_io_add_watch(channel, G_IO_IN, (GIOFunc)io_watch_handle_keyboard, &data);
        g_io_channel_unref(channel);
    }

    g_main_loop_run(main_loop);
    g_main_context_unref(g_main_loop_get_context(main_loop));
    g_main_loop_unref(main_loop);

    return 0;
}
