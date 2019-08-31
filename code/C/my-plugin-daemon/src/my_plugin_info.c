#include "config.h"

#include <string.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <gmodule.h>
#include <gio/gio.h>

#include "my_plugin_info.h"
#include "my_plugin.h"
#include "my_module.h"

#define MY_PLUGIN_INFO_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), MY_TYPE_PLUGIN_INFO, MyPluginInfoPrivate))

#define PLUGIN_GROUP "My Plugin"//这个需要保证每个插件配置文件的group都是"My Plugin"，否则读取不到插件信息

//未来将支持两类插件，一类是C编写的，一类是Python编写的
typedef enum
{
    MY_PLUGIN_LOADER_C,
    MY_PLUGIN_LOADER_PY
} MyPluginLoader;

struct MyPluginInfoPrivate
{
    char *filename;
    GSettings *settings;

    MyPluginLoader loader;
    GTypeModule *module;
    MyPlugin *plugin;

    char *id;
    char *name;
    char *desc;
    char **authors;
    char *copyright;
    char *website;

    int enabled : 1;
    int active : 1;
    int available : 1;//如果加载插件配置文件成功，则为1,表示该插件可以被激活；否则为0，表示该插件不可被激活
};

enum {
    ACTIVATED,
    DEACTIVATED,
    LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0, };

G_DEFINE_TYPE (MyPluginInfo, my_plugin_info, G_TYPE_OBJECT)

static void my_plugin_info_finalize(GObject *object)
{
    MyPluginInfo *info;

    g_return_if_fail(object != NULL);
    g_return_if_fail(MY_IS_PLUGIN_INFO(object));

    info = MY_PLUGIN_INFO(object);

    g_return_if_fail(info->priv != NULL);

    if (info->priv->plugin != NULL) {
        g_debug("Unref plugin %s", info->priv->name);
        g_object_unref(info->priv->plugin);
    }

    g_free(info->priv->filename);
    g_free(info->priv->id);
    g_free(info->priv->name);
    g_free(info->priv->desc);
    g_free(info->priv->website);
    g_free(info->priv->copyright);
    g_strfreev(info->priv->authors);

    if (info->priv->settings != NULL) {
        g_object_unref(info->priv->settings);
    }

    G_OBJECT_CLASS(my_plugin_info_parent_class)->finalize(object);
}

static void my_plugin_info_class_init(MyPluginInfoClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = my_plugin_info_finalize;

    signals[ACTIVATED] =
            g_signal_new("activated",
                          G_TYPE_FROM_CLASS(object_class),
                          G_SIGNAL_RUN_LAST,
                          G_STRUCT_OFFSET(MyPluginInfoClass, activated),
                          NULL,
                          NULL,
                          g_cclosure_marshal_VOID__VOID,
                          G_TYPE_NONE,
                          0);
    signals[DEACTIVATED] =
            g_signal_new("deactivated",
                          G_TYPE_FROM_CLASS(object_class),
                          G_SIGNAL_RUN_LAST,
                          G_STRUCT_OFFSET(MyPluginInfoClass, deactivated),
                          NULL,
                          NULL,
                          g_cclosure_marshal_VOID__VOID,
                          G_TYPE_NONE,
                          0);

    g_type_class_add_private(klass, sizeof(MyPluginInfoPrivate));
}

static void my_plugin_info_init(MyPluginInfo *info)
{
    info->priv = MY_PLUGIN_INFO_GET_PRIVATE(info);
}

static void plugin_enabled_cb(GSettings *settings, gchar *key, MyPluginInfo *info)
{
    if (g_settings_get_boolean(info->priv->settings, key)) {
        my_plugin_info_activate(info);
    }
    else {
        my_plugin_info_deactivate(info);
    }
}

