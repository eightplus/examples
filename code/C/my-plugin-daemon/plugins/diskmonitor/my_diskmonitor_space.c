/*

gcc -DTEST my_diskmonitor_space.c my_diskmonitor_dialog.c -std=c++11 -fpermissive -g -o my-space-test `pkg-config --cflags --libs gobject-2.0 gio-unix-2.0 glib-2.0 gtk+-2.0`

*/

#ifndef TEST
#include "config.h"
#endif

#include <sys/statvfs.h>
#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <glib-object.h>
#include <gio/gunixmounts.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include "my_diskmonitor_space.h"
#include "my_diskmonitor_dialog.h"

#define MONITOR_PATH "/"
#define GIGABYTE 1024 * 1024 * 1024
#define CHECK_EVERY_X_SECONDS 30

typedef struct
{
    GUnixMountEntry *mount;
    struct statvfs buf;
} MyMountInfo;

static GHashTable *notified_hash_table = NULL;
static unsigned int timeout_id = 0;
static GUnixMountMonitor *mount_monitor = NULL;
static MyDiskmonitorDialog *dialog = NULL;
static guint64 *time_read;

static double notify_size_unit = 0.5;

static gboolean popup_notify_warn_dialog(MyMountInfo *mount, gboolean multiple_volumes)
{
    gchar *name;
    gint64 free_space;
    gint response;
    gboolean retval = TRUE;
    gchar *path;

    /* Don't show a dialog if one is already displayed */
    if (dialog)
        return retval;

    free_space = (gint64)mount->buf.f_frsize * (gint64) mount->buf.f_bavail;
    path = g_strdup(g_unix_mount_get_mount_path(mount->mount));
    name = g_unix_mount_guess_name(mount->mount);

    if (g_strcmp0(path, MONITOR_PATH) != 0) {
        g_free(name);
        g_free(path);
        return retval;
    }

    if (free_space > (gint64)(notify_size_unit * GIGABYTE)) {
        g_free(name);
        g_free(path);
        return retval;
    }

    dialog = my_diskmonitor_dialog_new(multiple_volumes, free_space, name, path);

    g_free(name);
    g_free(path);
    g_object_ref(G_OBJECT(dialog));

    response = gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(GTK_WIDGET(dialog));
    dialog = NULL;

    switch(response) {
    case GTK_RESPONSE_CANCEL:
            retval = FALSE;
            break;
    case GTK_RESPONSE_OK:
    case GTK_RESPONSE_NONE:
    case GTK_RESPONSE_DELETE_EVENT:
            retval = TRUE;
            break;
    default:
            break;
            //g_asset_not_reached();
    }

    return retval;
}

static gboolean my_mount_has_space(MyMountInfo *mount)
{
    gchar *path = NULL;
    path = g_strdup(g_unix_mount_get_mount_path(mount->mount));

    if (g_strcmp0(path, MONITOR_PATH) == 0) {
        // 剩余空间>0.5G时提示剩余已经空间大于警告阈值了，可以放心使用系统空间了
        if (((gint64) mount->buf.f_frsize * (gint64) mount->buf.f_bavail) > (gint64)(notify_size_unit * GIGABYTE)) {
            if (dialog) {//如果有警告对话框，则销毁该对话框
                gtk_widget_destroy(GTK_WIDGET(dialog));
                dialog = NULL;
            }
            g_free(path);
            return TRUE;
        }
	else {
            g_free(path);
            return FALSE;
	}
    }
    g_free(path);

    /* If we got here, then this volume is low on space */
    return FALSE;
}

static gboolean my_mount_is_virtual(MyMountInfo *mount)
{
    if (mount->buf.f_blocks == 0) {
        /* Filesystems with zero blocks are virtual */
        return TRUE;
    }

    return FALSE;
}

static void my_free_mount_info(gpointer data)
{
    MyMountInfo *mount = data;

    g_return_if_fail(mount != NULL);

    g_unix_mount_free(mount->mount);
    g_free(mount);
}

static void my_iterator_warn_mounts(GList *mounts, gboolean multiple_volumes)
{
    GList *l;
    gboolean done = FALSE;

    for (l = mounts; l != NULL; l = l->next) {
        MyMountInfo *mount_info = l->data;
        MyMountInfo *previous_mount_info;
        gdouble free_space;
        gdouble previous_free_space;
        const gchar *path;
        gboolean show_notify = FALSE;

        if (done) {
            //Don't show any more dialogs if the user took action with the last one. The user action
            //might free up space on multiple volumes, making the next dialog redundant.
            my_free_mount_info(mount_info);
            continue;
        }

        path = g_unix_mount_get_mount_path(mount_info->mount);

        previous_mount_info = g_hash_table_lookup(notified_hash_table, path);
        if (previous_mount_info != NULL)
            previous_free_space = (gdouble)previous_mount_info->buf.f_bavail / (gdouble)previous_mount_info->buf.f_blocks;

        free_space = (gdouble)mount_info->buf.f_bavail / (gdouble)mount_info->buf.f_blocks;

        if (previous_mount_info == NULL) {
            // We haven't notified for this mount yet
            show_notify = TRUE;
            g_hash_table_replace(notified_hash_table, g_strdup(path), mount_info);
        }
        else if (g_strcmp0(path, MONITOR_PATH) == 0) {
            if (((gint64)mount_info->buf.f_frsize * (gint64)mount_info->buf.f_bavail) < (gint64)(notify_size_unit * GIGABYTE)) {
                show_notify = TRUE;
            }
            else {
                show_notify = FALSE;
            }
            g_hash_table_replace(notified_hash_table, g_strdup(path), mount_info);
        }
        else {
            // We've notified for this mount before, but the free space hasn't decreased sufficiently to notify again
            my_free_mount_info(mount_info);
            show_notify = FALSE;
        }

        if (show_notify) {
            if (popup_notify_warn_dialog(mount_info, multiple_volumes))
                done = TRUE;
        }
    }
}

