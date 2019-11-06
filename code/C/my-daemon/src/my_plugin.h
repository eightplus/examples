#ifndef __MY_PLUGIN_H__
#define __MY_PLUGIN_H__

#include <glib-object.h>
#include <gmodule.h>

#ifdef __cplusplus
extern "C" {
#endif
#define MY_TYPE_PLUGIN              (my_plugin_get_type())
#define MY_PLUGIN(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), MY_TYPE_PLUGIN, MyPlugin))
#define MY_PLUGIN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass),  MY_TYPE_PLUGIN, MyPluginClass))
#define MY_IS_PLUGIN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), MY_TYPE_PLUGIN))
#define MY_IS_PLUGIN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MY_TYPE_PLUGIN))
#define MY_PLUGIN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj),  MY_TYPE_PLUGIN, MyPluginClass))

typedef struct
{
    GObject parent;
} MyPlugin;

typedef struct
{
    GObjectClass parent_class;

    /* Virtual public methods */
    void (*activate) (MyPlugin *plugin);
    void (*deactivate) (MyPlugin *plugin);
} MyPluginClass;

GType my_plugin_get_type(void) G_GNUC_CONST;

void my_plugin_activate(MyPlugin *plugin);
void my_plugin_deactivate(MyPlugin *plugin);

/*
 * Utility macro used to register plugins
 *
 * use: MY_PLUGIN_REGISTER (PluginName, plugin_name)
 */
#define MY_PLUGIN_REGISTER(PluginName, plugin_name)                 \
        G_DEFINE_DYNAMIC_TYPE (PluginName,                                     \
                               plugin_name,                                    \
                               MY_TYPE_PLUGIN)                      \
                                                                               \
G_MODULE_EXPORT GType                                                          \
register_my_plugin(GTypeModule *type_module)                       \
{                                                                              \
    plugin_name##_register_type(type_module);                             \
                                                                           \
    return plugin_name##_get_type();                                       \
}

#ifdef __cplusplus
}
#endif

#endif  /* __MY_PLUGIN_H__ */
