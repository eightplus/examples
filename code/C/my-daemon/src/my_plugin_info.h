#ifndef __MY_PLUGIN_INFO_H__
#define __MY_PLUGIN_INFO_H__

#include <glib-object.h>
#include <gmodule.h>

#ifdef __cplusplus
extern "C" {
#endif
#define MY_TYPE_PLUGIN_INFO              (my_plugin_info_get_type())
#define MY_PLUGIN_INFO(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), MY_TYPE_PLUGIN_INFO, MyPluginInfo))
#define MY_PLUGIN_INFO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass),  MY_TYPE_PLUGIN_INFO, MyPluginInfoClass))
#define MY_IS_PLUGIN_INFO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), MY_TYPE_PLUGIN_INFO))
#define MY_IS_PLUGIN_INFO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MY_TYPE_PLUGIN_INFO))
#define MY_PLUGIN_INFO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj),  MY_TYPE_PLUGIN_INFO, MyPluginInfoClass))

typedef struct MyPluginInfoPrivate MyPluginInfoPrivate;

typedef struct
{
    GObject parent;
    MyPluginInfoPrivate *priv;
} MyPluginInfo;

typedef struct
{
    GObjectClass parent_class;

    void (* activated) (MyPluginInfo *info);
    void (* deactivated) (MyPluginInfo *info);
} MyPluginInfoClass;

GType my_plugin_info_get_type (void) G_GNUC_CONST;

MyPluginInfo *my_plugin_info_from_file(const char *filename);

gboolean my_plugin_info_activate(MyPluginInfo *info);
gboolean my_plugin_info_deactivate(MyPluginInfo *info);

gboolean my_plugin_info_is_active(MyPluginInfo *info);
gboolean my_plugin_info_get_enabled(MyPluginInfo *info);
gboolean my_plugin_info_is_available(MyPluginInfo *info);

const char *my_plugin_info_get_id(MyPluginInfo *info);
const char *my_plugin_info_get_name(MyPluginInfo *info);
const char *my_plugin_info_get_description(MyPluginInfo *info);
const char **my_plugin_info_get_authors(MyPluginInfo *info);
const char *my_plugin_info_get_website(MyPluginInfo *info);
const char *my_plugin_info_get_copyright(MyPluginInfo *info);

void my_plugin_info_from_schema(MyPluginInfo *info, gchar *schema);

#ifdef __cplusplus
}
#endif

#endif  /* __MY_PLUGIN_INFO_H__ */