MyPluginInfo *my_plugin_info_from_file(const char *filename)
{
    MyPluginInfo *info;
    gboolean res;
    GKeyFile *plugin_file = NULL;
    char *str;

    res = FALSE;
    info = g_object_new(MY_TYPE_PLUGIN_INFO, NULL);

    // 从/usr/lib/x86_64-linux-gnu/my-plugin-daemon/diskmonitor.my-plugin文件中获取插件的相关信息
    info->priv->filename = g_strdup(filename);
    plugin_file = g_key_file_new();
    if (!g_key_file_load_from_file(plugin_file, filename, G_KEY_FILE_NONE, NULL)) {
        g_warning("Bad plugin file: %s", filename);
        goto out;
    }

    // Get Id
    str = g_key_file_get_string(plugin_file, PLUGIN_GROUP, "Id", NULL);
    if ((str != NULL) && (*str != '\0')) {
        info->priv->id = str;
    }
    else {
        g_free(str);
        g_warning("Could not find 'Module' in %s", filename);
        goto out;
    }

    //TODO: 未来将支持两类插件，一类是C编写的，一类是Python编写的，目前暂时固定为只支持C编写的插件
    info->priv->loader = MY_PLUGIN_LOADER_C;

    // Get Name
    str = g_key_file_get_locale_string(plugin_file, PLUGIN_GROUP, "Name", NULL, NULL);
    if (str != NULL) {
        info->priv->name = str;
    }
    else {
        g_warning("Could not find 'Name' in %s", filename);
        goto out;
    }

    // Get Description
    str = g_key_file_get_locale_string(plugin_file, PLUGIN_GROUP, "Description", NULL, NULL);
    if (str != NULL) {
        info->priv->desc = str;
    }
    else {
        g_debug("Could not find 'Description' in %s", filename);
    }

    // Get Authors
    info->priv->authors = g_key_file_get_string_list(plugin_file, PLUGIN_GROUP, "Authors", NULL, NULL);
    if (info->priv->authors == NULL) {
        g_debug("Could not find 'Authors' in %s", filename);
    }

    // Get Copyright
    str = g_key_file_get_string(plugin_file, PLUGIN_GROUP, "Copyright", NULL);
    if (str != NULL) {
        info->priv->copyright = str;
    }
    else {
        g_debug("Could not find 'Copyright' in %s", filename);
    }

    // Get Website
    str = g_key_file_get_string(plugin_file, PLUGIN_GROUP, "Website", NULL);
    if (str != NULL) {
        info->priv->website = str;
    }
    else {
        g_debug("Could not find 'Website' in %s", filename);
    }
    g_key_file_free(plugin_file);

    g_debug("MyPluginInfo: id='%s' name='%s' filename='%s'", info->priv->id, info->priv->name, info->priv->filename);

    // 当插件配置文件都读取正常后，表示该插件可以被激活
    info->priv->available = TRUE;

    res = TRUE;

out:
    if (!res) {
        g_object_unref(info);
        info = NULL;
    }

    return info;
}

static gboolean load_plugin_module(MyPluginInfo *info)
{
    char *path;
    char *dirname;
    gboolean ret;

    ret = FALSE;

    g_return_val_if_fail(MY_IS_PLUGIN_INFO(info), FALSE);
    g_return_val_if_fail(info->priv->filename != NULL, FALSE);
    g_return_val_if_fail(info->priv->id != NULL, FALSE);
    g_return_val_if_fail(info->priv->plugin == NULL, FALSE);
    g_return_val_if_fail(info->priv->available, FALSE);

    switch (info->priv->loader) {
        case MY_PLUGIN_LOADER_C:
            dirname = g_path_get_dirname(info->priv->filename);
            g_return_val_if_fail(dirname != NULL, FALSE);

            path = g_module_build_path(dirname, info->priv->id);
            g_free(dirname);
            g_return_val_if_fail(path != NULL, FALSE);

            info->priv->module = G_TYPE_MODULE(my_module_new(path));
            g_free(path);

            break;
        default:
            g_return_val_if_reached(FALSE);
    }

    if (!g_type_module_use(info->priv->module)) {
        switch (info->priv->loader) {
            case MY_PLUGIN_LOADER_C:
                g_warning("Cannot load plugin '%s' since file '%s' cannot be read.",
                           info->priv->name,
                           my_module_get_path(MY_MODULE(info->priv->module)));
                break;
            default:
                g_return_val_if_reached(FALSE);
        }

        g_object_unref(G_OBJECT(info->priv->module));
        info->priv->module = NULL;
        info->priv->available = FALSE;

        goto out;
    }

    switch (info->priv->loader) {
        case MY_PLUGIN_LOADER_C:
            info->priv->plugin = MY_PLUGIN(my_module_new_object(MY_MODULE(info->priv->module)));
            break;
        default:
            g_return_val_if_reached(FALSE);
    }

    g_type_module_unuse(info->priv->module);
    ret = TRUE;

out:
    return ret;
}

