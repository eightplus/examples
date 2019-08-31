#include <glib/gi18n.h>
#include <gio/gio.h>
#include <glib.h>

#include "my_diskmonitor_dialog.h"

static double free_size_gb_no_notify_again = 0.5;

#define MONITOR_PATH "/"
#define GIGABYTE 1024 * 1024 * 1024

enum
{
    PROP_0,
    PROP_HAS_MULTIPLE_VOLUMES,
    PROP_SPACE_REMAINING,
    PROP_PARTITION_NAME,
    PROP_MOUNT_PATH
};

#define MY_DISKMONITOR_DIALOG_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), MY_TYPE_DISKMONITOR_DIALOG, MyDiskmonitorDialogPrivate))

static void my_diskmonitor_dialog_class_init(MyDiskmonitorDialogClass *klass);
static void my_diskmonitor_dialog_init(MyDiskmonitorDialog *dialog);

G_DEFINE_TYPE (MyDiskmonitorDialog, my_diskmonitor_dialog, GTK_TYPE_DIALOG);

static gchar *my_diskmonitor_dialog_get_primary_text(MyDiskmonitorDialog *dialog)
{
    gchar *primary_text, *free_space;

    g_return_val_if_fail(MY_IS_DISKMONITOR_DIALOG(dialog), NULL);

    free_space = g_format_size(dialog->priv->space_remaining);

    //如果有/分区之外的其他分区存在，则显示是该分区没有剩余空间，否则显示整个系统没有剩余空间
    if (dialog->priv->has_multiple_volumes) {
        primary_text = g_strdup_printf(_("The volume \"%s\" has only %s disk space remaining."),
                                       dialog->priv->mount_path, free_space);
    }
    else {
        primary_text = g_strdup_printf(_("This computer has only %s disk space remaining."),
                                       free_space);
    }

    g_free(free_space);

    return primary_text;
}

static void my_diskmonitor_dialog_init(MyDiskmonitorDialog *dialog)
{
    GtkWidget *main_vbox, *text_vbox, *hbox;
    GtkWidget *image;

    dialog->priv = MY_DISKMONITOR_DIALOG_GET_PRIVATE(dialog);

    main_vbox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    gtk_window_set_title(GTK_WINDOW(dialog), _("Low Disk Space"));
    gtk_window_set_icon_name(GTK_WINDOW(dialog), GTK_STOCK_DIALOG_WARNING);
    gtk_window_set_resizable(GTK_WINDOW (dialog), FALSE);
    gtk_window_set_position(GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
    gtk_window_set_urgency_hint(GTK_WINDOW (dialog), TRUE);
    gtk_window_set_focus_on_map(GTK_WINDOW (dialog), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(dialog), 5);

    image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_WARNING, GTK_ICON_SIZE_DIALOG);
    gtk_misc_set_alignment(GTK_MISC(image), 0.5, 0.0);

    dialog->priv->primary_label = gtk_label_new(NULL);
    gtk_label_set_line_wrap(GTK_LABEL(dialog->priv->primary_label), TRUE);
    gtk_label_set_single_line_mode(GTK_LABEL(dialog->priv->primary_label), FALSE);
    gtk_misc_set_alignment(GTK_MISC(dialog->priv->primary_label), 0.0, 0.0);

    dialog->priv->secondary_label = gtk_label_new(NULL);
    gtk_label_set_line_wrap(GTK_LABEL(dialog->priv->secondary_label), TRUE);
    gtk_label_set_single_line_mode(GTK_LABEL(dialog->priv->secondary_label), FALSE);
    gtk_misc_set_alignment(GTK_MISC(dialog->priv->secondary_label), 0.0, 0.0);

    gtk_box_set_spacing(GTK_BOX(main_vbox), 14);

    hbox = gtk_hbox_new(FALSE, 12);
    gtk_container_set_border_width(GTK_CONTAINER (hbox), 5);

    text_vbox = gtk_vbox_new(FALSE, 12);

    gtk_box_pack_start(GTK_BOX(text_vbox), dialog->priv->primary_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(text_vbox), dialog->priv->secondary_label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), text_vbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_vbox), hbox, FALSE, FALSE, 0);

    gtk_box_set_spacing(GTK_BOX(gtk_dialog_get_action_area(GTK_DIALOG(dialog))), 6);
    gtk_container_set_border_width(GTK_CONTAINER(gtk_dialog_get_action_area(GTK_DIALOG(dialog))), 5);

    gtk_widget_show_all(hbox);
}

static void my_diskmonitor_dialog_finalize(GObject *object)
{
    MyDiskmonitorDialog *self;

    g_return_if_fail(object != NULL);
    g_return_if_fail(MY_IS_DISKMONITOR_DIALOG(object));

    self = MY_DISKMONITOR_DIALOG(object);

    if (self->priv->partition_name)
        g_free (self->priv->partition_name);

    if (self->priv->mount_path)
        g_free (self->priv->mount_path);

    G_OBJECT_CLASS(my_diskmonitor_dialog_parent_class)->finalize(object);
}

