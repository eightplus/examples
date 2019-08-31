#ifndef __MY_DISKMONITOR_PLUGIN_H__
#define __MY_DISKMONITOR_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <gmodule.h>

#include "my_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MY_TYPE_DISKMONITOR_PLUGIN                (my_diskmonitor_plugin_get_type ())
#define MY_DISKMONITOR_PLUGIN(o)                  (G_TYPE_CHECK_INSTANCE_CAST ((o), MY_TYPE_DISKMONITOR_PLUGIN, MyDiskmonitorPlugin))
#define MY_DISKMONITOR_PLUGIN_CLASS(k)            (G_TYPE_CHECK_CLASS_CAST((k), MY_TYPE_DISKMONITOR_PLUGIN, MyDiskmonitorPluginClass))
#define MY_IS_DISKMONITOR_PLUGIN(o)               (G_TYPE_CHECK_INSTANCE_TYPE ((o), MY_TYPE_DISKMONITOR_PLUGIN))
#define MY_IS_DISKMONITOR_PLUGIN_CLASS(k)         (G_TYPE_CHECK_CLASS_TYPE ((k), MY_TYPE_DISKMONITOR_PLUGIN))
#define MY_DISKMONITOR_PLUGIN_GET_CLASS(o)        (G_TYPE_INSTANCE_GET_CLASS ((o), MY_TYPE_DISKMONITOR_PLUGIN, MyDiskmonitorPluginClass))

typedef struct {
    MyPlugin parent;
} MyDiskmonitorPlugin;

typedef struct {
    MyPluginClass parent_class;
} MyDiskmonitorPluginClass;

GType  my_diskmonitor_plugin_get_type(void) G_GNUC_CONST;

/* All the plugins must implement this function */
G_MODULE_EXPORT GType register_my_plugin(GTypeModule *module);

#ifdef __cplusplus
}
#endif

#endif /* __MY_DISKMONITOR_PLUGIN_H__ */