static gboolean my_check_all_mounts(gpointer data)
{
    GList *mounts;
    GList *l;
    GList *check_mounts = NULL;
    GList *full_mounts = NULL;
    guint number_of_mounts;
    guint number_of_full_mounts;
    gboolean multiple_volumes = FALSE;//判断是否存在多个分区，即是否有除了/分区之外的其他分区存在

    /* We iterate through the static mounts in /etc/fstab first, seeing if
     * they're mounted by checking if the GUnixMountPoint has a corresponding GUnixMountEntry.
     * Iterating through the static mounts means we automatically ignore dynamically mounted media.
     */
    mounts = g_unix_mount_points_get(time_read);

    for (l = mounts; l != NULL; l = l->next) {
        GUnixMountPoint *mount_point = l->data;
        GUnixMountEntry *mount;
        MyMountInfo *mount_info;
        const gchar *path;

        path = g_unix_mount_point_get_mount_path(mount_point);
        mount = g_unix_mount_at(path, time_read);
        g_unix_mount_point_free(mount_point);
        if (mount == NULL) {
            /* The GUnixMountPoint is not mounted */
            continue;
        }

        mount_info = g_new0(MyMountInfo, 1);
        mount_info->mount = mount;

        path = g_unix_mount_get_mount_path(mount);

        if (g_unix_mount_is_readonly(mount)) {
            my_free_mount_info(mount_info);
            continue;
        }

        if (statvfs(path, &mount_info->buf) != 0) {
            my_free_mount_info(mount_info);
            continue;
        }

        if (my_mount_is_virtual(mount_info)) {
            my_free_mount_info(mount_info);
            continue;
        }

        check_mounts = g_list_prepend(check_mounts, mount_info);
    }
    g_list_free(mounts);

    number_of_mounts = g_list_length(check_mounts);
    if (number_of_mounts > 1)
        multiple_volumes = TRUE;

    for (l = check_mounts; l != NULL; l = l->next) {
        MyMountInfo *mount_info = l->data;

        if (!my_mount_has_space(mount_info)) {
            full_mounts = g_list_prepend(full_mounts, mount_info);
        }
        else {
            g_hash_table_remove(notified_hash_table, g_unix_mount_get_mount_path(mount_info->mount));
            my_free_mount_info(mount_info);
        }
    }

    number_of_full_mounts = g_list_length(full_mounts);
    my_iterator_warn_mounts(full_mounts, multiple_volumes);
    g_list_free(check_mounts);
    g_list_free(full_mounts);

    return TRUE;
}

static void reset_timer()
{
    if (timeout_id)
        g_source_remove(timeout_id);
    //每隔固定时间CHECK_EVERY_X_SECONDS，检查所有挂载分区
    timeout_id = g_timeout_add_seconds(CHECK_EVERY_X_SECONDS, my_check_all_mounts, NULL);
}

static gboolean my_is_hash_item_not_in_mounts(gpointer key, gpointer value, gpointer user_data)
{
    GList *l;

    for (l = (GList *)user_data; l != NULL; l = l->next) {
        GUnixMountEntry *mount = l->data;
        const char *path;

        path = g_unix_mount_get_mount_path(mount);

        if (strcmp(path, key) == 0)
            return FALSE;
    }

    return TRUE;
}

static void my_mounts_changed(GObject *monitor, gpointer data)
{
    GList *mounts;

    /* remove the saved data for mounts that got removed */
    mounts = g_unix_mounts_get(time_read);
    g_hash_table_foreach_remove(notified_hash_table, my_is_hash_item_not_in_mounts, mounts);
    g_list_free_full(mounts, (GDestroyNotify)g_unix_mount_free);

    my_check_all_mounts(NULL);

    reset_timer();
}

void start_disk_monitor(gboolean check_now)
{
    if (notified_hash_table || timeout_id || mount_monitor) {
        g_warning("Low disk space monitor already initialized.");
        return;
    }

    notified_hash_table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, my_free_mount_info);
    mount_monitor = g_unix_mount_monitor_new();
    g_unix_mount_monitor_set_rate_limit(mount_monitor, 1000);
    g_signal_connect(mount_monitor, "mounts-changed", G_CALLBACK(my_mounts_changed), NULL);

    if (check_now)
        my_check_all_mounts(NULL);

    reset_timer();
}

void stop_disk_monitor(void)
{
    if (timeout_id)
        g_source_remove(timeout_id);
    timeout_id = 0;

    if (notified_hash_table)
        g_hash_table_destroy(notified_hash_table);
    notified_hash_table = NULL;

    if (mount_monitor)
        g_object_unref(mount_monitor);
    mount_monitor = NULL;

    if (dialog) {
        gtk_widget_destroy(GTK_WIDGET(dialog));
        dialog = NULL;
    }

}

#ifdef TEST
int main(int argc, char **argv)
{
    GMainLoop *loop;

    gtk_init(&argc, &argv);

    loop = g_main_loop_new(NULL, FALSE);

    start_disk_monitor(TRUE);

    g_main_loop_run(loop);

    stop_disk_monitor();

    g_main_loop_unref(loop);

    return 0;
}
#endif /* TEST */
