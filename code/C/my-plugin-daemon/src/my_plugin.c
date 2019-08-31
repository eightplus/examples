#include "config.h"

#include "my_plugin.h"

G_DEFINE_TYPE (MyPlugin, my_plugin, G_TYPE_OBJECT)

static void dummy(MyPlugin *plugin)
{

}

static void my_plugin_class_init(MyPluginClass *klass)
{
    klass->activate = dummy;
    klass->deactivate = dummy;
}

static void my_plugin_init(MyPlugin *plugin)
{

}

void my_plugin_activate(MyPlugin *plugin)
{
    g_return_if_fail(MY_IS_PLUGIN(plugin));

    MY_PLUGIN_GET_CLASS(plugin)->activate(plugin);
}

void my_plugin_deactivate(MyPlugin *plugin)
{
    g_return_if_fail(MY_IS_PLUGIN(plugin));

    MY_PLUGIN_GET_CLASS(plugin)->deactivate(plugin);
}