static void my_diskmonitor_dialog_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    MyDiskmonitorDialog *self;

    g_return_if_fail(MY_IS_DISKMONITOR_DIALOG(object));

    self = MY_DISKMONITOR_DIALOG(object);

    switch (prop_id)
    {
    case PROP_HAS_MULTIPLE_VOLUMES:
        self->priv->has_multiple_volumes = g_value_get_boolean(value);
        break;

    case PROP_SPACE_REMAINING:
        self->priv->space_remaining = g_value_get_int64(value);
        break;
    case PROP_PARTITION_NAME:
        self->priv->partition_name = g_value_dup_string(value);
        break;
    case PROP_MOUNT_PATH:
        self->priv->mount_path = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void my_diskmonitor_dialog_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    MyDiskmonitorDialog *self;

    g_return_if_fail(MY_IS_DISKMONITOR_DIALOG(object));

    self = MY_DISKMONITOR_DIALOG(object);

    switch (prop_id)
    {
    case PROP_HAS_MULTIPLE_VOLUMES:
        g_value_set_boolean(value, self->priv->has_multiple_volumes);
        break;
    case PROP_SPACE_REMAINING:
        g_value_set_int64(value, self->priv->space_remaining);
        break;
    case PROP_PARTITION_NAME:
        g_value_set_string(value, self->priv->partition_name);
        break;
    case PROP_MOUNT_PATH:
        g_value_set_string(value, self->priv->mount_path);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void my_diskmonitor_dialog_class_init(MyDiskmonitorDialogClass *klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = my_diskmonitor_dialog_finalize;
    object_class->set_property = my_diskmonitor_dialog_set_property;
    object_class->get_property = my_diskmonitor_dialog_get_property;

    g_object_class_install_property(object_class,
                                     PROP_HAS_MULTIPLE_VOLUMES,
                                     g_param_spec_boolean("has_multiple_volumes",
                                                           "has_multiple_volumes",
                                                           "Set to TRUE if there are multiple partitions on the system",
                                                           FALSE,
                                                           G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

    g_object_class_install_property(object_class,
                                     PROP_SPACE_REMAINING,
                                     g_param_spec_int64("space-remaining",
                                                         "space-remaining",
                                                         "Specify how much space is remaining in bytes",
                                                         G_MININT64, G_MAXINT64, 0,
                                                         G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

    g_object_class_install_property(object_class,
                                     PROP_PARTITION_NAME,
                                     g_param_spec_string("partition-name",
                                                          "partition-name",
                                                          "Specify the name of the partition",
                                                          "Unknown",
                                                          G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

    g_object_class_install_property(object_class,
                                     PROP_MOUNT_PATH,
                                     g_param_spec_string("mount-path",
                                                          "mount-path",
                                                          "Specify the mount path for the partition",
                                                          "Unknown",
                                                          G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

    g_type_class_add_private(klass, sizeof(MyDiskmonitorDialogPrivate));
}

MyDiskmonitorDialog* my_diskmonitor_dialog_new(
                     gboolean has_multiple_volumes,
                     gint64 space_remaining,
                     const gchar *partition_name,
                     const gchar *mount_path)
{
    MyDiskmonitorDialog *dialog;
    GtkWidget *ok_btn = NULL;
    GtkWidget *ok_image;
    gchar *primary_text, *primary_text_markup;
    const gchar *secondary_text;

    dialog = MY_DISKMONITOR_DIALOG(g_object_new(MY_TYPE_DISKMONITOR_DIALOG,
                                            "has_multiple_volumes", has_multiple_volumes,
                                            "space-remaining", space_remaining,
                                            "partition-name", partition_name,
                                            "mount-path", mount_path,
                                            NULL));

    dialog->priv->button_ok = NULL;
    ok_btn = gtk_dialog_add_button(GTK_DIALOG(dialog), _("OK"), GTK_RESPONSE_OK);
    ok_image = gtk_image_new_from_stock(GTK_STOCK_OK, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(ok_btn), ok_image);

    gtk_widget_grab_default(ok_btn);
    dialog->priv->button_ok = ok_btn;

    primary_text = my_diskmonitor_dialog_get_primary_text(dialog);

    primary_text_markup = g_markup_printf_escaped("<big><b>%s</b></big>", primary_text);
    gtk_label_set_markup(GTK_LABEL(dialog->priv->primary_label), primary_text_markup);

    if (g_strcmp0(mount_path, MONITOR_PATH) == 0) {
        secondary_text = _("Insufficient disk space, please clean up the disk." \
                 "Otherwise, it will affect the normal use of the system.");
    }

    gtk_label_set_text(GTK_LABEL(dialog->priv->secondary_label), secondary_text);

    if (g_strcmp0(mount_path, MONITOR_PATH) == 0 && (space_remaining < (gint64)(free_size_gb_no_notify_again * GIGABYTE))) {
        gtk_window_set_deletable(GTK_DIALOG(dialog), FALSE);
    }

    g_free(primary_text);
    g_free(primary_text_markup);

    return dialog;
} 
