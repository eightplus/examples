#include "config.h"

#include <glib/gi18n-lib.h>
#include <gmodule.h>
#include <stdio.h>

#include "my_diskmonitor_plugin.h"
#include "my_diskmonitor_space.h"

MY_PLUGIN_REGISTER (MyDiskmonitorPlugin, my_diskmonitor_plugin)

static void my_diskmonitor_plugin_init(MyDiskmonitorPlugin *plugin)
{
    g_debug("MyDiskmonitorPlugin initializing");
}

static void my_diskmonitor_plugin_finalize(GObject *object)
{
    MyDiskmonitorPlugin *plugin;

    g_return_if_fail(object != NULL);
    g_return_if_fail(MY_IS_DISKMONITOR_PLUGIN(object));

    g_debug("MyDiskmonitorPlugin finalizing");

    plugin = MY_DISKMONITOR_PLUGIN(object);

    G_OBJECT_CLASS(my_diskmonitor_plugin_parent_class)->finalize(object);
}

static void my_diskmonitor_plugin_activate(MyPlugin *plugin)
{
    g_debug("Activating diskmonitor plugin");

    start_disk_monitor(FALSE);//开始监控磁盘的/分区
}

static void my_diskmonitor_plugin_deactivate(MyPlugin *plugin)
{
    g_debug("Deactivating diskmonitor plugin");
    stop_disk_monitor();//停止监控磁盘的/分区
}

static void my_diskmonitor_plugin_class_init(MyDiskmonitorPluginClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    MyPluginClass *plugin_class = MY_PLUGIN_CLASS(klass);

    object_class->finalize = my_diskmonitor_plugin_finalize;
    plugin_class->activate = my_diskmonitor_plugin_activate;
    plugin_class->deactivate = my_diskmonitor_plugin_deactivate;
}

static void my_diskmonitor_plugin_class_finalize(MyDiskmonitorPluginClass *klass)
{

}