gboolean my_plugin_info_activate(MyPluginInfo *info)
{
    g_return_val_if_fail(MY_IS_PLUGIN_INFO(info), FALSE);

    gboolean res = TRUE;

    if (!info->priv->available) {
        return FALSE;
    }

    if (info->priv->active) {
        return TRUE;
    }

    if (info->priv->plugin == NULL) {
        res = load_plugin_module(info);
    }

    if (res) {
        my_plugin_activate(info->priv->plugin);
        g_signal_emit(info, signals[ACTIVATED], 0);
    }
    else {
        g_warning("Error activating plugin '%s'", info->priv->name);
    }

    if (res) {
        info->priv->active = TRUE;
        return TRUE;
    }

    return FALSE;
}

gboolean my_plugin_info_deactivate(MyPluginInfo *info)
{
    g_return_val_if_fail(MY_IS_PLUGIN_INFO(info), FALSE);

    if (!info->priv->active || !info->priv->available) {
        return TRUE;
    }

    my_plugin_deactivate(info->priv->plugin);
    g_signal_emit(info, signals[DEACTIVATED], 0);

    info->priv->active = FALSE;

    return TRUE;
}

gboolean my_plugin_info_is_active(MyPluginInfo *info)
{
    g_return_val_if_fail(MY_IS_PLUGIN_INFO(info), FALSE);

    return (info->priv->available && info->priv->active);
}

gboolean my_plugin_info_get_enabled(MyPluginInfo *info)
{
    g_return_val_if_fail(MY_IS_PLUGIN_INFO(info), FALSE);

    return (info->priv->enabled);
}

gboolean my_plugin_info_is_available(MyPluginInfo *info)
{
    g_return_val_if_fail(MY_IS_PLUGIN_INFO(info), FALSE);

    return (info->priv->available != FALSE);
}

const char *my_plugin_info_get_id(MyPluginInfo *info)
{
    g_return_val_if_fail(MY_IS_PLUGIN_INFO(info), NULL);

    return info->priv->id;
}

const char *my_plugin_info_get_name(MyPluginInfo *info)
{
    g_return_val_if_fail(MY_IS_PLUGIN_INFO(info), NULL);

    return info->priv->name;
}

const char *my_plugin_info_get_description(MyPluginInfo *info)
{
    g_return_val_if_fail(MY_IS_PLUGIN_INFO(info), NULL);

    return info->priv->desc;
}

const char **my_plugin_info_get_authors(MyPluginInfo *info)
{
    g_return_val_if_fail(MY_IS_PLUGIN_INFO(info), (const char **)NULL);

    return (const char **)info->priv->authors;
}

const char *my_plugin_info_get_website(MyPluginInfo *info)
{
    g_return_val_if_fail(MY_IS_PLUGIN_INFO(info), NULL);

    return info->priv->website;
}

const char *my_plugin_info_get_copyright(MyPluginInfo *info)
{
    g_return_val_if_fail(MY_IS_PLUGIN_INFO(info), NULL);

    return info->priv->copyright;
}

void my_plugin_info_from_schema(MyPluginInfo *info, gchar *schema)
{
    g_return_if_fail(MY_IS_PLUGIN_INFO(info));

    info->priv->settings = g_settings_new(schema);
    info->priv->enabled = g_settings_get_boolean(info->priv->settings, "active");

    g_signal_connect(G_OBJECT(info->priv->settings), "changed::active", G_CALLBACK(plugin_enabled_cb), info);
}
