#include "config.h"

#include "my_module.h"

#include <gmodule.h>

typedef struct _MyModuleClass MyModuleClass;

struct _MyModuleClass
{
    GTypeModuleClass parent_class;
};

struct _MyModule
{
    GTypeModule parent_instance;

    GModule    *library;

    char       *path;
    GType       type;
};

typedef GType (*MyModuleRegisterFunc) (GTypeModule *);

G_DEFINE_TYPE (MyModule, my_module, G_TYPE_TYPE_MODULE)

static gboolean my_module_load(GTypeModule *gmodule)
{
    MyModule *module;
    MyModuleRegisterFunc register_func;
    gboolean res;

    module = MY_MODULE(gmodule);

    g_debug("Loading %s", module->path);

    module->library = g_module_open(module->path, 0);

    if (module->library == NULL) {
        g_warning ("%s", g_module_error());
        return FALSE;
    }

    /* extract symbols from the lib */
    res = g_module_symbol(module->library, "register_my_plugin", (void *)&register_func);
    if (!res) {
        g_warning("%s", g_module_error());
        g_module_close(module->library);

        return FALSE;
    }

    g_assert(register_func);

    module->type = register_func(gmodule);

    if (module->type == 0) {
        g_warning("Invalid my plugin in module %s", module->path);
        return FALSE;
    }

    return TRUE;
}

static void my_module_unload (GTypeModule *gmodule)
{
    MyModule *module = MY_MODULE(gmodule);

    g_debug("Unloading %s", module->path);

    g_module_close(module->library);

    module->library = NULL;
    module->type = 0;
}

const gchar *my_module_get_path(MyModule *module)
{
    g_return_val_if_fail(MY_IS_MODULE(module), NULL);

    return module->path;
}

GObject *my_module_new_object(MyModule *module)
{
    g_debug("Creating object of type %s", g_type_name(module->type));

    if (module->type == 0) {
        return NULL;
    }

    return g_object_new(module->type, NULL);
}

static void my_module_init(MyModule *module)
{
    g_debug("MyModule %p initialising", module);
}

static void my_module_finalize(GObject *object)
{
    MyModule *module = MY_MODULE(object);

    g_debug("MyModule %p finalizing", module);

    g_free(module->path);

    G_OBJECT_CLASS(my_module_parent_class)->finalize(object);
}

static void my_module_class_init(MyModuleClass *class)
{
    GObjectClass *object_class = G_OBJECT_CLASS(class);
    GTypeModuleClass *module_class = G_TYPE_MODULE_CLASS(class);

    object_class->finalize = my_module_finalize;

    module_class->load = my_module_load;
    module_class->unload = my_module_unload;
}

MyModule *my_module_new(const char *path)
{
    MyModule *result;

    if (path == NULL || path[0] == '\0') {
        return NULL;
    }

    result = g_object_new(MY_TYPE_MODULE, NULL);

    g_type_module_set_name(G_TYPE_MODULE(result), path);
    result->path = g_strdup(path);

    return result;
}
