#ifndef _MY_DISKMONITOR_DIALOG_H_
#define _MY_DISKMONITOR_DIALOG_H_

#include <glib-object.h>
#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MY_TYPE_DISKMONITOR_DIALOG             (my_diskmonitor_dialog_get_type ())
#define MY_DISKMONITOR_DIALOG(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MY_TYPE_DISKMONITOR_DIALOG, MyDiskmonitorDialog))
#define MYDISKMONITOR_DIALOG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), MY_TYPE_DISKMONITOR_DIALOG, MyDiskmonitorDialogClass))
#define MY_IS_DISKMONITOR_DIALOG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MY_TYPE_DISKMONITOR_DIALOG))
#define MY_IS_DISKMONITOR_DIALOG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), MY_TYPE_DISKMONITOR_DIALOG))
#define MY_DISKMONITOR_DIALOG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), MY_TYPE_DISKMONITOR_DIALOG, MyDiskmonitorDialogClass))

typedef struct MyDiskmonitorDialogPrivate MyDiskmonitorDialogPrivate;
typedef struct _MyDiskmonitorDialogClass MyDiskmonitorDialogClass;
typedef struct _MyDiskmonitorDialog MyDiskmonitorDialog;

struct _MyDiskmonitorDialogClass
{
    GtkDialogClass parent_class;
};

struct _MyDiskmonitorDialog
{
    GtkDialog parent_instance;
    MyDiskmonitorDialogPrivate *priv;
};

struct MyDiskmonitorDialogPrivate
{
    GtkWidget *primary_label;
    GtkWidget *secondary_label;
    GtkWidget *button_ok;
    gboolean has_multiple_volumes;
    gint64 space_remaining;
    gchar *partition_name;
    gchar *mount_path;
};

GType my_diskmonitor_dialog_get_type(void) G_GNUC_CONST;

MyDiskmonitorDialog *my_diskmonitor_dialog_new(gboolean has_multiple_volumes,
                                     gint64 space_remaining,
                                     const gchar *partition_name,
                                     const gchar *mount_path);

#ifdef __cplusplus
}
#endif

#endif /* _MY_DISKMONITOR_DIALOG_H_ */
